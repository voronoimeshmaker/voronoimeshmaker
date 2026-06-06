//==============================================================================
// c++ includes
//==============================================================================
#include <iostream>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/Box3D.hpp>

int main()
{
    const auto box = vmm::domain::make_box_3d(2.0, 3.0, 4.0);
    const auto surface = box.to_polyhedron().surface();

    std::cout << "volume=" << box.volume() << '\n';
    std::cout << "surface_vertices=" << surface.vertex_count() << '\n';
    std::cout << "surface_faces=" << surface.face_count() << '\n';
    return 0;
}
