// ============================================================================
// File: VMMException.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Library exception with localized message and code.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

#include <exception>
#include <source_location>
#include <string>
#include <string_view>

#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/MessageCatalog.hpp>
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file VMMException.h
 * @brief Exception class for VoronoiMeshMaker.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/**
 * @brief Minimal exception that stores code, severity and messages.
 *
 * - what(): English message (stable for logs).
 * - message(): Localized message according to ErrorConfig::language.
 */
class VMMException : public std::exception {
public:
    template <ErrorEnum E>
    VMMException(E e,
                 std::initializer_list<std::pair<std::string_view,
                                                 std::string_view>> kv = {},
                 std::source_location loc = std::source_location::current())
        : code_(error_code(e)),
          severity_(ErrorTraits<E>::default_severity(e)),
          key_(ErrorTraits<E>::key(e)),
          loc_(loc) {
        // English for what()
        what_ = render(e, Language::EnUS, kv);
        // Localized for message()
        auto cfg = Config::get();
        message_ = render(e, cfg->language, kv);
    }

    const char* what() const noexcept override { return what_.c_str(); }

    [[nodiscard]] const std::string& message() const noexcept {
        return message_;
    }

    [[nodiscard]] std::uint32_t code() const noexcept { return code_; }
    [[nodiscard]] Severity severity() const noexcept { return severity_; }
    [[nodiscard]] std::string_view key() const noexcept { return key_; }

    [[nodiscard]] const std::source_location& location() const noexcept {
        return loc_;
    }

private:
    std::uint32_t code_{0};
    Severity severity_{Severity::Error};
    std::string what_;
    std::string message_;
    std::string_view key_;
    std::source_location loc_;
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
