# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

# Project Name
PROJECT(ccWebServer)

# Configuration

include_directories(
	./include
	../../Library/ccCore/include
	../../Library/ccNetwork/include)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccWebServer
	${SRC_FILES}
)
