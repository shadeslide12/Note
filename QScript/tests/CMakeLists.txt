include(FetchContent)

# --- 1. 下载 Google Test ---
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# --- 2. 下载 RapidCheck ---
FetchContent_Declare(
  rapidcheck
  GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
  GIT_TAG master
)

# 一次性使它们可用
FetchContent_MakeAvailable(googletest rapidcheck)

# --- 3. 定义测试可执行文件 ---
set(TEST_SOURCES
    test_main.cpp
    unit/test_cfd_solver.cpp
    unit/test_cfd_solver_thread.cpp
    unit/test_computation_controller.cpp
    unit/test_monitor_widget.cpp
    unit/test_main_window.cpp
    property/test_properties_solver.cpp
)

add_executable(cfd_solver_tests ${TEST_SOURCES})

# 链接业务代码（直接引入源文件）
target_sources(cfd_solver_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/cfd_solver.cpp
    ${CMAKE_SOURCE_DIR}/src/cfd_solver_thread.cpp
    ${CMAKE_SOURCE_DIR}/src/computation_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/monitor_widget.cpp
    ${CMAKE_SOURCE_DIR}/src/main_window.cpp
)
target_include_directories(cfd_solver_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# --- 4. 添加 RapidCheck gtest extras 头文件路径 ---
target_include_directories(cfd_solver_tests PRIVATE 
    ${rapidcheck_SOURCE_DIR}/extras/gtest/include
)

# --- 5. 链接测试库 ---
# 注意：FetchContent 产生的库名通常是 gtest 和 rapidcheck
target_link_libraries(cfd_solver_tests
    PRIVATE
    gtest
    rapidcheck
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::Test
)

# --- 5. 注册到 CMake 测试系统 ---
include(GoogleTest)
gtest_discover_tests(cfd_solver_tests)

# 开启宏定义，方便在代码中判断
target_compile_definitions(cfd_solver_tests PRIVATE HAVE_RAPIDCHECK)