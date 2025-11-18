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
/// \file PYBIND11_Controller.cpp
///
/// \brief Bindings file for the Controller class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Controller
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<ControlMode>(m, "ControlMode")
    .value("LOAD_FOLLOWING", ControlMode::LOAD_FOLLOWING)
    .value("CYCLE_CHARGING", ControlMode::CYCLE_CHARGING)
    .value("N_CONTROL_MODES", ControlMode::N_CONTROL_MODES);


pybind11::class_<Controller>(m, "Controller")
    .def_readwrite("control_mode", &Controller::control_mode)
    .def_readwrite("control_string", &Controller::control_string)
    .def_readwrite("firm_dispatch_ratio", &Controller::firm_dispatch_ratio)
    .def_readwrite("load_reserve_ratio", &Controller::load_reserve_ratio)
    .def_readwrite("net_load_vec_kW", &Controller::net_load_vec_kW)
    .def_readwrite("missed_load_vec_kW", &Controller::missed_load_vec_kW)
    .def_readwrite(
        "missed_firm_dispatch_vec_kW",
        &Controller::missed_firm_dispatch_vec_kW
    )
    .def_readwrite(
        "missed_spinning_reserve_vec_kW",
        &Controller::missed_spinning_reserve_vec_kW
    )
    .def_readwrite("combustion_map", &Controller::combustion_map)
    
    .def(pybind11::init<>())
    .def("setControlMode", &Controller::setControlMode)
    .def("init", &Controller::init)
    .def("applyDispatchControl", &Controller::applyDispatchControl)
    .def("clear", &Controller::clear);
