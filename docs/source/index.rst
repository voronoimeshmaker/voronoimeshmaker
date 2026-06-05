VoronoiMeshMaker Documentation
==============================

.. raw:: html

   <section class="vmm-home">
     <div class="vmm-hero">
       <div class="vmm-hero-copy">
         <span class="vmm-kicker">Canonical Voronoi finite-volume meshes</span>
         <h1>VoronoiMeshMaker</h1>
         <p class="vmm-lead">A C++20 mesh-preparation library that owns Voronoi generation, finite-volume connectivity reconstruction, mesh audits and export packages consumed by Mohid-NG and diagnostic tools.</p>
         <div class="vmm-hero-actions" aria-label="Documentation entry points">
           <a class="vmm-button primary" href="en-gb/index.html">English</a>
           <a class="vmm-button" href="pt-br/index.html">Português do Brasil</a>
           <a class="vmm-button" href="pt-pt/index.html">Português de Portugal</a>
         </div>
       </div>
       <div class="vmm-hero-visual" aria-label="VoronoiMeshMaker identity">
         <img src="_static/logo/vmm-logo.svg" alt="VoronoiMeshMaker logo" />
         <div class="vmm-mesh-signal">
           <span></span><span></span><span></span><span></span><span></span><span></span>
         </div>
       </div>
     </div>

     <div class="vmm-split-band">
       <div class="vmm-panel">
         <h2>VMM Responsibility</h2>
         <p>VMM prepares audited mesh packages: domains, sites, tessellation, finite-volume tables, boundary patches, mesh diagnostics and export-ready connectivity.</p>
       </div>
       <div class="vmm-panel">
         <h2>Mohid-NG Responsibility</h2>
         <p>Mohid-NG consumes validated Voronoi mesh packages and focuses on fields, finite-volume operators, physical models and reproducible simulation workflows.</p>
       </div>
     </div>

     <div class="vmm-workflow" aria-label="VMM workflow">
       <div class="vmm-workflow-step"><strong>Domain</strong><span>geometry and GIS context</span></div>
       <div class="vmm-workflow-step"><strong>Sites</strong><span>resolution and generator policy</span></div>
       <div class="vmm-workflow-step"><strong>Voronoi</strong><span>clipping and reconstruction</span></div>
       <div class="vmm-workflow-step"><strong>Audit</strong><span>index, topology and geometry gates</span></div>
       <div class="vmm-workflow-step"><strong>Export</strong><span>Mohid-NG and diagnostics</span></div>
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

   English <en-gb/index>
   Português do Brasil <pt-br/index>
   Português de Portugal <pt-pt/index>
   tutorials/index
   api/index
   architecture/index
   planning/index
   developer/index
   error-codes/index
   status/index
   design_system
