VoronoiMeshMaker Documentation
==============================

.. raw:: html

   <section class="vmm-home">
     <div class="vmm-hero vmm-hero-home">
       <div class="vmm-hero-copy">
         <span class="vmm-kicker">Audited Voronoi finite-volume mesh preparation</span>
         <h1>VoronoiMeshMaker</h1>
         <p class="vmm-lead">A C++20 mesh-preparation library for domains, sites, clipped Voronoi cells, canonical finite-volume tables, mesh audits and export-ready connectivity.</p>
       </div>
       <div class="vmm-hero-visual" aria-label="VoronoiMeshMaker identity">
         <img src="_static/logo/vmm-logo-dark.jpg" alt="VoronoiMeshMaker logo" />
         <div class="vmm-mesh-signal">
           <span></span><span></span><span></span><span></span><span></span><span></span>
         </div>
       </div>
     </div>

     <div class="vmm-split-band">
       <div class="vmm-panel">
         <h2>VMM Responsibility</h2>
         <p>VMM owns mesh generation, site policies, remeshing plans, face reconstruction, boundary patches, mesh-quality audits and solver-neutral connectivity.</p>
       </div>
     </div>

     <div class="vmm-workflow" aria-label="VMM workflow">
       <div class="vmm-workflow-step"><strong>GIS / Domain</strong><span>metric geometry and patches</span></div>
       <div class="vmm-workflow-step"><strong>Sites</strong><span>resolution and generator policy</span></div>
       <div class="vmm-workflow-step"><strong>Voronoi</strong><span>clipping and FV reconstruction</span></div>
       <div class="vmm-workflow-step"><strong>Audit</strong><span>index, topology and geometry gates</span></div>
       <div class="vmm-workflow-step"><strong>Export</strong><span>diagnostics and mesh packages</span></div>
     </div>

     <div class="vmm-entry-grid" aria-label="Documentation entry points">
       <a class="vmm-entry-card" href="en-gb/index.html">
         <strong>Default documentation</strong>
         <span>British English overview, workflow, developer notes and API references.</span>
       </a>
       <a class="vmm-entry-card" href="pt-br/index.html">
         <strong>Português do Brasil</strong>
         <span>Visão geral, fluxo de trabalho, notas de desenvolvimento e APIs.</span>
       </a>
     </div>
   </section>

.. admonition:: Extensibility policy
   :class: vmm-policy

   Functional categories that may grow with new workflows must use traits,
   tags, identifiers or registries. Closed enums are reserved only for structural
   compile-time cases such as the 2D/3D dimension selector.

.. admonition:: Validation gate
   :class: vmm-gate

   Every implementation block must leave a testable artifact: canonical data
   tables, focused audits, stable text diagnostics, or export validation.

.. toctree::
   :hidden:
   :maxdepth: 2

   languages
   tutorials/index
   api/index
   architecture/index
   planning/index
   developer/index
   error-codes/index
   status/index
   design_system
