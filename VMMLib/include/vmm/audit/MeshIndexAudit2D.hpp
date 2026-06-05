//==============================================================================
// File        : MeshIndexAudit2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Index audit for canonical 2D finite-volume mesh tables.
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
// @file MeshIndexAudit2D.hpp
// @brief Validates ID ranges and references in 2D finite-volume mesh tables.
// @ingroup vmm_audit
//==============================================================================
#pragma once

#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

namespace vmm::audit {

class MeshIndexAudit2D final {
public:
    [[nodiscard]] MeshAuditReport run(const vmm::mesh::FiniteVolumeMesh2D& mesh) const;
};

[[nodiscard]] MeshAuditReport audit_mesh_indices_2d(const vmm::mesh::FiniteVolumeMesh2D& mesh);

} // namespace vmm::audit
