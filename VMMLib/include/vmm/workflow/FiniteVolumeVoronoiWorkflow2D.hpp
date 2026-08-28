//==============================================================================
// File        : FiniteVolumeVoronoiWorkflow2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Integrated 2D finite-volume Voronoi generation workflow.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <optional>
#include <span>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/audit/MeshProjectionAudit2D.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>
#include <vmm/mesh/FiniteVolumeStencilGraph2D.hpp>
#include <vmm/raster/RasterField2D.hpp>
#include <vmm/tessellation/CentroidalVoronoiRelaxation2D.hpp>
#include <vmm/tessellation/TessellationResult.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>

namespace vmm::workflow {

struct CompleteFiniteVolumeVoronoiMesh2DOptions final {
    vmm::tessellation::GenerationParams2D tessellation;
    vmm::tessellation::LloydRelaxation2DOptions lloyd;
    vmm::mesh::FiniteVolumeMeshBuilder2DOptions mesh_builder;
    vmm::audit::MeshProjectionAudit2DOptions projection_audit;
    bool require_valid_mesh{true};
    bool build_stencil_graph{true};
    const vmm::raster::RasterScalarField2D* raster_field{nullptr};
    vmm::raster::RasterSampleMethod raster_sample_method{vmm::raster::RasterSampleMethodTraits::Bilinear};
};

struct CompleteFiniteVolumeVoronoiMesh2D final {
    vmm::tessellation::ClippedVoronoiMesh2D clipped_voronoi;
    vmm::mesh::FiniteVolumeMesh2D finite_volume_mesh;
    vmm::audit::MeshAuditReport connectivity_geometry_report;
    vmm::audit::MeshAuditReport patch_report;
    vmm::audit::MeshAuditReport projection_report;
    std::vector<vmm::tessellation::LloydIterationReport2D> lloyd_iterations;
    std::optional<vmm::mesh::FiniteVolumeStencilGraph2D> stencil_graph;
    std::optional<vmm::raster::CellRasterSamples2D> raster_samples;

    [[nodiscard]] bool valid() const noexcept;
};

[[nodiscard]] CompleteFiniteVolumeVoronoiMesh2D
generate_complete_finite_volume_voronoi_mesh_2d(
    std::span<const vmm::domain::Point2D> sites,
    const vmm::domain::PolygonalDomain2D& domain,
    const CompleteFiniteVolumeVoronoiMesh2DOptions& options = {});

} // namespace vmm::workflow
