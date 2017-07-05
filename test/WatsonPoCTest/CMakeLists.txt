# CMake Version
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

# Project Name
PROJECT(WatsonPoCTest)

# Configuration
SET(LUNA_SW_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/../../)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-std=c++11")


# -------------------------------------------------
set(LINKLIBS)
if (CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
    message(STATUS "NetBSD detected, using OSS")
    find_package(Threads REQUIRED CMAKE_THREAD_PREFER_PTHREAD)
    list(APPEND LINKLIBS ossaudio ${CMAKE_THREAD_LIBS_INIT})
    set(AUDIO_LINUX_OSS ON)
elseif (UNIX AND NOT APPLE)
    if (NOT AUDIO_LINUX_PULSE AND NOT AUDIO_LINUX_ALSA AND NOT AUDIO_LINUX_OSS AND NOT AUDIO_UNIX_JACK)
        set(AUDIO_LINUX_ALSA ON)
    endif()

    if (AUDIO_LINUX_PULSE)
        find_library(PULSE_LIB pulse)
        find_library(PULSESIMPLE_LIB pulse-simple)
        find_package(Threads REQUIRED CMAKE_THREAD_PREFER_PTHREAD)
        list(APPEND LINKLIBS ${PULSE_LIB} ${PULSESIMPLE_LIB} ${CMAKE_THREAD_LIBS_INIT})
        add_definitions(-D__LINUX_PULSE__)
        message(STATUS "Using Linux PulseAudio")
    endif (AUDIO_LINUX_PULSE)
    if (AUDIO_LINUX_ALSA)
        find_package(ALSA)
        find_package(Threads REQUIRED CMAKE_THREAD_PREFER_PTHREAD)
        if (NOT ALSA_FOUND)
            message(FATAL_ERROR "ALSA API requested but no ALSA dev libraries found")
        endif()
        include_directories(${ALSA_INCLUDE_DIR})
        list(APPEND LINKLIBS ${ALSA_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
        add_definitions(-D__LINUX_ALSA__)
        message(STATUS "Using Linux ALSA")
    endif (AUDIO_LINUX_ALSA)
endif ()

if (APPLE)
    if (NOT AUDIO_OSX_CORE AND NOT AUDIO_UNIX_JACK)
        set(AUDIO_OSX_CORE ON)
    endif()

    if (AUDIO_OSX_CORE)
        find_library(COREAUDIO_LIB CoreAudio)
        find_library(COREFOUNDATION_LIB CoreFoundation)
        list(APPEND LINKLIBS ${COREAUDIO_LIB} ${COREFOUNDATION_LIB})
        add_definitions(-D__MACOSX_CORE__)
        message(STATUS "Using OSX CoreAudio")
    endif (AUDIO_OSX_CORE)
endif (APPLE)

# JACK supported on many Unices
if (UNIX)
    if (AUDIO_UNIX_JACK)
        find_library(JACK_LIB jack)
        list(APPEND LINKLIBS ${JACK_LIB})
        add_definitions(-D__UNIX_JACK__)
        message(STATUS "Using JACK")
    endif (AUDIO_UNIX_JACK)
endif (UNIX)

if (WIN32)
    if (NOT AUDIO_WINDOWS_DS AND NOT AUDIO_WINDOWS_ASIO AND NOT AUDIO_WINDOWS_WASAPI)
        set(AUDIO_WINDOWS_WASAPI ON)
    endif()

    include_directories(include)
    list(APPEND LINKLIBS winmm ole32)

    if (AUDIO_WINDOWS_DS)
        add_definitions(-D__WINDOWS_DS__)
        message(STATUS "Using Windows DirectSound")
        list(APPEND LINKLIBS dsound)
    endif (AUDIO_WINDOWS_DS)
    if (AUDIO_WINDOWS_WASAPI)
        add_definitions(-D__WINDOWS_WASAPI__)
        message(STATUS "Using Windows WASAPI")
        list(APPEND LINKLIBS uuid ksuser)
    endif (AUDIO_WINDOWS_WASAPI)
    if (AUDIO_WINDOWS_ASIO)
        list(APPEND rtaudio_SOURCES
            include/asio.cpp
            include/asiodrivers.cpp
            include/asiolist.cpp
            include/iasiothiscallresolver.cpp)
        add_definitions(-D__WINDOWS_ASIO__)
        message(STATUS "Using Windows ASIO")
    endif (AUDIO_WINDOWS_ASIO)
endif (WIN32)
# -------------------------------------------------


include_directories(
    ./ 
    ${LUNA_SW_PLATFORM}/src/Library/ccCore/include
    ${LUNA_SW_PLATFORM}/src/Library/ccNetwork/include
    ${LUNA_SW_PLATFORM}/src/FossLibrary/ccJsonParser/include
    ${LUNA_SW_PLATFORM}/src/FossLibrary/ccRestClient/include
    ${LUNA_SW_PLATFORM}/dependency/curl/include
    ${LUNA_SW_PLATFORM}/extlibs/libnyquist/include
    ${LUNA_SW_PLATFORM}/extlibs
    ${LUNA_SW_PLATFORM}/extlibs/rtaudio
    ${LUNA_SW_PLATFORM}/extlibs/rtaudio/include
)

link_directories(
    ${LUNA_SW_PLATFORM}/build/lib
    ${LUNA_SW_PLATFORM}/extlibs/lib
)

file(GLOB SRC_FILES 
	src/*.cpp
	src/AudioManager/*.cpp
    src/AudioManager/rtaudio/*.cpp
)

add_executable (WatsonPoCTest
    ${SRC_FILES}
)

TARGET_LINK_LIBRARIES(WatsonPoCTest
    nyquist
    rtaudio
    ccRestClient
    ccJsonParser
    ccNetwork
    ccCore
    curl
    mbedtls
    mbedcrypto
    mbedx509
    pthread
    ${LINKLIBS}
) 
