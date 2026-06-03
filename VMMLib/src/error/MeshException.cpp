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
// @file MeshException.cpp
// @brief VoronoiMeshMaker exception implementation.
// @ingroup vmm_error
//==============================================================================

#include <vmm/error/MeshException.hpp>

#include <utility>

namespace vmm::error {

MeshException::MeshException(Error error)
    : std::runtime_error(format_error(error)),
      error_(std::move(error))
{
}

MeshException::MeshException(ErrorId id, std::string message)
    : MeshException(Error{id, std::move(message)})
{
}

MeshException::MeshException(ErrorId id, std::string_view context, std::string_view message)
    : MeshException(Error{id, std::string{message}, std::string{context}})
{
}

const Error& MeshException::error() const noexcept
{
    return error_;
}

ErrorId MeshException::id() const noexcept
{
    return error_.id();
}

ErrorCode MeshException::code() const noexcept
{
    return error_.code();
}

const std::string& MeshException::context() const noexcept
{
    return error_.context();
}

void throw_error(Error error)
{
    throw MeshException{std::move(error)};
}

void throw_error(ErrorId id, std::string message)
{
    throw MeshException{Error{id, std::move(message)}};
}

void throw_error(ErrorId id, std::string_view context, std::string_view message)
{
    throw MeshException{Error{id, std::string{message}, std::string{context}}};
}

void throw_file_not_found(std::string_view context, std::string_view path)
{
    std::string message{"Could not open file: "};
    message += path;
    throw MeshException{Error{builtin_error_id::file_not_found, std::move(message), std::string{context}}};
}

void throw_parse_error(std::string_view context, std::string_view message)
{
    throw MeshException{Error{builtin_error_id::parse_error, std::string{message}, std::string{context}}};
}

void throw_invalid_argument(std::string_view context, std::string_view message)
{
    throw MeshException{Error{builtin_error_id::invalid_argument, std::string{message}, std::string{context}}};
}

} // namespace vmm::error
