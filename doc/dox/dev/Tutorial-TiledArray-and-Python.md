# Using TiledArray in Python {#Tutorial-TiledArray-and-Python}

TiledArray provides (optionally-built) bindings to Python which provide the distributed-memory tensor algebra, including
TA-like (native math DSL) and einsum-like interfaces. Efficient intra-node parallel execution is
automatically supported for most operations. This tutorial discusses how to build and use the Python bindings.

## Building TA-Python bindings

To build the bindings set the `TA_PYTHON` CMake cache
variable by providing `-DTA_PYTHON=ON` to CMake as a command-line argument. Note that all TiledArray and MADNESS runtime libraries must be compiled as position independent (this is true by default; set `CMAKE_POSITION_INDEPENDENT_CODE` CMake variable to `ON` to guarantee this).

## Using TA Python bindings

work in progress ... for now, see
[this test example](https://github.com/ValeevGroup/tiledarray/blob/ta-python-module/python/test_tiledarray.py) .
To execute this script with multiple MPI ranks do `mpirun -n X python3 test_tiledarray.py`.
