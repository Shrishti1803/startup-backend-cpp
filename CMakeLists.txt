cmake_minimum_required(VERSION 3.16)

# Allow CMAKE_MSVC_RUNTIME_LIBRARY to control the MSVC runtime
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Simplipal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
)

# ---------- Include Directories ----------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ---------- Source Files ----------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# ---------- MSVC Runtime ----------
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---------- Executable ----------
add_executable(app ${SOURCES})

# ---------- Packages ----------
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

get_target_property(
    MYSQL_INCLUDE_DIRS
    unofficial::mysql-connector-cpp::connector-jdbc
    INTERFACE_INCLUDE_DIRECTORIES
)

message(STATUS "MYSQL_INCLUDE_DIRS=${MYSQL_INCLUDE_DIRS}")

get_target_property(
    MYSQL_INCLUDES
    unofficial::mysql-connector-cpp::connector-jdbc
    INTERFACE_INCLUDE_DIRECTORIES
)

message(STATUS "MYSQL_INCLUDES=${MYSQL_INCLUDES}")

# ---------- MySQL JDBC Include Directory ----------
target_include_directories(app PRIVATE
    "${MYSQL_INCLUDE_DIRS}/jdbc"
)

# ---------- Link Libraries ----------
target_link_libraries(app PRIVATE
    fmt::fmt
    spdlog::spdlog
    JsonCpp::JsonCpp
    unofficial-sodium::sodium

    ws2_32
    bcrypt
    unofficial::mysql-connector-cpp::connector
    unofficial::mysql-connector-cpp::connector-jdbc
)

# ---------- Compiler Warnings ----------
if(MSVC)
    target_compile_options(app PRIVATE /W4)
endif()

# ---------- Output ----------
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)