# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (SyncTools)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)
#定义ELLOG位置
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG AND NOT ${ELLOG_LIBRARY})
	set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ellog)
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
include_directories(.)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
link_directories(${ELLOG_LINK_DIR})
# 指定生成目标
add_executable(SyncTools ${DIR_SRCS})
target_link_libraries(SyncTools libpthread.so ellog)

