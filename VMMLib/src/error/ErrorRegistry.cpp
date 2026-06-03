//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file ErrorRegistry.cpp
// @brief Built-in VoronoiMeshMaker error catalogue implementation.
// @ingroup vmm_error
//==============================================================================

#include <vmm/error/ErrorRegistry.hpp>

#include <array>

namespace vmm::error {
namespace {

constexpr std::array<ErrorDescriptor, 12U> descriptors{
    ErrorDescriptor{builtin_error_id::unknown, severity::error, "unknown", "Unknown error.", "Erro desconhecido."},
    ErrorDescriptor{builtin_error_id::invalid_argument, severity::error, "invalid_argument", "Invalid argument.", "Argumento invalido."},
    ErrorDescriptor{builtin_error_id::invalid_configuration, severity::error, "invalid_configuration", "Invalid configuration.", "Configuracao invalida."},
    ErrorDescriptor{builtin_error_id::file_not_found, severity::error, "file_not_found", "File not found.", "Ficheiro nao encontrado."},
    ErrorDescriptor{builtin_error_id::parse_error, severity::error, "parse_error", "Could not parse input.", "Nao foi possivel interpretar a entrada."},
    ErrorDescriptor{builtin_error_id::geometry_error, severity::error, "geometry_error", "Invalid geometry operation.", "Operacao geometrica invalida."},
    ErrorDescriptor{builtin_error_id::invalid_polygon, severity::error, "invalid_polygon", "Invalid polygon geometry.", "Geometria de poligono invalida."},
    ErrorDescriptor{builtin_error_id::degenerate_polygon, severity::error, "degenerate_polygon", "Degenerate polygon geometry.", "Geometria de poligono degenerada."},
    ErrorDescriptor{builtin_error_id::self_intersecting_polygon, severity::error, "self_intersecting_polygon", "Self-intersecting polygon geometry.", "Geometria de poligono com auto-interseccao."},
    ErrorDescriptor{builtin_error_id::topology_error, severity::error, "topology_error", "Invalid mesh topology.", "Topologia de malha invalida."},
    ErrorDescriptor{builtin_error_id::backend_error, severity::error, "backend_error", "Backend operation failed.", "Operacao do backend falhou."},
    ErrorDescriptor{builtin_error_id::unsupported_operation, severity::error, "unsupported_operation", "Unsupported operation.", "Operacao nao suportada."}
};

} // namespace

std::span<const ErrorDescriptor> builtin_error_descriptors() noexcept
{
    return std::span<const ErrorDescriptor>{descriptors.data(), descriptors.size()};
}

const ErrorDescriptor* find_builtin_error(ErrorId id) noexcept
{
    for(const auto& descriptor : descriptors) {
        if(descriptor.id == id) {
            return &descriptor;
        }
    }

    return nullptr;
}

} // namespace vmm::error
