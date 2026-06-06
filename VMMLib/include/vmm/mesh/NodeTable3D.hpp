//==============================================================================
// File        : NodeTable3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Data-oriented 3D node coordinate table.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <span>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>

namespace vmm::mesh {

struct NodeTable3D final {
    std::vector<vmm::core::Real> x;
    std::vector<vmm::core::Real> y;
    std::vector<vmm::core::Real> z;

    [[nodiscard]] std::size_t size() const noexcept { return x.size(); }
    [[nodiscard]] bool empty() const noexcept { return x.empty(); }

    [[nodiscard]] bool contains(vmm::core::NodeId node_id) const noexcept
    {
        return vmm::core::is_valid(node_id) && node_id.value < x.size();
    }

    [[nodiscard]] bool has_consistent_storage() const noexcept
    {
        return x.size() == y.size() && x.size() == z.size();
    }

    [[nodiscard]] std::span<const vmm::core::Real> xs() const noexcept { return x; }
    [[nodiscard]] std::span<const vmm::core::Real> ys() const noexcept { return y; }
    [[nodiscard]] std::span<const vmm::core::Real> zs() const noexcept { return z; }
};

} // namespace vmm::mesh
