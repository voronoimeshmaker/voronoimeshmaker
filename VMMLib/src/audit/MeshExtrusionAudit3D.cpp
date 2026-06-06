//==============================================================================
// File        : MeshExtrusionAudit3D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Basic audit for extruded 3D FV meshes.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshExtrusionAudit3D.hpp>

namespace vmm::audit {
namespace {

using vmm::audit::MeshAuditSeverityTraits;
using vmm::core::BoundaryPatchId;
using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::NodeId;
using vmm::core::is_valid;

void add_error(MeshAuditReport& report, std::string code, std::string message)
{
    report.add(MeshAuditSeverityTraits::Error, std::move(code), std::move(message));
}

} // namespace

MeshAuditReport MeshExtrusionAudit3D::run(const vmm::mesh::FiniteVolumeMesh3D& mesh) const
{
    MeshAuditReport report;

    if(!mesh.has_consistent_storage()) {
        add_error(report, "3d-inconsistent-table-storage", "3D mesh tables have inconsistent storage.");
        return report;
    }

    if(mesh.cell_faces.size() == 0U && mesh.cell_count() > 0U) {
        add_error(report, "3d-empty-cell-face-connectivity", "3D cell-face connectivity is empty.");
    }

    for(std::size_t node = 0U; node < mesh.node_count(); ++node) {
        if(!std::isfinite(mesh.nodes.x[node]) || !std::isfinite(mesh.nodes.y[node]) ||
           !std::isfinite(mesh.nodes.z[node])) {
            auto issue = make_cell_issue(MeshAuditSeverityTraits::Error,
                                         "3d-non-finite-node",
                                         "3D node coordinates must be finite.",
                                         CellId{});
            issue.node_id = NodeId{node};
            report.add(std::move(issue));
        }
    }

    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto cell_id = CellId{cell};
        if(!std::isfinite(mesh.cells.centre_x[cell]) || !std::isfinite(mesh.cells.centre_y[cell]) ||
           !std::isfinite(mesh.cells.centre_z[cell])) {
            report.add(make_cell_issue(MeshAuditSeverityTraits::Error,
                                       "3d-non-finite-cell-centre",
                                       "3D cell centre coordinates must be finite.",
                                       cell_id));
        }
        if(!(mesh.cells.volume[cell] > 0.0) || !std::isfinite(mesh.cells.volume[cell])) {
            report.add(make_cell_issue(MeshAuditSeverityTraits::Error,
                                       "3d-non-positive-cell-volume",
                                       "3D cell volume must be positive and finite.",
                                       cell_id));
        }
        const auto offset = mesh.cells.face_offset[cell];
        const auto count = mesh.cells.face_count[cell];
        if(offset > mesh.cell_faces.size() || count > mesh.cell_faces.size() - static_cast<std::size_t>(offset)) {
            report.add(make_cell_issue(MeshAuditSeverityTraits::Error,
                                       "3d-invalid-cell-face-span",
                                       "3D cell face span exceeds connectivity storage.",
                                       cell_id));
        }
        if(count < 5U) {
            report.add(make_cell_issue(MeshAuditSeverityTraits::Error,
                                       "3d-too-few-cell-faces",
                                       "Extruded 3D cells require at least five faces.",
                                       cell_id));
        }
    }

    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        if(!mesh.contains(mesh.faces.owner[face])) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-invalid-face-owner",
                                       "3D face owner is outside cell table.",
                                       face_id));
        }
        if(is_valid(mesh.faces.neighbour[face]) && !mesh.contains(mesh.faces.neighbour[face])) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-invalid-face-neighbour",
                                       "3D face neighbour is outside cell table.",
                                       face_id));
        }
        if(!(mesh.faces.area[face] > 0.0) || !std::isfinite(mesh.faces.area[face])) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-non-positive-face-area",
                                       "3D face area must be positive and finite.",
                                       face_id));
        }
        if(!std::isfinite(mesh.faces.normal_x[face]) || !std::isfinite(mesh.faces.normal_y[face]) ||
           !std::isfinite(mesh.faces.normal_z[face])) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-non-finite-face-normal",
                                       "3D face normal must be finite.",
                                       face_id));
        }
        if(is_valid(mesh.faces.neighbour[face]) && is_valid(mesh.faces.patch_id[face])) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-internal-face-with-patch",
                                       "Internal 3D faces must not reference boundary patches.",
                                       face_id));
        }
        if(!is_valid(mesh.faces.neighbour[face])) {
            if(!is_valid(mesh.faces.patch_id[face]) || !mesh.contains(mesh.faces.patch_id[face])) {
                report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                           "3d-boundary-face-without-patch",
                                           "Boundary 3D faces must reference a valid boundary patch.",
                                           face_id));
            }
        }
        const auto offset = mesh.faces.node_offset[face];
        const auto count = mesh.faces.node_count[face];
        if(offset > mesh.face_nodes.size() || count > mesh.face_nodes.size() - static_cast<std::size_t>(offset)) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-invalid-face-node-span",
                                       "3D face node span exceeds connectivity storage.",
                                       face_id));
        }
        if(count < 3U) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-too-few-face-nodes",
                                       "3D faces require at least three nodes.",
                                       face_id));
        }
    }

    for(const auto face_id : mesh.cell_faces.faces()) {
        if(!mesh.contains(face_id)) {
            report.add(make_face_issue(MeshAuditSeverityTraits::Error,
                                       "3d-invalid-cell-face-reference",
                                       "3D cell-face connectivity references an invalid face.",
                                       face_id));
        }
    }

    for(const auto node_id : mesh.face_nodes.nodes()) {
        if(!mesh.contains(node_id)) {
            auto issue = make_face_issue(MeshAuditSeverityTraits::Error,
                                         "3d-invalid-face-node-reference",
                                         "3D face-node connectivity references an invalid node.",
                                         FaceId{});
            issue.node_id = node_id;
            report.add(std::move(issue));
        }
    }

    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        if(mesh.patches.id[patch] != BoundaryPatchId{patch}) {
            auto issue = make_face_issue(MeshAuditSeverityTraits::Error,
                                         "3d-invalid-patch-row-id",
                                         "3D patch row id must match its table row.",
                                         FaceId{});
            issue.patch_id = BoundaryPatchId{patch};
            report.add(std::move(issue));
        }
    }

    return report;
}

MeshAuditReport audit_extruded_mesh_3d(const vmm::mesh::FiniteVolumeMesh3D& mesh)
{
    return MeshExtrusionAudit3D{}.run(mesh);
}

} // namespace vmm::audit
