## Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
## 项目信息
project (uvbase)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)
#定义uvbase位置
find_library(UV uv ${CMAKE_MODULE_PATH}/lib/3rdlib/lib)
if(NOT UV)
	message(STATUS "not find ${CMAKE_MODULE_PATH}")
  set(UVBASE_DIR ${CMAKE_MODULE_PATH}/3rdparty)
  add_subdirectory(${UVBASE_DIR} ${PROJECT_BINARY_DIR}/uvbase.out)
	set(UVBASE_LINK_DIR ${CMAKE_MODULE_PATH}/lib/3rdlib/libuv/lib)
else()
	message(STATUS "finded")
  set(UVBASE_LINK_DIR ${CMAKE_MODULE_PATH}/lib/3rdlib/libuv/lib)
endif(NOT UV)

link_directories(${UVBASE_LINK_DIR})
include_directories(${CMAKE_MODULE_PATH}/lib/3rdlib/libuv/include)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./  DIR_LIB_SRCS)
add_library(uvbase SHARED ${DIR_LIB_SRCS})
add_library(uvbase_static STATIC ${DIR_LIB_SRCS})
add_dependencies(uvbase uv)
target_link_libraries(uvbase libuv.so)
set_target_properties(uvbase_static PROPERTIES OUTPUT_NAME "uvbase")
set_target_properties(uvbase PROPERTIES VERSION 0.1 SOVERSION 1)
