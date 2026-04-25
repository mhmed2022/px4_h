#pragma once
/**
 * gps_usb_ubx.h — USB GPS UBX Driver
 * يقرأ بروتوكول UBX الثنائي من u-blox عبر USB (libusb)
 * وينشر sensor_gps مباشرة على uORB — نفس نمط servo_usb_output.h
 */

// تشغيل/إيقاف thread القراءة (يُستدعى من px4_jni.cpp)
void gps_usb_ubx_start();
void gps_usb_ubx_stop();

// يُستدعى من JNI عند توصيل/فصل USB GPS
// fd >= 0 = USB متصل، fd < 0 = USB مفصول
void gps_usb_set_fd(int fd);

// حالة GPS للواجهة
int gps_usb_get_status();      // 0=disconnected, 1=connected, 2=configured, 3=receiving
long gps_usb_get_rx_count();   // عدد الحزم المستقبلة
int gps_usb_get_fd();          // fd الحالي
const char* gps_usb_get_diag_msg(); // رسالة تشخيصية
