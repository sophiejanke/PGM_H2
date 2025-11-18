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
/// \file PYBIND11_Tidal.cpp
///
/// \brief Bindings file for the Tidal class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Tidal
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<TidalPowerProductionModel>(m, "TidalPowerProductionModel")
    .value("TIDAL_POWER_CUBIC", TidalPowerProductionModel::TIDAL_POWER_CUBIC)
    .value(
        "TIDAL_POWER_EXPONENTIAL",
        TidalPowerProductionModel::TIDAL_POWER_EXPONENTIAL
    )
    .value("TIDAL_POWER_LOOKUP", TidalPowerProductionModel::TIDAL_POWER_LOOKUP)
    .value(
        "N_TIDAL_POWER_PRODUCTION_MODELS",
        TidalPowerProductionModel::N_TIDAL_POWER_PRODUCTION_MODELS
    );


pybind11::class_<TidalInputs>(m, "TidalInputs")
    .def_readwrite("renewable_inputs", &TidalInputs::renewable_inputs)
    .def_readwrite("resource_key", &TidalInputs::resource_key)
    .def_readwrite("firmness_factor", &TidalInputs::firmness_factor)
    .def_readwrite("capital_cost", &TidalInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &TidalInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("design_speed_ms", &TidalInputs::design_speed_ms)
    .def_readwrite("power_model", &TidalInputs::power_model)
    
    .def(pybind11::init());


pybind11::class_<Tidal>(m, "Tidal")
    .def_readwrite("design_speed_ms", &Tidal::design_speed_ms)
    .def_readwrite("power_model", &Tidal::power_model)
    .def_readwrite("power_model_string", &Tidal::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, TidalInputs, std::vector<double>*>())
    .def("handleReplacement", &Tidal::handleReplacement)
    .def("computeProductionkW", &Tidal::computeProductionkW)
    .def("commit", &Tidal::commit);
