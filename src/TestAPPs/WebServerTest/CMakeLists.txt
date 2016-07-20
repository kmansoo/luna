# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Project Name
PROJECT(WebServerTest)

# Configuration
include_directories( ./ ../../Library ../../FossLibrary ../../ThirdPartyLibrary  ../../ProductLibrary)

add_executable (WebServerTest
    src/main.cpp
)

TARGET_LINK_LIBRARIES(WebServerTest
	ccRESTfulChatting
	ccMongooseServer
	ccWebsocketClient
	ccJsonParser
	ccWebServer
	ccCore
	pthread
)  # WebServerTest now depends on ccCore ... and ccRESTfulChatting
