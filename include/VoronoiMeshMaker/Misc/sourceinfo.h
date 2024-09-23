//==============================================================================
// Name        : sourceinfo.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Class that provides information about the source of an error in a function.
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

#ifndef __VORONOIFVMAKER_SOURCEINFO_H__
#define __VORONOIFVMAKER_SOURCEINFO_H__

//==============================================================================
//  c++ include
//==============================================================================

#include <string_view>

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/Misc.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Class that provides information about the source of an error in a function.
 * 
 * This class stores the file location, line number, and function name where an error or
 * event occurred, making it easier to debug and trace.
 */
class SourceInfo {
public:
//==============================================================================
// Constructors / Destructor
//==============================================================================

    /**
     * @brief Default constructor.
     */
    constexpr SourceInfo() noexcept = default;

    /**
     * @brief Constructs a SourceInfo object with the provided file, line, and function information.
     * 
     * This constructor initializes the SourceInfo object with the location (file), line number, 
     * and the name of the function where the error or event occurred.
     * 
     * @param file A string view representing the file location where the error occurred.
     * @param line An integer representing the line number where the error occurred.
     * @param function A string view representing the name of the function where the error occurred.
     */
    constexpr SourceInfo(std::string_view file, int line, std::string_view function)
        : fileLocation(file), lineLocation(line), functionName(function) {}

//==============================================================================
// Inline functions to retrieve data
//==============================================================================

    /**
     * @brief Returns the file location where the error occurred.
     * 
     * @return A string view representing the file location.
     */
    [[nodiscard]] constexpr std::string_view FileLocation() const { return fileLocation; }

    /**
     * @brief Returns the line number where the error occurred.
     * 
     * @return An integer representing the line number.
     */
    [[nodiscard]] constexpr int LineNumber() const { return lineLocation; }

    /**
     * @brief Returns the function name where the error occurred.
     * 
     * @return A string view representing the function name.
     */
    [[nodiscard]] constexpr std::string_view FunctionName() const { return functionName; }

private:
//==============================================================================
// Private member variables (immutable)
//==============================================================================

    const std::string_view fileLocation;  //!< The file location where the error occurred
    const int lineLocation;               //!< The line number where the error occurred
    const std::string_view functionName;  //!< The name of the function where the error occurred
};

/**
 * @brief Function to capture current file, line, and function information.
 * 
 * This function captures the current file location, line number, and function name
 * using the predefined macros __FILE__, __LINE__, and __func__.
 * 
 * @return A SourceInfo object with the captured information.
 */
constexpr SourceInfo CaptureSourceInfo(const char* file, int line, const char* function);

VORMAKER_NAMESPACE_CLOSE

#endif  /*  __VORONOIFVMAKER_SOURCEINFO_H__ */
