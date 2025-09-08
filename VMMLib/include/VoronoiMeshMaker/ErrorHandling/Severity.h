// ============================================================================
// File: Severity.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Severity levels for errors and logs.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

#include <VoronoiMeshMaker/Core/namespace.h>

/**
 * @file Severity.h
 * @brief Severity levels for error records and exceptions.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/** @brief Severity of an error or log record. */
enum class Severity {
    Info,
    Warning,
    Error,
    Fatal
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
