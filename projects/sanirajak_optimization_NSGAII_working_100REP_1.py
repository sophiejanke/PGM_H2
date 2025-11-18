"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    Copyright 2024 (C)

    Sophie Janke
    email: sophiejanke@uvic.ca

    Original script Authour: Anthony Truelove
    github: gears1763-2
"""


import os
import numpy as np
import sys
import time

from pymoo.visualization.pcp import PCP
import matplotlib.pyplot as plt

from pymoo.algorithms.moo.nsga2 import NSGA2
from pymoo.core.problem import ElementwiseProblem
from pymoo.optimize import minimize
from pymoo.operators.sampling.lhs import LatinHypercubeSampling
from pymoo.operators.crossover.sbx import SimulatedBinaryCrossover
from pymoo.operators.mutation.pm import PolynomialMutation


sys.path.insert(0, "../pybindings/precompiled_bindings/")
import PGMcpp

LOAD = "../data/sanirajak/electrical_load/sanirajak_load_scaled2025_csv.csv" 
candidate_count = 1

class OptimizationSizingProblem(ElementwiseProblem):
    def __init__(self):
        n_var = 6
        n_obj = 2
        n_constr = 0
        xl = np.array([0, 0, 0, 0, 0, 0])
        xu = np.array([1500, 5000, 1500, 2000, 10000, 50000])

        super().__init__(
            n_var=n_var,
            n_obj=n_obj,
            n_constr=n_constr,
            xl=xl,
            xu=xu,
            elementwise_evaluation=True
        )

    def _evaluate(self, x, out, *args, **kwargs):
        global candidate_count

        model_inputs = PGMcpp.ModelInputs()
        model_inputs.path_2_electrical_load_time_series = LOAD
        model_inputs.control_mode = PGMcpp.ControlMode.LOAD_FOLLOWING
        model_inputs.firm_dispatch_ratio = 0.0
        model_inputs.load_reserve_ratio = 0.1
        model = PGMcpp.Model(model_inputs)
        model.reset()

        # Unpack variables
        el_capacity_kW, h2_tank_capacity_kg, fc_capacity_kW, \
        solar_capacity_kW, wind_capacity_kW, \
        liion_energy_capacity_kWh = x

        if solar_capacity_kW > 0:
            solar_inputs = PGMcpp.SolarInputs()
            solar_inputs.renewable_inputs.production_inputs.capacity_kW = solar_capacity_kW
            solar_inputs.capital_cost = solar_capacity_kW * 3617.35
            solar_inputs.firmness_factor = 1.0
            solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
                "../data/sanirajak/normalized_production/normalized_solar_production.csv"
            )
            model.addSolar(solar_inputs)

        if wind_capacity_kW > 0:
            wind_inputs = PGMcpp.WindInputs()
            wind_inputs.renewable_inputs.production_inputs.capacity_kW = wind_capacity_kW
            wind_inputs.capital_cost = wind_capacity_kW * 10000
            wind_inputs.firmness_factor = 1.0
            wind_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
                "../data/sanirajak/normalized_production/normalized_wind_production.csv"
            )
            model.addWind(wind_inputs)

        if liion_energy_capacity_kWh > 0:
            liion_inputs = PGMcpp.LiIonInputs()
            liion_inputs.storage_inputs.energy_capacity_kWh = liion_energy_capacity_kWh
            liion_inputs.storage_inputs.power_capacity_kW = liion_energy_capacity_kWh / 2
            liion_inputs.capital_cost = 2025 * liion_energy_capacity_kWh
            model.addLiIon(liion_inputs)

        if el_capacity_kW > 0 and h2_tank_capacity_kg > 0 and fc_capacity_kW > 0:
            h2_inputs = PGMcpp.H2Inputs()
            h2_inputs.el_capacity_kW = el_capacity_kW
            h2_inputs.h2_tank_capacity_kg = h2_tank_capacity_kg
            h2_inputs.fc_capacity_kW = fc_capacity_kW
            # h2_inputs.fc_min_load_ratio = 0.0
            # h2_inputs.el_min_load_ratio = 0.0
            h2_inputs.fuelcell_inputs.fc_capital_cost_per_kW = 3400 
            h2_inputs.electrolyzer_inputs.el_capital_cost_per_kW = 1400 
            h2_inputs.h2_tank_cost_kg = 1500 
            h2_inputs.init_SOC = 0.0
            h2_inputs.storage_inputs.nominal_discount_annual = 0.08
            h2_inputs.storage_inputs.nominal_inflation_annual = 0.02
            h2_inputs.compression_included = False
            h2_inputs.water_treatment_included = True
            model.addH2(h2_inputs)

        model.run()

        net_present_cost = model.net_present_cost

        total_missed_load_kWh = np.dot(
            model.controller.missed_load_vec_kW,
            model.electrical_load.dt_vec_hrs
        )

        M_CAD = 1e12
        missed_load_penalty = M_CAD * total_missed_load_kWh 

        f1 = net_present_cost + missed_load_penalty
        f2 = model.total_missed_load_kWh if hasattr(model, 'total_missed_load_kWh') else 0

        print(f"Candidate {candidate_count} => NPC:", f1, end='\r', flush=True)

        candidate_count += 1

        out["F"] = [f1, f2]

# Run NSGA-II
if __name__ == "__main__":

    # Set up the problem
    problem = OptimizationSizingProblem()

    algorithm = NSGA2(
        pop_size=500,
        sampling=LatinHypercubeSampling(),
        crossover=SimulatedBinaryCrossover(prob=0.9, eta=10),
        mutation=PolynomialMutation(prob=1.0 / problem.n_var, eta=25),
        eliminate_duplicates=True
    )

    # Terminate after 10 generations
    termination = ("n_gen", 400)
    t0 = time.time() 

    # Run the optimization (verbose=False suppresses the generation table)
    res = minimize(
        problem,
        algorithm,
        termination,
        # seed=42,
        save_history=True,
        verbose=False
    )

    best_index = np.argmin(res.F[:, 0])
    best_solution = res.X[best_index]

    # Choose the index of the best solution by lowest NPC (F[:, 0])
    best_index = np.argmin(res.F[:, 0])
    best_x = res.X[best_index]
    best_f = res.F[best_index]

    # Re-run the model for the best solution
    x = best_x

    model_inputs = PGMcpp.ModelInputs()
    model_inputs.path_2_electrical_load_time_series = LOAD
    model_inputs.control_mode = PGMcpp.ControlMode.LOAD_FOLLOWING
    model_inputs.firm_dispatch_ratio = 0.0
    model_inputs.load_reserve_ratio = 0.1

    model = PGMcpp.Model(model_inputs)
    model.reset()

    # Unpack variables
    el_capacity_kW, h2_tank_capacity_kg, fc_capacity_kW, \
    solar_capacity_kW, wind_capacity_kW, liion_energy_capacity_kWh = x

    # Add renewables and storage
    if solar_capacity_kW > 0:
        solar_inputs = PGMcpp.SolarInputs()
        solar_inputs.renewable_inputs.production_inputs.capacity_kW = solar_capacity_kW
        solar_inputs.capital_cost = solar_capacity_kW * 3617.35
        solar_inputs.firmness_factor = 1.0
        solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
            "../data/sanirajak/normalized_production/normalized_solar_production.csv")
        model.addSolar(solar_inputs)

    if wind_capacity_kW > 0:
        wind_inputs = PGMcpp.WindInputs()
        wind_inputs.renewable_inputs.production_inputs.capacity_kW = wind_capacity_kW
        wind_inputs.capital_cost = wind_capacity_kW * 10000
        wind_inputs.firmness_factor = 1.0
        wind_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
            "../data/sanirajak/normalized_production/normalized_wind_production.csv")
        model.addWind(wind_inputs)

    if liion_energy_capacity_kWh > 0:
        liion_inputs = PGMcpp.LiIonInputs()
        liion_inputs.storage_inputs.energy_capacity_kWh = liion_energy_capacity_kWh
        liion_inputs.storage_inputs.power_capacity_kW = liion_energy_capacity_kWh / 2
        liion_inputs.capital_cost = 2025 * liion_energy_capacity_kWh
        model.addLiIon(liion_inputs)

    if el_capacity_kW > 0 and h2_tank_capacity_kg > 0 and fc_capacity_kW > 0:
        h2_inputs = PGMcpp.H2Inputs()
        h2_inputs.el_capacity_kW = el_capacity_kW
        h2_inputs.h2_tank_capacity_kg = h2_tank_capacity_kg
        h2_inputs.fc_capacity_kW = fc_capacity_kW
        # h2_inputs.fc_min_load_ratio = 0.0
        # h2_inputs.el_min_load_ratio = 0.0
        h2_inputs.fuelcell_inputs.fc_capital_cost_per_kW = 3400
        h2_inputs.electrolyzer_inputs.el_capital_cost_per_kW = 1400 
        h2_inputs.h2_tank_cost_kg = 1500 
        h2_inputs.init_SOC = 0.0
        h2_inputs.storage_inputs.nominal_discount_annual = 0.08
        h2_inputs.storage_inputs.nominal_inflation_annual = 0.02
        h2_inputs.compression_included = False
        h2_inputs.water_treatment_included = True
        model.addH2(h2_inputs)

    model.run()
    lcoe = model.levellized_cost_of_energy_kWh
    total_missed_load_kWh = np.dot(
        model.controller.missed_load_vec_kW,
        model.electrical_load.dt_vec_hrs
    )

    print(128 * " ")
    print(f"\nOptimization completed in {round(time.time() - t0)} seconds\n")

    print(f"Net Present Cost: {round(best_f[0], 2)}")
    print(f"Total Fuel Consumed: {round(best_f[1], 2)}")
    total_missed_load_kWh
    print("Missed Load: ", total_missed_load_kWh)
    print("LCOE: ", lcoe)
    print("Optimal sizing:\n")
    
    # print("\tDiesel:", round(diesel_capacity_kW, 3), " kW")
    print("\tElectrolyzer:", round(el_capacity_kW, 2), " kW")
    print("\tH2 Storage:", round(h2_tank_capacity_kg, 2), " kg")
    print("\tFuel Cell:", round(fc_capacity_kW, 2), " kW")
    print("\tSolar:", round(solar_capacity_kW, 2), " kW")
    print("\tWind:", round(wind_capacity_kW, 2), " kW")
    print(
        "\tLiIon:",
        round(liion_energy_capacity_kWh, 2),
        "  kWh")
    
    print()
    
    print("Convergence Plot...\n")

    # Track best objective value (net present cost)
    best_f1 = [gen.opt.get("F")[:, 0].min() for gen in res.history]
    print("best_f1\n")
    plt.figure(figsize=(8, 5))
    plt.plot(best_f1, marker='o')
    plt.title("Convergence Plot (Best Net Present Cost per Generation)")
    plt.xlabel("Generation")
    plt.ylabel("Best Objective Value (NPC + Penalty)")
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    model.writeResults("Sanirajak_100REP_no_compression")