// ============================================================================
// File: ErrorConfig.cpp
// Author: VoronoiMeshMaker Team
// Version: 0.1.2 (Fix circular dependency)
// Description: Runtime configuration for error handling.
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ============================================================================

#include <VoronoiMeshMaker/ErrorHandling/ErrorConfig.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorManager.h> // Include aqui é seguro

VORMAKER_NAMESPACE_OPEN
namespace error {

// Define o construtor que foi apenas declarado no header.
// Ele precisa do include de ErrorManager.h para conhecer a classe ThreadLocalBufferLogger.
ErrorConfig::ErrorConfig()
    : logger(std::make_shared<ThreadLocalBufferLogger>()) {}

}  // namespace error
VORMAKER_NAMESPACE_CLOSE