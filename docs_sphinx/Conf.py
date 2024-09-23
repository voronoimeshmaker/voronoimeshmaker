# conf.py for VoronoiMeshMaker documentation

import os
import sys

# Add the project root directory to the Python path for autodoc (if using)
sys.path.insert(0, os.path.abspath('../src'))

# -- Project information -----------------------------------------------------

project = 'VoronoiMeshMaker'
copyright = '2024, João Flávio Vasconcellos'
author = 'João Flávio Vasconcellos'

# The full version, including alpha/beta/rc tags
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

html_theme = 'sphinxdoc'

#html_static_path = ['_static']

# -- Breathe Configuration ---------------------------------------------------

breathe_projects = {
    "VoronoiMeshMaker": "doxygen/xml"  # Path to the Doxygen XML output
}
breathe_default_project = "VoronoiMeshMaker"
