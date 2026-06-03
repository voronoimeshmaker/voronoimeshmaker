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
// @file PhaseTimer.hpp
// @brief Optional scoped phase timing support.
// @ingroup vmm_instrumentation
//==============================================================================
#pragma once

#include <chrono>
#include <string_view>

namespace vmm::instrumentation {

/**
 * @brief RAII timer for named algorithmic phases.
 * @ingroup vmm_instrumentation
 *
 * When VMM_ENABLE_TIMING is disabled, construction, destruction, and stop() are
 * all empty inline operations. Use VMM_TIME_PHASE(name) at phase boundaries.
 */
class PhaseTimer final {
public:
    explicit PhaseTimer(std::string_view phase_name) noexcept;
    ~PhaseTimer() noexcept;

    PhaseTimer(const PhaseTimer&) = delete;
    PhaseTimer& operator=(const PhaseTimer&) = delete;
    PhaseTimer(PhaseTimer&&) = delete;
    PhaseTimer& operator=(PhaseTimer&&) = delete;

    void stop() noexcept;

private:
#if defined(VMM_ENABLE_TIMING)
    using Clock = std::chrono::steady_clock;

    std::string_view phase_name_;
    Clock::time_point start_;
    bool running_;
#endif
};

} // namespace vmm::instrumentation

#ifndef VMM_DETAIL_CONCAT
#define VMM_DETAIL_CONCAT_IMPL(left, right) left##right
#define VMM_DETAIL_CONCAT(left, right) VMM_DETAIL_CONCAT_IMPL(left, right)
#endif

#if defined(VMM_ENABLE_TIMING)
#define VMM_TIME_PHASE(name) const ::vmm::instrumentation::PhaseTimer VMM_DETAIL_CONCAT(vmm_phase_timer_, __LINE__){(name)}
#else
#define VMM_TIME_PHASE(name) static_cast<void>(0)
#endif
