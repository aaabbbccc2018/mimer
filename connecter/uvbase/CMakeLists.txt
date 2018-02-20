## Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
## 项目信息
project (uvbase)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)

#定义uvbase位置
message(STATUS "UVLIB_PATH ${UVLIB_PATH}")
find_library(UV uv ${UVLIB_PATH})
if(NOT UV)
	message(STATUS "not find ${CMAKE_MODULE_PATH}")
	set(UV_DIR ${CMAKE_MODULE_PATH}/3rdparty)
	add_subdirectory(${UV_DIR} ${PROJECT_BINARY_DIR}/uv.out)
else()
	message(STATUS "finded")
	set(UVBASE_LINK_DIR ${UVLIB_PATH})
endif(NOT UV)

link_directories(${UVBASE_LINK_DIR} ${CMAKE_INSTALL_PREFIX}/lib)
include_directories(${CMAKE_INSTALL_PREFIX}/include
	               ${CMAKE_MODULE_PATH}/ellog)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./  DIR_LIB_SRCS)
add_library(uvbase SHARED ${DIR_LIB_SRCS})
add_library(uvbase_static STATIC ${DIR_LIB_SRCS})
#add_dependencies(uvbase uv)
target_link_libraries(uvbase uv ellog)
set_target_properties(uvbase_static PROPERTIES OUTPUT_NAME "uvbase")
set_target_properties(uvbase PROPERTIES VERSION 0.1 SOVERSION 1)
install (TARGETS uvbase DESTINATION lib)
