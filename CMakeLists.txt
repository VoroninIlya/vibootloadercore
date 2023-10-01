cmake_minimum_required(VERSION 3.22)

project(tst_vibootloadercore)

#include(CodeCoverage)
include(CTest)

if(PROJECT_IS_TOP_LEVEL)
# Set as executable
add_subdirectory(${UNITY_ROOT_PATH} ${${EXECUTABLE}_BINARY_DIR}/modules/unity)
endif()

# Add core subdir
add_subdirectory(core)

add_executable(tst_vibootloadercore)
enable_testing()

# Add key executable block
target_sources(tst_vibootloadercore PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tests/main/main.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/tst_vibootloadercore.c
)

# Add key include paths
target_include_directories(tst_vibootloadercore PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/core/str/inc
    ${UNITY_ROOT_PATH}/src
    ${UNITY_ROOT_PATH}/extras/fixture/src
    ${UNITY_ROOT_PATH}/extras/memory/src
)

# Compilation definition information
target_compile_definitions(tst_vibootloadercore PUBLIC
    WIN32
    _DEBUG
    CONSOLE
)

# Compiler options
target_compile_options(tst_vibootloadercore PRIVATE
    -g
    --coverage
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
  tst_vibootloadercore vibootloadercore unity -g -coverage -lgcov)

add_test(NAME VIBCORE_SomeFunc COMMAND tst_vibootloadercore "--gtest_filter=VIBCORE_SomeFunc.*")
