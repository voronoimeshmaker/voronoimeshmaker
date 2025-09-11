// ============================================================================
// File: ErrorManeger.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Minimal thread-safe error recorder and flush.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <chrono>
#include <thread>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

/**
 * @file ErrorManeger.h
 * @brief Simple per-thread buffer for error records and flush.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

struct ErrorRecord {
    std::uint32_t code{0};
    Severity severity{Severity::Error};
    std::string message;
    std::chrono::system_clock::time_point ts{
        std::chrono::system_clock::now()};
    std::thread::id tid{std::this_thread::get_id()};
};

/**
 * @brief Minimal thread-local error recorder.
 * For now it stores message strings; can be replaced by a ring buffer.
 */
class ErrorManeger {
public:
    static void log(ErrorRecord rec) {
        auto cap = Config::get()->thread_buffer_cap;
        auto& buf = buffer();
        if (buf.size() < cap) {
            buf.emplace_back(std::move(rec));
        }
        // else: drop (minimal implementation); can be changed to ring buffer
    }

    static std::vector<ErrorRecord> flush() {
        // Merge all thread-local buffers into a global vector.
        // Minimal version: move from current thread only.
        auto& buf = buffer();
        std::vector<ErrorRecord> out;
        out.swap(buf);
        // (Future: accumulate into a global sink with mutex.)
        return out;
    }

private:
    static std::vector<ErrorRecord>& buffer() {
        thread_local std::vector<ErrorRecord> tl_buf;
        return tl_buf;
    }
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
