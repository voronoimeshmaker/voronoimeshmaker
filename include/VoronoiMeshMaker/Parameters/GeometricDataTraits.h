//==============================================================================
// Name        : GeometricDataTraits.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.1
// Description : Defines the GeometricDataTraits structure used for identifying 
//               and working with different data types.
//               This file is part of the 'parameters' group of utility files.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : This program is free software: you can redistribute it and/or modify
//               it under the terms of the GNU General Public License as published by
//               the Free Software Foundation, either version 3 of the License, or
//               (at your option) any later version.
//
//               This program is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty of
//               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//               GNU General Public License for more details.
//
//               You should have received a copy of the GNU General Public License
//               along with this program. If not, see <https://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file GeometricDataTraits.h
 * @brief Defines the GeometricDataTraits structure used for identifying and working with different data types.
 *
 * This file contains the definition of the GeometricDataTraits structure, which is part
 * of the Parameters module of the VoronoiMeshMaker library. The structure provides a
 * mechanism to identify and manipulate various data types used in geometric computations.
 *
 * @author João Flávio Vieira de Vasconcellos
 * @version 1.1
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

#ifndef VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_TRAITS_H
#define VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_TRAITS_H

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================

#include <type_traits>

//==============================================================================
// Namespace for VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @struct GeometricDataTraits
 * @brief Provides type identification for geometric data types.
 *
 * This structure is used to determine the characteristics of various data types used
 * in the geometric calculations within the VoronoiMeshMaker library.
 */
template <typename T>
struct GeometricDataTraits {
    static constexpr bool is_supported = false;
};

//==============================================================================
// Specializations for supported data types
//==============================================================================

template <>
struct GeometricDataTraits<int> {
    static constexpr bool is_supported = true;
};

template <>
struct GeometricDataTraits<Real> {
    static constexpr bool is_supported = true;
};

template <>
struct GeometricDataTraits<std::string> {
    static constexpr bool is_supported = true;
};


template <>
struct GeometricDataTraits<gtp::Point2D> {
    static constexpr bool is_supported = true;
};

template <>
struct GeometricDataTraits<gtp::Point3D> {
    static constexpr bool is_supported = true;
};

template <>
struct GeometricDataTraits<gtp::Vector2D> {
    static constexpr bool is_supported = true;
};

template <>
struct GeometricDataTraits<gtp::Vector3D> {
    static constexpr bool is_supported = true;
};


VORMAKER_NAMESPACE_CLOSE

#endif // VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_TRAITS_H
