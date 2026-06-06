Block 11 Completion
===================

Scope
-----

Block 11 adds the first raster bathymetry/topography layer after the GIS vector
and CRS foundation. The implementation keeps raster data in VMM-owned structures
and treats GDAL as an optional backend.

Implemented
-----------

* ``vmm::raster::RasterScalarField2D`` stores a regular 2D scalar raster with
  row-major values, no-data metadata, grid-centre geometry, and CRS metadata.
* ``vmm::raster::RasterSampleMethod`` uses traits for nearest-neighbour and
  bilinear sampling rather than a closed enum.
* ``vmm::raster::sample_cell_centres`` samples bathymetry/topography values at
  canonical finite-volume cell centres and records a validity mask.
* ``vmm::raster::read_gis_raster_scalar_field_2d`` reads GDAL-supported rasters
  when GDAL is available and reports an explicit unsupported operation otherwise.
* CMake discovers the same optional GDAL backend for vector and raster GIS
  workflows without exposing GDAL types in public headers.
* Unit tests cover geometry mapping, storage validation, interpolation,
  no-data handling, cell-centre sampling, and unsupported-backend behaviour.

Deliberate limits
-----------------

* Rotated raster geotransforms are rejected for now.
* CRS transformation is recorded at the API boundary but not resampled yet.
* Raster values are sampled onto the mesh; they are not written to HDF5
  packages in this block.

Next dependencies
-----------------

Later import-validation work can combine vector CRS metadata, raster CRS
metadata, and mesh metric checks before accepting a production GIS import.
