cmake_minimum_required(VERSION 3.22)

project(vibootloadercore)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register core library
add_library(vibootloadercore INTERFACE)
target_sources(vibootloadercore PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/vibootloadercore.c)
target_include_directories(vibootloadercore INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/inc)

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")