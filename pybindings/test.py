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
    A suite of tests for the Python 3 bindings for PGMcpp.
    
    Just some simple spot checks. The tests in test/ are much more thorough.
"""


import os
import sys
sys.path.insert(0, "precompiled_bindings/")  # <-- just need to point to a directory with the PGMcpp bindings

os.system("color")
end = "\n"

import PGMcpp


FLOAT_TOLERANCE = 1e-6


print()
print()
print("## ================ Testing Python Bindings ================ ##")
print()


if __name__ == "__main__":
    time_list_hrs = [i for i in range(0, 8760)]
    
    try:
        ## ================ test Interpolator ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Interpolator " +
            29 * "." +
            " " +
            "\033[0m"
        )
        
        test_interpolator = PGMcpp.Interpolator();
        
        data_key_1D = 1
        path_2_data_1D = "../data/test/interpolation/diesel_fuel_curve.csv"
        
        data_key_2D = 2
        path_2_data_2D = (
            "../data/test/interpolation/" +
            "wave_energy_converter_normalized_performance_matrix.csv"
        )
        
        test_interpolator.addData1D(1, path_2_data_1D)
        test_interpolator.addData2D(2, path_2_data_2D)
        
        assert (test_interpolator.path_map_1D[data_key_1D] == path_2_data_1D)
        assert (test_interpolator.path_map_2D[data_key_2D] == path_2_data_2D)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Production ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production " +
            31 * "." +
            " " +
            "\033[0m"
        )
        
        test_production_inputs = PGMcpp.ProductionInputs()
        
        test_production = PGMcpp.Production(
            8760,
            1,
            test_production_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Combustion ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Combustion " +
            16 * "." +
            " " +
            "\033[0m"
        )
        
        test_combustion_inputs = PGMcpp.CombustionInputs()
        
        test_combustion = PGMcpp.Combustion(
            8760,
            1,
            test_combustion_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Diesel ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Combustion <-- Diesel " +
            5 * "." +
            " " +
            "\033[0m"
        )
        
        test_diesel_inputs = PGMcpp.DieselInputs()
        test_diesel_inputs.combustion_inputs.production_inputs.is_sunk = True
        
        test_diesel = PGMcpp.Diesel(
            8760,
            1,
            test_diesel_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Noncombustion ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Noncombustion " +
            13 * "." +
            " " +
            "\033[0m"
        )
        
        test_noncombustion_inputs = PGMcpp.NoncombustionInputs()
        
        test_noncombustion = PGMcpp.Noncombustion(
            8760,
            1,
            test_noncombustion_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Hydro ================ ##
        
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Noncombustion <-- Hydro " +
            3 * "." +
            " " +
            "\033[0m"
        )
        
        test_hydro_inputs = PGMcpp.HydroInputs()
        test_hydro_inputs.noncombustion_inputs.production_inputs.is_sunk= True
        
        test_hydro = PGMcpp.Hydro(
            8760,
            1,
            test_hydro_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        
        ## ================ test Renewable ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable " +
            17 * "." +
            " " +
            "\033[0m"
        )
        
        test_renewable_inputs = PGMcpp.RenewableInputs()
        
        test_renewable = PGMcpp.Renewable(
            8760,
            1,
            test_renewable_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Solar ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Solar " +
            7 * "." +
            " " +
            "\033[0m"
        )
        
        test_solar_inputs = PGMcpp.SolarInputs()
        
        test_solar_inputs.firmness_factor = 1
        
        test_solar = PGMcpp.Solar(
            8760,
            1,
            test_solar_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Tidal ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Tidal " +
            7 * "." +
            " " +
            "\033[0m"
        )
        
        test_tidal_inputs = PGMcpp.TidalInputs()
        
        test_tidal = PGMcpp.Tidal(
            8760,
            1,
            test_tidal_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Wave ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Wave " +
            8 * "." +
            " " +
            "\033[0m"
        )
        
        test_wave_inputs = PGMcpp.WaveInputs()
        
        test_wave = PGMcpp.Wave(
            8760,
            1,
            test_wave_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Wind ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Wind " +
            8 * "." +
            " " +
            "\033[0m"
        )
        
        test_wind_inputs = PGMcpp.WindInputs()
        
        test_wind = PGMcpp.Wind(
            8760,
            1,
            test_wind_inputs,
            time_list_hrs
        )
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Storage ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Storage " +
            34 * "." +
            " " +
            "\033[0m"
        )
        
        test_storage_inputs = PGMcpp.StorageInputs()
        
        test_storage = PGMcpp.Storage(8760, 1, test_storage_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test LiIon ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Storage <-- LiIon " +
            24 * "." +
            " " +
            "\033[0m"
        )
        
        test_liion_inputs = PGMcpp.LiIonInputs()
        
        test_liion = PGMcpp.LiIon(8760, 1, test_liion_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Controller ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Controller " +
            31 * "." +
            " " +
            "\033[0m"
        )
        
        test_controller = PGMcpp.Controller()
        
        test_controller.setControlMode(PGMcpp.ControlMode.LOAD_FOLLOWING);
        
        assert (test_controller.control_mode == PGMcpp.ControlMode.LOAD_FOLLOWING)
        assert (test_controller.control_string == "LOAD_FOLLOWING")
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test ElectricalLoad ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing ElectricalLoad " +
            27 * "." +
            " " +
            "\033[0m"
        )
        
        path_2_electrical_load_time_series = (
            "../data/test/electrical_load/" +
            "electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
        )
        
        test_electrical_load = PGMcpp.ElectricalLoad(path_2_electrical_load_time_series)
        
        assert (
            test_electrical_load.path_2_electrical_load_time_series ==
            path_2_electrical_load_time_series
        )
        
        assert (
            abs(test_electrical_load.min_load_kW - 82.1211213927802) <=
            FLOAT_TOLERANCE
        )
        
        assert (
            abs(test_electrical_load.mean_load_kW - 258.373472633202) <=
            FLOAT_TOLERANCE
        )
        
        assert (abs(test_electrical_load.max_load_kW - 500) <= FLOAT_TOLERANCE)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Resources ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Resources " +
            32 * "." +
            " " +
            "\033[0m"
        )
        
        test_resources = PGMcpp.Resources();
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Model ================ ##
        ## ====== essentially repeats example.py ====== ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Model " +
            36 * "." +
            " " +
            "\033[0m"
        )
        
        #   construct Model
        path_2_electrical_load_time_series = (
            "../data/test/electrical_load/" +
            "electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
        )

        test_model_inputs = PGMcpp.ModelInputs()
        
        test_model_inputs.firm_dispatch_ratio = 0.1

        test_model_inputs.path_2_electrical_load_time_series = (
            path_2_electrical_load_time_series
        )
        test_model_inputs.control_mode = PGMcpp.ControlMode.LOAD_FOLLOWING

        test_model = PGMcpp.Model(test_model_inputs)

        assert (
            test_model.controller.control_mode == PGMcpp.ControlMode.LOAD_FOLLOWING
        )
        
        assert (test_model.controller.control_string == "LOAD_FOLLOWING")
        
        assert (test_model.controller.firm_dispatch_ratio == 0.1)
        
        #   add Diesel assets
        test_diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 200
        
        for i in range(0, 3):
            if i == 1:
                test_diesel_inputs.combustion_inputs.cycle_charging_setpoint = 1
            else:
                test_diesel_inputs.combustion_inputs.cycle_charging_setpoint = 0.85
            
            test_model.addDiesel(test_diesel_inputs)
        
        #   add renewable resources
        solar_resource_key = 0
        path_2_solar_resource_data = (
            "../data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv"
        )

        test_model.addResource(
            PGMcpp.RenewableType.SOLAR,
            path_2_solar_resource_data,
            solar_resource_key
        )
        
        tidal_resource_key = 1
        path_2_tidal_resource_data = (
            "../data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv"
        )

        test_model.addResource(
            PGMcpp.RenewableType.TIDAL,
            path_2_tidal_resource_data,
            tidal_resource_key
        )
        
        wave_resource_key = 2
        path_2_wave_resource_data = (
            "../data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv"
        )

        test_model.addResource(
            PGMcpp.RenewableType.WAVE,
            path_2_wave_resource_data,
            wave_resource_key
        )
        
        wind_resource_key = 3
        path_2_wind_resource_data = (
            "../data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv"
        )

        test_model.addResource(
            PGMcpp.RenewableType.WIND,
            path_2_wind_resource_data,
            wind_resource_key
        )
        
        hydro_resource_key = 4
        path_2_hydro_resource_data = (
            "../data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv"
        )

        test_model.addResource(
            PGMcpp.NoncombustionType.HYDRO,
            path_2_hydro_resource_data,
            hydro_resource_key
        )
        
        #   add Hydro asset
        hydro_inputs = PGMcpp.HydroInputs()
        hydro_inputs.noncombustion_inputs.production_inputs.capacity_kW = 300
        hydro_inputs.reservoir_capacity_m3 = 10000
        hydro_inputs.init_reservoir_state = 0.5
        hydro_inputs.noncombustion_inputs.production_inputs.is_sunk = True
        hydro_inputs.resource_key = hydro_resource_key

        test_model.addHydro(hydro_inputs);
        
        #   add Renewable assets
        solar_inputs = PGMcpp.SolarInputs()

        solar_inputs.renewable_inputs.production_inputs.capacity_kW = 250
        solar_inputs.resource_key = solar_resource_key

        test_model.addSolar(solar_inputs)
        
        solar_inputs.renewable_inputs.production_inputs.capacity_kW = 100
        solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = (
            "../data/test/normalized_production/normalized_solar_production.csv"
        )
        
        test_model.addSolar(solar_inputs)
        
        tidal_inputs = PGMcpp.TidalInputs()

        tidal_inputs.renewable_inputs.production_inputs.capacity_kW = 120
        tidal_inputs.design_speed_ms = 2.5
        tidal_inputs.resource_key = tidal_resource_key

        test_model.addTidal(tidal_inputs)
        
        wind_inputs = PGMcpp.WindInputs()

        wind_inputs.renewable_inputs.production_inputs.capacity_kW = 150
        wind_inputs.resource_key = wind_resource_key

        test_model.addWind(wind_inputs)
        
        wave_inputs = PGMcpp.WaveInputs()

        wave_inputs.renewable_inputs.production_inputs.capacity_kW = 100
        wave_inputs.resource_key = wave_resource_key

        test_model.addWave(wave_inputs)
        
        #   add LiIon asset
        liion_inputs = PGMcpp.LiIonInputs()

        liion_inputs.storage_inputs.power_capacity_kW = 500
        liion_inputs.storage_inputs.energy_capacity_kWh = 1050
        liion_inputs.power_degradation_flag = True

        test_model.addLiIon(liion_inputs)
        
        #   run and write
        test_model.run()
        test_model.writeResults("test_results/")
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    except:
        sys.stdout.write("\x1B[31mFAIL\x1b[0m" + end)
        raise


    finally:
        print()
