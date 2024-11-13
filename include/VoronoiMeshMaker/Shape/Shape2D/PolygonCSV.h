#ifndef __VORONOMESHVMAKER_POLYGONCSV_H__
#define __VORONOMESHVMAKER_POLYGONCSV_H__

//==============================================================================
// Name        : PolygonCSV.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Class for reading the vertices of a polygon
//               in CSV format and manipulating the data using CGAL.
//
// Copyright   : Copyright (C) 2024 João Flávio Vieira de Vasconcellos
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
//==============================================================================

/**
 * @file PolygonCSV.h
 * @brief Declares the PolygonCSV class for reading polygon vertex coordinates
 *        in CSV format and manipulating them using CGAL.
 *
 * The PolygonCSV class is responsible for loading the vertices from a CSV file,
 * verifying if the points form a valid polygon, and adjusting the orientation,
 * if necessary, to ensure that the polygon is oriented clockwise.
 *
 * @ingroup Shape2D
 * @version 1.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 *
 * Licensed under the GNU General Public License, version 3.
 */

//==============================================================================
// C++ includes
//==============================================================================

#include <memory>
#include <string>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup Shape2D
 * @class PolygonCSV
 * @brief Class to represent a two-dimensional polygon from CSV data.
 *
 * The PolygonCSV class allows reading the vertices of a polygon from a CSV
 * file. It uses CGAL to verify if the points form a valid polygon and adjusts
 * the orientation to ensure it is clockwise.
 */
class PolygonCSV : public Shape2D {

//==============================================================================
// Class ID
//==============================================================================    
    
public:       
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
    static ClassID getClassID() {
        return ClassID::PolygonCSV; 
    }        
    //==============================================================================
    // Constructors / Destructor
    //==============================================================================

public:
    PolygonCSV() noexcept = default;
    PolygonCSV(const PolygonCSV&) noexcept = default;
    virtual ~PolygonCSV() noexcept = default;

    /**
     * @brief Constructor that receives the directory and filename of the CSV.     *
     */
    PolygonCSV(const GeometricDataHolder&);

    //==============================================================================
    // Operator Overloads
    //==============================================================================

public:
    PolygonCSV& operator=(const PolygonCSV&) noexcept = default;

    //==============================================================================
    // Public Functions
    //==============================================================================

private:
    [[nodiscard]] bool GenerateBoundary2D();

    /**
     * @brief Reads the vertex coordinates from the CSV file.
     */
    void ReadCSV();

    //==============================================================================
    // Class Variables
    //==============================================================================

private:
    std::string directory_;  ///< Directory of the CSV file.
    std::string filename_;   ///< Filename of the CSV file.
};

//==============================================================================
// Typedefs
//==============================================================================

using PtrPolygonCSV = std::shared_ptr<PolygonCSV>;
using PtrConstPolygonCSV = std::shared_ptr<const PolygonCSV>;

VORMAKER_NAMESPACE_CLOSE

#endif /* __VORONOMESHVMAKER_POLYGONCSV_H__ */
