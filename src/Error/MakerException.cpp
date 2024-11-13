//==============================================================================
// Name        : MakerException.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 2.0
// Description : Implementation of the MakerException class for error handling
//               in the VoronoiMeshMaker library. Part of the 'error' group for
//               robust error management.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//==============================================================================

/**
 * @file MakerException.cpp
 * @brief Implements the MakerException class for robust error handling in the
 *        VoronoiMeshMaker library.
 *
 * The MakerException class captures detailed information about errors,
 * including file, line, and function where they occurred, and supports custom
 * error messages and handling mechanisms. This file provides the implementation
 * of methods to trace and retrieve error information, including the execution
 * path to the error.
 *
 * @ingroup error
 * @version 2.0
 * @date 2024
 * @author
 * Joao Flavio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Standard C++ Library Includes
//==============================================================================

#include <format>
#include <string_view>

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================

#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Error/MakerLogger.h>
#include <VoronoiMeshMaker/Misc/Misc.h>

VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Static Member Definitions
//==============================================================================

/**
 * @brief Provides a map of error codes to corresponding messages.
 *
 * This function returns a static map that associates error codes
 * (`MakerErrorCode`) with their respective error messages (`std::string_view`).
 *
 * @return A reference to the map containing error codes and messages.
 */
const std::unordered_map<MakerErrorCode, std::string_view>& MakerException::errorCodeMap() {
    static const std::unordered_map<MakerErrorCode, std::string_view> errorMap = {
        {MakerErrorCode::FileNotFound, "File Not Found"},
        {MakerErrorCode::InvalidPolygon, "Invalid Polygon"},
        {MakerErrorCode::NullPointer, "Null Pointer"},
        // Add other error codes and messages here
    };
    return errorMap;
}

//==============================================================================
// Constructor
//==============================================================================

/**
 * @brief Constructs a new MakerException object with an error code, message,
 *        and source location.
 *
 * This constructor initializes an instance of MakerException with a given
 * error code, a custom message, and the source location where the exception
 * was thrown.
 *
 * @param errorCode The type of error (based on `MakerErrorCode`).
 * @param message A custom error message providing additional details.
 * @param location The source location where the exception was created
 *                 (default: current location).
 */
MakerException::MakerException(const MakerErrorCode& errorCode,
                               const std::string_view message,
                               const std::source_location& location)
    : errorCode_(errorCode), message_(message), location_(location) {
    formatMessage();
    logException();
}

//==============================================================================
// Public Functions
//==============================================================================

/**
 * @brief Provides a description of the exception.
 *
 * This function returns a C-style string containing the full error message,
 * which includes the error code, message, file, function, and line.
 *
 * @return A C-style string representing the full error message.
 */
const char* MakerException::what() const noexcept {
    return fullMessage_.c_str();
}

/**
 * @brief Gets the error code associated with this exception.
 *
 * This function returns the `MakerErrorCode` that represents the type of error.
 *
 * @return The error code as a `MakerErrorCode`.
 */
MakerErrorCode MakerException::errorCode() const noexcept {
    return errorCode_;
}

/**
 * @brief Gets the source location where the exception was thrown.
 *
 * This function returns the source location (`std::source_location`) where the
 * exception was created, providing information about the file, function, and
 * line number.
 *
 * @return A reference to the `std::source_location` where the exception occurred.
 */
const std::source_location& MakerException::location() const noexcept {
    return location_;
}

//==============================================================================
// Private Functions
//==============================================================================

/**
 * @brief Formats the full error message, including error code, message, and
 *        source location.
 *
 * This function creates the full error message that includes details such as
 * the error code, custom message, file name, function name, and line number.
 * The formatted message is stored in `fullMessage_`.
 */
void MakerException::formatMessage() {
    std::string errorCodeStr;
    auto it = errorCodeMap().find(errorCode_);
    if (it != errorCodeMap().end()) {
        errorCodeStr = it->second;
    } else {
        errorCodeStr = "Unknown Error";
    }

    fullMessage_ = std::format("Error: {}\nMessage: {}\nFile: {}\nFunction: {}\nLine: {}\n",
                               errorCodeStr,
                               message_,
                               getFileName(location_.file_name()),
                               location_.function_name(),
                               location_.line());
}

/**
 * @brief Logs the exception details using MakerLogger.
 *
 * This function logs the full error message using the `MakerLogger` and also
 * logs the execution path to provide context about the program flow leading to
 * the exception.
 */
void MakerException::logException() const {
    // Logs the detailed exception message
    MakerLogger::error(fullMessage_, location_);

    // Retrieves and logs the execution path
    std::string breadcrumbTrail = MakerLogger::getBreadcrumbTrail();
    MakerLogger::error(breadcrumbTrail, location_);
}

VORMAKER_NAMESPACE_CLOSE
