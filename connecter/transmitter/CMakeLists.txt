## Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
## 项目信息
project (transmitter)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)

#定义uvbase位置
get_property(transmitter_UVBASE_LIBRARY GLOBAL PROPERTY "UVBASE_LIBRARY")
find_library(UVBASE uvbase ${CMAKE_MODULE_PATH}/lib)
if(NOT UVBASE AND NOT transmitter_UVBASE_LIBRARY)
	message(STATUS "not find ${CMAKE_MODULE_PATH}")
	set(UVBASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../uvbase)
	add_subdirectory(${UVBASE_DIR} ${PROJECT_BINARY_DIR}/uvbase.out)
else()
	message(STATUS "finded")
	set(UVBASE_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT UVBASE AND NOT transmitter_UVBASE_LIBRARY)

link_directories(${UVBASE_LINK_DIR} ${CMAKE_MODULE_PATH}/lib/3rdlib/libuv/lib ${CMAKE_MODULE_PATH}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../uvbase
	                  ${CMAKE_MODULE_PATH}//lib/3rdlib/libuv/include
	                  ./src/transmitter
					  ${CMAKE_MODULE_PATH}/ellog)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./ DIR_SRCS)
aux_source_directory(./src/transmitter DIR_SRCS)
add_executable(transmitter ${DIR_SRCS})
target_link_libraries(transmitter uvbase uv ellog)
