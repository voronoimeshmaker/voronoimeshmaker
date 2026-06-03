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
// @file FlowTracer.hpp
// @brief Optional scoped flow tracing support.
// @ingroup vmm_instrumentation
//==============================================================================
#pragma once

#include <string_view>

namespace vmm::instrumentation {

/**
 * @brief RAII helper that logs entry and exit of a named scope.
 * @ingroup vmm_instrumentation
 */
class FlowTracer final {
public:
    explicit FlowTracer(std::string_view scope_name) noexcept;
    ~FlowTracer() noexcept;

    FlowTracer(const FlowTracer&) = delete;
    FlowTracer& operator=(const FlowTracer&) = delete;
    FlowTracer(FlowTracer&&) = delete;
    FlowTracer& operator=(FlowTracer&&) = delete;

private:
#if defined(VMM_ENABLE_LOGGING)
    std::string_view scope_name_;
#endif
};

} // namespace vmm::instrumentation

#ifndef VMM_DETAIL_CONCAT
#define VMM_DETAIL_CONCAT_IMPL(left, right) left##right
#define VMM_DETAIL_CONCAT(left, right) VMM_DETAIL_CONCAT_IMPL(left, right)
#endif

#if defined(VMM_ENABLE_LOGGING)
#define VMM_TRACE_SCOPE(name) const ::vmm::instrumentation::FlowTracer VMM_DETAIL_CONCAT(vmm_flow_tracer_, __LINE__){(name)}
#else
#define VMM_TRACE_SCOPE(name) static_cast<void>(0)
#endif
