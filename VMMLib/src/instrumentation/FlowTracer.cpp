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
// @file FlowTracer.cpp
// @brief Optional scoped flow tracing implementation.
// @ingroup vmm_instrumentation
//==============================================================================

#include <vmm/instrumentation/FlowTracer.hpp>

#include <vmm/instrumentation/Logger.hpp>

#include <string>

namespace vmm::instrumentation {

FlowTracer::FlowTracer(std::string_view scope_name) noexcept
#if defined(VMM_ENABLE_LOGGING)
    : scope_name_(scope_name)
#endif
{
#if defined(VMM_ENABLE_LOGGING)
    log(LogLevel::Debug, std::string{"enter "} + std::string{scope_name_});
#else
    static_cast<void>(scope_name);
#endif
}

FlowTracer::~FlowTracer() noexcept
{
#if defined(VMM_ENABLE_LOGGING)
    log(LogLevel::Debug, std::string{"leave "} + std::string{scope_name_});
#endif
}

} // namespace vmm::instrumentation
