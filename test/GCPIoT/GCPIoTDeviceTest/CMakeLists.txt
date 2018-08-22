#
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

#
PROJECT(GCPIoTDeviceTest)

# Configuration
#

find_package(OpenSSL)

SET(LUNA_SW_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_DEFINITIONS(-g)

include_directories(
	./ 
	${LUNA_SW_PLATFORM}/src/Library/ccCore/include
	${LUNA_SW_PLATFORM}/src/Library/ccNetwork/include
	${LUNA_SW_PLATFORM}/src/FossLibrary/ccMqttClient/include
	${LUNA_SW_PLATFORM}/src/CloudIoTLibrary/ccGCPIoTDevice/include	
	${LUNA_SW_PLATFORM}/dependency/json/single_include

	${OPENSSL_INCLUDE_DIR}
)

link_directories(${LUNA_SW_PLATFORM}/build/lib)

add_executable(${PROJECT_NAME}
    src/main.cpp
		src/ccGCPIoTDevice.cpp
		src/JWT/jwt.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${OPENSSL_SSL_LIBRARY}
	${OPENSSL_CRYPTO_LIBRARY}
	ccMqttClient
	ccNetwork
	ccCore
	pthread
)
