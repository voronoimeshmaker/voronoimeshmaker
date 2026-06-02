//==============================================================================
// File        : MeshTopology.hpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Dimension-agnostic topology container with structure-of-arrays
//               (SoA) layout, CSR neighbour indexing, and index-based access.
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
// @file MeshTopology.hpp
// @brief Core topology container with SoA layout and CSR connectivity.
// @ingroup vmm_core
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <vector>
#include <span>
// #include <cstdint>
// #include <stdexcept>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>

namespace vmm::core {

/**
 * @brief High-performance topology container for Voronoi meshes.
 * @tparam Dim Dimensional configuration (D2 or D3).
 * @ingroup vmm_core
 *
 * Stores geometric and topological data in a structure-of-arrays (SoA) layout
 * to maximise cache locality and enable compiler auto-vectorisation. The public
 * API exposes read-only `std::span` views, preventing external mutation of
 * internal buffers whilst guaranteeing zero-copy access.
 *
 * @note All mutators are reserved for the generation and remeshing phases.
 *       Consumers should treat instances as immutable after construction.
 */
template<Dimension Dim>
class MeshTopology {
public:
    /**
     * @brief Constructs an empty topology with pre-allocated capacity.
     * @param capacity Expected number of cells. Avoids reallocation during generation.
     */
    explicit MeshTopology(std::size_t capacity);

    // =========================================================================
    // Read-only views (zero-copy, cache-friendly)
    // =========================================================================

    /** @returns Contiguous view of cell centroids. */
    [[nodiscard]] std::span<const Point<Dim>> centroids() const noexcept;

    /** @returns Contiguous view of sequential generator identifiers. */
    [[nodiscard]] std::span<const GeneratorID> generator_ids() const noexcept;

    /**
     * @brief Retrieves neighbours for a specific cell via CSR indexing.
     * @param cell_id Target cell index. Must be < cell_count().
     * @returns Read-only span of neighbour indices.
     * @throws std::out_of_range If cell_id exceeds valid range.
     */
    [[nodiscard]] std::span<const CellIndex> neighbours(CellIndex cell_id) const;

    /** @returns Boundary classification for the specified cell. */
    [[nodiscard]] BoundaryType boundary_type(CellIndex cell_id) const;

    // =========================================================================
    // Metadata queries
    // =========================================================================

    /** @returns Total number of cells in the topology. */
    [[nodiscard]] std::size_t cell_count() const noexcept;

    /** @returns Total number of neighbour entries across all cells. */
    [[nodiscard]] std::size_t total_neighbours() const noexcept;

    // =========================================================================
    // Mutators (internal/generation phase only)
    // =========================================================================

    /** @brief Sets the geometric centroid for a cell. */
    void set_centroid(CellIndex id, const Point<Dim>& p);

    /** @brief Assigns the deterministic generator identifier. */
    void assign_generator_id(CellIndex id, GeneratorID gid);

    /** @brief Flags a cell as internal, external boundary, or hole boundary. */
    void set_boundary_flag(CellIndex id, BoundaryType type);

private:
    // Structure-of-Arrays layout (hot fields contiguous in memory)
    std::vector<Point<Dim>>       centroids_;
    std::vector<GeneratorID>      generator_ids_;
    std::vector<CellIndex>        cell_offsets_;     // CSR start indices (size = cell_count + 1)
    std::vector<CellIndex>        neighbour_ids_;    // Flattened adjacency list
    std::vector<uint8_t>          boundary_flags_;   // BoundaryType encoded as uint8_t
};

} // namespace vmm::core