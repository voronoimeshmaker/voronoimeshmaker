//==============================================================================
// File        : BoundaryPatchTable.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented boundary patch table for finite-volume meshes.
//==============================================================================
#pragma once

#include <vmm/core/StrongIds.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace vmm::mesh {

enum class BoundaryPatchType : std::uint8_t {
    internal = 0,
    wall,
    island,
    inlet,
    outlet,
    open_boundary,
    land_boundary,
    no_flux,
    bottom,
    surface
};

struct BoundaryPatchTable final {
    std::vector<vmm::core::BoundaryPatchId> id;
    std::vector<std::string> name;
    std::vector<BoundaryPatchType> type;

    [[nodiscard]] std::size_t size() const noexcept { return id.size(); }
    [[nodiscard]] bool empty() const noexcept { return id.empty(); }

    [[nodiscard]] bool contains(vmm::core::BoundaryPatchId patch_id) const noexcept
    {
        return vmm::core::is_valid(patch_id) && patch_id.value < id.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept { return id.size() == name.size() && id.size() == type.size(); }

    [[nodiscard]] std::span<const vmm::core::BoundaryPatchId> ids() const noexcept { return id; }
    [[nodiscard]] std::span<const std::string> names() const noexcept { return name; }
    [[nodiscard]] std::span<const BoundaryPatchType> types() const noexcept { return type; }
};

} // namespace vmm::mesh
