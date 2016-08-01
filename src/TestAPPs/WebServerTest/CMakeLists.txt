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

add_custom_command(
	TARGET WebServerTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    	${CMAKE_CURRENT_SOURCE_DIR}/html/index.html
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/index.html
)

add_custom_command(
	TARGET WebServerTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    	${CMAKE_CURRENT_SOURCE_DIR}/html/chat.html
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/html/chat.html
)
