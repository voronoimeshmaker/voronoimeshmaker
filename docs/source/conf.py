from pathlib import Path
import sys

project = "VoronoiMeshMaker"
author = "Instituto Politécnico, Universidade do Estado do Rio de Janeiro"
copyright = "2026, Universidade do Estado do Rio de Janeiro"
release = "0.2.0"

extensions = [
    "sphinx.ext.autosectionlabel",
]

source_suffix = {
    ".rst": "restructuredtext",
}

master_doc = "index"
language = "en"
templates_path = ["_templates"]
exclude_patterns = ["_build", "build", "Thumbs.db", ".DS_Store"]

html_theme = "alabaster"
html_static_path = ["_static"]
html_css_files = ["css/vmm.css"]
html_favicon = "_static/logo/vmm-logo.svg"
html_logo = "_static/logo/vmm-logo.svg"
html_title = "VoronoiMeshMaker"
html_short_title = "VMM"

html_theme_options = {
    "description": "Audited Voronoi finite-volume mesh preparation",
    "fixed_sidebar": True,
}

html_context = {
    "default_mode": "light",
}

autosectionlabel_prefix_document = True

repo_root = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(repo_root))

# Placeholder for future Doxygen/Breathe integration.
# breathe_projects = {"VoronoiMeshMaker": "../build/doxygen/xml"}
# breathe_default_project = "VoronoiMeshMaker"
