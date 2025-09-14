// ============================================================================
// File: ErrorManager.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.1 (Refactored)
// Description: Error management facade and default thread-local logger.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/IErrorLogger.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>

/**
 * @file ErrorManager.h
 * @brief Provides a default logger and a global access point for logging.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/**
 * @brief A default logger implementation that stores error records in a
 * thread-local buffer with a configurable capacity.
 */
class ThreadLocalBufferLogger : public IErrorLogger {
public:
    void log(const ErrorRecord& record) override {
        auto cap = Config::get()->thread_buffer_cap;
        auto& buf = buffer();
        if (buf.size() < cap) {
            buf.push_back(record);
        }
        // else: drop (minimal implementation); can be changed to ring buffer
    }

    std::vector<ErrorRecord> flush() override {
        auto& buf = buffer();
        std::vector<ErrorRecord> out;
        out.swap(buf);
        return out;
    }

private:
    static std::vector<ErrorRecord>& buffer() {
        thread_local std::vector<ErrorRecord> tl_buf;
        return tl_buf;
    }
};


/**
 * @brief Global facade for the error handling system.
 *
 * Delegates logging and flushing operations to the currently configured
 * IErrorLogger instance.
 */
class ErrorManager {
public:
    /** @brief Logs a record using the globally configured logger. */
    static void log(ErrorRecord rec) {
        if (auto cfg = Config::get()) {
            if (cfg->logger) {
                cfg->logger->log(std::move(rec));
            }
        }
    }

    /** @brief Flushes the globally configured logger. */
    static std::vector<ErrorRecord> flush() {
        if (auto cfg = Config::get()) {
            if (cfg->logger) {
                return cfg->logger->flush();
            }
        }
        return {};
    }
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE