// ============================================================================
// File: IErrorLogger.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.1 (Refactored)
// Description: Abstract interface for error logging sinks.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include c++
// -----------------------------------------------------------------------------
#include <vector>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorRecord.h>

/**
 * @file IErrorLogger.h
 * @brief Abstract base class for error logging destinations (sinks).
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/**
 * @brief Interface for error logging destinations (sinks).
 *
 * Implement this interface to create custom loggers (e.g., FileLogger,
 * ConsoleLogger, NetworkLogger).
 */
class IErrorLogger {
public:
    virtual ~IErrorLogger() = default;

    /** @brief Logs a single error record. */
    virtual void log(const ErrorRecord& record) = 0;

    /**
     * @brief Flushes any buffered error records.
     * @return A vector of the flushed records. Default implementation is a no-op.
     */
    virtual std::vector<ErrorRecord> flush() { return {}; }
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE