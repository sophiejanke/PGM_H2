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
/// \file test_Solar.cpp
///
/// \brief Testing suite for Solar class.
///
/// A suite of tests for the Solar class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Solar.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable* testConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Solar object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A Renewable pointer to a test Solar object.
///

Renewable* testConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
{
    SolarInputs solar_inputs;

    Renewable* test_solar_ptr = new Solar(
        8760,
        1,
        solar_inputs,
        time_vec_hrs_ptr
    );
    
    testTruth(
        not solar_inputs.renewable_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_solar_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->type,
        RenewableType :: SOLAR,
        __FILE__,
        __LINE__
    );

    testTruth(
        test_solar_ptr->type_str == "SOLAR",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->capital_cost,
        350118.723363,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->operation_maintenance_cost_kWh,
        0.01,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_solar_ptr->firmness_factor,
        0.2,
        __FILE__,
        __LINE__
    );
    
    return test_solar_ptr;
}   /* testConstruct_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Function to test the trying to construct a Solar object given bad 
///     inputs is being handled as expected.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testBadConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
{
    bool error_flag = true;

    try {
        SolarInputs bad_solar_inputs;
        bad_solar_inputs.derating = -1;
        
        Solar bad_solar(8760, 1, bad_solar_inputs, time_vec_hrs_ptr);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionOverride_Solar(
///         std::string path_2_normalized_production_time_series,
///         std::vector<double>* time_vec_hrs_ptr
///     ) 
///
/// \brief Function to test that normalized production data is being read in correctly,
///     and that the associated production override feature is behaving as expected.
///
/// \param path_2_normalized_production_time_series A path (either relative or absolute)
///     to the given normalized production time series data.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testProductionOverride_Solar(
    std::string path_2_normalized_production_time_series,
    std::vector<double>* time_vec_hrs_ptr
) 
{
    SolarInputs solar_inputs;
    
    solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = 
        path_2_normalized_production_time_series;

    Solar test_solar_override(
        time_vec_hrs_ptr->size(),
        1,
        solar_inputs,
        time_vec_hrs_ptr
    );
    
    
    std::vector<double> expected_normalized_production_vec = {
        0.916955708517556,
        0.90947506148393,
        0.38425267564517,
        0.191510884037643,
        0.803361391862077,
        0.261511294927198,
        0.221944653883198,
        0.858495335855501,
        0.0162863861443092,
        0.774345409915512,
        0.354898664149867,
        0.11158009453439,
        0.191670176408956,
        0.0149072402795702,
        0.30174228469322,
        0.0815062957850151,
        0.776404660266821,
        0.207069187162109,
        0.518926216750454,
        0.148538109788597,
        0.443035200791027,
        0.62119079547209,
        0.270792717524391,
        0.761074879460849,
        0.0545251308358993,
        0.0895417089500092,
        0.21787190761933,
        0.834403724509682,
        0.908807953036246,
        0.815888965292123,
        0.416663215314571,
        0.523649705576525,
        0.490890480401437,
        0.28317138282312,
        0.877382682055847,
        0.14972090597986,
        0.480161632646382,
        0.0655830129932816,
        0.41802666403448,
        0.48692477737368,
        0.275957323208066,
        0.228651250718341,
        0.574371311550247,
        0.251872481275769,
        0.802697508767121,
        0.00130607304363551,
        0.481240172488057,
        0.702527508293784
    };
    
    for (size_t i = 0; i < expected_normalized_production_vec.size(); i++) {
        testFloatEquals(
            test_solar_override.normalized_production_vec[i],
            expected_normalized_production_vec[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_solar_override.computeProductionkW(i, rand(), rand()),
            test_solar_override.capacity_kW * expected_normalized_production_vec[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testProductionOverride_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testDetailed_Solar(
///         std::string path_2_normalized_production_time_series,
///         std::vector<double>* time_vec_hrs_ptr
///     ) 
///
/// \brief Function to test that the detailed production model is not acting erratically.
///     The underlying theory is very complicated, and there's plenty of opportunity for
///     floating point and divide-by-zero errors. =)
///
/// \param path_2_normalized_production_time_series A path (either relative or absolute)
///     to the given normalized production time series data.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testDetailed_Solar(void) 
{
    // init time and solar resource vectors
    std::vector<double> time_vec_hrs = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23
    };
    
    std::vector<double> solar_resource_vec_kWm2 = {
        0,
        0,
        0,
        0,
        0,
        0,
        8.51702662684015E-05,
        0.000348341567045,
        0.00213793728593,
        0.004099863613322,
        0.000997135230553,
        0.009534527624657,
        0.022927996790616,
        0.0136071715294,
        0.002535134127751,
        0.005206897515821,
        0.005627658648597,
        0.000701186722215,
        0.00017119827089,
        0,
        0,
        0,
        0,
        0
    };
    
    // init expected results (simple and detailed)
    std::vector<double> expected_simple_production_vec_kW = {
        0,
        0,
        0,
        0,
        0,
        0,
        0.00681362130147212,
        0.0278673253636,
        0.1710349828744,
        0.32798908906576,
        0.07977081844424,
        0.7627622099725601,
        1.83423974324928,
        1.088573722352,
        0.20281073022008,
        0.41655180126568,
        0.45021269188776,
        0.0560949377772,
        0.0136958616712,
        0,
        0,
        0,
        0,
        0
    };
    
    std::vector<double> expected_detailed_production_vec_kW = {
        0,
        0,
        0,
        0,
        0,
        0,
        0.007338124437333107,
        0.03001323298400045,
        0.1842098680357352,
        0.3532627387497894,
        0.085919752082476,
        0.8215778242841695,
        1.975723895381408,
        1.17256966118828,
        0.2184652818009985,
        0.4487156859620408,
        0.4849877212456633,
        0.06042929047364313,
        0.01475448450756636,
        0,
        0,
        0,
        0,
        0
    };
    
    // init Solar (simple)
    SolarInputs solar_inputs;
    
    Solar test_solar_simple(
        time_vec_hrs.size(),
        1,
        solar_inputs,
        &time_vec_hrs
    );
    
    // init Solar (detailed)
    solar_inputs.power_model = SolarPowerProductionModel :: SOLAR_POWER_DETAILED;
    
    solar_inputs.julian_day = 8766;
    solar_inputs.latitude_deg = 50;
    solar_inputs.longitude_deg = -125;
    solar_inputs.panel_azimuth_deg = 180;
    solar_inputs.panel_tilt_deg = 30;
    solar_inputs.albedo_ground_reflectance = 0.5;

    Solar test_solar_detailed(
        time_vec_hrs.size(),
        1,
        solar_inputs,
        &time_vec_hrs
    );
    
    // test simple production
    double production_kW = 0;
    
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        production_kW = test_solar_simple.computeProductionkW(
            i, 1, solar_resource_vec_kWm2[i]
        );
        
        test_solar_simple.commit(
            i, 1, production_kW, 100
        );
        
        testFloatEquals(
            production_kW,
            expected_simple_production_vec_kW[i],
            __FILE__,
            __LINE__
        );
    }
    
    // test detailed production
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        production_kW = test_solar_detailed.computeProductionkW(
            i, 1, solar_resource_vec_kWm2[i]
        );
        
        test_solar_detailed.commit(
            i, 1, production_kW, 100
        );
        
        testFloatEquals(
            production_kW,
            expected_detailed_production_vec_kW[i],
            __FILE__,
            __LINE__
        );
    }
    
}   /* testDetailed_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionConstraint_Solar(Renewable* test_solar_ptr)
///
/// \brief Function to test that the production constraint is active and behaving as
///     expected.
///
/// \param test_solar_ptr A Renewable pointer to the test Solar object.
///

void testProductionConstraint_Solar(Renewable* test_solar_ptr)
{
    testFloatEquals(
        test_solar_ptr->computeProductionkW(0, 1, 2),
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->computeProductionkW(0, 1, -1),
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testProductionConstraint_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommit_Solar(Renewable* test_solar_ptr)
///
/// \brief Function to test if the commit method is working as expected, by checking
///     some post-call attributes of the test Solar object. Uses a randomized resource
///     input.
///
/// \param test_solar_ptr A Renewable pointer to the test Solar object.
///

void testCommit_Solar(Renewable* test_solar_ptr)
{
    std::vector<double> dt_vec_hrs (48, 1);

    std::vector<double> load_vec_kW = {
        1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0
    };

    double load_kW = 0;
    double production_kW = 0;
    double roll = 0;
    double solar_resource_kWm2 = 0;

    for (int i = 0; i < 48; i++) {
        roll = (double)rand() / RAND_MAX;
        
        solar_resource_kWm2 = roll;
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll <= 0.1) {
            solar_resource_kWm2 = 0;
        }
        
        else if (roll >= 0.95) {
            solar_resource_kWm2 = 1.25;
        }
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll >= 0.95) {
            roll = 1.25;
        }
        
        load_vec_kW[i] *= roll * test_solar_ptr->capacity_kW;
        load_kW = load_vec_kW[i];
        
        production_kW = test_solar_ptr->computeProductionkW(
            i,
            dt_vec_hrs[i],
            solar_resource_kWm2
        );
        
        load_kW = test_solar_ptr->commit(
            i,
            dt_vec_hrs[i],
            production_kW,
            load_kW
        );
        
        // is running (or not) as expected
        if (solar_resource_kWm2 > 0) {
            testTruth(
                test_solar_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        else {
            testTruth(
                not test_solar_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        // load_kW <= load_vec_kW (i.e., after vs before)
        testLessThanOrEqualTo(
            load_kW,
            load_vec_kW[i],
            __FILE__,
            __LINE__
        );
        
        // production = dispatch + storage + curtailment
        testFloatEquals(
            test_solar_ptr->production_vec_kW[i] -
            test_solar_ptr->dispatch_vec_kW[i] -
            test_solar_ptr->storage_vec_kW[i] -
            test_solar_ptr->curtailment_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
        
        // capacity constraint
        if (solar_resource_kWm2 > 1) {
            testFloatEquals(
                test_solar_ptr->production_vec_kW[i],
                test_solar_ptr->capacity_kW,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testCommit_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEconomics_Solar(Combustion* test_solar_ptr)
///
/// \brief Function to test that the post-commit model economics for the test Solar
///     object are as expected (> 0 when running, = 0 when not).
///
/// \param test_solar_ptr A Combustion pointer to the test Solar object.
///

void testEconomics_Solar(Renewable* test_solar_ptr)
{
    for (int i = 0; i < 48; i++) {
        // resource, O&M > 0 whenever solar is running (i.e., producing)
        if (test_solar_ptr->is_running_vec[i]) {
            testGreaterThan(
                test_solar_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
        
        // resource, O&M = 0 whenever solar is not running (i.e., not producing)
        else {
            testFloatEquals(
                test_solar_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testEconomics_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Solar");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Renewable* test_solar_ptr = testConstruct_Solar(&time_vec_hrs);
    
    
    try {
        testBadConstruct_Solar(&time_vec_hrs);
        
        std::string path_2_normalized_production_time_series = 
            "data/test/normalized_production/normalized_solar_production.csv";
        
        testProductionOverride_Solar(
            path_2_normalized_production_time_series,
            &time_vec_hrs
        );
        
        testDetailed_Solar();
        
        testProductionConstraint_Solar(test_solar_ptr);
        
        testCommit_Solar(test_solar_ptr);
        testEconomics_Solar(test_solar_ptr);
    }


    catch (...) {
        delete test_solar_ptr;
        
        printGold(" ....... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_solar_ptr;

    printGold(" ....... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
