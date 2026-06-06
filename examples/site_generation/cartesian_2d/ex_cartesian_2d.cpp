//==============================================================================
// c++ includes
//==============================================================================
#include <iostream>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/CartesianSitePattern2D.hpp>

int main()
{
    const auto domain = vmm::domain::make_rectangle_2d(2.0, 2.0);
    const auto sites = vmm::site_generation::make_cartesian_sites_2d(domain, 0.5);

    std::cout << "cartesian_sites=" << sites.size() << '\n';
    return 0;
}
