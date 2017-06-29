# CMake Version 
CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

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
SET(CURL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/dependency/curl)
SET(MBED_TLS ${CMAKE_CURRENT_SOURCE_DIR}/dependency/mbedtls)

#
# You can refer to following the SET command when you need to set the environment value in dependency Libraries.

#
# To customize options for cur
#
# For mbedTLS in curl
SET(CMAKE_USE_MBEDTLS ON CACHE BOOL "Enable mbedTLS for SSL/TLS" FORCE)
SET(CURL_ZLIB OFF CACHE BOOL "Set to ON to enable building curl with zlib support." FORCE)

SET(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
SET(CURL_STATICLIB ON CACHE BOOL "" FORCE)
SET(ENABLE_MANUAL OFF CACHE BOOL "" FORCE)
SET(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# For mbedTLS in curl
SET(MBEDCRYPTO_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedcrypto.a)
SET(MBEDTLS_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedtls.a)
SET(MBEDX509_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedx509.a)
SET(MBEDTLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependency/mbedtls/include)

#
# Check OS and Processor Information 
#
SET(CMAKE_CXX_FLAGS "-std=c++11")
ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

#
# Make Dependency Libraries
#
ADD_SUBDIRECTORY(dependency/mbedtls)
ADD_SUBDIRECTORY(dependency/curl)

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


