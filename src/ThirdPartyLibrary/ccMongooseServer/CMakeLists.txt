#
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

#
PROJECT(ccMongooseServer)

# Configuration
#

include_directories( ./ ../../Library ../../FossLibrary ../../ThirdPartyLibrary)

file(GLOB SRC_FILES 
	src/*.cpp
	src/mongoose/*.c
)

add_library (ccMongooseServer
	${SRC_FILES}
)
