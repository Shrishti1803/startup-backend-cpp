cmake_minimum_required(VERSION 3.16)

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
    "${CMAKE_SOURCE_DIR}/mysql-connector-c++-9.7.0-winx64/include/jdbc"
)

# ---------- Library Directories ----------
link_directories(
    "${CMAKE_SOURCE_DIR}/mysql-connector-c++-9.7.0-winx64/lib64/vs14"
)
# ---------- Source Files ----------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# ---------- Executable ----------
add_executable(app ${SOURCES})

# ---------- Packages ----------
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)

# ---------- Link Libraries ----------
target_link_libraries(app PRIVATE
    fmt::fmt
    spdlog::spdlog
    JsonCpp::JsonCpp
    unofficial-sodium::sodium

    ws2_32
    bcrypt

    mysqlcppconn
    libssl
    libcrypto
)

# ---------- Compiler Warnings ----------
if(MSVC)
    target_compile_options(app PRIVATE /W4)
endif()

# ---------- Output ----------
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
