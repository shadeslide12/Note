// Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
// Modified by ZJUI Fluid lab
// SPDX-License-Identifier: (BSD-3-Clause)

#pragma once
#include <string>
#include "post_base_export.h"

namespace Utilities
{
    namespace File
    {

        /*!
         * \brief Returns the current working directory within the file system
         * \return path of cwd if found, else, returns "./"
         */
        POST_BASE_API std::string GetCWD();

        /*!
         * \brief Changes the current working directory within the file system
         * \param [in] dirName an absolute or relative directory path
         * \return Status code 0 for success, non-zero otherwise
         */
        POST_BASE_API int ChangeCWD(const std::string &dirName);

        /*!
         * \brief Checks if the path pointed to by fileName exists
         * \param [in] fileName name of the file to check
         * \return true if file system contains a file named fileName, false otherwise
         */
        POST_BASE_API bool PathExists(const std::string &fileName);

        /*!
         * \brief Joins two strings with the given separator char
         *
         * \param [in] fileDir The directory of the file
         * \param [in] fileName The name of the file
         * \param [in] separator a single character to seperate the two strings
         *
         * \note fileDir can be the empty string
         * \note fileName can include directories (e.g. a/b/c.txt), but should not be an
         * absolute path f a non-empty fileDir is supplied.
         *
         * \returns The concatenated string: fileDir + fileName, with separator in
         *  between, if necessary
         * \note Example1:  joinPath("abc", "def") -> "abc/def"
         * \note Example2:  joinPath("abc/", "def") -> "abc/def"
         * \note Example3:  joinPath("abc/", "def/ghi") -> "abc/def/ghi"
         */
        POST_BASE_API std::string JoinPath(const std::string &fileDir,
                             const std::string &fileName,
                             const std::string &separator = "/");

        /*!
         * \brief Make directories for a given path string
         *
         * \param [in] path  string representing an absolute or relative directory path
         *
         * Everything in the path is assumed to be intended to be a directory.  If
         * a directory in the path already exists, nothing is done.  If a directory
         * doesn't exist, it is created.
         */
        POST_BASE_API int MakeDirsForPath(const std::string &path, const std::string &separator = "/");

        /*!
         * \brief Get directory name from a path that contains a file name
         *
         * \param [out] dir  a directory path formed by removing the file name from
         *                   the input path
         * \param [in] path  an absolute or relative directory/file path
         *
         * This function assumes that the input path has a file name at the end, and
         * it removes that file name, leaving a string containing only a directory
         * path.
         *
         * For example, if the path string is "abc/def/ghi/file.txt", the output dir
         * string will be "abc/def/ghi".  If the path string is "file.txt", the
         * output dir string will be "" (the empty string).
         */
        POST_BASE_API std::string GetDirName(const std::string &path, const std::string &separator = "/");
    } // namespace File
}