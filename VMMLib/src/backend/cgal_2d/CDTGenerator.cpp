//==============================================================================
// VoronoiMeshMaker
//
// Copyright 2026 Joao Flavio Vieira de Vasconcellos
// Licence GPLv3 or later.
//
// This file is part of VoronoiMeshMaker.
//
// VoronoiMeshMaker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public Licence as published by
// the Free Software Foundation, either version 3 of the Licence, or
// any later version.
//
// VoronoiMeshMaker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public Licence for more details.
//
// You should have received a copy of the GNU General Public Licence
// along with VoronoiMeshMaker. If not, see <https://www.gnu.org/licenses/>.
//
// @file CDTGenerator.cpp
// @brief Reserved private CGAL 2D backend translation unit.
// @ingroup vmm_backend_cgal_2d
//==============================================================================

// The former constrained-Delaunay-only implementation was removed because it
// did not represent the clipped Voronoi pipeline required by the library. The
// real 2D backend is split across DelaunayGenerator, BoundaryPropagation2D,
// PolygonClipper2D, ClippedVoronoi2D, and CgalToMesh2D.
