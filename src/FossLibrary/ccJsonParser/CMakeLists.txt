# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

# Project Name
PROJECT(ccJsonParser)

# Configuration

include_directories(
	./include
	../../Library/ccCore/include
)

file(GLOB SRC_FILES 
	src/lib_json/*.cpp
)

add_library (ccJsonParser STATIC
	${SRC_FILES}
)

