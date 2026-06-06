Block 15 Completion: Raster-to-Export Fields and CRS Validation
================================================================

Goal
----

Block 15 connects the raster sampling work to the MohidNG-oriented export path
and adds an explicit CRS compatibility check for workflows that combine vector
domains, raster fields, and package metadata.

Implemented scope
-----------------

* Added optional raster cell-field export to ``write_mohidng_hdf5_mesh_2d``.
* Added ``MohidNGHDF5Writer2DOptions::raster_samples`` and
  ``raster_field_name``.
* Wrote sampled raster values to ``/cell_fields/value``.
* Wrote matching ``/cell_fields/cell_id`` and ``/cell_fields/valid`` arrays.
* Stored the field name as HDF5 metadata on ``/cell_fields``.
* Validated that raster samples have consistent storage, match the mesh cell
  count, use row-aligned cell IDs, and contain binary validity flags.
* Added CRS compatibility helpers in ``vmm::gis``.

CRS policy
----------

``coordinate_reference_systems_compatible`` accepts empty CRS descriptors because
an unknown CRS cannot prove a mismatch. When both sides provide identifiers,
comparison is case-insensitive. When identifiers are unavailable but WKT exists
on both sides, exact WKT equality is required.

Tests
-----

The block adds regression coverage that:

* writes a 2D MohidNG HDF5 package with raster samples;
* reads back raster field values, validity flags, cell ids, and field metadata;
* rejects raster samples with wrong cell ids;
* accepts equivalent CRS identifiers with different case;
* rejects incompatible known CRS identifiers.

Design notes
------------

Raster fields remain optional. Existing callers of the 2D HDF5 writer keep the
same behaviour unless they pass sampled raster data through the options object.
The CRS helpers live with the GIS API because they compare CRS descriptors
rather than mesh tables, and they avoid introducing a closed CRS enum.
