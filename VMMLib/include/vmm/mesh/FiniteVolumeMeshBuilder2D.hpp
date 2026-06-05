//==============================================================================
// File        : FiniteVolumeMeshBuilder2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Reconstructs canonical 2D FV meshes from planar cells.
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
// @file FiniteVolumeMeshBuilder2D.hpp
// @brief Builds canonical face-based 2D finite-volume meshes from planar cells.
// @ingroup vmm_mesh
//==============================================================================
#pragma once

#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/mesh/BoundaryPatchTable.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

#include <span>
#include <string>

namespace vmm::mesh {

struct FiniteVolumeMeshBuilder2DOptions final {
    vmm::core::Real vertex_tolerance{1.0e-12};
    std::string default_boundary_patch_name{"default-boundary"};
    BoundaryPatchType default_boundary_patch_type{BoundaryPatchTypeTraits::Wall};
};

class FiniteVolumeMeshBuilder2D final {
public:
    [[nodiscard]] FiniteVolumeMesh2D build(std::span<const vmm::domain::PlanarCell2D> cells,
                                           const FiniteVolumeMeshBuilder2DOptions& options = {}) const;
};

[[nodiscard]] FiniteVolumeMesh2D build_finite_volume_mesh_2d(
    std::span<const vmm::domain::PlanarCell2D> cells,
    const FiniteVolumeMeshBuilder2DOptions& options = {});

} // namespace vmm::mesh
