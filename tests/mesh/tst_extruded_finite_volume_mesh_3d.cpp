//==============================================================================
// c++ includes
//==============================================================================
#include <sstream>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshExtrusionAudit3D.hpp>
#include <vmm/audit/MeshTextAuditWriter3D.hpp>
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/ExtrudedFiniteVolumeMesh3D.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::mesh;

namespace {

PlanarCell2D make_cell(std::vector<Point2D> vertices, Point2D site, GeneratorID generator_id)
{
    return PlanarCell2D{Polygon2D{std::move(vertices)}, site, generator_id};
}

FiniteVolumeMesh2D make_two_square_mesh()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 1U),
    };
    return build_finite_volume_mesh_2d(cells);
}

std::size_t count_faces_with_role(const FiniteVolumeMesh3D& mesh, FaceRole3D role)
{
    std::size_t count{};
    for(const auto face_role : mesh.faces.role) {
        if(face_role == role) {
            ++count;
        }
    }
    return count;
}

} // namespace

TEST(ExtrudedFiniteVolumeMesh3DTest, ExtrudesTwoLayerMeshWithCanonicalCounts)
{
    const auto mesh2d = make_two_square_mesh();
    const std::vector<Real> z_interfaces{0.0, 2.0, 5.0};

    const auto mesh3d = extrude_finite_volume_mesh_3d(mesh2d, z_interfaces);

    EXPECT_EQ(mesh3d.node_count(), 18U);
    EXPECT_EQ(mesh3d.cell_count(), 4U);
    EXPECT_EQ(mesh3d.face_count(), 20U);
    EXPECT_EQ(mesh3d.patch_count(), mesh2d.patch_count() + 2U);
    EXPECT_EQ(mesh3d.cell_faces.size(), 24U);
    EXPECT_TRUE(mesh3d.has_consistent_storage());
    EXPECT_EQ(count_faces_with_role(mesh3d, FaceRole3DTraits::Lateral), 14U);
    EXPECT_EQ(count_faces_with_role(mesh3d, FaceRole3DTraits::Bottom), 2U);
    EXPECT_EQ(count_faces_with_role(mesh3d, FaceRole3DTraits::Top), 2U);
    EXPECT_EQ(count_faces_with_role(mesh3d, FaceRole3DTraits::InternalHorizontal), 2U);
}

TEST(ExtrudedFiniteVolumeMesh3DTest, ComputesVolumesAndFaceAreas)
{
    const auto mesh2d = make_two_square_mesh();
    const std::vector<Real> z_interfaces{0.0, 2.0, 5.0};

    const auto mesh3d = extrude_finite_volume_mesh_3d(mesh2d, z_interfaces);

    ASSERT_EQ(mesh3d.cell_count(), 4U);
    EXPECT_DOUBLE_EQ(mesh3d.cells.volume[0], 2.0);
    EXPECT_DOUBLE_EQ(mesh3d.cells.volume[1], 2.0);
    EXPECT_DOUBLE_EQ(mesh3d.cells.volume[2], 3.0);
    EXPECT_DOUBLE_EQ(mesh3d.cells.volume[3], 3.0);
    EXPECT_DOUBLE_EQ(mesh3d.faces.area[0], 2.0);
    EXPECT_DOUBLE_EQ(mesh3d.faces.area[7], 3.0);
}

TEST(ExtrudedFiniteVolumeMesh3DTest, AuditAcceptsCleanExtrusion)
{
    const auto mesh2d = make_two_square_mesh();
    const std::vector<Real> z_interfaces{0.0, 1.0, 3.0};

    const auto mesh3d = extrude_finite_volume_mesh_3d(mesh2d, z_interfaces);
    const auto report = vmm::audit::audit_extruded_mesh_3d(mesh3d);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(report.empty());
}

TEST(ExtrudedFiniteVolumeMesh3DTest, RejectsInvalidVerticalInterfaces)
{
    const auto mesh2d = make_two_square_mesh();

    EXPECT_THROW((void)extrude_finite_volume_mesh_3d(mesh2d, std::vector<Real>{0.0}),
                 vmm::error::MeshException);
    EXPECT_THROW((void)extrude_finite_volume_mesh_3d(mesh2d, std::vector<Real>{0.0, 0.0}),
                 vmm::error::MeshException);
}

TEST(ExtrudedFiniteVolumeMesh3DTest, TextAuditWritesStableSections)
{
    const auto mesh2d = make_two_square_mesh();
    const std::vector<Real> z_interfaces{0.0, 1.0};
    const auto mesh3d = extrude_finite_volume_mesh_3d(mesh2d, z_interfaces);
    std::ostringstream output;

    vmm::audit::write_mesh_text_audit_3d(output, mesh3d);

    EXPECT_NE(output.str().find("[mesh_3d]\n"), std::string::npos);
    EXPECT_NE(output.str().find("valid: true\n"), std::string::npos);
    EXPECT_NE(output.str().find("[faces]\n"), std::string::npos);
}
