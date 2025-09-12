#pragma once
//==============================================================================
// Name        : Boundary2DConcepts.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 0.2.0
// Description : Lightweight C++20 concepts for Boundary2D. Kept minimal and
//               independent from external libs. Focus on compile-time
//               contracts for points, polygonization policy, and shapes.
// License     : GNU GPL v3
//==============================================================================

/**
 * @file Boundary2DConcepts.hpp
 * @brief Minimal concepts used by the Boundary2D core.
 *
 * Notes:
 *  - Concepts here must not include CGAL or heavy dependencies.
 *  - Point concept is liberal: allows field-access (x,y) or method-access
 *    (x(), y()). Adapters can specialise access for external types.
 *  - Shape concept requires polygonize(policy) -> std::vector<Point2>.
 *  - Keep contracts small (ISP): broader behaviours live in algorithms.
 */



//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/Boundary2D/Boundary2DTypes.hpp>

VORMAKER_NAMESPACE_OPEN
BOUNDARY2D_NAMESPACE_OPEN

//------------------------------------------------------------------------------
// Point2-like concept
//------------------------------------------------------------------------------
// Accepts either field access (p.x, p.y) or method access (p.x(), p.y()).
// External types can be adapted via free functions in Adapters if needed.

DETAIL_NAMESPACE_OPEN

template <class P>
concept HasFieldXY = requires(const P& p) {
    { p.x } -> std::convertible_to<Real>;
    { p.y } -> std::convertible_to<Real>;
};

template <class P>
concept HasMethodXY = requires(const P& p) {
    { p.x() } -> std::convertible_to<Real>;
    { p.y() } -> std::convertible_to<Real>;
};

DETAIL_NAMESPACE_CLOSE

template <class P>
concept Point2Like = detail::HasFieldXY<P> || detail::HasMethodXY<P>;

//------------------------------------------------------------------------------
// Polygonization policy concept
//------------------------------------------------------------------------------
// Intentionally permissive. Any type is accepted; algorithms can SFINAE on
// members like 'epsilon' or 'segments' if they choose to.

template <class Policy>
concept PolygonizePolicyLike = true;

//------------------------------------------------------------------------------
// Shape2D concept
//------------------------------------------------------------------------------
// Requires a polygonize(policy) that returns std::vector<Point2>,
// using the module's canonical Point2 POD for storage and performance.

template <class S, class Policy>
concept Shape2DLike =
    PolygonizePolicyLike<Policy> &&
    requires(const S& s, const Policy& pol) {
        { s.polygonize(pol) } -> std::same_as<std::vector<Point2>>;
    };

//------------------------------------------------------------------------------
// Helpers (optional) to statically assert contracts inside templates
//------------------------------------------------------------------------------

template <Point2Like P>
inline constexpr bool kIsPoint2Like = true;

template <class S, class Policy>
inline constexpr bool kIsShape2DLike = Shape2DLike<S, Policy>;

BOUNDARY2D_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
