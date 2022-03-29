# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.16.3)

project("GlibNative" LANGUAGES C)

set(GST_ROOT ../Thirdparty/gst-android)
set(ABI_SUFFIX ${ANDROID_ABI})
set(NATIVE_BASE "../ios/Native")

IF(${ANDROID_ABI}  STREQUAL "armeabi-v7a")
    set(ABI_SUFFIX armv7)
ELSEIF(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ABI_SUFFIX arm64)
ELSEIF(${ANDROID_ABI} STREQUAL "x86")
#    set(ABI_SUFFIX x86)
ELSEIF(${ANDROID_ABI} STREQUAL "x86_64")
#    set(ABI_SUFFIX x86_64)
ENDIF()

set(GSTREAMER_ROOT ${GST_ROOT}/${ABI_SUFFIX})

include_directories(
        ${GSTREAMER_ROOT}/include/glib-2.0
        ${GSTREAMER_ROOT}/lib/glib-2.0/include)

link_directories(
        ${GSTREAMER_ROOT}/lib)

add_subdirectory(${NATIVE_BASE} ${CMAKE_SOURCE_DIR}/native-output)


