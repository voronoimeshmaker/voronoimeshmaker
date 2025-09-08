//------------------------------------------------------------------------------
// Name        : constants.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.2
// Description : Numerical constants (thresholds and defaults) for the core of
//               VoronoiMeshMaker. Single-responsibility: no state, no logic.
// License     : GNU GPL v3
//------------------------------------------------------------------------------

#pragma once

/**
 * @file constants.h
 * @brief Numerical constants used across the library.
 *
 * This header defines numerical thresholds and small defaults to be used by
 * geometry and mesh modules. It intentionally contains no logic/state.
 *
 * @ingroup core
 */

//------------------------------------------------------------------------------
//  C++ includes
//------------------------------------------------------------------------------

#include <limits>   // std::numeric_limits
#include <numbers>  // std::numbers::pi_v

//------------------------------------------------------------------------------
//  VoronoiMeshMaker includes
//------------------------------------------------------------------------------

#include <VoronoiMeshMaker/Core/namespace.h>            // For namespaces
#include <VoronoiMeshMaker/Core/type.h>                 // for Real

VORMAKER_NAMESPACE_OPEN

/**
 * @namespace vmm::constants
 * @brief Internal namespace for numerical constants.
 *
 * Keeping constants in a dedicated namespace avoids polluting the main API
 * while preserving simple names for frequent use.
 */
namespace constants {

//------------------------------------------------------------------------------
// Typed constants (prefer these in new code)
//------------------------------------------------------------------------------

/**
 * @brief Numerical zero tolerance (very small threshold).
 *
 * Reasoning: 1e250 * min() keeps the scale tied to the underlying floating
 * type. For IEEE double, this is ~1e-58. Adjust if your kernels change.
 */
inline constexpr Real kZeroTol = static_cast<Real>(1e-12);

/**
 * @brief Small limit threshold for conservative comparisons.
 * Example: clamp small negative areas/volumes to zero.
 */
inline constexpr Real kLimit = static_cast<Real>(1e-30);

/**
 * @brief Epsilon for equality checks in non-critical geometric predicates.
 * Use tighter/looser values where warranted by the algorithm.
 */
inline constexpr Real kEpsilon = static_cast<Real>(1e-6);

/**
 * @brief Default small size for containers/buffers used in utilities.
 */
inline constexpr int kDefaultSize = 0x50;

/**
 * @brief Compile-time constant for π (pi).
 */
inline constexpr Real kPi =
    static_cast<Real>(std::numbers::pi_v<long double>);

}  // namespace constants

//------------------------------------------------------------------------------
// Backwards-compatible aliases (kept to avoid breaking existing code)
// Prefer vmm::constants::k* in new code.
//------------------------------------------------------------------------------

inline constexpr Real ZERO    = constants::kZeroTol;
inline constexpr Real LIMIT   = constants::kLimit;
inline constexpr Real EPSILON = constants::kEpsilon;
inline constexpr int  LSIZE   = constants::kDefaultSize;

/// Pi as a constant (prefer this form in compile-time contexts).
inline constexpr Real PI = constants::kPi;

//------------------------------------------------------------------------------
// Sanity checks (compile-time): ensure Real behaves as a floating type.
//------------------------------------------------------------------------------
static_assert   (   std::numeric_limits<Real>::is_iec559 ||
                    std::numeric_limits<Real>::is_specialized,
                    "Real must be a floating point-like type."
                );

VORMAKER_NAMESPACE_CLOSE
