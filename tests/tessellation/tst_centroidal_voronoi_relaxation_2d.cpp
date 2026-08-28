//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/tessellation/CentroidalVoronoiRelaxation2D.hpp>
#include <vmm/tessellation/Tessellator2D.hpp>
#include <vmm/workflow/FiniteVolumeVoronoiWorkflow2D.hpp>

using namespace vmm::domain;
using namespace vmm::tessellation;

namespace {

[[nodiscard]] std::vector<Point2D> asymmetric_sites()
{
    return {
        {-0.65, -0.75},
        {0.25, -0.65},
        {-0.35, 0.35},
        {0.65, 0.55},
    };
}

[[nodiscard]] vmm::core::Real distance(Point2D lhs, Point2D rhs) noexcept
{
    const auto dx = lhs.x - rhs.x;
    const auto dy = lhs.y - rhs.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

} // namespace

TEST(CentroidalVoronoiRelaxation2DTest, ZeroIterationsKeepsInitialSites)
{
    const auto domain = make_rectangle_2d(2.0, 2.0);
    const auto sites = asymmetric_sites();

    const auto result = relax_sites_lloyd_2d(sites, domain);

    EXPECT_EQ(result.completed_iteration_count(), 0U);
    ASSERT_EQ(result.sites.size(), sites.size());
    EXPECT_EQ(result.clipped_voronoi.planar_cells.size(), sites.size());

    for(std::size_t site = 0U; site < sites.size(); ++site) {
        EXPECT_DOUBLE_EQ(result.sites[site].x, sites[site].x);
        EXPECT_DOUBLE_EQ(result.sites[site].y, sites[site].y);
    }
}

TEST(CentroidalVoronoiRelaxation2DTest, OneIterationMovesSitesToCellCentroids)
{
    const auto domain = make_rectangle_2d(2.0, 2.0);
    const auto sites = asymmetric_sites();
    const auto initial_mesh = generate_clipped_voronoi_2d(sites, domain);
    LloydRelaxation2DOptions options;
    options.iteration_count = 1U;

    const auto result = relax_sites_lloyd_2d(sites, domain, options);

    ASSERT_EQ(result.completed_iteration_count(), 1U);
    ASSERT_EQ(result.sites.size(), sites.size());
    EXPECT_GT(result.iterations.front().mean_generator_displacement, 0.0);
    EXPECT_GT(result.iterations.front().max_generator_displacement, 0.0);

    for(std::size_t site = 0U; site < sites.size(); ++site) {
        const auto expected = initial_mesh.planar_cells[site].centroid();
        EXPECT_NEAR(result.sites[site].x, expected.x, 1.0e-12);
        EXPECT_NEAR(result.sites[site].y, expected.y, 1.0e-12);
    }
}

TEST(CentroidalVoronoiRelaxation2DTest, ConvergenceToleranceCanStopIterations)
{
    const auto domain = make_rectangle_2d(2.0, 2.0);
    const auto sites = asymmetric_sites();
    LloydRelaxation2DOptions options;
    options.iteration_count = 5U;
    options.convergence_tolerance = 10.0;

    const auto result = relax_sites_lloyd_2d(sites, domain, options);

    ASSERT_EQ(result.completed_iteration_count(), 1U);
    EXPECT_TRUE(result.converged);
    EXPECT_TRUE(result.iterations.front().converged);
}

TEST(CentroidalVoronoiRelaxation2DTest, RejectsNegativeTolerance)
{
    const auto domain = make_rectangle_2d(2.0, 2.0);
    const auto sites = asymmetric_sites();
    LloydRelaxation2DOptions options;
    options.convergence_tolerance = -1.0;

    EXPECT_THROW((void)relax_sites_lloyd_2d(sites, domain, options), vmm::error::MeshException);
}

TEST(CentroidalVoronoiRelaxation2DTest, CompleteWorkflowCanRunLloydBeforeBuildingFiniteVolumeMesh)
{
    const auto domain = make_rectangle_2d(2.0, 2.0);
    const auto sites = asymmetric_sites();
    vmm::workflow::CompleteFiniteVolumeVoronoiMesh2DOptions options;
    options.lloyd.iteration_count = 2U;

    const auto result = vmm::workflow::generate_complete_finite_volume_voronoi_mesh_2d(sites, domain, options);

    EXPECT_TRUE(result.valid());
    EXPECT_EQ(result.finite_volume_mesh.cell_count(), sites.size());
    ASSERT_EQ(result.lloyd_iterations.size(), 2U);
    EXPECT_GT(result.lloyd_iterations.front().max_generator_displacement,
              result.lloyd_iterations.back().max_generator_displacement);

    for(std::size_t site = 0U; site < sites.size(); ++site) {
        const auto relaxed_site = result.clipped_voronoi.planar_cells[site].generator_site();
        EXPECT_GT(distance(relaxed_site, sites[site]), 0.0);
    }
}
