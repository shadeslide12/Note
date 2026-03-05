cmake_minimum_required(VERSION 3.14)
project(CFDSolverGUI VERSION 1.0.0 LANGUAGES CXX)

# --- 1. 基础环境配置 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置源文件编码为 UTF-8
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-finput-charset=UTF-8>")
add_compile_options("$<$<CXX_COMPILER_ID:Clang>:-finput-charset=UTF-8>")

# --- 2. 寻找 Qt 和 Lua ---
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Test Charts)
find_package(Lua REQUIRED)

# --- 2.1 添加 Sol2 头文件路径 ---
include_directories(${CMAKE_SOURCE_DIR}/external/sol2)

# --- 3. 主程序逻辑 ---
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${LUA_INCLUDE_DIR})
set(SOURCES 
    src/main.cpp 
    src/cfd_solver.cpp 
    src/cfd_solver_thread.cpp 
    src/computation_controller.cpp 
    src/monitor_widget.cpp 
    src/main_window.cpp
    src/script_engine.cpp
    src/script_editor_widget.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Charts ${LUA_LIBRARIES})

# --- 4. 开启测试选项 ---
# enable_testing()
# add_subdirectory(tests) # 跳转到子目录，那里才负责下载测试框架