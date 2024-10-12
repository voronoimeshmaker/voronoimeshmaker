//==============================================================================
// Name        : GeometricDataHolder.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Defines the GeometricDataHolder class and the DataTraits 
//               for handling multiple data types.
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

/**
 * @file GeometricDataHolder.h
 * @brief Defines the GeometricDataHolder class and DataTraits for handling multiple data types.
 *
 * This file contains the definition of the GeometricDataHolder class, which is part
 * of the Parameters module of the VoronoiMeshMaker library. The class provides a
 * flexible mechanism to store and manage geometric parameters, supporting multiple
 * data types such as integers, doubles, strings, and CGAL geometric types.
 *
 * The class uses a map to store data in a key-value pair format, with thread-safe
 * access using shared and unique locks. It also provides explicit template instantiations
 * for CGAL types like Point_2, Vector_2, Point_3, and Vector_3.
 *
 * @author João Flávio Vieira de Vasconcellos
 * @version 1.1
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

#ifndef VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_HOLDER_H
#define VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_HOLDER_H

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================

#include <shared_mutex>

//==============================================================================
// Namespace for VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class GeometricDataHolder
 * @brief Class to store and manage geometric parameters using a flexible data structure.
 *
 * This class allows storing various types of geometric data using a key-value approach,
 * where keys are strings and values can be of multiple data types, such as integers,
 * doubles, strings, CGAL points, and vectors.
 */
class GeometricDataHolder {
public:
    GeometricDataHolder() = default;
    GeometricDataHolder(const GeometricDataHolder&) = delete;
    GeometricDataHolder& operator=(const GeometricDataHolder&) = delete;
    GeometricDataHolder(GeometricDataHolder&&) noexcept = default;
    GeometricDataHolder& operator=(GeometricDataHolder&&) noexcept = default;
    ~GeometricDataHolder() = default;

    /**
     * @brief Sets a value for a given key.
     *
     * @tparam T The type of the value.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     */
    template <typename T>
    void set(std::string_view key, T value) {
        std::unique_lock lock(mutex_);
        data_[std::string(key)] = value;
    }

    /**
     * @brief Gets the value associated with a given key.
     *
     * @tparam T The expected type of the value.
     * @param key The key whose associated value is to be returned.
     * @return An optional containing the value if the key exists and the type matches, otherwise std::nullopt.
     */
    template <typename T>
    [[nodiscard]] std::optional<T> get(std::string_view key) const {
        std::shared_lock lock(mutex_);
        auto it = data_.find(std::string(key));
        if (it != data_.end()) {
            if (auto val = std::get_if<T>(&it->second)) {
                return *val;
            }
        }
        return std::nullopt;
    }

    /**
     * @brief Gets the mutex for thread-safe operations.
     *
     * @return A reference to the shared mutex.
     */
    std::shared_mutex& getMutex() { return mutex_; }
    const std::shared_mutex& getMutex() const { return mutex_; }

    /**
     * @brief Friend function to allow streaming the GeometricDataHolder to an output stream.
     *
     * @param os The output stream.
     * @param holder The GeometricDataHolder to be streamed.
     * @return A reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, GeometricDataHolder& holder);

private:
    
    std::map<std::string, std::variant<int, Real, std::string,
                                       gtp::Point2D, gtp::Vector2D,
                                       gtp::Point3D, gtp::Vector3D,
                                       std::list<int>, std::list<Real>>> data_;
    mutable std::shared_mutex mutex_;
};

//==============================================================================
// Type Definitions
//==============================================================================

using PtrGeometricDataHolder = std::shared_ptr<GeometricDataHolder>;
using PtrConstGeometricDataHolder = std::shared_ptr<GeometricDataHolder const>;

VORMAKER_NAMESPACE_CLOSE

#endif // VORONOI_MESH_MAKER_PARAMETERS_GEOMETRIC_DATA_HOLDER_H
