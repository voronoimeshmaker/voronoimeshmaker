# ------------------------------------------------------------
# Project options and policies
# ------------------------------------------------------------

# Set policies
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_POLICY_DEFAULT_CMP0167 NEW)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project options
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS    "Build tests"    ON)
option(BUILD_DOCS     "Build docs (Doxygen + Sphinx)" ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Project layout
set(VMMLib_DIR ${CMAKE_CURRENT_SOURCE_DIR}/VMMLib)
set(VMM_OUTPUT_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)