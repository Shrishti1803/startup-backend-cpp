cmake_minimum_required(VERSION 3.16)

project(Simlipal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Collect all source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Create executable
add_executable(app ${SOURCES})

# Link external libraries
target_link_libraries(app
    mysqlcppconn
    sodium
    spdlog
    fmt
)