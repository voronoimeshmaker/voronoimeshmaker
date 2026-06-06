//==============================================================================
// File        : FiniteVolumeStencilGraph2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Solver-neutral 2D FV stencil graph builder implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <string>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/FiniteVolumeStencilGraph2D.hpp>

namespace vmm::mesh {
namespace {

using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::Real;
using vmm::core::is_valid;

[[nodiscard]] Real cell_dx(const FiniteVolumeMesh2D& mesh, CellId owner, CellId neighbour) noexcept
{
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto neighbour_index = static_cast<std::size_t>(neighbour.value);
    return mesh.cells.centre_x[neighbour_index] - mesh.cells.centre_x[owner_index];
}

[[nodiscard]] Real cell_dy(const FiniteVolumeMesh2D& mesh, CellId owner, CellId neighbour) noexcept
{
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto neighbour_index = static_cast<std::size_t>(neighbour.value);
    return mesh.cells.centre_y[neighbour_index] - mesh.cells.centre_y[owner_index];
}

[[nodiscard]] Real boundary_distance(const FiniteVolumeMesh2D& mesh, std::size_t face_index, CellId owner) noexcept
{
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto dx = mesh.faces.centre_x[face_index] - mesh.cells.centre_x[owner_index];
    const auto dy = mesh.faces.centre_y[face_index] - mesh.cells.centre_y[owner_index];
    const auto projection = (dx * mesh.faces.normal_x[face_index]) + (dy * mesh.faces.normal_y[face_index]);
    return std::abs(projection);
}

void validate_mesh_for_stencils(const FiniteVolumeMesh2D& mesh)
{
    const auto geometry_report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    if(!geometry_report.valid()) {
        const auto code = geometry_report.empty() ? std::string{"unknown"} : geometry_report.issues.front().code;
        vmm::error::throw_invalid_argument("FiniteVolumeStencilGraphBuilder2D",
                                           "Cannot build stencil graph from invalid mesh. First geometry issue: " + code);
    }

    const auto patch_report = vmm::audit::audit_mesh_patches_2d(mesh);
    if(!patch_report.valid()) {
        const auto code = patch_report.empty() ? std::string{"unknown"} : patch_report.issues.front().code;
        vmm::error::throw_invalid_argument("FiniteVolumeStencilGraphBuilder2D",
                                           "Cannot build stencil graph from invalid mesh. First patch issue: " + code);
    }
}

void add_internal_stencil(FiniteVolumeStencilGraph2D& graph,
                          const FiniteVolumeMesh2D& mesh,
                          FaceId face_id,
                          std::size_t face_index)
{
    const auto owner = mesh.faces.owner[face_index];
    const auto neighbour = mesh.faces.neighbour[face_index];
    const auto dx = cell_dx(mesh, owner, neighbour);
    const auto dy = cell_dy(mesh, owner, neighbour);

    graph.internal.face_id.push_back(face_id);
    graph.internal.owner.push_back(owner);
    graph.internal.neighbour.push_back(neighbour);
    graph.internal.centre_distance.push_back(std::hypot(dx, dy));
    graph.internal.owner_to_neighbour_x.push_back(dx);
    graph.internal.owner_to_neighbour_y.push_back(dy);
    graph.internal.face_length.push_back(mesh.faces.length[face_index]);
}

void add_boundary_stencil(FiniteVolumeStencilGraph2D& graph,
                          const FiniteVolumeMesh2D& mesh,
                          FaceId face_id,
                          std::size_t face_index)
{
    const auto owner = mesh.faces.owner[face_index];
    graph.boundary.face_id.push_back(face_id);
    graph.boundary.cell.push_back(owner);
    graph.boundary.patch_id.push_back(mesh.faces.patch_id[face_index]);
    graph.boundary.centre_to_face_distance.push_back(boundary_distance(mesh, face_index, owner));
    graph.boundary.normal_x.push_back(mesh.faces.normal_x[face_index]);
    graph.boundary.normal_y.push_back(mesh.faces.normal_y[face_index]);
    graph.boundary.face_length.push_back(mesh.faces.length[face_index]);
}

} // namespace

bool InternalStencilTable2D::has_consistent_storage() const noexcept
{
    return face_id.size() == owner.size() && face_id.size() == neighbour.size() &&
           face_id.size() == centre_distance.size() && face_id.size() == owner_to_neighbour_x.size() &&
           face_id.size() == owner_to_neighbour_y.size() && face_id.size() == face_length.size();
}

bool BoundaryStencilTable2D::has_consistent_storage() const noexcept
{
    return face_id.size() == cell.size() && face_id.size() == patch_id.size() &&
           face_id.size() == centre_to_face_distance.size() && face_id.size() == normal_x.size() &&
           face_id.size() == normal_y.size() && face_id.size() == face_length.size();
}

bool FiniteVolumeStencilGraph2D::has_consistent_storage() const noexcept
{
    return internal.has_consistent_storage() && boundary.has_consistent_storage();
}

FiniteVolumeStencilGraph2D FiniteVolumeStencilGraphBuilder2D::build(
    const FiniteVolumeMesh2D& mesh,
    const FiniteVolumeStencilGraph2DOptions& options) const
{
    if(options.require_valid_mesh) {
        validate_mesh_for_stencils(mesh);
    }

    FiniteVolumeStencilGraph2D graph;
    graph.internal.face_id.reserve(mesh.face_count());
    graph.internal.owner.reserve(mesh.face_count());
    graph.internal.neighbour.reserve(mesh.face_count());
    graph.internal.centre_distance.reserve(mesh.face_count());
    graph.internal.owner_to_neighbour_x.reserve(mesh.face_count());
    graph.internal.owner_to_neighbour_y.reserve(mesh.face_count());
    graph.internal.face_length.reserve(mesh.face_count());
    graph.boundary.face_id.reserve(mesh.face_count());
    graph.boundary.cell.reserve(mesh.face_count());
    graph.boundary.patch_id.reserve(mesh.face_count());
    graph.boundary.centre_to_face_distance.reserve(mesh.face_count());
    graph.boundary.normal_x.reserve(mesh.face_count());
    graph.boundary.normal_y.reserve(mesh.face_count());
    graph.boundary.face_length.reserve(mesh.face_count());

    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        if(is_valid(mesh.faces.neighbour[face])) {
            add_internal_stencil(graph, mesh, face_id, face);
        } else {
            add_boundary_stencil(graph, mesh, face_id, face);
        }
    }

    return graph;
}

FiniteVolumeStencilGraph2D build_finite_volume_stencil_graph_2d(
    const FiniteVolumeMesh2D& mesh,
    const FiniteVolumeStencilGraph2DOptions& options)
{
    return FiniteVolumeStencilGraphBuilder2D{}.build(mesh, options);
}

} // namespace vmm::mesh
