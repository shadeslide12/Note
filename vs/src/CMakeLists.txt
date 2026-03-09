# Post module application
# Depends on: Qt5, VTK (with Qt support), post_base_lib

# ---- Find VTK (must have Qt support) ----
find_package(VTK REQUIRED)
if(NOT TARGET VTK::GUISupportQt)
    message(FATAL_ERROR "VTK was not built with Qt support (VTK::GUISupportQt not found)")
endif()

# ---- Determine Qt version and find it ----
if(NOT DEFINED VTK_QT_VERSION)
    set(VTK_QT_VERSION 5)
endif()

set(qt_components Core Gui Widgets)
if(${VTK_QT_VERSION} VERSION_GREATER_EQUAL 6)
    list(APPEND qt_components OpenGLWidgets)
endif()
list(SORT qt_components)

find_package(Qt${VTK_QT_VERSION} REQUIRED COMPONENTS ${qt_components})

foreach(_comp IN LISTS qt_components)
    list(APPEND qt_modules "Qt${VTK_QT_VERSION}::${_comp}")
endforeach()

message(STATUS "VTK_VERSION: ${VTK_VERSION}, Qt Version: ${Qt${VTK_QT_VERSION}Widgets_VERSION}")

# ---- Qt auto-processing ----
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# Do NOT use AUTORCC - we'll manually add resources to executable only
set(CMAKE_AUTORCC OFF)

# ---- Post library (static on Windows to avoid DLL init issues with Qt/VTK) ----
add_library(qt_post STATIC
    ColorBarDialog.cpp
    ColorMapPreset.cpp
    CutplaneDialog.cpp
    IsoSurfaceDialog.cpp
    CalculatorDialog.cpp
    MainWindow.cpp
    MeridionalPlaneDialog.cpp
    ConstHeightPlaneDialog.cpp
    SelectBoundaryDialog.cpp
    vtkAesReader.cpp
    vtkDisplayWindow.cpp
    ScaleFactorDialog.cpp
    ConstSettingDialog.cpp
    ControlPanel.cpp
    ProbePanel.cpp
    exportPicDialog.cpp
    PropertyPanel.cpp
)

target_include_directories(qt_post PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(Qt${VTK_QT_VERSION}Widgets_VERSION VERSION_LESS "5.11.0")
    qt5_use_modules(qt_post ${qt_components})
else()
    target_link_libraries(qt_post PUBLIC ${qt_modules})
endif()

target_link_libraries(qt_post PUBLIC ${VTK_LIBRARIES} post_base_lib)

# NOTE: Do NOT call vtk_module_autoinit on libraries, only on executables
# to avoid static initialization before main()

install(TARGETS qt_post
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# ---- Standalone executable ----
# Include resources directly in executable to ensure they're linked
add_executable(cfd_post 
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/resources.qrc
)
target_link_libraries(cfd_post PRIVATE qt_post)

# Manually enable AUTORCC only for the executable
set_target_properties(cfd_post PROPERTIES AUTORCC ON)

# Do NOT use vtk_module_autoinit - causes static init issues with Qt on Windows
# VTK modules will be manually imported in source files that use them

# ---- Post-build: Copy ColorMaps.json to executable directory ----
add_custom_command(TARGET cfd_post POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/ColorMaps.json
        $<TARGET_FILE_DIR:cfd_post>/ColorMaps.json
    COMMENT "Copying ColorMaps.json to output directory"
)

# ---- Windows: Deploy Qt plugins ----
if(WIN32)
    # Copy Qt platform plugin
    get_target_property(QT_QMAKE_EXECUTABLE Qt${VTK_QT_VERSION}::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    get_filename_component(QT_ROOT_DIR "${QT_BIN_DIR}" DIRECTORY)
    
    add_custom_command(TARGET cfd_post POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:cfd_post>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QT_ROOT_DIR}/plugins/platforms/qwindows.dll
            $<TARGET_FILE_DIR:cfd_post>/platforms/qwindows.dll
        COMMENT "Deploying Qt platform plugin"
    )
endif()

install(TARGETS cfd_post
    RUNTIME DESTINATION bin
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ColorMaps.json DESTINATION bin)
