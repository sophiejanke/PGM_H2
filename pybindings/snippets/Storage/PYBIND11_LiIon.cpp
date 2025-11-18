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
/// \file PYBIND11_LiIon.cpp
///
/// \brief Bindings file for the LiIon class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the LiIon
/// class. Only public attributes/methods are bound!
///


pybind11::class_<LiIonInputs>(m, "LiIonInputs")
    .def_readwrite("storage_inputs", &LiIonInputs::storage_inputs)
    .def_readwrite("capital_cost", &LiIonInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &LiIonInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("init_SOC", &LiIonInputs::init_SOC)
    .def_readwrite("min_SOC", &LiIonInputs::min_SOC)
    .def_readwrite("hysteresis_SOC", &LiIonInputs::hysteresis_SOC)
    .def_readwrite("max_SOC", &LiIonInputs::max_SOC)
    .def_readwrite("charging_efficiency", &LiIonInputs::charging_efficiency)
    .def_readwrite("discharging_efficiency", &LiIonInputs::discharging_efficiency)
    .def_readwrite("replace_SOH", &LiIonInputs::replace_SOH)
    .def_readwrite("power_degradation_flag", &LiIonInputs::power_degradation_flag)
    .def_readwrite("degradation_alpha", &LiIonInputs::degradation_alpha)
    .def_readwrite("degradation_beta", &LiIonInputs::degradation_beta)
    .def_readwrite("degradation_B_hat_cal_0", &LiIonInputs::degradation_B_hat_cal_0)
    .def_readwrite("degradation_r_cal", &LiIonInputs::degradation_r_cal)
    .def_readwrite("degradation_Ea_cal_0", &LiIonInputs::degradation_Ea_cal_0)
    .def_readwrite("degradation_a_cal", &LiIonInputs::degradation_a_cal)
    .def_readwrite("degradation_s_cal", &LiIonInputs::degradation_s_cal)
    .def_readwrite("gas_constant_JmolK", &LiIonInputs::gas_constant_JmolK)
    .def_readwrite("gas_constant_JmolK", &LiIonInputs::gas_constant_JmolK)
    .def_readwrite("p_liion", &LiIonInputs::p_liion)
    .def_readwrite("cp_liion", &LiIonInputs::cp_liion) 
    .def_readwrite("self_discharge_rate", &LiIonInputs::self_discharge_rate)     
    
    .def(pybind11::init());


pybind11::class_<LiIon>(m, "LiIon")
    .def_readwrite("power_degradation_flag", &LiIon::power_degradation_flag)
    .def_readwrite("dynamic_energy_capacity_kWh", &LiIon::dynamic_energy_capacity_kWh)
    .def_readwrite("dynamic_power_capacity_kW", &LiIon::dynamic_power_capacity_kW)
    .def_readwrite("SOH", &LiIon::SOH)
    .def_readwrite("replace_SOH", &LiIon::replace_SOH)
    .def_readwrite("degradation_alpha", &LiIon::degradation_alpha)
    .def_readwrite("degradation_beta", &LiIon::degradation_beta)
    .def_readwrite("degradation_B_hat_cal_0", &LiIon::degradation_B_hat_cal_0)
    .def_readwrite("degradation_r_cal", &LiIon::degradation_r_cal)
    .def_readwrite("degradation_Ea_cal_0", &LiIon::degradation_Ea_cal_0)
    .def_readwrite("degradation_a_cal", &LiIon::degradation_a_cal)
    .def_readwrite("degradation_s_cal", &LiIon::degradation_s_cal)
    .def_readwrite("gas_constant_JmolK", &LiIon::gas_constant_JmolK)
    .def_readwrite("temperature_K", &LiIon::temperature_K)

    .def_readwrite("p_liion", &LiIon::p_liion)
    .def_readwrite("cp_liion", &LiIon::cp_liion)
    .def_readwrite("self_discharge_rate", &LiIon::self_discharge_rate)  

    .def_readwrite("init_SOC", &LiIon::init_SOC)
    .def_readwrite("min_SOC", &LiIon::min_SOC)
    .def_readwrite("hysteresis_SOC", &LiIon::hysteresis_SOC)
    .def_readwrite("max_SOC", &LiIon::max_SOC)
    .def_readwrite("charging_efficiency", &LiIon::charging_efficiency)
    .def_readwrite("discharging_efficiency", &LiIon::discharging_efficiency)
    .def_readwrite("SOH_vec", &LiIon::SOH_vec)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, LiIonInputs>())
    .def("handleReplacement", &LiIon::handleReplacement)
    .def("getAvailablekW", &LiIon::getAvailablekW)
    .def("getAcceptablekW", &LiIon::getAcceptablekW)
    .def("commitCharge", &LiIon::commitCharge)
    .def("commitDischarge", &LiIon::commitDischarge)
    .def("getThermalOutput", &LiIon::getThermalOutput)
    .def("getMcp", &LiIon::getMcp)
    .def("commit_SelfDischarge", &LiIon::commit_SelfDischarge);

