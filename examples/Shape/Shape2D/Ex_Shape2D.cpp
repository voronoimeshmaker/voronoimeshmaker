
#include <iostream>


#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>  
//#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>  
        
using vmm::gtp::Point2D;
using vmm::gtp::Vector2D;



int main() {
    // Criando o DataHolder
    vmm::GeometricDataHolder geometricDataHolder;

    // Adicionando diferentes tipos de dados
    geometricDataHolder.addValue("nPontos", 42);
    geometricDataHolder.addValue("nDados", 412);
    geometricDataHolder.addValue("dx", 0.1415);
    geometricDataHolder.addValue("dy", 0.3);
    geometricDataHolder.addValue("center", Point2D(1.0, 2.0));
    geometricDataHolder.addValue("center1", Point2D(-1.0, 2.0));
    geometricDataHolder.addValue("displacement", Vector2D(0.5, 1.5));
    geometricDataHolder.addValue("displacement1", Vector2D(1.5, -1.5));

    // Imprimindo todas as variáveis
    std::cout << "Valores armazenados no GeometricDataHolder:" << std::endl;
    std::cout << geometricDataHolder;

    
    
    
    
    // Recuperando valores de diferentes tipos
    if (auto intValue = geometricDataHolder.getValue<int>("intVar")) {
        std::cout << "Valor de 'intVar': " << *intValue << std::endl;
    }

    if (auto RealValue = geometricDataHolder.getValue<Real>("RealVar")) {
        std::cout << "Valor de 'RealVar': " << *RealValue << std::endl;
    }

    if (auto pointValue = geometricDataHolder.getValue<Point2D>("pointVar")) {
        std::cout << "Valor de 'pointVar': (" << pointValue->x() << ", " << pointValue->y() << ")" << std::endl;
    }

    if (auto vectorValue = geometricDataHolder.getValue<Vector2D>("vectorVar")) {
        std::cout << "Valor de 'vectorVar': (" << vectorValue->x() << ", " << vectorValue->y() << ")" << std::endl;
    }

    // Imprimindo os tipos dos dados armazenados usando DataTraits
    std::cout << "\nTipos de dados armazenados no DataHolder:" << std::endl;
    geometricDataHolder.printTypes();

    return 0;
}
