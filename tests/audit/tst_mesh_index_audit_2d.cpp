#include <vmm/audit/MeshIndexAudit2D.hpp>

#include <gtest/gtest.h>

#include <algorithm>
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

TEST(MeshAuditReportTest, CustomSeverityCanBeUsedWithoutChangingCentralEnum)
{
    constexpr MeshAuditSeverity structuralFailure{"structural-failure", MeshAuditSeverityTraits::Fatal.rank};
    MeshAuditReport report;

    report.add(structuralFailure, "VMM-IDX-CUSTOM", "Custom fatal audit issue");

    ASSERT_EQ(report.issue_count(), 1U);
    EXPECT_EQ(report.all_issues().front().severity, structuralFailure);
    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(report.has_fatal_errors());
}

TEST(MeshIndexAudit2DTest, ValidManualMeshPasses)
{
    const auto mesh = make_two_cell_mesh();

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_TRUE(report.valid());
    EXPECT_TRUE(report.empty());
    EXPECT_EQ(report.issue_count(), 0U);
}

TEST(MeshIndexAudit2DTest, InconsistentTableStorageFailsWithoutIndexingPastEnd)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.owner.pop_back();

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(report.has_fatal_errors());
    EXPECT_TRUE(has_code(report, "VMM-IDX-000"));
}

TEST(MeshIndexAudit2DTest, InvalidFaceOwnerFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.owner[0] = CellId{99U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-006"));
}

TEST(MeshIndexAudit2DTest, OutOfRangeNeighbourFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.neighbour[1] = CellId{99U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-007"));
}

TEST(MeshIndexAudit2DTest, BoundaryFaceWithoutPatchFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.patch_id[0] = BoundaryPatchId::invalid();

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-008"));
}

TEST(MeshIndexAudit2DTest, InternalFaceWithPatchFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.patch_id[1] = BoundaryPatchId{0U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-009"));
}

TEST(MeshIndexAudit2DTest, CellFaceOffsetPastConnectivityFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cells.face_offset[1] = 100U;

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-010"));
}

TEST(MeshIndexAudit2DTest, InvalidCellFaceReferenceFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.cell_faces.face_ids[3] = FaceId{77U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-002"));
}

TEST(MeshIndexAudit2DTest, InvalidFaceNodeFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.node0[0] = NodeId{88U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-003"));
}

TEST(MeshIndexAudit2DTest, InvalidPatchTableIdFails)
{
    auto mesh = make_two_cell_mesh();
    mesh.patches.id[1] = BoundaryPatchId{42U};

    const auto report = audit_mesh_indices_2d(mesh);

    EXPECT_FALSE(report.valid());
    EXPECT_TRUE(has_code(report, "VMM-IDX-005"));
}
