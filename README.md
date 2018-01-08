# DB

A tiny SQL database.

Copyright © Tang Shizhi

## Dependencies

This project only works on Linux.

You need to install the two dependencies listed below manually:

- CMake. This is a compile script generator. If you are on a Ubuntu, install it via `apt-get install cmake`.
- uuid-dev. This required by ANTLR parser generator. Likewise, install it via `apt-get install uuid-dev`.

Other dependencies listed below can be downloaded automatically, that you DON'T have to install manually:

- ANTLR4. This is used to generate SQL parser (just like LEX/YACC). Usually you don't have to re-generate the parser, and in this situation only the runtime library of ANTLR4 is used.
- Java Runtime (OPTIONAL). If you want to re-generate the parser code, Java Runtime is required by ANTLR4.
- Google Test + Google Mock (OPTIONAL). This is a unit test framework, which we use to preform unit tests.

## Build

Quick build:

```
cmake .
make
```

Advanced options (NOT mandatory):

- Append `-DCMAKE_BUILD_TYPE=Debug` to `cmake` to enable debug mode.
- Append `-DREBUILD_PARSER=YES` to re-generate to SQL parser.
- Append `-DBUILD_TEST=YES` to compile unit tests.

Executables will be generated to `bin/`.

NOTE: Remember to `make clean` before changing CMake options.

## Run

Run `bin/db` to enter a human friendly interative SQL console.

Run `bin/db < input.sql > output.csv` to input commands from a SQL file and output results to a CSV file.

Run `bin/db` with `-dPATH_TO_DB` or `--db-path=PATH_TO_DB` to specify a directory where DB files are stored in (will automatically create one if not exists).

Run `bin/db --help` for further help information.

Run `bin/db_test` to perform a unit test (if compiled).

## Source-level document

Doxygen script is provided to generate a source-level document. Run `doxygen Doxyfile` to generate the document to `doxygen_doc/`. Then, open `doxygen_doc/html/index.html` in a browser to read the document.

