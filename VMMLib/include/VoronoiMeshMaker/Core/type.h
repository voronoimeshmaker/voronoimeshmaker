//==============================================================================
// Name        : type.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.2
// Description : Common types used across the library (CGAL kernel, points,
//               segments, vectors, polygons, smart pointers, etc.).
// License     : GNU GPL v3
//==============================================================================

#pragma once

/**
 * @file type.h
 * @brief Defines common geometric and numeric types used in the library.
 *
 * This header centralises key computational-geometry types (2D/3D points,
 * segments, rays, vectors, polygons/polyhedra) and smart-pointer aliases.
 *
 * @ingroup core
 */

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
#include <VoronoiMeshMaker/Core/namespace.h>

VORMAKER_NAMESPACE_OPEN

//------------------------------------------------------------------------------
// Geometric kernel and types (kept inside a sub-namespace gtp).
//------------------------------------------------------------------------------
namespace gtp {

/// Kernel used in CGAL computations (robust predicates, fast constructions).
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

//--- 2D/3D primitives ---------------------------------------------------------

using Point2D   = Kernel::Point_2;
using Point3D   = Kernel::Point_3;

using Segment2D = Kernel::Segment_2;
using Segment3D = Kernel::Segment_3;

using Ray2D     = Kernel::Ray_2;
using Ray3D     = Kernel::Ray_3;

using Vector2D  = Kernel::Vector_2;
using Vector3D  = Kernel::Vector_3;

//--- Composite shapes ---------------------------------------------------------

using Polygon2D    = CGAL::Polygon_2<Kernel>;
using Polyhedron3D = CGAL::Polyhedron_3<Kernel>;

//--- Smart pointers -----------------------------------------------------------

using PtrPolygon2DUnique        = std::unique_ptr<Polygon2D>;
using PtrConstPolygon2DUnique   = std::unique_ptr<const Polygon2D>;
using PtrPolyhedron3DUnique     = std::unique_ptr<Polyhedron3D>;
using PtrConstPolyhedron3DUnique= std::unique_ptr<const Polyhedron3D>;

using PtrPolygon2DShared        = std::shared_ptr<Polygon2D>;
using PtrConstPolygon2DShared   = std::shared_ptr<const Polygon2D>;
using PtrPolyhedron3DShared     = std::shared_ptr<Polyhedron3D>;
using PtrConstPolyhedron3DShared= std::shared_ptr<const Polyhedron3D>;

//--- Containers ---------------------------------------------------------------

using LstPoint2D = std::list<Point2D>;
using LstPoint3D = std::list<Point3D>;
using VecPoint2D = std::vector<Point2D>;
using VecPoint3D = std::vector<Point3D>;

/**
 * @brief Enum to classify different geometry types.
 *
 * Useful for light-weight tagging/dispatch without RTTI.
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

}  // namespace gtp

//------------------------------------------------------------------------------
// Numeric type exposed at the vmm level (used by constants.h).
//------------------------------------------------------------------------------

using Real = gtp::Kernel::FT;
using Int  = gtp::Kernel::RT;

//--- Common integer/real containers (non-geometry) ----------------------------

using LstInt  = std::list<Int>;
using VecInt  = std::vector<Int>;
using LstReal = std::list<Real>;
using VecReal = std::vector<Real>;

VORMAKER_NAMESPACE_CLOSE
