# CFD Post-processing Standalone (zjui-post)

从 `zjui-cfd-pre-wsl` 项目中拆分出的独立 Post 后处理模块，支持 Linux 和 Windows 跨平台编译。

## 项目结构

```
zjui-post/
├── CMakeLists.txt          # 顶层 CMake（跨平台）
├── lib/                    # 轻量基础库 (post_base_lib)
│   ├── CMakeLists.txt      # 仅依赖 HDF5
│   └── Utilities/          # ErrorUtils, PrintUtils, FileUtils, StringUtils, H5 封装, ndarray
├── src/                    # Post 模块源码
│   ├── CMakeLists.txt      # 依赖 Qt5 + VTK + post_base_lib
│   ├── main.cpp            # 程序入口
│   ├── MainWindow.*        # 主窗口
│   ├── vtkDisplayWindow.*  # VTK 渲染管理
│   ├── vtkAesReader.*      # HDF5 数据读取
│   ├── ColorMaps.json      # 颜色映射预设
│   └── ...                 # 各种对话框和面板
└── resources/              # Qt 资源文件
    ├── resources.qrc
    └── PostIcons/          # 图标 (20 个 PNG)
```

## 依赖

| 库 | 用途 | 备注 |
|---|---|---|
| **CMake** ≥ 3.16 | 构建系统 | |
| **Qt 5** (Core, Gui, Widgets) | GUI 框架 | 建议 5.15.x |
| **VTK 9.1.0** (带 Qt 支持) | 3D 可视化 | 必须自行编译，开启 `VTK_GROUP_ENABLE_Qt` |
| **HDF5** (C) | 数据读取 | 不需要 MPI |

**不需要**: CGNS, yaml-cpp, OpenBLAS, MPI（原项目的这些依赖已被剥离）

## Linux 编译

```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="/path/to/qt5;/path/to/vtk-9.1.0"
make -j$(nproc)
./bin/cfd_post
```

## Windows 编译 (MSVC)

```powershell
mkdir build; cd build
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_PREFIX_PATH="C:/Qt/5.15.2/msvc2019_64;C:/vtk-9.1.0-install" `
  -DHDF5_ROOT="C:/HDF5"
cmake --build . --config Release --parallel 8
.\bin\Release\cfd_post.exe
```

## 与原项目的差异

| 项目 | 原项目 (zjui-cfd-pre-wsl) | 本项目 (zjui-post) |
|---|---|---|
| 基础库 | `PRE_BASE_LIB` (CGNS+HDF5+yaml-cpp+OpenBLAS) | `post_base_lib` (仅 HDF5) |
| 外部依赖数 | 4~5 个 | 1 个 |
| 编译器标志 | GCC-only (`-fPIC`, `$ENV{CC}`) | 跨平台 (MSVC/GCC/Clang) |
| Debug backtrace | Linux-only (`execinfo.h`) | 已加 `!_WIN32` 守护 |
| 硬编码路径 | 存在多处 | 已清理 |
| 文件名大小写 | `ExportPicDialog.h` 不一致 | 已修正 |
