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
// @file Macros.hpp
// @brief Convenience macros for VoronoiMeshMaker error handling.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/Error.hpp>
#include <vmm/error/MeshException.hpp>

#define VMM_THROW_IF(condition, error_id, context, message) \
    do { \
        if(condition) { \
            ::vmm::error::throw_error((error_id), (context), (message)); \
        } \
    } while(false)

#define VMM_RETURN_ERROR_IF(condition, error_id, context, message) \
    do { \
        if(condition) { \
            return ::vmm::error::make_error((error_id), (message), (context)); \
        } \
    } while(false)

#define VMM_RETURN_IF_ERROR(result_expression) \
    do { \
        auto vmm_result__ = (result_expression); \
        if(vmm_result__.has_error()) { \
            return ::vmm::error::Error{vmm_result__.error()}; \
        } \
    } while(false)
