//==============================================================================
// File        : FiniteVolumeMesh3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Canonical extruded 3D finite-volume mesh aggregate.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <span>
#include <stdexcept>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/mesh/BoundaryPatchTable.hpp>
#include <vmm/mesh/CellFaceConnectivity.hpp>
#include <vmm/mesh/CellTable3D.hpp>
#include <vmm/mesh/FaceTable3D.hpp>
#include <vmm/mesh/NodeTable3D.hpp>

namespace vmm::mesh {

struct FiniteVolumeMesh3D final {
    NodeTable3D nodes;
    CellTable3D cells;
    FaceTable3D faces;
    BoundaryPatchTable patches;
    CellFaceConnectivity cell_faces;
    FaceNodeConnectivity3D face_nodes;

    [[nodiscard]] std::size_t node_count() const noexcept { return nodes.size(); }
    [[nodiscard]] std::size_t cell_count() const noexcept { return cells.size(); }
    [[nodiscard]] std::size_t face_count() const noexcept { return faces.size(); }
    [[nodiscard]] std::size_t patch_count() const noexcept { return patches.size(); }

    [[nodiscard]] bool contains(vmm::core::NodeId node_id) const noexcept { return nodes.contains(node_id); }
    [[nodiscard]] bool contains(vmm::core::CellId cell_id) const noexcept { return cells.contains(cell_id); }
    [[nodiscard]] bool contains(vmm::core::FaceId face_id) const noexcept { return faces.contains(face_id); }
    [[nodiscard]] bool contains(vmm::core::BoundaryPatchId patch_id) const noexcept { return patches.contains(patch_id); }

    [[nodiscard]] bool has_consistent_storage() const noexcept
    {
        return nodes.has_consistent_storage() && cells.has_consistent_storage() &&
               faces.has_consistent_storage() && patches.has_consistent_storage();
    }

    [[nodiscard]] std::span<const vmm::core::FaceId> faces_of(vmm::core::CellId cell_id) const
    {
        if(!contains(cell_id)) {
            throw std::out_of_range("Cell id exceeds finite-volume 3D mesh bounds");
        }
        const auto cell = static_cast<std::size_t>(cell_id.value);
        return cell_faces.cell_faces(cells.face_offset[cell], cells.face_count[cell]);
    }

    [[nodiscard]] std::span<const vmm::core::NodeId> nodes_of(vmm::core::FaceId face_id) const
    {
        if(!contains(face_id)) {
            throw std::out_of_range("Face id exceeds finite-volume 3D mesh bounds");
        }
        const auto face = static_cast<std::size_t>(face_id.value);
        return face_nodes.face_nodes(faces.node_offset[face], faces.node_count[face]);
    }
};

} // namespace vmm::mesh
