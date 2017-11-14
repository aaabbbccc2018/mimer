# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)
# get libuv from github
set(LIBUV_PATH ${CMAKE_MODULE_PATH}/connecter/libuv)
find_path(uv_src autogen.sh ${LIBUV_PATH})
if (NOT uv_src)
	EXECUTE_PROCESS(COMMAND echo "get source form github...")
	EXECUTE_PROCESS(COMMAND git clone https://github.com/libuv/libuv.git ${LIBUV_PATH})
	EXECUTE_PROCESS(COMMAND echo "end of get libuv source")
else()
	message(STATUS "libuv source is exist at ${LIBUV_PATH}")
endif(NOT uv_src)
configure_file(${CMAKE_MODULE_PATH}/connecter/libuv.vcxproj ${LIBUV_PATH}/ COPYONLY)
#check libuv is build and install
set(LOG "log.txt")  
set(UV_PREFIX ${CMAKE_MODULE_PATH}/lib/3rdlib/libuv)
find_library(uv_lib uv ${UV_PREFIX}/lib)
if (NOT uv_lib)
	message(STATUS "libuv.so source is not build")
	add_custom_command(OUTPUT  ${LOG}
		COMMAND echo "Start build libuv..."
		COMMAND cd ${LIBUV_PATH} && sh +x autogen.sh && ./configure --prefix=${UV_PREFIX} && make && make install
		COMMAND echo "End of build libuv"
		COMMENT "uv is builded"
		)
	add_custom_target(uv ALL DEPENDS ${LOG})
	#add_custom_target(uv)
else()
	message(STATUS "libuv.so source is install at ${UV_PREFIX}")
endif(NOT uv_lib)

