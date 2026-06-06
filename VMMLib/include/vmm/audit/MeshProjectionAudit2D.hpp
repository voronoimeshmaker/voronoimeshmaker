//==============================================================================
// File        : MeshProjectionAudit2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Projection and orthogonality audit for 2D FV meshes.
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
// @file MeshProjectionAudit2D.hpp
// @brief Reports projection and non-orthogonality anomalies without remeshing.
// @ingroup vmm_audit
//==============================================================================
#pragma once

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/core/Types.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

namespace vmm::audit {

struct MeshProjectionAudit2DOptions final {
    vmm::core::Real internal_midpoint_tolerance{1.0e-9};
    vmm::core::Real boundary_projection_tolerance{1.0e-9};
    vmm::core::Real max_non_orthogonality_degrees{70.0};
    bool run_base_audits{true};
};

class MeshProjectionAudit2D final {
public:
    [[nodiscard]] MeshAuditReport run(
        const vmm::mesh::FiniteVolumeMesh2D& mesh,
        const MeshProjectionAudit2DOptions& options = {}) const;
};

[[nodiscard]] MeshAuditReport audit_mesh_projection_2d(
    const vmm::mesh::FiniteVolumeMesh2D& mesh,
    const MeshProjectionAudit2DOptions& options = {});

} // namespace vmm::audit
