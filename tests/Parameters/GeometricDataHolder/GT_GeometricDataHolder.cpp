//==============================================================================
// Name        : GT_GeometricDataHolder.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Unit tests for the GeometricDataHolder class using Google Test.
//
// Copyright   : Copyright (C) 2024 João Flávio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : This program is free software: you can redistribute it and/or modify
//               it under the terms of the GNU General Public License as published by
//               the Free Software Foundation, either version 3 of the License, or
//               (at your option) any later version.
//
//               This program is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty of
//               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//               GNU General Public License for more details.
//
//               You should have received a copy of the GNU General Public License
//               along with this program. If not, see <https://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================

#include <mutex>
#include <string>
#include <variant>

//==============================================================================
// External Libraries
//==============================================================================

#include <gtest/gtest.h>

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Misc/namespace.h>
#include <VoronoiMeshMaker/Misc/type.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

// Namespace for Google Test
using namespace testing;

/**
 * @file GT_GeometricDataHolder.cpp
 * @brief Unit tests for the GeometricDataHolder class using Google Test framework.
 *
 * This file contains unit tests for the GeometricDataHolder class, which is part
 * of the Parameters module of the VoronoiMeshMaker library. The tests validate the behavior
 * of various functions and features provided by GeometricDataHolder, ensuring that
 * it meets expected requirements and handles edge cases appropriately.
 *
 * @author João Flávio Vieira de Vasconcellos
 * @version 1.1
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

/**
 * @brief Test fixture for GeometricDataHolder.
 *
 * This class provides a test fixture for the GeometricDataHolder class, setting up
 * reusable test data and configurations.
 */
class GeometricDataHolderTest : public ::Test {
    
protected:
    
    vmm::GeometricDataHolder dataHolder;

    void SetUp() override {
        // Initialize with some common data used in multiple tests
        dataHolder.set("length", 10.5);
        dataHolder.set("width", 5.0);
        dataHolder.set("name", std::string("Rectangle"));
    }

    void TearDown() override {
        // Any cleanup after tests
    }
};

/**
 * @test Verify basic set and get operations for GeometricDataHolder.
 *
 * This test verifies that values can be correctly set and retrieved from
 * GeometricDataHolder.
 */
TEST_F(GeometricDataHolderTest, SetGetValues) {
    EXPECT_EQ(dataHolder.get<Real>("length").value(), 10.5);
    EXPECT_EQ(dataHolder.get<Real>("width").value(), 5.0);
    EXPECT_EQ(dataHolder.get<std::string>("name").value(), "Rectangle");
}

/**
 * @test Verify behavior when requesting a non-existent key.
 *
 * This test verifies that attempting to get a value for a key that has not
 * been set results in an appropriate empty optional.
 */
TEST_F(GeometricDataHolderTest, GetNonExistentKey) {
    EXPECT_EQ(dataHolder.get<Real>("height"), std::nullopt);
}

/**
 * @test Verify updating an existing key with a new value.
 *
 * This test checks whether updating an existing key in GeometricDataHolder
 * works as expected, replacing the old value with the new one.
 */
TEST_F(GeometricDataHolderTest, UpdateValue) {
    dataHolder.set("length", 15.0);
    EXPECT_EQ(dataHolder.get<Real>("length").value(), 15.0);
}

/**
 * @test Verify handling of multiple data types.
 *
 * This test verifies that GeometricDataHolder can handle multiple data types,
 * such as integers, Reals, and strings.
 */
TEST_F(GeometricDataHolderTest, HandleMultipleDataTypes) {
    dataHolder.set("count", 3);
    EXPECT_EQ(dataHolder.get<int>("count").value(), 3);

    dataHolder.set("pi", 3.14159);
    EXPECT_EQ(dataHolder.get<Real>("pi").value(), 3.14159);

    dataHolder.set("label", std::string("TestLabel"));
    EXPECT_EQ(dataHolder.get<std::string>("label").value(), "TestLabel");
}
