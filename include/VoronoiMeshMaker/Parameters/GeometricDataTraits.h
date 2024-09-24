//==============================================================================
// Name        : GeometricDataTraits.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the GeometricDataTraits structure used for identifying 
//               and working with different data types.
//               This file is part of the 'parameters' group of utility files.
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
 * @file GeometricDataTraits.h
 * @brief Defines the `GeometricDataTraits` structure used for type identification and operations on various data types.
 * 
 * This file defines a generic trait structure, `GeometricDataTraits`, that provides type information
 * and possible operations for different data types used in the `GeometricDataHolder` class.
 * Specializations of this trait structure are provided for specific data types used in computational geometry.
 * 
 * @ingroup parameters
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_GEOMETRICDATATRAITS_H__
#define __VORONOIMESHMAKER_GEOMETRICDATATRAITS_H__

//==============================================================================
//  C++ includes
//==============================================================================

//#include <list>
//#include <vector>


//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Traits structure used for identifying and working with different data types.
 * 
 * The `GeometricDataTraits` structure provides a way to extract type information (such as type names)
 * for supported data types in the `GeometricDataHolder` class. Specializations are provided for
 * each data type that the system handles.
 * 
 * @tparam T The data type for which traits are provided.
 */
template<typename T>
struct GeometricDataTraits {
    /**
     * @brief Returns the name of the data type as a string.
     * 
     * This method provides a way to get a human-readable name for the data type.
     * @return const char* The name of the data type.
     */
    static const char* name() {
        return "Unknown";
    }
};

//==============================================================================
// Specializations for supported types
//==============================================================================

template<>
struct GeometricDataTraits<int> {
    static const char* name() {
        return "int";
    }
};

template<>
struct GeometricDataTraits<Real> {
    static const char* name() {
        return "Real";
    }
};

template<>
struct GeometricDataTraits<gtp::Point2D> {
    static const char* name() {
        return "Point2D";
    }
};

template<>
struct GeometricDataTraits<gtp::Point3D> {
    static const char* name() {
        return "Point3D";
    }
};

template<>
struct GeometricDataTraits<gtp::Vector2D> {
    static const char* name() {
        return "Vector2D";
    }
};

template<>
struct GeometricDataTraits<gtp::Vector3D> {
    static const char* name() {
        return "Vector3D";
    }
};

template<>
struct GeometricDataTraits<std::list<int>> {
    static const char* name() {
        return "std::list<int>";
    }
};

template<>
struct GeometricDataTraits<std::list<Real>> {
    static const char* name() {
        return "std::list<Real>";
    }
};

template<>
struct GeometricDataTraits<std::list<gtp::Point2D>> {
    static const char* name() {
        return "std::list<Point2D>";
    }
};

template<>
struct GeometricDataTraits<std::list<gtp::Point3D>> {
    static const char* name() {
        return "std::list<Point3D>";
    }
};

template<>
struct GeometricDataTraits<std::vector<int>> {
    static const char* name() {
        return "std::vector<int>";
    }
};

template<>
struct GeometricDataTraits<std::vector<Real>> {
    static const char* name() {
        return "std::vector<Real>";
    }
};

template<>
struct GeometricDataTraits<std::vector<gtp::Point2D>> {
    static const char* name() {
        return "std::vector<Point2D>";
    }
};

template<>
struct GeometricDataTraits<std::vector<gtp::Point3D>> {
    static const char* name() {
        return "std::vector<Point3D>";
    }
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOIMESHMAKER_GEOMETRICDATATRAITS_H__
