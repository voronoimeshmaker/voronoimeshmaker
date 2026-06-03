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
// @file ex_openfoam_export.cpp
// @brief Demonstrates the guarded OpenFOAM exporter entry point.
// @ingroup vmm_examples
//==============================================================================

#include <vmm/export/OpenFOAMExporter.hpp>

#include <filesystem>
#include <iostream>

int main()
{
    const vmm::core::MeshTopology<vmm::core::Dimension::D3> topology{1U};

    try {
        vmm::io::write_openfoam_polymesh(std::filesystem::path{"polyMesh"}, topology);
    } catch(const vmm::error::MeshException& exception) {
        std::cout << "OpenFOAM export is not available for cell-centred topology: "
                  << exception.error().message() << '\n';
        return 0;
    }

    return 1;
}
