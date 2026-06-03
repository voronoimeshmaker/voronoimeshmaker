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
// @file Transform2D.hpp
// @brief Rigid 2D transforms for planar domains.
// @ingroup vmm_domain
//==============================================================================
#pragma once

#include <vmm/domain/PolygonalDomain2D.hpp>

namespace vmm::domain {

class Transform2D final {
public:
    constexpr Transform2D() noexcept = default;
    constexpr Transform2D(vmm::core::Real translation_x,
                          vmm::core::Real translation_y,
                          vmm::core::Real rotation_radians) noexcept
        : translation_x_(translation_x),
          translation_y_(translation_y),
          rotation_radians_(rotation_radians)
    {
    }

    [[nodiscard]] constexpr vmm::core::Real translation_x() const noexcept { return translation_x_; }
    [[nodiscard]] constexpr vmm::core::Real translation_y() const noexcept { return translation_y_; }
    [[nodiscard]] constexpr vmm::core::Real rotation_radians() const noexcept { return rotation_radians_; }

    [[nodiscard]] Polygon2D apply(const Polygon2D& polygon, Point2D centre = {}) const
    {
        return polygon.rotated(rotation_radians_, centre).translated(translation_x_, translation_y_);
    }

    [[nodiscard]] PolygonalDomain2D apply(const PolygonalDomain2D& domain, Point2D centre = {}) const
    {
        return domain.rotated(rotation_radians_, centre).translated(translation_x_, translation_y_);
    }

private:
    vmm::core::Real translation_x_{};
    vmm::core::Real translation_y_{};
    vmm::core::Real rotation_radians_{};
};

} // namespace vmm::domain
