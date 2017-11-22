# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (testMimProt)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)

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

include (CTest)
# 指定include 路径
include_directories(../MIMProtocol ../ellog ../utils/stream)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../utils/stream/Stream.cpp DIR_SRCS)
link_directories(${ELLOG_LINK_DIR})
# 指定生成目标
add_executable(testMimProt ${DIR_SRCS})
target_link_libraries(testMimProt ellog MIMProtocol)

# 启用测试
enable_testing()
# 测试程序是否成功运行
foreach (index RANGE 1 14)
	add_test (packet_type_${index}_dried testMimProt ${index} 1)
	add_test (packet_type_${index}_official testMimProt ${index} 0)
endforeach ()
