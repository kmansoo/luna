#
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

#
PROJECT(ccGCPIoTDevice)

# Configuration
#

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL)

if (NOT OpenSSL_FOUND)
		message(STATUS "OpenSSL not found.")
endif()

include_directories(
	./include
	../../Library/ccCore/include
	../../Library/ccNetwork/include
	../../FossLibrary/ccWebsocketClient/include
	../../FossLibrary/ccMqttClient/include
	../../Library/ccWebServer/include
	../../ThirdPartyLibrary/ccMongooseServer/include)

file(GLOB SRC_FILES 
	src/*.cpp
	src/JWT/*.cpp
)

add_library(${PROJECT_NAME} STATIC
	${SRC_FILES}
)
