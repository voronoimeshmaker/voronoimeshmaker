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
// @file ExportRegistry.hpp
// @brief Open identifiers for exporter formats.
// @ingroup vmm_export
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::io {

/**
 * @brief Lightweight, open exporter format identifier.
 * @ingroup vmm_export
 */
class ExportFormat final {
public:
    constexpr ExportFormat() noexcept = default;
    constexpr explicit ExportFormat(std::string_view value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::string_view value() const noexcept { return value_; }

private:
    std::string_view value_{"vmm.export.unknown"};
};

namespace export_format {
inline constexpr ExportFormat vtu_ascii{"vmm.export.vtu_ascii"};
inline constexpr ExportFormat msh_ascii{"vmm.export.msh_ascii"};
inline constexpr ExportFormat openfoam_polymesh{"vmm.export.openfoam_polymesh"};
inline constexpr ExportFormat ugrid_ascii{"vmm.export.ugrid_ascii"};
} // namespace export_format

} // namespace vmm::io
