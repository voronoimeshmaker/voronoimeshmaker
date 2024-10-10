# VoronoiMeshMaker

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Dependencies](#dependencies)
  - [Installing Dependencies](#installing-dependencies)
- [How to Build VoronoiMeshMaker and Examples](#how-to-build-voronoimeshmaker-and-examples)
  - [Cloning the Repository](#cloning-the-repository)
  - [Building the Library](#building-the-library)
  - [Building Examples](#building-examples)
  - [Running Examples](#running-examples)
  - [Building and Running Tests](#building-and-running-tests)
- [License](#license)
- [Contact](#contact)

## Overview
VoronoiMeshMaker is a library designed to generate Voronoi meshes, which can be used to discretize both two-dimensional and three-dimensional geometries. The library is primarily intended for use in finite volume methods for solving partial differential equations. The current version of the library is 1.0 and it is in active development.

## Features
- Generate Voronoi meshes in 2D and 3D.
- Support for finite volume discretization.
- Integration with external visualization tools.

## Dependencies
To use VoronoiMeshMaker, the following libraries must be pre-installed on your system:
- **CGAL**: Required for geometric operations. Ensure you have the latest version installed.
- **VTK**: Required for visualization support.
- **TBB**: Required for parallelism.
- **GMP** and **MPFR**: Required for numerical precision in geometric calculations.
- **CMake** (version 3.10 or later): Required for building the project.

If you plan to run tests, the following library is also required:
- **Google Test (GTest)**: Required for unit testing.

### Installing Dependencies
On Ubuntu-based systems, you can install most dependencies using the following commands:

```bash
sudo apt-get update
sudo apt-get install libcgal-dev libvtk7-dev libtbb-dev libgmp-dev libmpfr-dev cmake
```
For Google Test:

```bash
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/
```

## How to Build VoronoiMeshMaker and Examples

### Cloning the Repository
Start by cloning the repository:

```bash
git clone https://github.com/username/VoronoiMeshMaker.git
cd VoronoiMeshMaker
```

### Building the Library
To build the VoronoiMeshMaker library, you need to configure the build using CMake and then compile it.

1. Create a build directory:

   ```bash
   mkdir build
   cd build
   ```

2. Configure and build the library:

   ```bash
   cmake ..
   cmake --build .
   ```

This will generate the shared library `libVoronoiMeshMaker.so` in the `lib` directory.

### Building Examples
To build the examples, enable the `BUILD_EXAMPLES` option during configuration:

```bash
cmake -S .. -B . -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build .
```

### Running Examples
After building, the executables for examples will be available in the respective directories where their source files (`.cpp`) are located. Run them with:

```bash
./path/to/example/ExampleExecutable
```

### Building and Running Tests
To build and run the tests, enable the `BUILD_TESTS` option during configuration:

```bash
cmake -S .. -B . -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=ON
cmake --build .
ctest --output-on-failure
```

## License
VoronoiMeshMaker is distributed under the GNU General Public License (GPL). For more details, see the `LICENSE` file included in the repository.

## Credits

(Any third-party tools, libraries, or resources used will be credited here.)

## Authors and Acknowledgments

(Details of contributors or acknowledgments will be added here.)

## Contact
For support or feedback, please contact:
- **Email:** [voronoimeshmaker@gmail.com](mailto:voronoimeshmaker@gmail.com)
- **GitHub Issues:** [Link to issues](https://github.com/voronoimeshmaker/voronoimeshmaker.git)