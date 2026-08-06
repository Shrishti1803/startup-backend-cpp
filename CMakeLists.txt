cmake_minimum_required(VERSION 3.16)

# Allow CMAKE_MSVC_RUNTIME_LIBRARY to control the MSVC runtime
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Simplipal)

# ---------- C++ ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Windows ----------
if(WIN32)
    add_definitions(
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
    )
endif()

# ---------- Include ----------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ---------- Sources ----------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# ---------- MSVC Runtime ----------
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---------- Executable ----------
add_executable(app ${SOURCES})

# WINDOWS
if(WIN32)

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

    target_include_directories(app PRIVATE
        "${MYSQL_INCLUDE_DIRS}/jdbc"
    )

    target_link_libraries(app PRIVATE
        fmt::fmt
        spdlog::spdlog
        JsonCpp::JsonCpp
        unofficial-sodium::sodium
        unofficial::mysql-connector-cpp::connector
        unofficial::mysql-connector-cpp::connector-jdbc
        ws2_32
        bcrypt
    )

    target_compile_options(app PRIVATE /W4)

# ==========================================================
#                    LINUX (Ubuntu)
# ==========================================================
else()

    find_package(PkgConfig REQUIRED)

    find_package(fmt REQUIRED)
    find_package(spdlog REQUIRED)

    pkg_check_modules(JSONCPP REQUIRED jsoncpp)

    target_include_directories(app PRIVATE
        ${JSONCPP_INCLUDE_DIRS}
    )

    target_link_libraries(app PRIVATE
        fmt::fmt
        spdlog::spdlog
        ${JSONCPP_LIBRARIES}
        mysqlcppconn
        sodium
        pthread
    )

    target_compile_options(app PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -fdiagnostics-color=always
    )

endif()

# ---------- Output ----------
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)