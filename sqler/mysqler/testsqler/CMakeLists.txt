# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (testsqler)
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

set(MYSQL_HOME "/usr/local/mysql")
# 添加 mysql 头文件
include_directories(${MYSQL_HOME}/include)

# 添加 mysql 头文件
# add sqler shared lib
#定义sqler位置
if(NOT SQLER_DIR)
	set(SQLER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
endif()
if(IS_DIRECTORY ${SQLER_DIR})
	add_subdirectory(${SQLER_DIR} ${PROJECT_BINARY_DIR}/sqler.out)
	set(SQLER_LINK_DIR ${PROJECT_BINARY_DIR}/sqler.out)
else()
	message(FATAL_ERROR   "INVALID FOLDER 'SQLER_DIR'=${SQLER_DIR}" )
endif()
# valgrind
include (CTest)
# 指定include 路径
include_directories(../)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
link_directories(${SQLER_LINK_DIR})
# 指定生成目标
add_executable(testsqler ${DIR_SRCS})
target_link_libraries(testsqler mysqler)

