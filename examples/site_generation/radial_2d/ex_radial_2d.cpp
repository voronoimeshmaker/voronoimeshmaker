//==============================================================================
// c++ includes
//==============================================================================
#include <iostream>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Rectangle2D.hpp>
#include <vmm/site_generation/RadialSitePattern2D.hpp>

int main()
{
    const auto domain = vmm::domain::make_rectangle_2d(4.0, 4.0);
    const auto sites = vmm::site_generation::RadialSitePattern2D{0.5, 12U}.generate(domain);

    std::cout << "radial_sites=" << sites.size() << '\n';
    return 0;
}
