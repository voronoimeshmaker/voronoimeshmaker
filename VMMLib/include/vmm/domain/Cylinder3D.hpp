//==============================================================================
// File        : Cylinder3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Vertical circular cylinder 3D domain.
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

class Cylinder3D final {
public:
    Cylinder3D(vmm::core::Real radius, vmm::core::Real height, Point3D centre = {});

    [[nodiscard]] vmm::core::Real radius() const noexcept { return radius_; }
    [[nodiscard]] vmm::core::Real height() const noexcept { return height_; }
    [[nodiscard]] Point3D centre() const noexcept { return centre_; }
    [[nodiscard]] vmm::core::Real volume() const noexcept;
    [[nodiscard]] Bounds3D bounds() const noexcept;
    [[nodiscard]] bool contains(Point3D point) const noexcept;

private:
    vmm::core::Real radius_{};
    vmm::core::Real height_{};
    Point3D centre_{};
};

inline Cylinder3D::Cylinder3D(vmm::core::Real radius,
                              vmm::core::Real height,
                              Point3D centre)
    : radius_(radius),
      height_(height),
      centre_(centre)
{
    if(radius_ <= 0.0 || height_ <= 0.0) {
        vmm::error::throw_invalid_argument("Cylinder3D", "Cylinder radius and height must be positive.");
    }
}

inline vmm::core::Real Cylinder3D::volume() const noexcept
{
    return static_cast<vmm::core::Real>(std::numbers::pi_v<long double>) * radius_ * radius_ * height_;
}

inline Bounds3D Cylinder3D::bounds() const noexcept
{
    const auto hz = height_ / 2.0;
    return Bounds3D{{centre_.x - radius_, centre_.y - radius_, centre_.z - hz},
                    {centre_.x + radius_, centre_.y + radius_, centre_.z + hz}};
}

inline bool Cylinder3D::contains(Point3D point) const noexcept
{
    const auto dz = point.z - centre_.z;
    if(dz < -height_ / 2.0 || dz > height_ / 2.0) {
        return false;
    }
    const auto dx = point.x - centre_.x;
    const auto dy = point.y - centre_.y;
    return (dx * dx) + (dy * dy) <= radius_ * radius_;
}

[[nodiscard]] inline Cylinder3D make_cylinder_3d(vmm::core::Real radius,
                                                 vmm::core::Real height,
                                                 Point3D centre = {})
{
    return Cylinder3D{radius, height, centre};
}

} // namespace vmm::domain
