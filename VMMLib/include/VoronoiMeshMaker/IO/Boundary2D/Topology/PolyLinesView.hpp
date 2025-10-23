#pragma once
//==============================================================================
// Name        : PolyLinesView.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO/Boundary2D/Topology
// Description : Visão adaptadora sobre Boundary2DData para topologia de polilinhas.
// License     : GNU GPL v3
// Version     : 1.0.1 (Corrected)
//==============================================================================

#include <VoronoiMeshMaker/Core/namespace.h>
// ✅ CORREÇÃO: Apenas inclui o header, não redefine a classe.
#include <VoronoiMeshMaker/Boundary2D/Boundary2DData.hpp>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

class PolyLinesView {
public:
    explicit PolyLinesView(const vmm::b2d::Boundary2DData& data)
        : data_ref_(data)
    {
        num_lines_ = data_ref_.ring_count();
        // O formato VTK precisa do número total de inteiros na lista de conectividade.
        // Para cada um dos N anéis: (1 para o tamanho) + (pontos do anel + 1 para fechar).
        // A soma total é: num_anéis + vertex_count + num_anéis.
        total_connectivity_size_ = num_lines_ + data_ref_.vertex_count() + num_lines_;
    }

    [[nodiscard]] std::size_t num_lines() const noexcept { return num_lines_; }
    [[nodiscard]] std::size_t total_connectivity_size() const noexcept { return total_connectivity_size_; }
    [[nodiscard]] const vmm::b2d::Boundary2DData& data() const noexcept { return data_ref_; }

private:
    const vmm::b2d::Boundary2DData& data_ref_;
    std::size_t num_lines_{0};
    std::size_t total_connectivity_size_{0};
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE