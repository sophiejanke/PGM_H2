/*
 * PGMcpp : PRIMED Grid Modelling (in C++)
 * Copyrighad 2023 (C)
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
/// \file PYBIND11_ThermalModel.cpp
///
/// \brief Bindings file for the ThermalModel class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Model
/// class. Only public attributes/methods are bound!
///


pybind11::class_<ThermalModelInputs>(m, "ThermalModelInputs")
    .def_readwrite("path_2_environmental_temperature_time_series", &ThermalModelInputs::path_2_environmental_temperature_time_series)
    .def_readwrite("R_storage_housing", &ThermalModelInputs::R_storage_housing)
    .def_readwrite("A_storage_housing", &ThermalModelInputs::A_storage_housing)
    .def_readwrite("V_storage_housing", &ThermalModelInputs::V_storage_housing)
    .def_readwrite("cp_air", &ThermalModelInputs::cp_air)
    .def_readwrite("x_air", &ThermalModelInputs::x_air)
    .def_readwrite("p_air", &ThermalModelInputs::p_air)
    .def_readwrite("T_room_initial", &ThermalModelInputs::T_room_initial)
    .def_readwrite("T_storage_housing_min_h2", &ThermalModelInputs::T_storage_housing_min_h2)
    .def_readwrite("T_storage_housing_max_h2", &ThermalModelInputs::T_storage_housing_max_h2)
    .def_readwrite("T_storage_housing_min_liion", &ThermalModelInputs::T_storage_housing_min_liion)
    .def_readwrite("T_storage_housing_max_liion", &ThermalModelInputs::T_storage_housing_max_liion)
    .def_readwrite("n_heat_transfer", &ThermalModelInputs::n_heat_transfer)
    .def_readwrite("n_storage_heat_transfer", &ThermalModelInputs::n_storage_heat_transfer)

    .def(pybind11::init());

pybind11::class_<ThermalModel>(m, "ThermalModel")

    .def_readwrite("n_points", &ThermalModel::n_points)
    .def_readwrite("R_storage_housing", &ThermalModel::R_storage_housing)
    .def_readwrite("A_storage_housing", &ThermalModel::A_storage_housing)
    .def_readwrite("V_storage_housing", &ThermalModel::V_storage_housing)
    .def_readwrite("cp_air", &ThermalModel::cp_air)
    .def_readwrite("x_air", &ThermalModel::x_air)
    .def_readwrite("p_air", &ThermalModel::p_air)
    .def_readwrite("m_air", &ThermalModel::m_air)
    .def_readwrite("T_room_initial", &ThermalModel::T_room_initial)
    .def_readwrite("T_room_h2", &ThermalModel::T_room_h2)
    .def_readwrite("T_room_liion", &ThermalModel::T_room_liion)
    .def_readwrite("T_storage_housing_min_h2", &ThermalModel::T_storage_housing_min_h2)
    .def_readwrite("T_storage_housing_max_h2", &ThermalModel::T_storage_housing_max_h2)
    .def_readwrite("T_storage_housing_min_liion", &ThermalModel::T_storage_housing_min_liion)
    .def_readwrite("T_storage_housing_max_liion", &ThermalModel::T_storage_housing_max_liion)

    .def_readwrite("sending_Q_to_liion", &ThermalModel::sending_Q_to_liion)
    .def_readwrite("Q_h2_to_liion_kW", &ThermalModel::Q_h2_to_liion_kW)
    .def_readwrite("Q_req_h2_kW", &ThermalModel::Q_req_h2_kW)
    .def_readwrite("Q_req_liion_kW", &ThermalModel::Q_req_liion_kW)
    .def_readwrite("Q_external_heating_potential_kW", &ThermalModel::Q_external_heating_potential_kW)
    .def_readwrite("Q_storage_heating_load_kW", &ThermalModel::Q_storage_heating_load_kW)
    .def_readwrite("n_heat_transfer", &ThermalModel::n_heat_transfer)    
    .def_readwrite("n_storage_heat_transfer", &ThermalModel::n_storage_heat_transfer)   

    .def_readwrite("path_2_environmental_temperature_time_series", &ThermalModel::path_2_environmental_temperature_time_series)

    .def_readwrite("storage_ptr_vec", &ThermalModel::storage_ptr_vec)

    .def_readwrite("T_room_h2_vec_C", &ThermalModel::T_room_h2_vec_C) 
    .def_readwrite("H2_thermal_out_vec_kW", &ThermalModel::H2_thermal_out_vec_kW) 
    .def_readwrite("T_room_liion_vec_C", &ThermalModel::T_room_liion_vec_C) 
    .def_readwrite("T_env_vec_C", &ThermalModel::T_env_vec_C) 
    .def_readwrite("Q_req_liion_vec_kW", &ThermalModel::Q_req_liion_vec_kW) 
    .def_readwrite("Q_req_h2_vec_kW", &ThermalModel::Q_req_h2_vec_kW) 
    .def_readwrite("Q_req_remaining_vec_kW", &ThermalModel::Q_req_remaining_vec_kW) 
    .def_readwrite("Q_heating_external_vec_kW", &ThermalModel::Q_heating_external_vec_kW) 
    .def_readwrite("Q_load_vec_kW", &ThermalModel::Q_load_vec_kW) 
    .def_readwrite("Q_h2_to_liion_vec_kW", &ThermalModel::Q_h2_to_liion_vec_kW) 

    .def(pybind11::init<>())
    .def(pybind11::init<ThermalModelInputs>())
    
    .def("commitLiIonThermalTracking", &ThermalModel::commitLiIonThermalTracking)
    .def("commitH2ThermalTracking", &ThermalModel::commitH2ThermalTracking)
    .def("commitThermalBalance", &ThermalModel::commitThermalBalance)
    .def("__readTemperatureData", &ThermalModel::__readTemperatureData)
    .def("__checkInputs", &ThermalModel::__checkInputs)
    .def("__writeSummary", &ThermalModel::__writeSummary)
    .def("__writeTimeSeries", &ThermalModel::__writeTimeSeries)

    .def("clear", &ThermalModel::clear);

