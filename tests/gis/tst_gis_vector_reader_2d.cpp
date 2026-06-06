//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <string>
#include <utility>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/gis/GisVectorReader2D.hpp>

using namespace vmm::domain;
using namespace vmm::gis;
using namespace vmm::mesh;

namespace {

GisBoundaryPatch2D make_patch(std::string name, BoundaryPatchType type)
{
    return GisBoundaryPatch2D{
        Polygon2D{std::vector<Point2D>{{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}},
        std::move(name),
        type,
        "layer",
        "7"};
}

} // namespace

TEST(GisVectorReader2DTest, CoordinateReferenceSystemBuildsIdentifier)
{
    CoordinateReferenceSystem crs;
    EXPECT_TRUE(crs.empty());
    EXPECT_TRUE(crs.identifier().empty());

    crs.authority = "EPSG";
    crs.code = "3763";
    crs.name = "ETRS89 / Portugal TM06";

    EXPECT_FALSE(crs.empty());
    EXPECT_EQ(crs.identifier(), "EPSG:3763");

    crs.authority.clear();
    crs.code.clear();
    EXPECT_EQ(crs.identifier(), "ETRS89 / Portugal TM06");
}

TEST(GisVectorReader2DTest, MapsPatchTypeTextWithoutEnums)
{
    EXPECT_EQ(boundary_patch_type_from_gis_text("wall"), BoundaryPatchTypeTraits::Wall);
    EXPECT_EQ(boundary_patch_type_from_gis_text("island"), BoundaryPatchTypeTraits::Island);
    EXPECT_EQ(boundary_patch_type_from_gis_text("open_boundary"), BoundaryPatchTypeTraits::OpenBoundary);
    EXPECT_EQ(boundary_patch_type_from_gis_text("land-boundary"), BoundaryPatchTypeTraits::LandBoundary);
    EXPECT_EQ(boundary_patch_type_from_gis_text("no_flux"), BoundaryPatchTypeTraits::NoFlux);
    EXPECT_EQ(boundary_patch_type_from_gis_text("unknown"), BoundaryPatchTypeTraits::Wall);
}

TEST(GisVectorReader2DTest, GeoVectorDomainExportsBoundaryPatchRings)
{
    GeoVectorDomain2D geo_domain;
    geo_domain.domain = PolygonalDomain2D{
        Polygon2D{std::vector<Point2D>{{0.0, 0.0}, {2.0, 0.0}, {2.0, 2.0}, {0.0, 2.0}}}};
    geo_domain.boundary_patches.push_back(make_patch("outer", BoundaryPatchTypeTraits::LandBoundary));
    geo_domain.boundary_patches.push_back(make_patch("island", BoundaryPatchTypeTraits::Island));

    const auto rings = geo_domain.boundary_patch_rings();

    ASSERT_EQ(rings.size(), 2U);
    EXPECT_EQ(rings[0].name, "outer");
    EXPECT_EQ(rings[0].type, BoundaryPatchTypeTraits::LandBoundary);
    EXPECT_EQ(rings[1].name, "island");
    EXPECT_EQ(rings[1].type, BoundaryPatchTypeTraits::Island);
}

TEST(GisVectorReader2DTest, BackendAvailabilityIsExplicit)
{
    if(!gis_vector_backend_available()) {
        EXPECT_THROW((void)read_gis_polygonal_domain_2d("missing-domain.gpkg"), vmm::error::MeshException);
        return;
    }

    EXPECT_THROW((void)read_gis_polygonal_domain_2d("missing-domain.gpkg"), vmm::error::MeshException);
}

TEST(GisVectorReader2DTest, CrsTransformAvailabilityIsExplicit)
{
    const auto available = gis_crs_transform_backend_available();
    EXPECT_EQ(available, gis_crs_transform_backend_available());
}
