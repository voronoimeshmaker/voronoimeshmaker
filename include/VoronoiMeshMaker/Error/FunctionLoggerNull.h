#ifndef VORMAKER_FUNCTIONNULL_LOGGER_H
#define VORMAKER_FUNCTIONNULL_LOGGER_H

//==============================================================================
// Name        : FunctionLoggerNull.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.1
// Description : Null implementation of function flow logger for the
//               VoronoiMeshMaker library. Part of the 'debug' group for
//               tracking function entry and exit.
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
 * @file FunctionLoggerNull.h
 * @brief Defines the null implementation of the FunctionLogger class for the
 *        VoronoiMeshMaker library.
 *
 * The FunctionLogger class, in this null version, is used in release builds
 * where the tracking of function entry and exit is disabled to avoid
 * performance overhead. This implementation ensures that code using
 * FunctionLogger can be compiled without modifications, but it will not
 * perform any action at runtime.
 *
 * The behavior is controlled by the `NDEBUG1` macro. If `NDEBUG1` is defined,
 * this null implementation will be used. Otherwise, the full implementation
 * in `FunctionLoggerFull.h` will be used to provide detailed logging
 * information.
 *
 * @ingroup debug
 * @version 2.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// VoronoiMeshMaker Library Includes
//==============================================================================
#include <VoronoiMeshMaker/Misc/namespace.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup debug
 * @class FunctionLogger
 * @brief Empty implementation for release builds of the VoronoiMeshMaker
 *        library.
 *
 * In release mode, the FunctionLogger class is defined but performs no action,
 * ensuring there is no runtime overhead.
 */
class FunctionLogger {
public:
    /**
     * @brief Default constructor that performs no action.
     *
     * @param location Optional parameter for compatibility, not used in this
     *                 implementation.
     */
    FunctionLogger(const std::source_location& =
                   std::source_location::current()) {}

    /**
     * @brief Default destructor that performs no action.
     */
    ~FunctionLogger() {}
};

VORMAKER_NAMESPACE_CLOSE

#endif // VORMAKER_FUNCTIONNULL_LOGGER_H