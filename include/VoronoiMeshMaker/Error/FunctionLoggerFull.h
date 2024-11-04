#ifndef VORMAKER_FUNCTIONFULL_LOGGER_H
#define VORMAKER_FUNCTIONFULL_LOGGER_H

//==============================================================================
// Name        : FunctionLoggerFull.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.1
// Description : Function flow logger for the VoronoiMeshMaker library.
//               Part of the 'debug' group for function entry and exit tracking.
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
 * @file FunctionLoggerFull.h
 * @brief Defines the FunctionLogger class, which provides automatic tracking
 * of function entry and exit for the VoronoiMeshMaker library.
 *
 * The FunctionLogger class logs the entry and exit of functions, capturing detailed
 * information such as file, line, and function name. It supports automated logging
 * via RAII, simplifying the tracking of program flow, especially in debugging and
 * exception handling contexts.
 *
 * This is the full implementation of FunctionLogger, used in debug builds to provide
 * detailed logging information. In release builds, it is recommended to use the null
 * implementation to avoid performance overhead.
 *
 * @ingroup debug
 * @version 2.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Standard Library Includes
//==============================================================================
#include <string>
#include <source_location>
#include <iostream> // Added for std::cout

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================
#include <VoronoiMeshMaker/Error/MakerLogger.h>
#include <VoronoiMeshMaker/Misc/namespace.h>

//==============================================================================
// Namespace Declaration
//==============================================================================

VORMAKER_NAMESPACE_OPEN

#pragma message("FunctionLoggerFull.h has been included")

/**
 * @ingroup debug
 * @class FunctionLogger
 * @brief Class to log function entry and exit in the VoronoiMeshMaker library.
 *
 * The FunctionLogger class uses RAII to automatically log when a function is called
 * and when it finishes. This aids in tracking program flow and provides useful
 * information for debugging and understanding the sequence of function calls,
 * especially in complex codebases.
 */
class FunctionLogger {
public:
    /**
     * @brief Constructor that logs function entry.
     *
     * Logs the entry of the function using MakerLogger and stores the location information
     * for use in the destructor, which logs the exit of the function.
     *
     * @param location The source location of the function (default: current location).
     */
    FunctionLogger(const std::source_location& location = std::source_location::current())
        : location_(location) {
        std::cout << "Entering function: " << location_.function_name() << std::endl; // Debug output
        MakerLogger::logEnter(location_);
    }

    /**
     * @brief Destructor that logs function exit.
     *
     * Automatically logs the exit of the function using MakerLogger when the instance
     * of FunctionLogger goes out of scope.
     */
    ~FunctionLogger() {
        std::cout << "Exiting function: " << location_.function_name() << std::endl; // Debug output
        MakerLogger::logExit(location_);
    }

private:
    std::source_location location_; ///< Source location of the function being logged.
};

VORMAKER_NAMESPACE_CLOSE

#endif // VORMAKER_FUNCTIONFULL_LOGGER_H
