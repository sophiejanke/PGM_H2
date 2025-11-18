"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    Copyright 2023 (C)

    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    CONTINUED USE OF THIS SOFTWARE CONSTITUTES ACCEPTANCE OF THESE TERMS.
"""


"""
    An example PGMcpp project, accessed using the Python bindings for PGMcpp.
    See comments below for guidance.
"""


import os
import sys
import numpy as np
sys.path.insert(0, "../pybindings/precompiled_bindings/")  # <-- just need to point to a directory with the PGMcpp bindings

import PGMcpp


"""
    1. construct Model object
    
"""

path_2_electrical_load_time_series = (
    "../data/chesterfield/electrical_load/chesterfield_load_scaled2025_csv.csv"
)

model_inputs = PGMcpp.ModelInputs()

model_inputs.path_2_electrical_load_time_series = path_2_electrical_load_time_series
model_inputs.control_mode = PGMcpp.ControlMode.LOAD_FOLLOWING
model_inputs.firm_dispatch_ratio = 0.0
model_inputs.load_reserve_ratio = 0.1

model = PGMcpp.Model(model_inputs)

"""
    2. add Diesel objects to Model
    
"""

diesel_inputs = PGMcpp.DieselInputs()

#   2.1. add G1 and G2 - Detroit S60-1 and Detroit S60-2
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 320
diesel_inputs.capital_cost = 320*5334
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
diesel_inputs.fuel_cost_L = 1.498
diesel_inputs.replace_running_hrs = 50000
diesel_inputs.minimum_runtime_hrs = 1

model.addDiesel(diesel_inputs)
model.addDiesel(diesel_inputs)


#   2.2. add G3 - Volvo D16-1 
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 400
diesel_inputs.capital_cost = 400*5334
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
diesel_inputs.fuel_cost_L = 1.498
diesel_inputs.replace_running_hrs = 50000
diesel_inputs.minimum_runtime_hrs = 1

model.addDiesel(diesel_inputs)

#   5.1. add 1 x 250 kW solar PV array
solar_inputs = PGMcpp.SolarInputs()

solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
"../data/chesterfield/normalized_production/normalized_solar_production.csv")
solar_inputs.renewable_inputs.production_inputs.capacity_kW = 937.5
solar_inputs.renewable_inputs.production_inputs.nominal_inflation_annual = 0.03
solar_inputs.renewable_inputs.production_inputs.nominal_discount_annual = 0.06
solar_inputs.renewable_inputs.production_inputs.replace_running_hrs = 200000
solar_inputs.capital_cost = 8709.96*937.5
solar_inputs.firmness_factor = 1.0

model.addSolar(solar_inputs)


# #   5.3. add 1 x 1000 kW wind turbine
wind_inputs = PGMcpp.WindInputs()

wind_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
"../data/chesterfield/normalized_production/normalized_wind_production_1MW.csv")
wind_inputs.renewable_inputs.production_inputs.capacity_kW = 1000
wind_inputs.renewable_inputs.production_inputs.nominal_inflation_annual = 0.03
wind_inputs.renewable_inputs.production_inputs.nominal_discount_annual = 0.06
wind_inputs.capital_cost = 15610*1000
wind_inputs.firmness_factor = 1.0
wind_inputs.renewable_inputs.production_inputs.replace_running_hrs = 200000

model.addWind(wind_inputs)

"""
    3. add Storage objects to Model
    
"""

#   6.1. add 1 x (900 kW, ) lithium ion battery energy storage system

liion_inputs = PGMcpp.LiIonInputs()

liion_inputs.storage_inputs.power_capacity_kW = 1000 # <--- C-rate = 0.5 ( 2 hour battery) 
liion_inputs.storage_inputs.energy_capacity_kWh = 4200  
liion_inputs.init_SOC = 1
liion_inputs.min_SOC = 0.2
liion_inputs.storage_inputs.nominal_inflation_annual = 0.03
liion_inputs.storage_inputs.nominal_discount_annual = 0.06
liion_inputs.charging_efficiency = 0.95
liion_inputs.discharging_efficiency = 0.95
liion_inputs.max_SOC = 1.0
liion_inputs.capital_cost = 1262.03*4200 
liion_inputs.operation_maintenance_cost_kWh = 0 

model.addLiIon(liion_inputs)

#   6.2. add a regenerative green hydrogen system
h2_inputs = PGMcpp.H2Inputs()

h2_inputs.h2_tank_capacity_kg = 543.61
h2_inputs.el_capacity_kW = 242.54
h2_inputs.fc_capacity_kW = 219.85
h2_inputs.fuelcell_inputs.fc_capital_cost_per_kW = 2000
h2_inputs.electrolyzer_inputs.el_capital_cost_per_kW = 1600
h2_inputs.h2_tank_cost_kg = 1200
h2_inputs.storage_inputs.nominal_discount_annual = 0.06
h2_inputs.storage_inputs.nominal_inflation_annual = 0.03
h2_inputs.compression_included = True
h2_inputs.water_treatment_included = True
h2_inputs.init_SOC = 0.5

model.addH2(h2_inputs)

"""
    4. add thermal modelling
"""

thermal_model_inputs = PGMcpp.ThermalModelInputs()
thermal_model_inputs.path_2_environmental_temperature_time_series = (
"../data/sanirajak/environmental_temperature/sanirajak_environmental_temperature.csv")
model.addThermalModel(thermal_model_inputs)


"""
    5. run and write results
    
"""

model.run()

net_present_cost = model.net_present_cost
total_missed_load_kWh = np.dot(
    model.controller.missed_load_vec_kW,
    model.electrical_load.dt_vec_hrs
)
total_fuel_consumed_L = model.total_fuel_consumed_L

print("\tNet Present Cost:", round(net_present_cost), " CAD")
print("\tTotal Missed Load:", round(total_missed_load_kWh), " kWh")
print("\tTotal Fuel Consumed:", round(total_fuel_consumed_L), " L")

model.writeResults("chesterfield_s1_test")
