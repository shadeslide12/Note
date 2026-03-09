# CFD Post-processing Standalone Project
cmake_minimum_required(VERSION 3.16)
project(CFD_POST C CXX)

set(CMAKE_CXX_STANDARD 17)  # 从 11 改为 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Cross-platform compiler flags ----
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_options(/W3)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-D_DEBUG)
    endif()
else()
    add_compile_options(-Wall -Wextra)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g -ggdb)
        add_definitions(-D_DEBUG)
    else()
        add_compile_options(-O3)
    endif()
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---- Build lightweight base library (HDF5 wrapper) ----
add_subdirectory(lib)

# ---- Build Post application ----
add_subdirectory(src)
