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
// @file tst_msh_exporter.cpp
// @brief Unit tests for the Gmsh MSH exporter.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/export/MSHExporter.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

[[nodiscard]] std::string read_text_file(const std::filesystem::path& path)
{
    std::ifstream input{path};
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

TEST(MSHExporterTest, WritesD3CellCentresAsPointElements)
{
    vmm::core::MeshTopology<vmm::core::Dimension::D3> topology{1U};
    topology.set_centroid(0U, {1.0, 2.0, 3.0});

    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_msh_exporter.msh";
    vmm::io::write_msh_cell_centres(output, topology);

    const std::string text = read_text_file(output);
    EXPECT_NE(text.find("$MeshFormat"), std::string::npos);
    EXPECT_NE(text.find("2.2 0 8"), std::string::npos);
    EXPECT_NE(text.find("$Nodes\n1\n1 1 2 3"), std::string::npos);
    EXPECT_NE(text.find("$Elements\n1\n1 15 2 0 0 1"), std::string::npos);
}

} // namespace
