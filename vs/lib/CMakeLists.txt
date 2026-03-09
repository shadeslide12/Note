# Lightweight base library for Post module
# Only depends on HDF5 (no CGNS, yaml-cpp, OpenBLAS)
add_library(post_base_lib)

# ---- HDF5 (the only external C library dependency) ----
find_package(HDF5 REQUIRED COMPONENTS C)
target_include_directories(post_base_lib PUBLIC ${HDF5_C_INCLUDE_DIRS})
target_compile_definitions(post_base_lib PUBLIC ${HDF5_C_DEFINITIONS})
target_compile_definitions(post_base_lib PRIVATE POST_BASE_EXPORTS)
target_link_libraries(post_base_lib PUBLIC ${HDF5_C_LIBRARIES})

# ---- Source files ----
target_sources(post_base_lib PRIVATE
    Utilities/ErrorUtils.cpp
    Utilities/PrintUtils.cpp
    Utilities/FileUtils.cpp
    Utilities/StringUtils.cpp
    Utilities/H5/H5ObjectBase.cpp
    Utilities/H5/H5File.cpp
    Utilities/H5/H5PropertyList.cpp
    Utilities/H5/H5Dataset.cpp
    Utilities/H5/H5Dataspace.cpp
    Utilities/H5/H5Datatype.cpp
    Utilities/H5/H5Group.cpp
    Utilities/H5/H5Attribute.cpp
)

# ---- Public include path ----
target_include_directories(post_base_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Install ----
install(TARGETS post_base_lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
