//==============================================================================
// Name        : Ex_GeometricData.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.2
// Description : Example usage of GeometricDataHolder for storing geometric data.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
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
 * @file Ex_GeometricData.cpp
 * @brief Demonstrates the use of the GeometricDataHolder class for managing various geometric data.
 *
 * This example shows how to create an instance of GeometricDataHolder, store multiple data types,
 * and retrieve the stored data. It highlights the flexibility of the GeometricDataHolder class
 * in handling different geometric parameters in a thread-safe manner.
 *
 * @author João Flávio Vieira de Vasconcellos
 * @version 1.2
 * @date 2024
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes from C++ Standard Library
//==============================================================================

#include <iostream>
#include <list>
#include <string>

//==============================================================================
// Includes from VoronoiMeshMaker library
//==============================================================================


#include <VoronoiMeshMaker/Misc/type.h>
#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>  
#include <VoronoiMeshMaker/Error/MakerLogger.h>
#include <VoronoiMeshMaker/Error/FunctionLogger.h>
        
using vmm::gtp::Point2D;
using vmm::gtp::Vector2D;
using vmm::gtp::Point3D;
using vmm::gtp::Vector3D;

/**
 * @brief Main function demonstrating GeometricDataHolder usage.
 *
 * This function creates an instance of GeometricDataHolder and demonstrates how to
 * store and retrieve various types of geometric and scalar data.
 *
 * @return int Exit status.
 */
int main() {
    // Creating the GeometricDataHolder instance
    vmm::GeometricDataHolder geometricDataHolder;
    
    vmm::MakerLogger::init();
    vmm::FunctionLogger functionLogger;
    
    // Adding different types of data
    geometricDataHolder.set("nPoints", 412);
    geometricDataHolder.set("dx", 0.1415);
    geometricDataHolder.set("dy", 0.3);
    geometricDataHolder.set("Point2D", Point2D(1.0, 2.0));
    geometricDataHolder.set("Vector2D", Vector2D(0.5, 1.5));
    geometricDataHolder.set("Point3D", Point3D(1.0, 2.0, 1));
    geometricDataHolder.set("Vector3D", Vector3D(0.5, 1.5, 5));

    std::list<int> myIntList = {1, 2, 3, 4, 5};
    geometricDataHolder.set("MyIntList", myIntList);

    std::list<Real> myRealList = {1.1, 2.2, 3.3, 4.4, 5.5};
    geometricDataHolder.set("MyRealList", myRealList);

    // Printing all the stored values
    std::cout << "\n\nValues stored in GeometricDataHolder:" << std::endl;
    std::cout << geometricDataHolder;

    std::cout << "\n\n\n";    
    // Retrieving values of different types
    if (auto intValue = geometricDataHolder.get<int>("nPoints")) {
        std::cout << "Value of 'nPoints': " << *intValue << std::endl;
    }

    if (auto realValue = geometricDataHolder.get<Real>("dx")) {
        std::cout << "Value of 'dx': " << *realValue << std::endl;
    }
    
    if (auto point2DValue = geometricDataHolder.get<Point2D>("Point2D")) {
        std::cout << "Value of 'Point2D': " << *point2DValue << std::endl;
    }
    
    if (auto vecIntValue = geometricDataHolder.get<std::list<int>>("MyIntList")) {
        for (auto x : *vecIntValue) {
            std::cout << "VecInt = " << x << "\n";
        }
    }
    
    return 0;
}

