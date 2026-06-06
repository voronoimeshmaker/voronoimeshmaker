//==============================================================================
// File        : Sphere3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Spherical 3D domain.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <numbers>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/SurfaceMesh3D.hpp>

namespace vmm::domain {

class Sphere3D final {
public:
    explicit Sphere3D(vmm::core::Real radius, Point3D centre = {});

    [[nodiscard]] vmm::core::Real radius() const noexcept { return radius_; }
    [[nodiscard]] Point3D centre() const noexcept { return centre_; }
    [[nodiscard]] vmm::core::Real volume() const noexcept;
    [[nodiscard]] Bounds3D bounds() const noexcept;
    [[nodiscard]] bool contains(Point3D point) const noexcept;

private:
    vmm::core::Real radius_{};
    Point3D centre_{};
};

inline Sphere3D::Sphere3D(vmm::core::Real radius, Point3D centre)
    : radius_(radius),
      centre_(centre)
{
    if(radius_ <= 0.0) {
        vmm::error::throw_invalid_argument("Sphere3D", "Sphere radius must be positive.");
    }
}

inline vmm::core::Real Sphere3D::volume() const noexcept
{
    return (4.0 / 3.0) * static_cast<vmm::core::Real>(std::numbers::pi_v<long double>) *
           radius_ * radius_ * radius_;
}

inline Bounds3D Sphere3D::bounds() const noexcept
{
    return Bounds3D{{centre_.x - radius_, centre_.y - radius_, centre_.z - radius_},
                    {centre_.x + radius_, centre_.y + radius_, centre_.z + radius_}};
}

inline bool Sphere3D::contains(Point3D point) const noexcept
{
    const auto dx = point.x - centre_.x;
    const auto dy = point.y - centre_.y;
    const auto dz = point.z - centre_.z;
    return (dx * dx) + (dy * dy) + (dz * dz) <= radius_ * radius_;
}

[[nodiscard]] inline Sphere3D make_sphere_3d(vmm::core::Real radius, Point3D centre = {})
{
    return Sphere3D{radius, centre};
}

} // namespace vmm::domain
