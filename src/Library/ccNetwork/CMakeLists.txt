# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

# Project Name
PROJECT(ccNetwork)

# Configuration

include_directories( 
	./include
	../../Library/ccCore/include)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccNetwork STATIC
	${SRC_FILES}
)
