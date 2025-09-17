# VoronoiMeshMaker — Sphinx config (PETSc-like layout via PyData theme)

import os
from pathlib import Path

# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
project = "VoronoiMeshMaker"
html_title = "VoronoiMeshMaker Documentation"
copyright = "2024, VoronoiMeshMaker Team"
author = "VoronoiMeshMaker Team"

# -----------------------------------------------------------------------------
# Extensions
# -----------------------------------------------------------------------------
extensions = [
    "breathe",          # Bridge Doxygen XML -> Sphinx
    "myst_parser",      # Markdown support
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "sphinx.ext.intersphinx",
    "sphinx.ext.viewcode",
    "sphinx.ext.todo",
    "sphinx.ext.mathjax",
    "sphinx_design",    # For better design elements
]

# Markdown tweaks
myst_enable_extensions = ["deflist", "smartquotes", "dollarmath", "fieldlist"]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# -----------------------------------------------------------------------------
# PETSc-like HTML layout: PyData Sphinx Theme
# -----------------------------------------------------------------------------
html_theme = "pydata_sphinx_theme"

_here = Path(__file__).parent
_static_dir = _here / "_static"

# Only set html_static_path if the folder exists
html_static_path = ["_static"] if _static_dir.is_dir() else []

# Theme options for PETSc-like layout
html_theme_options = {
    "navbar_start": ["navbar-logo"],
    "navbar_center": ["navbar-nav"],
    "navbar_end": ["theme-switcher", "navbar-icon-links"],
    "navigation_depth": 4,
    "show_toc_level": 2,
    "show_nav_level": 1,
    "header_links_before_dropdown": 6,
    
    "icon_links": [
        {
            "name": "GitHub",
            "url": "https://github.com/SEU_USUARIO/VoronoiMeshMaker",
            "icon": "fa-brands fa-github",
        },
    ],
    
    "external_links": [
        {
            "name": "CGAL Documentation", 
            "url": "https://doc.cgal.org/latest/Manual/index.html"
        },
    ],
}

# Code style
pygments_style = "default"
pygments_dark_style = "native"

# -----------------------------------------------------------------------------
# Breathe Configuration
# -----------------------------------------------------------------------------
breathe_projects = {
    "VoronoiMeshMaker": "../doxygen/xml"
}
breathe_default_project = "VoronoiMeshMaker"
breathe_domain_by_extension = {
    "h": "c",
    "hpp": "cpp",
    "cpp": "cpp",
}
breathe_show_define_initializer = True
breathe_show_enumvalue_initializer = True

# -----------------------------------------------------------------------------
# C++ specific settings for PETSc-style documentation
# -----------------------------------------------------------------------------
cpp_id_attributes = [
    "VORMAKER_NAMESPACE_OPEN", "VORMAKER_NAMESPACE_CLOSE",
    "CONSTANTS_NAMESPACE_OPEN", "CONSTANTS_NAMESPACE_CLOSE",
    "ERROR_NAMESPACE_OPEN", "ERROR_NAMESPACE_CLOSE",
    "DETAIL_NAMESPACE_OPEN", "DETAIL_NAMESPACE_CLOSE",
    "GEOTYPES_NAMESPACE_OPEN", "GEOTYPES_NAMESPACE_CLOSE",
    "VMM_PUBLIC", "VMM_INTERNAL", "VMM_API", "VMM_INLINE",
    "PETSC_EXTERN", "PETSC_DEPRECATED",
]

cpp_paren_attributes = [
    "__attribute__", "__declspec", "__cdecl", "__stdcall"
]

# -----------------------------------------------------------------------------
# Intersphinx Mapping
# -----------------------------------------------------------------------------
intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "sphinx": ("https://www.sphinx-doc.org/en/master", None),
    "cgal": ("https://doc.cgal.org/latest/Manual", None),
}

# -----------------------------------------------------------------------------
# HTML extras
# -----------------------------------------------------------------------------
html_show_sourcelink = True
html_copy_source = False
todo_include_todos = True
html_show_copyright = True

# Enable equation numbering
mathjax3_config = {
    'tex': {
        'tags': 'ams',
        'tagSide': 'right',
        'useLabelIds': True
    }
}