cmake_minimum_required(VERSION 3.26)
project(cokol VERSION 0.1 LANGUAGES C)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

set(SOURCES_DIR ../src)
set(SOURCES_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(${SOURCES_DIR} ${SOURCES_BINARY_DIR})
