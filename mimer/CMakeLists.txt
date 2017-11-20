# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (MIMProtocol)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)
#定义ELLOG位置
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG AND NOT ${ELLOG_LIBRARY})
	set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ellog/)
	add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
	set(ELLOG_LIBRARY TRUE CACHE INTERNAL "ellog is builded at ${PROJECT_NAME}")
	message(STATUS "ELLOG_LIBRARY = ${ELLOG_LIBRARY} ellog is build at ${PROJECT_NAME}")
else()
	#set(ELLOG_LIBRARY TRUE CACHE INTERNAL "ellog is builded at ${PROJECT_NAME}")
	set(ELLOG_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
	message(STATUS "ELLOG_LIBRARY = ${ELLOG_LIBRARY} ellog is found at ${CMAKE_MODULE_PATH}/lib")
endif(NOT ELLOG AND NOT ${ELLOG_LIBRARY})

# valgrind
include (CTest)
# 指定include 路径
include_directories(./ ../ellog/ ../utils/stream)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../utils/stream/Stream.cpp DIR_SRCS)
link_directories(${ELLOG_LINK_DIR})
# 生成动态链接库
add_library(MIMProtocol SHARED ${DIR_SRCS})
add_library(MIMProtocol_static STATIC ${DIR_SRCS})
target_link_libraries(MIMProtocol ellog)
set_target_properties(MIMProtocol_static PROPERTIES OUTPUT_NAME "MIMProtocol")
set_target_properties(MIMProtocol PROPERTIES VERSION 0.1 SOVERSION 1)
