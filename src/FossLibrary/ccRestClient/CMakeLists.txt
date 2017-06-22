# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(ccRestClient)

# Configuration

include_directories(
	./include
	../../Library/ccCore/include
)

file(GLOB SRC_FILES 
	src/*.cc
)

add_library (ccRestClient
	${SRC_FILES}
)

