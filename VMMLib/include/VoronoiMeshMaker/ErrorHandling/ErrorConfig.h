// ============================================================================
// File: ErrorConfig.h
// Author: VoronoiMeshMaker Team
// Version: 0.1.0
// Description: Runtime configuration for error handling.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#pragma once

#include <atomic>
#include <memory>

#include <VoronoiMeshMaker/ErrorHandling/Language.h>
#include <VoronoiMeshMaker/ErrorHandling/Severity.h>

/**
 * @file ErrorConfig.h
 * @brief Runtime configuration (language, policy, thresholds).
 * @ingroup errorhandling
 */

VORMAKER_NAMESPACE_OPEN
namespace error {

/** @brief Policy for error handling at call sites. */
enum class Policy {
    Throw,   ///< Throw exceptions on error
    Status   ///< Return Status/StatusOr on error
};

/** @brief Immutable configuration blob (shared). */
struct ErrorConfig {
    Language language{Language::EnUS};
    Policy policy{Policy::Throw};
    Severity min_severity{Severity::Warning};
    bool deterministic{false};
    bool expose_breadcrumbs{false};
    std::size_t thread_buffer_cap{256};
};

/**
 * @brief Global config handle (atomic shared_ptr swap).
 * Minimal and header-only for now.
 */
class Config {
public:
    static std::shared_ptr<const ErrorConfig> get() noexcept {
        return instance().ptr_.load(std::memory_order_acquire);
    }

    static void set(ErrorConfig cfg) {
        auto sp = std::make_shared<const ErrorConfig>(std::move(cfg));
        instance().ptr_.store(sp, std::memory_order_release);
    }

private:
    Config() {
        ptr_.store(std::make_shared<const ErrorConfig>(),
                   std::memory_order_release);
    }

    static Config& instance() {
        static Config inst;
        return inst;
    }

    std::atomic<std::shared_ptr<const ErrorConfig>> ptr_;
};

}  // namespace error
VORMAKER_NAMESPACE_CLOSE
