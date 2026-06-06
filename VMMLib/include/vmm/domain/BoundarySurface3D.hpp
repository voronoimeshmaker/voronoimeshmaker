//==============================================================================
// File        : BoundarySurface3D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Open boundary-surface traits for 3D domains.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstdint>
#include <string_view>

namespace vmm::domain {

struct BoundarySurfaceType final {
    std::string_view name;
    std::uint8_t code;

    [[nodiscard]] friend constexpr bool operator==(BoundarySurfaceType lhs,
                                                   BoundarySurfaceType rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }
};

struct BoundarySurfaceTypeTraits final {
    static constexpr BoundarySurfaceType Wall{"wall", 0U};
    static constexpr BoundarySurfaceType LandBoundary{"land-boundary", 1U};
    static constexpr BoundarySurfaceType OpenBoundary{"open-boundary", 2U};
    static constexpr BoundarySurfaceType Bottom{"bottom", 3U};
    static constexpr BoundarySurfaceType Surface{"surface", 4U};
};

} // namespace vmm::domain
