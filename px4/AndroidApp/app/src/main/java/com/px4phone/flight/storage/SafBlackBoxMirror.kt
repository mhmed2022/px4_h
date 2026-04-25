package com.ardophone.px4v17.storage

import android.content.Context
import android.net.Uri
import android.os.Build
import android.util.Log
import androidx.documentfile.provider.DocumentFile
import java.io.File
import java.io.RandomAccessFile
import java.util.concurrent.Executors
import java.util.concurrent.ScheduledFuture
import java.util.concurrent.TimeUnit
import java.util.concurrent.atomic.AtomicInteger

/**
 * مزامنة مجلد PX4 (logs, eeprom) → فلاش OTG عبر SAF.
 *
 * التحسين: نسخ تزايدي (incremental append) — يحفظ آخر offset لكل ملف
 * وينسخ فقط البايتات الجديدة. هذا يجعل المزامنة سريعة دائماً
 * بغض النظر عن حجم ملف اللوق.
 */
object SafBlackBoxMirror {

    private const val TAG = "SafMirror"
    private const val ROOT_FOLDER = "PX4BlackBox"
    private const val BUFFER_SIZE = 64 * 1024 // 64KB buffer

    private val executor = Executors.newSingleThreadScheduledExecutor { r ->
        Thread(r, "blackbox-saf-mirror").apply { isDaemon = true }
    }

    private var scheduled: ScheduledFuture<*>? = null

    // Track last synced offset per file (for incremental append)
    private val lastOffset = HashMap<String, Long>()
    // Track DocumentFile URIs to avoid repeated findFile() calls
    private val uriCache = HashMap<String, Uri>()

    fun isScheduled(): Boolean = scheduled != null && !scheduled!!.isDone

    /** جدولة مزامنة كل ~1 ثانية. */
    fun schedule(context: Context, sourceRoot: File, treeUri: Uri) {
        cancelSchedule()
        lastOffset.clear()
        uriCache.clear()
        val app = context.applicationContext
        scheduled = executor.scheduleWithFixedDelay({
            try {
                runSyncPass(app, sourceRoot, treeUri)
            } catch (e: Exception) {
                Log.e(TAG, "sync error", e)
            }
        }, 2, 1, TimeUnit.SECONDS) // 2s initial delay for PX4 to create first files
        Log.i(TAG, "Mirror scheduled: ${sourceRoot.path} → USB")
    }

    fun cancelSchedule() {
        try {
            scheduled?.cancel(false)
        } catch (_: Exception) { }
        scheduled = null
    }

    /** مزامنة نهائية عند الإيقاف — تضمن عدم فقدان آخر البيانات. */
    fun syncOnce(context: Context, sourceRoot: File, treeUri: Uri) {
        val app = context.applicationContext
        val done = AtomicInteger(0)
        executor.execute {
            try {
                runSyncPass(app, sourceRoot, treeUri)
            } finally {
                done.incrementAndGet()
            }
        }
        var wait = 0
        while (done.get() == 0 && wait < 50) {
            try { Thread.sleep(20) } catch (_: InterruptedException) { break }
            wait++
        }
    }

    private fun runSyncPass(context: Context, sourceRoot: File, treeUri: Uri) {
        if (!sourceRoot.isDirectory) return
        val tree = DocumentFile.fromTreeUri(context, treeUri) ?: return
        val destFolder = tree.findFile(ROOT_FOLDER) ?: tree.createDirectory(ROOT_FOLDER) ?: return

        // Only sync .ulg files, directly into PX4BlackBox/ (no subdirectories)
        sourceRoot.walkTopDown()
            .filter { it.isFile && it.extension == "ulg" }
            .forEach { file ->
                try {
                    syncFile(context, file, destFolder, file.name)
                } catch (e: Exception) {
                    Log.w(TAG, "skip ${file.name}: ${e.message}")
                }
            }

        // Backup calibration params (small ~4KB — always overwrite with latest)
        backupParams(context, sourceRoot, destFolder)
    }

