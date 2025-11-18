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
/// \file PYBIND11_Wave.cpp
///
/// \brief Bindings file for the Wave class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Wave
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<WavePowerProductionModel>(m, "WavePowerProductionModel")
    .value("WAVE_POWER_GAUSSIAN", WavePowerProductionModel::WAVE_POWER_GAUSSIAN)
    .value("WAVE_POWER_PARABOLOID", WavePowerProductionModel::WAVE_POWER_PARABOLOID)
    .value("WAVE_POWER_LOOKUP", WavePowerProductionModel::WAVE_POWER_LOOKUP)
    .value(
        "N_WAVE_POWER_PRODUCTION_MODELS",
        WavePowerProductionModel::N_WAVE_POWER_PRODUCTION_MODELS
    );


pybind11::class_<WaveInputs>(m, "WaveInputs")
    .def_readwrite("renewable_inputs", &WaveInputs::renewable_inputs)
    .def_readwrite("resource_key", &WaveInputs::resource_key)
    .def_readwrite("firmness_factor", &WaveInputs::firmness_factor)
    .def_readwrite("capital_cost", &WaveInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &WaveInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite(
        "design_significant_wave_height_m",
        &WaveInputs::design_significant_wave_height_m
    )
    .def_readwrite("design_energy_period_s", &WaveInputs::design_energy_period_s)
    .def_readwrite("power_model", &WaveInputs::power_model)
    .def_readwrite(
        "path_2_normalized_performance_matrix",
        &WaveInputs::path_2_normalized_performance_matrix
    )
    
    .def(pybind11::init());


pybind11::class_<Wave>(m, "Wave")
    .def_readwrite(
        "design_significant_wave_height_m",
        &Wave::design_significant_wave_height_m
    )
    .def_readwrite("design_energy_period_s", &Wave::design_energy_period_s)
    .def_readwrite("power_model", &Wave::power_model)
    .def_readwrite("power_model_string", &Wave::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, WaveInputs, std::vector<double>*>())
    .def("handleReplacement", &Wave::handleReplacement)
    .def("computeProductionkW", &Wave::computeProductionkW)
    .def("commit", &Wave::commit);
