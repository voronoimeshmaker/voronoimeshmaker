// ============================================================================
// File: MessageCatalog.hpp
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Minimal message rendering (uses ErrorTraits text).
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once


// -----------------------------------------------------------------------------
//  includec++
// -----------------------------------------------------------------------------
#include <initializer_list>
using Initializer = std::initializer_list<std::pair<std::string_view, std::string_view>>;
// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>
#include <VoronoiMeshMaker/ErrorHandling/Language.h>

/**
 * @file MessageCatalog.hpp
 * @brief Renders localized messages using ErrorTraits text.
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @brief Very small named placeholder replacer: replaces "{name}" tokens.
 * This is not a full formatter; it is enough for short messages.
 */
inline std::string replace_named(
    std::string_view tmpl,
    std::initializer_list<std::pair<std::string_view, std::string_view>> kv) {
    std::string out;
    out.reserve(tmpl.size() + 32);
    out.assign(tmpl.begin(), tmpl.end());
    for (auto&& [k, v] : kv) {
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
    return out;
}

/** @brief Render message for enum value E in the given language. */
template <ErrorEnum E>
inline std::string render   (E  e
                            ,   Language        _lang
                            ,   Initializer     _kv
                            ) {
    const std::string_view t =
        (_lang == Language::PtBR) ? ErrorTraits<E>::ptBR(e)
                                 : ErrorTraits<E>::enUS(e);
    return replace_named(t, _kv);
}

ERROR_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE
