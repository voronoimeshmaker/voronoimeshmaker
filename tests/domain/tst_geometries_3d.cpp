//==============================================================================
// c++ includes
//==============================================================================
#include <numbers>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Box3D.hpp>
#include <vmm/domain/Cylinder3D.hpp>
#include <vmm/domain/DomainConcept.hpp>
#include <vmm/domain/Ellipsoid3D.hpp>
#include <vmm/domain/Sphere3D.hpp>
#include <vmm/domain/Tetrahedron3D.hpp>
#include <vmm/domain/Transform3D.hpp>
#include <vmm/error/MeshException.hpp>

using namespace vmm::domain;

namespace {

TEST(Geometries3DTest, BoxComputesBoundsVolumeAndSurface)
{
    const auto box = make_box_3d(2.0, 3.0, 4.0, Point3D{1.0, 2.0, 3.0});

    EXPECT_DOUBLE_EQ(box.volume(), 24.0);
    EXPECT_TRUE(box.contains(Point3D{1.0, 2.0, 3.0}));
    EXPECT_FALSE(box.contains(Point3D{3.0, 2.0, 3.0}));

    const auto bounds = box.bounds();
    EXPECT_DOUBLE_EQ(bounds.min.x, 0.0);
    EXPECT_DOUBLE_EQ(bounds.max.z, 5.0);

    const auto polyhedron = box.to_polyhedron();
    EXPECT_EQ(polyhedron.surface().vertex_count(), 8U);
    EXPECT_EQ(polyhedron.surface().face_count(), 12U);
    EXPECT_DOUBLE_EQ(polyhedron.volume(), box.volume());
}

TEST(Geometries3DTest, AnalyticRoundDomainsComputeVolumeBoundsAndContainment)
{
    const auto sphere = make_sphere_3d(2.0);
    const auto cylinder = make_cylinder_3d(2.0, 5.0);
    const auto ellipsoid = make_ellipsoid_3d(1.0, 2.0, 3.0);
    const auto pi = static_cast<vmm::core::Real>(std::numbers::pi_v<long double>);

    EXPECT_DOUBLE_EQ(sphere.volume(), (4.0 / 3.0) * pi * 8.0);
    EXPECT_DOUBLE_EQ(cylinder.volume(), pi * 4.0 * 5.0);
    EXPECT_DOUBLE_EQ(ellipsoid.volume(), (4.0 / 3.0) * pi * 6.0);
    EXPECT_TRUE(sphere.contains(Point3D{1.0, 1.0, 1.0}));
    EXPECT_FALSE(cylinder.contains(Point3D{3.0, 0.0, 0.0}));
    EXPECT_TRUE(ellipsoid.contains(Point3D{0.0, 1.0, 0.0}));
}

TEST(Geometries3DTest, TetrahedronComputesVolumeAndSurface)
{
    const auto tetrahedron = make_tetrahedron_3d(
        Point3D{0.0, 0.0, 0.0},
        Point3D{1.0, 0.0, 0.0},
        Point3D{0.0, 1.0, 0.0},
        Point3D{0.0, 0.0, 1.0});

    EXPECT_DOUBLE_EQ(tetrahedron.volume(), 1.0 / 6.0);
    EXPECT_TRUE(tetrahedron.contains(Point3D{0.1, 0.1, 0.1}));
    EXPECT_FALSE(tetrahedron.contains(Point3D{1.0, 1.0, 1.0}));

    const auto polyhedron = tetrahedron.to_polyhedron();
    EXPECT_EQ(polyhedron.surface().face_count(), 4U);
    EXPECT_DOUBLE_EQ(polyhedron.volume(), tetrahedron.volume());
}

TEST(Geometries3DTest, RejectsInvalid3DGeometries)
{
    EXPECT_THROW((void)make_box_3d(0.0, 1.0, 1.0), vmm::error::MeshException);
    EXPECT_THROW((void)make_sphere_3d(-1.0), vmm::error::MeshException);
    EXPECT_THROW((void)make_tetrahedron_3d(
                     Point3D{0.0, 0.0, 0.0},
                     Point3D{1.0, 0.0, 0.0},
                     Point3D{2.0, 0.0, 0.0},
                     Point3D{3.0, 0.0, 0.0}),
                 vmm::error::MeshException);
}

TEST(Geometries3DTest, TransformsPointsAndSurfaceMeshes)
{
    const auto point = translate(Point3D{1.0, 2.0, 3.0}, 4.0, -1.0, 2.0);
    EXPECT_DOUBLE_EQ(point.x, 5.0);
    EXPECT_DOUBLE_EQ(point.y, 1.0);
    EXPECT_DOUBLE_EQ(point.z, 5.0);

    const auto rotated = rotate_about_z(Point3D{1.0, 0.0, 2.0}, std::numbers::pi_v<double> / 2.0);
    EXPECT_NEAR(rotated.x, 0.0, 1.0e-12);
    EXPECT_NEAR(rotated.y, 1.0, 1.0e-12);
    EXPECT_DOUBLE_EQ(rotated.z, 2.0);

    const auto moved_surface = translated(make_box_3d(1.0, 1.0, 1.0).to_polyhedron().surface(), 1.0, 2.0, 3.0);
    EXPECT_EQ(moved_surface.vertex_count(), 8U);
    EXPECT_DOUBLE_EQ(moved_surface.bounds().min.z, 2.5);
}

TEST(Geometries3DTest, PolyhedronModelsPolyhedralDomainConcept)
{
    static_assert(PolyhedralDomain<Polyhedron3D>);
    SUCCEED();
}

} // namespace
