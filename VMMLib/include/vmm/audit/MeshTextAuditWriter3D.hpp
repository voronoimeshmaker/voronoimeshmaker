//==============================================================================
// File        : MeshTextAuditWriter3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Stable text audit writer for extruded 3D FV meshes.
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
#include <vmm/mesh/FiniteVolumeMesh3D.hpp>

namespace vmm::audit {

class MeshTextAuditWriter3D final {
public:
    void write(std::ostream& output,
               const vmm::mesh::FiniteVolumeMesh3D& mesh,
               const MeshAuditReport& report) const;

    void write(const std::filesystem::path& file_path,
               const vmm::mesh::FiniteVolumeMesh3D& mesh,
               const MeshAuditReport& report) const;
};

void write_mesh_text_audit_3d(std::ostream& output,
                              const vmm::mesh::FiniteVolumeMesh3D& mesh,
                              const MeshAuditReport& report);

void write_mesh_text_audit_3d(const std::filesystem::path& file_path,
                              const vmm::mesh::FiniteVolumeMesh3D& mesh,
                              const MeshAuditReport& report);

void write_mesh_text_audit_3d(std::ostream& output, const vmm::mesh::FiniteVolumeMesh3D& mesh);

void write_mesh_text_audit_3d(const std::filesystem::path& file_path, const vmm::mesh::FiniteVolumeMesh3D& mesh);

} // namespace vmm::audit
