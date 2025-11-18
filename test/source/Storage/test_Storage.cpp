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
/// \file test_Storage.cpp
///
/// \brief Testing suite for Storage class.
///
/// A suite of tests for the Storage class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Storage/Storage.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Storage* testConstruct_Storage(void)
///
/// \brief A function to construct a Storage object and spot check some
///     post-construction attributes.
///
/// \return A Renewable pointer to a test Storage object.
///

Storage* testConstruct_Storage(void)
{
    StorageInputs storage_inputs;

    Storage* test_storage_ptr = new Storage(8760, 1, storage_inputs);
    
    testFloatEquals(
        test_storage_ptr->power_capacity_kW,
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->energy_capacity_kWh,
        1000,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->charge_vec_kWh.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->charging_power_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->discharging_power_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->capital_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->operation_maintenance_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );
    
    return test_storage_ptr;
}   /* testConstruct_Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Storage(void)
///
/// \brief Function to test the trying to construct a Storage object given bad 
///     inputs is being handled as expected.
///

void testBadConstruct_Storage(void)
{
    bool error_flag = true;

    try {
        StorageInputs bad_storage_inputs;
        bad_storage_inputs.energy_capacity_kWh = 0;
        
        Storage bad_storage(8760, 1, bad_storage_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Storage");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    Storage* test_storage_ptr = testConstruct_Storage();
    
    
    try {
        testBadConstruct_Storage();
    }


    catch (...) {
        delete test_storage_ptr;
        
        printGold(" .................................. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_storage_ptr;

    printGold(" .................................. ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
