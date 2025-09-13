#pragma once
//==============================================================================
// Name        : Sinks.hpp
// Project     : VoronoiMeshMaker (VMM)
// Module      : IO
// Description : Sinks utilitários (ostream/arquivo) para serialização.
// License     : GNU GPL v3
// Version     : 0.1.0
//==============================================================================

/**
 * @file Sinks.hpp
 * @brief Sinks básicos: `OStreamSink` (wrap de ostream) e `FileSink`.
 *
 * Contrato esperado pelos writers:
 *  - void write(std::string_view)
 *  - void write_binary(std::span<const std::byte>)
 *  - void flush()
 *
 * Observações:
 * - `FileSink` abre o arquivo com `std::ios::binary` por padrão para evitar
 *   transformações de newline em plataformas como Windows. ASCII continua
 *   funcionando normalmente em modo binário.
 */

#include <cstddef>
#include <span>
#include <ostream>
#include <fstream>
#include <string>
#include <utility>

#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/ErrorHandling/Macros.h>
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

/// @brief Sink que escreve em um `std::ostream` existente (não toma posse).
struct OStreamSink {
    std::ostream& os;

    /// Escreve texto como está.
    void write(std::string_view s) {
        os.write(s.data(), static_cast<std::streamsize>(s.size()));
    }

    /// Escreve bytes arbitrários (binário).
    void write_binary(std::span<const std::byte> b) {
        os.write(reinterpret_cast<const char*>(b.data()),
                 static_cast<std::streamsize>(b.size()));
    }

    /// Garante que o buffer do stream seja drenado.
    void flush() { os.flush(); }
};

/**
 * @brief Sink que gerencia um arquivo próprio (abre/fecha).
 *
 * Abre em modo `std::ios::binary | std::ios::out | std::ios::trunc` por padrão,
 * para comportar tanto escrita ASCII quanto binária sem surpresas.
 */
class FileSink {
public:
    explicit FileSink(std::string path,
                      std::ios::openmode mode =
                          (std::ios::out | std::ios::trunc | std::ios::binary))
        : path_(std::move(path)), ofs_(path_, mode)
    {
        if (!ofs_) {
            VMM_THROW(::vmm::error::CoreErr::InvalidArgument,
                      {{"file","cannot_open"}});
        }
    }

    FileSink(const FileSink&) = delete;
    FileSink& operator=(const FileSink&) = delete;

    FileSink(FileSink&&) noexcept = default;
    FileSink& operator=(FileSink&&) noexcept = default;

    /// Escreve texto como está.
    void write(std::string_view s) {
        ofs_.write(s.data(), static_cast<std::streamsize>(s.size()));
    }

    /// Escreve bytes arbitrários (binário).
    void write_binary(std::span<const std::byte> b) {
        ofs_.write(reinterpret_cast<const char*>(b.data()),
                   static_cast<std::streamsize>(b.size()));
    }

    /// Garante que o buffer do arquivo seja drenado.
    void flush() { ofs_.flush(); }

    /// Caminho associado (para diagnóstico/log).
    const std::string& path() const noexcept { return path_; }

private:
    std::string  path_;
    std::ofstream ofs_;
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
