# CMake Version 
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

# Project Name
PROJECT(LUNA-SW-PLATFORM)

#
# Configuration
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)
#

option(GIT_SUBMODULE      "Check submodules during build"               OFF)
option(USE_MBEDTLS        "Use Mbedtls library"                         OFF)
option(USE_CURL           "Use Curl library"                            OFF)

if (GIT_SUBMODULE)
  message(STATUS "Submodule update")
  execute_process(COMMAND git submodule update --init --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  RESULT_VARIABLE GIT_SUBMOD_RESULT)

  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif()

#
# Configuration
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)
#

# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.arm.cmake

# BUILD TYPE
if ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "debug")
else ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "release")
endif ("${build}" MATCHES "debug")

#
# Add Compiler Option and Macro
#
ADD_DEFINITIONS(-DCALIB_CFG_BUILD_MODE="${CMAKE_BUILD_TYPE}")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

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
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  ADD_DEFINITIONS(-DOSX)
endif()

if (USE_CURL)
  ADD_DEFINITIONS(-LUNA_USE_CURL)
endif()

ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

if (USE_MBEDTLS)
  option(ENABLE_TESTING     "Build mbed TLS tests."       OFF)
  option(ENABLE_PROGRAMS    "Build mbed TLS programs."    OFF)

  ADD_SUBDIRECTORY(dependency/mbedtls)
endif()

#
# Make Libraries
#
ADD_SUBDIRECTORY(src/Library/ccCore)
ADD_SUBDIRECTORY(src/Library/ccNetwork)
ADD_SUBDIRECTORY(src/Library/ccWebServer)
ADD_SUBDIRECTORY(src/FossLibrary/ccJsonParser)
ADD_SUBDIRECTORY(src/FossLibrary/ccWebsocketClient)

if (USE_CURL EQUAL ON)
  ADD_SUBDIRECTORY(src/FossLibrary/ccRestClient)
else()
  message(STATUS "=============================================================================================")
  message(STATUS "The ccRestClient is excluded in building libraries because Luna doesn't support CURL library.")
  message(STATUS "=============================================================================================")
endif()

ADD_SUBDIRECTORY(src/FossLibrary/ccMqttClient)
ADD_SUBDIRECTORY(src/FossLibrary/poco)
ADD_SUBDIRECTORY(src/ThirdPartyLibrary/ccMongooseServer)
