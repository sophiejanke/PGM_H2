# PGMcpp : PRIMED Grid Modelling (in C++)

    Anthony Truelove MASc, P.Eng.
    email:   gears1763@tutanota.com
    github:  gears1763-2

***See license terms***


Testing README for PGMcpp. This is for the tests that are run on the C++ build of PGMcpp.
These tests are not run on the Python 3 bindings (for more details on that, see
`pybindings/README.md`).

--------


## Contents

In this `test/` sub-directory, you should find this README and the following
sub-directories:

    source/     to hold the various testing source files (has the same file structure as `source/`)
    utils/      to hold various testing utilities

Note that each `test/source/../test_<>.cpp` contains the tests corresponding to
`source/../<>.cpp`.

--------


## Use

The provided makefile is set up such that invoking

    make PGMcpp

will build PGMcpp and then run the test suite. If you see `PASS` across the board, then
the build and testing was successful.
