//==============================================================================
// Name        : ID.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file for identifying each class in the library.
//               This file is part of the 'misc' group of utility files.
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

#ifndef __VORONOMESHVMAKER_ID_H__
#define __VORONOMESHVMAKER_ID_H__

//==============================================================================
//  c++ include
//==============================================================================

#include <string_view>

//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/configure.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @defgroup misc Miscellaneous Utilities
 * @brief This group contains utility classes and functions used across the library.
 *
 * The 'misc' group is composed of various utility components that aid in
 * the general functionality of the library, including base classes and common definitions.
 */

/**
 * @ingroup misc
 * @brief Base class to provide identity functionality to derived classes.
 * 
 * This class offers an interface for derived classes to implement methods
 * that return a unique class name and ID, allowing them to be identified
 * and compared.
 */
class ID {
public:
    /**
     * @brief Virtual destructor.
     * 
     * Ensures proper cleanup of resources when derived classes are destroyed.
     */
    virtual ~ID() = default;

    /**
     * @brief Get the class name.
     * 
     * @return A string view representing the name of the class.
     *         The default implementation returns "no class".
     */
    virtual std::string_view className() const noexcept {
        return "no class";
    }

    /**
     * @brief Get the class ID.
     * 
     * @return A ClassID enum representing the unique ID of the class.
     *         The default implementation returns ClassID::None.
     */
    virtual ClassID classID() const noexcept {
        return ClassID::None;
    }

    /**
     * @brief Compare if two classes are the same.
     * 
     * Compares the class ID of this object with another ID object.
     * 
     * @param other The ID object to compare with.
     * @return True if both objects have the same class ID, false otherwise.
     */
    bool sameClass(const ID& other) const noexcept {
        return classID() == other.classID();
    }
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_ID_H__
