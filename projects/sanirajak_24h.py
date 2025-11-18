"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    Copyright 2023 (C)

    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2

"""


import os
import sys
sys.path.insert(0, "../pybindings/precompiled_bindings/")  # <-- just need to point to a directory with the PGMcpp bindings

import PGMcpp

"""
    1. construct Model object
    
"""

path_2_electrical_load_time_series = (
    "../data/sanirajak_24h/electrical_load/sanirajak_load_scaled2025_csv.csv"
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

#   2.1. add G1 - 165kW
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 165
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
diesel_inputs.fuel_cost_L = 1.02
diesel_inputs.replace_running_hrs = 50000
diesel_inputs.minimum_runtime_hrs = 0

model.addDiesel(diesel_inputs)

#   2.2. add G2 and G4
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 550
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
diesel_inputs.fuel_cost_L = 1.02
diesel_inputs.replace_running_hrs = 50000
diesel_inputs.minimum_runtime_hrs = 0

model.addDiesel(diesel_inputs)
model.addDiesel(diesel_inputs)

#   2.2. add G3
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 330
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
diesel_inputs.fuel_cost_L = 1.02
diesel_inputs.replace_running_hrs = 50000
diesel_inputs.minimum_runtime_hrs = 0

model.addDiesel(diesel_inputs)

"""
    3. add renewable resources to Model

"""

#   3.1. add solar resource time series
# solar_resource_key = 0
# path_2_solar_resource_data = (
#     "../data/sanirajak/resources/513.6kWHelioscopeGridPower.csv"
# )

# model.addResource(
#     PGMcpp.RenewableType.SOLAR,
#     path_2_solar_resource_data,
#     solar_resource_key
# )


"""
    4. add Hydro object to Model
    
"""

"""
    5. add Renewable objects to Model
"""
#   5.1. add 1 x 250 kW solar PV array
solar_inputs = PGMcpp.SolarInputs()

solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
"../data/sanirajak_24h/normalized_production/normalized_solar_production.csv")

solar_inputs.renewable_inputs.production_inputs.capacity_kW = 513.6
solar_inputs.renewable_inputs.production_inputs.is_sunk = True
solar_inputs.firmness_factor = 0.2

model.addSolar(solar_inputs)

#   5.3. add 1 x 150 kW wind turbine
wind_inputs = PGMcpp.WindInputs()

wind_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
"../data/sanirajak_24h/normalized_production/normalized_wind_production.csv")

wind_inputs.renewable_inputs.production_inputs.capacity_kW = 1005
wind_inputs.firmness_factor = 0.5
# wind_inputs.resource_key = wind_resource_key

model.addWind(wind_inputs)

"""
    6. add LiIon object to Model
    
"""

#   6.2. add 1 x (900 kW, ) lithium ion battery energy storage system
liion_inputs = PGMcpp.LiIonInputs()

liion_inputs.storage_inputs.power_capacity_kW = 450
liion_inputs.storage_inputs.energy_capacity_kWh = 900  #<-- about 4 hours of mean load autonomy
liion_inputs.init_SOC = 1.0

model.addLiIon(liion_inputs)

#   6.1. add a regenerative green hydrogen system
h2_inputs = PGMcpp.H2Inputs()

h2_inputs.h2_tank_capacity_kg = 200
h2_inputs.el_capacity_kW = 200
h2_inputs.fc_capacity_kW = 100
h2_inputs.init_SOC = 0.5
h2_inputs.water_treatment_included = True
h2_inputs.compression_included = True
h2_inputs.excess_hydrogen_potential_included = False
h2_inputs.external_hydrogen_load_included = False

model.addH2(h2_inputs)


"""
    7. add thermal modelling
"""

thermal_model_inputs = PGMcpp.ThermalModelInputs()
thermal_model_inputs.path_2_environmental_temperature_time_series = (
"../data/sanirajak_24h/environmental_temperature/sanirajak_environmental_temperature.csv")
model.addThermalModel(thermal_model_inputs)

"""
    8. run and write results
"""

model.run()
model.writeResults("sanirajak_24h")

