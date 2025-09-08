#include <iostream>
#include <cmath>

#include <VoronoiMeshMaker/Core/type.h>
#include <VoronoiMeshMaker/Core/constants.h>

int main() {
    using namespace vmm;

    // Triângulo CCW: área esperada = 0.5
    gtp::Polygon2D tri;
    tri.push_back(gtp::Point2D(0.0, 0.0));
    tri.push_back(gtp::Point2D(1.0, 0.0));
    tri.push_back(gtp::Point2D(0.0, 1.0));

    const double area = CGAL::to_double(tri.area());
    std::cout << "PI       = " << static_cast<double>(PI) << "\n";
    std::cout << "EPSILON  = " << static_cast<double>(EPSILON) << "\n";
    std::cout << "ZERO     = " << static_cast<double>(ZERO) << "\n";
    std::cout << "Tri area = " << area << " (expect 0.5)\n";

    if (std::abs(area) < static_cast<double>(ZERO)) {
        std::cout << "Area is ~zero by ZERO tolerance\n";
    }
    return 0;
}
