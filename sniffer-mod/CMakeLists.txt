cmake_minimum_required(VERSION 3.10)

# Project Information
project(SnifferModule VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include /opt/homebrew/include)

# Only include macOS-specific settings
if(APPLE)
  # Set the compiler flags for macOS
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -stdlib=libc++ -I/opt/homebrew/include")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpcap -lc++abi")
else()
  message(FATAL_ERROR "This project is configured to build only for macOS.")
endif()

# Define source files for the sniffer executable
set(SNIFFER_SOURCES src/db.cpp src/flow.cpp src/ip.cpp src/main.cpp
                    src/packet.cpp src/sniffer.cpp)

# Add the sniffer executable
add_executable(sniffer ${SNIFFER_SOURCES})

# Link libraries to the sniffer executable
target_link_libraries(sniffer pthread sqlite3 pcap)
