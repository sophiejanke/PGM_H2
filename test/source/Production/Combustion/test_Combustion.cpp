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
/// \file test_Combustion.cpp
///
/// \brief Testing suite for Combustion class.
///
/// A suite of tests for the Combustion class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Combustion/Combustion.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion* testConstruct_Combustion(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Combustion object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A pointer to a test Combustion object.
///

Combustion* testConstruct_Combustion(std::vector<double>* time_vec_hrs_ptr)
{
    CombustionInputs combustion_inputs;

    Combustion* test_combustion_ptr = new Combustion(
        8760,
        1,
        combustion_inputs,
        time_vec_hrs_ptr
    );
    
    testTruth(
        not combustion_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->fuel_consumption_vec_L.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->fuel_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CO2_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CO_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->NOx_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->SOx_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CH4_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->PM_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    return test_combustion_ptr;
}   /* testConstruct_Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Combustion");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Combustion* test_combustion_ptr = testConstruct_Combustion(&time_vec_hrs);
    
    
    try {
        //...
    }


    catch (...) {
        delete test_combustion_ptr;
        
        printGold(" ................ ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_combustion_ptr;

    printGold(" ................ ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
