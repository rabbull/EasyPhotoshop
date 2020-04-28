cmake_minimum_required(VERSION 3.0)
project(EasyPhotoshopCore C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLib REQUIRED glib-2.0)
message(STATUS "GLib Found:")
message(STATUS "  GLib include directories:   ${GLib_INCLUDE_DIRS}")
message(STATUS "  GLib library directories:   ${GLib_LIBRARY_DIRS}")
message(STATUS "  GLib libraries:             ${GLib_LIBRARIES}")
pkg_check_modules(GObj REQUIRED gobject-2.0)
message(STATUS "GObject Found:")
message(STATUS "  GObj include directories:   ${GObj_INCLUDE_DIRS}")
message(STATUS "  GObj library directories:   ${GObj_LIBRARY_DIRS}")
message(STATUS "  GObj libraries:             ${GObj_LIBRARIES}")

include_directories(${GLib_INCLUDE_DIRS} ${GObj_INCLUDE_DIRS})
link_directories(${GLib_LIBRARY_DIRS} ${GObj_LIBRARY_DIRS})

add_library(EasyPhotoshopCore SHARED image.c size.c)
target_link_libraries(EasyPhotoshopCore ${GLib_LIBRARIES} ${GObj_LIBRARIES})
