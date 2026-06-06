//==============================================================================
// File        : GisVectorReader2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : GIS vector reader implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
#include <gdal.h>
#include <ogr_api.h>
#include <ogr_srs_api.h>
#endif


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/gis/GisVectorReader2D.hpp>

namespace vmm::gis {
namespace {

[[nodiscard]] bool text_equals(std::string_view lhs, std::string_view rhs) noexcept
{
    if(lhs.size() != rhs.size()) {
        return false;
    }
    for(std::size_t i = 0U; i < lhs.size(); ++i) {
        const auto a = static_cast<char>(std::tolower(static_cast<unsigned char>(lhs[i])));
        const auto b = static_cast<char>(std::tolower(static_cast<unsigned char>(rhs[i])));
        if(a != b) {
            return false;
        }
    }
    return true;
}

[[noreturn]] void throw_gis_unavailable()
{
    vmm::error::throw_error(vmm::error::builtin_error_id::unsupported_operation,
                            "read_gis_polygonal_domain_2d",
                            "GDAL/OGR vector support is not available in this VMM build.");
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

    ~GdalDatasetHandle() { close(); }

    [[nodiscard]] GDALDatasetH get() const noexcept { return dataset_; }
    [[nodiscard]] bool valid() const noexcept { return dataset_ != nullptr; }

private:
    void close() noexcept
    {
        if(dataset_ != nullptr) {
            GDALClose(dataset_);
        }
    }

    GDALDatasetH dataset_{nullptr};
};

class OgrGeometryHandle final {
public:
    OgrGeometryHandle() = default;
    explicit OgrGeometryHandle(OGRGeometryH geometry) noexcept : geometry_(geometry) {}
    OgrGeometryHandle(const OgrGeometryHandle&) = delete;
    OgrGeometryHandle& operator=(const OgrGeometryHandle&) = delete;
    OgrGeometryHandle(OgrGeometryHandle&& other) noexcept : geometry_(other.geometry_)
    {
        other.geometry_ = nullptr;
    }
    OgrGeometryHandle& operator=(OgrGeometryHandle&& other) noexcept
    {
        if(this != &other) {
            close();
            geometry_ = other.geometry_;
            other.geometry_ = nullptr;
        }
        return *this;
    }
    ~OgrGeometryHandle() { close(); }

    [[nodiscard]] OGRGeometryH get() const noexcept { return geometry_; }
    [[nodiscard]] bool valid() const noexcept { return geometry_ != nullptr; }

private:
    void close() noexcept
    {
        if(geometry_ != nullptr) {
            OGR_G_DestroyGeometry(geometry_);
        }
    }

