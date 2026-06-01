# Third-Party Dependencies & Attributions

## CGAL (Computational Geometry Algorithms Library)
- **License**: GPL-3.0-or-later / LGPL-3.0-or-later
- **URL**: https://www.cgal.org
- **Usage**: Constrained Delaunay triangulation (2D) and Regular/Delaunay triangulation (3D). Strictly isolated from public API.

## TBB (Intel oneAPI Threading Building Blocks)
- **License**: Apache-2.0
- **URL**: https://github.com/uxlfoundation/oneTBB
- **Usage**: Parallel seed generation and validation loops (optional).

## Catch2
- **License**: BSL-1.0
- **URL**: https://github.com/catchorg/Catch2
- **Usage**: Unit testing framework.

## spdlog (optional)
- **License**: MIT
- **Usage**: Structured logging & flow tracing (enabled via `VMM_ENABLE_FLOW_LOGGER`).

---
*Consumers using neutral file formats (MSH, UGRID, VTK) are not bound by the library license.*