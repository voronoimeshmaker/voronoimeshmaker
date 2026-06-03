//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file tst_vtk_exporter.cpp
// @brief Unit tests for the VTK exporter.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/export/VTKExporter.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

[[nodiscard]] std::string read_text_file(const std::filesystem::path& path)
{
    std::ifstream input{path};
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

TEST(VTKExporterTest, WritesD2CellCentresAsVertexCells)
{
    vmm::core::MeshTopology<vmm::core::Dimension::D2> topology{2U};
    topology.set_centroid(0U, {1.0, 2.0});
    topology.set_centroid(1U, {3.0, 4.0});
    topology.assign_generator_id(0U, 10U);
    topology.assign_generator_id(1U, 11U);
    topology.set_boundary_flag(1U, vmm::core::BoundaryType::ExternalBoundary);

    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_vtk_exporter.vtu";
    vmm::io::write_vtu_cell_centres(output, topology);

    const std::string text = read_text_file(output);
    EXPECT_NE(text.find("<VTKFile type=\"UnstructuredGrid\""), std::string::npos);
    EXPECT_NE(text.find("NumberOfPoints=\"2\" NumberOfCells=\"2\""), std::string::npos);
    EXPECT_NE(text.find("1 2 0"), std::string::npos);
    EXPECT_NE(text.find("3 4 0"), std::string::npos);
    EXPECT_NE(text.find("generator_id"), std::string::npos);
    EXPECT_NE(text.find("boundary_type"), std::string::npos);
}

TEST(VTKExporterTest, WritesPolygonalDomainAsLegacyPolyData)
{
    const vmm::domain::Polygon2D exterior{std::vector<vmm::domain::Point2D>{
        {0.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {0.0, 1.0}
    }};
    const vmm::domain::PolygonalDomain2D domain{exterior};
    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_domain.vtk";

    vmm::io::write_legacy_vtk_polygonal_domain_2d(output, "domain", domain);
    const std::string text = read_text_file(output);

    EXPECT_NE(text.find("DATASET POLYDATA"), std::string::npos);
    EXPECT_NE(text.find("POINTS 4 double"), std::string::npos);
    EXPECT_NE(text.find("LINES 1 6"), std::string::npos);
}

} // namespace
