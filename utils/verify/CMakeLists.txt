# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (verifyTools)
# 添加宏REVERSED
ADD_DEFINITIONS(-DREVERSED=0)
add_compile_options(-std=c++11)
# BUILD_TYPE
if(BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-write-strings -Wno-sign-compare -pthread")
else(BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wno-write-strings -Wno-sign-compare -pthread -fopenmp")
endif(BUILD_TYPE STREQUAL "Debug" )
# valgrind
include (CTest)
# 指定include 路径
include_directories(.)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 指定生成目标
add_executable(verifyTools ${DIR_SRCS})

