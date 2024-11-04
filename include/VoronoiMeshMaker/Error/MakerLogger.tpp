//==============================================================================
// Name        : MakerLogger.tpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.1
// Description : Template implementations for the MakerLogger class.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License.
//==============================================================================

/**
 * @file MakerLogger.tpp
 * @brief Implementations of the templates for the MakerLogger class.
 *
 * This file contains the implementations of the template functions declared in MakerLogger.h.
 *
 * @ingroup error
 * @version 2.1
 * @date 2024
 */

//==============================================================================
// Boost Includes
//==============================================================================
#include <boost/log/trivial.hpp>

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================
#include <VoronoiMeshMaker/Misc/Misc.h>

//==============================================================================
// Namespace Declaration
//==============================================================================

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Logs an informational message.
 * 
 * This function logs an informational message, including the source file, function name,
 * and line number where the log was generated. It also adds a breadcrumb to track program flow.
 * 
 * @tparam T The type of the message to be logged.
 * @param message The message to be logged.
 * @param location The source location where the log is generated (default: current location).
 */
template <typename T>
void MakerLogger::log(T message, const std::source_location& location) {
    BOOST_LOG_TRIVIAL(info) << "[File: " << getFileName(location.file_name())
                            << ", Function: " << location.function_name()
                            << ", Line: " << location.line()
                            << "] " << message;
    addBreadcrumb(message, location);
}

/**
 * @brief Logs an error message.
 * 
 * This function logs an error message, including the source file, function name,
 * and line number where the error was generated. It also adds a breadcrumb to track program flow.
 * 
 * @tparam T The type of the message to be logged.
 * @param message The error message to be logged.
 * @param location The source location where the log is generated (default: current location).
 */
template <typename T>
void MakerLogger::error(T message, const std::source_location& location) {
    BOOST_LOG_TRIVIAL(error) << "[File: " << getFileName(location.file_name())
                             << ", Function: " << location.function_name()
                             << ", Line: " << location.line()
                             << "] " << message;
    addBreadcrumb(message, location);
}

/**
 * @brief Logs a warning message.
 * 
 * This function logs a warning message, including the source file, function name,
 * and line number where the warning was generated. It also adds a breadcrumb to track program flow.
 * 
 * @tparam T The type of the message to be logged.
 * @param message The warning message to be logged.
 * @param location The source location where the log is generated (default: current location).
 */
template <typename T>
void MakerLogger::warning(T message, const std::source_location& location) {
    BOOST_LOG_TRIVIAL(warning) << "[File: " << getFileName(location.file_name())
                               << ", Function: " << location.function_name()
                               << ", Line: " << location.line()
                               << "] " << message;
    addBreadcrumb(message, location);
}

VORMAKER_NAMESPACE_CLOSE
