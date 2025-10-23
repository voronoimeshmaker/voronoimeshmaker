#pragma once
//==============================================================================
// Name        : TriangulatePolicy.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO/Boundary2D/Topology
// Description : Política/Visão que triangula um Boundary2DData.
// License     : GNU GPL v3
// Version     : 1.0.0
//==============================================================================

/**
 * @file TriangulatePolicy.hpp
 * @brief Fornece uma visão triangulada sobre um Boundary2DData para exportação como polígonos.
 */

#include <vector>
#include <array>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>

// NOTA: Para uma implementação real, os includes da biblioteca de triangulação (ex: CGAL)
// seriam necessários aqui.
// #include <CGAL/Constrained_Delaunay_triangulation_2.h>
// #include <CGAL/Triangulation_face_base_with_info_2.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

/**
 * @class TriangulatedView
 * @brief Uma visão que representa o resultado da triangulação de um Boundary2DData.
 *
 * Esta classe não possui os vértices originais (apenas uma referência), mas armazena
 * o resultado da triangulação: uma lista de triângulos (definidos por índices)
 * e a região original associada a cada triângulo.
 *
 * A "Política" no nome do ficheiro refere-se à ideia de que diferentes algoritmos
 * de triangulação poderiam ser usados para gerar esta visão no futuro.
 */
class TriangulatedView {
public:
    // Define um triângulo como um array de 3 índices de vértices.
    using TriangleIndices = std::array<vmm::b2d::Index, 3>;

    /**
     * @brief Constrói a visão triangulada a partir de um Boundary2DData.
     * @param data A fonte de dados a ser triangulada.
     */
    explicit TriangulatedView(const vmm::b2d::Boundary2DData& data)
        : data_ref_(data)
    {
        // ====================================================================
        // A LÓGICA DE TRIANGULAÇÃO REAL IRIA AQUI.
        // ====================================================================
        // Esta é uma operação complexa. Uma implementação robusta usaria uma
        // biblioteca como a CGAL para:
        // 1. Inserir todos os anéis (exteriores e buracos) como restrições
        //    numa Triangulação de Delaunay Restrita (CDT).
        // 2. Iterar sobre as faces da triangulação.
        // 3. Para cada face que está "dentro" do domínio (não dentro de um buraco),
        //    adicionar os seus três índices de vértices ao nosso vetor `triangles_`
        //    e o `RegionId` correspondente ao `triangle_regions_`.
        //
        // Como placeholder, vamos deixar os vetores vazios. O Writer que
        // criarmos a seguir irá consumir esta interface.
        // ====================================================================

        // Após a triangulação, calculamos o tamanho para o cabeçalho VTK.
        // Para N triângulos, o formato é: N (N * 4)
        // Onde cada linha é: 3 i0 i1 i2
        num_triangles_ = triangles_.size();
        total_connectivity_size_ = num_triangles_ * 4;
    }

    /// Retorna o número de triângulos na malha.
    [[nodiscard]] std::size_t num_triangles() const noexcept {
        return num_triangles_;
    }

    /// Retorna o tamanho total da lista de conectividade para o cabeçalho VTK POLYGONS.
    [[nodiscard]] std::size_t total_connectivity_size() const noexcept {
        return total_connectivity_size_;
    }

    /// Retorna uma visão sobre a lista de triângulos (array de índices).
    [[nodiscard]] std::span<const TriangleIndices> triangles() const noexcept {
        return triangles_;
    }

    /// Retorna uma visão sobre a lista de RegionIds, uma para cada triângulo.
    [[nodiscard]] std::span<const vmm::b2d::RegionId> triangle_regions() const noexcept {
        return triangle_regions_;
    }

    /// Fornece acesso ao objeto de dados original (para obter as coordenadas dos pontos).
    [[nodiscard]] const vmm::b2d::Boundary2DData& data() const noexcept {
        return data_ref_;
    }

private:
    const vmm::b2d::Boundary2DData& data_ref_;
    std::vector<TriangleIndices> triangles_;
    std::vector<vmm::b2d::RegionId> triangle_regions_;
    std::size_t num_triangles_{0};
    std::size_t total_connectivity_size_{0};
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE