//==============================================================================
// File        : FaceTable2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented 2D finite-volume face table.
//==============================================================================
#pragma once

#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace vmm::mesh {

struct FaceTable2D final {
    std::vector<vmm::core::CellId> owner;
    std::vector<vmm::core::CellId> neighbour;
    std::vector<vmm::core::Real> centre_x;
    std::vector<vmm::core::Real> centre_y;
    std::vector<vmm::core::Real> normal_x;
    std::vector<vmm::core::Real> normal_y;
    std::vector<vmm::core::Real> length;
    std::vector<vmm::core::BoundaryPatchId> patch_id;
    std::vector<vmm::core::NodeId> node0;
    std::vector<vmm::core::NodeId> node1;

    [[nodiscard]] std::size_t size() const noexcept { return owner.size(); }
    [[nodiscard]] bool empty() const noexcept { return owner.empty(); }

    [[nodiscard]] bool contains(vmm::core::FaceId face_id) const noexcept
    {
        return vmm::core::is_valid(face_id) && face_id.value < owner.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept
    {
        return owner.size() == neighbour.size() && owner.size() == centre_x.size() &&
               owner.size() == centre_y.size() && owner.size() == normal_x.size() &&
               owner.size() == normal_y.size() && owner.size() == length.size() &&
               owner.size() == patch_id.size() && owner.size() == node0.size() &&
               owner.size() == node1.size();
    }

    [[nodiscard]] std::span<const vmm::core::CellId> owners() const noexcept { return owner; }
    [[nodiscard]] std::span<const vmm::core::CellId> neighbours() const noexcept { return neighbour; }
    [[nodiscard]] std::span<const vmm::core::Real> centre_xs() const noexcept { return centre_x; }
    [[nodiscard]] std::span<const vmm::core::Real> centre_ys() const noexcept { return centre_y; }
    [[nodiscard]] std::span<const vmm::core::Real> normal_xs() const noexcept { return normal_x; }
    [[nodiscard]] std::span<const vmm::core::Real> normal_ys() const noexcept { return normal_y; }
    [[nodiscard]] std::span<const vmm::core::Real> lengths() const noexcept { return length; }
    [[nodiscard]] std::span<const vmm::core::BoundaryPatchId> patch_ids() const noexcept { return patch_id; }
    [[nodiscard]] std::span<const vmm::core::NodeId> first_nodes() const noexcept { return node0; }
    [[nodiscard]] std::span<const vmm::core::NodeId> second_nodes() const noexcept { return node1; }
};

} // namespace vmm::mesh
