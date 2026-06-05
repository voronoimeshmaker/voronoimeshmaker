Política De Extensibilidade
===========================

Categorias funcionais que podem crescer com novos workflows devem usar traits,
tags, identificadores ou registries. Enums fechados são evitados quando uma nova
funcionalidade exigiria alterações em vários pontos do código.

A exceção atual é o seletor de dimensão 2D/3D, que é estrutural e usado em
especializações de template.
