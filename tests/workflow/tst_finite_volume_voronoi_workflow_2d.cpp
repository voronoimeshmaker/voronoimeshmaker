//==============================================================================
// c++ includes
//==============================================================================
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/raster/RasterField2D.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>
#include <vmm/workflow/FiniteVolumeVoronoiWorkflow2D.hpp>

using namespace vmm::core;
using namespace vmm::raster;
using namespace vmm::workflow;

namespace {

vmm::domain::PolygonalDomain2D make_domain()
{
    return vmm::domain::make_rectangle_2d(3.0, 2.0);
}

RasterScalarField2D make_covering_raster()
{
    return RasterScalarField2D{
        9U,
        7U,
        RasterGridGeometry2D{-2.0, -1.5, 0.5, 0.5},
        std::vector<Real>(9U * 7U, 12.5)};
}

} // namespace

TEST(FiniteVolumeVoronoiWorkflow2DTest, BuildsAuditedFiniteVolumeMesh)
{
    const auto domain = make_domain();
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.5);

    const auto result = generate_complete_finite_volume_voronoi_mesh_2d(sites, domain);

    EXPECT_TRUE(result.valid());
    EXPECT_EQ(result.clipped_voronoi.planar_cells.size(), sites.size());
    EXPECT_EQ(result.finite_volume_mesh.cell_count(), sites.size());
    EXPECT_GT(result.finite_volume_mesh.face_count(), 0U);
    ASSERT_TRUE(result.stencil_graph.has_value());
    EXPECT_GT(result.stencil_graph->internal_count(), 0U);
    EXPECT_GT(result.stencil_graph->boundary_count(), 0U);
    EXPECT_FALSE(result.raster_samples.has_value());
}

TEST(FiniteVolumeVoronoiWorkflow2DTest, SamplesRasterWhenProvided)
{
    const auto domain = make_domain();
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.75);
    const auto raster = make_covering_raster();
    CompleteFiniteVolumeVoronoiMesh2DOptions options;
    options.raster_field = &raster;
    options.raster_sample_method = RasterSampleMethodTraits::Nearest;

    const auto result = generate_complete_finite_volume_voronoi_mesh_2d(sites, domain, options);

    ASSERT_TRUE(result.raster_samples.has_value());
    EXPECT_EQ(result.raster_samples->size(), result.finite_volume_mesh.cell_count());
    ASSERT_TRUE(result.raster_samples->has_consistent_storage());
    for(const auto valid : result.raster_samples->valid) {
        EXPECT_EQ(valid, 1U);
    }
}

TEST(FiniteVolumeVoronoiWorkflow2DTest, CanSkipStencilGraph)
{
    const auto domain = make_domain();
    const auto sites = vmm::site_generation::make_hexagonal_sites_2d(domain, 0.75);
    CompleteFiniteVolumeVoronoiMesh2DOptions options;
    options.build_stencil_graph = false;

    const auto result = generate_complete_finite_volume_voronoi_mesh_2d(sites, domain, options);

    EXPECT_TRUE(result.valid());
    EXPECT_FALSE(result.stencil_graph.has_value());
}
