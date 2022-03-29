find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

SET(DART_API_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/dart_api)

add_library(glib_native SHARED
native_register.c
${DART_API_ROOT}/dart_api_dl.c
${DART_API_ROOT}/dart_api_dl.h
${DART_API_ROOT}/dart_api.h
${DART_API_ROOT}/dart_native_api.h
${DART_API_ROOT}/dart_tools_api.h
${DART_API_ROOT}/dart_version.h
${DART_API_ROOT}/internal/dart_api_dl_impl.h
)

target_link_libraries(glib_native 
${GLIB_LDFLAGS}
intl
iconv
log
)