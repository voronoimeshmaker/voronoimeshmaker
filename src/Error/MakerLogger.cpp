//==============================================================================
// Name        : MakerLogger.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.4
// Description : Implementation of the MakerLogger class for logging in the
//               VoronoiMeshMaker library. Part of the 'error' group for robust
//               error management.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//==============================================================================

/**
 * @file MakerLogger.cpp
 * @brief Implements the MakerLogger class, which provides logging
 *        functionalities for the VoronoiMeshMaker library.
 *
 * This file contains the implementation of the methods of the MakerLogger
 * class, which are used to record important messages, warnings, and errors,
 * helping to track program flow.
 *
 * @ingroup error
 * @version 2.4
 * @date 2024
 */

//==============================================================================
// Boost Includes
//==============================================================================

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================
#include <VoronoiMeshMaker/Error/MakerLogger.h>
#include <VoronoiMeshMaker/Misc/Misc.h>

//==============================================================================
// Namespace Declaration
//==============================================================================

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

VORMAKER_NAMESPACE_OPEN

void MakerLogger::init() {
    // Add common attributes, such as timestamp, to the log
    logging::add_common_attributes();

    // Set the filter to include messages of level debug or higher
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::debug
    );

    // Configure console logging with a simple format
    logging::add_console_log(
        std::clog,
        keywords::format = "[%TimeStamp%] <%Severity%> %Message%"
    );

    // Optionally, configure file logging
    /*
    logging::add_file_log(
        keywords::file_name = "voronoi_log_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::format = "[%TimeStamp%] <%Severity%> %Message%"
    );
    */
}

void MakerLogger::addBreadcrumb(std::string_view message,
                                const std::source_location& location) {
    std::lock_guard<std::mutex> lock(breadcrumbMutex_);
    if (breadcrumbTrail_.size() >= maxBreadcrumbs_) {
        breadcrumbTrail_.erase(breadcrumbTrail_.begin());
    }
    std::ostringstream oss;
    oss << "[File: " << getFileName(location.file_name())
        << ", Function: " << location.function_name()
        << ", Line: " << location.line()
        << "] " << message;
    breadcrumbTrail_.emplace_back(oss.str());
}

std::string MakerLogger::getBreadcrumbTrail() {
    std::lock_guard<std::mutex> lock(breadcrumbMutex_);
    std::ostringstream oss;
    oss << "Breadcrumb trail leading to the exception:\n";
    for (const auto& breadcrumb : breadcrumbTrail_) {
        oss << breadcrumb << "\n";
    }
    return oss.str();
}

void MakerLogger::logEnter(const std::source_location& location) {
    BOOST_LOG_TRIVIAL(debug) << "[ENTER] Function: " << location.function_name()
                             << " at " << getFileName(location.file_name())
                             << ":" << location.line();
    addBreadcrumb("Entered function", location);
}

void MakerLogger::logExit(const std::source_location& location) {
    BOOST_LOG_TRIVIAL(debug) << "[EXIT] Function: " << location.function_name()
                             << " at " << getFileName(location.file_name())
                             << ":" << location.line();
    addBreadcrumb("Exited function", location);
}

void MakerLogger::error(std::string_view message,
                        const std::source_location& location) {
    BOOST_LOG_TRIVIAL(error) << "[ERROR] " << message
                             << " (File: " << getFileName(location.file_name())
                             << ", Function: " << location.function_name()
                             << ", Line: " << location.line() << ")";
    addBreadcrumb(message, location);
}

VORMAKER_NAMESPACE_CLOSE
