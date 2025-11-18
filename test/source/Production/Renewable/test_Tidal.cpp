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
/// \file test_Tidal.cpp
///
/// \brief Testing suite for Tidal class.
///
/// A suite of tests for the Tidal class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Tidal.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable* testConstruct_Tidal(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Tidal object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A Renewable pointer to a test Tidal object.
///

Renewable* testConstruct_Tidal(std::vector<double>* time_vec_hrs_ptr)
{
    TidalInputs tidal_inputs;

    Renewable* test_tidal_ptr = new Tidal(8760, 1, tidal_inputs, time_vec_hrs_ptr);
    
    testTruth(
        not tidal_inputs.renewable_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_tidal_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_tidal_ptr->type,
        RenewableType :: TIDAL,
        __FILE__,
        __LINE__
    );

    testTruth(
        test_tidal_ptr->type_str == "TIDAL",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_tidal_ptr->capital_cost,
        500237.446725,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_tidal_ptr->operation_maintenance_cost_kWh,
        0.069905,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_tidal_ptr->firmness_factor,
        0.8,
        __FILE__,
        __LINE__
    );

    return test_tidal_ptr;
}   /* testConstruct_Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Tidal(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Function to test the trying to construct a Tidal object given bad 
///     inputs is being handled as expected.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testBadConstruct_Tidal(std::vector<double>* time_vec_hrs_ptr)
{
    bool error_flag = true;

    try {
        TidalInputs bad_tidal_inputs;
        bad_tidal_inputs.design_speed_ms = -1;
        
        Tidal bad_tidal(8760, 1, bad_tidal_inputs, time_vec_hrs_ptr);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionConstraint_Tidal(Renewable* test_tidal_ptr)
///
/// \brief Function to test that the production constraint is active and behaving as
///     expected.
///
/// \param test_tidal_ptr A Renewable pointer to the test Tidal object.
///

void testProductionConstraint_Tidal(Renewable* test_tidal_ptr)
{
    testFloatEquals(
        test_tidal_ptr->computeProductionkW(0, 1, 1e6),
        0,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_tidal_ptr->computeProductionkW(
            0,
            1,
            ((Tidal*)test_tidal_ptr)->design_speed_ms
        ),
        test_tidal_ptr->capacity_kW,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_tidal_ptr->computeProductionkW(0, 1, -1),
        0,
        __FILE__,
        __LINE__
    );
        
    return;
}   /* testProductionConstraint_Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommit_Tidal(Renewable* test_tidal_ptr)
///
/// \brief Function to test if the commit method is working as expected, by checking
///     some post-call attributes of the test Tidal object. Uses a randomized resource
///     input.
///
/// \param test_tidal_ptr A Renewable pointer to the test Tidal object.
///

void testCommit_Tidal(Renewable* test_tidal_ptr)
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
    double tidal_resource_ms = 0;

    for (int i = 0; i < 48; i++) {
        roll = (double)rand() / RAND_MAX;
        
        tidal_resource_ms = roll * ((Tidal*)test_tidal_ptr)->design_speed_ms;
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll <= 0.1) {
            tidal_resource_ms = 0;
        }
        
        else if (roll >= 0.95) {
            tidal_resource_ms = 3 * ((Tidal*)test_tidal_ptr)->design_speed_ms;
        }
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll >= 0.95) {
            roll = 1.25;
        }
        
        load_vec_kW[i] *= roll * test_tidal_ptr->capacity_kW;
        load_kW = load_vec_kW[i];
        
        production_kW = test_tidal_ptr->computeProductionkW(
            i,
            dt_vec_hrs[i],
            tidal_resource_ms
        );
        
        load_kW = test_tidal_ptr->commit(
            i,
            dt_vec_hrs[i],
            production_kW,
            load_kW
        );
        
        // is running (or not) as expected
        if (production_kW > 0) {
            testTruth(
                test_tidal_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        else {
            testTruth(
                not test_tidal_ptr->is_running,
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
            test_tidal_ptr->production_vec_kW[i] -
            test_tidal_ptr->dispatch_vec_kW[i] -
            test_tidal_ptr->storage_vec_kW[i] -
            test_tidal_ptr->curtailment_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testCommit_Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEconomics_Tidal(Combustion* test_tidal_ptr)
///
/// \brief Function to test that the post-commit model economics for the test Tidal
///     object are as expected (> 0 when running, = 0 when not).
///
/// \param test_tidal_ptr A Combustion pointer to the test Tidal object.
///

void testEconomics_Tidal(Renewable* test_tidal_ptr)
{
    for (int i = 0; i < 48; i++) {
        // resource, O&M > 0 whenever tidal is running (i.e., producing)
        if (test_tidal_ptr->is_running_vec[i]) {
            testGreaterThan(
                test_tidal_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
        
        // resource, O&M = 0 whenever tidal is not running (i.e., not producing)
        else {
            testFloatEquals(
                test_tidal_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testEconomics_Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Tidal");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Renewable* test_tidal_ptr = testConstruct_Tidal(&time_vec_hrs);
    
    
    try {
        testBadConstruct_Tidal(&time_vec_hrs);
        
        testProductionConstraint_Tidal(test_tidal_ptr);
        
        testCommit_Tidal(test_tidal_ptr);
        testEconomics_Tidal(test_tidal_ptr);
    }


    catch (...) {
        delete test_tidal_ptr;
        
        printGold(" ....... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_tidal_ptr;

    printGold(" ....... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
