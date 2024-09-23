/**
 * @file GeometricDataHolder.h
 * @brief Defines the GeometricDataHolder class and the DataTraits for handling multiple data types.
 * This class uses std::variant to store different types and allows access through string keys.
 * The DataTraits structure provides type identification for the supported data types.
 *
 * @ingroup parameters
 * 
 * This file is part of the "parameters" group, which defines classes and traits that handle
 * multiple types of parameters for computational geometry and other operations.
 * 
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOIMESHMAKER_DATAHOLDER_H__
#define __VORONOIMESHMAKER_DATAHOLDER_H__

#include <iostream>
#include <memory>
#include <variant>
#include <map>
#include <optional>

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/type.h>  
#include <VoronoiMeshMaker/Parameters/GeometricDataTraits.h>


VORMAKER_NAMESPACE_OPEN

    /**
     * @typedef DataType
     * @brief Defines the variant type that can store different data types, such as int, Real, and geometric points.
     */
    using DataType = std::variant<int, Real, gtp::Point2D, gtp::Point3D, gtp::Vector2D, gtp::Vector3D>;

    /**
     * @class GeometricDataHolder
     * @brief Stores multiple variables of different types using a map where each variable is identified by a string key.
     * 
     * The class uses std::variant to store different types of data and provides methods to add, retrieve, and print variables.
     * This class is part of the "parameters" group.
     */
    class GeometricDataHolder {
    public:
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
        std::optional<T> getValue(const std::string& key) const;

        /**
         * @brief Prints all stored values in the GeometricDataHolder.
         * @param os The output stream to which the values will be printed.
         * @param holder The GeometricDataHolder object containing the values.
         * @return std::ostream& The output stream after printing the values.
         * 
         * This operator overload allows printing all variables stored in the GeometricDataHolder.
         */
        friend std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder);

        /**
         * @brief Prints the types of all stored values in the GeometricDataHolder.
         * 
         * This method prints the type of each variable stored in the map using the DataTraits structure.
         */
        void printTypes() const;

    private:
        /**
         * @brief The internal map storing the variables, where each variable is identified by a string key.
         */
        std::map<std::string, DataType> data_;  // Map to store named variables
    };


VORMAKER_NAMESPACE_CLOSE

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.tpp>


#endif // __VORONOIMESHMAKER_DATAHOLDER_H__
