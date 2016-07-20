#
# cmake 버전을 2.8이상만 사용
#
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

#
# 프로젝트 이름
#
PROJECT(luna_sw_platform)

#
# SET함수는 변수를 만들거나 변수의 값을 설정
#
# SET(CMAKE_VERBOSE_MAKEFILE ON)

#
# cmake -D build=debug
# (주의) cmake에서는 문자열의 비교를 MATCHES로 처리합니다. 그리고 else와 endif에 if에서 사용된 표현식을 다시한번 써줍
#
if ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "debug")
else ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "release")
endif ("${build}" MATCHES "debug")

#
# 컴파일 옵션에 추가될 매크로 정의
#
ADD_DEFINITIONS(-DCALIB_CFG_BUILD_MODE="${CMAKE_BUILD_TYPE}")

#
# 빌드 비트를 확인
# cmake -D bit=32
if ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=32 -m32)
 SET (CMAKE_EXE_LINKER_FLAGS -m32)
else ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=64 -m64)
 SET (CMAKE_EXE_LINKER_FLAGS -m64)
endif ("${bit}" MATCHES "32")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#
# OS 및 Processor 정보
# (설명) CMAKE_SYSTEM_NAME과 CMAKE_SYS_PROCESSORS은 각각 운영체제 이름과 프로세서 정보를 가지고 있는 내장 변수 
#	    configure를 사용하지 않는 대신에 cmake가 가진 정보를 사용
#
ADD_DEFINITIONS(-Wall -std=c++11 -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

#
# src/CMakeLists.txt를 처리
# make libraries
ADD_SUBDIRECTORY(src/Library/ccCore)
ADD_SUBDIRECTORY(src/Library/ccNetwork)
ADD_SUBDIRECTORY(src/Library/ccWebServer)
ADD_SUBDIRECTORY(src/FossLibrary/ccJsonParser)
ADD_SUBDIRECTORY(src/FossLibrary/ccWebsocketClient)
ADD_SUBDIRECTORY(src/ThirdPartyLibrary/ccMongooseServer)
ADD_SUBDIRECTORY(src/ProductLibrary/ccRESTfulChatting)

#
# make test applications
#
ADD_SUBDIRECTORY(src/TestAPPs/WebServerTest)

