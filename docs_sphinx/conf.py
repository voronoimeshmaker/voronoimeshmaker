# VoronoiMeshMaker — Sphinx config (PETSc-like layout via PyData theme)

import os
import sys
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
# Breathe Configuration - COMPLETAMENTE GENÉRICO
# -----------------------------------------------------------------------------
def find_doxygen_xml_directory():
    """
    Find Doxygen XML directory robustly in this order:
    1. VMM_DOXY_XML environment variable (set by CMake)
    2. Common build directories
    3. Any directory containing doxygen output
    4. Default fallback with clear error message
    """
    # 1. Check environment variable (set by CMake during build)
    env_path = os.environ.get('VMM_DOXY_XML')
    if env_path:
        env_path = Path(env_path)
        if env_path.exists():
            print(f"Using Doxygen XML from environment: {env_path}")
            return str(env_path)
        else:
            print(f"Warning: VMM_DOXY_XML path does not exist: {env_path}")
    
    # 2. Check common build directories
    common_build_dirs = [
        "../build",
        "../cmake-build", 
        "../cmake-build-debug",
        "../cmake-build-release",
        "../out/build",
        "../x64",
        "../bin",
        "../.build"
    ]
    
    for build_dir in common_build_dirs:
        xml_path = Path(build_dir) / "docs_doxygen" / "xml"
        if xml_path.exists():
            print(f"Using Doxygen XML from build directory: {xml_path}")
            return str(xml_path)
    
    # 3. Search for any doxygen output in parent directories
    for item in Path("..").rglob("docs_doxygen/xml"):
        if item.exists():
            print(f"Found Doxygen XML at: {item}")
            return str(item)
    
    # 4. Final fallback with clear error message
    default_path = "../build/docs_doxygen/xml"
    print(f"Warning: Doxygen XML not found. Using fallback: {default_path}")
    print("To fix this, run: cmake --build <build-dir> --target doc_doxygen")
    return default_path

# Configure Breathe
doxygen_xml_path = find_doxygen_xml_directory()
breathe_projects = {
    "VoronoiMeshMaker": doxygen_xml_path
}

print(f"Breathe configured with: {breathe_projects['VoronoiMeshMaker']}")

breathe_default_project = "VoronoiMeshMaker"
breathe_domain_by_extension = {
    "h": "c",
    "hpp": "cpp",
    "cpp": "cpp",
    "tpp": "cpp",
    "ipp": "cpp",
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
    # CGAL comentado - não tem inventário intersphinx válido
    # "cgal": ("https://doc.cgal.org/latest/Manual", None),
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

# -----------------------------------------------------------------------------
# Debug info
# -----------------------------------------------------------------------------
print(f"Python executable: {sys.executable}")
print(f"Current directory: {Path.cwd()}")
print(f"Sphinx source: {_here}")