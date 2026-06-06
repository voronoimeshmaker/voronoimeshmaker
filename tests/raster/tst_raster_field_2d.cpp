//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <filesystem>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/raster/RasterField2D.hpp>

using namespace vmm::core;
using namespace vmm::mesh;
using namespace vmm::raster;

namespace {

RasterScalarField2D make_raster()
{
    return RasterScalarField2D{
        3U,
        3U,
        RasterGridGeometry2D{0.0, 0.0, 1.0, 1.0},
        std::vector<Real>{0.0, 10.0, 20.0, 100.0, 110.0, 120.0, 200.0, 210.0, 220.0}};
}

FiniteVolumeMesh2D make_cell_centres()
{
    FiniteVolumeMesh2D mesh;
    mesh.cells.centre_x = {0.0, 0.5, 5.0};
    mesh.cells.centre_y = {0.0, 0.5, 5.0};
    mesh.cells.area = {1.0, 1.0, 1.0};
    mesh.cells.site_id = {SiteId{0U}, SiteId{1U}, SiteId{2U}};
    mesh.cells.face_offset = {0U, 0U, 0U};
    mesh.cells.face_count = {0U, 0U, 0U};
    return mesh;
}

} // namespace

TEST(RasterField2DTest, GeometryMapsGridCentres)
{
    const RasterGridGeometry2D geometry{10.0, 20.0, 2.0, -3.0};
    const auto point = geometry.point_at(2U, 3U);

    EXPECT_DOUBLE_EQ(point.x, 16.0);
    EXPECT_DOUBLE_EQ(point.y, 14.0);
}

TEST(RasterField2DTest, RejectsInvalidStorage)
{
    EXPECT_THROW((RasterScalarField2D{2U, 2U, RasterGridGeometry2D{}, std::vector<Real>{1.0, 2.0}}),
                 vmm::error::MeshException);
    EXPECT_THROW((RasterScalarField2D{0U, 2U, RasterGridGeometry2D{}, std::vector<Real>{}}),
                 vmm::error::MeshException);
}

TEST(RasterField2DTest, SamplesNearestAndBilinear)
{
    const auto raster = make_raster();

    const auto nearest = raster.sample(Point<Dimension::D2>{0.49, 0.49}, RasterSampleMethodTraits::Nearest);
    ASSERT_TRUE(nearest.has_value());
    EXPECT_DOUBLE_EQ(nearest.value(), 0.0);

    const auto bilinear = raster.sample(Point<Dimension::D2>{0.5, 0.5}, RasterSampleMethodTraits::Bilinear);
    ASSERT_TRUE(bilinear.has_value());
    EXPECT_DOUBLE_EQ(bilinear.value(), 55.0);

    EXPECT_FALSE(raster.sample(Point<Dimension::D2>{10.0, 10.0}).has_value());
}

TEST(RasterField2DTest, NoDataInvalidatesSamples)
{
    const RasterScalarField2D raster{
        2U,
        2U,
        RasterGridGeometry2D{0.0, 0.0, 1.0, 1.0},
        std::vector<Real>{1.0, -9999.0, 3.0, 4.0},
        -9999.0};

    EXPECT_FALSE(raster.has_value(0U, 1U));
    EXPECT_FALSE(raster.sample(Point<Dimension::D2>{0.5, 0.5}, RasterSampleMethodTraits::Bilinear).has_value());
}

TEST(RasterField2DTest, SamplesFiniteVolumeCellCentres)
{
    const auto raster = make_raster();
    const auto samples = sample_cell_centres(make_cell_centres(), raster, RasterSampleMethodTraits::Nearest);

    ASSERT_TRUE(samples.has_consistent_storage());
    ASSERT_EQ(samples.size(), 3U);
    EXPECT_EQ(samples.cell_id[0], CellId{0U});
    EXPECT_EQ(samples.valid[0], 1U);
    EXPECT_DOUBLE_EQ(samples.value[0], 0.0);
    EXPECT_EQ(samples.valid[2], 0U);
    EXPECT_TRUE(std::isnan(samples.value[2]));
}

TEST(RasterField2DTest, BackendAvailabilityIsExplicit)
{
    if(!gis_raster_backend_available()) {
        EXPECT_THROW((void)read_gis_raster_scalar_field_2d(std::filesystem::path{"missing.tif"}),
                     vmm::error::MeshException);
    } else {
        SUCCEED() << "GDAL raster backend is available in this build.";
    }
}
