# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)

set(LOADSO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/loadso/)
add_subdirectory(${LOADSO_DIR} ${PROJECT_BINARY_DIR}/loadso.out)

set(STREAM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stream/)
add_subdirectory(${STREAM_DIR} ${PROJECT_BINARY_DIR}/stream.out)

set(THREAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/threads/)
add_subdirectory(${THREAD_DIR} ${PROJECT_BINARY_DIR}/thread.out)

set(VERIFY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/verify/)
add_subdirectory(${VERIFY_DIR} ${PROJECT_BINARY_DIR}/verify.out)

