Relation To Mohid-NG
====================

VoronoiMeshMaker and Mohid-NG are deliberately separated.

VMM owns mesh concerns: geometry, sites, Voronoi tessellation, reconstruction of
finite-volume connectivity, boundary patch metadata, mesh-quality diagnostics
and mesh package export.

Mohid-NG owns model concerns: fields, operators, time integration, physical
models, coupling, diagnostics and reproducible simulation workflows.

This separation keeps mesh-generation backends and GIS dependencies outside the
solver core while giving Mohid-NG a validated mesh package.
