// ============================================================================
// File: ErrorRecord.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.1 (Refactored)
// Description: Defines the structure for a single error record.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <chrono>
#include <string>
#include <thread>
#include <cstdint>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file ErrorRecord.h
 * @brief Structure for holding detailed information about a logged error.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/** @brief Holds all information about a single logged error event. */
struct ErrorRecord {
    std::uint32_t code{0};
    Severity severity{Severity::Error};
    std::string message;
    std::chrono::system_clock::time_point ts{
        std::chrono::system_clock::now()};
    std::thread::id tid{std::this_thread::get_id()};
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE