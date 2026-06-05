# Project scope

## What VMM is

VoronoiMeshMaker is a C++20 mesh-generation and mesh-packaging library for GIS-driven Voronoi finite-volume workflows.

It should prepare geometric and geospatial domains, generate or manipulate sites, build clipped Voronoi tessellations, reconstruct explicit finite-volume connectivity, audit the mesh and export mesh packages for MohidNG and diagnostic tools.

## What VMM is not

VMM is not a PDE solver.

VMM must not own:

- physical equations,
- hydrodynamic models,
- transport models,
- time integration,
- PETSc matrix assembly,
- Trilinos matrix assembly,
- nonlinear solver logic,
- MohidNG field evolution.

Those belong to MohidNG or another consumer.

## Main workflow

GIS or synthetic geometry
    -> domain preparation
    -> CRS transformation
    -> site generation
    -> tessellation
    -> clipping
    -> finite-volume mesh reconstruction
    -> index/connectivity/geometry audit
    -> anomaly detection
    -> solver-neutral stencil graph
    -> HDF5 MohidNG package
    -> VTK/VTU/audit text diagnostics

## First milestone

The first useful milestone is not full GIS or 3D.

The first useful milestone is:

- `FiniteVolumeMesh2D`,
- nodes,
- faces,
- cells,
- owner,
- neighbour,
- boundary patches,
- index audit,
- connectivity audit,
- text audit writer,
- HDF5 writer compatible with MohidNG.

## Later milestones

After the 2D finite-volume core is stable:

- holes and islands,
- boundary projections and anomalies,
- solver-neutral stencil graph,
- GIS input,
- raster bathymetry,
- remeshing by site editing,
- 3D extrusion,
- optional 3D Voronoi backends such as Voro++,
- optional geophysical backends such as JIGSAW.
