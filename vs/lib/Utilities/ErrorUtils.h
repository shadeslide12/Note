#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include "post_base_export.h"
#ifdef HAVE_MPI
#include "mpi.h"
#endif

namespace Utilities
{
#if defined(_DEBUG) && !defined(_WIN32)
    POST_BASE_API std::string Backtrace(int skip = 1);
#endif

    class POST_BASE_API ErrorUtils
    {
    public:
        enum ERROR_TYPE
        {
            efatal,
            ewarning
        };

        inline static void SetErrorStream(std::ostream &o)
        {
            m_errorstream = &o;
        }

        /**
         * @brief Main error function
         *
         * @param type type of error
         * @param file file name
         * @param lineNumber line number
         * @param msg error message
         * @param if_root if the error occurs on all the ranks
         */
        inline static void Error(ERROR_TYPE type,
                                 const char *file,
                                 int lineNumber,
                                 const char *msg,
                                 bool if_root = false)
        {
            std::string baseMsg;
            int rank = 0;
            int flag = 0;
#ifdef HAVE_MPI
            MPI_Initialized(&flag);
            if (if_root && flag)
            {
                MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            }
#endif
            // compose error message
            baseMsg = "File: " + std::string(file) + " Line: " + std::to_string(lineNumber);
            baseMsg += "\nMessage: " + std::string(msg);

            switch (type)
            {
            case ERROR_TYPE::efatal:
                if (!rank)
                {
                    (*m_errorstream) << "Fatal error: " << baseMsg << std::endl;
#if defined(_DEBUG) && !defined(_WIN32)
                    (*m_errorstream) << Backtrace() << std::endl;
#endif
                }
                if (!flag)
                    exit(EXIT_FAILURE);
#ifdef HAVE_MPI
                else
                    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
#endif
                break;
            case ERROR_TYPE::ewarning:
                if (!rank)
                {
                    (*m_errorstream) << "Warning: " << baseMsg << std::endl;
                }
                break;
            default:
                (*m_errorstream) << "Unknown exception: " << baseMsg << std::endl;
            }
        }

    private:
        static std::ostream *m_errorstream;
    };
}
// Macros for external use

// random error that can rise on any rank
#define FATAL_ERROR(msg) \
    Utilities::ErrorUtils::Error(Utilities::ErrorUtils::ERROR_TYPE::efatal, __FILE__, __LINE__, msg);
// error that rises on all ranks
#define FATAL_ERROR_ROOT(msg) \
    Utilities::ErrorUtils::Error(Utilities::ErrorUtils::ERROR_TYPE::efatal, __FILE__, __LINE__, msg, true);
#define ASSERT(condition, msg)                                               \
    if (!(condition))                                                        \
    {                                                                        \
        Utilities::ErrorUtils::Error(                                            \
            Utilities::ErrorUtils::ERROR_TYPE::efatal, __FILE__, __LINE__, msg); \
    }
#define WARNING(condition, msg)                                                \
    if (!(condition))                                                          \
    {                                                                          \
        Utilities::ErrorUtils::Error(                                              \
            Utilities::ErrorUtils::ERROR_TYPE::ewarning, __FILE__, __LINE__, msg); \
    }
    