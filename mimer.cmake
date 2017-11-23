# set build type
set(CMAKE_BUILD_TYPE "Debug")

#Add the c++11 flag, whatever it is
# std++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-sign-compare")
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wno-sign-compare -fopenmp")
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

#message("Build Type:" ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

set(TOP_PATH ${CMAKE_MODULE_PATH})
#message("TOP path:" ${TOP_PATH})

if (NOT (PROJECT_NAME STREQUAL "Project"))
	#set(BUILED_NAME ${PROJECT_NAME}_build)
	set_property(GLOBAL PROPERTY ${PROJECT_NAME}_build TRUE)
	get_property(BUILED_MODULE_GET GLOBAL PROPERTY "${PROJECT_NAME}_build")
	message("PROJECT_NAME: ${PROJECT_NAME} is defined variable: ${BUILED_MODULE_GET}")
endif(NOT (PROJECT_NAME STREQUAL "Project"))

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TOP_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TOP_PATH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOP_PATH}/bin)
