# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(luna_sw_platform)

# Configuration

include_directories( ./ ../../Library ../../FossLibrary ../../ThirdPartyLibrary  ../../ProductLibrary)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccRESTfulChatting
	${SRC_FILES}
)
