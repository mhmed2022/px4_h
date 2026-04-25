/* Android phone build - stub apps.h */
#pragma once

#include <px4_platform_common/tasks.h>
#include <map>
#include <string>

typedef std::map<std::string, px4_main_t> apps_map_type;

__EXPORT void init_app_map(apps_map_type &apps);
__EXPORT void list_builtins(apps_map_type &apps);
