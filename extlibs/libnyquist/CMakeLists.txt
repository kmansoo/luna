#
CMAKE_MINIMUM_REQUIRED (VERSION 3.5.1)

#
PROJECT(nyquist)

# Configuration
#
ADD_DEFINITIONS(-DUSE_ALLOCA -DOPUS_BUILD)
SET(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(
	./include
	./include/libnyquist
	./third_party
	./third_party/FLAC
	./third_party/FLAC/src/include
	./third_party/libmodplug
	./third_party/libogg/include
	./third_party/libvorbis/include
	./third_party/libvorbis/src
	./third_party/libvorbis/src/modes
	./third_party/musepack/include
	./third_party/opus
	./third_party/opus/celt
	./third_party/opus/libopus/include
	./third_party/opus/opusfile/include
	./third_party/opus/opusfile/src/include
	./third_party/opus/silk
	./third_party/opus/silk/fixed
	./third_party/opus/silk/float
	./third_party/opus/silk/arm
	./third_party/rtaudio
	./third_party/wavpack/include
	../rtaudio
)

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

    ADD_DEFINITIONS(-DHAVE_STDINT_H -DHAVE_SETENV)

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

link_directories(
	./
	../lib
)

file(GLOB LIB_SRC_FILES 
	src/*.cpp
	src/*.c
)

file(GLOB TEST_SRC_FILES 
	test/*.cpp
)

add_library(nyquist
	${LIB_SRC_FILES}
)

add_executable (test-player
    ${TEST_SRC_FILES}
)

TARGET_LINK_LIBRARIES(test-player
	nyquist
	rtaudio
	${LINKLIBS}
)
