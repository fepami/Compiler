project(Compiler)

cmake_minimum_required(VERSION 2.8.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/source/inc)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/source/tests)

add_subdirectory(source)

find_package(CMocka)
if (CMOCKA_FOUND)
    enable_testing()
    add_subdirectory(${TESTS_DIR})
endif (CMOCKA_FOUND)