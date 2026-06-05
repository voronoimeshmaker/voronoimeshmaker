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
// @file PhaseTimer.cpp
// @brief Optional scoped phase timing implementation.
// @ingroup vmm_instrumentation
//==============================================================================

#include <vmm/instrumentation/PhaseTimer.hpp>

#include <vmm/instrumentation/Logger.hpp>

#include <iomanip>
#include <sstream>

namespace vmm::instrumentation {

PhaseTimer::PhaseTimer(std::string_view phase_name) noexcept
#if defined(VMM_ENABLE_TIMING)
    : phase_name_(phase_name),
      start_(Clock::now()),
      running_(true)
#endif
{
#if !defined(VMM_ENABLE_TIMING)
    static_cast<void>(phase_name);
#endif
}

PhaseTimer::~PhaseTimer() noexcept
{
    stop();
}

void PhaseTimer::stop() noexcept
{
#if defined(VMM_ENABLE_TIMING)
    if(!running_) {
        return;
    }

    running_ = false;

    const auto finish = Clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(finish - start_);

    std::ostringstream message;
    message << phase_name_ << " took " << std::fixed << std::setprecision(3) << elapsed.count() << " ms";

    log(LogLevelTraits::Info, message.str());
#endif
}

} // namespace vmm::instrumentation
