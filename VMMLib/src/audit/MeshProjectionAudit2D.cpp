//==============================================================================
// File        : MeshProjectionAudit2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Projection and orthogonality audit implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <string>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/audit/MeshProjectionAudit2D.hpp>
#include <vmm/error/MeshException.hpp>

namespace vmm::audit {
namespace {

using vmm::core::CellId;
using vmm::core::FaceId;
using vmm::core::Real;
using vmm::core::is_valid;

constexpr auto WarningIssue = MeshAuditSeverityTraits::Warning;

[[nodiscard]] Real square(Real value) noexcept
{
    return value * value;
}

[[nodiscard]] Real distance(Real dx, Real dy) noexcept
{
    return std::sqrt(square(dx) + square(dy));
}

[[nodiscard]] Real radians_to_degrees(Real radians) noexcept
{
    return radians * Real{180.0} / std::numbers::pi_v<Real>;
}

[[nodiscard]] Real clamped_abs_cosine(Real dx, Real dy, Real nx, Real ny) noexcept
{
    const auto vector_norm = distance(dx, dy);
    const auto normal_norm = distance(nx, ny);
    if(vector_norm == Real{} || normal_norm == Real{}) {
        return Real{};
    }
    const auto cosine = ((dx * nx) + (dy * ny)) / (vector_norm * normal_norm);
    return std::clamp(std::abs(cosine), Real{}, Real{1.0});
}

[[nodiscard]] Real scaled_tolerance(Real tolerance, Real reference) noexcept
{
    return tolerance * std::max(Real{1.0}, reference);
}

void require_valid_options(const MeshProjectionAudit2DOptions& options)
{
    if(options.internal_midpoint_tolerance < Real{}) {
        vmm::error::throw_invalid_argument("MeshProjectionAudit2D", "Internal midpoint tolerance must be non-negative.");
    }
    if(options.boundary_projection_tolerance < Real{}) {
        vmm::error::throw_invalid_argument("MeshProjectionAudit2D", "Boundary projection tolerance must be non-negative.");
    }
    if(options.max_non_orthogonality_degrees < Real{} || options.max_non_orthogonality_degrees > Real{90.0}) {
        vmm::error::throw_invalid_argument("MeshProjectionAudit2D",
                                           "Maximum non-orthogonality angle must be between 0 and 90 degrees.");
    }
}

void append_report(MeshAuditReport& destination, MeshAuditReport source)
{
    for(auto& issue : source.issues) {
        destination.add(std::move(issue));
    }
}

[[nodiscard]] MeshAuditReport run_base_audits(const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    auto report = audit_mesh_connectivity_geometry_2d(mesh);
    if(report.has_fatal_errors()) {
        return report;
    }
    append_report(report, audit_mesh_patches_2d(mesh));
    return report;
}

void audit_internal_midpoint_anomaly(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                     const MeshProjectionAudit2DOptions& options,
                                     MeshAuditReport& report,
                                     FaceId face_id,
                                     std::size_t face)
{
    const auto owner = mesh.faces.owner[face];
    const auto neighbour = mesh.faces.neighbour[face];
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto neighbour_index = static_cast<std::size_t>(neighbour.value);
    const auto midpoint_x = (mesh.cells.centre_x[owner_index] + mesh.cells.centre_x[neighbour_index]) / Real{2.0};
    const auto midpoint_y = (mesh.cells.centre_y[owner_index] + mesh.cells.centre_y[neighbour_index]) / Real{2.0};
    const auto dx = mesh.faces.centre_x[face] - midpoint_x;
    const auto dy = mesh.faces.centre_y[face] - midpoint_y;
    const auto centre_dx = mesh.cells.centre_x[neighbour_index] - mesh.cells.centre_x[owner_index];
    const auto centre_dy = mesh.cells.centre_y[neighbour_index] - mesh.cells.centre_y[owner_index];
    const auto centre_distance = distance(centre_dx, centre_dy);
    const auto offset = distance(dx, dy);

    if(offset > scaled_tolerance(options.internal_midpoint_tolerance, centre_distance)) {
        auto issue = make_face_issue(WarningIssue,
                                     "VMM-PRJ-001",
                                     "Internal face centre is offset from the owner-neighbour midpoint",
                                     face_id);
        issue.cell_id = owner;
        report.add(std::move(issue));
    }
}

void audit_internal_non_orthogonality(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                      const MeshProjectionAudit2DOptions& options,
                                      MeshAuditReport& report,
                                      FaceId face_id,
                                      std::size_t face)
{
    const auto owner = mesh.faces.owner[face];
    const auto neighbour = mesh.faces.neighbour[face];
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto neighbour_index = static_cast<std::size_t>(neighbour.value);
    const auto dx = mesh.cells.centre_x[neighbour_index] - mesh.cells.centre_x[owner_index];
    const auto dy = mesh.cells.centre_y[neighbour_index] - mesh.cells.centre_y[owner_index];
    const auto abs_cosine = clamped_abs_cosine(dx, dy, mesh.faces.normal_x[face], mesh.faces.normal_y[face]);
    const auto angle_degrees = radians_to_degrees(std::acos(abs_cosine));

    if(angle_degrees > options.max_non_orthogonality_degrees) {
        auto issue = make_face_issue(WarningIssue,
                                     "VMM-ORT-001",
                                     "Internal face normal is strongly non-orthogonal to the owner-neighbour vector",
                                     face_id);
        issue.cell_id = owner;
        report.add(std::move(issue));
    }
}

void audit_boundary_projection_anomaly(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                       const MeshProjectionAudit2DOptions& options,
                                       MeshAuditReport& report,
                                       FaceId face_id,
                                       std::size_t face)
{
    const auto owner = mesh.faces.owner[face];
    const auto owner_index = static_cast<std::size_t>(owner.value);
    const auto dx = mesh.faces.centre_x[face] - mesh.cells.centre_x[owner_index];
    const auto dy = mesh.faces.centre_y[face] - mesh.cells.centre_y[owner_index];
    const auto normal_norm = distance(mesh.faces.normal_x[face], mesh.faces.normal_y[face]);
    if(normal_norm == Real{}) {
        return;
    }

    const auto nx = mesh.faces.normal_x[face] / normal_norm;
    const auto ny = mesh.faces.normal_y[face] / normal_norm;
    const auto projection = (dx * nx) + (dy * ny);
    const auto tangent_x = dx - (projection * nx);
    const auto tangent_y = dy - (projection * ny);
    const auto tangent_offset = distance(tangent_x, tangent_y);

    if(tangent_offset > scaled_tolerance(options.boundary_projection_tolerance, mesh.faces.length[face])) {
        auto issue = make_face_issue(WarningIssue,
                                     "VMM-PRJ-002",
                                     "Boundary face centre is tangentially offset from the owner normal projection",
                                     face_id);
        issue.cell_id = owner;
        issue.patch_id = mesh.faces.patch_id[face];
        report.add(std::move(issue));
    }
}

} // namespace

MeshAuditReport MeshProjectionAudit2D::run(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                           const MeshProjectionAudit2DOptions& options) const
{
    require_valid_options(options);

    MeshAuditReport report;
    if(options.run_base_audits) {
        report = run_base_audits(mesh);
        if(report.has_fatal_errors()) {
            return report;
        }
    }

    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        const auto face_id = FaceId{face};
        if(is_valid(mesh.faces.neighbour[face])) {
            audit_internal_midpoint_anomaly(mesh, options, report, face_id, face);
            audit_internal_non_orthogonality(mesh, options, report, face_id, face);
        } else {
            audit_boundary_projection_anomaly(mesh, options, report, face_id, face);
        }
    }

    return report;
}

MeshAuditReport audit_mesh_projection_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                         const MeshProjectionAudit2DOptions& options)
{
    return MeshProjectionAudit2D{}.run(mesh, options);
}

} // namespace vmm::audit
