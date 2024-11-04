//==============================================================================
// Name        : GeometricDataHolder.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Implementation of methods for GeometricDataHolder class.
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
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Overloaded output stream operator for GeometricDataHolder.
 * 
 * This operator allows the contents of GeometricDataHolder to be printed
 * in a readable format, primarily for debugging purposes.
 * 
 * The keys are sorted alphabetically to provide consistent output.
 * The output aligns keys and values for easier readability.
 * 
 * @param os The output stream to which the data will be printed.
 * @param holder The GeometricDataHolder instance to be printed.
 * @return Reference to the output stream.
 */
std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder) {
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
        std::visit([&os](const auto& val) {
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

/**
 * @brief Explicit instantiation for setting a Point2D value.
 */
template void GeometricDataHolder::set<gtp::Point2D>(const std::string_view&, gtp::Point2D);

/**
 * @brief Explicit instantiation for getting a Point2D value.
 */
template std::optional<gtp::Point2D> GeometricDataHolder::get<gtp::Point2D>(const std::string_view&) const;

/**
 * @brief Explicit instantiation for setting a Vector2D value.
 */
template void GeometricDataHolder::set<gtp::Vector2D>(const std::string_view&, gtp::Vector2D);

/**
 * @brief Explicit instantiation for getting a Vector2D value.
 */
template std::optional<gtp::Vector2D> GeometricDataHolder::get<gtp::Vector2D>(const std::string_view&) const;

/**
 * @brief Explicit instantiation for setting a Point3D value.
 */
template void GeometricDataHolder::set<gtp::Point3D>(const std::string_view&, gtp::Point3D);

/**
 * @brief Explicit instantiation for getting a Point3D value.
 */
template std::optional<gtp::Point3D> GeometricDataHolder::get<gtp::Point3D>(const std::string_view&) const;

/**
 * @brief Explicit instantiation for setting a Vector3D value.
 */
template void GeometricDataHolder::set<gtp::Vector3D>(const std::string_view&, gtp::Vector3D);

/**
 * @brief Explicit instantiation for getting a Vector3D value.
 */
template std::optional<gtp::Vector3D> GeometricDataHolder::get<gtp::Vector3D>(const std::string_view&) const;

/**
 * @brief Explicit instantiation for setting a list of integers.
 */
template void GeometricDataHolder::set<LstInt>(const std::string_view&, LstInt);

/**
 * @brief Explicit instantiation for getting a list of integers.
 */
template std::optional<LstInt> GeometricDataHolder::get<LstInt>(const std::string_view&) const;

/**
 * @brief Explicit instantiation for setting a list of Real values.
 */
template void GeometricDataHolder::set<LstReal>(const std::string_view&, LstReal);

/**
 * @brief Explicit instantiation for getting a list of Real values.
 */
template std::optional<LstReal> GeometricDataHolder::get<LstReal>(const std::string_view&) const;

VORMAKER_NAMESPACE_CLOSE
