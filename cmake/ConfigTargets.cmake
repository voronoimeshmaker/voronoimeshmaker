# cmake/ConfigTargets.cmake

include(CMakePackageConfigHelpers)

file(GLOB_RECURSE VMM_SOURCES CONFIGURE_DEPENDS
    "${VMMLib_DIR}/src/*.cpp"
)

add_library(VoronoiMeshMaker ${VMM_SOURCES})
add_library(VoronoiMeshMaker::VoronoiMeshMaker ALIAS VoronoiMeshMaker)

set_target_properties(VoronoiMeshMaker PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_include_directories(VoronoiMeshMaker
    PUBLIC
        $<BUILD_INTERFACE:${VMMLib_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(VoronoiMeshMaker
    PRIVATE ${HDF5_INCLUDE_DIRS}
)

target_compile_definitions(VoronoiMeshMaker
    PRIVATE
        VMM_HAS_GDAL=$<BOOL:${GDAL_FOUND}>
        VMM_HAS_PROJ=$<BOOL:${PROJ_FOUND}>
)

if(GDAL_FOUND)
    if(TARGET GDAL::GDAL)
        target_link_libraries(VoronoiMeshMaker PRIVATE GDAL::GDAL)
    else()
        target_include_directories(VoronoiMeshMaker PRIVATE ${GDAL_INCLUDE_DIRS})
        target_link_libraries(VoronoiMeshMaker PRIVATE ${GDAL_LIBRARIES})
    endif()
endif()

if(PROJ_FOUND)
    if(TARGET PROJ::proj)
        target_link_libraries(VoronoiMeshMaker PRIVATE PROJ::proj)
    elseif(TARGET PROJ::PROJ)
        target_link_libraries(VoronoiMeshMaker PRIVATE PROJ::PROJ)
    else()
        target_include_directories(VoronoiMeshMaker PRIVATE ${PROJ_INCLUDE_DIRS})
        target_link_libraries(VoronoiMeshMaker PRIVATE ${PROJ_LIBRARIES})
    endif()
endif()

target_compile_features(VoronoiMeshMaker
    PUBLIC cxx_std_${VMM_CXX_STANDARD}
)

target_link_libraries(VoronoiMeshMaker
    PRIVATE
        CGAL::CGAL
        CGAL::CGAL_Core
        TBB::tbb
        ${HDF5_LIBRARIES}
        vmm_project_options
        vmm_project_warnings
)

install(TARGETS VoronoiMeshMaker
    EXPORT VoronoiMeshMakerTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY "${VMMLib_DIR}/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.tpp"
)

install(EXPORT VoronoiMeshMakerTargets
    FILE VoronoiMeshMakerTargets.cmake
    NAMESPACE VoronoiMeshMaker::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VoronoiMeshMaker
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VoronoiMeshMakerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VoronoiMeshMakerConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VoronoiMeshMaker
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/VoronoiMeshMakerConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/VoronoiMeshMakerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VoronoiMeshMakerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VoronoiMeshMaker
)