//==============================================================================
// Name        : misc.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with utility definitions used across the library.
//               Part of the 'misc' group of utility files.
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

/**
 * @file misc.h
 * @brief Provides general-purpose utility functions for the VoronoiMeshMaker library.
 * 
 * This file defines utility functions used throughout the VoronoiMeshMaker library,
 * including functions for printing lines, manipulating strings, and handling file paths.
 * These functions are part of the "misc" group of utility components.
 * 
 * @ingroup misc
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOMESHVMAKER_MISC_H__
#define __VORONOMESHVMAKER_MISC_H__

#include <filesystem>
#include <VoronoiMeshMaker/Misc/configure.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Prints a horizontal line of a specified length to the output stream.
 * 
 * This function prints a line made of the '-' character to the given output stream.
 * The length of the line can be specified.
 * 
 * @param os The output stream where the line will be printed.
 * @param length The number of characters in the line. Defaults to `LSIZE`.
 */
void PrintLine(std::ostream& os, const unsigned& length = LSIZE);

/**
 * @ingroup misc
 * @brief Converts a string to a boolean value.
 * 
 * This function attempts to convert a string representation of a boolean value 
 * (e.g., "true", "false", "1", "0") into a boolean.
 * 
 * @param str The string to be converted.
 * @return True if the string represents a true value, false otherwise.
 */
[[nodiscard]] bool string2bool(const std::string& str);

/**
 * @ingroup misc
 * @brief Gets the current working directory.
 * 
 * This function retrieves the current working directory using the `std::filesystem` library.
 * 
 * @return A `std::filesystem::path` object representing the current working directory.
 */
[[nodiscard]] std::filesystem::path getCurrentWorkingDirectory();

/**
 * @ingroup misc
 * @brief Converts a relative or absolute path to its full, absolute form.
 * 
 * This function takes a relative or absolute path and converts it to an absolute path.
 * 
 * @param path The path to be converted.
 * @return The full path as a string.
 */
[[nodiscard]] std::string getFullPath(const std::string& path);

/**
 * @ingroup misc
 * @brief Renames a file from its old name to a new name.
 * 
 * This function renames a file in the filesystem by changing its current name to a new one.
 * 
 * @param oldName The current name of the file.
 * @param newName The new name for the file.
 * @return The new name of the file as a string.
 */
[[nodiscard]] std::string RenameFile(const std::string& oldName, const std::string& newName);

/**
 * @ingroup misc
 * @brief Creates a directory if it does not already exist.
 * 
 * This function checks if a directory exists at the given path. If the directory does not exist,
 * it is created.
 * 
 * @param path The path where the directory should be created.
 * @return True if the directory was successfully created or already exists, false otherwise.
 */
[[nodiscard]] bool CreateDirectoryIfNotExists(const std::string& path);

/**
 * @ingroup misc
 * @brief Converts a string to an unsigned integer using a hash function.
 * 
 * This function implements a simple hash function to convert a string to an unsigned integer.
 * It is primarily used for quick lookups and classifications.
 * 
 * @param _str The C-style string to convert.
 * @param _h The current hash value, default is 0.
 * @return The computed hash value as an unsigned integer.
 */
constexpr unsigned int str2int(const char* _str, const int& _h = 0) {
    return !_str[_h] ? 5381 : (str2int(_str, _h + 1) * 33) ^ _str[_h];
}

/**
 * @ingroup misc
 * @brief Checks if a string contains only whitespace.
 * 
 * This function returns true if the given string contains only whitespace characters.
 * 
 * @param str The string to check.
 * @return True if the string is blank, false otherwise.
 */
bool IsBlank(const std::string& str);

/**
 * @ingroup misc
 * @brief Checks if a string is empty or contains only whitespace.
 * 
 * This function returns true if the given string is either empty or contains only whitespace characters.
 * 
 * @param str The string to check.
 * @return True if the string is empty or blank, false otherwise.
 */
bool IsEmptyOrBlank(const std::string& str);

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_MISC_H__
