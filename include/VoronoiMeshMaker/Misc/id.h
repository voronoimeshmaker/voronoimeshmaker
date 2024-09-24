//==============================================================================
// Name        : ID.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Header file for identifying each class in the library.
//               Part of the 'misc' group of utility files.
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
 * @file ID.h
 * @brief Defines the base class `ID` for class identification and the `ClassID` enum for unique class identification.
 * 
 * This file provides a base class that allows each derived class in the library to have a unique
 * identifier and class name, enabling easier comparisons and object identification.
 * 
 * The `ClassID` enum allows each class to have a unique identifier, while the `ID` class provides
 * the infrastructure for storing and comparing these identifiers.
 * 
 * @ingroup misc
 * @version 1.0
 * @date 2024
 * 
 * Licensed under the GNU General Public License, version 3.
 */

#ifndef __VORONOMESHVMAKER_ID_H__
#define __VORONOMESHVMAKER_ID_H__

#include <string_view>
#include <VoronoiMeshMaker/Misc/configure.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup misc
 * @brief Enumeration for uniquely identifying each class in the VoronoiMeshMaker library.
 * 
 * This enum is used to assign a unique identifier (ID) to each class, allowing
 * for easy comparison and distinction between classes.
 */
enum class ClassID {
    None,                 ///< Represents no class or an uninitialized class.
    GeometricDataHolder,  ///< Identifier for the GeometricDataHolder class.
    Shape2D,  ///< Identifier for the Shape2D class.
    MarkerException       ///< Identifier for the MarkerException class.
    // Add other class IDs here as needed.
};

/**
 * @ingroup misc
 * @brief Base class to provide unique identification for derived classes.
 * 
 * The `ID` class serves as a base class for all objects that require unique identification.
 * It provides methods to retrieve the class name and class ID, which are then used
 * to compare objects or retrieve metadata about them.
 */
class ID {
public:
    /**
     * @brief Virtual destructor to allow proper cleanup in derived classes.
     * 
     * Ensures that resources in derived classes are properly released when
     * an object is destroyed.
     */
    virtual ~ID() = default;

    /**
     * @brief Retrieves the name of the class.
     * 
     * This function returns the name of the class as a string view.
     * By default, it returns "no class" if not overridden by derived classes.
     * 
     * @return A string view representing the class name.
     */
    virtual std::string_view className() const noexcept {
        return "no class";
    }

    /**
     * @brief Retrieves the unique identifier (ID) for the class.
     * 
     * This function returns the unique identifier for the class, represented
     * by the `ClassID` enum. By default, it returns `ClassID::None`.
     * 
     * @return The unique class identifier as a `ClassID` enum value.
     */
    virtual ClassID classID() const noexcept {
        return ClassID::None;
    }

    /**
     * @brief Compares if two objects are of the same class.
     * 
     * This function compares the class ID of the current object with
     * another object that also inherits from `ID`. If the class IDs are the same,
     * the two objects are considered to be of the same class.
     * 
     * @param other The `ID` object to compare against.
     * @return True if both objects have the same class ID, false otherwise.
     */
    bool sameClass(const ID& other) const noexcept {
        return classID() == other.classID();
    }
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_ID_H__
