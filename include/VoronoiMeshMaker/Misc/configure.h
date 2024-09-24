//==============================================================================
// Name        : configure.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with common variable definitions for the library.
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
 * @file configure.h
 * @brief Defines common constants used across the VoronoiMeshMaker library.
 * 
 * This file contains common constants, such as epsilon values and limits, 
 * that are shared across various components of the VoronoiMeshMaker library. 
 * These constants are used to handle numerical stability and default sizes.
 * 
 * @ingroup misc
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOMESHVMAKER_CONFIGURE_H__
#define __VORONOMESHVMAKER_CONFIGURE_H__

#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Very small value used as a numerical zero threshold.
 * 
 * This constant represents a very small value, typically used in numerical
 * operations where exact zero is impractical due to floating-point precision limits.
 */
inline constexpr Real ZERO = (1e250 * std::numeric_limits<Real>::min());

/**
 * @ingroup misc
 * @brief A small limit value used for numerical comparisons.
 * 
 * This constant is used as a small limit threshold in numerical calculations.
 */
inline constexpr Real LIMIT = 1e-30;

/**
 * @ingroup misc
 * @brief Epsilon value used for floating-point comparisons.
 * 
 * The epsilon value is used when comparing floating-point values for equality.
 * It represents a small range within which two values are considered equal.
 */
inline constexpr Real EPSILON = 1e-6;

/**
 * @ingroup misc
 * @brief Default size value used in buffer allocations and memory sizing.
 * 
 * The default size is used for allocating buffers or other memory structures
 * with a reasonable initial size.
 */
inline constexpr int LSIZE = 0x50;

VORMAKER_NAMESPACE_CLOSE

/**
 * @ingroup misc
 * @brief Computes the mathematical constant Pi.
 * 
 * @return The value of Pi using the formula `acos(-1.0L)`.
 */
constexpr Real Pi() { return acos(-1.0L); }

#endif // __VORONOMESHVMAKER_CONFIGURE_H__
