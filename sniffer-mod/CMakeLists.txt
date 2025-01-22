cmake_minimum_required(VERSION 3.10)

# Project name
project(SnifferFYP)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(src/include)

# Source files
file(GLOB SNIFFER_SOURCES "src/*.c" "src/*.cpp")

# Platform-specific settings
if(WIN32)
    set(NPCAP_SDK_URL "https://npcap.com/dist/npcap-sdk-1.13.zip")
    set(NPCAP_SDK_ZIP "${CMAKE_BINARY_DIR}/npcap-sdk.zip")
    set(NPCAP_SDK_DIR "${CMAKE_BINARY_DIR}/npcap-sdk")

    # Download Npcap SDK
    if(NOT EXISTS ${NPCAP_SDK_ZIP})
        message(STATUS "Downloading Npcap SDK...")
        file(DOWNLOAD ${NPCAP_SDK_URL} ${NPCAP_SDK_ZIP})
    endif()

    # Unzip Npcap SDK
    if(NOT EXISTS ${NPCAP_SDK_DIR})
        message(STATUS "Unzipping Npcap SDK...")
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${NPCAP_SDK_ZIP} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()

    include_directories("${CMAKE_BINARY_DIR}/Include")
    link_directories("${CMAKE_BINARY_DIR}/Lib/x64")
    set(PLATFORM_LIBS wpcap)

elseif(APPLE)
    set(PCAP_URL "https://www.tcpdump.org/release/libpcap-1.10.1.tar.gz")
    set(PCAP_TAR "${CMAKE_BINARY_DIR}/libpcap-1.10.1.tar.gz")
    set(PCAP_DIR "${CMAKE_BINARY_DIR}/libpcap-1.10.1")

    # Download Pcap library
    if(NOT EXISTS ${PCAP_TAR})
        message(STATUS "Downloading Pcap library...")
        file(DOWNLOAD ${PCAP_URL} ${PCAP_TAR})
    endif()

    # Extract Pcap library
    if(NOT EXISTS ${PCAP_DIR})
        message(STATUS "Extracting Pcap library...")
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PCAP_TAR} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()

    # Build libpcap from source if necessary
    if(NOT EXISTS "${PCAP_DIR}/libpcap.a")
        message(STATUS "Building libpcap...")
        add_custom_target(build_pcap ALL
            COMMAND ./configure --prefix=${PCAP_DIR} && make
            WORKING_DIRECTORY ${PCAP_DIR}
            COMMENT "Building libpcap"
        )
    else()
        add_custom_target(build_pcap ALL
            COMMAND ${CMAKE_COMMAND} -E echo "libpcap already built"
        )
    endif()

    # Include and link directories for libpcap
    include_directories("${PCAP_DIR}/include")
    link_directories("${PCAP_DIR}")

    # Use static library
    set(PCAP_LIBRARIES "${PCAP_DIR}/libpcap.a")
    set(PLATFORM_LIBS "-lpcap")

elseif(UNIX)
    if(EXISTS "/etc/debian_version")
        execute_process(COMMAND sudo apt-get update)
        execute_process(COMMAND sudo apt-get install -y libpcap-dev)
    endif()
    include_directories(/usr/include)
    link_directories(/usr/lib)
    set(PLATFORM_LIBS pcap)

else()
    include_directories(/usr/include)
    link_directories(/usr/lib)
    set(PLATFORM_LIBS pcap)
endif()

# Add FetchContent module for SQLite
include(FetchContent)

# Fetch SQLite amalgamation
FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3470200.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

# Make the content available
FetchContent_MakeAvailable(sqlite3)

# Create static library from sqlite3.c
add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

# Executable for sniffer
add_executable(sniffer ${SNIFFER_SOURCES})

if(APPLE)
    # Ensure sniffer depends on build_pcap
    add_dependencies(sniffer build_pcap)
endif()

# Link libraries (SQLite and platform-specific)
target_link_libraries(sniffer pthread sqlite3 ${PLATFORM_LIBS} ${PCAP_LIBRARIES})

if(WIN32)
    target_link_libraries(sniffer ws2_32)
endif()

# Generate Makefile
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)