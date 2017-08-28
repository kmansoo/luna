# CMake Version 
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

# Project Name
PROJECT(luna_sw_platform)

#
# Configuration
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)
#

# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.arm.cmake

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

# Set Build Directories
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(LUNA_SW_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR})

#
# You can refer to following the SET command when you need to set the environment value in dependency Libraries.

#
# Check OS and Processor Information 
#
SET(CMAKE_CXX_FLAGS "-std=c++11")
ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

#
# Make Libraries
#
ADD_SUBDIRECTORY(src/Library/ccCore)
ADD_SUBDIRECTORY(src/Library/ccNetwork)
ADD_SUBDIRECTORY(src/Library/ccWebServer)
ADD_SUBDIRECTORY(src/FossLibrary/ccJsonParser)
ADD_SUBDIRECTORY(src/FossLibrary/ccWebsocketClient)
ADD_SUBDIRECTORY(src/FossLibrary/ccRestClient)
ADD_SUBDIRECTORY(src/ThirdPartyLibrary/ccMongooseServer)
ADD_SUBDIRECTORY(src/IoTLibrary/ccIoTDevice)
