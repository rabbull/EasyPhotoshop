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

message(STATUS "Importing file io module..")
add_subdirectory(fileio)

message(STATUS "Configuring main program..")

include_directories(${CMAKE_SOURCE_DIR})

add_executable(EasyPhotoshop main.c imgproc/uniform-quantization.c imgproc/uniform-quantization.h)
target_link_libraries(EasyPhotoshop EasyPhotoshopCore EasyPhotoshopImageProcess EasyPhotoshopGUI EasyPhotoshopFileIO)
