//==============================================================================
// File        : ExtrudedFiniteVolumeMesh3D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Vertical extrusion from 2D FV meshes to 3D FV meshes.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/ExtrudedFiniteVolumeMesh3D.hpp>

namespace vmm::mesh {
namespace {

using vmm::core::BoundaryPatchId;
using vmm::core::CellId;
using vmm::core::EntityIdValue;
using vmm::core::FaceId;
using vmm::core::NodeId;
using vmm::core::Real;
using vmm::core::is_valid;

[[nodiscard]] NodeId node_3d_id(std::size_t level, std::size_t node_count_2d, NodeId node_id_2d) noexcept
{
    return NodeId{static_cast<EntityIdValue>((level * node_count_2d) + static_cast<std::size_t>(node_id_2d.value))};
}

[[nodiscard]] CellId cell_3d_id(std::size_t layer, std::size_t cell_count_2d, CellId cell_id_2d) noexcept
{
    return CellId{static_cast<EntityIdValue>((layer * cell_count_2d) + static_cast<std::size_t>(cell_id_2d.value))};
}

[[nodiscard]] FaceId lateral_face_id(std::size_t layer, std::size_t face_count_2d, std::size_t face_index) noexcept
{
    return FaceId{static_cast<EntityIdValue>((layer * face_count_2d) + face_index)};
}

[[nodiscard]] FaceId horizontal_face_id(std::size_t layer_count,
                                        std::size_t face_count_2d,
                                        std::size_t cell_count_2d,
                                        std::size_t level,
                                        std::size_t cell_index) noexcept
{
    return FaceId{static_cast<EntityIdValue>((layer_count * face_count_2d) + (level * cell_count_2d) + cell_index)};
}

void validate_z_interfaces(std::span<const Real> z_interfaces)
{
    if(z_interfaces.size() < 2U) {
        vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                           "At least two z interfaces are required for extrusion.");
    }
    for(std::size_t i = 0U; i < z_interfaces.size(); ++i) {
        if(!std::isfinite(z_interfaces[i])) {
            vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                               "Extrusion z interfaces must be finite.");
        }
        if(i > 0U && !(z_interfaces[i] > z_interfaces[i - 1U])) {
            vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                               "Extrusion z interfaces must be strictly increasing.");
        }
    }
}

void validate_2d_mesh(const FiniteVolumeMesh2D& mesh2d)
{
    const auto geometry_report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh2d);
    if(!geometry_report.valid()) {
        const auto code = geometry_report.empty() ? std::string{"unknown"} : geometry_report.issues.front().code;
        vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                           "Cannot extrude invalid 2D mesh. First geometry issue: " + code);
    }

    const auto patch_report = vmm::audit::audit_mesh_patches_2d(mesh2d);
    if(!patch_report.valid()) {
        const auto code = patch_report.empty() ? std::string{"unknown"} : patch_report.issues.front().code;
        vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                           "Cannot extrude invalid 2D mesh. First patch issue: " + code);
    }
}

BoundaryPatchId add_patch(BoundaryPatchTable& patches, std::string name, BoundaryPatchType type)
{
    const auto patch_id = BoundaryPatchId{static_cast<EntityIdValue>(patches.size())};
    patches.id.push_back(patch_id);
    patches.name.push_back(std::move(name));
    patches.type.push_back(type);
    return patch_id;
}

FaceId add_face(FiniteVolumeMesh3D& mesh,
                CellId owner,
                CellId neighbour,
                Real centre_x,
                Real centre_y,
                Real centre_z,
                Real normal_x,
                Real normal_y,
                Real normal_z,
                Real area,
                BoundaryPatchId patch_id,
                std::span<const NodeId> nodes,
                FaceRole3D role)
{
    const auto face_id = FaceId{static_cast<EntityIdValue>(mesh.faces.size())};
    mesh.faces.owner.push_back(owner);
    mesh.faces.neighbour.push_back(neighbour);
    mesh.faces.centre_x.push_back(centre_x);
    mesh.faces.centre_y.push_back(centre_y);
    mesh.faces.centre_z.push_back(centre_z);
    mesh.faces.normal_x.push_back(normal_x);
    mesh.faces.normal_y.push_back(normal_y);
    mesh.faces.normal_z.push_back(normal_z);
    mesh.faces.area.push_back(area);
    mesh.faces.patch_id.push_back(patch_id);
    mesh.faces.node_offset.push_back(static_cast<std::uint64_t>(mesh.face_nodes.size()));
    mesh.faces.node_count.push_back(static_cast<std::uint32_t>(nodes.size()));
    mesh.faces.role.push_back(role);
    for(const auto node_id : nodes) {
        mesh.face_nodes.node_ids.push_back(node_id);
    }
    return face_id;
}

