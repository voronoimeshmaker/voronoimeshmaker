Site Generation API
===================

Terminology
-----------

The public English term used by the library is ``site``. In Portuguese this is
the *ponto gerador* of the Voronoi cell.

Implemented classes and functions
---------------------------------

``vmm::site_generation::HexagonalSitePattern2D``
   Generates a hexagonal arrangement of sites inside a polygonal 2D domain.

``vmm::site_generation::make_hexagonal_sites_2d(domain, spacing)``
   Convenience function for the common hexagonal case.

``vmm::site_generation::SitePattern2D``
   Concept for extensible 2D site patterns.

``vmm::site_generation::SiteFactoryRegistry2D``
   Open factory for site generation. It avoids enums and accepts user
   registered patterns.

Planned patterns
----------------

Cartesian, random, triangular II, triangular IV, radial, and 3D patterns have
reserved files. They should be filled once their precise parameter contracts
are chosen.
