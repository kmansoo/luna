# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.0.0)

# Project Name
PROJECT(ccMqttClient)

include_directories(
	./include/ccMqttClient
)

file(GLOB SRC_FILES 
	src/*.*
)

add_library (${PROJECT_NAME} STATIC
	${SRC_FILES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${OPENSSL_CRYPTO_LIBRARY}
)