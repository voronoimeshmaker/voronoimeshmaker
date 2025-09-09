// ============================================================================
// File: ErrorTraits.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Traits and helpers to describe error enums (extensible).
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <cstdint>
#include <string_view>
// #include <type_traits>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file ErrorTraits.h
 * @brief Extensible traits for user-defined error enums.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/**
 * @brief Primary template for error traits. Specialize for your enums.
 *
 * Required static members for a specialization T:
 *  - static constexpr uint16_t domain_id() noexcept;
 *  - static constexpr std::string_view domain_name() noexcept;
 *  - static constexpr std::string_view key(T) noexcept;
 *  - static constexpr std::string_view enUS(T) noexcept;
 *  - static constexpr std::string_view ptBR(T) noexcept;
 *  - static constexpr Severity default_severity(T) noexcept;
 */
template <class T>
struct ErrorTraits;  // no definition: using it without specialization is an error

/** @brief Compose a 32-bit code from domain (hi) and value (lo). */
constexpr std::uint32_t make_code(std::uint16_t domain,
                                  std::uint16_t value) noexcept {
    return (static_cast<std::uint32_t>(domain) << 16) |
           static_cast<std::uint32_t>(value);
}

/** @brief C++20 concept for enums supported by ErrorTraits. */
template <class T>
concept ErrorEnum = std::is_enum_v<T> &&
    requires(T e) {
        { ErrorTraits<T>::domain_id() } noexcept -> std::same_as<uint16_t>;
        { ErrorTraits<T>::domain_name() } noexcept;
        { ErrorTraits<T>::key(e) } noexcept;
        { ErrorTraits<T>::enUS(e) } noexcept;
        { ErrorTraits<T>::ptBR(e) } noexcept;
        { ErrorTraits<T>::default_severity(e) } noexcept;
    };

/** @brief Return the composed 32-bit error code for an enum value. */
template <ErrorEnum E>
constexpr std::uint32_t error_code(E e) noexcept {
    using U = std::underlying_type_t<E>;
    return make_code(ErrorTraits<E>::domain_id(),
                     static_cast<std::uint16_t>(static_cast<U>(e)));
}

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
