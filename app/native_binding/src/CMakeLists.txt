# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(native_binding_library VERSION 0.0.1 LANGUAGES C)


find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})

add_library(native_binding SHARED
  "native_binding.c"
)

target_link_libraries(native_binding PRIVATE ${GST_LDFLAGS})

set_target_properties(native_binding PROPERTIES
  PUBLIC_HEADER native_binding.h
  OUTPUT_NAME "native_binding"
)

target_compile_definitions(native_binding PUBLIC DART_SHARED_LIB)