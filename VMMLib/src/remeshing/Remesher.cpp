//==============================================================================
// File        : Remesher.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Site-edit remeshing implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/error/MeshException.hpp>
#include <vmm/remeshing/Remesher.hpp>

namespace vmm::remeshing {
namespace {

[[nodiscard]] bool point_on_segment(vmm::domain::Point2D a,
                                    vmm::domain::Point2D b,
                                    vmm::domain::Point2D point) noexcept
{
    const auto cross = ((b.x - a.x) * (point.y - a.y)) - ((b.y - a.y) * (point.x - a.x));
    if(std::abs(cross) > static_cast<vmm::core::Real>(1.0e-12)) {
        return false;
    }

    return point.x >= std::min(a.x, b.x)
        && point.x <= std::max(a.x, b.x)
        && point.y >= std::min(a.y, b.y)
        && point.y <= std::max(a.y, b.y);
}

[[nodiscard]] bool point_in_ring(const vmm::domain::Polygon2D& polygon,
                                 vmm::domain::Point2D point) noexcept
{
    const auto vertices = polygon.vertices();
    const std::size_t count = polygon.is_closed() ? vertices.size() - 1U : vertices.size();
    bool inside = false;

    for(std::size_t i = 0U, j = count - 1U; i < count; j = i++) {
        const auto a = vertices[i];
        const auto b = vertices[j];
        if(point_on_segment(a, b, point)) {
            return true;
        }

        const bool crosses = ((a.y > point.y) != (b.y > point.y))
            && (point.x < (((b.x - a.x) * (point.y - a.y)) / (b.y - a.y)) + a.x);
        if(crosses) {
            inside = !inside;
        }
    }

    return inside;
}

[[nodiscard]] bool point_in_domain(const vmm::domain::PolygonalDomain2D& domain,
                                   vmm::domain::Point2D point) noexcept
{
    if(!point_in_ring(domain.exterior(), point)) {
        return false;
    }
    for(const auto& hole : domain.holes()) {
        if(point_in_ring(hole, point)) {
            return false;
        }
    }
    return true;
}

void validate_point(vmm::domain::Point2D point,
                    const vmm::domain::PolygonalDomain2D& domain,
                    const SiteRemeshingCriteria2D& criteria,
                    std::string_view context)
{
    if(!std::isfinite(point.x) || !std::isfinite(point.y)) {
        vmm::error::throw_invalid_argument(context, "Site coordinates must be finite.");
    }
    if(criteria.require_sites_inside_domain && !point_in_domain(domain, point)) {
        vmm::error::throw_invalid_argument(context, "Site edit places a generator outside the active domain.");
    }
}

void validate_site_count(std::size_t site_count,
                         const SiteRemeshingCriteria2D& criteria,
                         std::string_view context)
{
    if(criteria.require_at_least_three_sites && site_count < 3U) {
        vmm::error::throw_invalid_argument(context, "At least three sites are required for 2D remeshing.");
    }
}

void validate_minimum_distance(std::span<const vmm::domain::Point2D> sites,
                               const SiteRemeshingCriteria2D& criteria,
                               std::string_view context)
{
    if(criteria.minimum_site_distance < 0.0) {
        vmm::error::throw_invalid_argument(context, "Minimum site distance cannot be negative.");
    }
    if(criteria.minimum_site_distance == 0.0) {
        return;
    }

    const auto min_distance_squared = criteria.minimum_site_distance * criteria.minimum_site_distance;
    for(std::size_t i = 0U; i < sites.size(); ++i) {
        for(std::size_t j = i + 1U; j < sites.size(); ++j) {
            const auto dx = sites[i].x - sites[j].x;
            const auto dy = sites[i].y - sites[j].y;
            if((dx * dx) + (dy * dy) < min_distance_squared) {
                vmm::error::throw_invalid_argument(context, "Edited sites violate the minimum site distance.");
            }
        }
    }
}

[[nodiscard]] std::size_t checked_site_index(vmm::core::SiteId site_id,
                                             std::size_t site_count,
                                             std::string_view context)
{
    if(!vmm::core::is_valid(site_id) || site_id.value >= site_count) {
        vmm::error::throw_invalid_argument(context, "Site edit references an invalid site id.");
    }
    return static_cast<std::size_t>(site_id.value);
}

} // namespace

SiteEdit2D SiteEdit2D::insert(vmm::domain::Point2D point) noexcept
{
    return SiteEdit2D{SiteEditKindTraits::Insert, {}, point};
}

SiteEdit2D SiteEdit2D::remove(vmm::core::SiteId site_id) noexcept
{
    return SiteEdit2D{SiteEditKindTraits::Remove, site_id, {}};
}

SiteEdit2D SiteEdit2D::move(vmm::core::SiteId site_id, vmm::domain::Point2D point) noexcept
{
    return SiteEdit2D{SiteEditKindTraits::Move, site_id, point};
}

void SiteEditBatch2D::insert(vmm::domain::Point2D point)
{
    edits.push_back(SiteEdit2D::insert(point));
}

void SiteEditBatch2D::remove(vmm::core::SiteId site_id)
{
    edits.push_back(SiteEdit2D::remove(site_id));
}

void SiteEditBatch2D::move(vmm::core::SiteId site_id, vmm::domain::Point2D point)
{
    edits.push_back(SiteEdit2D::move(site_id, point));
}

std::size_t SiteEditBatch2D::size() const noexcept
{
    return edits.size();
}

bool SiteEditBatch2D::empty() const noexcept
{
    return edits.empty();
}

std::size_t SiteEditApplicationReport2D::edit_count() const noexcept
{
    return inserted_site_ids.size() + removed_site_ids.size() + moved_site_ids.size();
}

bool SiteEditApplicationReport2D::empty() const noexcept
{
    return edit_count() == 0U;
}

SiteEditApplication2D apply_site_edit_batch_2d(
    std::span<const vmm::domain::Point2D> initial_sites,
    const SiteEditBatch2D& batch,
    const vmm::domain::PolygonalDomain2D& domain,
    const SiteRemeshingCriteria2D& criteria)
{
    constexpr std::string_view context{"apply_site_edit_batch_2d"};
    std::vector<vmm::domain::Point2D> sites{initial_sites.begin(), initial_sites.end()};
    SiteEditApplicationReport2D report;

    for(const auto& site : sites) {
        validate_point(site, domain, criteria, context);
    }

    for(const auto& edit : batch.edits) {
        if(edit.kind == SiteEditKindTraits::Insert) {
            validate_point(edit.point, domain, criteria, context);
            sites.push_back(edit.point);
            report.inserted_site_ids.push_back(vmm::core::SiteId{
                static_cast<vmm::core::EntityIdValue>(sites.size() - 1U)});
        } else if(edit.kind == SiteEditKindTraits::Remove) {
            const auto index = checked_site_index(edit.site_id, sites.size(), context);
            sites.erase(sites.begin() + static_cast<std::ptrdiff_t>(index));
            report.removed_site_ids.push_back(edit.site_id);
        } else if(edit.kind == SiteEditKindTraits::Move) {
            const auto index = checked_site_index(edit.site_id, sites.size(), context);
            validate_point(edit.point, domain, criteria, context);
            sites[index] = edit.point;
            report.moved_site_ids.push_back(edit.site_id);
        } else {
            vmm::error::throw_invalid_argument(context, "Unsupported site edit kind.");
        }

        validate_site_count(sites.size(), criteria, context);
        validate_minimum_distance(sites, criteria, context);
    }

    validate_site_count(sites.size(), criteria, context);
    validate_minimum_distance(sites, criteria, context);
    return SiteEditApplication2D{std::move(sites), std::move(report)};
}

void SiteRemeshingRegistry2D::register_method(std::string id, SiteRemeshingMethod2D method)
{
    if(id.empty()) {
        vmm::error::throw_invalid_argument("SiteRemeshingRegistry2D", "Remeshing method id cannot be empty.");
    }
    if(!method) {
        vmm::error::throw_invalid_argument("SiteRemeshingRegistry2D", "Remeshing method cannot be empty.");
    }
    methods_[std::move(id)] = std::move(method);
}

bool SiteRemeshingRegistry2D::contains(std::string_view id) const
{
    return methods_.find(std::string{id}) != methods_.end();
}

SiteRemeshingResult2D SiteRemeshingRegistry2D::remesh(
    std::string_view id,
    std::span<const vmm::domain::Point2D> initial_sites,
    const SiteEditBatch2D& batch,
    const vmm::domain::PolygonalDomain2D& domain,
    const SiteRemeshingOptions2D& options) const
{
    const auto iterator = methods_.find(std::string{id});
    if(iterator == methods_.end()) {
        vmm::error::throw_invalid_argument("SiteRemeshingRegistry2D", "Unknown remeshing method.");
    }
    return iterator->second(initial_sites, batch, domain, options);
}

SiteRemeshingResult2D remesh_complete_finite_volume_voronoi_mesh_2d(
    std::span<const vmm::domain::Point2D> initial_sites,
    const SiteEditBatch2D& batch,
    const vmm::domain::PolygonalDomain2D& domain,
    const SiteRemeshingOptions2D& options)
{
    auto application = apply_site_edit_batch_2d(initial_sites, batch, domain, options.criteria);
    auto mesh = vmm::workflow::generate_complete_finite_volume_voronoi_mesh_2d(
        application.sites,
        domain,
        options.workflow);
    return SiteRemeshingResult2D{
        std::move(application.sites),
        std::move(application.report),
        std::move(mesh)};
}

SiteRemeshingRegistry2D default_site_remeshing_registry_2d()
{
    SiteRemeshingRegistry2D registry;
    registry.register_method(
        std::string{site_edit_rebuild_remeshing_method_2d_id},
        [](std::span<const vmm::domain::Point2D> initial_sites,
           const SiteEditBatch2D& batch,
           const vmm::domain::PolygonalDomain2D& domain,
           const SiteRemeshingOptions2D& options) {
            return remesh_complete_finite_volume_voronoi_mesh_2d(initial_sites, batch, domain, options);
        });
    return registry;
}

} // namespace vmm::remeshing
