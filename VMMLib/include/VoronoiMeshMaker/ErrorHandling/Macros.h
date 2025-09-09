// ============================================================================
// File: Macros.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Convenience macros for throwing and logging.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorManager.h>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>

/**
 * @file Macros.h
 * @brief Convenience macros for pre/post conditions, throw and log.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

inline void swallow(...) {}

DETAIL_NAMESPACE_CLOSE
ERROR_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE

/** @brief Throw a VMMException with key/value replacements. */
#define VMM_THROW(err_enum, /* {{"k","v"},...} */ ...)                                 \
    throw ::vmm::error::VMMException(                                                  \
        (err_enum), __VA_ARGS__, std::source_location::current())

/** @brief Register an error record (no throw). */
#define VMM_ERROR(err_enum, /* {{"k","v"},...} */ ...)                                 \
    do {                                                                               \
        using ::vmm::error::Config;                                                    \
        using ::vmm::error::ErrorRecord;                                               \
        using ::vmm::error::ErrorTraits;                                               \
        using ::vmm::error::error_code;                                                \
        using ::vmm::error::render;                                                    \
        auto e   = (err_enum);                                                         \
        auto cfg = Config::get();                                                      \
        ErrorRecord rec;                                                               \
        rec.code     = error_code(e);                                                  \
        rec.severity = ErrorTraits<decltype(e)>::default_severity(e);                  \
        rec.message  = render(e, cfg->language, __VA_ARGS__);                          \
        ::vmm::error::ErrorManager::log(std::move(rec));                               \
    } while (0)

/** @brief Precondition: throws if !(cond). */
#define VMM_REQUIRE(cond, err_enum, ...)                                               \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            VMM_THROW((err_enum), __VA_ARGS__);                                        \
        }                                                                              \
    } while (0)

/** @brief Postcondition: throws if !(cond). */
#define VMM_ENSURE(cond, err_enum, ...)                                                \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            VMM_THROW((err_enum), __VA_ARGS__);                                        \
        }                                                                              \
    } while (0)

/** @brief Debug-only assert. */
#ifndef NDEBUG
#  define VMM_ASSERT(cond)                                                             \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            VMM_THROW(::vmm::error::CoreErr::AssertFailed);                            \
        }                                                                              \
    } while (0)
#else
#  define VMM_ASSERT(cond) do { (void)sizeof(cond); } while (0)
#endif
