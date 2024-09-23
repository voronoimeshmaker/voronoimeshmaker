Examples
========

Here are some usage examples for the **VoronoiMeshMaker** library:

Basic Usage Example
-------------------

This is a simple example that demonstrates how to generate a 2D Voronoi mesh.

```cpp
// Include necessary headers
#include <VoronoiMeshMaker/VoronoiMeshMaker.h>

int main() {
    // Create a Voronoi mesh for a set of points
    VoronoiMesh mesh = VoronoiMeshMaker::generate2D(points);
    return 0;
}
