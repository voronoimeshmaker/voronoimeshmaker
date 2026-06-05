//==============================================================================
// File        : CellTable2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented 2D finite-volume cell table.
//==============================================================================
#pragma once

#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace vmm::mesh {

struct CellTable2D final {
    std::vector<vmm::core::Real> centre_x;
    std::vector<vmm::core::Real> centre_y;
    std::vector<vmm::core::Real> area;
    std::vector<vmm::core::SiteId> site_id;
    std::vector<std::uint64_t> face_offset;
    std::vector<std::uint32_t> face_count;

    [[nodiscard]] std::size_t size() const noexcept { return centre_x.size(); }
    [[nodiscard]] bool empty() const noexcept { return centre_x.empty(); }

    [[nodiscard]] bool contains(vmm::core::CellId cell_id) const noexcept
    {
        return vmm::core::is_valid(cell_id) && cell_id.value < centre_x.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept
    {
        return centre_x.size() == centre_y.size() && centre_x.size() == area.size() &&
               centre_x.size() == site_id.size() && centre_x.size() == face_offset.size() &&
               centre_x.size() == face_count.size();
    }

    [[nodiscard]] std::span<const vmm::core::Real> centre_xs() const noexcept { return centre_x; }
    [[nodiscard]] std::span<const vmm::core::Real> centre_ys() const noexcept { return centre_y; }
    [[nodiscard]] std::span<const vmm::core::Real> areas() const noexcept { return area; }
    [[nodiscard]] std::span<const vmm::core::SiteId> site_ids() const noexcept { return site_id; }
    [[nodiscard]] std::span<const std::uint64_t> face_offsets() const noexcept { return face_offset; }
    [[nodiscard]] std::span<const std::uint32_t> face_counts() const noexcept { return face_count; }
};

} // namespace vmm::mesh
