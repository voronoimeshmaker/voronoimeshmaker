//==============================================================================
// Name        : GeometricDataHolder.tpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.1
// Description : Template implementations for the GeometricDataHolder class.
//               This file contains the template function implementations for
//               the GeometricDataHolder class.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
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
 * @file GeometricDataHolder.tpp
 * @brief Template implementations for the GeometricDataHolder class.
 *
 * This file contains the template function implementations for the GeometricDataHolder class,
 * which is part of the Parameters module of the VoronoiMeshMaker library.
 *
 * @author João Flávio Vieira de Vasconcellos
 * @version 1.1
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Template function definitions for GeometricDataHolder
//==============================================================================

/**
 * @brief Sets a value for a given key.
 *
 * This function allows setting a value of any type supported by the GeometricDataHolder.
 *
 * @tparam T The type of the value to be stored.
 * @param key The key associated with the value.
 * @param value The value to be stored.
 */
template <typename T>
void GeometricDataHolder::set(std::string_view key, T value) {
    std::unique_lock lock(mutex_);
    data_[std::string(key)] = value;
}

/**
 * @brief Gets the value associated with a given key.
 *
 * This function attempts to retrieve the value associated with a given key.
 * If the key exists and the type matches, it returns the value wrapped in an optional.
 * Otherwise, it returns std::nullopt.
 *
 * @tparam T The expected type of the value.
 * @param key The key whose associated value is to be returned.
 * @return An optional containing the value if the key exists and the type matches, otherwise std::nullopt.
 */
template <typename T>
[[nodiscard]] std::optional<T> GeometricDataHolder::get(std::string_view key) const {
    std::shared_lock lock(mutex_);
    auto it = data_.find(std::string(key));
    if (it != data_.end()) {
        if (auto val = std::get_if<T>(&it->second)) {
            return *val;
        }
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder) {
        std::shared_lock lock(holder.mutex_);
        std::vector<std::pair<std::string, std::variant<int, double, std::string>>> sorted_data(holder.data_.begin(), holder.data_.end());
        
        std::sort(sorted_data.begin(), sorted_data.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        size_t max_key_length = 0;
        for (const auto& [key, _] : sorted_data) {
            max_key_length = std::max(max_key_length, key.size());
        }

        for (const auto& [key, value] : sorted_data) {
            os << std::left << std::setw(max_key_length + 2) << key << ": ";
            std::visit([&os](const auto& val) { os << val; }, value);
            os << '
';
        }
        return os;
    }

VORMAKER_NAMESPACE_CLOSE
