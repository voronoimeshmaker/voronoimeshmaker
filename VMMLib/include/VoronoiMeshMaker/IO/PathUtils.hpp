#pragma once
//==============================================================================
// Name        : PathUtils.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO
// Description : Utilitários de filesystem (pasta/arquivo/extensão padrão).
// License     : GNU GPL v3
// Version     : 0.2.0
//==============================================================================

/**
 * @file PathUtils.hpp
 * @brief Helpers para resolver pasta, criar diretórios e aplicar extensão padrão.
 *
 * Regras:
 * - Pasta vazia ⇒ usar diretório corrente (cwd).
 * - Se a pasta não existir ⇒ criar recursivamente (lança erro se falhar).
 * - Se o arquivo não tiver extensão ⇒ aplicar extensão padrão do dialeto:
 *     - Dialect::Legacy → .vtk
 *     - Dialect::XML    → .vtp
 *
 * Observação importante:
 * - As funções internas ficam em `vmm::io::detail`. Para uso externo (tests,
 *   exemplos, fachada), prefira as *wrappers* públicas em `vmm::io::`.
 *   Isso evita colisões com outros `detail` de módulos diferentes.
 */

#include <string_view>
#include <filesystem>
#include <system_error>
#include <utility>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/IO/Options.hpp>
#include <VoronoiMeshMaker/ErrorHandling/Macros.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

/// @brief Extensão padrão para cada dialeto.
/// @return ".vtk" para Legacy, ".vtp" para XML.
inline std::filesystem::path default_extension_for(Dialect d) noexcept {
    switch (d) {
        case Dialect::Legacy: return std::filesystem::path(".vtk");
        case Dialect::XML:    return std::filesystem::path(".vtp");
    }
    // Fallback defensivo (enum exaustivo, mas evita warnings).
    return std::filesystem::path(".vtk");
}

/// @brief Se não houver extensão, aplica a extensão padrão do dialeto.
inline std::filesystem::path ensure_extension(std::filesystem::path p, Dialect d) {
    if (!p.has_extension()) p.replace_extension(default_extension_for(d));
    return p;
}

/// @brief Resolve pasta vazia para `cwd`; caso contrário, retorna a pasta informada.
inline std::filesystem::path resolve_folder(std::string_view folder) {
    if (folder.empty()) return std::filesystem::current_path();
    return std::filesystem::path(folder);
}

/// @brief Garante que a pasta exista (criação recursiva quando necessário).
/// @throws VMMException(CoreErr::InvalidArgument) se falhar para criar.
inline void ensure_folder_exists(const std::filesystem::path& dir) {
    std::error_code ec;
    if (dir.empty()) return;
    if (std::filesystem::exists(dir, ec)) return;
    if (!std::filesystem::create_directories(dir, ec)) {
        VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                  {{"file","cannot_create_folder"},
                   {"path", dir.string()}});
    }
}

/**
 * @brief Constrói o caminho final (folder + filename + extensão) a partir do dialeto.
 * @throws VMMException(CoreErr::InvalidArgument) se `filename` for vazio.
 */
inline std::filesystem::path join_with_extension(std::string_view folder,
                                                 std::string_view filename,
                                                 Dialect d)
{
    if (filename.empty()) {
        VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                  {{"arg","filename_empty"}});
    }
    const auto dir = resolve_folder(folder);
    ensure_folder_exists(dir);
    std::filesystem::path p = dir / std::filesystem::path(filename);
    return ensure_extension(std::move(p), d);
}

/**
 * @brief Sobrecarga conveniente usando `VtkOptions` (usa `options.dialect`).
 */
inline std::filesystem::path join_with_extension(std::string_view folder,
                                                 std::string_view filename,
                                                 const VtkOptions& options)
{
    return join_with_extension(folder, filename, options.dialect);
}

DETAIL_NAMESPACE_CLOSE

//============================== Public wrappers ===============================

/**
 * @brief Wrapper público de `detail::join_with_extension(folder, filename, d)`.
 *        Preferir esta versão em código cliente e testes.
 */
inline std::filesystem::path join_with_extension(std::string_view folder,
                                                 std::string_view filename,
                                                 Dialect d)
{
    return detail::join_with_extension(folder, filename, d);
}

/**
 * @brief Wrapper público de `detail::join_with_extension(folder, filename, options)`.
 *        Preferir esta versão em código cliente e testes.
 */
inline std::filesystem::path join_with_extension(std::string_view folder,
                                                 std::string_view filename,
                                                 const VtkOptions& options)
{
    return detail::join_with_extension(folder, filename, options);
}

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
