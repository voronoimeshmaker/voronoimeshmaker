#pragma once
//==============================================================================
// Name        : Boundary2DTypes.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 0.2.0
// Description : Fast, POD-first basic types for Boundary2D (2D point, ids,
//               enums and small helpers). Designed for cache-friendliness and
//               data-parallel processing.
// License     : GNU GPL v3
//==============================================================================

/**
 * @file Boundary2DTypes.hpp
 * @brief Core value types for the Boundary2D module.
 *
 * Design goals (performance-oriented):
 *  - POD, trivially copyable, standard layout types.
 *  - Small integer ids (32-bit) and compact enums (8-bit).
 *  - Minimal dependencies; suitable for SoA/CSR layouts and SIMD-friendly
 *    batch operations.
 *  - Keep *logic* elsewhere (validators/transform/builder); this header only
 *    declares lightweight types and constants.
 */

//------------------------------------------------------------------------------
//      VoronoiMeshMaker includes
//------------------------------------------------------------------------------
#include <VoronoiMeshMaker/Core/constants.h>   // sanity checks on Real

VORMAKER_NAMESPACE_OPEN
BOUNDARY2D_NAMESPACE_OPEN 

//------------------------------------------------------------------------------
// Numeric & index aliases (compact, cache-friendly)
//------------------------------------------------------------------------------

/**
 * @brief Real used by Boundary2D. Delegates to the project-wide Real.
 *        Keep this alias here for a single include point across the module.
 */
using Real = ::vmm::Real;

/** @brief Signed index for CSR offsets, vertex indices, etc. */
using Index  = std::int32_t;

/** @brief Unsigned companion index (useful for sizes/bit ops). */
using UIndex = std::uint32_t;

/** @brief Sentinel for invalid indices. */
inline constexpr Index kInvalid = static_cast<Index>(-1);

//------------------------------------------------------------------------------
// Strong IDs (typed integers)
//------------------------------------------------------------------------------

/** @brief Region identifier for non-homogeneous domains. */
struct RegionId {
    Index value{0};
    constexpr RegionId() = default;
    constexpr explicit RegionId(Index v) : value(v) {}
    [[nodiscard]] constexpr explicit operator Index() const noexcept {
        return value;
    }

    friend constexpr bool operator==(const RegionId& _id1, const RegionId& _id2) noexcept {
        return _id1.value == _id2.value;  
    }

    friend constexpr bool operator!=(const RegionId& _id1, const RegionId& _id2) noexcept {
        return _id1.value != _id2.value;  
    }
};

/** @brief Optional tag identifier (edge/ring/patch labelling). */
struct TagId {
    Index value{0};
    constexpr TagId() = default;
    constexpr explicit TagId(Index v) : value(v) {}
    [[nodiscard]] constexpr explicit operator Index() const noexcept {
        return value;
    }

    friend constexpr bool operator==(const TagId& _id1, const TagId& _id2) noexcept {
        return _id1.value == _id2.value;
    }

    friend constexpr bool operator!=(const TagId& _id1, const TagId& _id2) noexcept {
        return _id1.value != _id2.value;
    }
};

//------------------------------------------------------------------------------
// Small enums (compact storage)
//------------------------------------------------------------------------------

/** @brief Loop kind for polygon-with-holes representation. */
enum class LoopKind : std::uint8_t {
    Outer = 0,  ///< Exterior ring (expected CCW)
    Hole  = 1   ///< Interior ring (expected CW)
};

//------------------------------------------------------------------------------
// Geometry PODs
//------------------------------------------------------------------------------

/**
 * @brief 2D point. Plain old data, aligned to 16 bytes to help vector loads.
 *        Keep only data members; math lives in Transform/Algorithms headers.
 */
struct alignas(16) Point2 {
    Real x{0};
    Real y{0};
};
// Trivial & layout guarantees (important for SoA/CSR and bulk memcpy).
static_assert(std::is_trivially_copyable_v<Point2>, "Point2 must be POD.");
static_assert(std::is_standard_layout_v<Point2>,     "Point2 std layout.");

/**
 * @brief Axis-aligned bounding box.
 *        Invariants: min.x <= max.x and min.y <= max.y when non-empty.
 */
struct Box2 {
    Point2 min{+std::numeric_limits<Real>::infinity(),
               +std::numeric_limits<Real>::infinity()};
    Point2 max{-std::numeric_limits<Real>::infinity(),
               -std::numeric_limits<Real>::infinity()};

    [[nodiscard]] static constexpr Box2 empty() noexcept { return {}; }

    /** @brief Expand to include a point (no branching on NAN/INF here). */
    constexpr void expand(Point2 p) noexcept {
        if (p.x < min.x) min.x = p.x;
        if (p.y < min.y) min.y = p.y;
        if (p.x > max.x) max.x = p.x;
        if (p.y > max.y) max.y = p.y;
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return !(min.x <= max.x && min.y <= max.y);
    }
};

//------------------------------------------------------------------------------
// Lightweight affine transform container (2x3). Math lives elsewhere.
//------------------------------------------------------------------------------

/**
 * @brief Row-major 2x3 affine transform:
 *        [ a00 a01 tx ]
 *        [ a10 a11 ty ]
 *
 * Provided here as a POD container to avoid circular includes; algorithms
 * that *use* it (compose/apply/invert) live in Boundary2DTransform.
 */
struct Affine2 {
    Real a00{1}, a01{0}, tx{0};
    Real a10{0}, a11{1}, ty{0};
};
static_assert(std::is_trivially_copyable_v<Affine2>, "Affine2 must be POD.");
static_assert(std::is_standard_layout_v<Affine2>,     "Affine2 std layout.");

//------------------------------------------------------------------------------
// Compile-time sanity checks
//------------------------------------------------------------------------------

static_assert(sizeof(LoopKind) == 1, "LoopKind must be byte-sized.");
static_assert(sizeof(Index) == 4, "Index must be 32-bit for CSR compactness.");

BOUNDARY2D_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
