//==============================================================================
// File        : Ellipsoid3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Axis-aligned ellipsoid 3D domain.
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

class Ellipsoid3D final {
public:
    Ellipsoid3D(vmm::core::Real radius_x,
                vmm::core::Real radius_y,
                vmm::core::Real radius_z,
                Point3D centre = {});

    [[nodiscard]] Point3D centre() const noexcept { return centre_; }
    [[nodiscard]] vmm::core::Real radius_x() const noexcept { return radius_x_; }
    [[nodiscard]] vmm::core::Real radius_y() const noexcept { return radius_y_; }
    [[nodiscard]] vmm::core::Real radius_z() const noexcept { return radius_z_; }
    [[nodiscard]] vmm::core::Real volume() const noexcept;
    [[nodiscard]] Bounds3D bounds() const noexcept;
    [[nodiscard]] bool contains(Point3D point) const noexcept;

private:
    vmm::core::Real radius_x_{};
    vmm::core::Real radius_y_{};
    vmm::core::Real radius_z_{};
    Point3D centre_{};
};

inline Ellipsoid3D::Ellipsoid3D(vmm::core::Real radius_x,
                                vmm::core::Real radius_y,
                                vmm::core::Real radius_z,
                                Point3D centre)
    : radius_x_(radius_x),
      radius_y_(radius_y),
      radius_z_(radius_z),
      centre_(centre)
{
    if(radius_x_ <= 0.0 || radius_y_ <= 0.0 || radius_z_ <= 0.0) {
        vmm::error::throw_invalid_argument("Ellipsoid3D", "Ellipsoid radii must be positive.");
    }
}

inline vmm::core::Real Ellipsoid3D::volume() const noexcept
{
    return (4.0 / 3.0) * static_cast<vmm::core::Real>(std::numbers::pi_v<long double>) *
           radius_x_ * radius_y_ * radius_z_;
}

inline Bounds3D Ellipsoid3D::bounds() const noexcept
{
    return Bounds3D{{centre_.x - radius_x_, centre_.y - radius_y_, centre_.z - radius_z_},
                    {centre_.x + radius_x_, centre_.y + radius_y_, centre_.z + radius_z_}};
}

inline bool Ellipsoid3D::contains(Point3D point) const noexcept
{
    const auto dx = (point.x - centre_.x) / radius_x_;
    const auto dy = (point.y - centre_.y) / radius_y_;
    const auto dz = (point.z - centre_.z) / radius_z_;
    return (dx * dx) + (dy * dy) + (dz * dz) <= 1.0;
}

[[nodiscard]] inline Ellipsoid3D make_ellipsoid_3d(vmm::core::Real radius_x,
                                                   vmm::core::Real radius_y,
                                                   vmm::core::Real radius_z,
                                                   Point3D centre = {})
{
    return Ellipsoid3D{radius_x, radius_y, radius_z, centre};
}

} // namespace vmm::domain
