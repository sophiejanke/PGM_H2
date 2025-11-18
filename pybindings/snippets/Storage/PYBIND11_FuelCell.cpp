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
/// \file PYBIND11_FuelCell.cpp
///
/// \brief Bindings file for the FuelCell class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the FuelCell
/// class. Only public attributes/methods are bound!
///


pybind11::class_<FuelCellInputs>(m, "FuelCellInputs")
    .def_readwrite("n_points", &FuelCellInputs::n_points)
    .def_readwrite("fc_capital_cost_per_kW", &FuelCellInputs::fc_capital_cost_per_kW)
    .def_readwrite("fc_operation_maintenance_cost_kWh", &FuelCellInputs::fc_operation_maintenance_cost_kWh)
    .def_readwrite("B_capacity_factor", &FuelCellInputs::B_capacity_factor)
    .def_readwrite("k1", &FuelCellInputs::k1)
    .def_readwrite("k2", &FuelCellInputs::k2)
    .def_readwrite("k3", &FuelCellInputs::k3)
    .def_readwrite("k4", &FuelCellInputs::k4) 

    .def(pybind11::init());


pybind11::class_<FuelCell>(m, "FuelCell")
    .def_readwrite("fc_consumption_kg", &FuelCell::fc_consumption_kg)
    .def_readwrite("fc_output_kW", &FuelCell::fc_output_kW)
    .def_readwrite("n_points", &FuelCell::n_points)
    .def_readwrite("fc_capital_cost_per_kW", &FuelCell::fc_capital_cost_per_kW)
    .def_readwrite("fc_operation_maintenance_cost_kWh", &FuelCell::fc_operation_maintenance_cost_kWh)
    .def_readwrite("B_capacity_factor", &FuelCell::B_capacity_factor)
    .def_readwrite("fc_SOH", &FuelCell::fc_SOH)

    .def_readwrite("N_start_stop", &FuelCell::N_start_stop)
    .def_readwrite("sum_capacity_ratio", &FuelCell::sum_capacity_ratio)
    .def_readwrite("k1", &FuelCell::k1)
    .def_readwrite("k2", &FuelCell::k2)
    .def_readwrite("k3", &FuelCell::k3)
    .def_readwrite("k4", &FuelCell::k4)

    .def_readwrite("fc_output_vec_kW", &FuelCell::fc_output_vec_kW)
    .def_readwrite("fc_draw_vec_kW", &FuelCell::fc_draw_vec_kW)
    .def_readwrite("fc_consumption_vec_kg", &FuelCell::fc_consumption_vec_kg)
    .def_readwrite("fc_operation_capacity_vec", &FuelCell::fc_operation_capacity_vec)
    .def_readwrite("Q_fc_vec_kW", &FuelCell::Q_fc_vec_kW)

    .def_readwrite("N_start_stop_vec", &FuelCell::N_start_stop_vec)
    .def_readwrite("operating_capacity_ratio_vec", &FuelCell::operating_capacity_ratio_vec)
    .def_readwrite("avg_operating_capacity_ratio_vec", &FuelCell::avg_operating_capacity_ratio_vec)
    .def_readwrite("n_cap_vec", &FuelCell::n_cap_vec)
    
      
    .def(pybind11::init<>())
    .def(pybind11::init<FuelCellInputs>())

    .def("__getGenericCapitalCost", &FuelCell::__getGenericCapitalCost)
    .def("__getGenericOpMaintCost", &FuelCell::__getGenericOpMaintCost)

    .def("commitDischarge", &FuelCell::commitDischarge)
    .def("commitDraw", &FuelCell::commitDraw)

    .def("fc_degradation", &FuelCell::fc_degradation)
    .def("getQ_fc", &FuelCell::getQ_fc);
