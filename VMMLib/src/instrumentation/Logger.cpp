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
// @file Logger.cpp
// @brief Optional logging support implementation.
// @ingroup vmm_instrumentation
//==============================================================================

#include <vmm/instrumentation/Logger.hpp>

#include <array>
#include <iostream>
#include <mutex>
#include <ostream>

namespace vmm::instrumentation {
namespace {

[[nodiscard]] constexpr std::string_view level_name(LogLevel level) noexcept
{
    return level.name;
}

[[nodiscard]] constexpr auto level_rank(LogLevel level) noexcept
{
    return level.rank;
}

} // namespace

struct Logger::Impl final {
    mutable std::mutex mutex{};
    std::ostream* output{&std::clog};
    LogLevel minimum_level{LogLevelTraits::Info};
};

Logger::Logger() noexcept
    : impl_(new Impl{})
{
}

Logger::~Logger() noexcept
{
    delete impl_;
}

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::set_output(std::ostream& output) noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    std::lock_guard<std::mutex> lock{impl_->mutex};
    impl_->output = &output;
#else
    static_cast<void>(output);
#endif
}

void Logger::set_minimum_level(LogLevel level) noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    std::lock_guard<std::mutex> lock{impl_->mutex};
    impl_->minimum_level = level;
#else
    static_cast<void>(level);
#endif
}

LogLevel Logger::minimum_level() const noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    std::lock_guard<std::mutex> lock{impl_->mutex};
    return impl_->minimum_level;
#else
    return LogLevelTraits::Error;
#endif
}

bool Logger::enabled(LogLevel level) const noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    std::lock_guard<std::mutex> lock{impl_->mutex};
    return level_rank(level) >= level_rank(impl_->minimum_level);
#else
    static_cast<void>(level);
    return false;
#endif
}

void Logger::write(LogLevel level, std::string_view message) noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    std::lock_guard<std::mutex> lock{impl_->mutex};

    if(level_rank(level) < level_rank(impl_->minimum_level) || impl_->output == nullptr) {
        return;
    }

    (*impl_->output) << "[vmm][" << level_name(level) << "] " << message << '\n';
#else
    static_cast<void>(level);
    static_cast<void>(message);
#endif
}

void log(LogLevel level, std::string_view message) noexcept
{
    Logger::instance().write(level, message);
}

} // namespace vmm::instrumentation
