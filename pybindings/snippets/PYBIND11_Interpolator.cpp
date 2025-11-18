/*
 * PGMcpp : PRIMED Grid Modelling (in C++)
 * Copyright 2023 (C)
 * 
 * Anthony Truelove MASc, P.Eng.
 * email:  gears1763@tutanota.com
 * github: gears1763-2
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 * 
 *  CONTINUED USE OF THIS SOFTWARE CONSTITUTES ACCEPTANCE OF THESE TERMS.
 *
 */


///
/// \file PYBIND11_Interpolator.cpp
///
/// \brief Bindings file for the Interpolator class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Interpolator
/// class. Only public attributes/methods are bound!
///


pybind11::class_<InterpolatorStruct1D>(m, "InterpolatorStruct1D")
    .def_readwrite("n_points", &InterpolatorStruct1D::n_points)
    .def_readwrite("x_vec", &InterpolatorStruct1D::x_vec)
    .def_readwrite("min_x", &InterpolatorStruct1D::min_x)
    .def_readwrite("max_x", &InterpolatorStruct1D::max_x)
    .def_readwrite("y_vec", &InterpolatorStruct1D::y_vec)
    
    .def(pybind11::init());


pybind11::class_<InterpolatorStruct2D>(m, "InterpolatorStruct2D")
    .def_readwrite("n_rows", &InterpolatorStruct2D::n_rows)
    .def_readwrite("n_cols", &InterpolatorStruct2D::n_cols)
    .def_readwrite("x_vec", &InterpolatorStruct2D::x_vec)
    .def_readwrite("min_x", &InterpolatorStruct2D::min_x)
    .def_readwrite("max_x", &InterpolatorStruct2D::max_x)
    .def_readwrite("y_vec", &InterpolatorStruct2D::y_vec)
    .def_readwrite("min_y", &InterpolatorStruct2D::min_y)
    .def_readwrite("max_y", &InterpolatorStruct2D::max_y)
    .def_readwrite("z_matrix", &InterpolatorStruct2D::z_matrix)
    
    .def(pybind11::init());


pybind11::class_<Interpolator>(m, "Interpolator")
    .def_readwrite("interp_map_1D", &Interpolator::interp_map_1D)
    .def_readwrite("path_map_1D", &Interpolator::path_map_1D)
    .def_readwrite("interp_map_2D", &Interpolator::interp_map_2D)
    .def_readwrite("path_map_2D", &Interpolator::path_map_2D)
    
    .def(pybind11::init<>())
    .def("addData1D", &Interpolator::addData1D)
    .def("addData2D", &Interpolator::addData2D)
    .def("interp1D", &Interpolator::interp1D)
    .def("interp2D", &Interpolator::interp2D);
