//==============================================================================
// File        : RasterField2D.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Regular 2D raster scalar fields and finite-volume sampling.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string_view>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/core/Types.hpp>
#include <vmm/gis/GisVectorReader2D.hpp>
#include <vmm/mesh/FiniteVolumeMesh2D.hpp>

namespace vmm::raster {

struct RasterSampleMethod final {
    std::string_view name;
    std::uint8_t code;

    [[nodiscard]] friend constexpr bool operator==(RasterSampleMethod lhs,
                                                   RasterSampleMethod rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }
};

struct RasterSampleMethodTraits final {
    static constexpr RasterSampleMethod Nearest{"nearest", 0U};
    static constexpr RasterSampleMethod Bilinear{"bilinear", 1U};
};

struct RasterGridGeometry2D final {
    vmm::core::Real origin_x{};
    vmm::core::Real origin_y{};
    vmm::core::Real spacing_x{1.0};
    vmm::core::Real spacing_y{1.0};

    [[nodiscard]] vmm::core::Point<vmm::core::Dimension::D2>
    point_at(std::size_t row, std::size_t column) const noexcept;
    void validate_or_throw() const;
};

class RasterScalarField2D final {
public:
    RasterScalarField2D() = default;
    RasterScalarField2D(std::size_t width,
                        std::size_t height,
                        RasterGridGeometry2D geometry,
                        std::vector<vmm::core::Real> values,
                        std::optional<vmm::core::Real> no_data_value = std::nullopt,
                        vmm::gis::CoordinateReferenceSystem crs = {});

    [[nodiscard]] std::size_t width() const noexcept;
    [[nodiscard]] std::size_t height() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] const RasterGridGeometry2D& geometry() const noexcept;
    [[nodiscard]] const vmm::gis::CoordinateReferenceSystem& crs() const noexcept;
    [[nodiscard]] std::optional<vmm::core::Real> no_data_value() const noexcept;
    [[nodiscard]] std::span<const vmm::core::Real> values() const noexcept;
    [[nodiscard]] bool contains_grid_index(std::size_t row, std::size_t column) const noexcept;
    [[nodiscard]] bool has_value(std::size_t row, std::size_t column) const;
    [[nodiscard]] vmm::core::Real value(std::size_t row, std::size_t column) const;
    [[nodiscard]] std::optional<vmm::core::Real>
    sample(vmm::core::Point<vmm::core::Dimension::D2> point,
           RasterSampleMethod method = RasterSampleMethodTraits::Bilinear) const;

private:
    [[nodiscard]] std::size_t offset(std::size_t row, std::size_t column) const noexcept;
    void validate_storage_or_throw() const;

    std::size_t width_{};
    std::size_t height_{};
    RasterGridGeometry2D geometry_{};
    std::vector<vmm::core::Real> values_{};
    std::optional<vmm::core::Real> no_data_value_{};
    vmm::gis::CoordinateReferenceSystem crs_{};
};

struct RasterReadOptions2D final {
    std::size_t band_index{1U};
    vmm::gis::CoordinateReferenceSystem working_crs{};
    bool transform_to_working_crs{true};
};

struct CellRasterSamples2D final {
    std::vector<vmm::core::CellId> cell_id;
    std::vector<vmm::core::Real> value;
    std::vector<std::uint8_t> valid;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool has_consistent_storage() const noexcept;
};

[[nodiscard]] bool gis_raster_backend_available() noexcept;

[[nodiscard]] RasterScalarField2D
read_gis_raster_scalar_field_2d(const std::filesystem::path& path,
                                const RasterReadOptions2D& options = {});

[[nodiscard]] CellRasterSamples2D
sample_cell_centres(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                    const RasterScalarField2D& raster,
                    RasterSampleMethod method = RasterSampleMethodTraits::Bilinear);

} // namespace vmm::raster
