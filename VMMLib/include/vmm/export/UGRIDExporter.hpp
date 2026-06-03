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
// @file UGRIDExporter.hpp
// @brief UGRID exporter interface.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <vmm/core/Types.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/mesh/MeshTopology.hpp>

#include <filesystem>

namespace vmm::io {

/**
 * @brief Reserved UGRID writer.
 * @ingroup vmm_export
 */
template<vmm::core::Dimension Dim>
void write_ugrid_ascii(const std::filesystem::path& file_path,
                       const vmm::core::MeshTopology<Dim>& topology)
{
    static_cast<void>(file_path);
    static_cast<void>(topology);
    vmm::error::throw_error(vmm::error::builtin_error_id::unsupported_operation,
                            "write_ugrid_ascii",
                            "UGRID export requires explicit cell faces and vertices.");
}

} // namespace vmm::io