void add_extruded_nodes(FiniteVolumeMesh3D& mesh, const FiniteVolumeMesh2D& mesh2d, std::span<const Real> z_interfaces)
{
    mesh.nodes.x.reserve(mesh2d.node_count() * z_interfaces.size());
    mesh.nodes.y.reserve(mesh2d.node_count() * z_interfaces.size());
    mesh.nodes.z.reserve(mesh2d.node_count() * z_interfaces.size());

    for(const auto z : z_interfaces) {
        for(std::size_t node = 0U; node < mesh2d.node_count(); ++node) {
            mesh.nodes.x.push_back(mesh2d.nodes.x[node]);
            mesh.nodes.y.push_back(mesh2d.nodes.y[node]);
            mesh.nodes.z.push_back(z);
        }
    }
}

void copy_2d_patches(FiniteVolumeMesh3D& mesh, const FiniteVolumeMesh2D& mesh2d)
{
    mesh.patches.id = mesh2d.patches.id;
    mesh.patches.name = mesh2d.patches.name;
    mesh.patches.type = mesh2d.patches.type;
}

void add_lateral_faces(FiniteVolumeMesh3D& mesh,
                       const FiniteVolumeMesh2D& mesh2d,
                       std::span<const Real> z_interfaces,
                       std::size_t layer_count)
{
    const auto node_count_2d = mesh2d.node_count();
    const auto cell_count_2d = mesh2d.cell_count();
    for(std::size_t layer = 0U; layer < layer_count; ++layer) {
        const auto height = z_interfaces[layer + 1U] - z_interfaces[layer];
        const auto z_centre = (z_interfaces[layer] + z_interfaces[layer + 1U]) / 2.0;
        for(std::size_t face = 0U; face < mesh2d.face_count(); ++face) {
            const auto owner = cell_3d_id(layer, cell_count_2d, mesh2d.faces.owner[face]);
            const auto neighbour = is_valid(mesh2d.faces.neighbour[face])
                ? cell_3d_id(layer, cell_count_2d, mesh2d.faces.neighbour[face])
                : CellId{};
            const NodeId nodes[] = {
                node_3d_id(layer, node_count_2d, mesh2d.faces.node0[face]),
                node_3d_id(layer, node_count_2d, mesh2d.faces.node1[face]),
                node_3d_id(layer + 1U, node_count_2d, mesh2d.faces.node1[face]),
                node_3d_id(layer + 1U, node_count_2d, mesh2d.faces.node0[face])};
            add_face(mesh,
                     owner,
                     neighbour,
                     mesh2d.faces.centre_x[face],
                     mesh2d.faces.centre_y[face],
                     z_centre,
                     mesh2d.faces.normal_x[face],
                     mesh2d.faces.normal_y[face],
                     0.0,
                     mesh2d.faces.length[face] * height,
                     mesh2d.faces.patch_id[face],
                     nodes,
                     FaceRole3DTraits::Lateral);
        }
    }
}

void add_horizontal_faces(FiniteVolumeMesh3D& mesh,
                          const FiniteVolumeMesh2D& mesh2d,
                          std::span<const Real> z_interfaces,
                          std::size_t layer_count,
                          BoundaryPatchId bottom_patch_id,
                          BoundaryPatchId surface_patch_id)
{
    const auto node_count_2d = mesh2d.node_count();
    const auto cell_count_2d = mesh2d.cell_count();
    const auto face_count_2d = mesh2d.face_count();
    std::vector<NodeId> nodes;

    for(std::size_t level = 0U; level < z_interfaces.size(); ++level) {
        for(std::size_t cell = 0U; cell < cell_count_2d; ++cell) {
            nodes.clear();
            for(const auto face_id : mesh2d.faces_of(CellId{static_cast<EntityIdValue>(cell)})) {
                const auto face = static_cast<std::size_t>(face_id.value);
                nodes.push_back(node_3d_id(level, node_count_2d, mesh2d.faces.node0[face]));
            }

            CellId owner{};
            CellId neighbour{};
            BoundaryPatchId patch_id{};
            Real normal_z{};
            auto role = FaceRole3DTraits::InternalHorizontal;
            if(level == 0U) {
                owner = cell_3d_id(0U, cell_count_2d, CellId{static_cast<EntityIdValue>(cell)});
                patch_id = bottom_patch_id;
                normal_z = -1.0;
                role = FaceRole3DTraits::Bottom;
            } else if(level == layer_count) {
                owner = cell_3d_id(layer_count - 1U, cell_count_2d, CellId{static_cast<EntityIdValue>(cell)});
                patch_id = surface_patch_id;
                normal_z = 1.0;
                role = FaceRole3DTraits::Top;
            } else {
                owner = cell_3d_id(level - 1U, cell_count_2d, CellId{static_cast<EntityIdValue>(cell)});
                neighbour = cell_3d_id(level, cell_count_2d, CellId{static_cast<EntityIdValue>(cell)});
                patch_id = BoundaryPatchId{};
                normal_z = 1.0;
            }

            const auto face_id = add_face(mesh,
                                          owner,
                                          neighbour,
                                          mesh2d.cells.centre_x[cell],
                                          mesh2d.cells.centre_y[cell],
                                          z_interfaces[level],
                                          0.0,
                                          0.0,
                                          normal_z,
                                          mesh2d.cells.area[cell],
                                          patch_id,
                                          nodes,
                                          role);
            const auto expected = horizontal_face_id(layer_count, face_count_2d, cell_count_2d, level, cell);
            if(face_id != expected) {
                vmm::error::throw_invalid_argument("extrude_finite_volume_mesh_3d",
                                                   "Internal horizontal face indexing invariant failed.");
            }
        }
    }
}

