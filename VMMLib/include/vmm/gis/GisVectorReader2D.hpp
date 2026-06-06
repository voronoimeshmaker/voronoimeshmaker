//==============================================================================
// File        : GisVectorReader2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Public GIS vector reader API for 2D domains.
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.
//
// @file GisVectorReader2D.hpp
// @brief Reads GIS vector polygons into VMM-owned domain and patch structures.
// @ingroup vmm_gis
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Polygon2D.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/mesh/BoundaryPatchTable.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

namespace vmm::gis {

struct CoordinateReferenceSystem final {
    std::string authority;
    std::string code;
    std::string name;
    std::string wkt;
    bool geographic{};

    [[nodiscard]] bool empty() const noexcept
    {
        return authority.empty() && code.empty() && name.empty() && wkt.empty();
    }

    [[nodiscard]] std::string identifier() const
    {
        if(!authority.empty() && !code.empty()) {
            return authority + ":" + code;
        }
        if(!name.empty()) {
            return name;
        }
        return {};
    }
};

struct GisBoundaryPatch2D final {
    vmm::domain::Polygon2D ring;
    std::string name;
    vmm::mesh::BoundaryPatchType type{vmm::mesh::BoundaryPatchTypeTraits::Wall};
    std::string source_layer;
    std::string source_feature_id;
};

struct GeoVectorDomain2D final {
    vmm::domain::PolygonalDomain2D domain;
    CoordinateReferenceSystem source_crs;
    CoordinateReferenceSystem working_crs;
    std::vector<GisBoundaryPatch2D> boundary_patches;

    [[nodiscard]] std::vector<vmm::mesh::BoundaryPatchRing2D> boundary_patch_rings() const
    {
        std::vector<vmm::mesh::BoundaryPatchRing2D> rings;
        rings.reserve(boundary_patches.size());
        for(const auto& patch : boundary_patches) {
            rings.push_back(vmm::mesh::BoundaryPatchRing2D{patch.ring, patch.name, patch.type});
        }
        return rings;
    }
};

struct GisVectorReadOptions final {
    std::string layer_name;
    std::string name_attribute{"name"};
    std::string patch_type_attribute{"type"};
    CoordinateReferenceSystem working_crs;
    bool transform_to_working_crs{true};
};

[[nodiscard]] bool gis_vector_backend_available() noexcept;
[[nodiscard]] bool gis_crs_transform_backend_available() noexcept;

[[nodiscard]] bool coordinate_reference_systems_compatible(
    const CoordinateReferenceSystem& lhs,
    const CoordinateReferenceSystem& rhs) noexcept;

void require_compatible_coordinate_reference_systems(std::string_view context,
                                                     std::string_view lhs_name,
                                                     const CoordinateReferenceSystem& lhs,
                                                     std::string_view rhs_name,
                                                     const CoordinateReferenceSystem& rhs);

[[nodiscard]] vmm::mesh::BoundaryPatchType boundary_patch_type_from_gis_text(std::string_view text) noexcept;

[[nodiscard]] GeoVectorDomain2D read_gis_polygonal_domain_2d(
    const std::filesystem::path& file_path,
    const GisVectorReadOptions& options = {});

} // namespace vmm::gis
