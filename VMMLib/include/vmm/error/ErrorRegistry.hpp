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
// @file ErrorRegistry.hpp
// @brief Built-in error catalogue for VoronoiMeshMaker.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/ErrorId.hpp>
#include <vmm/error/ErrorSeverity.hpp>

#include <span>
#include <string_view>

namespace vmm::error {

/**
 * @brief Static description for a known error identifier.
 * @ingroup vmm_error
 */
struct ErrorDescriptor final {
    ErrorId id{};
    ErrorSeverity default_severity{};
    std::string_view name{};
    std::string_view british_english_message{};
    std::string_view brazilian_portuguese_message{};
};

[[nodiscard]] std::span<const ErrorDescriptor> builtin_error_descriptors() noexcept;
[[nodiscard]] const ErrorDescriptor* find_builtin_error(ErrorId id) noexcept;

} // namespace vmm::error
