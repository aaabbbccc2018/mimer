# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mysqler)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)

#定义ELLOG位置
get_property(ELLOG_BUILED_MODULE GLOBAL PROPERTY "ellog_build")
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG AND NOT ELLOG_BUILED_MODULE)
	set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
	add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
	message(STATUS "ellog will build at ${PROJECT_NAME}")
else()
	set(ELLOG_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
	message(STATUS "ellog is found at ${CMAKE_MODULE_PATH}/lib")
endif(NOT ELLOG AND NOT ELLOG_BUILED_MODULE)

# 添加 mysql 头文件
set(MYSQL_HOME "/usr/local/mysql")
include_directories(${MYSQL_HOME}/include)
# 添加 mysql 库目录
link_directories(${MYSQL_HOME}/lib ${ELLOG_LINK_DIR})
# valgrind
include (CTest)
# 指定include 路径
include_directories(.)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
#aux_source_directory(testsqler DIR_SRCS)

# 指定生成目标
#add_executable(mysqler ${DIR_SRCS})
add_library(mysqler SHARED ${DIR_SRCS})
add_library(mysqler_static STATIC ${DIR_SRCS})
target_link_libraries(mysqler libmysqlclient.so ellog)
set_target_properties(mysqler_static PROPERTIES OUTPUT_NAME "mysqler")
set_target_properties(mysqler PROPERTIES VERSION 0.1 SOVERSION 1)

# 启用测试
enable_testing()
