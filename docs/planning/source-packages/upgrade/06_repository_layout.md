# Proposed repository layout

This is a target layout. It can be introduced progressively.

```text
include/
    vmm/
        core/
            Id.hpp
            StrongTypes.hpp
            Span.hpp
            Status.hpp
            ErrorCode.hpp

        geometry/
            Point.hpp
            Vector.hpp
            Segment.hpp
            Polygon.hpp
            BoundingBox.hpp
            CoordinateReferenceSystem.hpp

        gis/
            GeoDomain2D.hpp
            GeoLayer.hpp
            GeoFeature.hpp
            GisReaderOptions.hpp
            GisReader.hpp
            CrsTransformer.hpp

        sites/
            SiteTable2D.hpp
            SiteTable3D.hpp
            SiteGenerator2D.hpp
            SiteEditBatch2D.hpp
            BoundarySiteSampler2D.hpp

        mesh/
            NodeTable2D.hpp
            CellTable2D.hpp
            FaceTable2D.hpp
            BoundaryPatchTable.hpp
            CellFaceConnectivity.hpp
            FiniteVolumeMesh2D.hpp
            FiniteVolumeMesh3D.hpp
            MeshRemap2D.hpp

        tessellation/
            TessellationOptions.hpp
            TessellationAlgorithmConcepts.hpp
            TessellationAlgorithmTraits.hpp
            TessellationRegistry.hpp
            CgalClippedVoronoi2D.hpp

        remeshing/
            RemeshingOptions.hpp
            RemeshingPlan2D.hpp
            RemeshingAlgorithmConcepts.hpp
            RemeshingAlgorithmTraits.hpp
            RemeshingRegistry.hpp
            BoundarySamplingRemesher2D.hpp
            BoundaryProjectionAnomalyRemesher2D.hpp
            InternalFaceAnomalyRemesher2D.hpp

        audit/
            MeshAuditReport.hpp
            MeshIndexAudit2D.hpp
            MeshConnectivityAudit2D.hpp
            MeshGeometryAudit2D.hpp
            BoundaryProjectionAudit2D.hpp
            InternalFaceOrthogonalityAudit2D.hpp

        stencil/
            FiniteVolumeStencilGraph2D.hpp
            BoundaryStencilTable2D.hpp
            StencilGraphBuilder2D.hpp

        io/
            TextMeshAuditWriter2D.hpp
            MohidNgMeshPackageWriter2D.hpp
            VtkWriter2D.hpp
            VtuWriter2D.hpp

        backend/
            cgal/
                CgalKernel.hpp
                CgalDelaunay2D.hpp
                CgalPolygonClipper2D.hpp
            gdal/
                GdalVectorReader.hpp
                GdalRasterReader.hpp
            proj/
                ProjTransformer.hpp
            geos/
                GeosCleaner.hpp
            voro/
                VoroPlusPlusBackend3D.hpp

src/
    core/
    geometry/
    gis/
    sites/
    mesh/
    tessellation/
    remeshing/
    audit/
    stencil/
    io/
    backend/
        cgal/
        gdal/
        proj/
        geos/
        voro/

tests/
    unit/
        mesh/
        audit/
        tessellation/
        stencil/
        io/
    integration/
        mohidng_hdf5/
        island_domain/
        text_audit/

examples/
    01_simple_rectangle/
    02_domain_with_island/
    03_text_audit/
    04_mohidng_hdf5_export/
    05_solver_neutral_stencil/

docs/
    source/
        index.md
        en-gb/
            overview/
            getting_started/
            workflow/
            theory/
            api/
            file_formats/
            validation/
            developer/
```

## Important rule

Do not create a per-cell object graph. The layout may contain classes, but hot mesh data must live in tables.
