//==============================================================================
// Name        : PolygonCSV.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Definition of the PolygonCSV class for reading polygon 
//               coordinates from a CSV (Comma-Separated Values) file.
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
//                                      (jflavio at iprj.uerj.br)
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
//               along with this program. If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

#ifndef __VORONOMESHVMAKER_POLYGONCSV_H__
#define __VORONOMESHVMAKER_POLYGONCSV_H__

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================
#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @class PolygonCSV
 * @brief Class for handling two-dimensional polygon surfaces read from a CSV file.
 * 
 * This class is responsible for reading polygon coordinates from a CSV file and
 * using them to generate the corresponding polygon representation.
 * 
 * @ingroup Shape2D
 */
class PolygonCSV : public Shape2D
{
public:
    /**
     * @brief Default constructor.
     */
    PolygonCSV() noexcept = default;

    /**
     * @brief Copy constructor.
     */
    PolygonCSV(const PolygonCSV&) noexcept = default;

    /**
     * @brief Move constructor.
     */
    PolygonCSV(PolygonCSV&&) noexcept = default;

    /**
     * @brief Constructs a PolygonCSV object using a file name.
     *
     * @param _filename The name of the CSV file containing the polygon coordinates.
     */
    explicit PolygonCSV(const std::string& _filename);

    /**
     * @brief Destructor.
     */
    virtual ~PolygonCSV() noexcept = default;

    /**
     * @brief Reads the CSV file to extract polygon coordinates.
     * 
     * This method reads the CSV file specified by the filename and processes
     * the polygon coordinates contained within.
     */
    void readCSV();

    /**
     * @brief Get the class name.
     * 
     * @return A string view representing the name of this class.
     */
    virtual std::string_view className() const noexcept override {
        return "PolygonCSV";
    }

    /**
     * @brief Get the class ID.
     * 
     * @return A ClassID enum representing the unique ID of this class.
     */
    virtual ClassID classID() const noexcept override {
        return ClassID::PolygonCSV;
    }

private:
    
    std::string                 filename_; ///< Name of the CSV file containing polygon coordinates.
    gtp::PtrPolygon2DShared     ptrBoundaryPolygon_ = nullptr; ///< Shared pointer to the polygon representation of the boundary.

};

//==============================================================================
// Typedefs
//==============================================================================

/**
 * @typedef PtrPolygonCSV
 * @brief Defines a shared pointer to a PolygonCSV object.
 * 
 * This typedef is useful for managing the shared ownership of PolygonCSV instances.
 */
using PtrPolygonCSVShared = std::shared_ptr<PolygonCSV>;

/**
 * @typedef PtrConstPolygonCSV
 * @brief Defines a shared pointer to a const PolygonCSV object.
 * 
 * This typedef is used when the shared ownership of a constant PolygonCSV instance is required.
 */
using PtrConstPolygonCSVShared = std::shared_ptr<const PolygonCSV>;

VORMAKER_NAMESPACE_CLOSE

#endif /* __VORONOMESHVMAKER_POLYGONCSV_H__ */
