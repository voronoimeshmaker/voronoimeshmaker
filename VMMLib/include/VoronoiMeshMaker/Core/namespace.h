//==============================================================================
// Name        : namespace.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.2
// Description : Namespace macros for the VoronoiMeshMaker library.
//               Single-responsibility: no logic, no state.
// License     : GNU GPL v3
//==============================================================================

#pragma once

/**
 * @file namespace.h
 * @brief Namespace macros for the VoronoiMeshMaker library.
 *
 * These macros provide a consistent way to open/close the main library
 * namespace (`vmm`) and the geometry sub-namespace (`gtp`). They exist to
 * keep source files tidy and to make it harder to forget a closing brace.
 *
 * @ingroup core
 */

/** @defgroup core Core
 *  Core infrastructure (namespaces, types, constants).
 *  @{
 */

/**
 * @brief Open the main namespace of the library.
 *
 * Usage:
 * @code
 * VORMAKER_NAMESPACE_OPEN
 * // ... code inside namespace vmm ...
 * VORMAKER_NAMESPACE_CLOSE
 * @endcode
 */
#define VORMAKER_NAMESPACE_OPEN \
    namespace vmm {

/**
 * @brief Close the main namespace of the library.
 */
#define VORMAKER_NAMESPACE_CLOSE \
    }

/**
 * @brief Open the geometry sub-namespace (to be used *inside* vmm).
 *
 * Usage:
 * @code
 * VORMAKER_NAMESPACE_OPEN
 * GEOTYPES_NAMESPACE_OPEN
 * // ... code inside namespace vmm::gtp ...
 * GEOTYPES_NAMESPACE_CLOSE
 * VORMAKER_NAMESPACE_CLOSE
 * @endcode
 */
#define GEOTYPES_NAMESPACE_OPEN \
    namespace gtp {

/**
 * @brief Close the geometry sub-namespace.
 */
#define GEOTYPES_NAMESPACE_CLOSE \
    }

/** @} */  // end of core
