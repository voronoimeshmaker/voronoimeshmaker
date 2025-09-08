// ============================================================================
// File: Language.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Language selection for error messages.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

#include <VoronoiMeshMaker/Core/namespace.h>

/**
 * @file Language.h
 * @brief Language tag for localized messages.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/** @brief Supported languages for message localization. */
enum class Language {
    EnUS,  ///< English (US)
    PtBR   ///< Portuguese (Brazil)
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
