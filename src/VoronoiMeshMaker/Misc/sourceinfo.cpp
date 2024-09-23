//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/sourceinfo.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Function to capture current file, line, and function information.
 * 
 * This function captures the current file location, line number, and function name
 * using the predefined macros __FILE__, __LINE__, and __func__.
 * 
 * @return A SourceInfo object with the captured information.
 */
constexpr SourceInfo CaptureSourceInfo(const char* file, int line, const char* function) {
    return SourceInfo(file, line, function);
}

VORMAKER_NAMESPACE_CLOSE
