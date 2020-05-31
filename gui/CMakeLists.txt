cmake_minimum_required(VERSION 3.0)
project(EasyPhotoshopGUI C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
message(STATUS "GTK FOUND:")
message(STATUS "  GTK include directories:   ${GTK_INCLUDE_DIRS}")
message(STATUS "  GTK library directories:   ${GTK_LIBRARY_DIRS}")
message(STATUS "  GTK libraries:             ${GTK_LIBRARIES}")

include_directories(${CMAKE_SOURCE_DIR})

include_directories(${GTK_INCLUDE_DIRS})

add_library(EasyPhotoshopGUI main.c image-widget.c callbacks.c callbacks.h)
target_link_libraries(EasyPhotoshopGUI EasyPhotoshopCore EasyPhotoshopImageProcess ${GTK_LIBRARIES})
