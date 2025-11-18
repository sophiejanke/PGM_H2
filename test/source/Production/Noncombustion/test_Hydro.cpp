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
/// \file test_Hydro.cpp
///
/// \brief Testing suite for Hydro class.
///
/// A suite of tests for the Hydro class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Resources.h"
#include "../../../../header/ElectricalLoad.h"
#include "../../../../header/Production/Noncombustion/Hydro.h"



// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro* Noncombustion* testConstruct_Hydro(
///         HydroInputs hydro_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief A function to construct a Hydro object and spot check some
///     post-construction attributes.
///
/// \return A Noncombustion pointer to a test Hydro object.
///

Noncombustion* testConstruct_Hydro(
    HydroInputs hydro_inputs,
    std::vector<double>* time_vec_hrs_ptr
)
{
    Noncombustion* test_hydro_ptr =  new Hydro(
        8760,
        1,
        hydro_inputs,
        time_vec_hrs_ptr
    );
    
    testTruth(
        not hydro_inputs.noncombustion_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_hydro_ptr->type,
        NoncombustionType :: HYDRO,
        __FILE__,
        __LINE__
    );

    testTruth(
        test_hydro_ptr->type_str == "HYDRO",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((Hydro*)test_hydro_ptr)->turbine_type,
        HydroTurbineType :: HYDRO_TURBINE_PELTON,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((Hydro*)test_hydro_ptr)->reservoir_capacity_m3,
        10000,
        __FILE__,
        __LINE__
    );

    return test_hydro_ptr;
}   /* testConstruct_Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEfficiencyInterpolation_Hydro(Noncombustion* test_hydro_ptr)
///
/// \brief Function to test that the generator and turbine efficiency maps are being
///     initialized as expected, and that efficiency interpolation is returning the
///     expected values.
///
/// \param test_hydro_ptr A Noncombustion pointer to the test Hydro object.
///

void testEfficiencyInterpolation_Hydro(Noncombustion* test_hydro_ptr)
{
    std::vector<double> expected_gen_power_ratios = {
        0,   0.1, 0.2,  0.3, 0.4, 0.5,
        0.6, 0.7, 0.75, 0.8, 0.9, 1
    };

    std::vector<double> expected_gen_efficiencies = {
        0.000, 0.800, 0.900, 0.913,
        0.925, 0.943, 0.947, 0.950,
        0.953, 0.954, 0.956, 0.958
    };

    double query = 0;
    for (size_t i = 0; i < expected_gen_power_ratios.size(); i++) {
        testFloatEquals(
            test_hydro_ptr->interpolator.interp_map_1D[
                HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY
            ].x_vec[i],
            expected_gen_power_ratios[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_hydro_ptr->interpolator.interp_map_1D[
                HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY
            ].y_vec[i],
            expected_gen_efficiencies[i],
            __FILE__,
            __LINE__
        );
        
        if (i < expected_gen_power_ratios.size() - 1) {
            query = expected_gen_power_ratios[i] + ((double)rand() / RAND_MAX) *
                (expected_gen_power_ratios[i + 1] - expected_gen_power_ratios[i]);
            
            test_hydro_ptr->interpolator.interp1D(
                HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY,
                query
            );
        }
    }

    std::vector<double> expected_turb_power_ratios = {
        0,   0.1, 0.2, 0.3, 0.4,
        0.5, 0.6, 0.7, 0.8, 0.9,
        1
    };

    std::vector<double> expected_turb_efficiencies = {
        0.000, 0.780, 0.855, 0.875, 0.890,
        0.900, 0.908, 0.913, 0.918, 0.908,
        0.880
    };

    for (size_t i = 0; i < expected_turb_power_ratios.size(); i++) {
        testFloatEquals(
            test_hydro_ptr->interpolator.interp_map_1D[
                HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY
            ].x_vec[i],
            expected_turb_power_ratios[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_hydro_ptr->interpolator.interp_map_1D[
                HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY
            ].y_vec[i],
            expected_turb_efficiencies[i],
            __FILE__,
            __LINE__
        );
        
        if (i < expected_turb_power_ratios.size() - 1) {
            query = expected_turb_power_ratios[i] + ((double)rand() / RAND_MAX) *
                (expected_turb_power_ratios[i + 1] - expected_turb_power_ratios[i]);
            
            test_hydro_ptr->interpolator.interp1D(
                HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY,
                query
            );
        }
    }

    return;
}   /* testEfficiencyInterpolation_Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommit_Hydro(Noncombustion* test_hydro_ptr)
///
/// \brief Function to test if the commit method is working as expected, by checking
///     some post-call attributes of the test Hydro object.
///
/// \param test_hydro_ptr A Noncombustion pointer to the test Hydro object.
///

void testCommit_Hydro(
    Noncombustion* test_hydro_ptr,
    Resources* test_resources_ptr
)
{
    double load_kW = 100 * (double)rand() / RAND_MAX;
    double production_kW = 0;

    for (int i = 0; i < 8760; i++) {
        production_kW = test_hydro_ptr->requestProductionkW(
            i,
            1,
            load_kW,
            test_resources_ptr->resource_map_1D[test_hydro_ptr->resource_key][i]
        );
        
        load_kW = test_hydro_ptr->commit(
            i,
            1,
            production_kW,
            load_kW,
            test_resources_ptr->resource_map_1D[test_hydro_ptr->resource_key][i]
        );
        
        testGreaterThanOrEqualTo(
            test_hydro_ptr->production_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testLessThanOrEqualTo(
            test_hydro_ptr->production_vec_kW[i],
            test_hydro_ptr->capacity_kW,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_hydro_ptr->production_vec_kW[i] -
            test_hydro_ptr->dispatch_vec_kW[i] -
            test_hydro_ptr->curtailment_vec_kW[i] -
            test_hydro_ptr->storage_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThanOrEqualTo(
            ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testLessThanOrEqualTo(
            ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
            ((Hydro*)test_hydro_ptr)->maximum_flow_m3hr,
            __FILE__,
            __LINE__
        );
        
        testGreaterThanOrEqualTo(
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testLessThanOrEqualTo(
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i],
            ((Hydro*)test_hydro_ptr)->reservoir_capacity_m3,
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testCommit_Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Noncombustion <-- Hydro");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    std::string path_2_electrical_load_time_series = 
        "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

    ElectricalLoad* test_electrical_load_ptr =
        new ElectricalLoad(path_2_electrical_load_time_series);

    Resources* test_resources_ptr = new Resources();
    
    HydroInputs hydro_inputs;
    int hydro_resource_key = 0;

    hydro_inputs.reservoir_capacity_m3 = 10000;
    hydro_inputs.resource_key = hydro_resource_key;
    
    Noncombustion* test_hydro_ptr = testConstruct_Hydro(hydro_inputs, &time_vec_hrs);
    
    std::string path_2_hydro_resource_data =
        "data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv";

    test_resources_ptr->addResource(
        NoncombustionType::HYDRO,
        path_2_hydro_resource_data,
        hydro_resource_key,
        test_electrical_load_ptr
    );
    
    
    try {
        testEfficiencyInterpolation_Hydro(test_hydro_ptr);
        testCommit_Hydro(test_hydro_ptr, test_resources_ptr);
    }


    catch (...) {
        delete test_electrical_load_ptr;
        delete test_resources_ptr;
        delete test_hydro_ptr;
        
        printGold(" ... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_electrical_load_ptr;
    delete test_resources_ptr;
    delete test_hydro_ptr;

    printGold(" ... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
