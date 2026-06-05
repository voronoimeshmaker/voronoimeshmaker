# Text audit format

## Purpose

This format is for human inspection, debugging and regression tests.

It is not the main interchange format and does not replace HDF5.

## Header 2D

```text
VMM_CONNECTIVITY_AUDIT 1
DIMENSION 2
ORDER clockwise
BOUNDARY_NEIGHBOUR -1
MISSING_VALUE nan
NCELLS <n>
NFACES <n>
NNODES <n>
NPATCHES <n>
INDEX_STATUS ok
CONNECTIVITY_STATUS ok
GEOMETRY_STATUS ok
```

## Cell block 2D

```text
CELL 15
AREA 3.4829100000000000e+01
NFACES 5

# local  face_id  neighbour  distance              normal_distance       length                patch_id  patch_type  status
0        103      14         2.1030000000000000e+00 2.1000000000000000e+00 1.3400000000000000e+00 -1        internal    ok
1        104      22         2.4510000000000000e+00 2.4400000000000000e+00 1.2200000000000000e+00 -1        internal    midpoint_off_face
2        105      37         2.2100000000000000e+00 2.2000000000000000e+00 1.4100000000000000e+00 -1        internal    ok
3        106      -1         nan                    nan                    8.5000000000000000e-01 4         island      boundary_projection_off_face
4        107      11         1.9800000000000000e+00 1.9600000000000000e+00 1.3000000000000000e+00 -1        internal    ok

END_CELL
```

## Ordering 2D

Faces are printed clockwise around the cell, ordered by face centre angle relative to cell centre.

Use face centre, not neighbour centre, because boundary faces do not have neighbours.

## Header 3D extruded

```text
VMM_CONNECTIVITY_AUDIT 1
DIMENSION 3
ORDER lateral_clockwise_then_bottom_top
BOUNDARY_NEIGHBOUR -1
MISSING_VALUE nan
```

## Cell block 3D extruded

```text
CELL 42
VOLUME 1.8234000000000000e+02
NFACES 8

# local  role     face_id  neighbour  distance              normal_distance       area                  patch_id  patch_type  status
0        lateral  880      38         5.1200000000000000e+00 5.0800000000000000e+00 1.2440000000000000e+01 -1        internal    ok
1        lateral  881      43         4.9800000000000000e+00 4.9100000000000000e+00 1.1870000000000000e+01 -1        internal    ok
2        lateral  882      51         3.7700000000000000e+00 3.7100000000000000e+00 9.3400000000000000e+00 -1        internal    ok
3        lateral  883      -1         nan                    nan                    5.4400000000000000e+00 4         island      boundary_projection_off_face
4        lateral  884      -1         nan                    nan                    6.1000000000000000e+00 1         wall        ok
5        lateral  885      39         4.4400000000000000e+00 4.4000000000000000e+00 1.0020000000000000e+01 -1        internal    ok
6        bottom   886      17         2.0000000000000000e+00 2.0000000000000000e+00 2.4839200000000000e+01 -1        internal    ok
7        top      887      67         2.0000000000000000e+00 2.0000000000000000e+00 2.4839200000000000e+01 -1        internal    ok

END_CELL
```

## General 3D

For general polyhedral 3D, do not promise clockwise order. Use:

```text
ORDER face_id
```

or:

```text
ORDER deterministic
```

## Optional sections

At the end of the file:

```text
BOUNDARY_PROJECTION_ANOMALIES
# cell_id face_id patch_id segment_id x y distance status

INTERNAL_FACE_ANOMALIES
# face_id owner neighbour midpoint_x midpoint_y offset status
```
