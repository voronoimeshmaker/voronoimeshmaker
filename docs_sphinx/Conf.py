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
    'breathe'
]

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# Base URL for the sitemap

html_baseurl = 'https://voronoimeshmaker.github.io/voronoimeshmaker/'


html_theme = 'sphinxdoc'

# -- Breathe Configuration ---------------------------------------------------

breathe_projects = {
    "VoronoiMeshMaker": "./doxygen/xml"  # Caminho para a saída XML do Doxygen
}
breathe_default_project = "VoronoiMeshMaker"
