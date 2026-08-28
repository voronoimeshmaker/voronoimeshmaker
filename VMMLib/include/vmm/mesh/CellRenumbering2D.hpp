//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 VoronoiMeshMaker
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
// @file CellRenumbering2D.hpp
// @brief Cell renumbering methods and bandwidth comparison helpers.
// @ingroup vmm_mesh
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>
#include <vmm/mesh/FiniteVolumeStencilGraph2D.hpp>

namespace vmm::mesh {

struct CellRenumberingMethod2D final {
    std::string_view name;

    [[nodiscard]] friend constexpr bool operator==(CellRenumberingMethod2D lhs,
                                                   CellRenumberingMethod2D rhs) noexcept
    {
        return lhs.name == rhs.name;
    }

    [[nodiscard]] friend constexpr bool operator!=(CellRenumberingMethod2D lhs,
                                                   CellRenumberingMethod2D rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

struct CellRenumberingMethod2DTraits final {
    static constexpr CellRenumberingMethod2D Native{"native"};
    static constexpr CellRenumberingMethod2D Random{"random"};
    static constexpr CellRenumberingMethod2D ReverseCuthillMckee{"reverse-cuthill-mckee"};
    static constexpr CellRenumberingMethod2D Hilbert{"hilbert"};
    static constexpr CellRenumberingMethod2D Morton{"morton"};
    static constexpr CellRenumberingMethod2D MetisNestedDissection{"metis-nested-dissection"};
};

struct CellPermutation2D final {
    std::vector<vmm::core::CellId> old_to_new;
    std::vector<vmm::core::CellId> new_to_old;

    [[nodiscard]] std::size_t size() const noexcept { return old_to_new.size(); }
    [[nodiscard]] bool empty() const noexcept { return old_to_new.empty(); }
    [[nodiscard]] bool has_consistent_storage() const noexcept;
    [[nodiscard]] vmm::core::CellId new_id_for(vmm::core::CellId old_id) const;
    [[nodiscard]] vmm::core::CellId old_id_for(vmm::core::CellId new_id) const;
};

struct CellRenumbering2DOptions final {
    std::uint32_t random_seed{5489U};
    std::uint8_t space_filling_curve_bits{16U};
    bool require_valid_mesh{true};
};

using CellRenumberingFunction2D = CellPermutation2D (*)(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options);

struct CellRenumberingAlgorithm2D final {
    CellRenumberingMethod2D method;
    CellRenumberingFunction2D compute{};
};

[[nodiscard]] CellPermutation2D cell_renumbering_native_2d(const FiniteVolumeMesh2D& mesh);

[[nodiscard]] CellPermutation2D cell_renumbering_random_2d(const FiniteVolumeMesh2D& mesh,
                                                           const CellRenumbering2DOptions& options = {});

[[nodiscard]] CellPermutation2D cell_renumbering_reverse_cuthill_mckee_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] CellPermutation2D cell_renumbering_hilbert_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] CellPermutation2D cell_renumbering_morton_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] CellPermutation2D cell_renumbering_metis_nested_dissection_2d(
    const FiniteVolumeMesh2D& mesh,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] std::span<const CellRenumberingAlgorithm2D> default_cell_renumbering_algorithms_2d() noexcept;

[[nodiscard]] CellPermutation2D compute_cell_renumbering_2d(
    const FiniteVolumeMesh2D& mesh,
    CellRenumberingMethod2D method,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] CellPermutation2D compute_cell_renumbering_2d(
    const FiniteVolumeMesh2D& mesh,
    CellRenumberingMethod2D method,
    std::span<const CellRenumberingAlgorithm2D> algorithms,
    const CellRenumbering2DOptions& options = {});

[[nodiscard]] std::uint64_t finite_volume_matrix_half_bandwidth_2d(
    const FiniteVolumeStencilGraph2D& graph,
    const CellPermutation2D& permutation);

[[nodiscard]] std::uint64_t finite_volume_matrix_bandwidth_2d(
    const FiniteVolumeStencilGraph2D& graph,
    const CellPermutation2D& permutation);

} // namespace vmm::mesh
