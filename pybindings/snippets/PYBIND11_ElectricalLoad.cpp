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
/// \file PYBIND11_ElectricalLoad.cpp
///
/// \brief Bindings file for the ElectricalLoad class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the ElectricalLoad
/// class. Only public attributes/methods are bound!
///


pybind11::class_<ElectricalLoad>(m, "ElectricalLoad")
    .def_readwrite("n_points", &ElectricalLoad::n_points)
    .def_readwrite("n_years", &ElectricalLoad::n_years)
    .def_readwrite("min_load_kW", &ElectricalLoad::min_load_kW)
    .def_readwrite("mean_load_kW", &ElectricalLoad::mean_load_kW)
    .def_readwrite("max_load_kW", &ElectricalLoad::max_load_kW)
    .def_readwrite("path_2_electrical_load_time_series", &ElectricalLoad::path_2_electrical_load_time_series)
    .def_readwrite("time_vec_hrs", &ElectricalLoad::time_vec_hrs)
    .def_readwrite("dt_vec_hrs", &ElectricalLoad::dt_vec_hrs)
    .def_readwrite("load_vec_kW", &ElectricalLoad::load_vec_kW)
    
    .def(pybind11::init<>())
    .def(pybind11::init<std::string>())
    .def("readLoadData", &ElectricalLoad::readLoadData)
    .def("clear", &ElectricalLoad::clear);
