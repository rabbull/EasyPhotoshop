# NOTE: works for both latest ArchLinux and MSYS2/MinGW-w64

cmake_minimum_required(VERSION 3.0)
project(EasyPhotoshop C)

set(CMAKE_C_STANDARD 11)

message(STATUS "Importing core module..")
add_subdirectory(core)

message(STATUS "Importing imgproc module..")
add_subdirectory(imgproc)

message(STATUS "Importing gui module..")
add_subdirectory(gui)

message(STATUS "Configuring main program..")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
message(STATUS "GTK Found:")
message(STATUS "  GTK include directories:   ${GTK_INCLUDE_DIRS}")
message(STATUS "  GTK library directories:   ${GTK_LIBRARY_DIRS}")
message(STATUS "  GTK libraries:             ${GTK_LIBRARIES}")
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

add_executable(EasyPhotoshop main.c)
target_link_libraries(EasyPhotoshop EasyPhotoshopCore EasyPhotoshopImageProcess EasyPhotoshopGUI ${GTK_LIBRARIES} ${GLib_LIBRARIES} ${GObj_LIBRARIES})
