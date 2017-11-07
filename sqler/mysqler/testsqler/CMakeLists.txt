# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (testsqler)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(../../../mimer.cmake)
set(MYSQL_HOME "/usr/local/mysql")
# 添加 mysql 头文件
include_directories(${MYSQL_HOME}/include)
# add sqler shared lib
#定义sqler位置
find_library(SQLER mysqler ${CMAKE_MODULE_PATH}/lib)
if(NOT SQLER)
	set(SQLER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
	add_subdirectory(${SQLER_DIR} ${PROJECT_BINARY_DIR}/sqler.out)
else()
  set(SQLER_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT SQLER)
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

