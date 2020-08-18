# zenoh_minimal_cpp
Minimal package to test out pure Zenoh in C++

You are on the **cmake only branch**! This package was originally designed for building with `ament` using `colcon`. But this branch is here for anyone looking to build this using pure CMake.

Do **not** use this package with `ament`! The `CMakeLists.txt` omits the necessary lines for `ament` in order to make it buildable using CMake.

## Build Instructions

You will have to manually copy in the latest `zenoh-ffi` headers and library into the `include` and `lib` directories. Just replace the ones you find there with your own.

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Then you may run the appropriate executables in the `build/bin` directory.