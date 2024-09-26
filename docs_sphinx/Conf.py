# conf.py for VoronoiMeshMaker documentation

import os
import sys

# Adicionar o diretório raiz do projeto ao sys.path (se necessário)
sys.path.insert(0, os.path.abspath('../src'))

# -- Project information -----------------------------------------------------

project = 'VoronoiMeshMaker'
copyright = '2024, João Flávio Vasconcellos'
author = 'João Flávio Vasconcellos'

# A versão completa, incluindo tags alpha/beta/rc
release = '1.0'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
    'sphinx.ext.inheritance_diagram',
    'breathe'  # Breathe é necessário para integrar com o Doxygen
]

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinxdoc'
html_static_path = ['_static']

# Base URL for the sitemap (importante para gerar sitemap)
html_baseurl = 'https://voronoimeshmaker.github.io/voronoimeshmaker/'

# -- Breathe Configuration ---------------------------------------------------

# Define o caminho para o XML gerado pelo Doxygen
breathe_projects = {
    "VoronoiMeshMaker": "../build/doxygen/xml"  # Caminho para a saída XML gerada pelo Doxygen
}
breathe_default_project = "VoronoiMeshMaker"

# -- Options for Sphinx Sitemap ----------------------------------------------

# Configuração para o sitemap gerado pelo sphinx_sitemap
sitemap_url_scheme = "{link}"

# -- Autosectionlabel configuration ------------------------------------------

# Garante que rótulos de seções possam ser referenciados sem colisão
autosectionlabel_prefix_document = True

# -- Intersphinx configuration -----------------------------------------------

# Configura links para documentações externas
intersphinx_mapping = {
    'python': ('https://docs.python.org/3/', None),
    # Outros mapeamentos podem ser adicionados aqui
}

# -- Todo extension configuration --------------------------------------------

# Ativa ou desativa a exibição de itens "TODO" na documentação
todo_include_todos = True
