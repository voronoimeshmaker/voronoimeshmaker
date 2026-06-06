//==============================================================================
// File        : MeshPatchAudit2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the 2D finite-volume mesh patch audit.
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
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/mesh/BoundaryPatchTable.hpp>

namespace vmm::audit {
namespace {

using vmm::core::BoundaryPatchId;
using vmm::core::FaceId;
using vmm::core::is_valid;

constexpr auto FatalIssue = MeshAuditSeverityTraits::Fatal;
constexpr auto ErrorIssue = MeshAuditSeverityTraits::Error;

[[nodiscard]] MeshAuditIssue make_patch_issue(MeshAuditSeverity severity,
                                              std::string code,
                                              std::string message,
                                              BoundaryPatchId patch_id)
{
    MeshAuditIssue issue{severity, std::move(code), std::move(message)};
    issue.patch_id = patch_id;
    return issue;
}

[[nodiscard]] bool is_internal_patch_type(vmm::mesh::BoundaryPatchType patch_type) noexcept
{
    return patch_type == vmm::mesh::BoundaryPatchTypeTraits::Internal;
}

void audit_patch_identity(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        const auto expected_id = BoundaryPatchId{patch};
        const auto patch_id = mesh.patches.id[patch];
        if(patch_id != expected_id) {
            report.add(make_patch_issue(FatalIssue,
                                        "VMM-PAT-001",
                                        "Patch table id does not match its row index",
                                        patch_id));
        }
    }
}

void audit_patch_metadata(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        const auto patch_id = BoundaryPatchId{patch};
        if(mesh.patches.name[patch].empty()) {
            report.add(make_patch_issue(ErrorIssue, "VMM-PAT-002", "Patch name is empty", patch_id));
        }
        if(mesh.patches.type[patch].name.empty()) {
            report.add(make_patch_issue(ErrorIssue, "VMM-PAT-003", "Patch type name is empty", patch_id));
        }
    }
}

void audit_face_patch_semantics(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        const auto has_neighbour = is_valid(mesh.faces.neighbour[face]);
        const auto patch_id = mesh.faces.patch_id[face];
        const auto has_patch = is_valid(patch_id);

        if(has_neighbour || !has_patch || !mesh.contains(patch_id)) {
            continue;
        }

        const auto patch_index = static_cast<std::size_t>(patch_id.value);
        if(is_internal_patch_type(mesh.patches.type[patch_index])) {
            auto issue = make_face_issue(ErrorIssue,
                                        "VMM-PAT-004",
                                        "Boundary face references an internal patch type",
                                        face_id);
            issue.patch_id = patch_id;
            report.add(std::move(issue));
        }
    }
}

} // namespace

MeshAuditReport MeshPatchAudit2D::run(const vmm::mesh::FiniteVolumeMesh2D& mesh) const
{
    auto report = audit_mesh_indices_2d(mesh);
    if(report.has_fatal_errors()) {
        return report;
    }

    if(mesh.patches.empty()) {
        report.add(FatalIssue, "VMM-PAT-000", "Boundary patch table is empty");
        return report;
    }

    audit_patch_identity(mesh, report);
    if(report.has_fatal_errors()) {
        return report;
    }
    audit_patch_metadata(mesh, report);
    audit_face_patch_semantics(mesh, report);
    return report;
}

MeshAuditReport audit_mesh_patches_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    return MeshPatchAudit2D{}.run(mesh);
}

} // namespace vmm::audit
