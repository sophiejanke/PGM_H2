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
/// \file PYBIND11_H2.cpp
///
/// \brief Bindings file for the H2 class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the H2
/// class. Only public attributes/methods are bound!
///


pybind11::class_<H2Inputs>(m, "H2Inputs")
    .def_readwrite("storage_inputs", &H2Inputs::storage_inputs)
    .def_readwrite("electrolyzer_inputs", &H2Inputs::electrolyzer_inputs)
    .def_readwrite("fuelcell_inputs", &H2Inputs::fuelcell_inputs)
    .def_readwrite("path_2_external_hydrogen_load_time_series", &H2Inputs::path_2_external_hydrogen_load_time_series)

    .def_readwrite("capital_cost", &H2Inputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &H2Inputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("h2system_operation_maintenance_cost_kWh", &H2Inputs::h2system_operation_maintenance_cost_kWh)

    .def_readwrite("init_SOC", &H2Inputs::init_SOC)
    .def_readwrite("min_SOC", &H2Inputs::min_SOC)
    .def_readwrite("hysteresis_SOC", &H2Inputs::hysteresis_SOC)
    .def_readwrite("max_SOC", &H2Inputs::max_SOC)
    
    .def_readwrite("replace_SOH_el", &H2Inputs::replace_SOH_el)
    .def_readwrite("replace_SOH_fc", &H2Inputs::replace_SOH_fc)
    .def_readwrite("power_degradation_flag", &H2Inputs::power_degradation_flag)

    .def_readwrite("kWh_kg_conversion", &H2Inputs::kWh_kg_conversion)
    .def_readwrite("fc_min_load_ratio", &H2Inputs::fc_min_load_ratio)
    .def_readwrite("el_min_load_ratio", &H2Inputs::el_min_load_ratio)

    .def_readwrite("fc_min_runtime", &H2Inputs::fc_min_runtime)
    .def_readwrite("el_min_runtime", &H2Inputs::el_min_runtime)
    .def_readwrite("fc_ramp_loss", &H2Inputs::fc_ramp_loss)
    .def_readwrite("el_ramp_loss", &H2Inputs::el_ramp_loss)

    .def_readwrite("el_capacity_kW", &H2Inputs::el_capacity_kW)
    .def_readwrite("el_quantity", &H2Inputs::el_quantity)
    .def_readwrite("el_spec_consumption_kWh", &H2Inputs::el_spec_consumption_kWh)
    .def_readwrite("fc_capacity_kW", &H2Inputs::fc_capacity_kW)
    .def_readwrite("fc_quantity", &H2Inputs::fc_quantity)
    .def_readwrite("fc_spec_consumption_kg", &H2Inputs::fc_spec_consumption_kg)
    .def_readwrite("h2_tank_capacity_kg", &H2Inputs::h2_tank_capacity_kg)
    .def_readwrite("h2_tank_cost_kg", &H2Inputs::h2_tank_cost_kg)
    .def_readwrite("compressor_spec_consumption_kWh", &H2Inputs::compressor_spec_consumption_kWh)
    .def_readwrite("n_compressor", &H2Inputs::n_compressor)   
    .def_readwrite("compression_included", &H2Inputs::compression_included)
    .def_readwrite("excess_hydrogen_potential_included", &H2Inputs::excess_hydrogen_potential_included)
    .def_readwrite("compressor_cap_cost_kW", &H2Inputs::compressor_cap_cost_kW)
    .def_readwrite("water_treatment_included", &H2Inputs::water_treatment_included)
    .def_readwrite("external_hydrogen_load_included", &H2Inputs::external_hydrogen_load_included)        
    .def_readwrite("water_treatment_cap_cost", &H2Inputs::water_treatment_cap_cost)
    .def_readwrite("water_demand_L", &H2Inputs::water_demand_L)

    .def_readwrite("cp_air", &H2Inputs::cp_air)
    .def_readwrite("cp_el", &H2Inputs::cp_el)
    .def_readwrite("cp_fc", &H2Inputs::cp_fc)
    .def_readwrite("p_el", &H2Inputs::p_el)
    .def_readwrite("p_fc", &H2Inputs::p_fc)
    .def_readwrite("p_air", &H2Inputs::p_air)
    .def_readwrite("v_housing", &H2Inputs::v_housing)

    .def(pybind11::init());


pybind11::class_<H2>(m, "H2")
    .def_readwrite("electrolyzer", &H2::electrolyzer)
    .def_readwrite("fuelcell", &H2::fuelcell)

    .def_readwrite("path_2_external_hydrogen_load_time_series", &H2::path_2_external_hydrogen_load_time_series)

    .def_readwrite("h2system_operation_maintenance_cost_kWh", &H2::h2system_operation_maintenance_cost_kWh)

    .def_readwrite("dynamic_h2_energy_capacity_kWh", &H2::dynamic_h2_energy_capacity_kWh)
    .def_readwrite("dynamic_h2_power_capacity_kW", &H2::dynamic_h2_power_capacity_kW)

    .def_readwrite("SOH_el", &H2::SOH_el)
    .def_readwrite("SOH_fc", &H2::SOH_fc)
    .def_readwrite("replace_SOH_el", &H2::replace_SOH_el)
    .def_readwrite("replace_SOH_fc", &H2::replace_SOH_fc)
    .def_readwrite("power_degradation_flag", &H2::power_degradation_flag)
    .def_readwrite("el_replacements", &H2::el_replacements)
    .def_readwrite("fc_replacements", &H2::fc_replacements)

    .def_readwrite("el_capacity_kW", &H2::el_capacity_kW)
    .def_readwrite("el_quantity", &H2::el_quantity)
    .def_readwrite("el_spec_consumption_kWh", &H2::el_spec_consumption_kWh)
    .def_readwrite("el_spec_consumption_kWh_initial", &H2::el_spec_consumption_kWh_initial)
    .def_readwrite("el_output_kg", &H2::el_output_kg)
    .def_readwrite("fc_capacity_kW", &H2::fc_capacity_kW)
    .def_readwrite("fc_quantity", &H2::fc_quantity)
    .def_readwrite("fc_output_kW", &H2::fc_output_kW)
    .def_readwrite("fc_consumption_kg", &H2::fc_consumption_kg)
    .def_readwrite("fc_spec_consumption_kg", &H2::fc_spec_consumption_kg)
    .def_readwrite("fc_spec_consumption_kg_initial", &H2::fc_spec_consumption_kg_initial)
    .def_readwrite("h2_tank_capacity_kg", &H2::h2_tank_capacity_kg)
    .def_readwrite("h2_tank_cost_kg", &H2::h2_tank_cost_kg)
    .def_readwrite("tank_level_kg", &H2::tank_level_kg)
    .def_readwrite("compressor_spec_consumption_kWh", &H2::compressor_spec_consumption_kWh)
    .def_readwrite("n_compressor", &H2::n_compressor) 
    .def_readwrite("compression_included", &H2::compression_included)
    .def_readwrite("compressor_cap_cost_kW", &H2::compressor_cap_cost_kW)
    .def_readwrite("water_treatment_included", &H2::water_treatment_included)
    .def_readwrite("external_hydrogen_load_included", &H2::external_hydrogen_load_included)   
    .def_readwrite("excess_hydrogen_potential_included", &H2::excess_hydrogen_potential_included)
    .def_readwrite("water_treatment_cap_cost", &H2::water_treatment_cap_cost)
    .def_readwrite("water_demand_L", &H2::water_demand_L)

    .def_readwrite("total_water_demand_kg", &H2::total_water_demand_kg)
    .def_readwrite("total_h2_produced_kg", &H2::total_h2_produced_kg)
    .def_readwrite("total_el_consumption_kWh", &H2::total_el_consumption_kWh)
    .def_readwrite("total_curtailed_hydrogen_potential", &H2::total_curtailed_hydrogen_potential)

    .def_readwrite("fc_runtime_hrs", &H2::fc_runtime_hrs)
    .def_readwrite("fc_runtime_current_hrs", &H2::fc_runtime_current_hrs)
    .def_readwrite("fc_runtime_hrs_total", &H2::fc_runtime_hrs_total)
    .def_readwrite("fc_min_runtime", &H2::fc_min_runtime)
    .def_readwrite("fc_is_running", &H2::fc_is_running)
    .def_readwrite("fc_min_runtime_enforced", &H2::fc_min_runtime_enforced)
    .def_readwrite("fc_runtime_enforced_total", &H2::fc_runtime_enforced_total)

    .def_readwrite("making_hydrogen_for_external_load", &H2::making_hydrogen_for_external_load)   

    .def_readwrite("el_runtime_hrs", &H2::el_runtime_hrs)
    .def_readwrite("el_runtime_current_hrs", &H2::el_runtime_current_hrs)
    .def_readwrite("el_runtime_hrs_total", &H2::el_runtime_hrs_total)
    .def_readwrite("el_min_runtime", &H2::el_min_runtime)
    .def_readwrite("el_is_running", &H2::el_is_running)
    .def_readwrite("el_min_runtime_enforced", &H2::el_min_runtime_enforced)
    .def_readwrite("el_runtime_enforced_total", &H2::el_runtime_enforced_total)

    .def_readwrite("init_SOC", &H2::init_SOC)
    .def_readwrite("min_SOC", &H2::min_SOC)
    .def_readwrite("hysteresis_SOC", &H2::hysteresis_SOC)
    .def_readwrite("max_SOC", &H2::max_SOC)

    .def_readwrite("n_fuelcell", &H2::n_fuelcell)
    .def_readwrite("n_electrolyzer", &H2::n_electrolyzer)    
    .def_readwrite("n_h2_overall", &H2::n_h2_overall)    
    .def_readwrite("kWh_kg_conversion", &H2::kWh_kg_conversion)
    .def_readwrite("fc_min_load_ratio", &H2::fc_min_load_ratio)
    .def_readwrite("el_min_load_ratio", &H2::el_min_load_ratio)

    .def_readwrite("fc_ramp_loss", &H2::fc_ramp_loss)
    .def_readwrite("el_ramp_loss", &H2::el_ramp_loss)

    .def_readwrite("cp_air", &H2::cp_air)
    .def_readwrite("cp_el", &H2::cp_el)
    .def_readwrite("cp_fc", &H2::cp_fc)
    .def_readwrite("p_el", &H2::p_el)
    .def_readwrite("p_fc", &H2::p_fc)
    .def_readwrite("p_air", &H2::p_air)
    .def_readwrite("v_housing", &H2::v_housing)

    .def_readwrite("SOH_el_vec", &H2::SOH_el_vec)
    .def_readwrite("SOH_fc_vec", &H2::SOH_fc_vec)
    .def_readwrite("tank_level_vec_kg", &H2::tank_level_vec_kg)
    .def_readwrite("hydrogen_load_vec_kg", &H2::hydrogen_load_vec_kg)  
    .def_readwrite("curtailed_hydrogen_vec_kg", &H2::curtailed_hydrogen_vec_kg)   
    .def_readwrite("compression_power_vec_kW", &H2::compression_power_vec_kW)  
    .def_readwrite("water_demand_vec_kg", &H2::water_demand_vec_kg)  
    .def_readwrite("n_fuelcell_vec", &H2::n_fuelcell_vec)  
    .def_readwrite("n_electrolyzer_vec", &H2::n_electrolyzer_vec)  
    .def_readwrite("compression_Q_vec_kW", &H2::compression_Q_vec_kW)  

    .def(pybind11::init<>())
    .def(pybind11::init<int, double, H2Inputs>())
    
    .def("handleReplacement_el", &H2::handleReplacement_el)
    .def("handleReplacement_fc", &H2::handleReplacement_fc)

    .def("EL_minruntime", &H2::EL_minruntime)
    .def("FC_minruntime", &H2::FC_minruntime)

    .def("getAvailablekW", &H2::getAvailablekW)
    .def("getAcceptablekW", &H2::getAcceptablekW)
    .def("getMinELCapacitykW", &H2::getMinELCapacitykW)
    .def("getMinFCCapacitykW", &H2::getMinFCCapacitykW)
    .def("commitElectrolysis", &H2::commitElectrolysis)
    .def("commitFuelCell", &H2::commitFuelCell)
    .def("getThermalOutput", &H2::getThermalOutput)
    .def("getMcp", &H2::getMcp)

    .def("getExternalHydrogenLoadkg", &H2::getExternalHydrogenLoadkg)
    .def("commitExternalHydrogenLoadkg", &H2::commitExternalHydrogenLoadkg)
    .def("commitCurtailmentHydrogen", &H2::commitCurtailmentHydrogen)   
    .def("getCompressorPowerkW", &H2::getCompressorPowerkW)
    .def("getWaterDemand", &H2::getWaterDemand);


    