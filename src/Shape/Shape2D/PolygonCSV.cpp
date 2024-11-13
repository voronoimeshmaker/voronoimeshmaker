//==============================================================================
// Name        : PolygonCSV.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Implements the PolygonCSV class for reading polygon vertex
//               coordinates from a CSV file and manipulating them using CGAL.
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
 * @file PolygonCSV.cpp
 * @brief Implements the PolygonCSV class, which reads polygon vertex coordinates
 *        from a CSV file and creates a 2D polygon using CGAL.
 *
 * The PolygonCSV class reads a CSV file containing 2D coordinates, verifies if
 * the points form a valid polygon, and adjusts the orientation if necessary to
 * ensure the polygon is oriented clockwise.
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
// Includes from C++ Standard Library
//==============================================================================

#include <filesystem>
//#include <fstream>
//#include <sstream>
//#include <stdexcept>
//#include <string_view>
//#include <vector>
namespace fs = std::filesystem;

//==============================================================================
// Includes from VoronoiMeshMaker Library
//==============================================================================

#include <VoronoiMeshMaker/Misc/Misc.h>
#include <VoronoiMeshMaker/Shape/Shape2D/PolygonCSV.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>

VORMAKER_NAMESPACE_OPEN

using gtp::Point2D;
using gtp::Polygon2D;
using gtp::VecPoint2D;

//==============================================================================
// Constructor
//==============================================================================

/**
 * @brief Constructor that initializes PolygonCSV with the directory and filename.
 *
 * This constructor reads the CSV file from the specified directory and filename,
 * generates the polygon boundary, and validates the polygon.
 *
 * @param directory The directory containing the CSV file.
 * @param filename The name of the CSV file.
 */

PolygonCSV::PolygonCSV(const GeometricDataHolder& _holder) : Shape2D(_holder)
{

//    PolygonCSV::PolygonCSV(const std::string_view& directory,
//                       const std::string_view& filename)
//    : directory_(directory), filename_(filename) {
//    try {
//        if (!GenerateBoundary2D()) {
//            throw MakerException(
//                MakerErrorCode::InvalidPolygon,
//                "Failed to generate boundary from the CSV file.",
//                std::source_location::current()
//            );
//        }
//    } catch (const MakerException& e) {
//        std::cerr << "Exception: " << e.what() << "\n";
//        std::exit(EXIT_FAILURE);
//    }
//
//    if (!ptrPolygon2DShared_ || ptrPolygon2DShared_->size() < 3) {
//        std::cerr << "Polygon boundary test failed: insufficient points.\n";
//        std::exit(EXIT_FAILURE);
//    }
}

//==============================================================================
// Generate the boundary by reading from CSV
//==============================================================================

/**
 * @brief Generates the polygon boundary by reading the CSV file.
 *
 * This function reads the CSV file and creates the polygon boundary. If the
 * boundary cannot be generated, it returns false.
 *
 * @return True if the boundary was successfully generated, false otherwise.
 */
bool PolygonCSV::GenerateBoundary2D() {
    ReadCSV();

    if (!ptrPolygon2DShared_ || ptrPolygon2DShared_->size() == 0) {
        return false;
    }
    return true;
}

//==============================================================================
// Reads the CSV file and creates the polygon
//==============================================================================

/**
 * @brief Reads the vertex coordinates from the CSV file and creates the polygon.
 *
 * This function reads the CSV file line by line, extracts the coordinates, and
 * creates a polygon using CGAL. If the polygon is counterclockwise oriented,
 * it reverses the orientation to make it clockwise.
 */
void PolygonCSV::ReadCSV() {
    auto split = [](const std::string& s, char delimiter) -> std::vector<std::string> {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    };

    std::string line;
    std::ifstream input;
    VecPoint2D lstPoint2D;

    try {
        fs::path filePath = fs::path(directory_) / filename_;

        if (!fs::exists(filePath)) {
            throw MakerException(
                MakerErrorCode::FileNotFound,
                "File not found: " + filePath.string(),
                std::source_location::current()
            );
        }

        input.open(filePath);

        if (!input.is_open()) {
            throw MakerException(
                MakerErrorCode::InvalidPolygon,
                "Unable to open file: " + filePath.string(),
                std::source_location::current()
            );
        }

        while (std::getline(input, line)) {
            if (!IsEmptyOrBlank(line)) {
                auto tokens = split(line, ',');
                if (tokens.size() != 2) {
                    throw MakerException(
                        MakerErrorCode::InvalidPolygon,
                        "Invalid line format: " + line,
                        std::source_location::current()
                    );
                }

                lstPoint2D.emplace_back(std::stod(tokens[0]), std::stod(tokens[1]));
            }
        }

    } catch (const MakerException& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        std::exit(EXIT_FAILURE);
    }

    input.close();

    ptrPolygon2DShared_ = std::make_shared<Polygon2D>(
        std::begin(lstPoint2D), std::end(lstPoint2D)
    );
    lstPoint2D.clear();

    if (ptrPolygon2DShared_->is_counterclockwise_oriented()) {
        ptrPolygon2DShared_->reverse_orientation();
    }
}


// Register the Rectangle shape in the ShapeFactory
const bool registeredPolygonCSV = []{
    FunctionLogger logger;
    ShapeFactory::registerShape<vmm::PolygonCSV>();
    return true;
}();

VORMAKER_NAMESPACE_CLOSE
