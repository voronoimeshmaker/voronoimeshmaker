//==============================================================================
// File        : SurfaceMesh3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Lightweight triangular surface mesh for 3D domain boundaries.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>
#include <vmm/domain/SurfacePatch.hpp>
#include <vmm/error/MeshException.hpp>

namespace vmm::domain {

using Point3D = vmm::core::Point<vmm::core::Dimension::D3>;

struct Bounds3D final {
    Point3D min{};
    Point3D max{};
};

struct TriangleFace3D final {
    std::array<std::size_t, 3U> vertex{};
    std::size_t patch{};
};

class SurfaceMesh3D final {
public:
    SurfaceMesh3D() = default;
    SurfaceMesh3D(std::vector<Point3D> vertices,
                  std::vector<TriangleFace3D> faces,
                  std::vector<SurfacePatch> patches = {});

    [[nodiscard]] std::span<const Point3D> vertices() const noexcept { return vertices_; }
    [[nodiscard]] std::span<const TriangleFace3D> faces() const noexcept { return faces_; }
    [[nodiscard]] std::span<const SurfacePatch> patches() const noexcept { return patches_; }
    [[nodiscard]] std::size_t vertex_count() const noexcept { return vertices_.size(); }
    [[nodiscard]] std::size_t face_count() const noexcept { return faces_.size(); }
    [[nodiscard]] std::size_t patch_count() const noexcept { return patches_.size(); }
    [[nodiscard]] Bounds3D bounds() const;
    void validate_or_throw() const;

private:
    std::vector<Point3D> vertices_;
    std::vector<TriangleFace3D> faces_;
    std::vector<SurfacePatch> patches_;
};

inline SurfaceMesh3D::SurfaceMesh3D(std::vector<Point3D> vertices,
                                    std::vector<TriangleFace3D> faces,
                                    std::vector<SurfacePatch> patches)
    : vertices_(std::move(vertices)),
      faces_(std::move(faces)),
      patches_(std::move(patches))
{
    validate_or_throw();
}

inline Bounds3D SurfaceMesh3D::bounds() const
{
    if(vertices_.empty()) {
        vmm::error::throw_invalid_argument("SurfaceMesh3D", "Cannot compute bounds of an empty surface mesh.");
    }

    Bounds3D result{vertices_.front(), vertices_.front()};
    for(const auto vertex : vertices_) {
        result.min.x = std::min(result.min.x, vertex.x);
        result.min.y = std::min(result.min.y, vertex.y);
        result.min.z = std::min(result.min.z, vertex.z);
        result.max.x = std::max(result.max.x, vertex.x);
        result.max.y = std::max(result.max.y, vertex.y);
        result.max.z = std::max(result.max.z, vertex.z);
    }
    return result;
}

inline void SurfaceMesh3D::validate_or_throw() const
{
    if(vertices_.empty()) {
        vmm::error::throw_invalid_argument("SurfaceMesh3D", "A surface mesh requires at least one vertex.");
    }
    for(const auto vertex : vertices_) {
        if(!std::isfinite(vertex.x) || !std::isfinite(vertex.y) || !std::isfinite(vertex.z)) {
            vmm::error::throw_invalid_argument("SurfaceMesh3D", "Surface mesh coordinates must be finite.");
        }
    }
    for(const auto face : faces_) {
        for(const auto vertex_id : face.vertex) {
            if(vertex_id >= vertices_.size()) {
                vmm::error::throw_invalid_argument("SurfaceMesh3D", "Surface face references an invalid vertex.");
            }
        }
        if(face.vertex[0] == face.vertex[1] || face.vertex[1] == face.vertex[2] || face.vertex[2] == face.vertex[0]) {
            vmm::error::throw_invalid_argument("SurfaceMesh3D", "Surface face has repeated vertices.");
        }
        if(!patches_.empty() && face.patch >= patches_.size()) {
            vmm::error::throw_invalid_argument("SurfaceMesh3D", "Surface face references an invalid patch.");
        }
    }
}

} // namespace vmm::domain
