# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

# Project Name
PROJECT(ccNetwork)

# Configuration

include_directories( 
	./include
	../../Library/ccCore/include)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccNetwork
	${SRC_FILES}
)
