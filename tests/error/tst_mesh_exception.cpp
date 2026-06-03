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
// @file tst_mesh_exception.cpp
// @brief Unit tests for VoronoiMeshMaker error handling.
// @ingroup vmm_tests
//==============================================================================

#include <vmm/error/ErrorRegistry.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/error/Result.hpp>

#include <gtest/gtest.h>

#include <string>

namespace {

TEST(MeshExceptionTest, CarriesStructuredError)
{
    const vmm::error::MeshException exception{
        vmm::error::builtin_error_id::parse_error,
        "config",
        "invalid scalar"
    };

    EXPECT_EQ(exception.id(), vmm::error::builtin_error_id::parse_error);
    EXPECT_EQ(exception.code(), vmm::error::builtin_error_id::parse_error);
    EXPECT_EQ(exception.context(), "config");
    EXPECT_EQ(exception.error().message(), "invalid scalar");

    const std::string message{exception.what()};
    EXPECT_NE(message.find("vmm.parse_error"), std::string::npos);
    EXPECT_NE(message.find("config"), std::string::npos);
    EXPECT_NE(message.find("invalid scalar"), std::string::npos);
}

TEST(MeshExceptionTest, ThrowHelpersUseSpecificIds)
{
    try {
        vmm::error::throw_file_not_found("reader", "missing.yaml");
    } catch(const vmm::error::MeshException& exception) {
        EXPECT_EQ(exception.id(), vmm::error::builtin_error_id::file_not_found);
        EXPECT_EQ(exception.context(), "reader");
        return;
    }

    FAIL() << "Expected MeshException.";
}

TEST(ErrorRegistryTest, FindsBuiltinDescriptor)
{
    const auto* descriptor = vmm::error::find_builtin_error(vmm::error::builtin_error_id::geometry_error);

    ASSERT_NE(descriptor, nullptr);
    EXPECT_EQ(descriptor->id, vmm::error::builtin_error_id::geometry_error);
    EXPECT_EQ(descriptor->name, "geometry_error");
}

TEST(ResultTest, StoresValueOrError)
{
    const vmm::error::Result<int> value = vmm::error::Result<int>::success(42);
    EXPECT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), 42);

    const vmm::error::Result<int> failure = vmm::error::Result<int>::failure(
        vmm::error::make_error(vmm::error::builtin_error_id::invalid_argument, "bad input", "test")
    );

    EXPECT_TRUE(failure.has_error());
    EXPECT_EQ(failure.error().id(), vmm::error::builtin_error_id::invalid_argument);
    EXPECT_EQ(failure.error().context(), "test");
}

TEST(ResultTest, SupportsVoid)
{
    const auto ok = vmm::error::Result<void>::success();
    EXPECT_TRUE(ok.has_value());
    EXPECT_NO_THROW(ok.value());

    const auto failure = vmm::error::Result<void>::failure(
        vmm::error::make_error(vmm::error::builtin_error_id::backend_error, "backend failed")
    );
    EXPECT_TRUE(failure.has_error());
}

} // namespace
