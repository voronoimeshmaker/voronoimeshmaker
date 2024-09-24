//==============================================================================
// Name        : ShapeFactory.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of ShapeFactory class.
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

#include <VoronoiMeshMaker/Geometry/ShapeFactory.h>

VORMAKER_NAMESPACE_OPEN

template<typename T>
size_t ShapeFactory::getKey() {
    return typeid(T).hash_code();  // Get a unique key based on the type
}

template<typename T>
void ShapeFactory::registerShape() {
    registry_[getKey<T>()] = [](const GeometricDataHolder& data) -> std::unique_ptr<Shape> {
        return std::make_unique<T>(data);
    };
}

template<int Dimension, typename ShapeType>
std::unique_ptr<Shape> ShapeFactory::createShape(const GeometricDataHolder& data) {
    size_t key = getKey<ShapeType>();
    auto it = registry_.find(key);

    if (it == registry_.end()) {
        throw ShapeException("Shape not registered.");
    }

    return it->second(data);  // Call the corresponding constructor
}

VORMAKER_NAMESPACE_CLOSE
