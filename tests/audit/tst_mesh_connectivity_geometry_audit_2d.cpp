#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <string_view>

using namespace vmm::audit;
using namespace vmm::core;
using namespace vmm::mesh;

namespace {

FiniteVolumeMesh2D make_two_cell_mesh()
{
    FiniteVolumeMesh2D mesh;

    mesh.nodes.x = {0.0, 1.0, 2.0, 0.0, 1.0, 2.0};
    mesh.nodes.y = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};

    mesh.cells.centre_x = {0.5, 1.5};
    mesh.cells.centre_y = {0.5, 0.5};
    mesh.cells.area = {1.0, 1.0};
    mesh.cells.site_id = {SiteId{0U}, SiteId{1U}};
    mesh.cells.face_offset = {0U, 4U};
    mesh.cells.face_count = {4U, 4U};

    mesh.patches.id = {BoundaryPatchId{0U}, BoundaryPatchId{1U}};
    mesh.patches.name = {"wall", "open"};
    mesh.patches.type = {BoundaryPatchTypeTraits::Wall, BoundaryPatchTypeTraits::OpenBoundary};

    mesh.faces.owner = {
        CellId{0U}, CellId{0U}, CellId{0U}, CellId{0U}, CellId{1U}, CellId{1U}, CellId{1U}};
    mesh.faces.neighbour = {
        CellId::invalid(), CellId{1U}, CellId::invalid(), CellId::invalid(), CellId::invalid(), CellId::invalid(),
        CellId::invalid()};
    mesh.faces.centre_x = {0.5, 1.0, 0.5, 0.0, 1.5, 2.0, 1.5};
    mesh.faces.centre_y = {0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0};
    mesh.faces.normal_x = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, 0.0};
    mesh.faces.normal_y = {-1.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0};
    mesh.faces.length = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    mesh.faces.patch_id = {
        BoundaryPatchId{0U}, BoundaryPatchId::invalid(), BoundaryPatchId{0U}, BoundaryPatchId{0U},
        BoundaryPatchId{0U}, BoundaryPatchId{1U}, BoundaryPatchId{0U}};
    mesh.faces.node0 = {NodeId{0U}, NodeId{1U}, NodeId{4U}, NodeId{3U}, NodeId{1U}, NodeId{2U}, NodeId{5U}};
    mesh.faces.node1 = {NodeId{1U}, NodeId{4U}, NodeId{3U}, NodeId{0U}, NodeId{2U}, NodeId{5U}, NodeId{4U}};

    mesh.cell_faces.face_ids = {
        FaceId{0U}, FaceId{1U}, FaceId{2U}, FaceId{3U}, FaceId{1U}, FaceId{4U}, FaceId{5U}, FaceId{6U}};

    return mesh;
}

bool has_code(const MeshAuditReport& report, std::string_view code)
{
    return std::any_of(report.issues.begin(), report.issues.end(), [code](const MeshAuditIssue& issue) {
        return issue.code == code;
    });
}

} // namespace

TEST(MeshConnectivityGeometryAudit2DTest, ValidManualMeshPasses)
{
    const auto mesh = make_two_cell_mesh();

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(report.empty());
}

TEST(MeshConnectivityGeometryAudit2DTest, StopsAfterFatalIndexAudit)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.owner[0] = CellId{99U};

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-006"));
    EXPECT_FALSE(has_code(report, "VMM-CON-001"));
}

TEST(MeshConnectivityGeometryAudit2DTest, OwnerCellMustReferenceOwnedFace)
{
    auto mesh = make_two_cell_mesh();
    mesh.cell_faces.face_ids[0] = FaceId{2U};

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-CON-001"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NeighbourCellMustReferenceInternalFace)
{
    auto mesh = make_two_cell_mesh();
    mesh.cell_faces.face_ids[4] = FaceId{4U};

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-CON-002"));
}

TEST(MeshConnectivityGeometryAudit2DTest, DuplicateFaceInCellFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cell_faces.face_ids[2] = FaceId{1U};

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-CON-003"));
}


TEST(MeshConnectivityGeometryAudit2DTest, FaceOccurrenceMustMatchBoundaryOrInternalState)
{
    auto mesh = make_two_cell_mesh();
    mesh.cell_faces.face_ids[6] = FaceId{0U};

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-CON-005"));
}

TEST(MeshConnectivityGeometryAudit2DTest, CellWithTooFewFacesFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cells.face_count[0] = 2U;

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-CON-004"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NonFiniteCellCentreFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cells.centre_x[0] = std::numeric_limits<double>::quiet_NaN();

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-001"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NonPositiveCellAreaFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cells.area[0] = 0.0;

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-002"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NonFiniteFaceCentreFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.centre_y[0] = std::numeric_limits<double>::infinity();

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-003"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NonPositiveFaceLengthFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.length[0] = -1.0;

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-004"));
}

TEST(MeshConnectivityGeometryAudit2DTest, NonFiniteFaceNormalFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.normal_x[0] = std::numeric_limits<double>::quiet_NaN();

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-005"));
}

TEST(MeshConnectivityGeometryAudit2DTest, ZeroFaceNormalFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.normal_y[0] = 0.0;

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-006"));
}

TEST(MeshConnectivityGeometryAudit2DTest, InwardOwnerNormalFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.normal_y[0] = 1.0;

    const auto report = audit_mesh_connectivity_geometry_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-GEO-007"));
}
