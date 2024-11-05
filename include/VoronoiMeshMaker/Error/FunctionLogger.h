#ifndef VORMAKER_FUNCTION_LOGGER_H
#define VORMAKER_FUNCTION_LOGGER_H

//==============================================================================
// Name        : FunctionLogger.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 2.1
// Description : Function flow logger for the VoronoiMeshMaker library.
//               Part of the 'debug' group for tracking function entry and exit.
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
 * @file FunctionLogger.h
 * @brief Defines the FunctionLogger class, which provides automatic tracking
 * of function entry and exit for the VoronoiMeshMaker library.
 *
 * The FunctionLogger class logs function entry and exit, capturing detailed
 * information such as file, line, and function name. It supports automated
 * logging via RAII, simplifying program flow tracking, especially for debugging
 * and exception handling contexts.
 *
 * The behavior of the FunctionLogger can be controlled via the `NDEBUG1` macro.
 * If `NDEBUG1` is defined, the FunctionLogger will have an empty implementation
 * to avoid overhead in release builds. Otherwise, the full implementation is used
 * to provide detailed logging information.
 *
 * @ingroup debug
 * @version 2.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//#define NDEBUG1

#ifndef NDEBUG1  // Debug version: active logging function
    #include <VoronoiMeshMaker/Error/FunctionLoggerFull.h>
#else  // Release version: empty implementation to avoid overhead
    #include <VoronoiMeshMaker/Error/FunctionLoggerNull.h>
#endif  

#endif // VORMAKER_FUNCTION_LOGGER_H
