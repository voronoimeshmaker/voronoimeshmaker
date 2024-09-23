//==============================================================================
// Name        : misc.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with definitions that will be used by the library.
//               This file is part of the 'misc' group of utility files.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

#ifndef __VORONOMESHVMAKER_MISC_H__
#define __VORONOMESHVMAKER_MISC_H__


//==============================================================================
//  c++ include
//==============================================================================

#include <filesystem>

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/configure.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Prints a line to the specified output stream.
 * 
 * @param[out] os The output stream to print to.
 * @param[in] length The number of characters in the line. Default is `LSIZE`.
 */
void PrintLine (std::ostream& os, const unsigned& length = LSIZE);

/**
 * @ingroup misc
 * @brief Converts a string to a boolean value.
 * 
 * @param[in] str The string to convert.
 * @return The boolean value corresponding to the string.
 */
[[nodiscard]] bool string2bool(const std::string& str);

/**
     * @brief Gets the current working directory.
     * @return A std::filesystem::path representing the current working directory.
     */
    [[nodiscard]] std::filesystem::path getCurrentWorkingDirectory();

/**
 * @ingroup misc
 * @brief Gets the full path of a given file or directory.
 * 
 * @param[in] path The relative or absolute path to be converted.
 * @return The full path as a string.
 */
[[nodiscard]] std::string getFullPath(const std::string& path);

/**
 * @ingroup misc
 * @brief Renames a file from the old name to the new name.
 * 
 * @param[in] oldName The current name of the file.
 * @param[in] newName The new name for the file.
 * @return The new name of the file.
 */
[[nodiscard]] std::string RenameFile(const std::string& oldName, const std::string& newName);

[[nodiscard]] bool CreateDirectoryIfNotExists(const std::string& path);

//============================================================================
// Important Functions
//============================================================================

/**
 * @ingroup misc
 * @brief Converts a string to an unsigned integer using a hash function.
 * 
 * This function uses a simple hash function to convert a C-style string to an
 * unsigned integer.
 * 
 * @param[in] _str The C-style string to convert.
 * @param[in] _h The current hash value. Default is 0.
 * @return The computed hash value as an unsigned integer.
 */
constexpr unsigned int str2int(const char* _str, const int& _h = 0)
{
    return !_str[_h] ? 5381 : (str2int(_str, _h + 1) * 33) ^ _str[_h];
}

bool IsBlank(const std::string& str);
bool IsEmptyOrBlank(const std::string& str);

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_MISC_H__
