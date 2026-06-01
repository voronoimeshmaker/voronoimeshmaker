# VoronoiMeshMaker

Gerador de malhas não estruturadas Voronoi 2D/3D de alta performance para modelagem hidrodinâmica e científica. Desenvolvido em C++20, com Data-Oriented Design (DOD), isolamento rigoroso de dependências e instrumentação toggleável.

## 🌟 Principais Características
- **Tesselação & Remeshing**: Geração inicial via CGAL (CDT 2D / Delaunay 3D) + adaptação pós-geração guiada por métricas
- **Arquitetura Moderna**: C++20 Concepts, Ranges, SoA layout, zero herança virtual, SOLID reinterpretado para compile-time composition
- **Isolamento de Backend**: CGAL estritamente privado. API pública expõe apenas PODs, views e contratos
- **IDs Sequenciais & CSR**: Mapeamento determinístico `GeneratorID → Conectividade` para integração direta com solvers (Mohid-NG, PETSc, Trilinos)
- **Instrumentação Zero-Overhead**: Logging e timing desabilitáveis em tempo de compilação (`VMM_HAS_LOGGER`, `VMM_HAS_PROFILING`)
- **Erros Bilíngues**: Mensagens em `pt_BR` (default) e `en_US`, com códigos estáticos e contexto estruturado
- **Exportação Multi-Formato**: VTK/VTU, MSH v4, UGRID/NetCDF (CF-Conventions)
- **Documentação Acadêmica**: Sphinx + Doxygen + Breathe, bilíngue, com catálogo de erros e tutoriais reproduzíveis

## 🛠️ Build Rápido (WSL/Ubuntu)
```bash
sudo apt install libcgal-dev libtbb-dev cmake g++ make
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DVMM_BUILD_TESTS=ON
cmake --build . -j$(nproc)
ctest --output-on-failure