//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file MohidGeometryReader.hpp
// @brief Readers for MOHID-style 2D geometry files.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/error/MeshException.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace vmm::domain {

struct BathymetryPoint2D final {
    Point2D point{};
    vmm::core::Real depth{};
};

inline std::string trim_text(std::string_view text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if(first == std::string_view::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return std::string{text.substr(first, last - first + 1U)};
}

inline vmm::core::Real parse_real_text(std::string_view text, std::string_view context)
{
    char* end = nullptr;
    const std::string value{text};
    const auto parsed = std::strtod(value.c_str(), &end);
    if(end == value.c_str() || (end != nullptr && *end != '\0')) {
        vmm::error::throw_parse_error(context, "Invalid real value: " + value);
    }
    return parsed;
}

inline Polygon2D read_mohid_polygon_2d(const std::filesystem::path& file_path,
                                       std::string_view context = "read_mohid_polygon_2d")
{
    std::ifstream input{file_path};
    if(!input) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }

    std::vector<Point2D> vertices;
    bool inside_polygon = false;
    std::string line;

    while(std::getline(input, line)) {
        const auto comment = line.find('!');
        if(comment != std::string::npos) {
            line.erase(comment);
        }

        const auto clean = trim_text(line);
        if(clean.empty()) {
            continue;
        }
        if(clean == "<beginpolygon>") {
            inside_polygon = true;
            continue;
        }
        if(clean == "<endpolygon>") {
            inside_polygon = false;
            continue;
        }
        if(!inside_polygon) {
            continue;
        }

        std::istringstream stream{clean};
        std::string x_text;
        std::string y_text;
        stream >> x_text >> y_text;
        if(x_text.empty() || y_text.empty()) {
            vmm::error::throw_parse_error(context, "Polygon vertex requires x and y coordinates.");
        }
        vertices.push_back(Point2D{parse_real_text(x_text, context), parse_real_text(y_text, context)});
    }

    return Polygon2D{std::move(vertices)};
}

inline std::vector<Polygon2D> read_mohid_box_polygons_2d(const std::filesystem::path& file_path,
                                                         std::string_view context = "read_mohid_box_polygons_2d")
{
    std::ifstream input{file_path};
    if(!input) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }

    std::vector<Polygon2D> boxes;
    std::vector<Point2D> current;
    bool inside_vertices = false;
    std::string line;

    while(std::getline(input, line)) {
        const auto clean = trim_text(line);
        if(clean == "<<beginvertix>>") {
            current.clear();
            inside_vertices = true;
            continue;
        }
        if(clean == "<<endvertix>>") {
            inside_vertices = false;
            boxes.emplace_back(current);
            continue;
        }
        if(!inside_vertices || clean.empty()) {
            continue;
        }

        std::istringstream stream{clean};
        std::string x_text;
        std::string y_text;
        stream >> x_text >> y_text;
        if(x_text.empty() || y_text.empty()) {
            vmm::error::throw_parse_error(context, "Box vertex requires two indices.");
        }
        current.push_back(Point2D{parse_real_text(x_text, context), parse_real_text(y_text, context)});
    }

    if(boxes.empty()) {
        vmm::error::throw_invalid_argument(context, "No box polygons were found.");
    }
    return boxes;
}

inline std::vector<BathymetryPoint2D> read_mohid_bathymetry_points_2d(
    const std::filesystem::path& file_path,
    std::string_view context = "read_mohid_bathymetry_points_2d")
{
    std::ifstream input{file_path};
    if(!input) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }

    std::vector<BathymetryPoint2D> points;
    std::string line;
    while(std::getline(input, line)) {
        const auto clean = trim_text(line);
        if(clean.empty()) {
            continue;
        }

        std::istringstream stream{clean};
        std::string x_text;
        std::string y_text;
        std::string depth_text;
        stream >> x_text >> y_text >> depth_text;
        if(x_text.empty() || y_text.empty() || depth_text.empty()) {
            vmm::error::throw_parse_error(context, "Bathymetry point requires x, y, and depth.");
        }
        points.push_back(BathymetryPoint2D{
            Point2D{parse_real_text(x_text, context), parse_real_text(y_text, context)},
            parse_real_text(depth_text, context)
        });
    }
    return points;
}

} // namespace vmm::domain
