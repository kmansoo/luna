# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

# Project Name
PROJECT(WebServerTest)

# Configuration
SET(LUNA_SW_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/../../)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(
	./ 
	${LUNA_SW_PLATFORM}/src/Library/ccCore/include
	${LUNA_SW_PLATFORM}/src/Library/ccNetwork/include
	${LUNA_SW_PLATFORM}/src/Library/ccWebServer/include
	${LUNA_SW_PLATFORM}/src/FossLibrary/ccJsonParser/include
	${LUNA_SW_PLATFORM}/src/FossLibrary/ccWebsocketClient/include
	${LUNA_SW_PLATFORM}/src/ThirdPartyLibrary/ccMongooseServer/include
)

link_directories(
	${LUNA_SW_PLATFORM}/build/lib
)

add_executable (WebServerTest
    src/main.cpp
    src/ChattingWSManager.cpp
    src/RESTfulChattingApiManager.cpp
)

TARGET_LINK_LIBRARIES(WebServerTest
	ccMongooseServer
	ccWebsocketClient
	ccJsonParser
	ccWebServer
	ccNetwork
	ccCore
	pthread
)  # WebServerTest now depends on ccCore ... and ccNetwork and ..

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
