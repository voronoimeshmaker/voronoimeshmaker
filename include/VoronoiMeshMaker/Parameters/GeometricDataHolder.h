//==============================================================================
// Name        : GeometricDataHolder.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definition of the GeometricDataHolder class.
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

#ifndef VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_H
#define VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_H

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================
#include <map>
#include <variant>
#include <string_view>

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class GeometricDataHolder
 * @brief A class to hold geometric data in a flexible manner.
 *
 * The GeometricDataHolder class allows for storing various types of geometric data,
 * such as integers, doubles, strings, and CGAL geometric types, using a string key
 * for access.
 *
 * The class provides methods to set and get data of different types. The stored data
 * can be retrieved by providing the corresponding key and specifying the expected type.
 */
class GeometricDataHolder {
public:
    /**
     * @brief Default constructor.
     */
    GeometricDataHolder() = default;

    /**
     * @brief Copy constructor.
     * 
     * Initializes a new instance of GeometricDataHolder by copying the data from another instance.
     *
     * @param other The other GeometricDataHolder to copy from.
     */
    GeometricDataHolder(const GeometricDataHolder& other) noexcept = default;

    /**
     * @brief Move constructor.
     * 
     * Initializes a new instance of GeometricDataHolder by moving the data from another instance.
     *
     * @param other The other GeometricDataHolder to move from.
     */
    GeometricDataHolder(GeometricDataHolder&& other) noexcept = default;

    /**
     * @brief Copy assignment operator.
     * 
     * Assigns the data from another GeometricDataHolder to this instance.
     *
     * @param other The other GeometricDataHolder to copy from.
     * @return Reference to the updated GeometricDataHolder.
     */
    GeometricDataHolder& operator=(const GeometricDataHolder& other) = default;

    /**
     * @brief Move assignment operator.
     * 
     * Moves the data from another GeometricDataHolder to this instance.
     *
     * @param other The other GeometricDataHolder to move from.
     * @return Reference to the updated GeometricDataHolder.
     */
    GeometricDataHolder& operator=(GeometricDataHolder&& other) noexcept = default;

    /**
     * @brief Sets a value associated with a given key.
     *
     * @tparam T The type of the value to store.
     * @param key The key to associate the value with.
     * @param value The value to store.
     *
     * Stores a value in the data map, associated with a key. The key is a string
     * that can be used to retrieve the value later. If the key already exists,
     * the existing value will be replaced.
     */
    template <typename T>
    void set(const std::string_view& key, T value);

    /**
     * @brief Gets a value associated with a given key.
     *
     * @tparam T The type of the value expected.
     * @param key The key associated with the value.
     * @return An optional containing the value if the key exists and the type matches, otherwise std::nullopt.
     *
     * This method retrieves a value from the data map using the key. If the key does not
     * exist or the type does not match, it returns std::nullopt.
     */
    template <typename T>
    std::optional<T> get(const std::string_view& key) const;

    /**
     * @brief Overloaded output stream operator for GeometricDataHolder.
     *
     * Outputs the contents of GeometricDataHolder to the provided output stream
     * in a readable format, primarily for debugging purposes.
     *
     * @param os The output stream.
     * @param holder The GeometricDataHolder instance to be outputted.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder);

private:
    /**
     * @brief Storage map for data.
     *
     * The data map stores values of various types, accessible via string keys.
     * Supported types include:
     * - int
     * - Real (defined in VoronoiMeshMaker type.h)
     * - std::string
     * - gtp::Point2D, gtp::Vector2D, gtp::Point3D, gtp::Vector3D
     * - std::list<int>, std::list<Real>
     *
     * Each entry in the map is associated with a key that allows for easy retrieval
     * and modification of the stored values.
     */
    std::map    <std::string, std::variant  <   int
                                            ,   Real
                                            ,   std::string
                                            ,   gtp::Point2D
                                            ,   gtp::Vector2D
                                            ,   gtp::Point3D
                                            ,   gtp::Vector3D
                                            ,   LstInt
                                            ,   LstReal
                                            >
                > data_;
};


VORMAKER_NAMESPACE_CLOSE

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.tpp>

#endif // VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_H
