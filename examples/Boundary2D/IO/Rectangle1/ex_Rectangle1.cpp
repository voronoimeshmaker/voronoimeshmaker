// -----------------------------------------------------------------------------
//  Includes da Biblioteca Padrão C++
// -----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <exception>

// -----------------------------------------------------------------------------
//  Includes da Biblioteca VoronoiMeshMaker (VMMLib)
// -----------------------------------------------------------------------------
// Para a classe Rectangle e seus conceitos relacionados.
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>
// Para os tipos básicos como Point2 e PolygonizePolicy.
#include <VoronoiMeshMaker/Boundary2D/Boundary2DTypes.hpp>
// Para a classe de exceção VMMException.
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>

// --- Usando namespaces para simplificar o código ---
using namespace vmm::b2d;
using vmm::error::VMMException;

// --- Função auxiliar para imprimir os vértices de um polígono ---
void print_vertices(const std::string& title, const std::vector<Point2>& vertices) {
    std::cout << "--- " << title << " ---\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "  Vértice " << i << ": ("
                  << static_cast<double>(vertices[i].x) << ", "
                  << static_cast<double>(vertices[i].y) << ")\n";
    }
    std::cout << std::endl;
}

int main() {
    try {
        // =====================================================================
        // EXEMPLO 1: Retângulo básico (ancorado na origem)
        // =====================================================================
        // Constrói um retângulo de 3x2 com o canto inferior esquerdo em (0,0).
        Rectangle rect_basic(3.0, 2.0);

        // A política de poligonização é necessária pela interface,
        // mas não armazena estado para formas simples como o retângulo.
        PolygonizePolicy policy{};
        auto vertices_basic = rect_basic.polygonize(policy);
        print_vertices("Retângulo Básico (3x2, ancorado em 0,0)", vertices_basic);


        // =====================================================================
        // EXEMPLO 2: Retângulo centrado na origem
        // =====================================================================
        // Constrói um retângulo de 4x4 centrado em (0,0) usando a tag 'Centered'.
        Rectangle rect_centered(4.0, 4.0, Rectangle::Centered);
        auto vertices_centered = rect_centered.polygonize(policy);
        print_vertices("Retângulo Centrado (4x4)", vertices_centered);


        // =====================================================================
        // EXEMPLO 3: Retângulo com âncora personalizada
        // =====================================================================
        // Constrói um retângulo de 2x1 com o canto inferior esquerdo em (10, 5).
        Point2 anchor_point{10.0, 5.0};
        Rectangle rect_anchored(anchor_point, 2.0, 1.0);
        auto vertices_anchored = rect_anchored.polygonize(policy);
        print_vertices("Retângulo Ancorado (2x1, em 10,5)", vertices_anchored);


        // =====================================================================
        // EXEMPLO 4: Tratamento de Erros
        // =====================================================================
        // O código a seguir, se descomentado, lançaria uma exceção porque
        // as dimensões não podem ser zero ou negativas.
        /*
        std::cout << "--- Tentando criar um retângulo inválido ---" << std::endl;
        Rectangle rect_invalid(5.0, 0.0); // Lançaria VMMException!
        */

    } catch (const VMMException& e) {
        std::cerr << "\nERRO CAPTURADO: Uma exceção da biblioteca VMM ocorreu!\n";
        // O método what() fornece uma mensagem de erro estável em inglês.
        std::cerr << "  Mensagem (what()): " << e.what() << "\n";
        // O método message() fornece a mensagem traduzida, se houver.
        std::cerr << "  Mensagem (localizada): " << e.message() << "\n";
        // O método code() retorna o código de erro completo de 32 bits.
        std::cerr << "  Código do Erro: " << e.code() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "ERRO INESPERADO: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}