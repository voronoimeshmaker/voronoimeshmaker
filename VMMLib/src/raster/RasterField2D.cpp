//==============================================================================
// File        : RasterField2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Regular 2D raster scalar field implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <limits>
#include <optional>
#include <utility>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
#include <gdal.h>
#include <ogr_srs_api.h>
#endif


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/raster/RasterField2D.hpp>

namespace vmm::raster {
namespace {

using Point2D = vmm::core::Point<vmm::core::Dimension::D2>;

[[nodiscard]] bool is_no_data(vmm::core::Real value,
                              std::optional<vmm::core::Real> no_data_value) noexcept
{
    return no_data_value.has_value() && value == no_data_value.value();
}

[[noreturn]] void throw_raster_unavailable()
{
    vmm::error::throw_error(vmm::error::builtin_error_id::unsupported_operation,
                            "read_gis_raster_scalar_field_2d",
                            "GDAL raster support is not available in this VMM build.");
}

#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL

class GdalDatasetHandle final {
public:
    GdalDatasetHandle() = default;
    explicit GdalDatasetHandle(GDALDatasetH dataset) noexcept : dataset_(dataset) {}
    GdalDatasetHandle(const GdalDatasetHandle&) = delete;
    GdalDatasetHandle& operator=(const GdalDatasetHandle&) = delete;
    GdalDatasetHandle(GdalDatasetHandle&& other) noexcept : dataset_(other.dataset_)
    {
        other.dataset_ = nullptr;
    }

    GdalDatasetHandle& operator=(GdalDatasetHandle&& other) noexcept
    {
        if(this != &other) {
            close();
            dataset_ = other.dataset_;
            other.dataset_ = nullptr;
        }
        return *this;
    }

    ~GdalDatasetHandle()
    {
        close();
    }

    [[nodiscard]] GDALDatasetH get() const noexcept { return dataset_; }

private:
    void close() noexcept
    {
        if(dataset_ != nullptr) {
            GDALClose(dataset_);
            dataset_ = nullptr;
        }
    }