    OGRGeometryH geometry_{nullptr};
};

[[nodiscard]] std::string feature_field_text(OGRFeatureH feature, std::string_view field_name)
{
    if(field_name.empty()) {
        return {};
    }
    const auto field_index = OGR_F_GetFieldIndex(feature, std::string{field_name}.c_str());
    if(field_index < 0 || !OGR_F_IsFieldSetAndNotNull(feature, field_index)) {
        return {};
    }
    const auto* text = OGR_F_GetFieldAsString(feature, field_index);
    return text == nullptr ? std::string{} : std::string{text};
}

[[nodiscard]] CoordinateReferenceSystem crs_from_spatial_ref(OGRSpatialReferenceH spatial_ref)
{
    CoordinateReferenceSystem crs;
    if(spatial_ref == nullptr) {
        return crs;
    }

    const auto* authority = OSRGetAuthorityName(spatial_ref, nullptr);
    const auto* code = OSRGetAuthorityCode(spatial_ref, nullptr);
    const auto* projected_name = OSRGetAttrValue(spatial_ref, "PROJCS", 0);
    const auto* geographic_name = OSRGetAttrValue(spatial_ref, "GEOGCS", 0);
    crs.authority = authority == nullptr ? std::string{} : std::string{authority};
    crs.code = code == nullptr ? std::string{} : std::string{code};
    crs.name = projected_name != nullptr ? std::string{projected_name}
              : geographic_name != nullptr ? std::string{geographic_name}
                                           : std::string{};
    crs.geographic = OSRIsGeographic(spatial_ref) != 0;

    char* raw_wkt = nullptr;
    if(OSRExportToWkt(spatial_ref, &raw_wkt) == OGRERR_NONE && raw_wkt != nullptr) {
        crs.wkt = raw_wkt;
        CPLFree(raw_wkt);
    }
    return crs;
}

[[nodiscard]] vmm::domain::Polygon2D polygon_from_ring(OGRGeometryH ring)
{
    const auto point_count = OGR_G_GetPointCount(ring);
    std::vector<vmm::domain::Point2D> vertices;
    vertices.reserve(static_cast<std::size_t>(point_count));
    for(int point = 0; point < point_count; ++point) {
        vertices.push_back(vmm::domain::Point2D{OGR_G_GetX(ring, point), OGR_G_GetY(ring, point)});
    }
    if(vertices.size() > 1U && vertices.front().x == vertices.back().x && vertices.front().y == vertices.back().y) {
        vertices.pop_back();
    }
    return vmm::domain::Polygon2D{std::move(vertices)};
}

void append_polygon(OGRGeometryH polygon,
                    std::vector<vmm::domain::Polygon2D>& polygons,
                    std::vector<vmm::domain::Polygon2D>& holes)
{
    const auto exterior = OGR_G_GetGeometryRef(polygon, 0);
    if(exterior == nullptr) {
        return;
    }
    polygons.push_back(polygon_from_ring(exterior));

    const auto ring_count = OGR_G_GetGeometryCount(polygon);
    for(int ring_index = 1; ring_index < ring_count; ++ring_index) {
        const auto ring = OGR_G_GetGeometryRef(polygon, ring_index);
        if(ring != nullptr) {
            holes.push_back(polygon_from_ring(ring));
        }
    }
}

void append_polygons_from_geometry(OGRGeometryH geometry,
                                   std::vector<vmm::domain::Polygon2D>& polygons,
                                   std::vector<vmm::domain::Polygon2D>& holes)
{
    if(geometry == nullptr) {
        return;
    }
    const auto geometry_type = wkbFlatten(OGR_G_GetGeometryType(geometry));
    if(geometry_type == wkbPolygon) {
        append_polygon(geometry, polygons, holes);
        return;
    }
    if(geometry_type == wkbMultiPolygon) {
        const auto count = OGR_G_GetGeometryCount(geometry);
        for(int i = 0; i < count; ++i) {
            append_polygon(OGR_G_GetGeometryRef(geometry, i), polygons, holes);
        }
    }
}

[[nodiscard]] std::size_t largest_polygon_index(const std::vector<vmm::domain::Polygon2D>& polygons)
{
    auto best = std::size_t{};
    auto best_area = vmm::core::Real{};
    for(std::size_t i = 0U; i < polygons.size(); ++i) {
        if(polygons[i].area() > best_area) {
            best = i;
            best_area = polygons[i].area();
        }
    }
    return best;
}

[[nodiscard]] OGRLayerH selected_layer(GDALDatasetH dataset, const GisVectorReadOptions& options)
{
    if(!options.layer_name.empty()) {
        auto* layer = GDALDatasetGetLayerByName(dataset, options.layer_name.c_str());
        if(layer == nullptr) {
            vmm::error::throw_invalid_argument("read_gis_polygonal_domain_2d", "Requested GIS layer was not found.");
        }
        return layer;
    }
    auto* layer = GDALDatasetGetLayer(dataset, 0);
    if(layer == nullptr) {
        vmm::error::throw_parse_error("read_gis_polygonal_domain_2d", "GIS vector source has no readable layers.");
    }
    return layer;
}

#endif

} // namespace

bool gis_vector_backend_available() noexcept
{
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
    return true;
#else
    return false;
#endif
}

bool gis_crs_transform_backend_available() noexcept
{
#if defined(VMM_HAS_PROJ) && VMM_HAS_PROJ
    return true;
#else
    return false;
#endif
}

bool coordinate_reference_systems_compatible(const CoordinateReferenceSystem& lhs,
                                             const CoordinateReferenceSystem& rhs) noexcept
{
    if(lhs.empty() || rhs.empty()) {
        return true;
    }

    const auto lhs_identifier = lhs.identifier();
    const auto rhs_identifier = rhs.identifier();
    if(!lhs_identifier.empty() && !rhs_identifier.empty()) {
        return text_equals(lhs_identifier, rhs_identifier);
    }

    if(!lhs.wkt.empty() && !rhs.wkt.empty()) {
        return lhs.wkt == rhs.wkt;
    }

    return false;
}

void require_compatible_coordinate_reference_systems(std::string_view context,
                                                     std::string_view lhs_name,
                                                     const CoordinateReferenceSystem& lhs,
                                                     std::string_view rhs_name,
                                                     const CoordinateReferenceSystem& rhs)
{
    if(coordinate_reference_systems_compatible(lhs, rhs)) {
        return;
    }

    const auto lhs_identifier = lhs.identifier().empty() ? std::string{"unknown"} : lhs.identifier();
    const auto rhs_identifier = rhs.identifier().empty() ? std::string{"unknown"} : rhs.identifier();
    vmm::error::throw_invalid_argument(
        context,
        std::string{"Incompatible coordinate reference systems: "} + std::string{lhs_name} + "=" +
            lhs_identifier + ", " + std::string{rhs_name} + "=" + rhs_identifier + ".");
}

vmm::mesh::BoundaryPatchType boundary_patch_type_from_gis_text(std::string_view text) noexcept
{
    if(text_equals(text, "wall")) {
        return vmm::mesh::BoundaryPatchTypeTraits::Wall;
    }
    if(text_equals(text, "island")) {
        return vmm::mesh::BoundaryPatchTypeTraits::Island;
    }
    if(text_equals(text, "inlet")) {
        return vmm::mesh::BoundaryPatchTypeTraits::Inlet;
    }
    if(text_equals(text, "outlet")) {
        return vmm::mesh::BoundaryPatchTypeTraits::Outlet;
    }
    if(text_equals(text, "open") || text_equals(text, "open-boundary") || text_equals(text, "open_boundary")) {
        return vmm::mesh::BoundaryPatchTypeTraits::OpenBoundary;
    }
    if(text_equals(text, "land") || text_equals(text, "land-boundary") || text_equals(text, "land_boundary")) {
        return vmm::mesh::BoundaryPatchTypeTraits::LandBoundary;
    }
    if(text_equals(text, "noflux") || text_equals(text, "no-flux") || text_equals(text, "no_flux")) {
        return vmm::mesh::BoundaryPatchTypeTraits::NoFlux;
    }
    return vmm::mesh::BoundaryPatchTypeTraits::Wall;
}

GeoVectorDomain2D read_gis_polygonal_domain_2d(const std::filesystem::path& file_path,
                                               const GisVectorReadOptions& options)
{
#if defined(VMM_HAS_GDAL) && VMM_HAS_GDAL
    if(!std::filesystem::exists(file_path)) {
        vmm::error::throw_file_not_found("read_gis_polygonal_domain_2d", file_path.string());
    }

    GDALAllRegister();
    GdalDatasetHandle dataset{GDALOpenEx(file_path.string().c_str(), GDAL_OF_VECTOR | GDAL_OF_READONLY, nullptr, nullptr, nullptr)};
    if(!dataset.valid()) {
        vmm::error::throw_parse_error("read_gis_polygonal_domain_2d", "Cannot open GIS vector source with GDAL/OGR.");
    }

    auto* layer = selected_layer(dataset.get(), options);
    const auto layer_name = std::string{OGR_L_GetName(layer) == nullptr ? "" : OGR_L_GetName(layer)};
    const auto source_crs = crs_from_spatial_ref(OGR_L_GetSpatialRef(layer));

    std::vector<vmm::domain::Polygon2D> polygons;
    std::vector<vmm::domain::Polygon2D> holes;
    std::string feature_name;
    std::string feature_type;
    GIntBig feature_id{};

    OGR_L_ResetReading(layer);
    while(auto* feature = OGR_L_GetNextFeature(layer)) {
        OGRGeometryH geometry = OGR_F_GetGeometryRef(feature);
        if(geometry == nullptr) {
            OGR_F_Destroy(feature);
            continue;
        }

        OgrGeometryHandle flattened{OGR_G_ForceTo(OGR_G_Clone(geometry), wkbMultiPolygon, nullptr)};
        append_polygons_from_geometry(flattened.get(), polygons, holes);
        if(feature_name.empty()) {
            feature_name = feature_field_text(feature, options.name_attribute);
        }
        if(feature_type.empty()) {
            feature_type = feature_field_text(feature, options.patch_type_attribute);
        }
        feature_id = OGR_F_GetFID(feature);
        OGR_F_Destroy(feature);
    }

    if(polygons.empty()) {
        vmm::error::throw_parse_error("read_gis_polygonal_domain_2d", "No polygon geometry was found in GIS vector source.");
    }

    const auto exterior_index = largest_polygon_index(polygons);
    auto exterior = polygons[exterior_index];
    for(std::size_t i = 0U; i < polygons.size(); ++i) {
        if(i != exterior_index) {
            holes.push_back(polygons[i]);
        }
    }

    GeoVectorDomain2D result;
    result.source_crs = source_crs;
    result.working_crs = options.working_crs.empty() ? source_crs : options.working_crs;
    result.domain = vmm::domain::PolygonalDomain2D{std::move(exterior), holes};

    const auto exterior_name = feature_name.empty() ? std::string{"outer-boundary"} : feature_name;
    const auto exterior_type = feature_type.empty() ? vmm::mesh::BoundaryPatchTypeTraits::LandBoundary
                                                    : boundary_patch_type_from_gis_text(feature_type);
    result.boundary_patches.push_back(GisBoundaryPatch2D{
        result.domain.exterior(),
        exterior_name,
        exterior_type,
        layer_name,
        std::to_string(feature_id)});

    for(std::size_t hole = 0U; hole < result.domain.holes().size(); ++hole) {
        result.boundary_patches.push_back(GisBoundaryPatch2D{
            result.domain.holes()[hole],
            "hole-" + std::to_string(hole),
            vmm::mesh::BoundaryPatchTypeTraits::Island,
            layer_name,
            std::to_string(feature_id)});
    }

    return result;
#else
    static_cast<void>(file_path);
    static_cast<void>(options);
    throw_gis_unavailable();
#endif
}

} // namespace vmm::gis
