# CFD Solver GUI

A Qt5-based graphical user interface for controlling and monitoring CFD (Computational Fluid Dynamics) solver computations.

## Requirements

- CMake 3.10+
- C++17 compatible compiler (GCC 7+, Clang 6+, or MSVC 2017+)
- Qt5 (Qt 5.12+)
  - QtCore
  - QtWidgets
- Google Test
- RapidCheck (optional, for property-based testing)

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running Tests

```bash
cd build
ctest
```

## Project Structure

```
.
├── CMakeLists.txt          # Root CMake configuration
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── computation_state.h
│   ├── computation_progress.h
│   ├── solver_configuration.h
│   └── error_info.h
└── tests/                  # Test files
    ├── CMakeLists.txt
    └── unit/
        └── test_main.cpp
```

## Architecture

The system uses a Model-View-Controller architecture with Qt's signal-slot mechanism for component communication. Computation runs in a separate worker thread to keep the GUI responsive.

See `.kiro/specs/cfd-solver-gui/design.md` for detailed architecture documentation.
