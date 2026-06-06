//==============================================================================
// File        : FiniteVolumeStencilGraph2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Solver-neutral stencil graph derived from 2D FV face tables.
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
// @file FiniteVolumeStencilGraph2D.hpp
// @brief Builds compact internal and boundary stencil arrays from FV faces.
// @ingroup vmm_mesh
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <span>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

namespace vmm::mesh {

struct InternalStencilTable2D final {
    std::vector<vmm::core::FaceId> face_id;
    std::vector<vmm::core::CellId> owner;
    std::vector<vmm::core::CellId> neighbour;
    std::vector<vmm::core::Real> centre_distance;
    std::vector<vmm::core::Real> owner_to_neighbour_x;
    std::vector<vmm::core::Real> owner_to_neighbour_y;
    std::vector<vmm::core::Real> face_length;

    [[nodiscard]] std::size_t size() const noexcept { return face_id.size(); }
    [[nodiscard]] bool empty() const noexcept { return face_id.empty(); }
    [[nodiscard]] bool has_consistent_storage() const noexcept;
};

struct BoundaryStencilTable2D final {
    std::vector<vmm::core::FaceId> face_id;
    std::vector<vmm::core::CellId> cell;
    std::vector<vmm::core::BoundaryPatchId> patch_id;
    std::vector<vmm::core::Real> centre_to_face_distance;
    std::vector<vmm::core::Real> normal_x;
    std::vector<vmm::core::Real> normal_y;
    std::vector<vmm::core::Real> face_length;

    [[nodiscard]] std::size_t size() const noexcept { return face_id.size(); }
    [[nodiscard]] bool empty() const noexcept { return face_id.empty(); }
    [[nodiscard]] bool has_consistent_storage() const noexcept;
};

struct FiniteVolumeStencilGraph2D final {
    InternalStencilTable2D internal;
    BoundaryStencilTable2D boundary;

    [[nodiscard]] std::size_t internal_count() const noexcept { return internal.size(); }
    [[nodiscard]] std::size_t boundary_count() const noexcept { return boundary.size(); }
    [[nodiscard]] bool has_consistent_storage() const noexcept;
};

struct FiniteVolumeStencilGraph2DOptions final {
    bool require_valid_mesh{true};
};

class FiniteVolumeStencilGraphBuilder2D final {
public:
    [[nodiscard]] FiniteVolumeStencilGraph2D build(
        const FiniteVolumeMesh2D& mesh,
        const FiniteVolumeStencilGraph2DOptions& options = {}) const;
};

[[nodiscard]] FiniteVolumeStencilGraph2D build_finite_volume_stencil_graph_2d(
    const FiniteVolumeMesh2D& mesh,
    const FiniteVolumeStencilGraph2DOptions& options = {});

} // namespace vmm::mesh
