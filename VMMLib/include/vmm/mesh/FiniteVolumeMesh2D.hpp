//==============================================================================
// File        : FiniteVolumeMesh2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Canonical face-based 2D finite-volume mesh aggregate.
//==============================================================================
#pragma once

#include <vmm/mesh/BoundaryPatchTable.hpp>
#include <vmm/mesh/CellFaceConnectivity.hpp>
#include <vmm/mesh/CellTable2D.hpp>
#include <vmm/mesh/FaceTable2D.hpp>
#include <vmm/mesh/NodeTable2D.hpp>

#include <cstddef>
#include <span>
#include <stdexcept>

namespace vmm::mesh {

struct FiniteVolumeMesh2D final {
    NodeTable2D nodes;
    CellTable2D cells;
    FaceTable2D faces;
    BoundaryPatchTable patches;
    CellFaceConnectivity cell_faces;

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
        return nodes.has_consistent_storage() && cells.has_consistent_storage() && faces.has_consistent_storage() &&
               patches.has_consistent_storage();
    }

    [[nodiscard]] std::span<const vmm::core::FaceId> faces_of(vmm::core::CellId cell_id) const
    {
        if(!contains(cell_id)) {
            throw std::out_of_range("Cell id exceeds finite-volume mesh bounds");
        }
        const auto cell = static_cast<std::size_t>(cell_id.value);
        return cell_faces.cell_faces(cells.face_offset[cell], cells.face_count[cell]);
    }
};

} // namespace vmm::mesh
