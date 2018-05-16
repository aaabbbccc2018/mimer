#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
project (transmitter)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include(../../mimer.cmake)

#uvbaseλ
get_property(UVBASE_BUILED_MODULE GLOBAL PROPERTY "uvbase_build")
find_library(UVBASE uvbase ${CMAKE_MODULE_PATH}/lib)
if(NOT UVBASE AND NOT UVBASE_BUILED_MODULE)
        message(STATUS "not find ${CMAKE_MODULE_PATH}")
        set(UVBASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../uvbase)
        add_subdirectory(${UVBASE_DIR} ${PROJECT_BINARY_DIR}/uvbase.out)
else()
        message(STATUS "finded")
        set(UVBASE_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
endif(NOT UVBASE AND NOT UVBASE_BUILED_MODULE)

link_directories(${UVBASE_LINK_DIR} ${UVLIB_PATH} ${CMAKE_INSTALL_PREFIX}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../uvbase
                          ${CMAKE_INSTALL_PREFIX}/include
                          ./ ${CMAKE_MODULE_PATH}/ellog
                                          ${CMAKE_MODULE_PATH}/MIMProtocol)
aux_source_directory(. DIR_SRCS)
add_library(transmitter SHARED ${DIR_SRCS})
add_library(transmitter_static STATIC ${DIR_SRCS})
#add_dependencies(uvbase uv)
target_link_libraries(transmitter uv uvbase ellog)
set_target_properties(transmitter_static PROPERTIES OUTPUT_NAME "transmitter")
set_target_properties(transmitter PROPERTIES VERSION 0.1 SOVERSION 1)
install (TARGETS transmitter DESTINATION lib)
