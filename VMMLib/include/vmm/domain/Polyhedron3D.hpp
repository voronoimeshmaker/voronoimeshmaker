//==============================================================================
// File        : Polyhedron3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Closed triangular 3D polyhedron domain.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/SurfaceMesh3D.hpp>

namespace vmm::domain {

class Polyhedron3D final {
public:
    explicit Polyhedron3D(SurfaceMesh3D surface);

    [[nodiscard]] const SurfaceMesh3D& surface() const noexcept { return surface_; }
    [[nodiscard]] Bounds3D bounds() const { return surface_.bounds(); }
    [[nodiscard]] vmm::core::Real volume() const noexcept;

private:
    SurfaceMesh3D surface_;
};

inline Polyhedron3D::Polyhedron3D(SurfaceMesh3D surface)
    : surface_(std::move(surface))
{
    if(surface_.face_count() < 4U) {
        vmm::error::throw_invalid_argument("Polyhedron3D", "A closed triangular polyhedron requires at least four faces.");
    }
}

inline vmm::core::Real Polyhedron3D::volume() const noexcept
{
    vmm::core::Real signed_volume{};
    for(const auto face : surface_.faces()) {
        const auto& a = surface_.vertices()[face.vertex[0]];
        const auto& b = surface_.vertices()[face.vertex[1]];
        const auto& c = surface_.vertices()[face.vertex[2]];
        signed_volume +=
            (a.x * ((b.y * c.z) - (b.z * c.y)) +
             a.y * ((b.z * c.x) - (b.x * c.z)) +
             a.z * ((b.x * c.y) - (b.y * c.x))) /
            6.0;
    }
    return std::abs(signed_volume);
}

} // namespace vmm::domain
