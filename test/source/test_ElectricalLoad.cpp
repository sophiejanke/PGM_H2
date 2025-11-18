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
/// \file test_ElectricalLoad.cpp
///
/// \brief Testing suite for ElectricalLoad class.
///
/// A suite of tests for the ElectricalLoad class.
///


#include "../utils/testing_utils.h"
#include "../../header/ElectricalLoad.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn ElectricalLoad* testConstruct_ElectricalLoad(void)
///
/// \brief A function to construct an ElectricalLoad object.
///
/// \return A pointer to a test ElectricalLoad object.
///

ElectricalLoad* testConstruct_ElectricalLoad(void)
{
    std::string path_2_electrical_load_time_series = 
        "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

    ElectricalLoad* test_electrical_load_ptr =
        new ElectricalLoad(path_2_electrical_load_time_series);

    testTruth(
        test_electrical_load_ptr->path_2_electrical_load_time_series ==
        path_2_electrical_load_time_series,
        __FILE__,
        __LINE__
    );

    return test_electrical_load_ptr;
}   /* testConstruct_ElectricalLoad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testPostConstructionAttributes_ElectricalLoad(
///         ElectricalLoad* test_electrical_load_ptr
///     )
///
/// \brief A function to check the values of various post-construction attributes.
///
/// \param test_electrical_load_ptr A pointer to the test ElectricalLoad object.
///

void testPostConstructionAttributes_ElectricalLoad(
    ElectricalLoad* test_electrical_load_ptr
)
{
    testFloatEquals(
        test_electrical_load_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_electrical_load_ptr->n_years,
        0.999886,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_electrical_load_ptr->min_load_kW,
        82.1211213927802,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_electrical_load_ptr->mean_load_kW,
        258.373472633202,
        __FILE__,
        __LINE__
    );


    testFloatEquals(
        test_electrical_load_ptr->max_load_kW,
        500,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testPostConstructionAttributes_ElectricalLoad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testDataRead_ElectricalLoad(ElectricalLoad* test_electrical_load_ptr)
///
/// \brief A function to check the values read into the test ElectricalLoad object.
///
/// \param test_electrical_load_ptr A pointer to the test ElectricalLoad object.
///

void testDataRead_ElectricalLoad(ElectricalLoad* test_electrical_load_ptr)
{
    std::vector<double> expected_dt_vec_hrs (48, 1);
    
    std::vector<double> expected_time_vec_hrs = {
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
        36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
    };

    std::vector<double> expected_load_vec_kW = {
        360.253836463674,
        355.171277826775,
        353.776453532298,
        353.75405737934,
        346.592867404975,
        340.132411175118,
        337.354867340578,
        340.644115618736,
        363.639028500678,
        378.787797779238,
        372.215798201712,
        395.093925731298,
        402.325427142659,
        386.907725462306,
        380.709170928091,
        372.062070914977,
        372.328646856954,
        391.841444284136,
        394.029351759596,
        383.369407765254,
        381.093099675206,
        382.604158946193,
        390.744843709034,
        383.13949492437,
        368.150393976985,
        364.629744480226,
        363.572736804082,
        359.854924202248,
        355.207590170267,
        349.094656012401,
        354.365935871597,
        343.380608328546,
        404.673065729266,
        486.296896820126,
        480.225974100847,
        457.318764401085,
        418.177339948609,
        414.399018364126,
        409.678420185754,
        404.768766016563,
        401.699589920585,
        402.44339040654,
        398.138372541906,
        396.010498627646,
        390.165117432277,
        375.850429417013,
        365.567100746484,
        365.429624610923
    };

    for (int i = 0; i < 48; i++) {
        testFloatEquals(
            test_electrical_load_ptr->dt_vec_hrs[i],
            expected_dt_vec_hrs[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_electrical_load_ptr->time_vec_hrs[i],
            expected_time_vec_hrs[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_electrical_load_ptr->load_vec_kW[i],
            expected_load_vec_kW[i],
            __FILE__,
            __LINE__
        );
    
    }
    
    return;
}   /* testDataRead_ElectricalLoad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting ElectricalLoad");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    ElectricalLoad* test_electrical_load_ptr = testConstruct_ElectricalLoad();


    try {
        testPostConstructionAttributes_ElectricalLoad(test_electrical_load_ptr);
        testDataRead_ElectricalLoad(test_electrical_load_ptr);
    }


    catch (...) {
        delete test_electrical_load_ptr;
        
        printGold(" ........................... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    } 


    delete test_electrical_load_ptr;

    printGold(" ........................... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;
}   /* main() */

// ---------------------------------------------------------------------------------- //
