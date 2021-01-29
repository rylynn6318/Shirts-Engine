cmake_minimum_required(VERSION 3.1)
project(Shirts_Engine)

# TODO : 엔진 빌드한 다음 그걸 링크 하게

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Source files
set(INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

set(TEST_TARGET "${PROJECT_NAME}_test")
set(TEST_INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/test/include")
set(TEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/test/src")
set(TEST_SOURCE "${TEST_SOURCE_DIR}/main.cpp")

# Test target
add_executable(${TEST_TARGET} ${TEST_SOURCE})
set_property(TARGET ${TEST_TARGET} PROPERTY CXX_STANDARD 20)
target_include_directories(${TEST_TARGET} PRIVATE "${SRC_DIR}")
target_link_libraries(${TEST_TARGET} ${CONAN_LIBS})