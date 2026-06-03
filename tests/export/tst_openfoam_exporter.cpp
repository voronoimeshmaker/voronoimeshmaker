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
// @file tst_openfoam_exporter.cpp
// @brief Unit tests for the OpenFOAM exporter interface.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/export/OpenFOAMExporter.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

TEST(OpenFOAMExporterTest, RejectsTopologyWithoutFaces)
{
    const vmm::core::MeshTopology<vmm::core::Dimension::D3> topology{1U};
    const auto directory = std::filesystem::temp_directory_path() / "vmm_openfoam_polyMesh";

    EXPECT_THROW(static_cast<void>(vmm::io::write_openfoam_polymesh(directory, topology)),
                 vmm::error::MeshException);
}

} // namespace
