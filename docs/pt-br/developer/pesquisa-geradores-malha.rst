Pesquisa Sobre Geradores De Malha
=================================

A pesquisa externa analisou Gmsh, CGAL, Triangle, JIGSAW, MMG, DistMesh,
snappyHexMesh, OceanMesh2D, MOAB, CGNS e meshio.

A conclusão para o VMM é arquitetural: manter uma malha canônica própria,
isolar backends, usar tags e traits em vez de enums fechados, introduzir uma
abstração de campo de tamanho antes de remeshing avançado e manter auditorias
como gates explícitos da pipeline.
