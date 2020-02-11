# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.1)

# Project Name
PROJECT(WebServerTest)

# Configuration

# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.arm.cmake

#
# To build with a release mode, cmake -DCMAKE_BUILD_TYPE=Release ..
#
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
  SET(CMAKE_DEBUG_POSTFIX "d")
endif(NOT CMAKE_BUILD_TYPE)

SET(LUNA_SW_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/../../)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
	./ 
	${LUNA_SW_PLATFORM}/src/Library/ccCore/include
	${LUNA_SW_PLATFORM}/src/Library/ccNetwork/include
	${LUNA_SW_PLATFORM}/src/Library/ccWebServer/include
	${LUNA_SW_PLATFORM}/src/FossLibrary/ccJsonParser/include
	${LUNA_SW_PLATFORM}/src/FossLibrary/ccWebsocketClient/include
	${LUNA_SW_PLATFORM}/src/ThirdPartyLibrary/ccMongooseWebServer/include
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
	ccMongooseWebServer${CMAKE_DEBUG_POSTFIX}
	ccWebsocketClient${CMAKE_DEBUG_POSTFIX}
	ccJsonParser${CMAKE_DEBUG_POSTFIX}
	ccWebServer${CMAKE_DEBUG_POSTFIX}
	ccNetwork${CMAKE_DEBUG_POSTFIX}
	ccCore${CMAKE_DEBUG_POSTFIX}
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
