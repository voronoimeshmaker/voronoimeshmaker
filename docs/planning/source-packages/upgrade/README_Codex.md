# VoronoiMeshMaker, upgrade plan for MohidNG integration

This package is a working specification for Codex. It summarises the design discussion about upgrading VoronoiMeshMaker, VMM, so that it can support MohidNG, GIS-driven domains, finite-volume connectivity, tessellation, remeshing, auditability and future 3D workflows.

The key design decision is:

VMM is not a solver. VMM produces mesh geometry, finite-volume topology, patches, audit data and solver-neutral stencil connectivity. MohidNG, or another consumer, builds physical operators and adapts the data to PETSc, Trilinos or another linear algebra backend.

The second key decision is:

The VMM core must be Data-Oriented Design, DOD. Use contiguous arrays, SoA tables, offsets, spans and batch operations. Avoid per-cell object graphs, pointer networks and runtime polymorphism inside hot loops.

Recommended reading order:

1. `00_codex_master_prompt.md`
2. `01_conversation_context.md`
3. `02_project_scope.md`
4. `03_functional_requirements.md`
5. `04_non_functional_requirements.md`
6. `05_architecture_overview.md`
7. `06_repository_layout.md`
8. `07_data_model_dod.md`
9. `08_tessellation_and_remeshing.md`
10. `09_gis_support.md`
11. `10_audit_and_validation.md`
12. `11_solver_neutral_connectivity.md`
13. `12_mohidng_hdf5_contract.md`
14. `13_text_audit_format.md`
15. `14_3d_strategy.md`
16. `15_dependency_architecture_lessons.md`
17. `16_block_roadmap.md`
18. `17_codex_implementation_tasks.md`
19. `18_open_questions.md`

Do not try to implement everything at once. Start with Block 1 to Block 5: DOD finite-volume mesh 2D, face reconstruction, index audit, text audit writer and MohidNG HDF5 writer.
