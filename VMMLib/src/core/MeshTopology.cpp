//==============================================================================
// File        : MeshTopology.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the MeshTopology template class, including
//               bounds-checked accessors, CSR neighbour retrieval, and explicit
//               template instantiation for 2D and 3D configurations.
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
// @file MeshTopology.cpp
// @brief Implementation of the dimension-agnostic topology container.
// @ingroup vmm_core
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <stdexcept>

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/MeshTopology.hpp>

namespace vmm::core {

template<Dimension Dim>
MeshTopology<Dim>::MeshTopology(std::size_t capacity)
    : centroids_(capacity)
    , generator_ids_(capacity)
    , cell_offsets_(capacity + 1, 0)
    , neighbour_ids_()
    , boundary_flags_(capacity, static_cast<uint8_t>(BoundaryType::Internal))
{}

template<Dimension Dim>
std::span<const Point<Dim>> MeshTopology<Dim>::centroids() const noexcept {
    return std::span<const Point<Dim>>{centroids_.data(), centroids_.size()};
}

template<Dimension Dim>
std::span<const GeneratorID> MeshTopology<Dim>::generator_ids() const noexcept {
    return std::span<const GeneratorID>{generator_ids_.data(), generator_ids_.size()};
}

template<Dimension Dim>
std::span<const CellIndex> MeshTopology<Dim>::neighbours(CellIndex cell_id) const {
    if (cell_id >= cell_count()) {
        throw std::out_of_range("Cell index exceeds topology bounds");
    }
    const auto start = cell_offsets_[cell_id];
    const auto end   = cell_offsets_[cell_id + 1];
    return std::span<const CellIndex>{neighbour_ids_.data() + start, static_cast<std::size_t>(end - start)};
}

template<Dimension Dim>
BoundaryType MeshTopology<Dim>::boundary_type(CellIndex cell_id) const {
    return static_cast<BoundaryType>(boundary_flags_.at(cell_id));
}

template<Dimension Dim>
std::size_t MeshTopology<Dim>::cell_count() const noexcept {
    return centroids_.size();
}

template<Dimension Dim>
std::size_t MeshTopology<Dim>::total_neighbours() const noexcept {
    return neighbour_ids_.size();
}

template<Dimension Dim>
void MeshTopology<Dim>::set_centroid(CellIndex id, const Point<Dim>& p) {
    centroids_.at(id) = p;
}

template<Dimension Dim>
void MeshTopology<Dim>::assign_generator_id(CellIndex id, GeneratorID gid) {
    generator_ids_.at(id) = gid;
}

template<Dimension Dim>
void MeshTopology<Dim>::set_boundary_flag(CellIndex id, BoundaryType type) {
    boundary_flags_.at(id) = static_cast<uint8_t>(type);
}

// Explicit template instantiations
template class MeshTopology<Dimension::D2>;
template class MeshTopology<Dimension::D3>;

} // namespace vmm::core