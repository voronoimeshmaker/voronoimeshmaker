//==============================================================================
// File        : FaceTable3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented 3D finite-volume face table.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>

namespace vmm::mesh {

struct FaceRole3D final {
    std::string_view name;
    std::uint8_t code;

    [[nodiscard]] friend constexpr bool operator==(FaceRole3D lhs, FaceRole3D rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }
};

struct FaceRole3DTraits final {
    static constexpr FaceRole3D Lateral{"lateral", 0U};
    static constexpr FaceRole3D Bottom{"bottom", 1U};
    static constexpr FaceRole3D Top{"top", 2U};
    static constexpr FaceRole3D InternalHorizontal{"internal-horizontal", 3U};
};

struct FaceTable3D final {
    std::vector<vmm::core::CellId> owner;
    std::vector<vmm::core::CellId> neighbour;
    std::vector<vmm::core::Real> centre_x;
    std::vector<vmm::core::Real> centre_y;
    std::vector<vmm::core::Real> centre_z;
    std::vector<vmm::core::Real> normal_x;
    std::vector<vmm::core::Real> normal_y;
    std::vector<vmm::core::Real> normal_z;
    std::vector<vmm::core::Real> area;
    std::vector<vmm::core::BoundaryPatchId> patch_id;
    std::vector<std::uint64_t> node_offset;
    std::vector<std::uint32_t> node_count;
    std::vector<FaceRole3D> role;

    [[nodiscard]] std::size_t size() const noexcept { return owner.size(); }
    [[nodiscard]] bool empty() const noexcept { return owner.empty(); }

    [[nodiscard]] bool contains(vmm::core::FaceId face_id) const noexcept
    {
        return vmm::core::is_valid(face_id) && face_id.value < owner.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept
    {
        return owner.size() == neighbour.size() && owner.size() == centre_x.size() &&
               owner.size() == centre_y.size() && owner.size() == centre_z.size() &&
               owner.size() == normal_x.size() && owner.size() == normal_y.size() &&
               owner.size() == normal_z.size() && owner.size() == area.size() &&
               owner.size() == patch_id.size() && owner.size() == node_offset.size() &&
               owner.size() == node_count.size() && owner.size() == role.size();
    }
};

struct FaceNodeConnectivity3D final {
    std::vector<vmm::core::NodeId> node_ids;

    [[nodiscard]] std::size_t size() const noexcept { return node_ids.size(); }
    [[nodiscard]] bool empty() const noexcept { return node_ids.empty(); }
    [[nodiscard]] std::span<const vmm::core::NodeId> nodes() const noexcept { return node_ids; }

    [[nodiscard]] std::span<const vmm::core::NodeId> face_nodes(
        std::uint64_t offset,
        std::uint32_t count) const
    {
        const auto begin = static_cast<std::size_t>(offset);
        const auto n = static_cast<std::size_t>(count);
        if(begin > node_ids.size() || n > node_ids.size() - begin) {
            throw std::out_of_range("Face node span exceeds connectivity storage");
        }
        return std::span<const vmm::core::NodeId>{node_ids.data() + begin, n};
    }
};

} // namespace vmm::mesh
