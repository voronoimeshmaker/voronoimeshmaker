//==============================================================================
// Name        : type.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file with the definitions of the most common variables
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

#ifndef __VORONOIFVMAKER_TYPE_H__
#define __VORONOIFVMAKER_TYPE_H__

//==============================================================================
//  c++ include
//==============================================================================

#include <memory>
#include <list>

//==============================================================================
//  CGAL include
//==============================================================================

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyhedron_3.h>

/**
 * @ingroup misc
 * @brief Namespace for the VoronoiMeshMaker library.
 * 
 * This namespace contains the constants and configurations used across the
 * VoronoiMeshMaker library.
 */

#define VORMAKER_NAMESPACE_OPEN namespace vmm {
#define VORMAKER_NAMESPACE_CLOSE }

        
#define GEOTYPES_NAMESPACE_OPEN namespace gtp {
#define GEOTYPES_NAMESPACE_CLOSE }

        
        
VORMAKER_NAMESPACE_OPEN
GEOTYPES_NAMESPACE_OPEN
//==============================================================================
//  Type Definitions
//==============================================================================

    /**
     * @ingroup misc
     * @brief Defines the kernel used in CGAL.
     */
    using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

    /**
     * @ingroup misc
     * @brief Defines a 2D point type.
     */
    using Point2D = Kernel::Point_2;

    /**
     * @ingroup misc
     * @brief Defines a 3D point type.
     */
    using Point3D = Kernel::Point_3;

    /**
     * @ingroup misc
     * @brief Defines a 2D segment type.
     */
    using Segment2D = Kernel::Segment_2;

    /**
     * @ingroup misc
     * @brief Defines a 3D segment type.
     */
    using Segment3D = Kernel::Segment_3;

    /**
     * @ingroup misc
     * @brief Defines a 2D ray type.
     */
    using Ray2D = Kernel::Ray_2;

    /**
     * @ingroup misc
     * @brief Defines a 3D ray type.
     */
    using Ray3D = Kernel::Ray_3;

    /**
     * @ingroup misc
     * @brief Defines a 2D vector type.
     */
    using Vector2D = Kernel::Vector_2;

    /**
     * @ingroup misc
     * @brief Defines a 3D vector type.
     */
    using Vector3D = Kernel::Vector_3;

    /**
     * @ingroup misc
     * @brief Defines a 2D polygon type.
     */
    using Polygon2D = CGAL::Polygon_2<Kernel>;

    /**
     * @ingroup misc
     * @brief Defines a 2D polyhedron type.
     */
    using Polyhedron3D = CGAL::Polyhedron_3<Kernel>;

    /**
     * @ingroup misc
     * @brief Defines a smart pointer type for 2D polygons.
     */
    using PtrPolygon2D = std::unique_ptr<Polygon2D>;

    /**
     * @ingroup misc
     * @brief Defines a smart pointer type for 3D polyhedron.
     */
    using PtrPolyhedron3D = std::unique_ptr<Polyhedron3D>;

    /**
     * @ingroup misc
     * @brief Defines a smart pointer type for const 2D polygons.
     */
    using PtrConstPolygon2D = std::unique_ptr<const Polygon2D>;

    /**
     * @ingroup misc
     * @brief Defines a smart pointer type for const 3D polyhedron.
     */
    using PtrConstPolyhedron3D = std::unique_ptr<const Polyhedron3D>;

    /**
     * @ingroup misc
     * @brief Defines a list type for 2D points.
     */
    using LstPoint2D = std::list<Point2D>;

    /**
     * @ingroup misc
     * @brief Defines a list type for 3D points.
     */
    using LstPoint3D = std::list<Point3D>;

//==============================================================================
//  Enum to Classify Geometry Types
//==============================================================================

    /**
     * @ingroup misc
     * @brief Enum to classify different geometry types.
     * 
     * This enum helps in distinguishing between different geometric types
     * when processing various objects in the VoronoiMeshMaker library.
     */
    enum class GeometryType {
        Point2D,
        Point3D,
        Segment2D,
        Segment3D,
        Ray2D,
        Ray3D,
        Vector2D,
        Vector3D,
        Polygon2D,
        Polyhedron3D
    };

GEOTYPES_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE

    /**
     * @ingroup misc
     * @brief Defines the real number type based on the Kernel.
     * 
     * The `FT` type defined by the Kernel is a field type used for all real numbers
     * in geometric computations. By using this, we ensure consistency across the library.
     */
    using Real = vmm::gtp::Kernel::FT;


#endif // __VORONOIFVMAKER_TYPE_H__
