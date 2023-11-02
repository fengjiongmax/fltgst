# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(native_binding_library VERSION 0.0.1 LANGUAGES C)


find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})

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

add_library(native_binding SHARED
  "native_binding.c"
)

IF(WIN32)
  target_link_options(native_binding PRIVATE ${GST_LDFLAGS})
ELSE()
  target_link_libraries(native_binding PRIVATE ${GST_LDFLAGS})
ENDIF()

set_target_properties(native_binding PROPERTIES
  PUBLIC_HEADER native_binding.h
  OUTPUT_NAME "native_binding"
)

target_compile_definitions(native_binding PUBLIC DART_SHARED_LIB)
