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
// @file ErrorId.hpp
// @brief Extensible identifiers for VoronoiMeshMaker errors.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::error {

/**
 * @brief Lightweight, extensible identifier for an error kind.
 * @ingroup vmm_error
 *
 * ErrorId is intentionally open: users and downstream modules may create new
 * identifiers without changing VoronoiMeshMaker headers or rebuilding the core.
 */
class ErrorId final {
public:
    constexpr ErrorId() noexcept = default;
    constexpr explicit ErrorId(std::string_view value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::string_view value() const noexcept { return value_; }
    [[nodiscard]] constexpr bool empty() const noexcept { return value_.empty(); }

private:
    std::string_view value_{"vmm.unknown"};
};

[[nodiscard]] constexpr bool operator==(ErrorId lhs, ErrorId rhs) noexcept
{
    return lhs.value() == rhs.value();
}

[[nodiscard]] constexpr bool operator!=(ErrorId lhs, ErrorId rhs) noexcept
{
    return !(lhs == rhs);
}

namespace builtin_error_id {
inline constexpr ErrorId unknown{"vmm.unknown"};
inline constexpr ErrorId invalid_argument{"vmm.invalid_argument"};
inline constexpr ErrorId invalid_configuration{"vmm.invalid_configuration"};
inline constexpr ErrorId file_not_found{"vmm.file_not_found"};
inline constexpr ErrorId parse_error{"vmm.parse_error"};
inline constexpr ErrorId geometry_error{"vmm.geometry_error"};
inline constexpr ErrorId invalid_polygon{"vmm.domain.invalid_polygon"};
inline constexpr ErrorId degenerate_polygon{"vmm.domain.degenerate_polygon"};
inline constexpr ErrorId self_intersecting_polygon{"vmm.domain.self_intersecting_polygon"};
inline constexpr ErrorId topology_error{"vmm.topology_error"};
inline constexpr ErrorId backend_error{"vmm.backend_error"};
inline constexpr ErrorId unsupported_operation{"vmm.unsupported_operation"};
} // namespace builtin_error_id

[[nodiscard]] constexpr std::string_view to_string(ErrorId id) noexcept
{
    return id.value();
}

} // namespace vmm::error
