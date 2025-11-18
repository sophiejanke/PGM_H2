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
/// \file PYBIND11_Wind.cpp
///
/// \brief Bindings file for the Wind class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Wind
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<WindPowerProductionModel>(m, "WindPowerProductionModel")
    .value("WIND_POWER_CUBIC", WindPowerProductionModel::WIND_POWER_CUBIC)
    .value("WIND_POWER_EXPONENTIAL", WindPowerProductionModel::WIND_POWER_EXPONENTIAL)
    .value("WIND_POWER_LOOKUP", WindPowerProductionModel::WIND_POWER_LOOKUP)
    .value(
        "N_WIND_POWER_PRODUCTION_MODELS",
        WindPowerProductionModel::N_WIND_POWER_PRODUCTION_MODELS
    );


pybind11::class_<WindInputs>(m, "WindInputs")
    .def_readwrite("renewable_inputs", &WindInputs::renewable_inputs)
    .def_readwrite("resource_key", &WindInputs::resource_key)
    .def_readwrite("firmness_factor", &WindInputs::firmness_factor)
    .def_readwrite("capital_cost", &WindInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &WindInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("design_speed_ms", &WindInputs::design_speed_ms)
    .def_readwrite("power_model", &WindInputs::power_model)
    
    .def(pybind11::init());


pybind11::class_<Wind>(m, "Wind")
    .def_readwrite("design_speed_ms", &Wind::design_speed_ms)
    .def_readwrite("power_model", &Wind::power_model)
    .def_readwrite("power_model_string", &Wind::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, WindInputs, std::vector<double>*>())
    .def("handleReplacement", &Wind::handleReplacement)
    .def("computeProductionkW", &Wind::computeProductionkW)
    .def("commit", &Wind::commit);
