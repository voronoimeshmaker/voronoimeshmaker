//==============================================================================
// Name        : GeometricDataHolder.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Implementation of the GeometricDataHolder class methods.
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
 * @file GeometricDataHolder.cpp
 * @brief Implementation of the GeometricDataHolder class methods.
 *
 * This file contains the implementation of the methods of the GeometricDataHolder class,
 * which is part of the Parameters module of the VoronoiMeshMaker library. The implementation
 * provides thread-safe storage and retrieval of various geometric data types, including CGAL types.
 *
 * The file also defines the overloaded stream output operator for printing the contents of
 * GeometricDataHolder in a readable format, with values sorted by key.
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

std::ostream& operator<<(std::ostream& os, GeometricDataHolder& holder) {
    std::shared_lock lock(holder.getMutex());

    // Sort the data by key for ordered output
    std::vector<std::pair<std::string, std::variant<int, Real, std::string,
                                                     gtp::Point2D, gtp::Vector2D,
                                                     gtp::Point3D, gtp::Vector3D,
                                                     std::list<int>, std::list<Real>>>> sorted_data(holder.data_.begin(), holder.data_.end());

    std::sort(sorted_data.begin(), sorted_data.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    // Determine the maximum key length for alignment
    size_t max_key_length = 0;
    for (const auto& [key, _] : sorted_data) {
        max_key_length = std::max(max_key_length, key.length());
    }

    // Print each key-value pair in a formatted manner
    for (const auto& [key, value] : sorted_data) {
        os << std::left << std::setw(max_key_length + 2) << key << ": ";
        std::visit([
            &os
        ](const auto& val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::list<int>> || std::is_same_v<T, std::list<Real>>) {
                os << "[ ";
                for (const auto& elem : val) {
                    os << elem << " ";
                }
                os << "]";
            } else {
                os << val;
            }
        }, value);
        os << '\n';
    }

    return os;
}

// Explicit template instantiations for CGAL types

// Point_2
template void GeometricDataHolder::set<gtp::Point2D>(std::string_view, gtp::Point2D);
template std::optional<gtp::Point2D> GeometricDataHolder::get<gtp::Point2D>(std::string_view) const;

// Vector_2
template void GeometricDataHolder::set<gtp::Vector2D>(std::string_view, gtp::Vector2D);
template std::optional<gtp::Vector2D> GeometricDataHolder::get<gtp::Vector2D>(std::string_view) const;

// Point_3
template void GeometricDataHolder::set<gtp::Point3D>(std::string_view, gtp::Point3D);
template std::optional<gtp::Point3D> GeometricDataHolder::get<gtp::Point3D>(std::string_view) const;

// Vector_3
template void GeometricDataHolder::set<gtp::Vector3D>(std::string_view, gtp::Vector3D);
template std::optional<gtp::Vector3D> GeometricDataHolder::get<gtp::Vector3D>(std::string_view) const;

// List of integers
template void GeometricDataHolder::set<std::list<int>>(std::string_view, std::list<int>);
template std::optional<std::list<int>> GeometricDataHolder::get<std::list<int>>(std::string_view) const;

// List of Reals
template void GeometricDataHolder::set<std::list<Real>>(std::string_view, std::list<Real>);
template std::optional<std::list<Real>> GeometricDataHolder::get<std::list<Real>>(std::string_view) const;

VORMAKER_NAMESPACE_CLOSE