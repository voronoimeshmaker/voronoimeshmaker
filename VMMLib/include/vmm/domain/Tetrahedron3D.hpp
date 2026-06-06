//==============================================================================
// File        : Tetrahedron3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Tetrahedral 3D domain.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <array>
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Polyhedron3D.hpp>

namespace vmm::domain {

class Tetrahedron3D final {
public:
    Tetrahedron3D(Point3D a, Point3D b, Point3D c, Point3D d);

    [[nodiscard]] std::array<Point3D, 4U> vertices() const noexcept { return {a_, b_, c_, d_}; }
    [[nodiscard]] vmm::core::Real volume() const noexcept;
    [[nodiscard]] Bounds3D bounds() const noexcept;
    [[nodiscard]] bool contains(Point3D point) const noexcept;
    [[nodiscard]] Polyhedron3D to_polyhedron() const;

private:
    [[nodiscard]] static vmm::core::Real signed_volume(Point3D a, Point3D b, Point3D c, Point3D d) noexcept;

    Point3D a_{};
    Point3D b_{};
    Point3D c_{};
    Point3D d_{};
};

inline Tetrahedron3D::Tetrahedron3D(Point3D a, Point3D b, Point3D c, Point3D d)
    : a_(a),
      b_(b),
      c_(c),
      d_(d)
{
    if(volume() <= 0.0) {
        vmm::error::throw_invalid_argument("Tetrahedron3D", "Tetrahedron volume must be positive.");
    }
}

inline vmm::core::Real Tetrahedron3D::signed_volume(Point3D a, Point3D b, Point3D c, Point3D d) noexcept
{
    const Point3D ab{b.x - a.x, b.y - a.y, b.z - a.z};
    const Point3D ac{c.x - a.x, c.y - a.y, c.z - a.z};
    const Point3D ad{d.x - a.x, d.y - a.y, d.z - a.z};
    return (ab.x * ((ac.y * ad.z) - (ac.z * ad.y)) +
            ab.y * ((ac.z * ad.x) - (ac.x * ad.z)) +
            ab.z * ((ac.x * ad.y) - (ac.y * ad.x))) /
           6.0;
}

inline vmm::core::Real Tetrahedron3D::volume() const noexcept
{
    return std::abs(signed_volume(a_, b_, c_, d_));
}

inline Bounds3D Tetrahedron3D::bounds() const noexcept
{
    Bounds3D result{a_, a_};
    for(const auto vertex : std::array<Point3D, 3U>{b_, c_, d_}) {
        result.min.x = std::min(result.min.x, vertex.x);
        result.min.y = std::min(result.min.y, vertex.y);
        result.min.z = std::min(result.min.z, vertex.z);
        result.max.x = std::max(result.max.x, vertex.x);
        result.max.y = std::max(result.max.y, vertex.y);
        result.max.z = std::max(result.max.z, vertex.z);
    }
    return result;
}

inline bool Tetrahedron3D::contains(Point3D point) const noexcept
{
    constexpr auto tolerance = 1.0e-12;
    const auto total = volume();
    const auto sub =
        std::abs(signed_volume(point, b_, c_, d_)) +
        std::abs(signed_volume(a_, point, c_, d_)) +
        std::abs(signed_volume(a_, b_, point, d_)) +
        std::abs(signed_volume(a_, b_, c_, point));
    return sub <= total + tolerance;
}

inline Polyhedron3D Tetrahedron3D::to_polyhedron() const
{
    std::vector<Point3D> vertices{a_, b_, c_, d_};
    std::vector<TriangleFace3D> faces{
        {{0U, 2U, 1U}, 0U},
        {{0U, 1U, 3U}, 0U},
        {{1U, 2U, 3U}, 0U},
        {{2U, 0U, 3U}, 0U}};
    std::vector<SurfacePatch> patches{{"tetrahedron-wall", BoundarySurfaceTypeTraits::Wall}};
    return Polyhedron3D{SurfaceMesh3D{std::move(vertices), std::move(faces), std::move(patches)}};
}

[[nodiscard]] inline Tetrahedron3D make_tetrahedron_3d(Point3D a, Point3D b, Point3D c, Point3D d)
{
    return Tetrahedron3D{a, b, c, d};
}

} // namespace vmm::domain
