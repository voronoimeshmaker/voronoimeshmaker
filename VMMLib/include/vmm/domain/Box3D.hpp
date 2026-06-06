//==============================================================================
// File        : Box3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Axis-aligned 3D box domain.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <vector>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Polyhedron3D.hpp>

namespace vmm::domain {

class Box3D final {
public:
    Box3D(vmm::core::Real width,
          vmm::core::Real height,
          vmm::core::Real depth,
          Point3D centre = {});

    [[nodiscard]] vmm::core::Real width() const noexcept { return width_; }
    [[nodiscard]] vmm::core::Real height() const noexcept { return height_; }
    [[nodiscard]] vmm::core::Real depth() const noexcept { return depth_; }
    [[nodiscard]] Point3D centre() const noexcept { return centre_; }
    [[nodiscard]] vmm::core::Real volume() const noexcept { return width_ * height_ * depth_; }
    [[nodiscard]] Bounds3D bounds() const noexcept;
    [[nodiscard]] bool contains(Point3D point) const noexcept;
    [[nodiscard]] Polyhedron3D to_polyhedron() const;

private:
    vmm::core::Real width_{};
    vmm::core::Real height_{};
    vmm::core::Real depth_{};
    Point3D centre_{};
};

inline Box3D::Box3D(vmm::core::Real width,
                    vmm::core::Real height,
                    vmm::core::Real depth,
                    Point3D centre)
    : width_(width),
      height_(height),
      depth_(depth),
      centre_(centre)
{
    if(width_ <= 0.0 || height_ <= 0.0 || depth_ <= 0.0) {
        vmm::error::throw_invalid_argument("Box3D", "Box dimensions must be positive.");
    }
}

inline Bounds3D Box3D::bounds() const noexcept
{
    const auto hx = width_ / 2.0;
    const auto hy = height_ / 2.0;
    const auto hz = depth_ / 2.0;
    return Bounds3D{{centre_.x - hx, centre_.y - hy, centre_.z - hz},
                    {centre_.x + hx, centre_.y + hy, centre_.z + hz}};
}

inline bool Box3D::contains(Point3D point) const noexcept
{
    const auto b = bounds();
    return point.x >= b.min.x && point.x <= b.max.x &&
           point.y >= b.min.y && point.y <= b.max.y &&
           point.z >= b.min.z && point.z <= b.max.z;
}

inline Polyhedron3D Box3D::to_polyhedron() const
{
    const auto b = bounds();
    std::vector<Point3D> vertices{
        {b.min.x, b.min.y, b.min.z},
        {b.max.x, b.min.y, b.min.z},
        {b.max.x, b.max.y, b.min.z},
        {b.min.x, b.max.y, b.min.z},
        {b.min.x, b.min.y, b.max.z},
        {b.max.x, b.min.y, b.max.z},
        {b.max.x, b.max.y, b.max.z},
        {b.min.x, b.max.y, b.max.z}};
    std::vector<TriangleFace3D> faces{
        {{0U, 2U, 1U}, 0U}, {{0U, 3U, 2U}, 0U},
        {{4U, 5U, 6U}, 0U}, {{4U, 6U, 7U}, 0U},
        {{0U, 1U, 5U}, 0U}, {{0U, 5U, 4U}, 0U},
        {{1U, 2U, 6U}, 0U}, {{1U, 6U, 5U}, 0U},
        {{2U, 3U, 7U}, 0U}, {{2U, 7U, 6U}, 0U},
        {{3U, 0U, 4U}, 0U}, {{3U, 4U, 7U}, 0U}};
    std::vector<SurfacePatch> patches{{"box-wall", BoundarySurfaceTypeTraits::Wall}};
    return Polyhedron3D{SurfaceMesh3D{std::move(vertices), std::move(faces), std::move(patches)}};
}

[[nodiscard]] inline Box3D make_box_3d(vmm::core::Real width,
                                       vmm::core::Real height,
                                       vmm::core::Real depth,
                                       Point3D centre = {})
{
    return Box3D{width, height, depth, centre};
}

} // namespace vmm::domain
