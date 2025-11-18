# PGMcpp : PRIMED Grid Modelling (in C++)

    Anthony Truelove MASc, P.Eng.
    email:   gears1763@tutanota.com
    github:  gears1763-2

***See license terms***


This is a microgrid modelling code, which can be used to assess the economic and
environmental impacts of integrating renewable energy production and storage assets 
into an otherwise isolated microgrid (presumably reliant on diesel, or other
fuel-based, generation to begin with).

The Pacific Regional Institute for Marine Energy Discovery (PRIMED):  
<https://onlineacademiccommunity.uvic.ca/primed/><br>  

The Institute for Integrated Energy Systems (IESVic):  
<https://www.uvic.ca/research/centres/iesvic/index.php>

--------


## Contents

In the directory for this project, you should find this README, a LICENSE file, a
makefile, a TODO list, and the following sub-directories:

    data/           to hold sample input data for testing and examples
    docs/           to hold various documentation
    header/         to hold header files
    projects/       to hold PGMcpp projects (ships with some example projects)
    pybindings/     to hold source and setup files for building Python 3 bindings (ships with some pre-compiled bindings)
    source/         to hold source files
    test/           to hold the source files for a suite of tests
    third_party/    to hold third party content used in the development of PGMcpp

--------


## Key Features

  * A time-domain microgrid modelling code that will work with any time series data (can be non-uniform series of arbitrary length).
  
  * Support for modelling diesel generators. This includes modelling fuel consumption and emissions. Up to 30 diesel generators can be modelled simultaneously.
  
  * Support for modelling hydro, solar, wind, tidal, and wave renewable production assets. Any number of assets can be modelled (up to memory limitations).
  
  * Any number of renewable resource time series can be modelled (up to memory limitations), with resources being associated with chosen production assets.
  
  * Support for modelling lithium ion battery energy storage. This includes modelling use-based battery degradation dynamics.
  
  * Support for modelling both load following and cycle charging dispatch control.
  
  * Can be either accessed natively in C++, or accessed in Python 3 by way of the provided bindings.

--------


##  Quick Start (Windows Tutorial Videos)

For a quick start with PGMcpp (on Windows), consider viewing the tutorial videos  

  * 1 - Getting PGMcpp Source Code from GitHub: <https://youtu.be/Z2-56tzYG0E>  

  * 2 - Compiling PGMcpp from Source (C++): <https://youtu.be/d9ozOhWM3H0>  

  * 3 - Building Python Bindings for PGMcpp: <https://youtu.be/nNRSS3f5UDE>  

  * 4 - An Example Python Project in PGMcpp: <https://youtu.be/hw8BC_ztJLo>  

--------


## Setup

### C++ Setup

To build (and test) PGMcpp, you can simply

    make PGMcpp

once appropriately set up to do so. See below for some OS-specific notes.

#### --- Linux (Debian/Ubuntu) Setup Notes ---

On Linux (Debian/Ubuntu), this should be pretty turn-key. If not, you might need to
install the build essential package; this can be done by invoking

    sudo apt-get install build-essential

#### --- Windows Setup Notes ---

On Windows, building is achieved using the environment provided by the `MSYS2` project
(see <https://www.msys2.org/>). You can follow the download and installation instructions
provided there. Specifically, be sure to

    pacman -S mingw-w64-x86_64-gcc make

and

    pacman -Syu

from within `MSYS2` after installing. Then, close `MSYS2` and run `MSYS2 MINGW64`, and
ensure everything needed has been installed by issuing

    g++ --version

and then

    make --version

If you get version info each time rather than a `command not found` error, then
everything you need is set up and ready to go.

For `MSYS2`, if you do run into any `undefined reference to` errors at compile time,
here are some possible fixes

  * You may just need to update your MSYS2. This can be done by invoking `pacman -Syu` within an MSYS2 terminal. The terminal will close and need to be restarted.
  * The debugging (`-g`) and profiling (`-p`) compiler flags may be causing issues. A solution here is to modify the `CXXFLAGS` definition in the provided `makefile` to simply `-Wall -fPIC`.
  * For missing dependencies, you likely just need to install them (`pacman`); just search for the missing dependency and you should find install instructions.

### Python 3 Setup

The `pybindings/` sub-directory contains the infrastructure needed to build Python 3 
bindings for PGMcpp (for more details, see `pybindings/README.md`). In summary, you can 
build the bindings by way of

    python(3) setup.py build_ext --inplace

depending on your setup.

--------


## Documentation

Documentation for this project is auto-generated using Doxygen
(see <https://www.doxygen.nl/>). HTML documentation can be found in
`docs/PGMcpp_manual_html.7z`, and LaTeX documentation can be found in
`docs/PGMcpp_manual_LaTeX.pdf`. Additionally, shareable references are included in
`docs/refs/`, and all references are listed in `docs/refs.bib`.

If you do make changes to the code, you can easily generate updated documentation by way
of

    make docs

assuming you are set up to do so (i.e., `doxygen` installed, etc.).

--------


## Testing

Invoking

    make PGMcpp

will build PGMcpp and then run the suite of tests defined in `test/` (for more details,
see `test/README.md`). Additionally, `pybindings/test.py` is provided to test the Python
3 bindings for PGMcpp (for more details, see `pybindings/README.md`).

The provided makefile and all source code was successfully tested on the following OS
and architectures:

    Operating System: Linux Mint 21.2
              Kernel: Linux 6.5.6-76060506-generic
        Architecture: x86-64

    Operating System: Windows 11 Home
             Version: 22H2
        Architecture: 64-bit OS, x64-based processor


The following compilers were used in testing:

    g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    g++ (Rev10, Built by MSYS2 project) 13.2.0
    Microsoft C/C++ Optimizing Compiler Version 19.37.32825


PGMcpp has the following dependencies (by compiler link):

    -lpthread

--------


## Profiling

Invoking

    make profile

will profile `test/bin/test_Model.out`, generate `gmon.out`, and then write profiling
results to `profiling_results`. The profiler being used here is `gprof` (see
<https://www.math.utah.edu/docs/info/gprof_toc.html>), and the profiling command being
issued is simply

    gprof test/bin/test_Model.out > profiling_results

Of course, `test/bin/test_Model.out` must exist for this to work, so be sure to 
`make PGMcpp` beforehand.
