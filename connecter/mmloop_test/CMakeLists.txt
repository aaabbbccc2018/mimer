## Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
## 项目信息
project (mmloop_test)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)

#定义uvbase位置
get_property(UVBASE_BUILED_MODULE GLOBAL PROPERTY "uvbase_build")
find_library(UVBASE uvbase ${CMAKE_MODULE_PATH}/lib)
if(NOT UVBASE AND NOT UVBASE_BUILED_MODULE)
	set(UVBASE_DIR ../uvbase)
	add_subdirectory(${UVBASE_DIR} ${PROJECT_BINARY_DIR}/uvbase.out)
else()
	set(UVBASE_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT UVBASE AND NOT UVBASE_BUILED_MODULE)

link_directories(${UVBASE_LINK_DIR} ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_MODULE_PATH}/lib)
include_directories(./ ../uvbase ../../ ${CMAKE_INSTALL_PREFIX}/include ${CMAKE_MODULE_PATH}/ellog)
aux_source_directory(./ DIR_TEST_SRCS)
aux_source_directory(./Test_async DIR_TEST_SRCS)
aux_source_directory(./Test_DNS DIR_TEST_SRCS)
aux_source_directory(./Test_file DIR_TEST_SRCS)
aux_source_directory(./Test_idle DIR_TEST_SRCS)
aux_source_directory(./Test_pipe DIR_TEST_SRCS)
aux_source_directory(./Test_tcpClient DIR_TEST_SRCS)
aux_source_directory(./Test_TcpServer DIR_TEST_SRCS)
#aux_source_directory(./Test_ThreadPool DIR_TEST_SRCS)
aux_source_directory(./Test_timer DIR_TEST_SRCS)
aux_source_directory(./Test_udpClient DIR_TEST_SRCS)
aux_source_directory(./Test_udpServer DIR_TEST_SRCS)
aux_source_directory(./udpsend DIR_TEST_SRCS)
# 指定生成目标
add_executable(mmloop_test ${DIR_TEST_SRCS})
#add_dependencies(mmloop_test uvbase uv)
target_link_libraries(mmloop_test uvbase uv ellog)
