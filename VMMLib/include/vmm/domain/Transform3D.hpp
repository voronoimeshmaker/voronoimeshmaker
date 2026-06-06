//==============================================================================
// File        : Transform3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Basic rigid transforms for 3D points and surface meshes.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/SurfaceMesh3D.hpp>

namespace vmm::domain {

[[nodiscard]] inline Point3D translate(Point3D point,
                                       vmm::core::Real dx,
                                       vmm::core::Real dy,
                                       vmm::core::Real dz) noexcept
{
    return Point3D{point.x + dx, point.y + dy, point.z + dz};
}

[[nodiscard]] inline Point3D rotate_about_z(Point3D point,
                                            vmm::core::Real angle_radians,
                                            Point3D centre = {}) noexcept
{
    const auto cos_angle = std::cos(angle_radians);
    const auto sin_angle = std::sin(angle_radians);
    const auto local_x = point.x - centre.x;
    const auto local_y = point.y - centre.y;
    return Point3D{
        centre.x + (local_x * cos_angle) - (local_y * sin_angle),
        centre.y + (local_x * sin_angle) + (local_y * cos_angle),
        point.z};
}

[[nodiscard]] inline SurfaceMesh3D translated(const SurfaceMesh3D& surface,
                                              vmm::core::Real dx,
                                              vmm::core::Real dy,
                                              vmm::core::Real dz)
{
    std::vector<Point3D> vertices;
    vertices.reserve(surface.vertex_count());
    for(const auto vertex : surface.vertices()) {
        vertices.push_back(translate(vertex, dx, dy, dz));
    }
    return SurfaceMesh3D{std::move(vertices),
                         std::vector<TriangleFace3D>{surface.faces().begin(), surface.faces().end()},
                         std::vector<SurfacePatch>{surface.patches().begin(), surface.patches().end()}};
}

} // namespace vmm::domain
