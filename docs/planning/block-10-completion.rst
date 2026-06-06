Block 10 Completion: GIS Vector Input and CRS Handling
======================================================

Purpose
-------

Block 10 introduces the GIS layer boundary for VMM. The goal is not to make
every shapefile workflow complete at once; it is to create a stable public API
that can read real vector polygon domains when GDAL/OGR is available while
keeping external GIS types out of VMM public headers.

Implemented Artifacts
---------------------

* ``vmm::gis::CoordinateReferenceSystem`` stores CRS authority, code, name, WKT,
  and whether the CRS is geographic.
* ``vmm::gis::GeoVectorDomain2D`` combines a ``PolygonalDomain2D``, source CRS,
  working CRS, and boundary patch metadata.
* ``vmm::gis::read_gis_polygonal_domain_2d`` is the public vector reader entry
  point.
* ``vmm::gis::gis_vector_backend_available`` and
  ``vmm::gis::gis_crs_transform_backend_available`` expose runtime backend
  availability without requiring callers to include GDAL or PROJ headers.
* CMake detects GDAL/OGR and PROJ optionally. If GDAL is absent, the reader
  throws a clear unsupported-operation error instead of creating link or include
  failures.
* Boundary patch type text from GIS attributes is mapped into the existing open
  ``BoundaryPatchType`` traits without adding enums.

Design Decisions
----------------

GDAL/OGR and PROJ are implementation backends, not public API dependencies. The
public API uses VMM-owned geometry, CRS, and patch structures. This keeps future
backend replacement possible and avoids leaking external handles through the
rest of the mesh pipeline.

The first reader implementation focuses on polygon and multipolygon domains. It
selects the largest polygon as the exterior and records remaining polygon rings
and interior rings as holes/islands. More advanced feature classification,
topology cleaning, overlay validation, and CRS transformation policy remain
future GIS-block work.

When a working CRS is supplied, it is recorded in the returned object. Full
coordinate transformation is enabled only when the PROJ/GDAL backend is present
and will be expanded in later import-validation work.

Exit Criteria
-------------

Block 10 is complete when:

* VMM has a public ``vmm::gis`` namespace for vector domain import;
* GDAL/OGR and PROJ discovery is optional and isolated in CMake;
* public GIS headers do not expose GDAL/OGR/PROJ types;
* unavailable backends fail explicitly at runtime;
* CRS metadata and boundary patch rings can be represented by VMM-owned types;
* API/status/planning documentation records the GIS layer;
* build, tests, Sphinx, and whitespace checks pass.
