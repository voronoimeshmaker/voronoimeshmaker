Examples
========

Here are some examples demonstrating how to use VoronoiMeshMaker.

Basic Voronoi Mesh
------------------

This example shows how to generate a simple 2D Voronoi mesh:

.. code-block:: cpp

   #include <VoronoiMeshMaker/Shape2D/Rectangle.h>
   #include <VoronoiMeshMaker/VoronoiMeshMaker.h>

   int main() {
       VoronoiMeshMaker::Rectangle rect(10, 20);
       VoronoiMeshMaker::VoronoiMeshMaker maker;
       auto mesh = maker.createMesh(rect);
       return 0;
   }

Advanced 3D Voronoi Mesh
------------------------

This example shows how to generate a 3D Voronoi mesh:

.. code-block:: cpp

   #include <VoronoiMeshMaker/Shape3D/Cube.h>
   #include <VoronoiMeshMaker/VoronoiMeshMaker.h>

   int main() {
       VoronoiMeshMaker::Cube cube(10);
       VoronoiMeshMaker::VoronoiMeshMaker maker;
       auto mesh = maker.createMesh(cube);
       return 0;
   }
