//==============================================================================
// Name        : type.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with the definitions of common types used in the library.
//               Part of the 'misc' group of utility files.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vieira de Vasconcellos
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
 * @file type.h
 * @brief Defines common geometric and numeric types used in the VoronoiMeshMaker library.
 * 
 * This file defines key types used in computational geometry and numerical calculations.
 * Types such as points, segments, vectors, and polygons in 2D and 3D are defined here, 
 * along with smart pointers for memory management.
 * 
 * @ingroup misc
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_TYPE_H__
#define __VORONOIMESHMAKER_TYPE_H__

//==============================================================================
//  C++ includes
//==============================================================================

#include <list>
#include <memory>
#include <vector>

//==============================================================================
//  CGAL includes
//==============================================================================

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyhedron_3.h>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>

VORMAKER_NAMESPACE_OPEN
GEOTYPES_NAMESPACE_OPEN

/// Defines the kernel used in CGAL computations.
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

/// 2D point type.
using Point2D = Kernel::Point_2;

/// 3D point type.
using Point3D = Kernel::Point_3;

/// 2D segment type.
using Segment2D = Kernel::Segment_2;

/// 3D segment type.
using Segment3D = Kernel::Segment_3;

/// 2D ray type.
using Ray2D = Kernel::Ray_2;

/// 3D ray type.
using Ray3D = Kernel::Ray_3;

/// 2D vector type.
using Vector2D = Kernel::Vector_2;

/// 3D vector type.
using Vector3D = Kernel::Vector_3;

/// 2D polygon type.
using Polygon2D = CGAL::Polygon_2<Kernel>;

/// 3D polyhedron type.
using Polyhedron3D = CGAL::Polyhedron_3<Kernel>;

/// Unique pointer to 2D polygon.
using PtrPolygon2DUnique = std::unique_ptr<Polygon2D>;

/// Unique pointer to 3D polyhedron.
using PtrPolyhedron3DUnique = std::unique_ptr<Polyhedron3D>;

/// Unique pointer to const 2D polygon.
using PtrConstPolygon2DUnique = std::unique_ptr<const Polygon2D>;

/// Unique pointer to const 3D polyhedron.
using PtrConstPolyhedron3DUnique = std::unique_ptr<const Polyhedron3D>;

/// Shared pointer to 2D polygon.
using PtrPolygon2DShared = std::shared_ptr<Polygon2D>;

/// Shared pointer to 3D polyhedron.
using PtrPolyhedron3DShared = std::shared_ptr<Polyhedron3D>;

/// Shared pointer to const 2D polygon.
using PtrConstPolygon2DShared = std::shared_ptr<const Polygon2D>;

/// Shared pointer to const 3D polyhedron.
using PtrConstPolyhedron3DShared = std::shared_ptr<const Polyhedron3D>;

/// List of 2D points.
using LstPoint2D = std::list<Point2D>;

/// List of 3D points.
using LstPoint3D = std::list<Point3D>;

/// Vector of 2D points.
using VecPoint2D = std::vector<Point2D>;

/// Vector of 3D points.
using VecPoint3D = std::vector<Point3D>;

/**
 * @ingroup misc
 * @brief Enum to classify different geometry types.
 * 
 * This enum is used to distinguish between different types of geometric
 * objects in the VoronoiMeshMaker library.
 */
enum class GeometryType {
    Point2D,       ///< 2D point type.
    Point3D,       ///< 3D point type.
    Segment2D,     ///< 2D segment type.
    Segment3D,     ///< 3D segment type.
    Ray2D,         ///< 2D ray type.
    Ray3D,         ///< 3D ray type.
    Vector2D,      ///< 2D vector type.
    Vector3D,      ///< 3D vector type.
    Polygon2D,     ///< 2D polygon type.
    Polyhedron3D   ///< 3D polyhedron type.
};

GEOTYPES_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE

/// Real number type based on the CGAL kernel.
using Real = vmm::gtp::Kernel::FT;

/// List of integers.
using LstInt = std::list<int>;

/// Vector of integers.
using VecInt = std::vector<int>;

/// List of Real numbers.
using LstReal = std::list<Real>;

/// Vector of Real numbers.
using VecReal = std::vector<Real>;

#endif // __VORONOIMESHMAKER_TYPE_H__
