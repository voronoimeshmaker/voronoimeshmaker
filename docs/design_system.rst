Design System
=============

Identity
--------

VoronoiMeshMaker uses a minimalist cracked-earth / Voronoi visual direction.
The documentation follows clear language entry points, a restrained technical 
hero, workflow strips, responsibility boundaries and validation gates, adapted 
to the VMM palette and logo.

Language Scope
--------------

The VMM web documentation has two public language entry points:

* British English, under ``en-gb``;
* Português do Brasil, under ``pt-br``.

Design Stack
------------

The current repository build uses:

* Sphinx;
* reStructuredText;
* Alabaster, until the PyData Sphinx Theme dependency is introduced;
* custom CSS in ``docs/_static/css/vmm.css``;
* the VMM project logo from ``logo/vmm_logo_dark_git.jpg``.

Palette
-------

Deep Slate
   ``#263640`` for primary dark surfaces and ink.

Off White
   ``#F5F7F8`` for light backgrounds and text over dark surfaces.

Steel Blue
   ``#4C6A7F`` for secondary accents and borders.

Cool Graphite
   ``#3A434B`` for dark panels and code surfaces.

Teal Accent
   ``#3FA7A0`` for links, highlights and restrained calls to action.

Core Components
---------------

The documentation home page uses:

* ``.vmm-hero`` for the first-viewport project identity;
* ``.vmm-button`` for language entry points;
* ``.vmm-split-band`` for responsibility boundaries;
* ``.vmm-workflow`` for the mesh-preparation pipeline;
* ``.vmm-entry-card`` for bilingual documentation paths;
* ``.vmm-policy`` for extensibility-policy notices;
* ``.vmm-gate`` for validation-gate notices.

Documentation Rules
-------------------

* The first page must identify VMM as a mesh-preparation library for finite-volume workflows.
* Pages should distinguish implemented APIs from planned blocks.
* Extension policies must be documented next to developer-facing design notes.
* Public mesh concepts should be explained through canonical tables and audit
  gates before discussing backend algorithms.
* Web pages must not show personal authorship names.