void add_extruded_cells(FiniteVolumeMesh3D& mesh,
                        const FiniteVolumeMesh2D& mesh2d,
                        std::span<const Real> z_interfaces,
                        std::size_t layer_count)
{
    const auto cell_count_2d = mesh2d.cell_count();
    const auto face_count_2d = mesh2d.face_count();

    for(std::size_t layer = 0U; layer < layer_count; ++layer) {
        const auto height = z_interfaces[layer + 1U] - z_interfaces[layer];
        const auto z_centre = (z_interfaces[layer] + z_interfaces[layer + 1U]) / 2.0;
        for(std::size_t cell = 0U; cell < cell_count_2d; ++cell) {
            mesh.cells.centre_x.push_back(mesh2d.cells.centre_x[cell]);
            mesh.cells.centre_y.push_back(mesh2d.cells.centre_y[cell]);
            mesh.cells.centre_z.push_back(z_centre);
            mesh.cells.volume.push_back(mesh2d.cells.area[cell] * height);
            mesh.cells.site_id.push_back(mesh2d.cells.site_id[cell]);
            mesh.cells.face_offset.push_back(static_cast<std::uint64_t>(mesh.cell_faces.size()));
            mesh.cells.face_count.push_back(mesh2d.cells.face_count[cell] + 2U);

            for(const auto face_id : mesh2d.faces_of(CellId{static_cast<EntityIdValue>(cell)})) {
                mesh.cell_faces.face_ids.push_back(lateral_face_id(layer, face_count_2d, static_cast<std::size_t>(face_id.value)));
            }
            mesh.cell_faces.face_ids.push_back(horizontal_face_id(layer_count, face_count_2d, cell_count_2d, layer, cell));
            mesh.cell_faces.face_ids.push_back(horizontal_face_id(layer_count, face_count_2d, cell_count_2d, layer + 1U, cell));
        }
    }
}

} // namespace

FiniteVolumeMesh3D extrude_finite_volume_mesh_3d(
    const FiniteVolumeMesh2D& mesh2d,
    std::span<const Real> z_interfaces,
    const ExtrudedFiniteVolumeMesh3DOptions& options)
{
    if(options.require_valid_2d_mesh) {
        validate_2d_mesh(mesh2d);
    }
    validate_z_interfaces(z_interfaces);

    FiniteVolumeMesh3D mesh;
    copy_2d_patches(mesh, mesh2d);
    const auto bottom_patch_id = add_patch(mesh.patches, options.bottom_patch_name, BoundaryPatchTypeTraits::Bottom);
    const auto surface_patch_id = add_patch(mesh.patches, options.surface_patch_name, BoundaryPatchTypeTraits::Surface);
    const auto layer_count = z_interfaces.size() - 1U;

    add_extruded_nodes(mesh, mesh2d, z_interfaces);
    add_lateral_faces(mesh, mesh2d, z_interfaces, layer_count);
    add_horizontal_faces(mesh, mesh2d, z_interfaces, layer_count, bottom_patch_id, surface_patch_id);
    add_extruded_cells(mesh, mesh2d, z_interfaces, layer_count);

    return mesh;
}

} // namespace vmm::mesh
