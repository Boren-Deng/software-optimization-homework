cmake_minimum_required(VERSION 2.8)
project(MatrixMultiplication)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -std=c++11 -pthread -msse2 -mavx -mavx2")
set(CMAKE_BUILD_TYPE "Release")

aux_source_directory(. CUR_DIR_SRCS)

IF("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    message(STATUS "building for: debugging")

    set(CXX_FORMAT_SIGNEDNESS "-Wformat-signedness")
    set(CXX_FORMAT_SECURITY "-Werror=format-security")
    set(CXX_STACK_PROTECTOR "-fstack-protector-all")
    set(CXX_FLAGS_DEBUG "-O0")
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -ggdb -Wall -Wextra -DNETDATA_INTERNAL_CHECKS=1 -DNETDATA_VERIFY_LOCKS=1 ${CXX_FORMAT_SIGNEDNESS} ${CXX_FORMAT_SECURITY} ${CXX_STACK_PROTECTOR} ${CXX_FLAGS_DEBUG}")
ELSE()
    message(STATUS "building for: release")
ENDIF()

add_executable(Matrix_mul ${CUR_DIR_SRCS})

target_include_directories(Matrix_mul PRIVATE
        ${PROJECT_SOURCE_DIR})