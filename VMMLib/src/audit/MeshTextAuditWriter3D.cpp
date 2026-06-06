//==============================================================================
// File        : MeshTextAuditWriter3D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Stable text audit writer for extruded 3D FV meshes.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ostream>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshExtrusionAudit3D.hpp>
#include <vmm/audit/MeshTextAuditWriter3D.hpp>
#include <vmm/error/MeshException.hpp>

namespace vmm::audit {

void MeshTextAuditWriter3D::write(std::ostream& output,
                                  const vmm::mesh::FiniteVolumeMesh3D& mesh,
                                  const MeshAuditReport& report) const
{
    output << std::boolalpha;
    output << "[mesh_3d]\n";
    output << "valid: " << report.valid() << '\n';
    output << "nodes: " << mesh.node_count() << '\n';
    output << "cells: " << mesh.cell_count() << '\n';
    output << "faces: " << mesh.face_count() << '\n';
    output << "patches: " << mesh.patch_count() << '\n';
    output << "issues: " << report.issue_count() << "\n\n";

    output << "[cells]\n";
    output << "id centre_x centre_y centre_z volume face_count\n";
    output << std::setprecision(17);
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        output << cell << ' ' << mesh.cells.centre_x[cell] << ' ' << mesh.cells.centre_y[cell] << ' '
               << mesh.cells.centre_z[cell] << ' ' << mesh.cells.volume[cell] << ' '
               << mesh.cells.face_count[cell] << '\n';
    }
    output << '\n';

    output << "[faces]\n";
    output << "id owner neighbour centre_x centre_y centre_z area patch role\n";
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        output << face << ' ' << mesh.faces.owner[face].value << ' ' << mesh.faces.neighbour[face].value << ' '
               << mesh.faces.centre_x[face] << ' ' << mesh.faces.centre_y[face] << ' '
               << mesh.faces.centre_z[face] << ' ' << mesh.faces.area[face] << ' '
               << mesh.faces.patch_id[face].value << ' ' << mesh.faces.role[face].name << '\n';
    }
    output << '\n';

    output << "[issues]\n";
    output << "severity code message\n";
    for(const auto& issue : report.issues) {
        output << issue.severity.name << ' ' << issue.code << ' ' << issue.message << '\n';
    }
}

void MeshTextAuditWriter3D::write(const std::filesystem::path& file_path,
                                  const vmm::mesh::FiniteVolumeMesh3D& mesh,
                                  const MeshAuditReport& report) const
{
    std::filesystem::create_directories(file_path.parent_path());
    std::ofstream output{file_path};
    if(!output) {
        vmm::error::throw_file_not_found("write_mesh_text_audit_3d", file_path.string());
    }
    write(output, mesh, report);
}

void write_mesh_text_audit_3d(std::ostream& output,
                              const vmm::mesh::FiniteVolumeMesh3D& mesh,
                              const MeshAuditReport& report)
{
    MeshTextAuditWriter3D{}.write(output, mesh, report);
}

void write_mesh_text_audit_3d(const std::filesystem::path& file_path,
                              const vmm::mesh::FiniteVolumeMesh3D& mesh,
                              const MeshAuditReport& report)
{
    MeshTextAuditWriter3D{}.write(file_path, mesh, report);
}

void write_mesh_text_audit_3d(std::ostream& output, const vmm::mesh::FiniteVolumeMesh3D& mesh)
{
    write_mesh_text_audit_3d(output, mesh, audit_extruded_mesh_3d(mesh));
}

void write_mesh_text_audit_3d(const std::filesystem::path& file_path, const vmm::mesh::FiniteVolumeMesh3D& mesh)
{
    write_mesh_text_audit_3d(file_path, mesh, audit_extruded_mesh_3d(mesh));
}

} // namespace vmm::audit
