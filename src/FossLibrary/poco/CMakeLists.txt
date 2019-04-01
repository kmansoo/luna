cmake_minimum_required(VERSION 3.2.0)

project(Poco)

option(POCO_VERBOSE_MESSAGES "Enable informational messages during configure" ON)
option(POCO_STATIC "Make static libraries" ON)

file(STRINGS "${PROJECT_SOURCE_DIR}/libversion" SHARED_LIBRARY_VERSION)

# Read the version information from the VERSION file
file (STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PACKAGE_VERSION )
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(PROJECT_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")

if(POCO_VERBOSE_MESSAGES)
    message(STATUS "Poco package version: ${PROJECT_VERSION}")
endif()

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin" like the Visual Studio projects do.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Append our module directory to CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(${CMAKE_VERSION} VERSION_LESS "3.4")
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/V33)
endif()
#################################################################################
# Setup C/C++ compiler options
#################################################################################
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option(POCO_DISABLE_CPP14 "Disable C++14 if not available" OFF)

if(NOT POCO_DISABLE_CPP14)
  # C++14 compiler flags
  include(CXX1x)
  check_for_cxx14_compiler(CXX14_COMPILER)

  # If a C++14 compiler is available, then set the appropriate flags
  if(CXX14_COMPILER)
    set(CMAKE_CXX_STANDARD 14)
  else()
    set(POCO_DISABLE_CPP14 ON CACHE BOOL "Disable C++14 if not available" FORCE)
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include some common macros to simpilfy the Poco CMake files
include(PocoMacros)

if(POCO_STATIC)
	message(DEPRECATION "POCO_STATIC is deprecated and will be removed! Use BUILD_SHARED_LIBS instead")
	if(POCO_VERBOSE_MESSAGES)
		message(STATUS "Building static libraries")
	endif()
	option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
else()
	if(POCO_VERBOSE_MESSAGES)
		message(STATUS "Building dynamic libraries")
	endif()
	option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

# Allow enabling and disabling components
option(POCO_ENABLE_NET "Enable Net" ON)
option(POCO_ENABLE_WSTRING "Enable std::wstring support" ON)

include(DefinePlatformSpecifc)

# Pthreads/threads support
find_package(Threads REQUIRED)

add_subdirectory(Foundation)

if(POCO_ENABLE_NET)
    add_subdirectory(Net)
    list(APPEND Poco_COMPONENTS "Net")
endif()

