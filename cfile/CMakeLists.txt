cmake_minimum_required(VERSION 3.5)

project(fltgst VERSION 0.0.1
LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GST_LDFLAGS})