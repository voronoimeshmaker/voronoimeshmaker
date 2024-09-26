
#include <iostream>

//#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

#include <VoronoiMeshMaker/Parameters/GeometricDataHolder.h>  
#include <VoronoiMeshMaker/Shape/ShapeFactory.h>

#include <VoronoiMeshMaker/Shape/Shape2D/Ellipse.h>  
#include <VoronoiMeshMaker/Shape/Shape2D/Rectangle.h>  
#include <VoronoiMeshMaker/Shape/Shape2D/Star.h>  

using vmm::gtp::Point2D;
//using vmm::gtp::Vector2D;
using vmm::Shape2D;


enum SHAPE2D {RECTANGLE, STAR, ELLIPSE};

int main() {

    SHAPE2D testShape = ELLIPSE;
    vmm::GeometricDataHolder geometricDataHolder;
    vmm::PtrShape       ptrShape;
     
    switch (testShape) {
        
        case ELLIPSE :     geometricDataHolder.addValue("Length", 2.0);
                            geometricDataHolder.addValue("Width", 2.0);
                            geometricDataHolder.addValue("NPoints", 30);     
                            std::cout << "Dados para a geração do ellipses\n";
                            std::cout << geometricDataHolder << "\n";
                            ptrShape = vmm::ShapeFactory::createShape(vmm::Ellipse::getClassID(), geometricDataHolder);
                            break;

        case RECTANGLE :    geometricDataHolder.addValue("Length", 2.0);
                            geometricDataHolder.addValue("Width", 2.0);
                            geometricDataHolder.addValue("BottomLeftCorner", Point2D(3,1));     // Este dado é opcional
                            std::cout << "Dados para a geração do rectangle\n";
                            std::cout << geometricDataHolder << "\n";
                            ptrShape = vmm::ShapeFactory::createShape(vmm::Rectangle::getClassID(), geometricDataHolder);
                            break;

        case STAR :    geometricDataHolder.addValue("Radius", 2.0);
                            std::cout << "Dados para a geração da star\n";
                            std::cout << geometricDataHolder << "\n";
                            ptrShape = vmm::ShapeFactory::createShape(vmm::Star::getClassID(), geometricDataHolder);

                            break;
        
        default :           std::cout << "Opcao nao liberada" << "\n";
                            exit (EXIT_FAILURE);
    }
    
    
//
////    // Adicionando diferentes tipos de dados
//    geometricDataHolder.addValue("nPontos", 42);
//    geometricDataHolder.addValue("nDados", 412);
//    geometricDataHolder.addValue("dx", 0.1415);
//    geometricDataHolder.addValue("dy", 0.3);
//    geometricDataHolder.addValue("center", Point2D(1.0, 2.0));
//    geometricDataHolder.addValue("center1", Point2D(-1.0, 2.0));
//    geometricDataHolder.addValue("displacement", Vector2D(0.5, 1.5));
//    geometricDataHolder.addValue("displacement1", Vector2D(1.5, -1.5));
//
//    // Imprimindo todas as variáveis
//    std::cout << "Valores armazenados no GeometricDataHolder:" << std::endl;
//    std::cout << geometricDataHolder;
//
//    
//    
//    
//    
//    // Recuperando valores de diferentes tipos
//    if (auto intValue = geometricDataHolder.getValue<int>("intVar")) {
//        std::cout << "Valor de 'intVar': " << *intValue << std::endl;
//    }
//
//    if (auto RealValue = geometricDataHolder.getValue<Real>("RealVar")) {
//        std::cout << "Valor de 'RealVar': " << *RealValue << std::endl;
//    }
//
//    if (auto pointValue = geometricDataHolder.getValue<Point2D>("pointVar")) {
//        std::cout << "Valor de 'pointVar': (" << pointValue->x() << ", " << pointValue->y() << ")" << std::endl;
//    }
//
//    if (auto vectorValue = geometricDataHolder.getValue<Vector2D>("vectorVar")) {
//        std::cout << "Valor de 'vectorVar': (" << vectorValue->x() << ", " << vectorValue->y() << ")" << std::endl;
//    }

    return 0;
}
