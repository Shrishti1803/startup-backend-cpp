cmake_minimum_required(VERSION 3.16)

# Allow CMAKE_MSVC_RUNTIME_LIBRARY to control the MSVC runtime
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Simplipal)

# ---------- C++ Standard ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Windows Definitions ----------
if(WIN32)
    add_definitions(
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
    )
endif()

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
    # Match the static runtime used by vcpkg (x64-windows-static)
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

# ---------- Windows-only MySQL JDBC Include ----------
if(WIN32)

    get_target_property(
        MYSQL_INCLUDE_DIRS
        unofficial::mysql-connector-cpp::connector-jdbc
        INTERFACE_INCLUDE_DIRECTORIES
    )

    message(STATUS "MYSQL_INCLUDE_DIRS=${MYSQL_INCLUDE_DIRS}")

    target_include_directories(app PRIVATE
        "${MYSQL_INCLUDE_DIRS}/jdbc"
    )

endif()

# ---------- Link Libraries ----------
target_link_libraries(app PRIVATE
    fmt::fmt
    spdlog::spdlog
    JsonCpp::JsonCpp
    unofficial-sodium::sodium
    unofficial::mysql-connector-cpp::connector
    unofficial::mysql-connector-cpp::connector-jdbc
)

# ---------- Windows System Libraries ----------
if(WIN32)
    target_link_libraries(app PRIVATE
        ws2_32
        bcrypt
    )
endif()

# ---------- Compiler Warnings ----------
if(MSVC)
    target_compile_options(app PRIVATE /W4)
endif()

# ---------- Output ----------
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)