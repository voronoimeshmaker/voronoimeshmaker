//==============================================================================
// File        : NodeTable2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented 2D node coordinate table.
//==============================================================================
#pragma once

#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace vmm::mesh {

struct NodeTable2D final {
    std::vector<vmm::core::Real> x;
    std::vector<vmm::core::Real> y;

    [[nodiscard]] std::size_t size() const noexcept { return x.size(); }
    [[nodiscard]] bool empty() const noexcept { return x.empty(); }

    [[nodiscard]] bool contains(vmm::core::NodeId node_id) const noexcept
    {
        return vmm::core::is_valid(node_id) && node_id.value < x.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept { return x.size() == y.size(); }

    [[nodiscard]] std::span<const vmm::core::Real> xs() const noexcept { return x; }
    [[nodiscard]] std::span<const vmm::core::Real> ys() const noexcept { return y; }
};

} // namespace vmm::mesh
