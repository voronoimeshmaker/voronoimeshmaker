//==============================================================================
// Name        : configure.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with the definitions of some common variables
//               in the library. This file is part of the 'misc' group of utility files.
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

#ifndef __VORONOMESHVMAKER_CONFIGURE_H__
#define __VORONOMESHVMAKER_CONFIGURE_H__

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/type.h>

/**
 * @defgroup misc Miscellaneous Utilities
 * @brief This group contains utility classes and functions used across the library.
 *
 * The 'misc' group is composed of various utility components that aid in
 * the general functionality of the library, including base classes and common definitions.
 */



/**
 * @brief Open the VoronoiMeshMaker namespace.
 */
VORMAKER_NAMESPACE_OPEN

    /**
     * @ingroup misc
     * @brief Zero value constant.
     * 
     * This constant represents a very small value close to zero,
     * calculated as (1e250 * std::numeric_limits<Real>::min()).
     */
    const Real ZERO = (1e250 * std::numeric_limits<Real>::min());  

    /**
     * @ingroup misc
     * @brief Limit value constant.
     * 
     * This constant represents a small limit value, defined as 1e-30.
     */
    const Real LIMIT = 1e-30; 

    /**
     * @ingroup misc
     * @brief Epsilon value constant.
     * 
     * This constant represents a small epsilon value, defined as 1e-6.
     */
    const Real EPSILON = 1e-6; 

    /**
     * @ingroup misc
     * @brief Default size constant.
     * 
     * This constant represents a default size, defined as 0x50 (80 in decimal).
     */
    const int LSIZE = 0x50;
    
VORMAKER_NAMESPACE_CLOSE

/**
 * @ingroup misc
 * @brief Calculate the value of Pi.
 * 
 * This function returns the value of Pi using the `acos` function.
 * 
 * @return The value of Pi.
 */
constexpr Real Pi() { return acos(-1.0L); }

#endif // __VORONOMESHVMAKER_CONFIGURE_H__
