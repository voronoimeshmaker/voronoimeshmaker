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
// @file Locale.hpp
// @brief Locale identifiers reserved for diagnostic formatting.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::error {

/**
 * @brief Open locale identifier used by diagnostic formatters.
 * @ingroup vmm_error
 */
class LocaleId final {
public:
    constexpr LocaleId() noexcept = default;
    constexpr explicit LocaleId(std::string_view value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::string_view value() const noexcept { return value_; }

private:
    std::string_view value_{"en-GB"};
};

namespace locale {
inline constexpr LocaleId british_english{"en-GB"};
inline constexpr LocaleId brazilian_portuguese{"pt-BR"};
} // namespace locale

} // namespace vmm::error
