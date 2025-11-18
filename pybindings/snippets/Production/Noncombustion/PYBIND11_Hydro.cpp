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
/// \file PYBIND11_Hydro.cpp
///
/// \brief Bindings file for the Hydro class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Hydro
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<HydroTurbineType>(m, "HydroTurbineType")
    .value("HYDRO_TURBINE_PELTON", HydroTurbineType::HYDRO_TURBINE_PELTON)
    .value("HYDRO_TURBINE_FRANCIS", HydroTurbineType::HYDRO_TURBINE_FRANCIS)
    .value("HYDRO_TURBINE_KAPLAN", HydroTurbineType::HYDRO_TURBINE_KAPLAN)
    .value("N_HYDRO_TURBINES", HydroTurbineType::N_HYDRO_TURBINES);


pybind11::class_<HydroInputs>(m, "HydroInputs")
    .def_readwrite("noncombustion_inputs", &HydroInputs::noncombustion_inputs)
    .def_readwrite("resource_key", &HydroInputs::resource_key)
    .def_readwrite("capital_cost", &HydroInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &HydroInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("fluid_density_kgm3", &HydroInputs::fluid_density_kgm3)
    .def_readwrite("net_head_m", &HydroInputs::net_head_m)
    .def_readwrite("reservoir_capacity_m3", &HydroInputs::reservoir_capacity_m3)
    .def_readwrite("init_reservoir_state", &HydroInputs::init_reservoir_state)
    .def_readwrite("turbine_type", &HydroInputs::turbine_type)
    
    .def(pybind11::init());


pybind11::class_<Hydro>(m, "Hydro")
    .def_readwrite("turbine_type", &Hydro::turbine_type)
    .def_readwrite("fluid_density_kgm3", &Hydro::fluid_density_kgm3)
    .def_readwrite("net_head_m", &Hydro::net_head_m)
    .def_readwrite("reservoir_capacity_m3", &Hydro::reservoir_capacity_m3)
    .def_readwrite("init_reservoir_state", &Hydro::init_reservoir_state)
    .def_readwrite("stored_volume_m3", &Hydro::stored_volume_m3)
    .def_readwrite("minimum_power_kW", &Hydro::minimum_power_kW)
    .def_readwrite("minimum_flow_m3hr", &Hydro::minimum_flow_m3hr)
    .def_readwrite("maximum_flow_m3hr", &Hydro::maximum_flow_m3hr)
    .def_readwrite("turbine_flow_vec_m3hr", &Hydro::turbine_flow_vec_m3hr)
    .def_readwrite("spill_rate_vec_m3hr", &Hydro::spill_rate_vec_m3hr)
    .def_readwrite("stored_volume_vec_m3", &Hydro::stored_volume_vec_m3)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, HydroInputs, std::vector<double>*>())
    .def("handleReplacement", &Hydro::handleReplacement)
    .def("requestProductionkW", &Hydro::requestProductionkW)
    .def("commit", &Hydro::commit);
