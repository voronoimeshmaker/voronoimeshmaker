//==============================================================================
// File        : MeshPatchAudit2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Boundary patch audit for canonical 2D finite-volume meshes.
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
// @file MeshPatchAudit2D.hpp
// @brief Validates boundary patch identity and face assignment semantics.
// @ingroup vmm_audit
//==============================================================================
#pragma once

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

namespace vmm::audit {

class MeshPatchAudit2D final {
public:
    [[nodiscard]] MeshAuditReport run(const vmm::mesh::FiniteVolumeMesh2D& mesh) const;
};

[[nodiscard]] MeshAuditReport audit_mesh_patches_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh);

} // namespace vmm::audit
