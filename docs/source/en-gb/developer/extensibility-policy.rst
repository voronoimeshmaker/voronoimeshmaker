Extensibility Policy
====================

Functional categories that may grow with workflows use traits, tags, identifiers
or registries. Closed enums are avoided when new functionality would require
editing several unrelated files.

The current exception is the compile-time dimension selector, which remains
closed because it drives template specialisation for 2D and 3D code paths.
