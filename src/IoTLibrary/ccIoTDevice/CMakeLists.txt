#
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

#
PROJECT(ccIoTDevice)

# Configuration
#

include_directories(
	./include
	../../Library/ccCore/include
	../../Library/ccNetwork/include
	../../FossLibrary/ccJsonParser/include
	../../FossLibrary/ccWebsocketClient/include
	../../Library/ccWebServer/include
	../../ThirdPartyLibrary/ccMongooseServer/include)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library(ccIoTDevice
	${SRC_FILES}
)
