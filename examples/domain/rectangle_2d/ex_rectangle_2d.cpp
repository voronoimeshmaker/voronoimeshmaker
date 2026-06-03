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
// @file ex_rectangle_2d.cpp
// @brief Reads a rectangle domain description, creates a VMM domain, and exports VTK.
// @ingroup vmm_examples
//==============================================================================

#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/domain/Transform2D.hpp>
#include <vmm/export/VTKExporter.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

#ifndef VMM_EXAMPLE_SOURCE_DIR
#define VMM_EXAMPLE_SOURCE_DIR "."
#endif

namespace {

[[nodiscard]] std::string trim(std::string_view text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if(first == std::string_view::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return std::string{text.substr(first, last - first + 1U)};
}

[[nodiscard]] vmm::core::Real parse_real(std::string_view text, std::string_view context)
{
    char* end = nullptr;
    const std::string value{text};
    const auto parsed = std::strtod(value.c_str(), &end);
    if(end == value.c_str() || (end != nullptr && *end != '\0')) {
        vmm::error::throw_parse_error(context, "Invalid real value. / Valor real invalido.");
    }
    return parsed;
}

[[nodiscard]] std::unordered_map<std::string, vmm::core::Real> read_scalar_yaml(
    const std::filesystem::path& file_path,
    std::string_view context)
{
    std::ifstream input{file_path};
    if(!input) {
        vmm::error::throw_file_not_found(context, file_path.string());
    }

    std::unordered_map<std::string, vmm::core::Real> values;
    std::string section;
    std::string line;

    while(std::getline(input, line)) {
        const auto comment = line.find('#');
        if(comment != std::string::npos) {
            line.erase(comment);
        }

        const auto clean = trim(line);
        if(clean.empty()) {
            continue;
        }
        if(clean.back() == ':') {
            section = trim(std::string_view{clean}.substr(0U, clean.size() - 1U));
            continue;
        }

        const auto separator = clean.find(':');
        if(separator == std::string::npos) {
            continue;
        }

        const auto key = trim(std::string_view{clean}.substr(0U, separator));
        const auto value = trim(std::string_view{clean}.substr(separator + 1U));
        values[section + "." + key] = parse_real(value, context);
    }

    return values;
}

[[nodiscard]] vmm::core::Real value_or(const std::unordered_map<std::string, vmm::core::Real>& values,
                                       std::string_view key,
                                       vmm::core::Real fallback)
{
    const auto iterator = values.find(std::string{key});
    if(iterator == values.end()) {
        return fallback;
    }
    return iterator->second;
}

constexpr vmm::core::Real pi()
{
    return static_cast<vmm::core::Real>(3.141592653589793238462643383279502884L);
}

} // namespace

int main()
{
    try {
        const std::filesystem::path source_dir{VMM_EXAMPLE_SOURCE_DIR};
        const auto data_file = source_dir / "geometry_data" / "rectangle_2d.yaml";
        const auto output_file = source_dir / "output_data" / "rectangle_2d.vtk";
        const auto values = read_scalar_yaml(data_file, "ex_rectangle_2d");

        const vmm::domain::Point2D centre{
            value_or(values, "rectangle.centre_x", 0.0),
            value_or(values, "rectangle.centre_y", 0.0)
        };
        auto domain = vmm::domain::make_rectangle_2d(value_or(values, "rectangle.width", 0.0),
                                                     value_or(values, "rectangle.height", 0.0),
                                                     centre);
        const vmm::domain::Transform2D transform{
            value_or(values, "transform.translation_x", 0.0),
            value_or(values, "transform.translation_y", 0.0),
            value_or(values, "transform.rotation_degrees", 0.0) * pi() / 180.0
        };
        domain = transform.apply(domain, centre);
        vmm::io::write_legacy_vtk_polygonal_domain_2d(output_file, "Rectangle 2D geometry", domain);

        std::cout << "Rectangle 2D example\n";
        std::cout << "data_file: " << data_file << '\n';
        std::cout << "area: " << domain.area() << '\n';
        std::cout << "vtk_file: " << output_file << '\n';
    } catch(const vmm::error::MeshException& exception) {
        std::cerr << "ex_rectangle_2d failed: " << exception.what() << '\n';
        return 1;
    }
    return 0;
}
