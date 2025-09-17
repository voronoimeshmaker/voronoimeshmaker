# ------------------------------------------------------------
# Main library target
# ------------------------------------------------------------

# Find sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     "${VMMLib_DIR}/src/*.cpp")

# Create library
add_library(VoronoiMeshMaker SHARED ${SOURCES})

# Link dependencies
target_link_libraries(VoronoiMeshMaker
    PUBLIC CGAL::CGAL CGAL::CGAL_Core
)

# Set optimizations
set_target_optimizations(VoronoiMeshMaker)

# Output directories
set_target_properties(VoronoiMeshMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${VMM_OUTPUT_BIN_DIR}
)

# Install rules
install(TARGETS VoronoiMeshMaker
    EXPORT VoronoiMeshMakerTargets
    LIBRARY DESTINATION ${VMM_OUTPUT_BIN_DIR}
    ARCHIVE DESTINATION ${VMM_OUTPUT_BIN_DIR}
    RUNTIME DESTINATION ${VMM_OUTPUT_BIN_DIR})

install(DIRECTORY ${VMMLib_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tpp")

install(DIRECTORY ${VMMLib_DIR}/src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.tpp")