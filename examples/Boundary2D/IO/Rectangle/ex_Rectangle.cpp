#include <iostream>
#include <string>
// #include <filesystem>

// YAML
#include <yaml-cpp/yaml.h>

// VMM
#include <VoronoiMeshMaker/Boundary2D/Boundary2DTypes.hpp>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>
#include <VoronoiMeshMaker/Boundary2D/Policies/PolygonizePolicy.hpp>
#include <VoronoiMeshMaker/Boundary2D/Shapes/Rectangle.hpp>

#include <VoronoiMeshMaker/IO/Options.hpp>
#include <VoronoiMeshMaker/IO/PathUtils.hpp>
#include <VoronoiMeshMaker/IO/Sinks.hpp>
#include <VoronoiMeshMaker/IO/Boundary2D/Writers/VTK_Legacy_PolyLines.hpp>

#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>
// #include <VoronoiMeshMaker/ErrorHandling/Macros.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

int main(int argc, char** argv) {
    using namespace vmm::b2d;
    using namespace vmm::io;

    try {
        // 1) Caminho do YAML (padrão: ./Dados/boundary.yaml dentro da pasta do exemplo)
        const std::filesystem::path yaml_path =
            (argc > 1) ? std::filesystem::path(argv[1])
                       : (std::filesystem::current_path() / "Dados" / "boundary.yaml");

        if (!std::filesystem::exists(yaml_path)) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","yaml_not_found"},
                       {"path", yaml_path.string()},
                       {"where","ex_Rectangle"}});
        }

        // 2) Carregar YAML
        YAML::Node root = YAML::LoadFile(yaml_path.string());

        // 3) Verificar chaves obrigatórias
        if (!root["output"]) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","missing_yaml_key"},
                       {"yaml_key","output"},
                       {"where","ex_Rectangle"}});
        }
        if (!root["rectangle"]) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","missing_yaml_key"},
                       {"yaml_key","rectangle"},
                       {"where","ex_Rectangle"}});
        }

        const YAML::Node& out  = root["output"];
        const YAML::Node& rect = root["rectangle"];

        // 4) Ler opções de saída
        const std::string folder   = out["folder"]   ? out["folder"].as<std::string>()   : std::string{};
        const std::string filename = out["filename"] ? out["filename"].as<std::string>() : "boundary";

        // 5) Ler dimensões (obrigatórias)
        if (!rect["width"]) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","missing_yaml_key"},
                       {"yaml_key","rectangle.width"},
                       {"where","ex_Rectangle"}});
        }
        if (!rect["height"]) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","missing_yaml_key"},
                       {"yaml_key","rectangle.height"},
                       {"where","ex_Rectangle"}});
        }

        const Real W = rect["width"].as<double>();
        const Real H = rect["height"].as<double>();
        if (!(W > Real{0}) || !(H > Real{0})) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"name","rectangle_non_positive_dims"},
                       {"w", std::to_string(static_cast<double>(W))},
                       {"h", std::to_string(static_cast<double>(H))}});
        }

        // 6) Opções VTK (ASCII Legacy para facilitar inspeção)
        VtkOptions opt{};
        opt.dialect   = Dialect::Legacy;
        opt.topology  = Topology::PolyLines;
        opt.encoding  = Encoding::ASCII;
        opt.precision = (root["format"] && root["format"]["precision"])
                        ? root["format"]["precision"].as<int>() : 12;
        opt.cell_data = true;

        // 7) Construir geometria e Boundary2DData
        Rectangle R{ .w = W, .h = H };
        PolygonizePolicy pol{};
        const auto outer = R.polygonize(pol);

        Boundary2DData data;
        data.append_ring(std::span<const Point2>(outer.data(), outer.size()),
                         LoopKind::Outer, RegionId{0});

        // 8) Resolver caminho final e escrever (cria pasta se necessário)
        const auto out_path = ::vmm::io::join_with_extension(folder, filename, opt);
        {
            FileSink sink(out_path.string());
            VTK_Legacy_PolyLinesWriter{}(data, opt, sink);
        }

        std::cout << "[ok] Arquivo escrito em: " << out_path << "\n";
        return 0;
    } catch (const ::vmm::error::VMMException& e) {
        std::cerr << "[VMMException] what: " << e.what() << "\n"
                  << "message: " << e.message() << "\n";
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "[std::exception] " << e.what() << "\n";
        return 3;
    }
}
