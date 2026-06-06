//==============================================================================
// File        : FiniteVolumeMeshBuilder2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Canonical 2D FV mesh reconstruction from planar cells.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

namespace vmm::mesh {
namespace {

using vmm::core::BoundaryPatchId;
using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::NodeId;
using vmm::core::SiteId;
using vmm::core::Real;
using vmm::domain::Point2D;

struct EdgeKey final {
    NodeId first{};
    NodeId second{};

    [[nodiscard]] friend bool operator<(EdgeKey lhs, EdgeKey rhs) noexcept
    {
        if(lhs.first.value != rhs.first.value) {
            return lhs.first.value < rhs.first.value;
        }
        return lhs.second.value < rhs.second.value;
    }
};

[[nodiscard]] bool same_point(Point2D lhs, Point2D rhs, Real tolerance) noexcept
{
    return std::abs(lhs.x - rhs.x) <= tolerance && std::abs(lhs.y - rhs.y) <= tolerance;
}

[[nodiscard]] NodeId find_or_add_node(FiniteVolumeMesh2D& mesh, Point2D point, Real tolerance)
{
    for(std::size_t node = 0U; node < mesh.node_count(); ++node) {
        if(same_point({mesh.nodes.x[node], mesh.nodes.y[node]}, point, tolerance)) {
            return NodeId{node};
        }
    }

    const auto node_id = NodeId{mesh.node_count()};
    mesh.nodes.x.push_back(point.x);
    mesh.nodes.y.push_back(point.y);
    return node_id;
}

[[nodiscard]] EdgeKey make_edge_key(NodeId node0, NodeId node1) noexcept
{
    if(node1.value < node0.value) {
        std::swap(node0, node1);
    }
    return EdgeKey{node0, node1};
}

[[nodiscard]] Real edge_length(Point2D a, Point2D b) noexcept
{
    const auto dx = b.x - a.x;
    const auto dy = b.y - a.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

[[nodiscard]] Point2D node_point(const FiniteVolumeMesh2D& mesh, NodeId node_id) noexcept
{
    const auto index = static_cast<std::size_t>(node_id.value);
    return {mesh.nodes.x[index], mesh.nodes.y[index]};
}

[[nodiscard]] Point2D face_centre(Point2D a, Point2D b) noexcept
{
    return {(a.x + b.x) / 2.0, (a.y + b.y) / 2.0};
}

[[nodiscard]] Point2D outward_unit_normal(Point2D a, Point2D b, Real signed_area) noexcept
{
    const auto dx = b.x - a.x;
    const auto dy = b.y - a.y;
    const auto length = edge_length(a, b);
    if(length == Real{}) {
        return {};
    }

    if(signed_area >= Real{}) {
        return {dy / length, -dx / length};
    }
    return {-dy / length, dx / length};
}

[[nodiscard]] std::size_t visible_vertex_count(const vmm::domain::Polygon2D& polygon) noexcept
{
    return polygon.is_closed() ? polygon.vertex_count() - 1U : polygon.vertex_count();
}

[[nodiscard]] Real cross(Point2D a, Point2D b, Point2D c) noexcept
{
    return ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
}

[[nodiscard]] bool point_on_segment(Point2D segment0, Point2D segment1, Point2D point, Real tolerance) noexcept
{
    const auto dx = segment1.x - segment0.x;
    const auto dy = segment1.y - segment0.y;
    const auto length = std::sqrt((dx * dx) + (dy * dy));
    const auto scaled_tolerance = tolerance * std::max(Real{1.0}, length);
    if(std::abs(cross(segment0, segment1, point)) > scaled_tolerance) {
        return false;
    }

    const auto dot = ((point.x - segment0.x) * dx) + ((point.y - segment0.y) * dy);
    const auto length_squared = (dx * dx) + (dy * dy);
    return dot >= -scaled_tolerance && dot <= length_squared + scaled_tolerance;
}

[[nodiscard]] bool edge_on_segment(Point2D face0,
                                   Point2D face1,
                                   Point2D segment0,
                                   Point2D segment1,
                                   Real tolerance) noexcept
{
    return point_on_segment(segment0, segment1, face0, tolerance)
        && point_on_segment(segment0, segment1, face1, tolerance);
}

[[nodiscard]] bool edge_on_ring(Point2D face0,
                                Point2D face1,
                                const vmm::domain::Polygon2D& ring,
                                Real tolerance) noexcept
{
    const auto vertices = ring.vertices();
    const auto count = visible_vertex_count(ring);
    for(std::size_t vertex = 0U; vertex < count; ++vertex) {
        const auto segment0 = vertices[vertex];
        const auto segment1 = vertices[(vertex + 1U) % count];
        if(edge_on_segment(face0, face1, segment0, segment1, tolerance)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] BoundaryPatchId classify_boundary_patch(Point2D face0,
                                                      Point2D face1,
                                                      const FiniteVolumeMeshBuilder2DOptions& options)
{
    for(std::size_t patch = 0U; patch < options.boundary_patches.size(); ++patch) {
        if(edge_on_ring(face0, face1, options.boundary_patches[patch].ring, options.boundary_tolerance)) {
            return BoundaryPatchId{patch + 1U};
        }
    }
    return BoundaryPatchId{0U};
}

void add_patch(FiniteVolumeMesh2D& mesh, std::string name, BoundaryPatchType type)
{
    mesh.patches.id.push_back(BoundaryPatchId{mesh.patches.size()});
    mesh.patches.name.push_back(std::move(name));
    mesh.patches.type.push_back(type);
}

void add_boundary_patches(FiniteVolumeMesh2D& mesh, const FiniteVolumeMeshBuilder2DOptions& options)
{
    add_patch(mesh, options.default_boundary_patch_name, options.default_boundary_patch_type);
    for(const auto& patch : options.boundary_patches) {
        add_patch(mesh, patch.name, patch.type);
    }
}

void add_cell(FiniteVolumeMesh2D& mesh, const vmm::domain::PlanarCell2D& cell)
{
    const auto centre = cell.centroid();
    mesh.cells.centre_x.push_back(centre.x);
    mesh.cells.centre_y.push_back(centre.y);
    mesh.cells.area.push_back(cell.area());
    mesh.cells.site_id.push_back(SiteId{cell.generator_id()});
    mesh.cells.face_offset.push_back(static_cast<std::uint64_t>(mesh.cell_faces.size()));
    mesh.cells.face_count.push_back(0U);
}

void add_face(FiniteVolumeMesh2D& mesh,
              CellId owner,
              NodeId node0,
              NodeId node1,
              Real signed_area,
              BoundaryPatchId patch_id)
{
    const auto a = node_point(mesh, node0);
    const auto b = node_point(mesh, node1);
    const auto centre = face_centre(a, b);
    const auto normal = outward_unit_normal(a, b, signed_area);

    mesh.faces.owner.push_back(owner);
    mesh.faces.neighbour.push_back(CellId::invalid());
    mesh.faces.centre_x.push_back(centre.x);
    mesh.faces.centre_y.push_back(centre.y);
    mesh.faces.normal_x.push_back(normal.x);
    mesh.faces.normal_y.push_back(normal.y);
    mesh.faces.length.push_back(edge_length(a, b));
    mesh.faces.patch_id.push_back(patch_id);
    mesh.faces.node0.push_back(node0);
    mesh.faces.node1.push_back(node1);
}

} // namespace

FiniteVolumeMesh2D FiniteVolumeMeshBuilder2D::build(std::span<const vmm::domain::PlanarCell2D> cells,
                                                     const FiniteVolumeMeshBuilder2DOptions& options) const
{
    if(options.vertex_tolerance < Real{}) {
        vmm::error::throw_invalid_argument("FiniteVolumeMeshBuilder2D", "Vertex tolerance must be non-negative.");
    }

    if(options.boundary_tolerance < Real{}) {
        vmm::error::throw_invalid_argument("FiniteVolumeMeshBuilder2D", "Boundary tolerance must be non-negative.");
    }

    FiniteVolumeMesh2D mesh;
    add_boundary_patches(mesh, options);
    std::map<EdgeKey, FaceId> edge_to_face;

    for(std::size_t cell_index = 0U; cell_index < cells.size(); ++cell_index) {
        const auto& cell = cells[cell_index];
        const auto cell_id = CellId{cell_index};
        add_cell(mesh, cell);

        const auto& exterior = cell.exterior();
        const auto vertices = exterior.vertices();
        const auto count = visible_vertex_count(exterior);
        const auto signed_area = exterior.signed_area();

        for(std::size_t vertex = 0U; vertex < count; ++vertex) {
            const auto a = vertices[vertex];
            const auto b = vertices[(vertex + 1U) % count];
            const auto node0 = find_or_add_node(mesh, a, options.vertex_tolerance);
            const auto node1 = find_or_add_node(mesh, b, options.vertex_tolerance);
            const auto key = make_edge_key(node0, node1);
            const auto existing = edge_to_face.find(key);

            FaceId face_id{};
            if(existing == edge_to_face.end()) {
                face_id = FaceId{mesh.face_count()};
                const auto patch_id =
                    classify_boundary_patch(node_point(mesh, node0), node_point(mesh, node1), options);
                add_face(mesh, cell_id, node0, node1, signed_area, patch_id);
                edge_to_face.emplace(key, face_id);
            } else {
                face_id = existing->second;
                const auto face_index = static_cast<std::size_t>(face_id.value);
                if(vmm::core::is_valid(mesh.faces.neighbour[face_index])) {
                    vmm::error::throw_invalid_argument("FiniteVolumeMeshBuilder2D",
                                                       "An edge is shared by more than two cells.");
                }
                mesh.faces.neighbour[face_index] = cell_id;
                mesh.faces.patch_id[face_index] = BoundaryPatchId::invalid();
            }

            mesh.cell_faces.face_ids.push_back(face_id);
            ++mesh.cells.face_count[cell_index];
        }
    }

    return mesh;
}

FiniteVolumeMesh2D build_finite_volume_mesh_2d(std::span<const vmm::domain::PlanarCell2D> cells,
                                               const FiniteVolumeMeshBuilder2DOptions& options)
{
    return FiniteVolumeMeshBuilder2D{}.build(cells, options);
}

} // namespace vmm::mesh
