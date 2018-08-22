# CMake version
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

#
# Project Name
PROJECT(ccCore)

#
# Configuration

include_directories( ./include)

# add_library(<name> [STATIC | SHARED | MODULE]
#             [EXCLUDE_FROM_ALL]
#             source1 [source2 ...])

file(GLOB SRC_FILES 
	src/*.cpp
)

add_library (ccCore STATIC
	${SRC_FILES}
)
