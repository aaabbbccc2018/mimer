# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mysqler)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)
#定义ELLOG位置
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG)
  set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ellog/)
  add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
else()
  set(ELLOG_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT ELLOG)
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
add_library (mysqler SHARED ${DIR_SRCS})
target_link_libraries(mysqler libmysqlclient.so ellog)

# 启用测试
enable_testing()
