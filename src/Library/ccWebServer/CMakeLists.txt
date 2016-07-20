# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(ccWebServer)

# Configuration

include_directories( ./ ../../Library)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccWebServer
	${SRC_FILES}
)
