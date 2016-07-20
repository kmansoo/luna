# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(ccJsonParser)

# Configuration

include_directories( ./ ../../Library ../../FossLibrary)

file(GLOB SRC_FILES 
	src/lib_json/*.cpp
)

add_library (ccJsonParser
	${SRC_FILES}
)

