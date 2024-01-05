# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SRC_FILES src/*.cpp)
include_directories(headers)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Building")
    add_executable(clipboard_server ${SRC_FILES})    
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

