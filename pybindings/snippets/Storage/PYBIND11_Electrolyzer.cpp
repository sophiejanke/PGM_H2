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
/// \file PYBIND11_Electrolyzer.cpp
///
/// \brief Bindings file for the Electrolyzer class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Electrolyzer
/// class. Only public attributes/methods are bound!
///



pybind11::class_<ElectrolyzerInputs>(m, "ElectrolyzerInputs")
    .def_readwrite("n_points", &ElectrolyzerInputs::n_points)
    .def_readwrite("el_capital_cost_per_kW", &ElectrolyzerInputs::el_capital_cost_per_kW)
    .def_readwrite("el_operation_maintenance_cost_kWh", &ElectrolyzerInputs::el_operation_maintenance_cost_kWh)
    .def_readwrite("B_capacity_factor", &ElectrolyzerInputs::B_capacity_factor)
    .def_readwrite("k1", &ElectrolyzerInputs::k1)
    .def_readwrite("k2", &ElectrolyzerInputs::k2)
    .def_readwrite("k3", &ElectrolyzerInputs::k3)
    .def_readwrite("k4", &ElectrolyzerInputs::k4)

    .def(pybind11::init());


pybind11::class_<Electrolyzer>(m, "Electrolyzer")
    .def_readwrite("el_output_kg", &Electrolyzer::el_output_kg)
    .def_readwrite("el_draw_kW", &Electrolyzer::el_draw_kW)
    .def_readwrite("charging_efficiency", &Electrolyzer::charging_efficiency)
    .def_readwrite("n_points", &Electrolyzer::n_points)
    .def_readwrite("el_capital_cost_per_kW", &Electrolyzer::el_capital_cost_per_kW)
    .def_readwrite("el_operation_maintenance_cost_kWh", &Electrolyzer::el_operation_maintenance_cost_kWh)
    .def_readwrite("el_SOH", &Electrolyzer::el_SOH)
    .def_readwrite("B_capacity_factor", &Electrolyzer::B_capacity_factor) 
    .def_readwrite("N_start_stop", &Electrolyzer::N_start_stop)
    .def_readwrite("sum_capacity_ratio", &Electrolyzer::sum_capacity_ratio)
    .def_readwrite("k1", &Electrolyzer::k1)
    .def_readwrite("k2", &Electrolyzer::k2)
    .def_readwrite("k3", &Electrolyzer::k3)
    .def_readwrite("k4", &Electrolyzer::k4)

    .def_readwrite("el_output_vec_kg", &Electrolyzer::el_output_vec_kg)
    .def_readwrite("Q_el_vec_kW", &Electrolyzer::Q_el_vec_kW)
    .def_readwrite("el_draw_vec_kW", &Electrolyzer::el_draw_vec_kW)
    .def_readwrite("n_cap_vec", &Electrolyzer::n_cap_vec)

      
    .def(pybind11::init<>())
    .def(pybind11::init<ElectrolyzerInputs>())

    .def("__getGenericCapitalCost", &Electrolyzer::__getGenericCapitalCost)
    .def("__getGenericOpMaintCost", &Electrolyzer::__getGenericOpMaintCost)

    .def("commitCharge", &Electrolyzer::commitCharge)
    .def("commitDraw", &Electrolyzer::commitDraw)

    .def("el_degradation", &Electrolyzer::el_degradation)   
    .def("getQ_el", &Electrolyzer::getQ_el);
