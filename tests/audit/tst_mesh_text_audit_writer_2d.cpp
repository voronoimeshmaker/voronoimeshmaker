#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshTextAuditWriter2D.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

std::string read_text_file(const std::filesystem::path& path)
{
    std::ifstream input{path};
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

} // namespace

TEST(MeshTextAuditWriter2DTest, ValidMeshWritesStableSections)
{
    const auto mesh = make_two_cell_mesh();
    const auto report = audit_mesh_connectivity_geometry_2d(mesh);
    std::ostringstream output;

    write_mesh_text_audit_2d(output, mesh, report);
    const auto text = output.str();

    EXPECT_NE(text.find("VMM Mesh Audit 2D\nversion: 1\nvalid: true\n"), std::string::npos);
    EXPECT_LT(text.find("[counts]"), text.find("[issues]"));
    EXPECT_LT(text.find("[issues]"), text.find("[nodes]"));
    EXPECT_LT(text.find("[nodes]"), text.find("[cells]"));
    EXPECT_LT(text.find("[cells]"), text.find("[faces]"));
    EXPECT_LT(text.find("[faces]"), text.find("[patches]"));
    EXPECT_LT(text.find("[patches]"), text.find("[cell_faces]"));
    EXPECT_NE(text.find("cell_face_entries: 8\n"), std::string::npos);
    EXPECT_NE(text.find("1 0 1 1 0.5 1 0 1 invalid 1 4\n"), std::string::npos);
    EXPECT_NE(text.find("0 wall wall 1\n"), std::string::npos);
}

TEST(MeshTextAuditWriter2DTest, ConvenienceWriterRunsAudit)
{
    auto mesh = make_two_cell_mesh();
    mesh.faces.normal_y[0] = 1.0;
    std::ostringstream output;

    write_mesh_text_audit_2d(output, mesh);

    const auto text = output.str();
    EXPECT_NE(text.find("valid: false\n"), std::string::npos);
    EXPECT_NE(text.find("fatal VMM-GEO-007 0 0 - - - Face normal does not point outward from owner\n"),
              std::string::npos);
}

TEST(MeshTextAuditWriter2DTest, WriterKeepsInvalidSentinelsReadable)
{
    const auto mesh = make_two_cell_mesh();
    std::ostringstream output;

    write_mesh_text_audit_2d(output, mesh);

    EXPECT_NE(output.str().find("0 0 invalid 0.5 0 0 -1 1 0 0 1\n"), std::string::npos);
}

TEST(MeshTextAuditWriter2DTest, WritesToFilePath)
{
    const auto mesh = make_two_cell_mesh();
    const auto output_path = std::filesystem::temp_directory_path() / "vmm_mesh_text_audit_2d.txt";

    write_mesh_text_audit_2d(output_path, mesh);

    const auto text = read_text_file(output_path);
    EXPECT_NE(text.find("VMM Mesh Audit 2D\n"), std::string::npos);
    EXPECT_NE(text.find("[cell_faces]\n"), std::string::npos);
}
