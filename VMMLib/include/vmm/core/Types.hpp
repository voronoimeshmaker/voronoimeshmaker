//==============================================================================
// File        : Types.hpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Core type definitions, dimensional traits, and fundamental
//               POD structures for the Voronoi mesh topology system.
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.
//
// @file Types.hpp
// @brief Core type definitions and dimensional traits for mesh topology.
// @ingroup vmm_core
//==============================================================================
#pragma once

//==============================================================================
// C++ standard library includes
//==============================================================================
#include <cstdint>
// #include <cstddef>
#include <string_view>
#include <type_traits>

namespace vmm::core {

//==============================================================================
// Fundamental scalar type alias
//==============================================================================
/**
 * @brief Primary scalar type for all geometric and topological computations.
 * @ingroup vmm_core
 *
 * Centralised alias to enable future migration to float (performance-critical
 * paths) or long double (high-precision validation) without widespread source
 * changes. Also facilitates template instantiation for automatic differentiation
 * or arbitrary-precision arithmetic in research configurations.
 */
using Real = double;

//==============================================================================
// Dimensional configuration
//==============================================================================
/**
 * @brief Dimensional configuration for mesh generation and analysis.
 * @ingroup vmm_core
 */
enum class Dimension : uint8_t { D2 = 2, D3 = 3 };

//==============================================================================
// Identifier and index types
//==============================================================================
/** @brief Sequential generator identifier. Guaranteed unique and monotonic per generation pass. */
using GeneratorID = uint64_t;

/** @brief Index type for addressing cells within a topology container. */
using CellIndex   = uint32_t;

/** @brief Index type for addressing faces/edges within a topology container. */
using FaceIndex   = uint32_t;

//==============================================================================
// Boundary classification
//==============================================================================
/**
 * @brief Boundary classification for cells and faces.
 * @ingroup vmm_core
 */
struct BoundaryType final {
    std::string_view name;
    std::uint8_t code;

    [[nodiscard]] friend constexpr bool operator==(BoundaryType lhs, BoundaryType rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }

    [[nodiscard]] friend constexpr bool operator!=(BoundaryType lhs, BoundaryType rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

struct BoundaryTypeTraits final {
    static constexpr BoundaryType Internal{"internal", 0U};
    static constexpr BoundaryType ExternalBoundary{"external-boundary", 1U};
    static constexpr BoundaryType HoleBoundary{"hole-boundary", 2U};
};

//==============================================================================
// Dimensional point structures
//==============================================================================
/**
 * @brief Primary dimensional point structure.
 * @tparam Dim Dimensional configuration (D2 or D3).
 * @ingroup vmm_core
 *
 * Provides a cache-friendly, trivially copyable coordinate container.
 * Specialised for 2D and 3D to eliminate unused members and ensure
 * optimal alignment for SIMD operations.
 */
template<Dimension Dim>
struct Point;

/** @brief 2D Cartesian point. */
template<>
struct Point<Dimension::D2> {
    Real x{};
    Real y{};
    constexpr Point() noexcept = default;
    constexpr Point(Real x_, Real y_) noexcept : x(x_), y(y_) {}
};

/** @brief 3D Cartesian point. */
template<>
struct Point<Dimension::D3> {
    Real x{};
    Real y{};
    Real z{};
    constexpr Point() noexcept = default;
    constexpr Point(Real x_, Real y_, Real z_) noexcept : x(x_), y(y_), z(z_) {}
};

//==============================================================================
// Compile-time layout guarantees (DOD/SIMD compliance)
//==============================================================================
static_assert(std::is_standard_layout_v<Point<Dimension::D2>>, "D2 Point must have standard layout for DOD/SIMD");
static_assert(std::is_standard_layout_v<Point<Dimension::D3>>, "D3 Point must have standard layout for DOD/SIMD");
static_assert(std::is_trivially_copyable_v<Point<Dimension::D2>>, "D2 Point must be trivially copyable for memcpy/DMA");
static_assert(std::is_trivially_copyable_v<Point<Dimension::D3>>, "D3 Point must be trivially copyable for memcpy/DMA");

} // namespace vmm::core