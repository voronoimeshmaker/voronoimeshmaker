#ifndef __VORMAKER_LOGGER_H__
#define __VORMAKER_LOGGER_H__

//==============================================================================
// Name        : MakerLogger.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.4
// Description : Logging utility for tracking program flow and errors in the
//               VoronoiMeshMaker library. Part of the 'error' group for robust
//               error management.
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
 * @file MakerLogger.h
 * @brief Defines the MakerLogger class, which provides logging functionalities
 *        for the VoronoiMeshMaker library.
 *
 * The MakerLogger class is used to record important messages, warnings, and
 * errors. It helps to track program flow and debug potential issues. The
 * logger can be enabled or disabled depending on the compilation mode (enabled
 * in DEBUG mode).
 *
 * @ingroup error
 * @version 2.4
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Standard C++ Library Includes
//==============================================================================

#include <mutex>
#include <vector>

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================
#include <VoronoiMeshMaker/Misc/namespace.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup error
 * @class MakerLogger
 * @brief Provides logging functionalities for the VoronoiMeshMaker library.
 *
 * The MakerLogger class is used to record important messages, warnings, and
 * errors. It helps to track program flow and debug potential issues. The
 * logger can be enabled or disabled depending on the compilation mode (enabled
 * in DEBUG mode).
 */
class MakerLogger {
public:
    /**
     * @brief Initializes the logger.
     *
     * Configures logging to both the console and a file. This function should
     * be called once at the start of the application.
     */
    static void init();

    /**
     * @brief Logs entry into a function.
     *
     * @param location The source location where the entry is being logged.
     */
    static void logEnter(const std::source_location& location =
                         std::source_location::current());

    /**
     * @brief Logs exit from a function.
     *
     * @param location The source location where the exit is being logged.
     */
    static void logExit(const std::source_location& location =
                        std::source_location::current());

    /**
     * @brief Logs an error message.
     *
     * Logs an error message, including information about the file, function,
     * and line where the error occurred.
     *
     * @param message The error message to be logged.
     * @param location The source location where the error is being logged
     *                 (default: current location).
     */
    static void error(std::string_view message,
                      const std::source_location& location =
                          std::source_location::current());

    /**
     * @brief Adds a breadcrumb to track program flow.
     *
     * @param message The breadcrumb message to be added.
     * @param location The source location where the breadcrumb is being added.
     */
    static void addBreadcrumb(std::string_view message,
                              const std::source_location& location =
                                  std::source_location::current());

    /**
     * @brief Retrieves the breadcrumb trail as a string.
     *
     * @return A string containing the breadcrumb trail.
     */
    static std::string getBreadcrumbTrail();

private:
    static inline std::vector<std::string> breadcrumbTrail_; ///< Stores the breadcrumb messages.
    static inline std::mutex breadcrumbMutex_; ///< Mutex for thread-safe access to breadcrumbs.
    static const size_t maxBreadcrumbs_ = 200; ///< Maximum number of breadcrumbs to keep.
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORMAKER_LOGGER_H__
