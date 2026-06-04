Domain API
==========

Implemented classes and functions
---------------------------------

``vmm::domain::Polygon2D``
   Validated polygonal ring used to represent planar geometry.
   It computes signed area, area, bounds, translation, rotation, and rejects
   degenerate or self-intersecting rings.

``vmm::domain::PolygonalDomain2D``
   Planar domain composed of one exterior polygon and optional hole polygons.
   The current clipped Voronoi cell construction uses the exterior ring; hole
   support for individual cell geometry is the next planned extension.

``vmm::domain::Rectangle2D``
   Factory geometry for axis-aligned rectangles, with ``to_polygon()`` and
   ``to_domain()`` helpers.

``vmm::domain::Triangle2D``
   Factory geometry for triangular domains.

``vmm::domain::Ellipse2D``
   Polygonal approximation of an ellipse.

``vmm::domain::Transform2D``
   Rigid transform helper for translating and rotating polygonal domains.

``vmm::domain::PlanarCell2D``
   Rich cell geometry with an exterior ring, optional holes, generator metadata,
   area, centroid, and boundary classification. ``VoronoiCell2D`` is currently
   an alias for this type.

``vmm::domain::DomainFactoryRegistry``
   Open registry-based factory. It uses string identifiers instead of closed
   enums, allowing new domain constructors to be registered without changing
   the core API.

Planned classes
---------------

The 3D domain headers are present as architectural placeholders. They should
be documented as implemented only after their geometry and tests are complete.
