Pesquisa Sobre Geradores De Malha
=================================

A pesquisa externa analisou Gmsh, CGAL, Triangle, JIGSAW, MMG, DistMesh,
snappyHexMesh, OceanMesh2D, MOAB, CGNS e meshio.

A principal consequência para o VMM é manter uma malha canónica própria,
isolar backends, usar tags e traits em vez de enums fechados, preparar uma
abstração de campo de tamanho e manter auditorias como gates explícitos.
