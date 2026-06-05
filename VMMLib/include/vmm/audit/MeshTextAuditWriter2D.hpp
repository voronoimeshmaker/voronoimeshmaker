//==============================================================================
// File        : MeshTextAuditWriter2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Stable text audit writer for canonical 2D FV meshes.
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
// @file MeshTextAuditWriter2D.hpp
// @brief Writes deterministic diagnostic text for canonical 2D FV meshes.
// @ingroup vmm_audit
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <iosfwd>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshAuditReport.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>


namespace vmm::audit {

class MeshTextAuditWriter2D final {
public:
    void write(std::ostream& output,
               const vmm::mesh::FiniteVolumeMesh2D& mesh,
               const MeshAuditReport& report) const;

    void write(const std::filesystem::path& file_path,
               const vmm::mesh::FiniteVolumeMesh2D& mesh,
               const MeshAuditReport& report) const;
};

void write_mesh_text_audit_2d(std::ostream& output,
                              const vmm::mesh::FiniteVolumeMesh2D& mesh,
                              const MeshAuditReport& report);

void write_mesh_text_audit_2d(const std::filesystem::path& file_path,
                              const vmm::mesh::FiniteVolumeMesh2D& mesh,
                              const MeshAuditReport& report);

void write_mesh_text_audit_2d(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh);

void write_mesh_text_audit_2d(const std::filesystem::path& file_path, const vmm::mesh::FiniteVolumeMesh2D& mesh);

} // namespace vmm::audit
