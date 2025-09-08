#include <iostream>
#include <cmath>

#include <VoronoiMeshMaker/Core/type.h>
#include <VoronoiMeshMaker/Core/constants.h>
#include <VoronoiMeshMaker/IO/IOHelpers.hpp>


int main() {
    using namespace vmm;


    // ---- HeaderPrinter (default license text) -------------------------------
    vmm::io::HeaderPrinter hp_default(
        "ex_core.cpp",
        "VoronoiMeshMaker Team",
        "1.0.0",
        "Exemplo de uso das classes da pasta Core"
    );
    std::cout << hp_default << "\n\n";


    std::cout << "\n\n" << vmm::io::PrintLine() ;
    std::cout << vmm::io::CommentPrinter("Imprimindo valores constantes da biblioteca");
    std::cout << vmm::io::PrintLine() << "\n\n" ;
       
    // Triângulo CCW: área esperada = 0.5
    gtp::Polygon2D tri;
    tri.push_back(gtp::Point2D(0.0, 0.0));
    tri.push_back(gtp::Point2D(1.0, 0.0));
    tri.push_back(gtp::Point2D(0.0, 1.0));

    const Real area = CGAL::to_double(tri.area());
    std::cout << "PI       = " << static_cast<Real>(PI) << "\n";
    std::cout << "EPSILON  = " << static_cast<Real>(EPSILON) << "\n";
    std::cout << "ZERO     = " << static_cast<Real>(ZERO) << "\n";
    std::cout << "Tri area = " << area << " (expect 0.5)\n";

    if (std::abs(area) < static_cast<Real>(ZERO)) {
        std::cout << "Area is ~zero by ZERO tolerance\n";
    }


    std::cout << "\n\n" << vmm::io::PrintLine() ;
    std::cout << vmm::io::CommentPrinter("Programa ex_core.cpp encerrado corretamente");
    std::cout << vmm::io::PrintLine() << "\n\n" ;
    return 0;
}
