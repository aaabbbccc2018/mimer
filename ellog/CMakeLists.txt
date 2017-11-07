# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (ellog)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
aux_source_directory(../utils/stream DIR_LIB_SRCS)
## 指定include 路径
include_directories(./ ../utils/stream/)
# 生成动态链接库
add_library (ellog SHARED ${DIR_LIB_SRCS})
# 生成静态链接库
#add_library (ellog STATIC ${DIR_LIB_SRCS})

