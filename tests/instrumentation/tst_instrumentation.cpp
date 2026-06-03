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
// @file tst_instrumentation.cpp
// @brief Unit tests for optional instrumentation helpers.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/instrumentation/FlowTracer.hpp>
#include <vmm/instrumentation/Logger.hpp>
#include <vmm/instrumentation/PhaseTimer.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <string>

namespace {

TEST(InstrumentationTest, LoggerHonoursCompileTimeSwitch)
{
    std::ostringstream output;

    auto& logger = vmm::instrumentation::Logger::instance();
    logger.set_output(output);
    logger.set_minimum_level(vmm::instrumentation::LogLevel::trace);

    vmm::instrumentation::log(vmm::instrumentation::LogLevel::info, "instrumentation message");

#if defined(VMM_ENABLE_LOGGING)
    EXPECT_NE(output.str().find("instrumentation message"), std::string::npos);
#else
    EXPECT_TRUE(output.str().empty());
#endif
}

TEST(InstrumentationTest, PhaseTimerCanBeScoped)
{
    std::ostringstream output;

    auto& logger = vmm::instrumentation::Logger::instance();
    logger.set_output(output);
    logger.set_minimum_level(vmm::instrumentation::LogLevel::trace);

    {
        VMM_TIME_PHASE("test phase");
    }

#if defined(VMM_ENABLE_TIMING) && defined(VMM_ENABLE_LOGGING)
    EXPECT_NE(output.str().find("test phase took"), std::string::npos);
#else
    EXPECT_TRUE(output.str().empty());
#endif
}

TEST(InstrumentationTest, FlowTracerCanBeScoped)
{
    std::ostringstream output;

    auto& logger = vmm::instrumentation::Logger::instance();
    logger.set_output(output);
    logger.set_minimum_level(vmm::instrumentation::LogLevel::trace);

    {
        VMM_TRACE_SCOPE("scope");
    }

#if defined(VMM_ENABLE_LOGGING)
    EXPECT_NE(output.str().find("enter scope"), std::string::npos);
    EXPECT_NE(output.str().find("leave scope"), std::string::npos);
#else
    EXPECT_TRUE(output.str().empty());
#endif
}

} // namespace
