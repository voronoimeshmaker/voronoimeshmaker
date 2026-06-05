# GIS support

## Purpose

GIS support is needed so that VMM can generate meshes for realistic hydraulic and environmental domains.

Examples:

- rivers,
- estuaries,
- coastal domains,
- lakes,
- islands,
- internal obstacles,
- boundary attributes,
- bathymetry/topography rasters,
- refinement regions.

## Essential libraries

Use these as backend dependencies, not public API types:

```text
GDAL/OGR
    vector and raster I/O

PROJ
    coordinate reference transformations

GEOS
    geometry validation, cleaning, overlay and predicates
```

## Optional libraries

Future optional support:

```text
GeographicLib
    geodesic calculations and local coordinate systems

PDAL
    point clouds and LiDAR/bathymetric point sets

NetCDF/CF
    oceanographic and environmental fields

S2 Geometry
    global spherical geometry
```

## GeoDomain2D

Proposed structure:

```cpp
struct GeoDomain2D {
    CoordinateReferenceSystem source_crs;
    CoordinateReferenceSystem working_crs;

    Polygon2D outer_boundary;
    std::vector<DomainHole2D> holes;

    std::vector<BoundarySegment2D> boundary_segments;
    std::vector<RefinementRegion2D> refinement_regions;
    std::vector<Polyline2D> constraint_lines;

    std::optional<RasterField2D> bathymetry;
    std::optional<RasterField2D> topography;
};
```

## Domain holes

```cpp
struct DomainHole2D {
    HoleId id;
    Polygon2D boundary;
    BoundaryPatchId patch_id;
    std::string name;
};
```

An island is a `DomainHole2D`.

## CRS rule

Do not compute finite-volume geometry directly in latitude/longitude unless a geodetic mode is explicitly implemented.

Default workflow:

```text
read GIS data
    -> detect source CRS
    -> transform to metric working CRS
    -> clean geometry
    -> generate mesh
    -> store source and working CRS in metadata
```

## GIS cleaning

The GIS layer should validate or prepare:

- closed rings,
- ring orientation,
- self-intersections,
- slivers,
- near-duplicate vertices,
- short segments,
- holes inside outer boundary,
- patch attributes,
- open boundary segments.

## Patch attributes

Boundary patches may come from GIS attributes such as:

```text
name
type
boundary_condition
feature_id
source_layer
```

The VMM should convert these to `BoundaryPatchTable`.
