//==============================================================================
// Name        : GeometricDataHolder.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Defines the GeometricDataHolder class and the DataTraits 
//               for handling multiple data types.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
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
 * @file GeometricDataHolder.h
 * @brief Defines the GeometricDataHolder class and the DataTraits for handling multiple data types.
 * This class uses std::variant to store different types and allows access through string keys.
 * The DataTraits structure provides type identification for the supported data types.
 *
 * @ingroup parameters
 * 
 * This file is part of the "parameters" group, which defines classes and traits to handle
 * multiple types of parameters for computational geometry and other operations.
 * 
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_DATAHOLDER_H__
#define __VORONOIMESHMAKER_DATAHOLDER_H__

#include <execution>

//==============================================================================
//  VoronoiMeshMaker includes
//==============================================================================

//#include <VoronoiMeshMaker/Misc/type.h>
#include <VoronoiMeshMaker/Misc/ID.h>                   // class ID
#include <VoronoiMeshMaker/Parameters/GeometricDataTraits.h>

VORMAKER_NAMESPACE_OPEN

        
/**
 * @typedef DataType
 * @brief Defines the variant type that can store different data types, such as int, Real, and geometric points.
 */
using DataType = std::variant<
    int, Real, gtp::Point2D, gtp::Point3D, gtp::Vector2D, gtp::Vector3D, 
    LstInt, LstReal, gtp::LstPoint2D, gtp::LstPoint3D, 
    VecInt, VecReal, gtp::VecPoint2D, gtp::VecPoint3D
>;

/**
 * @class GeometricDataHolder
 * @brief Stores multiple variables of different types using a map where each variable is identified by a string key.
 * 
 * The class uses std::variant to store different types of data and provides methods to add, retrieve, and print variables.
 * This class is part of the "parameters" group and inherits from the ID class for identification purposes.
 */
class GeometricDataHolder : public ID {
public:
    /**
     * @brief Get the class name.
     *
     * @return A string view representing the name of this class.
     */
    virtual std::string_view className() const noexcept override {
        return "GeometricDataHolder";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
    virtual ClassID classID() const noexcept override {
        return ClassID::GeometricDataHolder;
    }

    /**
     * @brief Adds a value to the GeometricDataHolder.
     * @tparam T The type of the value to be added.
     * @param key The string key associated with the value.
     * @param value The value to be stored.
     * 
     * This method allows you to store any supported data type using a string as the key.
     */
    template<typename T>
    void addValue(const std::string& key, T value);

    /**
     * @brief Retrieves a value from the GeometricDataHolder.
     * @tparam T The type of the value to be retrieved.
     * @param key The string key associated with the value.
     * @return std::optional<T> The value if found and of the correct type, or std::nullopt if not found or of the wrong type.
     * 
     * Use this method to access stored values by their string key and ensure the correct type is returned.
     */
    template<typename T>
    [[nodiscard]] std::optional<T> getValue(const std::string& key) const;

    /**
     * @brief Prints all stored values in the GeometricDataHolder.
     * @param os The output stream to which the values will be printed.
     * @param holder The GeometricDataHolder object containing the values.
     * @return std::ostream& The output stream after printing the values.
     * 
     * This operator overload allows printing all variables stored in the GeometricDataHolder.
     */
    friend std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder);

private:
    /**
     * @brief The internal map storing the variables, where each variable is identified by a string key.
     */
    std::map<std::string, DataType> data_;  // Map to store named variables
};

VORMAKER_NAMESPACE_CLOSE

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.tpp>

        
//==============================================================================
// Typedefs
//==============================================================================

using PtrGeometricDataHolder = std::unique_ptr<vmm::GeometricDataHolder>;
using PtrConstGeometricDataHolder = std::unique_ptr<vmm::GeometricDataHolder const>;
        
#endif // __VORONOIMESHMAKER_DATAHOLDER_H__
