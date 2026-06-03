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
// @file Result.hpp
// @brief Expected-like result type for non-throwing VoronoiMeshMaker APIs.
// @ingroup vmm_error
//==============================================================================
#pragma once

#include <vmm/error/Error.hpp>

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

namespace vmm::error {

/**
 * @brief Stores either a value or a structured Error.
 * @ingroup vmm_error
 */
template<class T>
class [[nodiscard]] Result final {
    static_assert(!std::is_reference_v<T>, "Result<T> cannot store references.");

public:
    using value_type = T;

    Result(T value) : storage_(std::move(value)) {}
    Result(Error error) : storage_(std::move(error)) {}

    [[nodiscard]] static Result success(T value) { return Result{std::move(value)}; }
    [[nodiscard]] static Result failure(Error error) { return Result{std::move(error)}; }

    [[nodiscard]] bool has_value() const noexcept { return std::holds_alternative<T>(storage_); }
    [[nodiscard]] bool has_error() const noexcept { return !has_value(); }
    [[nodiscard]] explicit operator bool() const noexcept { return has_value(); }

    [[nodiscard]] T& value() & { return std::get<T>(storage_); }
    [[nodiscard]] const T& value() const& { return std::get<T>(storage_); }
    [[nodiscard]] T&& value() && { return std::get<T>(std::move(storage_)); }

    [[nodiscard]] const Error& error() const& { return std::get<Error>(storage_); }
    [[nodiscard]] Error&& error() && { return std::get<Error>(std::move(storage_)); }

private:
    std::variant<T, Error> storage_;
};

/**
 * @brief Void specialisation of Result.
 * @ingroup vmm_error
 */
template<>
class [[nodiscard]] Result<void> final {
public:
    Result() = default;
    Result(Error error) : has_value_(false), error_(std::move(error)) {}

    [[nodiscard]] static Result success() { return Result{}; }
    [[nodiscard]] static Result failure(Error error) { return Result{std::move(error)}; }

    [[nodiscard]] bool has_value() const noexcept { return has_value_; }
    [[nodiscard]] bool has_error() const noexcept { return !has_value_; }
    [[nodiscard]] explicit operator bool() const noexcept { return has_value_; }

    void value() const
    {
        if(!has_value_) {
            throw std::logic_error{"Result<void> has no value."};
        }
    }

    [[nodiscard]] const Error& error() const& { return error_; }
    [[nodiscard]] Error&& error() && { return std::move(error_); }

private:
    bool has_value_{true};
    Error error_{};
};

} // namespace vmm::error
