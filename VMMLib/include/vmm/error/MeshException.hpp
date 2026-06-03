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
// @file MeshException.hpp
// @brief VoronoiMeshMaker exception type and throw helpers.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/Error.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace vmm::error {

/**
 * @brief Exception transport for structured VoronoiMeshMaker errors.
 * @ingroup vmm_error
 */
class MeshException : public std::runtime_error {
public:
    explicit MeshException(Error error);
    MeshException(ErrorId id, std::string message);
    MeshException(ErrorId id, std::string_view context, std::string_view message);

    [[nodiscard]] const Error& error() const noexcept;
    [[nodiscard]] ErrorId id() const noexcept;
    [[nodiscard]] ErrorCode code() const noexcept;
    [[nodiscard]] const std::string& context() const noexcept;

private:
    Error error_;
};

[[noreturn]] void throw_error(Error error);
[[noreturn]] void throw_error(ErrorId id, std::string message);
[[noreturn]] void throw_error(ErrorId id, std::string_view context, std::string_view message);
[[noreturn]] void throw_file_not_found(std::string_view context, std::string_view path);
[[noreturn]] void throw_parse_error(std::string_view context, std::string_view message);
[[noreturn]] void throw_invalid_argument(std::string_view context, std::string_view message);

} // namespace vmm::error
