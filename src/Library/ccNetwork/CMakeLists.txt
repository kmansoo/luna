# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(ccNetwork)

# Configuration

include_directories( ./ ../../Library)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccNetwork
	${SRC_FILES}
)
