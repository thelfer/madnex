# The `MADNEX` project

![CMake](https://github.com/thelfer/madnex/workflows/CMake/badge.svg)

This project aims at providing tools (functions, classes, bindings,
etc...) to manipulate `MADNEX` files.

`MADNEX` provide a data model on top of the [HDF5 file
format](https://www.hdfgroup.org/solutions/hdf5/), which can handle:

- Experimental data.
- [`MFront` files](http://tfel.sourceforge.net).
- [`MTest` unit tests.](http://tfel.sourceforge.net)

# Website

The official website of the project is here:
<https://thelfer.github.io/madnex/web/index.html>

# Licence

This project is released on the LGPLv3 licence. See the `LGPL-3.0.txt`
file distributed with the sources of this project.

# Dependencies

The project requires a `C++-11` compliant compiler.

The project requires the official `C++` bindings of the [HDF5
library](https://www.hdfgroup.org/solutions/hdf5/).

Python bindings requires the [`Boost/Python`
library](https://www.boost.org/).

Documentation is written using
[Doxygen](https://www.doxygen.nl/index.html).
