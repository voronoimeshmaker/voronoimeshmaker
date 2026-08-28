Relação Com O Mohid NG
======================

O VoronoiMeshMaker foi desenhado para preparar e conferir pacotes de malha
Voronoi de volumes finitos que podem ser consumidos por ferramentas de modelação
downstream como o Mohid NG. A biblioteca fica responsável pelas etapas
geométricas: descrição do domínio, geração de sites, tesselação Voronoi,
conectividade de volumes finitos, metadados de patches de fronteira, auditorias
e exportação de pacotes.

A relação é intencionalmente desacoplada. O VMM não incorpora lógica de solver
do Mohid NG, políticas de equações ou configuração de execução. Em vez disso,
ele produz uma representação de malha validada, com nós, células, faces,
patches, conectividade, diagnósticos de projeção, campos raster opcionais por
célula e metadados de pacote HDF5.

Essa separação mantém o VMM útil como gerador e biblioteca de diagnóstico
independente, ao mesmo tempo em que suporta arquivos de troca orientados ao
Mohid NG.
