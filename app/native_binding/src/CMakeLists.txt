# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(native_binding_library VERSION 0.0.1 LANGUAGES C)


find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED gstreamer-1.0)

IF(WIN32)
  find_program(CMAKE_PKGCONFIG_EXECUTABLE pkg-config)
  IF(CMAKE_PKGCONFIG_EXECUTABLE)
  # pkg-config.exe gstreamer-1.0 --libs --msvc-syntax
    EXEC_PROGRAM(${CMAKE_PKGCONFIG_EXECUTABLE}
    ARGS " --libs --msvc-syntax gstreamer-1.0"
    OUTPUT_VARIABLE GST_LDFLAGS)
    # replace spaces with semicolons so that we don't have quotation marks in command line option
    string(REPLACE " " ";" GST_LDFLAGS ${GST_LDFLAGS})
    message("GST_LDFLAGS: ${GST_LDFLAGS}")
  ENDIF()
ENDIF()

IF(ANDROID OR __ANDROID__)
  SET(GST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gst-android)
  SET(ABI_SUFFIX ${ANDROID_ABI})

  IF(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    SET(ABI_SUFFIX armv7)
  ELSEIF(${ANDROID_ABI} STREQUAL "arm64-v8a")
    SET(ABI_SUFFIX arm64)
  ELSEIF(${ANDROID_ABI} STREQUAL "x86")
    # skipped
  ELSEIF(${ANDROID_ABI} STREQUAL "x86_64")
    # skipped
  ENDIF()

  SET(GST_ROOT ${GST_FOLDER}/${ABI_SUFFIX})

  # -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
  include_directories(
    ${GST_ROOT}/include/gstreamer-1.0
    ${GST_ROOT}/include/glib-2.0
    ${GST_ROOT}/lib/glib-2.0/include
  )

  link_directories(
    ${GST_ROOT}/lib
    ${GST_ROOT}/lib/gstreamer-1.0
  )

  SET(PLUGINS_DECLARATION)
  SET(PLUGINS_REGISTRATION)

  LIST(APPEND GST_PLUGINS coreelements coretracers adder app audioconvert audiorate audiotestsrc gio autodetect opensles)
  foreach(GST_P ${GST_PLUGINS})
    LIST(APPEND LINK_LIBS "gst${GST_P}")
    LIST(APPEND PLUGINS_DECLARATION "\nGST_PLUGIN_STATIC_DECLARE(${GST_P})")
    LIST(APPEND PLUGINS_REGISTRATION "\nGST_PLUGIN_STATIC_REGISTER(${GST_P})")
  endforeach()
  
  configure_file(gst_android.c.in ${CMAKE_CURRENT_SOURCE_DIR}/gst_plugin_init_android.c)

  LIST(APPEND APPENDED_SOURCE gst_plugin_init_android.c)

  LIST(APPEND LINK_LIBS intl ffi iconv gmodule-2.0 pcre2-8 gstbase-1.0 gstaudio-1.0 orc-0.4 gstapp-1.0 gio-2.0 android log z OpenSLES)

ELSE()
  include_directories(${GST_INCLUDE_DIRS})
ENDIF()

add_library(native_binding SHARED
  "native_binding.c"
  ${APPENDED_SOURCE}
)

IF(WIN32)
  target_link_options(native_binding PRIVATE ${GST_LDFLAGS})
ELSE()
  target_link_libraries(native_binding PRIVATE ${GST_LDFLAGS} ${LINK_LIBS})
ENDIF()

set_target_properties(native_binding PROPERTIES
  PUBLIC_HEADER native_binding.h
  OUTPUT_NAME "native_binding"
)

target_compile_definitions(native_binding PUBLIC DART_SHARED_LIB)
