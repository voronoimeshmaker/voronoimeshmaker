# Contexto da conversa de hoje

Este documento resume a conversa que levou ao plano de upgrade do VoronoiMeshMaker.

## Objectivo principal

O VoronoiMeshMaker, VMM, deverá ser actualizado porque será usado pelo MohidNG. O VMM deve deixar de ser apenas um gerador de células Voronoi simples e passar a produzir uma malha finito-volume completa, auditável e exportável para o MohidNG.

O VMM deve suportar, progressivamente:

- dados GIS,
- domínios 2D com ilhas, buracos e regiões de fluxo nulo,
- malhas 3D, provavelmente primeiro por extrusão vertical,
- tessellation por múltiplos algoritmos,
- remeshing por múltiplos algoritmos,
- auditoria obrigatória de índices,
- conectividade compacta solver-neutral,
- exportação HDF5 para MohidNG,
- relatórios textuais para conferência humana.

## Estado inicial observado

O MohidNG já possui um contrato HDF5 para leitura de malhas Voronoi, com nodes, cells, faces e boundary_patches. O VMM ainda não produz directamente esse contrato.

O VMM já tem intenções arquitecturais boas, como C++20, Data-Oriented Design, CGAL isolado no backend e API pública sem tipos CGAL. Porém, ainda falta a camada principal de malha finito-volume explícita.

## Decisões tomadas

1. O VMM deve ser DOD.
2. A malha canónica deve ter nodes, faces, cells, owner, neighbour e patches.
3. O VMM deve tratar ilhas como holes do domínio fluido.
4. Ilhas e regiões de fluxo nulo não devem ter células activas.
5. A borda da ilha deve virar patch interno, por exemplo `island`, `wall` ou `no_flux`.
6. A edição da malha deve ser feita por edição de sites, não por edição directa de volumes finais.
7. Remeshing deve produzir um plano de edição de sites e reconstruir a malha.
8. O primeiro remeshing robusto deve usar reconstrução global da camada FV depois da edição de sites.
9. O VMM deve permitir múltiplos algoritmos por Strategy, Registry, Concepts e Traits.
10. O VMM não deve conhecer PETSc nem Trilinos no core.
11. O VMM deve produzir um finite-volume stencil graph solver-neutral.
12. MohidNG adapta o stencil graph para PETSc, Trilinos ou outro backend.
13. Toda operação que mexe em índices deve passar por auditoria obrigatória.
14. O relatório textual de auditoria deve ser column-aligned, com uma célula por bloco e uma face por linha.
15. Para 2D, vizinhos devem ser impressos em sentido horário.
16. Para 3D extrudido, faces laterais podem seguir sentido horário herdado da célula 2D, depois bottom e top.
17. Em 3D poliédrico geral não há sentido horário global, usar ordem por face_id ou ordem determinística.
18. O VMM deve calcular projecções ortogonais dos seeds sobre fronteiras físicas.
19. O ponto de projecção pode não pertencer à face de fronteira da célula, estas células devem ser marcadas como anómalas.
20. Para faces internas, o ponto médio entre os dois seeds pode não pertencer à face interna, estas faces devem ser marcadas como anómalas.
21. A documentação do VMM deve ter estrutura equivalente à do MohidNG, mas com conteúdo próprio: GIS, tessellation, remeshing, FV mesh, audit, HDF5 export.
22. Foram analisadas arquitecturas de OpenFOAM polyMesh, MeshKernel, Gmsh, CGAL, JIGSAW, Voro++, MMG, Omega_h, MOAB, DUNE, p4est e MFEM.

## Esclarecimento sobre o Codex

Codex não tem acesso automático a este chat. Este ZIP foi criado para transportar o contexto, as decisões e o plano de implementação.
