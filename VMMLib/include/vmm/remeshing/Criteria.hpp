//==============================================================================
// File        : Criteria.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Criteria for site-edit remeshing workflows.
//==============================================================================
#pragma once

//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/Types.hpp>

namespace vmm::remeshing {

struct SiteRemeshingCriteria2D final {
    vmm::core::Real minimum_site_distance{0.0};
    bool require_sites_inside_domain{true};
    bool require_at_least_three_sites{true};
};

} // namespace vmm::remeshing
