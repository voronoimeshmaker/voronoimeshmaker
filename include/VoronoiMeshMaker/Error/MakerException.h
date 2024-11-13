#ifndef __VORMAKER_EXCEPTION_H__
#define __VORMAKER_EXCEPTION_H__

//==============================================================================
// Name        : MakerException.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 2.0
// Description : Exception handling for the VoronoiMeshMaker library.
//               Part of the 'error' group for robust error management.
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
 * @file MakerException.h
 * @brief Defines the MakerException class, providing robust error-handling
 *        capabilities for the VoronoiMeshMaker library.
 *
 * The MakerException class captures detailed information about errors,
 * including the file, line, and function where they occurred, and supports
 * custom error messages and handling mechanisms. This class is part of the
 * 'error' group in the VoronoiMeshMaker library.
 *
 * @ingroup error
 *
 * @version 2.0
 * @date 2024
 * @author
 * Joao Flavio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Standard C++ Library Includes
//==============================================================================

#include <unordered_map>
#include <source_location>
#include <string>


//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>

//==============================================================================
// Namespace Declaration
//==============================================================================

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup error
 * @enum MakerErrorCode
 * @brief Enumeration of possible error codes in the VoronoiMeshMaker library.
 *
 * This enum represents the various types of errors that can occur in the
 * VoronoiMeshMaker library.
 */
enum class MakerErrorCode {
    FileNotFound = 0,
    InvalidPolygon,
    NullPointer,
    // Add other error codes as needed
};

/**
 * @ingroup error
 * @class MakerException
 * @brief Exception class for error handling in the VoronoiMeshMaker library.
 *
 * This class captures detailed information about errors, including the
 * function, file, and line where the exception was thrown. It supports custom
 * error messages and integrates with MakerLogger to provide a trace of the
 * program flow leading to the error.
 */
class MakerException : public std::exception {

public:
    /**
     * @brief Constructor to create an exception with an error code.
     *
     * @param errorCode The type of error (based on MakerErrorCode).
     * @param message A custom error message to provide additional information.
     * @param location The source location of the error (default: current
     *                 location).
     */
    MakerException(const MakerErrorCode& errorCode,
                   const std::string_view message = "",
                   const std::source_location& location =
                       std::source_location::current());

    /**
     * @brief Gets a description of the exception.
     *
     * @return A C-style string containing the error message.
     */
    virtual const char* what() const noexcept override;

    /**
     * @brief Gets the error code.
     *
     * @return The MakerErrorCode associated with this exception.
     */
    MakerErrorCode errorCode() const noexcept;

    /**
     * @brief Gets the source location where the exception was thrown.
     *
     * @return A reference to the source location object.
     */
    const std::source_location& location() const noexcept;

private:
    MakerErrorCode errorCode_;       ///< Enum representing the type of error.
    std::string message_;            ///< Custom error message.
    std::source_location location_;  ///< Source location of the error.
    std::string fullMessage_;        ///< Formatted full error message.

    /**
     * @brief Formats the full error message.
     */
    void formatMessage();

    /**
     * @brief Logs the exception using MakerLogger.
     */
    void logException() const;

    /**
     * @brief Returns the mapping of error codes to their string representations.
     *
     * @return A reference to the unordered_map containing error codes and
     *         their messages.
     */
    static const std::unordered_map<MakerErrorCode, std::string_view>& errorCodeMap();
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORMAKER_EXCEPTION_H__
