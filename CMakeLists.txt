# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)
include(mimer.cmake)

# libuv
set(UV_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
add_subdirectory(${UV_SRC_DIR} ${PROJECT_BINARY_DIR}/3rdparty.out)

# uvbase
set(UVBASE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/connecter/uvbase)
add_subdirectory(${UVBASE_SRC_DIR} ${PROJECT_BINARY_DIR}/uvbase.out)

# transmitter
set(TRANSMITTER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/connecter/transmitter)
add_subdirectory(${TRANSMITTER_SRC_DIR} ${PROJECT_BINARY_DIR}/transmitter.out)

# mmloop_test
set(MMLOOP_TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/connecter/mmloop_test)
add_subdirectory(${MMLOOP_TEST_SRC_DIR} ${PROJECT_BINARY_DIR}/connecter/mmloop_test.out)

# ellog
set(ELLOG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ellog)
add_subdirectory(${ELLOG_SRC_DIR} ${PROJECT_BINARY_DIR}/ellog.out)

# testellog
set(TEST_ELLOG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ellog/test)
add_subdirectory(${TEST_ELLOG_SRC_DIR} ${PROJECT_BINARY_DIR}/testellog.out)

# MIMProtocol 
set(PRO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MIMProtocol)
add_subdirectory(${PRO_SRC_DIR} ${PROJECT_BINARY_DIR}/MIMProtocol.out)

# utils
set(UTILS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${UTILS_SRC_DIR} ${PROJECT_BINARY_DIR}/utils.out)

# testsqler
set(TEST_SQLER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sqler/mysqler/testsqler)
add_subdirectory(${TEST_SQLER_SRC_DIR} ${PROJECT_BINARY_DIR}/testsqler.out)

