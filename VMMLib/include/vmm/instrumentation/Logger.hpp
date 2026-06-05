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
// @file Logger.hpp
// @brief Optional logging support for diagnostics and development traces.
// @ingroup vmm_instrumentation
//==============================================================================
#pragma once

#include <cstdint>
#include <iosfwd>
#include <string_view>

namespace vmm::instrumentation {

/**
 * @brief Severity level used by the optional logger.
 * @ingroup vmm_instrumentation
 */
struct LogLevel final {
    std::string_view name;
    std::uint8_t rank;

    [[nodiscard]] friend constexpr bool operator==(LogLevel lhs, LogLevel rhs) noexcept
    {
        return lhs.rank == rhs.rank && lhs.name == rhs.name;
    }

    [[nodiscard]] friend constexpr bool operator!=(LogLevel lhs, LogLevel rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

struct LogLevelTraits final {
    static constexpr LogLevel Trace{"trace", 0U};
    static constexpr LogLevel Debug{"debug", 1U};
    static constexpr LogLevel Info{"info", 2U};
    static constexpr LogLevel Warning{"warning", 3U};
    static constexpr LogLevel Error{"error", 4U};
};

/**
 * @brief Process-wide diagnostic logger.
 * @ingroup vmm_instrumentation
 *
 * The public API is always available, but message emission is compiled out
 * unless VMM_ENABLE_LOGGING is defined by the build system. Prefer the
 * VMM_LOG_* macros inside hot paths so disabled logging has no run-time cost.
 */
class Logger final {
public:
    [[nodiscard]] static Logger& instance();
    ~Logger() noexcept;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void set_output(std::ostream& output) noexcept;
    void set_minimum_level(LogLevel level) noexcept;

    [[nodiscard]] LogLevel minimum_level() const noexcept;
    [[nodiscard]] bool enabled(LogLevel level) const noexcept;

    void write(LogLevel level, std::string_view message) noexcept;

private:
    Logger() noexcept;

    struct Impl;
    Impl* impl_;
};

void log(LogLevel level, std::string_view message) noexcept;

} // namespace vmm::instrumentation

#if defined(VMM_ENABLE_LOGGING)
#define VMM_LOG_TRACE(message) ::vmm::instrumentation::log(::vmm::instrumentation::LogLevelTraits::Trace, (message))
#define VMM_LOG_DEBUG(message) ::vmm::instrumentation::log(::vmm::instrumentation::LogLevelTraits::Debug, (message))
#define VMM_LOG_INFO(message) ::vmm::instrumentation::log(::vmm::instrumentation::LogLevelTraits::Info, (message))
#define VMM_LOG_WARNING(message) ::vmm::instrumentation::log(::vmm::instrumentation::LogLevelTraits::Warning, (message))
#define VMM_LOG_ERROR(message) ::vmm::instrumentation::log(::vmm::instrumentation::LogLevelTraits::Error, (message))
#else
#define VMM_LOG_TRACE(message) static_cast<void>(0)
#define VMM_LOG_DEBUG(message) static_cast<void>(0)
#define VMM_LOG_INFO(message) static_cast<void>(0)
#define VMM_LOG_WARNING(message) static_cast<void>(0)
#define VMM_LOG_ERROR(message) static_cast<void>(0)
#endif
