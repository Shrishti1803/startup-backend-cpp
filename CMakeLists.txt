cmake_minimum_required(VERSION 3.16)

project(Simplipal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional if custom libs installed
set(CMAKE_PREFIX_PATH "/usr/local")

# ---------- Include Directories ----------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/jsoncpp
)

# ---------- Collect Source Files ----------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# ---------- Create Executable ----------
add_executable(app ${SOURCES})

# ---------- Find Packages ----------
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# ---------- Include JSONCPP ----------
target_include_directories(app PRIVATE
    ${JSONCPP_INCLUDE_DIRS}
)

# ---------- Link Libraries ----------
target_link_libraries(app PRIVATE
    ${JSONCPP_LIBRARIES}
    mysqlcppconn
    sodium
    pthread
    fmt
)

# ---------- Compiler Warnings ----------
target_compile_options(app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -fdiagnostics-color=always
)

# ---------- Output Binary ----------
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)