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
#include <string_view>
#include <vector>

namespace vmm::mesh {

struct BoundaryPatchType final {
    std::string_view name;
    std::uint16_t code;

    [[nodiscard]] friend constexpr bool operator==(BoundaryPatchType lhs, BoundaryPatchType rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }

    [[nodiscard]] friend constexpr bool operator!=(BoundaryPatchType lhs, BoundaryPatchType rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

struct BoundaryPatchTypeTraits final {
    static constexpr BoundaryPatchType Internal{"internal", 0U};
    static constexpr BoundaryPatchType Wall{"wall", 1U};
    static constexpr BoundaryPatchType Island{"island", 2U};
    static constexpr BoundaryPatchType Inlet{"inlet", 3U};
    static constexpr BoundaryPatchType Outlet{"outlet", 4U};
    static constexpr BoundaryPatchType OpenBoundary{"open-boundary", 5U};
    static constexpr BoundaryPatchType LandBoundary{"land-boundary", 6U};
    static constexpr BoundaryPatchType NoFlux{"no-flux", 7U};
    static constexpr BoundaryPatchType Bottom{"bottom", 8U};
    static constexpr BoundaryPatchType Surface{"surface", 9U};
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
