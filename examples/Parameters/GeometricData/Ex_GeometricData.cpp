
#include <iostream>


#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>  
        
using vmm::gtp::Point2D;
using vmm::gtp::Vector2D;
using vmm::gtp::Point3D;
using vmm::gtp::Vector3D;


int main() {
    // Criando o DataHolder
    vmm::GeometricDataHolder geometricDataHolder;

    // Adicionando diferentes tipos de dados
    geometricDataHolder.addValue("nPontos", 412);
    geometricDataHolder.addValue("dx", 0.1415);
    geometricDataHolder.addValue("dy", 0.3);
    geometricDataHolder.addValue("Point2D", Point2D(1.0, 2.0));
    geometricDataHolder.addValue("Vector2D", Vector2D(0.5, 1.5));
    geometricDataHolder.addValue("Point3D", Point3D(1.0, 2.0, 1));
    geometricDataHolder.addValue("Vector3D", Vector3D(0.5, 1.5, 5));

std::list<int> myIntList = {1, 2, 3, 4, 5};
    geometricDataHolder.addValue("MyIntList", myIntList);

std::list<Real> myRealList = {1.1, 2.2, 3.3, 4.4, 5.5};
    geometricDataHolder.addValue("myRealList", myRealList);

    // Imprimindo todas as variáveis
    std::cout << "\n\nValores armazenados no GeometricDataHolder:" << std::endl;
    std::cout << geometricDataHolder;

    std::cout << "\n\n\n";    
    // Recuperando valores de diferentes tipos
    if (auto intValue = geometricDataHolder.getValue<int>("nPontos")) {
        std::cout << "Valor de 'nPontos': " << *intValue << std::endl;
    }

    if (auto RealValue = geometricDataHolder.getValue<Real>("dx")) {
        std::cout << "Valor de 'RealVar': " << *RealValue << std::endl;
    }
    
    if (auto Point2DValue = geometricDataHolder.getValue<Point2D>("Point2D")) {
        std::cout << "Valor de 'Point2D': " << *Point2DValue << std::endl;
    }
    
    if (auto VecIntValue = geometricDataHolder.getValue<std::list<int>>("MyIntList")) {
        
        for (auto x : *VecIntValue) {
            std::cout << "VecInt = " << x << "\n";
        }
    }
    
    return 0;
}
