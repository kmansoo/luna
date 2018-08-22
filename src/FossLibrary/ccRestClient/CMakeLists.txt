# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

# Project Name
PROJECT(ccRestClient)

# Configuration

include_directories(
	./include
	../../Library/ccCore/include
        ../../../dependency/curl/include
)

file(GLOB SRC_FILES 
	src/*.cc
)

add_library (ccRestClient STATIC
	${SRC_FILES}
)

