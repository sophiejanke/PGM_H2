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
/// \file PYBIND11_Diesel.cpp
///
/// \brief Bindings file for the Diesel class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Diesel
/// class. Only public attributes/methods are bound!
///


pybind11::class_<DieselInputs>(m, "DieselInputs")
    .def_readwrite("combustion_inputs", &DieselInputs::combustion_inputs)
    .def_readwrite("replace_running_hrs", &DieselInputs::replace_running_hrs)
    .def_readwrite("capital_cost", &DieselInputs::capital_cost)
    .def_readwrite("operation_maintenance_cost_kWh", &DieselInputs::operation_maintenance_cost_kWh)
    .def_readwrite("fuel_cost_L", &DieselInputs::fuel_cost_L)
    .def_readwrite("minimum_load_ratio", &DieselInputs::minimum_load_ratio)
    .def_readwrite("minimum_runtime_hrs", &DieselInputs::minimum_runtime_hrs)
    .def_readwrite("linear_fuel_slope_LkWh", &DieselInputs::linear_fuel_slope_LkWh)
    .def_readwrite("linear_fuel_intercept_LkWh", &DieselInputs::linear_fuel_intercept_LkWh)
    .def_readwrite("CO2_emissions_intensity_kgL", &DieselInputs::CO2_emissions_intensity_kgL)
    .def_readwrite("CO_emissions_intensity_kgL", &DieselInputs::CO_emissions_intensity_kgL)
    .def_readwrite("NOx_emissions_intensity_kgL", &DieselInputs::NOx_emissions_intensity_kgL)
    .def_readwrite("SOx_emissions_intensity_kgL", &DieselInputs::SOx_emissions_intensity_kgL)
    .def_readwrite("CH4_emissions_intensity_kgL", &DieselInputs::CH4_emissions_intensity_kgL)
    .def_readwrite("PM_emissions_intensity_kgL", &DieselInputs::PM_emissions_intensity_kgL)
    
    .def(pybind11::init());


pybind11::class_<Diesel>(m, "Diesel")
    .def_readwrite("minimum_load_ratio", &Diesel::minimum_load_ratio)
    .def_readwrite("minimum_runtime_hrs", &Diesel::minimum_runtime_hrs)
    .def_readwrite("time_since_last_start_hrs", &Diesel::time_since_last_start_hrs)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, DieselInputs, std::vector<double>*>())
    .def("handleReplacement", &Diesel::handleReplacement)
    .def("requestProductionkW", &Diesel::requestProductionkW)
    .def("commit", &Diesel::commit);
