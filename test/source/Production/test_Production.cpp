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
/// \file test_Production.cpp
///
/// \brief Testing suite for Production class.
///
/// A suite of tests for the Production class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Production/Production.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Production* testConstruct_Production(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Production object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A pointer to a test Production object.
///

Production* testConstruct_Production(std::vector<double>* time_vec_hrs_ptr)
{
    ProductionInputs production_inputs;

    Production* test_production_ptr = new Production(
        8760,
        1,
        production_inputs,
        time_vec_hrs_ptr
    );
    
    testTruth(
        not production_inputs.print_flag,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        production_inputs.nominal_inflation_annual,
        0.02,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        production_inputs.nominal_discount_annual,
        0.04,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->capacity_kW,
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->real_discount_annual,
        0.0196078431372549,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->production_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->dispatch_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->storage_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->curtailment_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->capital_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->operation_maintenance_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    return test_production_ptr;
}   /* testConstruct_Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Production(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Function to test the trying to construct a Production object given bad 
///     inputs is being handled as expected.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testBadConstruct_Production(std::vector<double>* time_vec_hrs_ptr)
{
    bool error_flag = true;

    try {
        ProductionInputs production_inputs;
        
        Production bad_production(0, 1, production_inputs, time_vec_hrs_ptr);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Production* test_production_ptr = testConstruct_Production(&time_vec_hrs);
    
        
    try {
        testBadConstruct_Production(&time_vec_hrs);
    }


    catch (...) {
        delete test_production_ptr;
        
        printGold(" ............................... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_production_ptr;

    printGold(" ............................... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
