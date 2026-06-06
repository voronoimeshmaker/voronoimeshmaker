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
#include <vmm/error/MeshException.hpp>
#include <vmm/remeshing/Remesher.hpp>
#include <vmm/site_generation/HexagonalSitePattern2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::remeshing;

namespace {

PolygonalDomain2D make_domain()
{
    return vmm::domain::make_rectangle_2d(4.0, 3.0);
}

std::vector<Point2D> make_sites(const PolygonalDomain2D& domain)
{
    return vmm::site_generation::make_hexagonal_sites_2d(domain, 0.75);
}

} // namespace

TEST(SiteEditRemesher2DTest, AppliesInsertMoveAndRemoveSequentially)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.insert(Point2D{0.0, 0.0});
    batch.move(SiteId{0U}, Point2D{-1.4, -1.0});
    batch.remove(SiteId{1U});

    const auto application = apply_site_edit_batch_2d(sites, batch, domain);

    EXPECT_EQ(application.sites.size(), sites.size());
    EXPECT_EQ(application.report.inserted_site_ids.size(), 1U);
    EXPECT_EQ(application.report.moved_site_ids.size(), 1U);
    EXPECT_EQ(application.report.removed_site_ids.size(), 1U);
    EXPECT_EQ(application.report.edit_count(), 3U);
    EXPECT_DOUBLE_EQ(application.sites[0].x, -1.4);
    EXPECT_DOUBLE_EQ(application.sites[0].y, -1.0);
}

TEST(SiteEditRemesher2DTest, RejectsInvalidSiteEdit)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.move(SiteId{999U}, Point2D{0.0, 0.0});

    EXPECT_THROW((void)apply_site_edit_batch_2d(sites, batch, domain), vmm::error::MeshException);
}

TEST(SiteEditRemesher2DTest, RejectsSitesOutsideDomain)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.insert(Point2D{100.0, 100.0});

    EXPECT_THROW((void)apply_site_edit_batch_2d(sites, batch, domain), vmm::error::MeshException);
}

TEST(SiteEditRemesher2DTest, RejectsMinimumDistanceViolation)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.insert(sites.front());
    SiteRemeshingCriteria2D criteria;
    criteria.minimum_site_distance = 0.1;

    EXPECT_THROW((void)apply_site_edit_batch_2d(sites, batch, domain, criteria), vmm::error::MeshException);
}

TEST(SiteEditRemesher2DTest, RebuildsCompleteFiniteVolumeMeshAfterSiteEdits)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.insert(Point2D{0.0, 0.0});
    SiteRemeshingOptions2D options;
    options.criteria.minimum_site_distance = 1.0e-8;

    const auto result = remesh_complete_finite_volume_voronoi_mesh_2d(sites, batch, domain, options);

    EXPECT_EQ(result.sites.size(), sites.size() + 1U);
    EXPECT_EQ(result.mesh.finite_volume_mesh.cell_count(), result.sites.size());
    EXPECT_TRUE(result.mesh.valid());
    ASSERT_TRUE(result.mesh.stencil_graph.has_value());
    EXPECT_GT(result.mesh.stencil_graph->internal_count(), 0U);
}

TEST(SiteEditRemesher2DTest, DefaultRegistryExposesSiteEditRebuildMethod)
{
    const auto domain = make_domain();
    const auto sites = make_sites(domain);
    SiteEditBatch2D batch;
    batch.insert(Point2D{0.0, 0.0});
    const auto registry = default_site_remeshing_registry_2d();

    ASSERT_TRUE(registry.contains(site_edit_rebuild_remeshing_method_2d_id));
    const auto result = registry.remesh(site_edit_rebuild_remeshing_method_2d_id, sites, batch, domain);

    EXPECT_EQ(result.sites.size(), sites.size() + 1U);
    EXPECT_TRUE(result.mesh.valid());
}
