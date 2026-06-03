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
// @file OpenFOAMExporter.hpp
// @brief OpenFOAM polyMesh exporter interface.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/MeshTopology.hpp>

#include <filesystem>

namespace vmm::io {

/**
 * @brief Reserved OpenFOAM polyMesh writer.
 * @ingroup vmm_export
 *
 * OpenFOAM requires face-centred connectivity: points, faces, owner, neighbour,
 * and boundary patches. MeshTopology currently stores only cell-centred data, so
 * this function fails explicitly rather than writing an invalid polyMesh.
 */
template<vmm::core::Dimension Dim>
void write_openfoam_polymesh(const std::filesystem::path& directory,
                             const vmm::core::MeshTopology<Dim>& topology)
{
    static_cast<void>(directory);
    static_cast<void>(topology);
    vmm::error::throw_error(vmm::error::builtin_error_id::unsupported_operation,
                            "write_openfoam_polymesh",
                            "OpenFOAM export requires face-centred mesh geometry.");
}

} // namespace vmm::io
