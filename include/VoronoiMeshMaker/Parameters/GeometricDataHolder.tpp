//==============================================================================
// Name        : GeometricDataHolder.tpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Template implementations for the GeometricDataHolder class.
//               This file contains the template function implementations for
//               the GeometricDataHolder class.
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
 * @file GeometricDataHolder.tpp
 * @brief Template implementations for the GeometricDataHolder class.
 * This file contains the template function implementations for the GeometricDataHolder class.
 *
 * @ingroup parameters
 * 
 * This file defines the methods used to add and retrieve values from the GeometricDataHolder class.
 * It provides template specializations for the supported data types.
 * 
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

VORMAKER_NAMESPACE_OPEN

// Remove Shape2D or any specific geometric shapes from GeometricDataHolder.
// From now on, geometric shapes will not be handled within this holder.
template<typename T>
void GeometricDataHolder::addValue(const std::string& key, T value) {
    data_[key] = value;
}

template<typename T>
std::optional<T> GeometricDataHolder::getValue(const std::string& key) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        if (auto pval = std::get_if<T>(&(it->second))) {
            return *pval;
        }
    }
    return std::nullopt;  // Return nullopt if the type or key is invalid
}

// Parallelize the visit operation using parallel algorithms in case of large data
inline std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder) {
    std::for_each(
        std::execution::par, holder.data_.begin(), holder.data_.end(),
        [&os](const auto& pair) {
            os << pair.first << ": ";
            std::visit([&os](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::list<int>> || std::is_same_v<T, std::list<Real>> ||
                              std::is_same_v<T, std::list<vmm::gtp::Point2D>> || std::is_same_v<T, std::list<vmm::gtp::Point3D>>) {
                    os << "[ ";
                    for (const auto& elem : arg) {
                        os << elem << " ";
                    }
                    os << "]";
                } else if constexpr (std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<Real>> ||
                                     std::is_same_v<T, std::vector<vmm::gtp::Point2D>> || std::is_same_v<T, std::vector<vmm::gtp::Point3D>>) {
                    os << "[ ";
                    for (const auto& elem : arg) {
                        os << elem << " ";
                    }
                    os << "]";
                } else {
                    os << arg;
                }
            }, pair.second);
            os << std::endl;
        }
    );
    return os;
}

VORMAKER_NAMESPACE_CLOSE
