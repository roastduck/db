# DB

## Build

This project only works on Linux.

You need to install CMake first. e.g. `apt-get install cmake`.

For debug mode:

```
cmake -DCMAKE_BUILD_TYPE=Debug .
make
```

For release mode

```
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

Executables will be generated to `bin/`.

Remember to `make clean` before switching to the other mode.

## Test

Unit tests are written with Google Test framework (will be installed automatically by CMake). See `test/` directory for how to write them.

Run `bin/db_test` to perform a unit test.
