from pathlib import Path
import sys

project = "VoronoiMeshMaker"
author = "Joao Flavio Vieira de Vasconcellos"
copyright = "2026, Joao Flavio Vieira de Vasconcellos"

extensions = [
    "sphinx.ext.autosectionlabel",
]

templates_path = ["_templates"]
exclude_patterns = []

html_theme = "alabaster"
html_static_path = ["_static"]
html_title = "VoronoiMeshMaker"
html_short_title = "VMM"

autosectionlabel_prefix_document = True

repo_root = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(repo_root))
