//==============================================================================
// File        : MeshConnectivityGeometryAudit2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of 2D FV connectivity and geometry audits.
//==============================================================================

#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>

#include <vmm/audit/MeshIndexAudit2D.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <vector>

namespace vmm::audit {
namespace {

using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::is_valid;

constexpr auto FatalIssue = MeshAuditSeverityTraits::Fatal;

[[nodiscard]] bool finite(double value) noexcept
{
    return std::isfinite(value);
}

[[nodiscard]] bool span_contains(std::span<const FaceId> faces, FaceId face_id) noexcept
{
    return std::find(faces.begin(), faces.end(), face_id) != faces.end();
}

void audit_face_membership(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        const auto owner = mesh.faces.owner[face];
        const auto owner_faces = mesh.faces_of(owner);
        if(!span_contains(owner_faces, face_id)) {
            auto issue = make_face_issue(FatalIssue, "VMM-CON-001", "Owner cell does not reference face", face_id);
            issue.cell_id = owner;
            report.add(issue);
        }

        const auto neighbour = mesh.faces.neighbour[face];
        if(is_valid(neighbour)) {
            const auto neighbour_faces = mesh.faces_of(neighbour);
            if(!span_contains(neighbour_faces, face_id)) {
                auto issue =
                    make_face_issue(FatalIssue, "VMM-CON-002", "Neighbour cell does not reference internal face", face_id);
                issue.cell_id = neighbour;
                report.add(issue);
            }
        }
    }
}

void audit_face_occurrence_counts(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    std::vector<std::uint32_t> occurrence_count(mesh.face_count(), 0U);
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        for(const auto face_id : mesh.faces_of(CellId{cell})) {
            ++occurrence_count[face_id.value];
        }
    }

    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto expected = is_valid(mesh.faces.neighbour[face]) ? 2U : 1U;
        if(occurrence_count[face] != expected) {
            auto issue = make_face_issue(
                FatalIssue, "VMM-CON-005", "Face occurrence count does not match boundary/internal state", FaceId{face});
            issue.cell_id = mesh.faces.owner[face];
            report.add(issue);
        }
    }
}

void audit_duplicate_cell_faces(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto cell_id = CellId{cell};
        const auto faces = mesh.faces_of(cell_id);
        auto duplicate_found = false;
        for(std::size_t i = 0U; i < faces.size() && !duplicate_found; ++i) {
            for(std::size_t j = i + 1U; j < faces.size(); ++j) {
                if(faces[i] == faces[j]) {
                    auto issue =
                        make_cell_issue(FatalIssue, "VMM-CON-003", "Cell face list contains duplicate face", cell_id);
                    issue.face_id = faces[i];
                    report.add(issue);
                    duplicate_found = true;
                    break;
                }
            }
        }
    }
}

void audit_minimum_cell_face_count(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        if(mesh.cells.face_count[cell] < 3U) {
            report.add(make_cell_issue(FatalIssue, "VMM-CON-004", "2D cell has fewer than three faces", CellId{cell}));
        }
    }
}

void audit_cell_geometry(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto cell_id = CellId{cell};
        if(!finite(mesh.cells.centre_x[cell]) || !finite(mesh.cells.centre_y[cell])) {
            report.add(make_cell_issue(FatalIssue, "VMM-GEO-001", "Cell centre is not finite", cell_id));
        }
        if(!finite(mesh.cells.area[cell]) || mesh.cells.area[cell] <= 0.0) {
            report.add(make_cell_issue(FatalIssue, "VMM-GEO-002", "Cell area is not positive", cell_id));
        }
    }
}

void audit_face_geometry(const vmm::mesh::FiniteVolumeMesh2D& mesh, MeshAuditReport& report)
{
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        if(!finite(mesh.faces.centre_x[face]) || !finite(mesh.faces.centre_y[face])) {
            report.add(make_face_issue(FatalIssue, "VMM-GEO-003", "Face centre is not finite", face_id));
        }
        if(!finite(mesh.faces.length[face]) || mesh.faces.length[face] <= 0.0) {
            report.add(make_face_issue(FatalIssue, "VMM-GEO-004", "Face length is not positive", face_id));
        }
        if(!finite(mesh.faces.normal_x[face]) || !finite(mesh.faces.normal_y[face])) {
            report.add(make_face_issue(FatalIssue, "VMM-GEO-005", "Face normal is not finite", face_id));
            continue;
        }

        const auto normal_norm2 = (mesh.faces.normal_x[face] * mesh.faces.normal_x[face]) +
                                  (mesh.faces.normal_y[face] * mesh.faces.normal_y[face]);
        if(normal_norm2 <= 0.0) {
            report.add(make_face_issue(FatalIssue, "VMM-GEO-006", "Face normal is zero", face_id));
            continue;
        }

        const auto owner = mesh.faces.owner[face];
        const auto owner_index = static_cast<std::size_t>(owner.value);
        const auto dx = mesh.faces.centre_x[face] - mesh.cells.centre_x[owner_index];
        const auto dy = mesh.faces.centre_y[face] - mesh.cells.centre_y[owner_index];
        const auto outward_dot = (dx * mesh.faces.normal_x[face]) + (dy * mesh.faces.normal_y[face]);
        if(finite(outward_dot) && outward_dot <= 0.0) {
            auto issue =
                make_face_issue(FatalIssue, "VMM-GEO-007", "Face normal does not point outward from owner", face_id);
            issue.cell_id = owner;
            report.add(issue);
        }
    }
}

} // namespace

MeshAuditReport MeshConnectivityGeometryAudit2D::run(const vmm::mesh::FiniteVolumeMesh2D& mesh) const
{
    auto report = audit_mesh_indices_2d(mesh);
    if(report.has_fatal_errors()) {
        return report;
    }

    audit_face_membership(mesh, report);
    audit_duplicate_cell_faces(mesh, report);
    audit_face_occurrence_counts(mesh, report);
    audit_minimum_cell_face_count(mesh, report);
    audit_cell_geometry(mesh, report);
    audit_face_geometry(mesh, report);
    return report;
}

MeshAuditReport audit_mesh_connectivity_geometry_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    return MeshConnectivityGeometryAudit2D{}.run(mesh);
}

} // namespace vmm::audit
