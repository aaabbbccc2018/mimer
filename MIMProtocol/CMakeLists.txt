# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mimer)
# 添加宏REVERSED
ADD_DEFINITIONS(-DREVERSED=0)
# std++11
include(CheckCXXCompilerFlag)  
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)  
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)  
if(COMPILER_SUPPORTS_CXX11)  
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  
elseif(COMPILER_SUPPORTS_CXX0X)  
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")  
else()  
     message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")  
endif()
# BUILD_TYPE
if(BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-write-strings -Wno-sign-compare -pthread")
else(BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wno-write-strings -Wno-sign-compare -pthread -fopenmp")
endif(BUILD_TYPE STREQUAL "Debug" )
# add ellog shared lib
#定义ELLOG位置
if(NOT ELLOG_DIR)
	set(ELLOG_DIR ${CMAKE_SOURCE_DIR}/../ellog)
endif()
if(IS_DIRECTORY ${ELLOG_DIR})
	add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
	set(ELLOG_LINK_DIR ${PROJECT_BINARY_DIR}/ellog.out)
else()
	message(FATAL_ERROR   "INVALID FOLDER 'ELLOG_DIR'=${ELLOG_DIR}" )
endif()
# valgrind
include (CTest)
# 指定include 路径
include_directories(./ ../ellog/ ../utils/stream)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../utils/stream DIR_SRCS)
link_directories(${ELLOG_LINK_DIR})
# 指定生成目标
add_executable(mimer ${DIR_SRCS})
target_link_libraries(mimer ellog)

# 启用测试
enable_testing()
# 测试程序是否成功运行
foreach (index RANGE 1 14)
	add_test (packet_type_${index}_dried mimer ${index} 1)
	add_test (packet_type_${index}_official mimer ${index} 0)
endforeach ()
