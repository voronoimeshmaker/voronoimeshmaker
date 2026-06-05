Mesh Generator Survey
=====================

The external survey covers Gmsh, CGAL, Triangle, JIGSAW, MMG, DistMesh,
snappyHexMesh, OceanMesh2D, MOAB, CGNS and meshio.

The design lesson for VMM is to keep a canonical mesh model, isolate optional
backends, prefer tags and traits over closed enums, introduce size fields before
advanced remeshing controls and keep audits as explicit pipeline gates.

No new mesh-generation feature is implied by this survey.
