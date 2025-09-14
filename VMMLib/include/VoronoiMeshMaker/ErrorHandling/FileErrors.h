// ============================================================================
// File: FileErrors.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.2
// Description: File I/O error domain and traits specialization.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  include C++
// -----------------------------------------------------------------------------
#include <array>
#include <cstdint>
#include <string_view>

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
// #include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>

/**
 * @file FileErrors.h
 * @brief File I/O error enum and ErrorTraits specialization.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/** @brief File I/O error codes (domain: File). */
enum class FileErr : std::uint16_t {
    FileNotFound   = 1,
    AccessDenied   = 2,
    ReadError      = 3,
    WriteError     = 4,
    InvalidPath    = 5,

    _Count  // sentinel for array sizing
};

namespace detail {

/** @brief Static metadata for FileErr messages. */
struct FileErrorInfo {
    std::string_view key;
    Severity         severity;
    std::string_view enUS;
    std::string_view ptBR;
};

// Index 0 is unused since enums start at 1.
constexpr std::array<FileErrorInfo,
    static_cast<size_t>(FileErr::_Count)>
file_err_data = {{
    {}, // index 0 (unused)

    // 1) FileNotFound
    {"FILE_NOT_FOUND",
     Severity::Error,
     "File not found: {path}.",
     "Arquivo não encontrado: {path}."},

    // 2) AccessDenied
    {"FILE_ACCESS_DENIED",
     Severity::Error,
     "Access denied for file: {path}.",
     "Acesso negado ao arquivo: {path}."},

    // 3) ReadError
    {"FILE_READ_ERROR",
     Severity::Error,
     "An error occurred while reading the file: {path}.",
     "Ocorreu um erro ao ler o arquivo: {path}."},

    // 4) WriteError
    {"FILE_WRITE_ERROR",
     Severity::Error,
     "An error occurred while writing to the file: {path}.",
     "Ocorreu um erro ao escrever no arquivo: {path}."},

    // 5) InvalidPath
    {"FILE_INVALID_PATH",
     Severity::Error,
     "The provided path is invalid: {path}.",
     "O caminho fornecido é inválido: {path}."}
}};

constexpr const FileErrorInfo&
get_file_info(FileErr e) noexcept {
    return file_err_data[static_cast<size_t>(e)];
}

} // namespace detail

/** @brief Traits specialization for the File error domain. */
template <>
struct ErrorTraits<FileErr> {
    static constexpr std::uint16_t domain_id() noexcept { return 0x0002; }
    static constexpr std::string_view domain_name() noexcept { return "File"; }

    static constexpr Severity default_severity(FileErr e) noexcept {
        return detail::get_file_info(e).severity;
    }
    static constexpr std::string_view key(FileErr e) noexcept {
        return detail::get_file_info(e).key;
    }
    static constexpr std::string_view enUS(FileErr e) noexcept {
        return detail::get_file_info(e).enUS;
    }
    static constexpr std::string_view ptBR(FileErr e) noexcept {
        return detail::get_file_info(e).ptBR;
    }
};

ERROR_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
