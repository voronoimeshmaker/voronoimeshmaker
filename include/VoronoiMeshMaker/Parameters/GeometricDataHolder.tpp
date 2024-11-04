//==============================================================================
// Name        : GeometricDataHolder.tpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Template implementations for GeometricDataHolder class methods.
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

#ifndef VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_TPP
#define VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_TPP

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Error/FunctionLogger.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Sets a value associated with a given key.
 *
 * @tparam T The type of the value to store.
 * @param key The key to associate the value with.
 * @param value The value to store.
 *
 * This method allows you to store a value in the internal data map of the
 * GeometricDataHolder class, associating it with a given key.
 * If the key already exists, the existing value will be replaced.
 */
template <typename T>
void GeometricDataHolder::set(const std::string_view& key, T value) {
    FunctionLogger functionLogger; 
    data_[std::string(key)] = value;
}

/**
 * @brief Gets a value associated with a given key.
 *
 * @tparam T The type of the value expected.
 * @param key The key associated with the value.
 * @return An optional containing the value if the key exists and the type matches, otherwise std::nullopt.
 *
 * This method retrieves a value from the data map using the provided key.
 * If the key does not exist or if the type of the stored value does not match the expected type,
 * this method returns std::nullopt to ensure type safety and avoid runtime errors.
 */
template <typename T>
std::optional<T> GeometricDataHolder::get(const std::string_view& key) const {
    auto it = data_.find(std::string(key));
    if (it != data_.end()) {
        if (auto val = std::get_if<T>(&it->second)) {
            return *val;
        }
    }
    return std::nullopt;
}

//==============================================================================
// Type Definitions
//==============================================================================
/**
 * @typedef PtrGeometricDataHolder
 * @brief Defines a shared pointer to a GeometricDataHolder.
 *
 * This type definition is useful when multiple entities need to share ownership
 * of a GeometricDataHolder instance, ensuring proper memory management through
 * reference counting.
 */
using PtrGeometricDataHolder = std::shared_ptr<GeometricDataHolder>;

/**
 * @typedef PtrConstGeometricDataHolder
 * @brief Defines a shared pointer to a const GeometricDataHolder.
 *
 * This type definition is used when multiple entities need to share ownership
 * of a const GeometricDataHolder instance, ensuring that none of the shared entities
 * can modify the instance.
 */
using PtrConstGeometricDataHolder = std::shared_ptr<GeometricDataHolder const>;

VORMAKER_NAMESPACE_CLOSE

#endif // VORONOI_MESH_MAKER_PARAMETERS_GEOMETRICDATAHOLDER_TPP
