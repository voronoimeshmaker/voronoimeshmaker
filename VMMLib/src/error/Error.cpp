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
// @file Error.cpp
// @brief Structured VoronoiMeshMaker error implementation.
// @ingroup vmm_error
//==============================================================================

#include <vmm/error/Error.hpp>
#include <vmm/error/ErrorRegistry.hpp>

#include <utility>

namespace vmm::error {

Error::Error(ErrorId id,
             std::string message,
             std::string context,
             ErrorSeverity error_severity,
             std::source_location location)
    : id_(id.empty() ? builtin_error_id::unknown : id),
      severity_(error_severity),
      message_(std::move(message)),
      context_(std::move(context)),
      file_name_(location.file_name()),
      function_name_(location.function_name()),
      line_(location.line())
{
    if(message_.empty()) {
        if(const ErrorDescriptor* descriptor = find_builtin_error(id_); descriptor != nullptr) {
            message_ = std::string{descriptor->british_english_message};
            message_ += " / ";
            message_ += descriptor->brazilian_portuguese_message;
        }
    }
}

ErrorId Error::id() const noexcept
{
    return id_;
}

ErrorCode Error::code() const noexcept
{
    return id_;
}

ErrorSeverity Error::severity() const noexcept
{
    return severity_;
}

const std::string& Error::message() const noexcept
{
    return message_;
}

const std::string& Error::context() const noexcept
{
    return context_;
}

std::string_view Error::file_name() const noexcept
{
    return file_name_;
}

std::string_view Error::function_name() const noexcept
{
    return function_name_;
}

std::uint_least32_t Error::line() const noexcept
{
    return line_;
}

Error make_error(ErrorId id,
                 std::string message,
                 std::string context,
                 ErrorSeverity error_severity,
                 std::source_location location)
{
    return Error{id, std::move(message), std::move(context), error_severity, location};
}

std::string format_error(const Error& error)
{
    std::string result{"["};
    result += to_string(error.id());
    result += "]";

    if(!error.context().empty()) {
        result += " ";
        result += error.context();
        result += ":";
    }

    result += " ";
    result += error.message();
    return result;
}

} // namespace vmm::error
