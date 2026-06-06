//==============================================================================
// File        : FiniteVolumeVoronoiWorkflow2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Integrated 2D finite-volume Voronoi generation workflow.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <span>
#include <string>
#include <utility>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/workflow/FiniteVolumeVoronoiWorkflow2D.hpp>

namespace vmm::workflow {
namespace {

[[nodiscard]] bool all_reports_valid(const CompleteFiniteVolumeVoronoiMesh2D& result) noexcept
{
    return result.connectivity_geometry_report.valid() && result.patch_report.valid() &&
           result.projection_report.valid();
}

void require_valid_workflow_result(const CompleteFiniteVolumeVoronoiMesh2D& result)
{
    if(all_reports_valid(result)) {
        return;
    }

    std::string message{"Integrated 2D finite-volume Voronoi workflow produced an invalid mesh."};
    message += " connectivity_geometry_issues=" +
               std::to_string(result.connectivity_geometry_report.issue_count());
    message += " patch_issues=" + std::to_string(result.patch_report.issue_count());
    message += " projection_issues=" + std::to_string(result.projection_report.issue_count());
    vmm::error::throw_invalid_argument("generate_complete_finite_volume_voronoi_mesh_2d", message);
}

} // namespace

bool CompleteFiniteVolumeVoronoiMesh2D::valid() const noexcept
{
    return all_reports_valid(*this);
}

CompleteFiniteVolumeVoronoiMesh2D generate_complete_finite_volume_voronoi_mesh_2d(
    std::span<const vmm::domain::Point2D> sites,
    const vmm::domain::PolygonalDomain2D& domain,
    const CompleteFiniteVolumeVoronoiMesh2DOptions& options)
{
    auto clipped_voronoi = vmm::tessellation::generate_clipped_voronoi_2d(
        sites,
        domain,
        options.tessellation);
    auto finite_volume_mesh = vmm::mesh::build_finite_volume_mesh_2d(
        clipped_voronoi.planar_cells,
        options.mesh_builder);

    CompleteFiniteVolumeVoronoiMesh2D result{
        std::move(clipped_voronoi),
        std::move(finite_volume_mesh),
        {},
        {},
        {},
        {},
        {}};

    result.connectivity_geometry_report =
        vmm::audit::audit_mesh_connectivity_geometry_2d(result.finite_volume_mesh);
    result.patch_report = vmm::audit::audit_mesh_patches_2d(result.finite_volume_mesh);
    result.projection_report = vmm::audit::audit_mesh_projection_2d(
        result.finite_volume_mesh,
        options.projection_audit);

    if(options.require_valid_mesh) {
        require_valid_workflow_result(result);
    }

    if(options.build_stencil_graph) {
        result.stencil_graph = vmm::mesh::build_finite_volume_stencil_graph_2d(result.finite_volume_mesh);
    }

    if(options.raster_field != nullptr) {
        result.raster_samples = vmm::raster::sample_cell_centres(
            result.finite_volume_mesh,
            *options.raster_field,
            options.raster_sample_method);
    }

    return result;
}

} // namespace vmm::workflow
