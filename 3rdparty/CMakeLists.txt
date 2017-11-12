# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)
set(LOG "log.txt")  
add_custom_command(OUTPUT  ${LOG}  
	COMMAND echo "get source form github..."  
	#COMMAND git clone https://github.com/libuv/libuv.git ${CMAKE_CURRENT_SOURCE_DIR}/libuv
	COMMAND echo "end of get libuv source"  
	COMMAND echo "Start build libuv..."  
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/libuv && sh +x autogen.sh && ./configure --prefix=${CMAKE_MODULE_PATH}/lib/3rdlib && make && make install
	COMMAND echo "End of build libuv"  
	COMMENT "uv is builded"  
)  
add_custom_target(uv ALL DEPENDS ${LOG})
#add_custom_target(uv)
