cmake_minimum_required(VERSION 2.8.9)
project(UTestingLib)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWINDOWS)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
 	# 64 bits
    	add_definitions(-DX64)
endif()
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#Generate the shared library from the sources
add_library(test_lib SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
# install(TARGETS testStudent DESTINATION /usr/lib)