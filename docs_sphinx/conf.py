# VoronoiMeshMaker — Sphinx config (PETSc-like layout via PyData theme)
# Comments/messages in English.

import os
from pathlib import Path

# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
project = "VoronoiMeshMaker"
html_title = "VoronoiMeshMaker Documentation"

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
]

# Markdown tweaks (optional)
myst_enable_extensions = ["deflist", "smartquotes", "dollarmath"]

templates_path = ["_templates"]
exclude_patterns = ["_build"]

# -----------------------------------------------------------------------------
# PETSc-like HTML layout: PyData Sphinx Theme
# -----------------------------------------------------------------------------
html_theme = "pydata_sphinx_theme"

_here = Path(__file__).parent
_static_dir = _here / "_static"

# Only set html_static_path if the folder exists (avoids Sphinx warning)
html_static_path = ["_static"] if _static_dir.is_dir() else []

# Optional: set logo/favicon *only* if present
_logo = _static_dir / "logo.png"
_favicon = _static_dir / "favicon.ico"
html_logo = "_static/logo.png" if _logo.is_file() else None
html_favicon = "_static/favicon.ico" if _favicon.is_file() else None

# Theme options (navbar, repo icon, optional version switcher)
navbar_end_items = ["theme-switcher", "navbar-icon-links"]
theme_options = {
    "navbar_end": navbar_end_items.copy(),
    "show_nav_level": 2,
    "navigation_depth": 4,
    # DO NOT enable 'use_edit_page_button' unless html_context is set properly.
    # 'repository_url', 'repository_branch' and 'path_to_docs' are not supported
    # by this theme version -> removed to avoid warnings.
    "icon_links": [
        {
            "name": "GitHub",
            "url": "https://github.com/SEU_USUARIO/SEU_REPO",
            "icon": "fa-brands fa-github",
        },
    ],
}

# Optional version switcher (enable only if _static/versions.json exists)
_versions_json = _static_dir / "versions.json"
if _versions_json.is_file():
    theme_options["switcher"] = {
        "json_url": "_static/versions.json",
        "version_match": os.environ.get("VMM_DOC_VERSION", "dev"),
    }
    theme_options["navbar_end"] = navbar_end_items + ["version-switcher"]

html_theme_options = theme_options

# Code style
pygments_style = "default"
pygments_dark_style = "native"

# -----------------------------------------------------------------------------
# Treat project macros as C++ attributes so parser ignores them (safety net)
# -----------------------------------------------------------------------------
cpp_id_attributes = [
    "VORMAKER_NAMESPACE_OPEN", "VORMAKER_NAMESPACE_CLOSE",
    "CONSTANTS_NAMESPACE_OPEN", "CONSTANTS_NAMESPACE_CLOSE",
    "ERROR_NAMESPACE_OPEN",    "ERROR_NAMESPACE_CLOSE",
    "DETAIL_NAMESPACE_OPEN",   "DETAIL_NAMESPACE_CLOSE",
    "GEOTYPES_NAMESPACE_OPEN", "GEOTYPES_NAMESPACE_CLOSE",
    "VMM_PUBLIC", "VMM_INTERNAL", "VMM_API", "VMM_INLINE",
]
cpp_paren_attributes = []

# -----------------------------------------------------------------------------
# Breathe (reads Doxygen XML path from env passed by CMake)
# -----------------------------------------------------------------------------
def _sanitize(path: str) -> str:
    if not path:
        return ""
    p = path.strip().strip('"').strip("'")
    return os.path.abspath(p) if not os.path.isabs(p) else p

import os.path  # after function: used below
doxy_xml = _sanitize(os.environ.get("VMM_DOXY_XML", ""))

breathe_projects = {}
if doxy_xml and os.path.exists(os.path.join(doxy_xml, "index.xml")):
    breathe_projects = {"VoronoiMeshMaker": doxy_xml}
breathe_default_project = "VoronoiMeshMaker"

# -----------------------------------------------------------------------------
# Intersphinx (Sphinx 8: inventory must be a string or None — not {})
# -----------------------------------------------------------------------------
intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "sphinx": ("https://www.sphinx-doc.org/en/master", None),
}

# -----------------------------------------------------------------------------
# HTML extras
# -----------------------------------------------------------------------------
html_show_sourcelink = True
html_copy_source = True
todo_include_todos = True
