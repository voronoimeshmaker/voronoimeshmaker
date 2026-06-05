Canonical Finite-Volume Mesh
============================

The canonical mesh is the internal contract between generation, audit and
export. It uses data-oriented tables for nodes, cells, faces, boundary patches
and cell-face connectivity.

Audits are mandatory gates. The current index audit checks storage consistency,
owner and neighbour ranges, patch references, node references and cell-face
offsets. Later blocks add reciprocity, geometry and diagnostic text output.
