//==============================================================================
// File        : MeshIndexAudit2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the 2D finite-volume mesh index audit.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <string>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshIndexAudit2D.hpp>

namespace vmm::audit {
namespace {

using vmm::core::BoundaryPatchId;
using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::NodeId;
using vmm::core::is_valid;

constexpr auto FatalIssue = MeshAuditSeverityTraits::Fatal;

void add_table_storage_issue(MeshAuditReport& report, std::string table_name)
{
    report.add(FatalIssue, "VMM-IDX-000", table_name + " has inconsistent column sizes");
}

void audit_table_storage(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    if(!mesh.nodes.has_consistent_storage()) {
        add_table_storage_issue(report, "NodeTable2D");
    }
    if(!mesh.cells.has_consistent_storage()) {
        add_table_storage_issue(report, "CellTable2D");
    }
    if(!mesh.faces.has_consistent_storage()) {
        add_table_storage_issue(report, "FaceTable2D");
    }
    if(!mesh.patches.has_consistent_storage()) {
        add_table_storage_issue(report, "BoundaryPatchTable");
    }
}

void audit_patch_table_ids(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        const auto patch_id = mesh.patches.id[patch];
        if(!mesh.contains(patch_id)) {
            MeshAuditIssue issue{FatalIssue, "VMM-IDX-005", "Patch table contains invalid patch id"};
            issue.patch_id = patch_id;
            report.add(std::move(issue));
        }
    }
}

void audit_face_owner(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                      MeshAuditReport& report,
                      FaceId face_id,
                      CellId owner)
{
    if(!mesh.contains(owner)) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-006", "Face owner is invalid", face_id);
        issue.cell_id = owner;
        report.add(std::move(issue));
    }
}

void audit_face_neighbour_and_patch(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                    MeshAuditReport& report,
                                    FaceId face_id,
                                    CellId neighbour,
                                    BoundaryPatchId patch_id)
{
    const auto has_neighbour = is_valid(neighbour);
    const auto has_patch = is_valid(patch_id);

    if(has_neighbour && !mesh.contains(neighbour)) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-007", "Face neighbour is out of range", face_id);
        issue.cell_id = neighbour;
        report.add(std::move(issue));
    }

    if(!has_neighbour && (!has_patch || !mesh.contains(patch_id))) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-008", "Boundary face has no valid patch", face_id);
        issue.patch_id = patch_id;
        report.add(std::move(issue));
    }

    if(has_neighbour && has_patch) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-009", "Internal face has boundary patch", face_id);
        issue.patch_id = patch_id;
        report.add(std::move(issue));
    }
}

void audit_face_nodes(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                      MeshAuditReport& report,
                      FaceId face_id,
                      NodeId node0,
                      NodeId node1)
{
    if(!mesh.contains(node0)) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-003", "Face first node is invalid", face_id);
        issue.node_id = node0;
        report.add(std::move(issue));
    }
    if(!mesh.contains(node1)) {
        auto issue = make_face_issue(FatalIssue, "VMM-IDX-003", "Face second node is invalid", face_id);
        issue.node_id = node1;
        report.add(std::move(issue));
    }
}

void audit_faces(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        audit_face_owner(mesh, report, face_id, mesh.faces.owner[face]);
        audit_face_neighbour_and_patch(mesh, report, face_id, mesh.faces.neighbour[face], mesh.faces.patch_id[face]);
        audit_face_nodes(mesh, report, face_id, mesh.faces.node0[face], mesh.faces.node1[face]);
    }
}

void audit_cell_face_offsets(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    const auto connectivity_size = mesh.cell_faces.size();
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto offset = static_cast<std::size_t>(mesh.cells.face_offset[cell]);
        const auto count = static_cast<std::size_t>(mesh.cells.face_count[cell]);
        if(offset > connectivity_size || count > connectivity_size - offset) {
            report.add(make_cell_issue(FatalIssue,
                                       "VMM-IDX-010",
                                       "Cell face offset/count exceeds connectivity storage",
                                       CellId{cell}));
        }
    }
}

void audit_cell_face_ids(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t entry = 0U; entry < mesh.cell_faces.size(); ++entry) {
        const auto face_id = mesh.cell_faces.face_ids[entry];
        if(!mesh.contains(face_id)) {
            report.add(make_face_issue(FatalIssue,
                                       "VMM-IDX-002",
                                       "Cell-face connectivity references invalid face",
                                       face_id));
        }
    }
}

} // namespace

MeshAuditReport MeshIndexAudit2D::run(const vmm::mesh::FiniteVolumeMesh2D& mesh) const
{
    MeshAuditReport report;
    audit_table_storage(mesh, report);
    if(report.has_fatal_errors()) {
        return report;
    }

    audit_patch_table_ids(mesh, report);
    audit_faces(mesh, report);
    audit_cell_face_offsets(mesh, report);
    audit_cell_face_ids(mesh, report);
    return report;
}

MeshAuditReport audit_mesh_indices_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    return MeshIndexAudit2D{}.run(mesh);
}

} // namespace vmm::audit
