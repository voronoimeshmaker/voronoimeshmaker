Error Codes Catalog
===================

All error codes follow the pattern ``[CATEGORY:CODE]`` and are bilingual by default.

.. csv-table:: Validation Errors
   :header: "Code", "Message (PT)", "Message (EN)", "Common Cause"
   :widths: 10, 30, 30, 30

   "VALIDATION:1042", "Volume de célula negativo detectado", "Negative cell volume detected", "Self-intersecting polygon, insufficient seeds"
   "VALIDATION:1043", "Ortogonalidade abaixo do limite", "Orthogonality below threshold", "Highly skewed refinement metric, degenerate seeds"

.. csv-table:: Generation Errors
   :header: "Code", "Message (PT)", "Message (EN)", "Common Cause"
   :widths: 10, 30, 30, 30

   "GENERATION:2001", "Falha na triangulação: precisão insuficiente", "Triangulation failed: insufficient precision", "Complex geometry, kernel tolerance mismatch"
   "GENERATION:2002", "Sementes duplicadas detectadas", "Duplicate seeds detected", "Poisson distance too small, manual input error"