// ============================================================================
// File: CoreErrors.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Example core error enum + ErrorTraits specialization.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

#include <string_view>

#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file CoreErrors.h
 * @brief Core error examples and their traits (extensible pattern).
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/** @brief Core error domain (example). */
enum class CoreErr : std::uint16_t {
    InvalidArgument = 1,
    OutOfRange      = 2,
    NotImplemented  = 3,
    AssertFailed    = 4
};

/** @brief Traits specialization for CoreErr. */
template <>
struct ErrorTraits<CoreErr> {
    static constexpr std::uint16_t domain_id() noexcept { return 0x0001; }
    static constexpr std::string_view domain_name() noexcept { return "Core"; }

    static constexpr Severity default_severity(CoreErr e) noexcept {
        switch (e) {
            case CoreErr::InvalidArgument: return Severity::Error;
            case CoreErr::OutOfRange:      return Severity::Error;
            case CoreErr::NotImplemented:  return Severity::Warning;
            case CoreErr::AssertFailed:    return Severity::Fatal;
        }
        return Severity::Error;
    }

    static constexpr std::string_view key(CoreErr e) noexcept {
        switch (e) {
            case CoreErr::InvalidArgument: return "CORE_INVALID_ARG";
            case CoreErr::OutOfRange:      return "CORE_OUT_OF_RANGE";
            case CoreErr::NotImplemented:  return "CORE_NOT_IMPLEMENTED";
            case CoreErr::AssertFailed:    return "CORE_ASSERT_FAILED";
        }
        return "CORE_UNKNOWN";
    }

    static constexpr std::string_view enUS(CoreErr e) noexcept {
        switch (e) {
            case CoreErr::InvalidArgument:
                return "Invalid argument: {name}.";
            case CoreErr::OutOfRange:
                return "Index out of range: {index}.";
            case CoreErr::NotImplemented:
                return "Feature not implemented.";
            case CoreErr::AssertFailed:
                return "Assertion failed.";
        }
        return "Unknown core error.";
    }

    static constexpr std::string_view ptBR(CoreErr e) noexcept {
        switch (e) {
            case CoreErr::InvalidArgument:
                return "Argumento inválido: {name}.";
            case CoreErr::OutOfRange:
                return "Índice fora do intervalo: {index}.";
            case CoreErr::NotImplemented:
                return "Recurso não implementado.";
            case CoreErr::AssertFailed:
                return "Falha de asserção.";
        }
        return "Erro de núcleo desconhecido.";
    }
};


ERROR_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
