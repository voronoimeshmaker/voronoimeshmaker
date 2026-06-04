Mesh API
========

``vmm::core::MeshTopology<Dim>``
   Structure-of-arrays topology container with CSR neighbour indexing.

Current data
------------

The topology stores:

* cell centroids;
* generator identifiers;
* neighbour offsets and neighbour ids;
* boundary flags.

Next direction
--------------

The next mesh layer should store explicit geometry:

* points;
* faces or edges;
* owner cells;
* neighbour cells;
* boundary patches;
* extensible tags.
