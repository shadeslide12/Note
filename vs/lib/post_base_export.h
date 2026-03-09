#pragma once

// DLL export/import macro for post_base_lib
// - Windows: dllexport when building DLL, dllimport when consuming
// - Linux/macOS: expands to nothing (symbols exported by default)

#ifdef _WIN32
    #ifdef POST_BASE_EXPORTS
        #define POST_BASE_API __declspec(dllexport)
    #else
        #define POST_BASE_API __declspec(dllimport)
    #endif
#else
    #define POST_BASE_API
#endif
