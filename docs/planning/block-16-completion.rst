Block 16 Completion: General 3D Domain Geometry
================================================

Goal
----

Block 16 replaces the reserved 3D domain headers with a usable first layer of
general 3D geometry. This layer is intentionally independent from any clipping
backend and can be used by future 3D site generation, import, audit, and
tessellation workflows.

Implemented scope
-----------------

* Added ``Box3D`` with volume, bounds, containment, and triangular surface
  conversion.
* Added ``Sphere3D`` with analytic volume, bounds, and containment.
* Added ``Cylinder3D`` as a vertical circular cylinder.
* Added ``Ellipsoid3D`` with axis-aligned radii.
* Added ``Tetrahedron3D`` with volume, containment, and triangular surface
  conversion.
* Added ``SurfaceMesh3D`` for lightweight triangular boundary surfaces.
* Added ``Polyhedron3D`` as a closed triangular surface wrapper with volume.
* Added open ``BoundarySurfaceType`` traits and ``SurfacePatch`` metadata.
* Added basic 3D point and surface transforms.
* Connected the new types to ``DomainTraits``, ``DomainConcept``, and the public
  aggregate header.

Tests
-----

The block replaces the placeholder 3D geometry test with checks for:

* box volume, bounds, containment, and triangulated surface volume;
* sphere, cylinder, and ellipsoid analytic volumes;
* tetrahedron volume, containment, and surface conversion;
* invalid 3D geometry rejection;
* point and surface transforms;
* the ``PolyhedralDomain`` concept.

Design notes
------------

Boundary-surface semantics use open traits rather than closed enums, matching
the extension policy used elsewhere in VMM. The 3D layer does not expose CGAL or
other backend types in public headers.
