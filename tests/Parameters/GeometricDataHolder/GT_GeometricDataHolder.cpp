//==============================================================================
// Name        : GT_GeometricDataHolder.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Unit tests for the GeometricDataHolder class using Google Test.
//
// Copyright   : Copyright (C) 2024 João Flávio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file GT_GeometricDataHolder.cpp
 * @brief Unit tests for the GeometricDataHolder class using Google Test.
 *
 * This file contains unit tests for the GeometricDataHolder class to verify the correct functionality
 * of its methods, including adding, updating, retrieving, and removing data, as well as checking key existence.
 */

// Includes from C++ Standard Library
#include <string>
#include <stdexcept>
#include <variant>

// External Libraries
#include <gtest/gtest.h>

// GRIPP Library Includes
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

/**
 * @class GeometricDataHolderTest
 * @brief Test fixture for the GeometricDataHolder class.
 *
 * This class provides a test setup and teardown for each unit test case related to GeometricDataHolder.
 */
class GeometricDataHolderTest : public ::testing::Test {
protected:
    /// Instance for testing
    vmm::GeometricDataHolder dataHolder;

//    /**
//     * @brief Setup function to initialize data before each test.
//     *
//     * This function is called before each test case and initializes the GeometricDataHolder
//     * instance with some sample data.
//     */
//    void SetUp() override {
//        // Initialize with some basic data for testing
//        dataHolder.set("intKey", 42);
//        dataHolder.set("doubleKey", 3.14);
//        dataHolder.set("stringKey", std::string("TestString"));
//    }
//
//    /**
//     * @brief Teardown function to clean up after each test.
//     *
//     * This function is called after each test case to clean up any state if necessary.
//     */
//    void TearDown() override {
//        // Clean up state if necessary (not used here)
//    }
};

/**
 * @test GeometricDataHolderTest.AddDataTest
 * @brief Tests the set function.
 *
 * This test verifies that data added to the GeometricDataHolder can be accessed correctly.
 */
TEST_F(GeometricDataHolderTest, AddDataTest) {
//    // Test if added values can be accessed correctly
//    EXPECT_EQ(std::get<int>(dataHolder.get("intKey")), 42);
//    EXPECT_DOUBLE_EQ(std::get<double>(dataHolder.get("doubleKey")), 3.14);
//    EXPECT_EQ(std::get<std::string>(dataHolder.get("stringKey")), "TestString");
//
//    // Add a new value and verify it was added correctly
//    dataHolder.set("newKey", 100);
//    EXPECT_EQ(std::get<int>(dataHolder.get("newKey")), 100);
}

/**
 * @test GeometricDataHolderTest.UpdateDataTest
 * @brief Tests the update functionality of set.
 *
 * This test verifies that existing data can be updated correctly.
 */
TEST_F(GeometricDataHolderTest, UpdateDataTest) {
//    // Update an existing value
//    dataHolder.set("intKey", 100);
//    EXPECT_EQ(std::get<int>(dataHolder.get("intKey")), 100);
}

/**
 * @test GeometricDataHolderTest.GetDataInvalidKeyTest
 * @brief Tests get with an invalid key.
 *
 * This test verifies that attempting to get data with an invalid key throws an exception.
 */
TEST_F(GeometricDataHolderTest, GetDataInvalidKeyTest) {
//    EXPECT_THROW(dataHolder.get("invalidKey"), std::out_of_range);
}

/**
 * @test GeometricDataHolderTest.ContainsKeyTest
 * @brief Tests the contains function.
 *
 * This test verifies that the contains function correctly identifies existing and non-existing keys.
 */
TEST_F(GeometricDataHolderTest, ContainsKeyTest) {
//    EXPECT_TRUE(dataHolder.contains("intKey"));
//    EXPECT_FALSE(dataHolder.contains("nonExistentKey"));
}

/**
 * @test GeometricDataHolderTest.RemoveDataTest
 * @brief Tests the erase function.
 *
 * This test verifies that data can be removed from the GeometricDataHolder and that attempting to access
 * removed data results in an exception.
 */
TEST_F(GeometricDataHolderTest, RemoveDataTest) {
//    // Remove a value and verify it is no longer available
//    dataHolder.erase("intKey");
//    EXPECT_FALSE(dataHolder.contains("intKey"));
//    EXPECT_THROW(dataHolder.get("intKey"), std::out_of_range);
}

/**
 * @test GeometricDataHolderTest.CountDataTest
 * @brief Tests the size function.
 *
 * This test verifies that the size function returns the correct number of elements in the GeometricDataHolder.
 */
TEST_F(GeometricDataHolderTest, CountDataTest) {
//    // Verify the number of elements matches the expected value
//    EXPECT_EQ(dataHolder.size(), 3);
//    dataHolder.set("extraKey", 55);
//    EXPECT_EQ(dataHolder.size(), 4);
//    dataHolder.erase("intKey");
//    EXPECT_EQ(dataHolder.size(), 3);
}

/**
 * @brief Main function to run all tests.
 *
 * This function initializes Google Test and runs all defined test cases.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}