# CMake Version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(luna_sw_platform)

#
# Configuration
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)
#
#
if ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "debug")
else ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "release")
endif ("${build}" MATCHES "debug")

#
# Add Compiler Option and Macro
#
ADD_DEFINITIONS(-DCALIB_CFG_BUILD_MODE="${CMAKE_BUILD_TYPE}")

#
# Check Build OS 
# cmake -D bit=32
if ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=32 -m32)
 SET (CMAKE_EXE_LINKER_FLAGS -m32)
else ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=64 -m64)
 SET (CMAKE_EXE_LINKER_FLAGS -m64)
endif ("${bit}" MATCHES "32")

# Set Build Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#
# Check OS and Processor Information 
#
ADD_DEFINITIONS(-Wall -std=c++11 -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

#
# Make Libraries
#
ADD_SUBDIRECTORY(src/Library/ccCore)
ADD_SUBDIRECTORY(src/Library/ccNetwork)
ADD_SUBDIRECTORY(src/Library/ccWebServer)
ADD_SUBDIRECTORY(src/FossLibrary/ccJsonParser)
ADD_SUBDIRECTORY(src/FossLibrary/ccWebsocketClient)
ADD_SUBDIRECTORY(src/ThirdPartyLibrary/ccMongooseServer)
ADD_SUBDIRECTORY(src/ProductLibrary/ccRESTfulChatting)

#
# Make Test Applications
#
ADD_SUBDIRECTORY(src/TestAPPs/WebServerTest)

