#include <iostream>
#include <vector>
#include <string_view>
#include <filesystem>
#include <string>

// --- Includes da Biblioteca VoronoiMeshMaker ---

// Inclui a nossa nova API de exportação de alto nível.
#include <VoronoiMeshMaker/IO/Export.hpp>

// Inclui os componentes necessários para criar a geometria.
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>
#include <VoronoiMeshMaker/Boundary2D/Policies/PolygonizePolicy.hpp>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>

// A função main não precisa mais de argc/argv para esta tarefa.
int main() {
    // --- Usando namespaces para simplificar o código ---
    using namespace vmm::b2d;
    using namespace vmm::io;

    try {
        // --- 1. Criar a geometria base ---
        // Cria um retângulo de 10x5 centrado na origem.
        Rectangle rect(10.0, 5.0, Rectangle::Centered);
        PolygonizePolicy policy{};
        auto vertices = rect.polygonize(policy);

        // --- 2. Colocar a geometria no contentor de dados da biblioteca ---
        Boundary2DData boundary_data;
        boundary_data.append_ring_checked(vertices, LoopKind::Outer, RegionId{1});

        // --- 3. Definir as opções de exportação ---
        VtkOptions options;
        options.dialect = Dialect::Legacy;         // Formato .vtk.
        options.topology = Topology::PolyLines;    // Escrever o contorno (as linhas).
        options.encoding = Encoding::ASCII;        // Ficheiro de texto legível.
        options.write_z = true;                    // Adicionar z=0 para compatibilidade 3D.
        options.precision = 6;                     // 6 casas decimais para os números.

        // --- 4. Lógica para definir o caminho de saída ---
        // A macro VMM_EXAMPLE_SOURCE_DIR é definida pelo CMake e contém o caminho
        // para a pasta onde este ficheiro .cpp está localizado.
        std::filesystem::path source_dir(VMM_EXAMPLE_SOURCE_DIR);
        // Cria o caminho para a nossa pasta de saída, relativa à pasta do código-fonte.
        std::filesystem::path output_folder = source_dir / "Resultados";
        const std::string output_filename = "Retangulo";

        // --- 5. Chamar a função de exportação ---
        std::cout << "Exportando o arquivo vtk..." << std::endl;
        export_to_vtk(boundary_data, output_folder.string(), output_filename, options);

        std::cout << "Arquivo '" << std::filesystem::weakly_canonical(output_folder).string()
                  << "/" << output_filename << ".vtk' gerado com sucesso!" << std::endl;
        std::cout << "Abra este arquivo no ParaView para visualizar o contorno do retângulo." << std::endl;

    } catch (const vmm::error::VMMException& e) {
        std::cerr << "ERRO: Ocorreu uma exceção da biblioteca VMM:\n"
                  << "  Mensagem: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}