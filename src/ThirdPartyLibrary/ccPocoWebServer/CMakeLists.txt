#
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

#
PROJECT(ccPocoWebServer)

# Configuration
#

include_directories( 
	./include 
	../../Library/ccCore/include
	../../Library/ccWebServer/include
	../../FossLibrary/poco/Foundation/include
	../../FossLibrary/poco/Net/include
)

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccPocoWebServer STATIC
	${SRC_FILES}
)
