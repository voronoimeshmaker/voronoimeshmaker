//==============================================================================
// File        : Remesher.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Site-edit remeshing API for 2D finite-volume Voronoi meshes.
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>
#include <vmm/domain/PolygonalDomain2D.hpp>
#include <vmm/remeshing/Criteria.hpp>
#include <vmm/workflow/FiniteVolumeVoronoiWorkflow2D.hpp>

namespace vmm::remeshing {

inline constexpr std::string_view site_edit_rebuild_remeshing_method_2d_id = "site-edit-rebuild";

struct SiteEditKind final {
    std::string_view name;
    std::uint8_t code;

    [[nodiscard]] friend constexpr bool operator==(SiteEditKind lhs, SiteEditKind rhs) noexcept
    {
        return lhs.code == rhs.code && lhs.name == rhs.name;
    }
};

struct SiteEditKindTraits final {
    static constexpr SiteEditKind Insert{"insert", 0U};
    static constexpr SiteEditKind Remove{"remove", 1U};
    static constexpr SiteEditKind Move{"move", 2U};
};

struct SiteEdit2D final {
    SiteEditKind kind{SiteEditKindTraits::Insert};
    vmm::core::SiteId site_id{};
    vmm::domain::Point2D point{};

    [[nodiscard]] static SiteEdit2D insert(vmm::domain::Point2D point) noexcept;
    [[nodiscard]] static SiteEdit2D remove(vmm::core::SiteId site_id) noexcept;
    [[nodiscard]] static SiteEdit2D move(vmm::core::SiteId site_id, vmm::domain::Point2D point) noexcept;
};

struct SiteEditBatch2D final {
    std::vector<SiteEdit2D> edits;

    void insert(vmm::domain::Point2D point);
    void remove(vmm::core::SiteId site_id);
    void move(vmm::core::SiteId site_id, vmm::domain::Point2D point);
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
};

struct SiteEditApplicationReport2D final {
    std::vector<vmm::core::SiteId> inserted_site_ids;
    std::vector<vmm::core::SiteId> removed_site_ids;
    std::vector<vmm::core::SiteId> moved_site_ids;

    [[nodiscard]] std::size_t edit_count() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
};

struct SiteEditApplication2D final {
    std::vector<vmm::domain::Point2D> sites;
    SiteEditApplicationReport2D report;
};

struct SiteRemeshingOptions2D final {
    SiteRemeshingCriteria2D criteria;
    vmm::workflow::CompleteFiniteVolumeVoronoiMesh2DOptions workflow;
};

struct SiteRemeshingResult2D final {
    std::vector<vmm::domain::Point2D> sites;
    SiteEditApplicationReport2D edit_report;
    vmm::workflow::CompleteFiniteVolumeVoronoiMesh2D mesh;
};

using SiteRemeshingMethod2D = std::function<SiteRemeshingResult2D(
    std::span<const vmm::domain::Point2D>,
    const SiteEditBatch2D&,
    const vmm::domain::PolygonalDomain2D&,
    const SiteRemeshingOptions2D&)>;

class SiteRemeshingRegistry2D final {
public:
    void register_method(std::string id, SiteRemeshingMethod2D method);
    [[nodiscard]] bool contains(std::string_view id) const;
    [[nodiscard]] SiteRemeshingResult2D remesh(
        std::string_view id,
        std::span<const vmm::domain::Point2D> initial_sites,
        const SiteEditBatch2D& batch,
        const vmm::domain::PolygonalDomain2D& domain,
        const SiteRemeshingOptions2D& options = {}) const;

private:
    std::unordered_map<std::string, SiteRemeshingMethod2D> methods_;
};

[[nodiscard]] SiteEditApplication2D apply_site_edit_batch_2d(
    std::span<const vmm::domain::Point2D> initial_sites,
    const SiteEditBatch2D& batch,
    const vmm::domain::PolygonalDomain2D& domain,
    const SiteRemeshingCriteria2D& criteria = {});

[[nodiscard]] SiteRemeshingResult2D remesh_complete_finite_volume_voronoi_mesh_2d(
    std::span<const vmm::domain::Point2D> initial_sites,
    const SiteEditBatch2D& batch,
    const vmm::domain::PolygonalDomain2D& domain,
    const SiteRemeshingOptions2D& options = {});

[[nodiscard]] SiteRemeshingRegistry2D default_site_remeshing_registry_2d();

} // namespace vmm::remeshing
