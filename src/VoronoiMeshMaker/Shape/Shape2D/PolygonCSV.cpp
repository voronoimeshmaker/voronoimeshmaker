//==============================================================================
// Name        : PolygonCSV.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of PolygonCSV class methods for reading polygon
//               coordinates from a CSV (Comma-Separated Values) file.
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
//                                      (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================

#include <filesystem>
#include <fstream>
#include <sstream>


namespace fs = std::filesystem;

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/PolygonCSV.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Misc/misc.h>
#include <VoronoiMeshMaker/Misc/type.h>

VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Constructor Implementation
//==============================================================================

/**
 * @brief Constructs a PolygonCSV object using a file name.
 * 
 * This constructor initializes the PolygonCSV object based on the given CSV file name.
 *
 * @param _filename The name of the CSV file containing the polygon coordinates.
 */
PolygonCSV::PolygonCSV(const std::string& _filename) : filename_(_filename) {
    
        readCSV();

}

//==============================================================================
// Public Method Implementation
//==============================================================================

/**
 * @brief Reads the CSV file to extract polygon coordinates.
 * 
 * This method reads the CSV file specified by the filename and processes
 * the polygon coordinates contained within.
 */
void PolygonCSV::readCSV() {
    
    
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
    gtp::LstPoint2D lstPoint2D;

    try {
        if (!fs::exists(filename_)) {
            throw VMMException(SourceInfo(__FILE__, __LINE__, __func__), VMMExceptionIndex::FileNotFound, "Filename: " + filename_, true);
        }

        input.open(filename_);

        if (!input.is_open()) {
            throw VMMException(SourceInfo(__FILE__, __LINE__, __func__), VMMExceptionIndex::FileNotFound, "Filename: " + filename_, true);
        }

        while (std::getline(input, line)) {
            if (!IsEmptyOrBlank(line)) {
                auto tokens = split(line, ',');
                if (tokens.size() != 2) {
                    throw std::runtime_error("Invalid line format: " + line);
                }
                
                lstPoint2D.emplace_back(std::stod(tokens[0]), std::stod(tokens[1]));
            }
        }
    } catch (const VMMException& e) {
        std::cerr << e;
        exit(EXIT_FAILURE);
    }

    input.close();

    ptrBoundaryPolygon_ = std::make_shared<gtp::Polygon2D>(std::begin(lstPoint2D), std::end(lstPoint2D));
    lstPoint2D.clear();

    if (ptrBoundaryPolygon_->is_counterclockwise_oriented()) {
        ptrBoundaryPolygon_->reverse_orientation();
    }
}

//==============================================================================
// Typedefs Documentation
//==============================================================================

/**
 * @typedef PtrPolygonCSV
 * @brief Defines a shared pointer to a PolygonCSV object.
 *
 * This typedef is useful for managing the shared ownership of PolygonCSV instances.
 */

/**
 * @typedef PtrConstPolygonCSV
 * @brief Defines a shared pointer to a const PolygonCSV object.
 *
 * This typedef is used when the shared ownership of a constant PolygonCSV instance is required.
 */

VORMAKER_NAMESPACE_CLOSE
