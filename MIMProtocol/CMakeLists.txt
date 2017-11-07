# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mimer)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)
#定义ELLOG位置
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG)
	#add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
	#set(ELLOG_LINK_DIR ${PROJECT_BINARY_DIR}/ellog.out)
	set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ellog/)
	add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
else()
	set(ELLOG_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT ELLOG)

# valgrind
include (CTest)
# 指定include 路径
include_directories(./ ../ellog/ ../utils/stream)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../utils/stream/Stream.cpp DIR_SRCS)
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
