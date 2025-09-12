#pragma once
//==============================================================================
// Name        : Rectangle.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : Boundary2D
// Description : Parametric rectangle (cx, cy, w, h, theta) with CCW vertices.
// License     : GNU GPL v3
// Version     : 1.0.1
//==============================================================================

/**
 * @file Rectangle.hpp
 * @brief Boundary2D shape: rectangle (center, width, height, rotation).
 *
 * Design notes:
 *  - Core is CGAL-free; adapters handle conversions.
 *  - Returns CCW vertices; validation (w>0,h>0) fica para validadores.
 *  - Oferece zero-allocation helper via polygonize_into(std::span).
 */


//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/Boundary2D/Boundary2DConcepts.hpp>
#include <VoronoiMeshMaker/Boundary2D/Policies/PolygonizePolicy.hpp>

#ifndef NDEBUG
  #include <cassert>
  #define VMM_DEBUG_ASSERT(cond) assert(cond)
#else
  #define VMM_DEBUG_ASSERT(cond) ((void)0)
#endif

VORMAKER_NAMESPACE_OPEN
BOUNDARY2D_NAMESPACE_OPEN

struct Rectangle {
    Real cx{0};     ///< center x
    Real cy{0};     ///< center y
    Real w{1};      ///< width  (> 0 expected by validators)
    Real h{1};      ///< height (> 0 expected by validators)
    Real theta{0};  ///< rotation in radians (CCW)

    static constexpr int kVertexCount = 4;

    [[nodiscard]] bool has_positive_dims() const noexcept {
        return (w > static_cast<Real>(0)) && (h > static_cast<Real>(0));
    }

    /// Local corners around origin, axis-aligned, CCW.
    [[nodiscard]] std::array<Point2, kVertexCount>
    local_corners_ccw() const noexcept {
        const Real hw = w * static_cast<Real>(0.5);
        const Real hh = h * static_cast<Real>(0.5);
        return { Point2{-hw,-hh}, Point2{ hw,-hh},
                 Point2{ hw, hh}, Point2{-hw, hh} };
    }

    /// World corners (rotated + translated), CCW.
    [[nodiscard]] std::array<Point2, kVertexCount>
    world_corners_ccw() const noexcept {
        const auto c = local_corners_ccw();
        const Real ct = std::cos(theta);
        const Real st = std::sin(theta);
        std::array<Point2, kVertexCount> out{};
        for (int i = 0; i < kVertexCount; ++i) {
            const Real xr =  ct * c[i].x - st * c[i].y;
            const Real yr =  st * c[i].x + ct * c[i].y;
            out[i] = Point2{ xr + cx, yr + cy };
        }
        return out;
    }

    /// Exact AABB from the 4 world corners.
    [[nodiscard]] Box2 bbox() const noexcept {
        const auto wc = world_corners_ccw();
        Box2 b;
        for (int i = 0; i < kVertexCount; ++i) b.expand(wc[i]);
        return b;
    }

    /**
     * @brief Polygonize to std::vector (allocates once; reserves 4).
     * @return CCW vertices (size=4).
     */
    [[nodiscard]] std::vector<Point2>
    polygonize(const ::vmm::b2d::PolygonizePolicy&) const {
        VMM_DEBUG_ASSERT(has_positive_dims());
        const auto wc = world_corners_ccw();
        std::vector<Point2> out;
        out.reserve(kVertexCount);
        for (int i = 0; i < kVertexCount; ++i) out.push_back(wc[i]);
        return out;
    }

    /**
     * @brief Zero-allocation polygonization into a pre-sized span (size>=4).
     * @note  Only the first 4 entries are written.
     */
    void polygonize_into(std::span<Point2> dst) const noexcept {
        VMM_DEBUG_ASSERT(has_positive_dims());
        VMM_DEBUG_ASSERT(dst.size() >= static_cast<std::size_t>(kVertexCount));
        const auto wc = world_corners_ccw();
        for (int i = 0; i < kVertexCount; ++i) dst[i] = wc[i];
    }
};

// Compile-time contract with our concepts.
static_assert(Shape2DLike<Rectangle, ::vmm::b2d::PolygonizePolicy>);

BOUNDARY2D_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
