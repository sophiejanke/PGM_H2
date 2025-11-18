"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    Copyright 2024 (C)

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
    A minimal(ish) working example showing how PGMcpp can be used to facilitate
    microgrid design optimization.
"""



import os
import sys
sys.path.insert(0, "../pybindings/precompiled_bindings/")  # <-- just need to point to a directory with the PGMcpp bindings

import numpy as np
import scipy.optimize as spo
import time

import PGMcpp



candidate_count = 1

def objectiveFunction(
    sizing_array : np.array,
    model : PGMcpp.Model,
    resource_key_dict : dict,
    objective_weights : np.array,
    print_flag : bool = False
) -> float:
    """
    An objective function (to be minimized) which sets up a PGMcpp Model under the given 
    Diesel-Solar-Wind-LiIon sizing, runs the model, and then returns a scalar objective 
    which is given by
    
    weight[0] * net_present_cost + weight[1] * total_missed_load + weight[2] * total_fuel_consumed
    
    Parameters
    ----------
    sizing_array : np.array
        A numpy array containing the Diesel-Solar-Wind-LiIon sizing as follows: [0]: 
        Diesel capacity [kW], [1]: Solar capacity [kW], [2]: Wind capacity [kW], [3]: 
        LiIon power capacity [kW], [4]: LiIon energy capacity [kWh].
    
    model : PGMcpp.Model
        A PGMcpp Model object (with electrical load and resource data already loaded).
    
    resource_key_dict : dict
        A dictionary of resource keys (int) by resource type (str, "Solar", "Wind").
    
    objective_weights : np.array
        A numpy array of objective weights, to define the objective function. The first 
        weight scales the net present cost, the second weight scales the total missed 
        load, and the third weight scales the total fuel consumed.
    
    print_flag : bool
        A flag which indicates whether or not to print the unweighted objective
        components individually. Defaults to `False`.
    
    Returns
    -------
    float
        A scalar objective function which is the weighted sum of net present cost, total 
        missed load, and total fuel consumed.
    """
    
    #   1. reset model
    model.reset()
    
    
    #   2. unpack sizing_array
    diesel_capacity_kW = sizing_array[0]
    solar_capacity_kW = sizing_array[1]
    wind_capacity_kW = sizing_array[2]
    liion_power_capacity_kW = sizing_array[3]
    liion_energy_capacity_kWh = sizing_array[4]
    
    
    #   3. add assets to model
    
    #   3.1. Diesel
    if diesel_capacity_kW > 0:
        diesel_inputs = PGMcpp.DieselInputs()
        
        diesel_inputs.combustion_inputs.production_inputs.capacity_kW = diesel_capacity_kW
        
        model.addDiesel(diesel_inputs)
    
    #   3.2. Solar
    if solar_capacity_kW > 0:
        solar_inputs = PGMcpp.SolarInputs()

        solar_inputs.renewable_inputs.production_inputs.capacity_kW = solar_capacity_kW
        solar_inputs.resource_key = resource_key_dict["Solar"]

        model.addSolar(solar_inputs)
    
    #   3.3. Wind
    if wind_capacity_kW > 0:
        wind_inputs = PGMcpp.WindInputs()

        wind_inputs.renewable_inputs.production_inputs.capacity_kW = wind_capacity_kW
        wind_inputs.resource_key = resource_key_dict["Wind"]

        model.addWind(wind_inputs)
    
    #   3.4. LiIon
    if liion_power_capacity_kW > 0 and liion_energy_capacity_kWh > 0:
        liion_inputs = PGMcpp.LiIonInputs()
        
        liion_inputs.storage_inputs.power_capacity_kW = liion_power_capacity_kW
        liion_inputs.storage_inputs.energy_capacity_kWh = liion_energy_capacity_kWh

        model.addLiIon(liion_inputs)
    
    
    #   4. run model
    model.run()
    
    
    #   5. extract and return scalar objective 
    #      (net present cost with fuel consumption and missed load penalties)
    net_present_cost = model.net_present_cost
    
    total_missed_load_kWh = np.dot(
        model.controller.missed_load_vec_kW,
        model.electrical_load.dt_vec_hrs
    )
    
    total_fuel_consumed_L = model.total_fuel_consumed_L
    
    if print_flag:
        print("\tNet Present Cost:", round(net_present_cost), " CAD")
        print("\tTotal Missed Load:", round(total_missed_load_kWh), " kWh")
        print("\tTotal Fuel Consumed:", round(total_fuel_consumed_L), " L")
        print()
    
    global candidate_count
    
    print(
        "candidate:",
        candidate_count,
        ",  NPC:",
        round(net_present_cost),
        "CAD,  missed load:",
        round(total_missed_load_kWh),
        " kWh,  fuel:",
        round(total_fuel_consumed_L),
        " L" + 16 * " ",
        end="\r",
        flush=True
    )
    
    objective = (
        objective_weights[0] * net_present_cost +
        objective_weights[1] * total_missed_load_kWh +
        objective_weights[2] * total_fuel_consumed_L
    )
    
    candidate_count += 1
    
    return objective



if __name__ == "__main__":
    #   1. construct Model object
    path_2_electrical_load_time_series = (
        "../data/test/electrical_load/electrical_load_generic_peak-500kW_25yrs_dt-1hr.csv"
    )

    model_inputs = PGMcpp.ModelInputs()

    model_inputs.path_2_electrical_load_time_series = path_2_electrical_load_time_series
    model_inputs.control_mode = PGMcpp.ControlMode.LOAD_FOLLOWING

    model_inputs.firm_dispatch_ratio = 0.1
    model_inputs.load_reserve_ratio = 0.1

    model = PGMcpp.Model(model_inputs)
    
    
    #   2. add renewable resource data to Model object
    resource_key_dict = {}
    
    #   2.1. Solar
    path_2_solar_resource_data = (
        "../data/test/resources/solar_GHI_peak-1kWm2_25yrs_dt-1hr.csv"
    )
    
    resource_key_dict["Solar"] = 0
    
    model.addResource(
        PGMcpp.RenewableType.SOLAR,
        path_2_solar_resource_data,
        resource_key_dict["Solar"]
    )
    
    #   2.2. Wind
    path_2_wind_resource_data = (
        "../data/test/resources/wind_speed_peak-25ms_25yrs_dt-1hr.csv"
    )
    
    resource_key_dict["Wind"] = 1

    model.addResource(
        PGMcpp.RenewableType.WIND,
        path_2_wind_resource_data,
        resource_key_dict["Wind"]
    )
    
    
    #   3. optimize asset sizes to minimize net present cost
    #      (under missed load and fuel consumed penalties)
    print("Seeking optimal sizing ...\n")
    
    bounds_list = [
        (0, 600),   # diesel_capacity_kW
        (0, 1200),  # solar_capacity_kW
        (0, 1200),  # wind_capacity_kW
        (0, 600),   # liion_power_capacity_kW
        (0, 2400)   # liion_energy_capacity_kWh
    ]
    
    objective_weights = np.array(
        [
            1e0,    # net present cost [CAD]
            1e2,    # total missed load [kWh]  (penalty factor)
            1e1     # total fuel consumed [L]  (penalty factor)
        ]
    )
    
    ## ==== OPTIMIZATION ============================================================ ##
    start_time = time.time()
    
    res_opt = spo.dual_annealing(
        objectiveFunction,
        bounds_list,
        args=(
            model,
            resource_key_dict,
            objective_weights
        )
    )
    
    end_time = time.time()
    ## ==== END OPTIMIZATION ======================================================== ##
    
    print(128 * " ")
    print("DONE  (total run time: {} seconds)\n".format(round(end_time - start_time)))
    
    
    #   4. Unpack, print, and write optimal results
    results_array = res_opt.x
    
    diesel_capacity_kW = results_array[0]
    solar_capacity_kW = results_array[1]
    wind_capacity_kW = results_array[2]
    liion_power_capacity_kW = results_array[3]
    liion_energy_capacity_kWh = results_array[4]
    
    print("Optimal sizing:\n")
    
    print("\tDiesel:", round(diesel_capacity_kW, 3), " kW")
    print("\tSolar:", round(solar_capacity_kW, 3), " kW")
    print("\tWind:", round(wind_capacity_kW, 3), " kW")
    print(
        "\tLiIon:",
        round(liion_power_capacity_kW, 3),
        "kW,  ",
        round(liion_energy_capacity_kWh, 3),
        "  kWh"
    )
    
    print()
    
    objectiveFunction(
        results_array,
        model,
        resource_key_dict,
        objective_weights,
        print_flag=True
    )
    
    print()
    
    model.writeResults("optimization_results")
