# PGMcpp : PRIMED Grid Modelling (in C++)

    Anthony Truelove MASc, P.Eng.
    email:   gears1763@tutanota.com
    github:  gears1763-2

***See license terms***


Example projects README for PGMcpp.

--------


## Contents

In this `projects/` sub-directory, you should find this README, an example C++ project
(`example.cpp`), an example Python 3 project (`example.py`), and an example dashboard 
script (`dashboard.py`).

--------


## Use

### C++

To compile and run the example C++ project, you can simply

    make project

from the root directory for PGMcpp (of course, you will need to have built PGMcpp
first!). This will run the model defined in `example.cpp` and then write example results
to `projects/example_cpp/`.

### Python 3

To run the example Python 3 project, you can simply

    python(3) example.py

from `projects/` (of course, you will need to have working bindings first!). This will
run the model defined in `example.py` and then write example results
to `projects/example_py/`.

### Dashboard

To run the dashboard (for the example project results), you can simply

    python(3) dashboard.py

from `projects/` aftering having run `example.py`. After doing so, you should be
presented with the IP address of a local dashboard server (running on your mahcine)
which you can then explore using the web browser of your choice. Will also work with 
the C++ example project, just need to change the `main_folder_path` in `dashboard.py`.
