// ============================================================================
// File: Detail.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.5
// Description: Internal helpers for ErrorHandling (logging & rendering).
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

// -----------------------------------------------------------------------------
//  C++ Standard Library
// -----------------------------------------------------------------------------
#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>

// -----------------------------------------------------------------------------
//  VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/Core/namespace.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorManager.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorRecord.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/Language.h>
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

VORMAKER_NAMESPACE_OPEN
namespace error {
namespace detail {

/**
 * @brief Log an error with named placeholders.
 *
 * Aceita lista como:
 *   { {"name", "thr" + std::to_string(t)}, {"path", some_str} }
 *
 * Observações:
 * - O valor é armazenado como std::string (cópia), evitando lifetime issues.
 * - Parâmetro 'kv' é opcional (default = vazio), permitindo VMM_ERROR(err) sem KV.
 */
template <ErrorEnum E>
inline void log_error(
    E err_enum,
    std::initializer_list<
        std::pair<std::string_view, std::string>> kv = {})
{
    // Lê configuração em tempo de execução
    auto cfg = Config::get();
    const Severity sev = ErrorTraits<E>::default_severity(err_enum);
    if (cfg && sev < cfg->min_severity) {
        return; // abaixo do limiar, não loga
    }

    // Seleciona template de mensagem conforme idioma
    const Language lang = cfg ? cfg->language : Language::EnUS;
    const std::string_view tmpl =
        (lang == Language::PtBR)
            ? ErrorTraits<E>::ptBR(err_enum)
            : ErrorTraits<E>::enUS(err_enum);

    // Render simples: substitui {key} -> value
    std::string out(tmpl);
    for (const auto& [k, v] : kv) {
        std::string token;
        token.reserve(k.size() + 2);
        token.push_back('{');
        token.append(k.data(), k.size());
        token.push_back('}');

        std::string::size_type pos = 0;
        while ((pos = out.find(token, pos)) != std::string::npos) {
            out.replace(pos, token.size(), v);
            pos += v.size();
        }
    }

    // Emite o registro
    ErrorRecord rec;
    rec.code     = error_code(err_enum);
    rec.severity = sev;
    rec.message  = std::move(out);

    ErrorManager::log(std::move(rec));
}

}  // namespace detail
}  // namespace error
VORMAKER_NAMESPACE_CLOSE
