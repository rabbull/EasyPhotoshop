cmake_minimum_required(VERSION 3.0)
project(EasyPhotoshopImageProcess C)

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
pkg_check_modules(BLAS REQUIRED openblas)
message(STATUS "BLAS Found:")
message(STATUS "  BLAS include directories:   ${BLAS_INCLUDE_DIRS}")
message(STATUS "  BLAS library directories:   ${BLAS_LIBRARY_DIRS}")
message(STATUS "  BLAS libraries:             ${BLAS_LIBRARIES}")

include_directories(${CMAKE_SOURCE_DIR})

include_directories(${GLib_INCLUDE_DIRS} ${GObj_INCLUDE_DIRS} ${BLAS_INCLUDE_DIRS})
link_directories(${GLib_LIBRARY_DIRS} ${GObj_LIBRARY_DIRS} ${BLAS_LIBRARY_DIRS})

add_library(EasyPhotoshopImageProcess histogram-equalization.c histogram-equalization.h color-convert.c)
target_link_libraries(EasyPhotoshopImageProcess ${GLib_LIBRARIES} ${GObj_LIBRARIES} ${BLAS_LIBRARIES})
