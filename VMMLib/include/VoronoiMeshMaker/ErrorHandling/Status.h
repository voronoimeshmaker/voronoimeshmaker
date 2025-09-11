// ============================================================================
// File: Status.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Minimal Status and StatusOr for non-throw policy.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file Status.h
 * @brief Lightweight status types to avoid exceptions when desired.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

class Status {
public:
    static Status Ok() noexcept { return Status(); }

    Status() noexcept : ok_(true), code_(0), severity_(Severity::Info) {}
    Status(std::uint32_t code, std::string msg, Severity sev)
        : ok_(false), code_(code), msg_(std::move(msg)), severity_(sev) {}

    [[nodiscard]] bool ok() const noexcept { return ok_; }
    [[nodiscard]] std::uint32_t code() const noexcept { return code_; }
    [[nodiscard]] const std::string& message() const noexcept { return msg_; }
    [[nodiscard]] Severity severity() const noexcept { return severity_; }

private:
    bool ok_;
    std::uint32_t code_;
    std::string msg_;
    Severity severity_;
};

template <class T>
class StatusOr {
public:
    StatusOr(T value) : value_(std::move(value)) {}
    StatusOr(Status st) : status_(std::move(st)) {}

    [[nodiscard]] bool ok() const noexcept {
        return status_.has_value() ? status_->ok() : true;
    }

    [[nodiscard]] const Status& status() const noexcept {
        static Status ok = Status::Ok();
        return status_.has_value() ? *status_ : ok;
    }

    [[nodiscard]] T& value() & { return *value_; }
    [[nodiscard]] const T& value() const& { return *value_; }
    [[nodiscard]] T&& value() && { return std::move(*value_); }

private:
    std::optional<T> value_;
    std::optional<Status> status_;
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
