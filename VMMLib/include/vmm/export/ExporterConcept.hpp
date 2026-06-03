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
// @file ExporterConcept.hpp
// @brief Concepts for mesh exporters.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <filesystem>

namespace vmm::io {

/**
 * @brief Exporter concept for types that can write a mesh-like object.
 * @ingroup vmm_export
 */
template<class Exporter, class Mesh>
concept MeshExporter = requires(const Exporter& exporter,
                                const Mesh& mesh,
                                const std::filesystem::path& path) {
    { exporter.write(path, mesh) };
};

} // namespace vmm::io