    GDALDatasetH dataset_{};
};

[[nodiscard]] vmm::gis::CoordinateReferenceSystem read_raster_crs(GDALDatasetH dataset)
{
    vmm::gis::CoordinateReferenceSystem crs;
    const char* projection = GDALGetProjectionRef(dataset);
    if(projection == nullptr || projection[0] == '\0') {
        return crs;
    }

    OGRSpatialReferenceH srs = OSRNewSpatialReference(projection);
    if(srs == nullptr) {
        crs.wkt = projection;
        return crs;
    }

    const char* name = OSRGetAttrValue(srs, "PROJCS", 0);
    if(name == nullptr) {
        name = OSRGetAttrValue(srs, "GEOGCS", 0);
    }
    if(name != nullptr) {
        crs.name = name;
    }

    const char* authority = OSRGetAuthorityName(srs, nullptr);
    const char* code = OSRGetAuthorityCode(srs, nullptr);
    if(authority != nullptr) {
        crs.authority = authority;
    }
    if(code != nullptr) {
        crs.code = code;
    }
    crs.geographic = OSRIsGeographic(srs) != 0;
    crs.wkt = projection;

    OSRDestroySpatialReference(srs);
    return crs;
}

#endif

} // namespace

Point2D RasterGridGeometry2D::point_at(std::size_t row, std::size_t column) const noexcept
{
    return Point2D{origin_x + (static_cast<vmm::core::Real>(column) * spacing_x),
                   origin_y + (static_cast<vmm::core::Real>(row) * spacing_y)};
}

void RasterGridGeometry2D::validate_or_throw() const
{
    if(!std::isfinite(origin_x) || !std::isfinite(origin_y) || !std::isfinite(spacing_x) ||
       !std::isfinite(spacing_y) || spacing_x == 0.0 || spacing_y == 0.0) {
        vmm::error::throw_invalid_argument("RasterGridGeometry2D",
                                           "Raster geometry requires finite origin and non-zero finite spacing.");
    }
}

RasterScalarField2D::RasterScalarField2D(std::size_t width,
                                         std::size_t height,
                                         RasterGridGeometry2D geometry,
                                         std::vector<vmm::core::Real> values,
                                         std::optional<vmm::core::Real> no_data_value,
                                         vmm::gis::CoordinateReferenceSystem crs)
    : width_(width),
      height_(height),
      geometry_(geometry),
      values_(std::move(values)),
      no_data_value_(no_data_value),
      crs_(std::move(crs))
{
    geometry_.validate_or_throw();
    validate_storage_or_throw();
}

std::size_t RasterScalarField2D::width() const noexcept
{
    return width_;
}

std::size_t RasterScalarField2D::height() const noexcept
{
    return height_;
}

bool RasterScalarField2D::empty() const noexcept
{
    return width_ == 0U || height_ == 0U;
}

const RasterGridGeometry2D& RasterScalarField2D::geometry() const noexcept
{
    return geometry_;
}

const vmm::gis::CoordinateReferenceSystem& RasterScalarField2D::crs() const noexcept
{
    return crs_;
}

std::optional<vmm::core::Real> RasterScalarField2D::no_data_value() const noexcept
{
    return no_data_value_;
}

std::span<const vmm::core::Real> RasterScalarField2D::values() const noexcept
{
    return values_;
}

bool RasterScalarField2D::contains_grid_index(std::size_t row, std::size_t column) const noexcept
{
    return row < height_ && column < width_;
}

bool RasterScalarField2D::has_value(std::size_t row, std::size_t column) const
{
    if(!contains_grid_index(row, column)) {
        return false;
    }
    return !is_no_data(values_[offset(row, column)], no_data_value_);
}

vmm::core::Real RasterScalarField2D::value(std::size_t row, std::size_t column) const
{
    if(!contains_grid_index(row, column)) {
        vmm::error::throw_invalid_argument("RasterScalarField2D",
                                           "Raster index is outside the stored grid.");
    }
    return values_[offset(row, column)];
}

std::optional<vmm::core::Real> RasterScalarField2D::sample(Point2D point,
                                                          RasterSampleMethod method) const
{
    if(empty()) {
        return std::nullopt;
    }

    const auto column_position = (point.x - geometry_.origin_x) / geometry_.spacing_x;
    const auto row_position = (point.y - geometry_.origin_y) / geometry_.spacing_y;

    if(!std::isfinite(column_position) || !std::isfinite(row_position)) {
        return std::nullopt;
    }

    if(method == RasterSampleMethodTraits::Nearest) {
        const auto column = static_cast<long long>(std::llround(column_position));
        const auto row = static_cast<long long>(std::llround(row_position));
        if(column < 0 || row < 0) {
            return std::nullopt;
        }
        const auto row_index = static_cast<std::size_t>(row);
        const auto column_index = static_cast<std::size_t>(column);
        if(!has_value(row_index, column_index)) {
            return std::nullopt;
        }
        return value(row_index, column_index);
    }

    if(method != RasterSampleMethodTraits::Bilinear) {
        vmm::error::throw_invalid_argument("RasterScalarField2D",
                                           "Unsupported raster sample method.");
    }

    const auto column0_real = std::floor(column_position);
    const auto row0_real = std::floor(row_position);
    if(column0_real < 0.0 || row0_real < 0.0) {
        return std::nullopt;
    }

    const auto column0 = static_cast<std::size_t>(column0_real);
    const auto row0 = static_cast<std::size_t>(row0_real);
    const auto column1 = column0 + 1U;
    const auto row1 = row0 + 1U;
    if(!contains_grid_index(row0, column0) || !contains_grid_index(row1, column1)) {
        return std::nullopt;
    }
    if(!has_value(row0, column0) || !has_value(row0, column1) || !has_value(row1, column0) ||
       !has_value(row1, column1)) {
        return std::nullopt;
    }

    const auto sx = column_position - column0_real;
    const auto sy = row_position - row0_real;
    const auto v00 = value(row0, column0);
    const auto v10 = value(row0, column1);
    const auto v01 = value(row1, column0);
    const auto v11 = value(row1, column1);
    const auto top = (v00 * (1.0 - sx)) + (v10 * sx);
    const auto bottom = (v01 * (1.0 - sx)) + (v11 * sx);
    return (top * (1.0 - sy)) + (bottom * sy);
}

std::size_t RasterScalarField2D::offset(std::size_t row, std::size_t column) const noexcept
{
    return (row * width_) + column;
}

void RasterScalarField2D::validate_storage_or_throw() const
{
    if(width_ == 0U || height_ == 0U) {
        vmm::error::throw_invalid_argument("RasterScalarField2D",
                                           "Raster dimensions must be non-zero.");
    }
    if(values_.size() != width_ * height_) {
        vmm::error::throw_invalid_argument("RasterScalarField2D",
                                           "Raster value count must equal width times height.");
    }
    for(const auto value : values_) {
        if(!std::isfinite(value) && !is_no_data(value, no_data_value_)) {
            vmm::error::throw_invalid_argument("RasterScalarField2D",
                                               "Raster values must be finite unless they match the no-data marker.");
        }
    }
}

std::size_t CellRasterSamples2D::size() const noexcept
{
    return cell_id.size();
}

bool CellRasterSamples2D::empty() const noexcept
{
    return cell_id.empty();
}

bool CellRasterSamples2D::has_consistent_storage() const noexcept
{
    return cell_id.size() == value.size() && cell_id.size() == valid.size();
}

bool gis_raster_backend_available() noexcept
{
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
    return true;
#else
    return false;
#endif
}

RasterScalarField2D read_gis_raster_scalar_field_2d(const std::filesystem::path& path,
                                                    const RasterReadOptions2D& options)
{
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
    GDALAllRegister();
    GdalDatasetHandle dataset{GDALOpen(path.string().c_str(), GA_ReadOnly)};
    if(dataset.get() == nullptr) {
        vmm::error::throw_file_not_found("read_gis_raster_scalar_field_2d", path.string());
    }

    if(options.band_index == 0U ||
       options.band_index > static_cast<std::size_t>(GDALGetRasterCount(dataset.get()))) {
        vmm::error::throw_invalid_argument("read_gis_raster_scalar_field_2d",
                                           "Raster band index is outside the dataset band count.");
    }

    double geo_transform[6]{};
    if(GDALGetGeoTransform(dataset.get(), geo_transform) != CE_None) {
        vmm::error::throw_invalid_argument("read_gis_raster_scalar_field_2d",
                                           "Raster dataset does not expose an affine geotransform.");
    }
    if(geo_transform[2] != 0.0 || geo_transform[4] != 0.0) {
        vmm::error::throw_invalid_argument("read_gis_raster_scalar_field_2d",
                                           "Rotated raster geotransforms are not supported yet.");
    }

    auto* band = GDALGetRasterBand(dataset.get(), static_cast<int>(options.band_index));
    const auto width = static_cast<std::size_t>(GDALGetRasterXSize(dataset.get()));
    const auto height = static_cast<std::size_t>(GDALGetRasterYSize(dataset.get()));
    std::vector<vmm::core::Real> values(width * height);
    const auto io_status = GDALRasterIO(band,
                                        GF_Read,
                                        0,
                                        0,
                                        static_cast<int>(width),
                                        static_cast<int>(height),
                                        values.data(),
                                        static_cast<int>(width),
                                        static_cast<int>(height),
                                        GDT_Float64,
                                        0,
                                        0);
    if(io_status != CE_None) {
        vmm::error::throw_parse_error("read_gis_raster_scalar_field_2d",
                                      "Unable to read raster band values.");
    }

    int has_no_data = 0;
    const auto no_data = GDALGetRasterNoDataValue(band, &has_no_data);
    std::optional<vmm::core::Real> no_data_value;
    if(has_no_data != 0) {
        no_data_value = no_data;
    }

    auto crs = read_raster_crs(dataset.get());
    if(options.transform_to_working_crs && !options.working_crs.empty()) {
        crs = options.working_crs;
    }

    const RasterGridGeometry2D geometry{
        geo_transform[0] + (0.5 * geo_transform[1]),
        geo_transform[3] + (0.5 * geo_transform[5]),
        geo_transform[1],
        geo_transform[5]};

    return RasterScalarField2D{width, height, geometry, std::move(values), no_data_value, std::move(crs)};
#else
    (void)path;
    (void)options;
    throw_raster_unavailable();
#endif
}

CellRasterSamples2D sample_cell_centres(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                        const RasterScalarField2D& raster,
                                        RasterSampleMethod method)
{
    if(!mesh.cells.has_consistent_storage()) {
        vmm::error::throw_invalid_argument("sample_cell_centres",
                                           "Finite-volume mesh cell table has inconsistent storage.");
    }

    CellRasterSamples2D result;
    result.cell_id.reserve(mesh.cell_count());
    result.value.reserve(mesh.cell_count());
    result.valid.reserve(mesh.cell_count());

    for(std::size_t i = 0U; i < mesh.cell_count(); ++i) {
        const auto cell_id = vmm::core::CellId{static_cast<vmm::core::EntityIdValue>(i)};
        const auto sampled = raster.sample(Point2D{mesh.cells.centre_x[i], mesh.cells.centre_y[i]}, method);
        result.cell_id.push_back(cell_id);
        result.value.push_back(sampled.value_or(std::numeric_limits<vmm::core::Real>::quiet_NaN()));
        result.valid.push_back(sampled.has_value() ? 1U : 0U);
    }

    return result;
}

} // namespace vmm::raster
