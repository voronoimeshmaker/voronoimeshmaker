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
#include <vmm/domain/Transform3D.hpp>

using namespace vmm::domain;

namespace {

TEST(Transform3DTest, TranslatesPoint)
{
    const auto point = translate(Point3D{1.0, 2.0, 3.0}, 4.0, -1.0, 2.0);

    EXPECT_DOUBLE_EQ(point.x, 5.0);
    EXPECT_DOUBLE_EQ(point.y, 1.0);
    EXPECT_DOUBLE_EQ(point.z, 5.0);
}

TEST(Transform3DTest, RotatesPointAroundZAxis)
{
    const auto point = rotate_about_z(Point3D{1.0, 0.0, 2.0}, std::numbers::pi_v<double> / 2.0);

    EXPECT_NEAR(point.x, 0.0, 1.0e-12);
    EXPECT_NEAR(point.y, 1.0, 1.0e-12);
    EXPECT_DOUBLE_EQ(point.z, 2.0);
}

TEST(Transform3DTest, TranslatesSurfaceMesh)
{
    const auto moved = translated(make_box_3d(1.0, 1.0, 1.0).to_polyhedron().surface(), 1.0, 2.0, 3.0);

    EXPECT_EQ(moved.vertex_count(), 8U);
    EXPECT_DOUBLE_EQ(moved.bounds().min.z, 2.5);
}

} // namespace
