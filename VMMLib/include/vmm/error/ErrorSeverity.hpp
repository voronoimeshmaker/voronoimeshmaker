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
// @file ErrorSeverity.hpp
// @brief Severity tags for VoronoiMeshMaker diagnostics.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::error {

/**
 * @brief Open severity identifier for diagnostics and failures.
 * @ingroup vmm_error
 */
class ErrorSeverity final {
public:
    constexpr ErrorSeverity() noexcept = default;
    constexpr explicit ErrorSeverity(std::string_view value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::string_view value() const noexcept { return value_; }

private:
    std::string_view value_{"error"};
};

[[nodiscard]] constexpr bool operator==(ErrorSeverity lhs, ErrorSeverity rhs) noexcept
{
    return lhs.value() == rhs.value();
}

[[nodiscard]] constexpr bool operator!=(ErrorSeverity lhs, ErrorSeverity rhs) noexcept
{
    return !(lhs == rhs);
}

namespace severity {
inline constexpr ErrorSeverity trace{"trace"};
inline constexpr ErrorSeverity information{"information"};
inline constexpr ErrorSeverity warning{"warning"};
inline constexpr ErrorSeverity error{"error"};
inline constexpr ErrorSeverity fatal{"fatal"};
} // namespace severity

} // namespace vmm::error
