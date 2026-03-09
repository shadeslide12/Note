#pragma once

#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <ctime>
#include "post_base_export.h"

namespace Utilities
{
    POST_BASE_API extern std::ostream *printstream;

    POST_BASE_API void set_print_stream(std::ostream &o);
    POST_BASE_API void print(const char *msg,
               bool if_root = false, bool time_stamp = false);
}
// Macros for external use

/// print on any rank
#define PRINT(msg) \
    Utilities::print(msg);

/// print on any rank with time stamp
#define PRINT_TIME(msg) \
    Utilities::print(msg, false, true);

/// print only on rank 0
#define PRINT_ROOT(msg) \
    Utilities::print(msg, true);

/// print on rank 0 with time stamp
#define PRINT_ROOT_TIME(msg) \
    Utilities::print(msg, true, true);
    