# CMake Version 
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

# Project Name
PROJECT(LUNA-SW-PLATFORM)

#
# Configuration
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)
#

option(GIT_SUBMODULE  "Check submodules during build" ON)
option(USE_CURL       "Use cURL Library"              ON)
option(USE_MBEDTLS    "Use Mbedtls Library"           ON)
option(USE_MOSQUITTO  "Use mosquitto Library"         ON)

if (USE_CURL OR USE_MBEDTLS OR USE_MOSQUITTO)
  SET(GIT_SUBMODULE, ON)
endif()

if (GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND git submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)

    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

if (USE_MOSQUITTO)
  find_package(OpenSSL)

  if (NOT OpenSSL_FOUND)
      message(STATUS "OpenSSL not found. So you can not use the MOSQUITTO library!!!")

      set(USE_MOSQUITTO OFF)
  elseif()
    SET(USE_MBEDTLS, OFF)
  endif()
endif()

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

if (USE_MBEDTLS)
  option(ENABLE_TESTING     "Build mbed TLS tests."       OFF)
  option(ENABLE_PROGRAMS    "Build mbed TLS programs."    OFF)

  ADD_SUBDIRECTORY(dependency/mbedtls)
endif()

if (USE_CURL)
  if (USE_MBEDTLS)
    option(CMAKE_USE_MBEDTLS  "Enable mbedTLS for SSL/TLS"  ON)

    # For mbedTLS in curl
    SET(MBEDCRYPTO_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedcrypto.a)
    SET(MBEDTLS_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedtls.a)
    SET(MBEDX509_LIBRARY ${CMAKE_BINARY_DIR}/lib/libmbedx509.a)

    SET(MBEDTLS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependency/mbedtls/include)
  endif()

  option(ENABLE_MANUAL "to provide the built-in manual for cURL"  OFF)
  option(BUILD_TESTING "Disable buinding for cURL test"           OFF)

  ADD_SUBDIRECTORY(dependency/curl)
endif()

if (USE_MOSQUITTO)
  option(WITH_TLS_PSK, "Build without TLS_PSK"  OFF)

  ADD_SUBDIRECTORY(dependency/mosquitto)
endif()

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
