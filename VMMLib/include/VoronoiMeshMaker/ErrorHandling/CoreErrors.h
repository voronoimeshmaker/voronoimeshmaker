// ============================================================================
// File:CoreErrors.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.1 (Refactored)
// Description: Example core error enum + ErrorTraits specialization.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <array>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>

/**
 * @fileCoreErrors.h
 * @brief Core error examples and their traits (extensible pattern).
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/** @brief Core error domain (example). */
enum class CoreErr : std::uint16_t {
    InvalidArgument = 1,
    OutOfRange      = 2,
    NotImplemented  = 3,
    AssertFailed    = 4,

    _Count // Helper for array size check
};

namespace detail {
    struct ErrorInfo {
        std::string_view key;
        Severity severity;
        std::string_view enUS;
        std::string_view ptBR;
    };

    // Using a static constexpr array for storing error metadata.
    // Index 0 is unused as enums start from 1.
    constexpr std::array<ErrorInfo, static_cast<size_t>(CoreErr::_Count)>
        core_err_data = {{{}, // Index 0
                          {"CORE_INVALID_ARG", Severity::Error,
                           "Invalid argument: {name}.",
                           "Argumento inválido: {name}."},
                          {"CORE_OUT_OF_RANGE", Severity::Error,
                           "Index out of range: {index}.",
                           "Índice fora do intervalo: {index}."},
                          {"CORE_NOT_IMPLEMENTED", Severity::Warning,
                           "Feature not implemented.",
                           "Recurso não implementado."},
                          {"CORE_ASSERT_FAILED", Severity::Fatal,
                           "Assertion failed.", "Falha de asserção."}}};

    constexpr const ErrorInfo& get_info(CoreErr e) noexcept {
        return core_err_data[static_cast<size_t>(e)];
    }
}  // namespace detail

/** @brief Traits specialization for CoreErr. */
template <>
struct ErrorTraits<CoreErr> {
    static constexpr std::uint16_t domain_id() noexcept { return 0x0001; }
    static constexpr std::string_view domain_name() noexcept { return "Core"; }

    static constexpr Severity default_severity(CoreErr e) noexcept {
        return detail::get_info(e).severity;
    }
    static constexpr std::string_view key(CoreErr e) noexcept {
        return detail::get_info(e).key;
    }
    static constexpr std::string_view enUS(CoreErr e) noexcept {
        return detail::get_info(e).enUS;
    }
    static constexpr std::string_view ptBR(CoreErr e) noexcept {
        return detail::get_info(e).ptBR;
    }
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE