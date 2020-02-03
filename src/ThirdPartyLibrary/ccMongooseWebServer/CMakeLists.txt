#
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

#
PROJECT(ccMongooseWebServer)

# Configuration
#

add_definitions(
	-DMG_DISABLE_CGI
	-DMG_ENABLE_HTTP_STREAMING_MULTIPART
)

include_directories( 
	./include 
	../../Library/ccCore/include
	../../Library/ccWebServer/include
)

file(GLOB SRC_FILES 
	src/*.cpp
	src/mongoose/*.c
)

add_library (ccMongooseWebServer STATIC
	${SRC_FILES}
)
