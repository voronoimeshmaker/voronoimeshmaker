//==============================================================================
// File        : Concepts.hpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : C++20 concept definitions that establish compile-time contracts
//               for generators, exporters, and quality analysers.
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.
//
// @file Concepts.hpp
// @brief Compile-time contracts for mesh generation, export, and analysis.
// @ingroup vmm_core
//==============================================================================

#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <concepts>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>
#include <vmm/mesh/MeshTopology.hpp>

namespace vmm::core {

/**
 * @brief Concept for dimension-specific mesh generators.
 * @tparam T Generator type.
 * @tparam Dim Target dimension.
 * @ingroup vmm_core
 *
 * Ensures that any backend implementing this concept provides a
 * `generate()` routine returning a fully initialised `MeshTopology<Dim>`.
 */
template<typename T, Dimension Dim>
concept MeshGenerator = requires(T gen, const void* config) {
    { gen.generate(config) } -> std::same_as<MeshTopology<Dim>>;
};

/**
 * @brief Concept for mesh exporters.
 * @tparam T Exporter type.
 * @tparam Dim Target dimension.
 * @ingroup vmm_core
 *
 * Guarantees a standard `export_mesh()` signature accepting a read-only
 * topology reference and an output path.
 */
template<typename T, Dimension Dim>
concept MeshExporter = requires(T exp, const MeshTopology<Dim>& mesh, const char* path) {
    { exp.export_mesh(mesh, path) } -> std::same_as<void>;
};

/**
 * @brief Concept for quality and geometric analysers.
 * @tparam T Analyser type.
 * @tparam Dim Target dimension.
 * @ingroup vmm_core
 *
 * Requires an `analyse()` method that returns a boolean pass/fail status
 * and optionally populates a structured report via side effects or return value.
 */
template<typename T, Dimension Dim>
concept MeshAnalyzer = requires(T ana, const MeshTopology<Dim>& mesh) {
    { ana.analyse(mesh) } -> std::convertible_to<bool>;
};

} // namespace vmm::core