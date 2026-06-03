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
// @file Error.hpp
// @brief Structured error object used by VoronoiMeshMaker.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/ErrorCode.hpp>
#include <vmm/error/ErrorSeverity.hpp>

#include <cstdint>
#include <source_location>
#include <string>
#include <string_view>

namespace vmm::error {

/**
 * @brief Structured failure description independent of exception transport.
 * @ingroup vmm_error
 */
class Error final {
public:
    Error() = default;

    Error(ErrorId id,
          std::string message,
          std::string context = {},
          ErrorSeverity severity = severity::error,
          std::source_location location = std::source_location::current());

    [[nodiscard]] ErrorId id() const noexcept;
    [[nodiscard]] ErrorCode code() const noexcept;
    [[nodiscard]] ErrorSeverity severity() const noexcept;
    [[nodiscard]] const std::string& message() const noexcept;
    [[nodiscard]] const std::string& context() const noexcept;
    [[nodiscard]] std::string_view file_name() const noexcept;
    [[nodiscard]] std::string_view function_name() const noexcept;
    [[nodiscard]] std::uint_least32_t line() const noexcept;

private:
    ErrorId id_{builtin_error_id::unknown};
    ErrorSeverity severity_{severity::error};
    std::string message_;
    std::string context_;
    std::string file_name_;
    std::string function_name_;
    std::uint_least32_t line_{};
};

[[nodiscard]] Error make_error(ErrorId id,
                               std::string message,
                               std::string context = {},
                               ErrorSeverity severity = severity::error,
                               std::source_location location = std::source_location::current());

[[nodiscard]] std::string format_error(const Error& error);

} // namespace vmm::error
