// ============================================================================
// File: Macros.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.6
// Description: Convenience macros for throwing and logging.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  VoronoiMeshMaker
// -----------------------------------------------------------------------------
// Precisamos do enum CoreErr aqui (VMM_ASSERT e usos diretos em chamadas).
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>
#include <VoronoiMeshMaker/ErrorHandling/Detail.h> // detail::log_error

/**
 * @file Macros.h
 * @brief Convenience macros for pre/post conditions, throw and log.
 * @ingroup errorhandling
 *
 * Observações:
 * - Suporta chamadas COM e SEM lista de pares {{"k","v"},...} graças a __VA_OPT__.
 * - VMMException já possui source_location padrão (current()), logo não passamos aqui.
 */

/** @brief Throw a VMMException with optional key/value replacements. */
#define VMM_THROW(err_enum, /* {{"k","v"},...} */ ...)                                  \
    do {                                                                                \
        throw ::vmm::error::VMMException(                                               \
            (err_enum) __VA_OPT__(, __VA_ARGS__)                                        \
        );                                                                              \
    } while (0)

/** @brief Register an error record (no throw). Delegates to helper function. */
#define VMM_ERROR(err_enum, /* {{"k","v"},...} */ ...)                                  \
    do {                                                                                \
        ::vmm::error::detail::log_error(                                                \
            (err_enum) __VA_OPT__(, __VA_ARGS__)                                        \
        );                                                                              \
    } while (0)

/** @brief Require condition, otherwise throw with optional KV message. */
#define VMM_REQUIRE(cond, err_enum, /* {{"k","v"},...} */ ...)                          \
    do {                                                                                \
        if (!(cond)) {                                                                  \
            VMM_THROW((err_enum) __VA_OPT__(, __VA_ARGS__));                            \
        }                                                                               \
    } while (0)

/** @brief Debug-only assert (throws VMMException in Debug; no-op in Release). */
#ifndef NDEBUG
#  define VMM_ASSERT(cond)                                                              \
    do {                                                                                \
        if (!(cond)) {                                                                  \
            VMM_THROW(::vmm::error::CoreErr::AssertFailed);                             \
        }                                                                               \
    } while (0)
#else
#  define VMM_ASSERT(cond) do { (void)sizeof(cond); } while (0)
#endif
