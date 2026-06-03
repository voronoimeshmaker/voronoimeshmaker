//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file ErrorCode.hpp
// @brief Backwards-compatible include for extensible VoronoiMeshMaker error identifiers.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/ErrorId.hpp>

namespace vmm::error {

/**
 * @brief Compatibility alias for older code that used ErrorCode.
 * @ingroup vmm_error
 *
 * New code should prefer ErrorId and builtin_error_id. The alias remains open:
 * any ErrorId value is valid, including project-specific identifiers.
 */
using ErrorCode = ErrorId;

} // namespace vmm::error