    /**
     * Backup eeprom/parameters to SAF tree so calibration survives app reinstall.
     * Always overwrites with the current file (it's small, no need for incremental).
     */
    private fun backupParams(context: Context, sourceRoot: File, destFolder: DocumentFile) {
        val paramsFile = File(sourceRoot, "eeprom/parameters")
        if (!paramsFile.exists() || paramsFile.length() == 0L) return
        try {
            val eeFolder = destFolder.findFile("eeprom")
                ?: destFolder.createDirectory("eeprom")
                ?: return
            val dest = eeFolder.findFile("parameters")
                ?: eeFolder.createFile("application/octet-stream", "parameters")
                ?: return
            context.contentResolver.openOutputStream(dest.uri, "wt")?.use { out ->
                paramsFile.inputStream().use { it.copyTo(out) }
                out.flush()
            }
            Log.d(TAG, "Params backed up to SAF (${paramsFile.length()} bytes)")
        } catch (e: Exception) {
            Log.w(TAG, "params backup failed: ${e.message}")
        }
    }

    /**
     * Called at startup: if local eeprom/parameters is missing but a SAF backup exists,
     * restore it. Returns true if restore was performed.
     */
    fun restoreParamsIfMissing(context: Context, sourceRoot: File, treeUri: Uri): Boolean {
        val paramsFile = File(sourceRoot, "eeprom/parameters")
        if (paramsFile.exists() && paramsFile.length() > 0L) return false

        val tree = DocumentFile.fromTreeUri(context, treeUri) ?: return false
        val rootFolder = tree.findFile(ROOT_FOLDER) ?: return false
        val eeFolder = rootFolder.findFile("eeprom") ?: return false
        val backup = eeFolder.findFile("parameters") ?: return false
        if ((backup.length() ?: 0L) == 0L) return false

        return try {
            paramsFile.parentFile?.mkdirs()
            context.contentResolver.openInputStream(backup.uri)?.use { inp ->
                paramsFile.outputStream().use { inp.copyTo(it) }
            }
            Log.i(TAG, "Calibration params restored from SAF backup (${paramsFile.length()} bytes)")
            true
        } catch (e: Exception) {
            Log.e(TAG, "params restore failed: ${e.message}")
            false
        }
    }

    private fun syncFile(context: Context, file: File, destFolder: DocumentFile, fileName: String) {
        val currentSize = file.length()
        val prevOffset = lastOffset[fileName] ?: 0L

        // Nothing new to sync
        if (currentSize <= prevOffset) return

        // Get or create destination file directly in destFolder
        val targetUri = uriCache[fileName] ?: run {
            val existing = destFolder.findFile(fileName)
            val doc = if (existing != null && existing.isFile) {
                existing
            } else {
                destFolder.createFile("application/octet-stream", fileName) ?: return
            }
            uriCache[fileName] = doc.uri
            doc.uri
        }

        if (prevOffset == 0L) {
            // First sync — copy entire file
            RandomAccessFile(file, "r").use { raf ->
                context.contentResolver.openOutputStream(targetUri, "wt")?.use { out ->
                    val buf = ByteArray(BUFFER_SIZE)
                    var read: Int
                    while (raf.read(buf).also { read = it } > 0) {
                        out.write(buf, 0, read)
                    }
                    out.flush()
                }
            }
        } else {
            // Incremental — append only new bytes
            RandomAccessFile(file, "r").use { raf ->
                raf.seek(prevOffset)
                context.contentResolver.openOutputStream(targetUri, "wa")?.use { out ->
                    val buf = ByteArray(BUFFER_SIZE)
                    var read: Int
                    while (raf.read(buf).also { read = it } > 0) {
                        out.write(buf, 0, read)
                    }
                    out.flush()
                }
            }
        }

        // fsync to ensure data hits the flash
        try {
            context.contentResolver.openFileDescriptor(targetUri, "rw")?.use { pfd ->
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    pfd.fileDescriptor.sync()
                }
            }
        } catch (_: Exception) { }

        lastOffset[fileName] = currentSize
    }
}
