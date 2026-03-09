// Copyright (c) 2017-2022, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
// Modified by ZJUI fluid lab
// SPDX-License-Identifier: (BSD-3-Clause)

#include "Utilities/StringUtils.h"
#include "Utilities/ErrorUtils.h"
#include <algorithm>
#include <cctype>
namespace Utilities
{
    namespace String
    {
        std::vector<std::string> split(const std::string &str, const std::string &delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            if (!str.empty())
            {
                size_t pos = str.find(delimiter);
                size_t left_pos = 0;

                while (pos != std::string::npos)
                {
                    tokens.push_back(str.substr(left_pos, pos - left_pos));
                    left_pos = pos + delimiter.size();
                    pos = str.find(delimiter, left_pos);
                }
                tokens.push_back(str.substr(left_pos, pos));
            }
            return tokens;
        }

        void toLower(std::string &str)
        {
            std::transform(str.begin(), str.end(), str.begin(), [](const unsigned char c)
                           { return std::tolower(c); });
        }

        void toUpper(std::string &str)
        {
            std::transform(str.begin(), str.end(), str.begin(), [](const unsigned char c)
                           { return std::toupper(c); });
        }

        std::vector<std::string> rsplitN(const std::string &input,
                                         const std::size_t n,
                                         const std::string &delim)
        {
            std::vector<std::string> result;

            auto last_pos = input.size();
            auto pos = input.rfind(delim, last_pos - 1);

            if (n > 0 && !input.empty())
            {
                while ((pos != std::string::npos) && (result.size() < n - 1))
                {
                    result.push_back(input.substr(pos + delim.size(), last_pos - pos - delim.size()));
                    last_pos = pos;
                    if (last_pos == 0)
                        break;
                    pos = input.rfind(delim, last_pos - 1);
                }

                // Add the rest of the string (first token)
                result.push_back(input.substr(0, last_pos));
                std::reverse(result.begin(), result.end());
            }

            return result;
        }

        std::string appendPrefix(const std::string &prefix,
                                 const std::string &name,
                                 const std::string &delim)
        {
            if (prefix.empty() || name.empty())
            {
                return name;
            }
            else
            {
                return prefix + delim + name;
            }
        }

        std::string removePrefix(const std::string &prefix,
                                 const std::string &name,
                                 const std::string &delim)
        {
            if (prefix.empty())
            {
                return name;
            }
            else if (startsWith(name, prefix + delim))
            {
                return name.substr(prefix.size() + delim.size());
            }
            return name;
        }

        std::string removeBeforeDelimiter(const std::string &str, const std::string &delim)
        {
            auto pos = str.rfind(delim);
            if (pos == std::string::npos)// if not found the delimiter, return the full string
                return str;
            return str.substr(pos + delim.size());
        }

        std::string removeAllInstances(const std::string &target,
                                       const std::string &substr)
        {
            std::string result = target;
            auto pos = result.find(substr);
            while (pos != std::string::npos)
            {
                result.erase(pos, substr.length());
                pos = result.find(substr);
            }
            return result;
        }

        std::string removeAfterDelimiter(const std::string &str, const std::string &delim)
        {
            auto pos = str.find(delim);
            if (pos == std::string::npos)// if not found the delimiter, return the full string
                return str;
            return str.substr(0, pos);
        }

        std::string removeHeadSpaces(const std::string &str)
        {
            for (size_t i = 0; i < str.size(); i++)
            {
                if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r')
                    return str.substr(i);
            }
            return "";
        }

        std::string removeTailSpaces(const std::string &str)
        {
            if(str.size()==0)
            return "";
            for (size_t i = str.size() - 1; i >= 0; i--)
            {
                if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r')
                    return str.substr(0, i + 1);
            }
            return "";
        }

    } // end namespace String
} // end namespace Utilities