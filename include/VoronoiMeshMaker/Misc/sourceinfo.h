//==============================================================================
// Name        : sourceinfo.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Provides information about the source of an error in a function.
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
 * @file sourceinfo.h
 * @brief Provides information about the source of an error or event in a function.
 * 
 * This file defines the SourceInfo class, which captures and stores file, line, and function
 * information to aid in debugging and tracing errors in the library.
 * 
 * @ingroup misc
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIFVMAKER_SOURCEINFO_H__
#define __VORONOIFVMAKER_SOURCEINFO_H__

#include <string_view>
#include <VoronoiMeshMaker/Misc/Misc.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Class that stores the source of an error or event, including file location, line number, and function name.
 * 
 * This class provides a simple way to capture and access information about where an error or event 
 * occurred, including the file, line, and function name. It is primarily used for debugging and tracing.
 */
class SourceInfo {
public:
    /**
     * @brief Default constructor.
     * 
     * Initializes an empty SourceInfo object.
     */
    constexpr SourceInfo() noexcept = default;

    /**
     * @brief Constructs a SourceInfo object with file, line, and function details.
     * 
     * @param file The name of the file where the event occurred.
     * @param line The line number where the event occurred.
     * @param function The name of the function where the event occurred.
     */
    constexpr SourceInfo(std::string_view file, int line, std::string_view function)
        : fileLocation(file), lineLocation(line), functionName(function) {}

    /**
     * @brief Retrieves the file location of the error or event.
     * 
     * @return The name of the file.
     */
    [[nodiscard]] constexpr std::string_view FileLocation() const { return fileLocation; }

    /**
     * @brief Retrieves the line number of the error or event.
     * 
     * @return The line number.
     */
    [[nodiscard]] constexpr int LineNumber() const { return lineLocation; }

    /**
     * @brief Retrieves the function name of the error or event.
     * 
     * @return The function name.
     */
    [[nodiscard]] constexpr std::string_view FunctionName() const { return functionName; }

private:
    const std::string_view fileLocation;  //!< File location of the event.
    const int lineLocation;               //!< Line number of the event.
    const std::string_view functionName;  //!< Function name of the event.
};

/**
 * @brief Captures the current file, line, and function information.
 * 
 * This function uses predefined macros (`__FILE__`, `__LINE__`, `__func__`) to capture
 * the current source information and return a SourceInfo object.
 * 
 * @param file The name of the file.
 * @param line The line number.
 * @param function The name of the function.
 * @return A SourceInfo object with the captured information.
 */
constexpr SourceInfo CaptureSourceInfo(const char* file, int line, const char* function) {
    return SourceInfo(file, line, function);
}

VORMAKER_NAMESPACE_CLOSE

#endif  /*  __VORONOIFVMAKER_SOURCEINFO_H__ */
