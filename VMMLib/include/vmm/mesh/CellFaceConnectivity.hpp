//==============================================================================
// File        : CellFaceConnectivity.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Flat cell-to-face connectivity storage for 2D FV meshes.
//==============================================================================
#pragma once

#include <vmm/core/StrongIds.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace vmm::mesh {

struct CellFaceConnectivity final {
    std::vector<vmm::core::FaceId> face_ids;

    [[nodiscard]] std::size_t size() const noexcept { return face_ids.size(); }
    [[nodiscard]] bool empty() const noexcept { return face_ids.empty(); }
    [[nodiscard]] std::span<const vmm::core::FaceId> faces() const noexcept { return face_ids; }

    [[nodiscard]] std::span<const vmm::core::FaceId> cell_faces(std::uint64_t offset, std::uint32_t count) const
    {
        const auto begin = static_cast<std::size_t>(offset);
        const auto n = static_cast<std::size_t>(count);
        if(begin > face_ids.size() || n > face_ids.size() - begin) {
            throw std::out_of_range("Cell face span exceeds connectivity storage");
        }
        return std::span<const vmm::core::FaceId>{face_ids.data() + begin, n};
    }
};

} // namespace vmm::mesh
