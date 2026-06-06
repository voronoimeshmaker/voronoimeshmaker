//==============================================================================
// File        : MeshTextAuditWriter2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of stable 2D FV mesh text audit output.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <string>
#include <string_view>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshTextAuditWriter2D.hpp>
#include <vmm/core/StrongIds.hpp>
#include <vmm/error/MeshException.hpp>

namespace vmm::audit {
namespace {

template<class Tag>
void write_id(std::ostream& output, vmm::core::StrongId<Tag> id)
{
    if(vmm::core::is_valid(id)) {
        output << id.value;
    } else {
        output << "invalid";
    }
}

void write_issue_id(std::ostream& output, auto id)
{
    if(vmm::core::is_valid(id)) {
        output << id.value;
    } else {
        output << "-";
    }
}

void write_header(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh, const MeshAuditReport& report)
{
    output << "VMM Mesh Audit 2D\n";
    output << "version: 1\n";
    output << "valid: " << (report.valid() ? "true" : "false") << "\n\n";

    output << "[counts]\n";
    output << "nodes: " << mesh.node_count() << "\n";
    output << "cells: " << mesh.cell_count() << "\n";
    output << "faces: " << mesh.face_count() << "\n";
    output << "patches: " << mesh.patch_count() << "\n";
    output << "cell_face_entries: " << mesh.cell_faces.size() << "\n\n";
}

void write_issues(std::ostream& output, const MeshAuditReport& report)
{
    output << "[issues]\n";
    output << "severity code cell face node site patch message\n";
    for(const auto& issue : report.all_issues()) {
        output << issue.severity.name << ' ' << issue.code << ' ';
        write_issue_id(output, issue.cell_id);
        output << ' ';
        write_issue_id(output, issue.face_id);
        output << ' ';
        write_issue_id(output, issue.node_id);
        output << ' ';
        write_issue_id(output, issue.site_id);
        output << ' ';
        write_issue_id(output, issue.patch_id);
        output << ' ' << issue.message << "\n";
    }
    output << "\n";
}

void write_nodes(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    output << "[nodes]\n";
    output << "id x y\n";
    for(std::size_t node = 0U; node < mesh.node_count(); ++node) {
        output << node << ' ' << mesh.nodes.x[node] << ' ' << mesh.nodes.y[node] << "\n";
    }
    output << "\n";
}

void write_cells(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    output << "[cells]\n";
    output << "id centre_x centre_y area site_id face_offset face_count\n";
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        output << cell << ' ' << mesh.cells.centre_x[cell] << ' ' << mesh.cells.centre_y[cell] << ' '
               << mesh.cells.area[cell] << ' ';
        write_id(output, mesh.cells.site_id[cell]);
        output << ' ' << mesh.cells.face_offset[cell] << ' ' << mesh.cells.face_count[cell] << "\n";
    }
    output << "\n";
}

void write_faces(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    output << "[faces]\n";
    output << "id owner neighbour centre_x centre_y normal_x normal_y length patch_id node0 node1\n";
    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        output << face << ' ';
        write_id(output, mesh.faces.owner[face]);
        output << ' ';
        write_id(output, mesh.faces.neighbour[face]);
        output << ' ' << mesh.faces.centre_x[face] << ' ' << mesh.faces.centre_y[face] << ' '
               << mesh.faces.normal_x[face] << ' ' << mesh.faces.normal_y[face] << ' ' << mesh.faces.length[face]
               << ' ';
        write_id(output, mesh.faces.patch_id[face]);
        output << ' ';
        write_id(output, mesh.faces.node0[face]);
        output << ' ';
        write_id(output, mesh.faces.node1[face]);
        output << "\n";
    }
    output << "\n";
}

void write_patches(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    output << "[patches]\n";
    output << "id name type code\n";
    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        write_id(output, mesh.patches.id[patch]);
        output << ' ' << mesh.patches.name[patch] << ' ' << mesh.patches.type[patch].name << ' '
               << mesh.patches.type[patch].code << "\n";
    }
    output << "\n";
}

void write_cell_faces(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    output << "[cell_faces]\n";
    output << "cell_id local_index face_id\n";
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        const auto offset = static_cast<std::size_t>(mesh.cells.face_offset[cell]);
        const auto count = static_cast<std::size_t>(mesh.cells.face_count[cell]);
        if(offset > mesh.cell_faces.size() || count > mesh.cell_faces.size() - offset) {
            output << cell << " invalid invalid\n";
            continue;
        }
        for(std::size_t local = 0U; local < count; ++local) {
            output << cell << ' ' << local << ' ';
            write_id(output, mesh.cell_faces.face_ids[offset + local]);
            output << "\n";
        }
    }
}

void open_output_file_or_throw(std::ofstream& output, const std::filesystem::path& file_path, std::string_view context)
{
    const auto parent = file_path.parent_path();
    if(!parent.empty()) {
        std::filesystem::create_directories(parent);
    }
    output.open(file_path);
    if(!output) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }
}

} // namespace

void MeshTextAuditWriter2D::write(std::ostream& output,
                                  const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                  const MeshAuditReport& report) const
{
    output << std::setprecision(17);
    write_header(output, mesh, report);
    write_issues(output, report);
    write_nodes(output, mesh);
    write_cells(output, mesh);
    write_faces(output, mesh);
    write_patches(output, mesh);
    write_cell_faces(output, mesh);
}

void MeshTextAuditWriter2D::write(const std::filesystem::path& file_path,
                                  const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                  const MeshAuditReport& report) const
{
    std::ofstream output;
    open_output_file_or_throw(output, file_path, "MeshTextAuditWriter2D::write");
    write(output, mesh, report);
}

void write_mesh_text_audit_2d(std::ostream& output,
                              const vmm::mesh::FiniteVolumeMesh2D& mesh,
                              const MeshAuditReport& report)
{
    MeshTextAuditWriter2D{}.write(output, mesh, report);
}

void write_mesh_text_audit_2d(const std::filesystem::path& file_path,
                              const vmm::mesh::FiniteVolumeMesh2D& mesh,
                              const MeshAuditReport& report)
{
    MeshTextAuditWriter2D{}.write(file_path, mesh, report);
}

void write_mesh_text_audit_2d(std::ostream& output, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    write_mesh_text_audit_2d(output, mesh, audit_mesh_connectivity_geometry_2d(mesh));
}

void write_mesh_text_audit_2d(const std::filesystem::path& file_path, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    write_mesh_text_audit_2d(file_path, mesh, audit_mesh_connectivity_geometry_2d(mesh));
}

} // namespace vmm::audit
