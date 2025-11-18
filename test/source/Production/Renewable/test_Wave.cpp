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
/// \file test_Wave.cpp
///
/// \brief Testing suite for Wave class.
///
/// A suite of tests for the Wave class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Wave.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable* testConstruct_Wave(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Wave object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A Renewable pointer to a test Wave object.
///

Renewable* testConstruct_Wave(std::vector<double>* time_vec_hrs_ptr)
{
    WaveInputs wave_inputs;

    Renewable* test_wave_ptr = new Wave(8760, 1, wave_inputs, time_vec_hrs_ptr);
    
    testTruth(
        not wave_inputs.renewable_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_wave_ptr->type,
        RenewableType :: WAVE,
        __FILE__,
        __LINE__
    );

    testTruth(
        test_wave_ptr->type_str == "WAVE",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_wave_ptr->capital_cost,
        850831.063539,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_wave_ptr->operation_maintenance_cost_kWh,
        0.069905,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_wave_ptr->firmness_factor,
        0.8,
        __FILE__,
        __LINE__
    );

    return test_wave_ptr;
}   /* testConstruct_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable* testConstructLookup_Wave(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Wave object using production lookup.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A Renewable pointer to a test Wave object.
///

Renewable* testConstructLookup_Wave(std::vector<double>* time_vec_hrs_ptr)
{
    WaveInputs wave_inputs;
    
    wave_inputs.power_model = WavePowerProductionModel :: WAVE_POWER_LOOKUP;
    wave_inputs.path_2_normalized_performance_matrix =
        "data/test/interpolation/wave_energy_converter_normalized_performance_matrix.csv";

    Renewable* test_wave_lookup_ptr = new Wave(8760, 1, wave_inputs, time_vec_hrs_ptr);
    
    return test_wave_lookup_ptr;
}   /* testConstructLookup_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Wave(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Function to test the trying to construct a Wave object given bad 
///     inputs is being handled as expected.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testBadConstruct_Wave(std::vector<double>* time_vec_hrs_ptr)
{
    bool error_flag = true;

    try {
        WaveInputs bad_wave_inputs;
        bad_wave_inputs.design_significant_wave_height_m = -1;
        
        Wave bad_wave(8760, 1, bad_wave_inputs, time_vec_hrs_ptr);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionConstraint_Wave(Renewable* test_wave_ptr)
///
/// \brief Function to test that the production constraint is active and behaving as
///     expected.
///
/// \param test_wave_ptr A Renewable pointer to the test Wave object.
///

void testProductionConstraint_Wave(Renewable* test_wave_ptr)
{
    testFloatEquals(
        test_wave_ptr->computeProductionkW(0, 1, 0, rand()),
        0,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_wave_ptr->computeProductionkW(0, 1, rand(), 0),
        0,
        __FILE__,
        __LINE__
    );
        
    return;
}   /* testProductionConstraint_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommit_Wave(Renewable* test_wave_ptr)
///
/// \brief Function to test if the commit method is working as expected, by checking
///     some post-call attributes of the test Wave object. Uses a randomized resource
///     input.
///
/// \param test_wave_ptr A Renewable pointer to the test Wave object.
///

void testCommit_Wave(Renewable* test_wave_ptr)
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
    double significant_wave_height_m = 0;
    double energy_period_s = 0;

    for (int i = 0; i < 48; i++) {
        roll = (double)rand() / RAND_MAX;
        
        if (roll <= 0.05) {
            roll = 0;
        }
        
        significant_wave_height_m = roll *
            ((Wave*)test_wave_ptr)->design_significant_wave_height_m;
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll <= 0.05) {
            roll = 0;
        }
        
        energy_period_s = roll * ((Wave*)test_wave_ptr)->design_energy_period_s;
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll >= 0.95) {
            roll = 1.25;
        }
        
        load_vec_kW[i] *= roll * test_wave_ptr->capacity_kW;
        load_kW = load_vec_kW[i];
        
        production_kW = test_wave_ptr->computeProductionkW(
            i,
            dt_vec_hrs[i],
            significant_wave_height_m,
            energy_period_s
        );
        
        load_kW = test_wave_ptr->commit(
            i,
            dt_vec_hrs[i],
            production_kW,
            load_kW
        );
        
        // is running (or not) as expected
        if (production_kW > 0) {
            testTruth(
                test_wave_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        else {
            testTruth(
                not test_wave_ptr->is_running,
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
            test_wave_ptr->production_vec_kW[i] -
            test_wave_ptr->dispatch_vec_kW[i] -
            test_wave_ptr->storage_vec_kW[i] -
            test_wave_ptr->curtailment_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testCommit_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEconomics_Wave(Combustion* test_wave_ptr)
///
/// \brief Function to test that the post-commit model economics for the test Wave
///     object are as expected (> 0 when running, = 0 when not).
///
/// \param test_wave_ptr A Combustion pointer to the test Wave object.
///

void testEconomics_Wave(Renewable* test_wave_ptr)
{
    for (int i = 0; i < 48; i++) {
        // resource, O&M > 0 whenever wave is running (i.e., producing)
        if (test_wave_ptr->is_running_vec[i]) {
            testGreaterThan(
                test_wave_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
        
        // resource, O&M = 0 whenever wave is not running (i.e., not producing)
        else {
            testFloatEquals(
                test_wave_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testEconomics_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionLookup_Wave(Renewable* test_wave_lookup_ptr)
///
/// \brief Function to test that production lookup (i.e., interpolation) is
///     returning the expected values.
///
/// \param test_wave_lookup_ptr A Renewable pointer to the test Wave object using
///     production lookup.
///

void testProductionLookup_Wave(Renewable* test_wave_lookup_ptr)
{
    std::vector<double> significant_wave_height_vec_m = {
        0.389211848822208,
        0.836477431896843,
        1.52738334015579,
        1.92640601114508,
        2.27297317532019,
        2.87416589636605,
        3.72275770908175,
        3.95063175885536,
        4.68097139867404,
        4.97775020449812,
        5.55184219980547,
        6.06566629451658,
        6.27927876785062,
        6.96218133671013,
        7.51754442460228
    };

    std::vector<double> energy_period_vec_s = {
        5.45741899698926,
        6.00101329139007,
        7.50567689404182,
        8.77681262912881,
        9.45143678206774,
        10.7767876462885,
        11.4795760857165,
        12.9430684577599,
        13.303544885703,
        14.5069863517863,
        15.1487890438045,
        16.086524049077,
        17.176609978648,
        18.4155153740256,
        19.1704554940162
    };

    std::vector<std::vector<double>> expected_normalized_performance_matrix = {
        {0.0337204906738533,0.145056406036013,0.334677248806653,0.441674658936075,0.533295755691263,0.68807895676592,0.899614883290132,0.943917981844937,1,1,0.672452312957154,0.457418996989256,0.457418996989256,0.457418996989256,0.457418996989256},
        {0.0310681846933292,0.135425896595439,0.324045598153363,0.430214268249038,0.520985043044784,0.673879556322479,0.882058036543473,0.933324805392533,0.999995747889915,1,1,1,1,1,1},
        {0.0237266281076604,0.108768742207538,0.294617294841705,0.398492020763049,0.486909112828702,0.63457575706117,0.83346085929357,0.884355524380891,0.993224142804052,1,1,1,1,1,1},
        {0.0175245009938255,0.0862488504001753,0.269756343931147,0.371693152028768,0.458121859300634,0.601372013927032,0.792406095612277,0.84076702793237,0.983559887777234,0.997991137104325,1,1,1,1,1},
        {0.0142328739589644,0.0742969694833995,0.256562003243255,0.357470308928265,0.442843729679424,0.583749940636223,0.770617285489097,0.817633501210732,0.962883795962208,0.98710374026292,1,1,1,1,1},
        {0.0077662203173173,0.0508165832074184,0.230640709501637,0.329528443353471,0.41282867283787,0.549130026772199,0.727811497701488,0.772185914581302,0.908206738599826,0.954998756889688,0.998402819830565,1,1,1,1},
        {0.00433717405958826,0.0383657337957315,0.21689552996585,0.314711823368423,0.396912710109449,0.530772265145106,0.70511304583499,0.748086606733608,0.879213356655784,0.928045483527716,0.988916918726541,1,1,1,1},
        {0.000102358416923608,0.0210697053701168,0.188272456115393,0.283857573197153,0.363769179652786,0.492543912767949,0.657845604738695,0.697902011728977,0.818837301879771,0.862960462039225,0.94142006034892,0.987816689724305,1,1,1},
        {0,0.0196038727057393,0.181222235960193,0.276257786480759,0.355605514643888,0.483127792688125,0.646203044346932,0.68554091993881,0.803965926411948,0.846929209326914,0.923608721107496,0.973919391957909,0.989317558497062,1,1},
        {0,0.0157252942367668,0.157685253727545,0.250886090139653,0.328351324840186,0.451692313207986,0.607334650020078,0.644273726654544,0.754318214361004,0.793409276338945,0.862016745415184,0.914414994064087,0.934224180981529,0.974901798693668,0.988382658030559},
        {0,0.0136568246246201,0.145132837191606,0.23735520935175,0.313816498778623,0.43492757979648,0.586605897674033,0.622265680157626,0.727840785965132,0.764866768841587,0.829169368189018,0.877316199136958,0.895454752548097,0.939965262172075,0.965227157200508},
        {0,0.0106345930466366,0.12679255826648,0.217585300741544,0.292579730277991,0.410432703770651,0.556319211544087,0.590109823140377,0.689154735306282,0.723163440464515,0.781176214156733,0.823111317965151,0.838551241095433,0.873600187044088,0.890490624702448},
        {0,0.00712134879261874,0.10547259059088,0.194603435839713,0.267892689267542,0.381958220518761,0.52111194060085,0.552729702860433,0.644183480893496,0.674684700495844,0.72538575988962,0.760099936341102,0.772402791022689,0.796452982124592,0.803611877462563},
        {0,0.00312847342058727,0.0812420026472571,0.168484067035528,0.239835352250276,0.349596376397684,0.481098142839729,0.510246416508033,0.59307271226737,0.619587601378226,0.661978744968056,0.688486186868201,0.697223697826578,0.708773574639095,0.704872369154718},
        {0,0.00103256269522045,0.0673448574082101,0.152567953107312,0.222738316872545,0.329876344040866,0.456715311514779,0.484358776163468,0.56192782425301,0.586013600672294,0.62334100908303,0.644847590392824,0.651412521404216,0.655345199353316,0.644704404330397}
    };

    for (size_t i = 0; i < energy_period_vec_s.size(); i++) {
        for (size_t j = 0; j < significant_wave_height_vec_m.size(); j++) {
            testFloatEquals(
                test_wave_lookup_ptr->computeProductionkW(
                    0,
                    1,
                    significant_wave_height_vec_m[j],
                    energy_period_vec_s[i]
                ),
                expected_normalized_performance_matrix[i][j] * 
                test_wave_lookup_ptr->capacity_kW,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testProductionLookup_Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Wave");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Renewable* test_wave_ptr = testConstruct_Wave(&time_vec_hrs);
    Renewable* test_wave_lookup_ptr = testConstructLookup_Wave(&time_vec_hrs);
    
    
    try {
        testBadConstruct_Wave(&time_vec_hrs);
        
        testProductionConstraint_Wave(test_wave_ptr);
        
        testCommit_Wave(test_wave_ptr);
        testEconomics_Wave(test_wave_ptr);
        
        testProductionLookup_Wave(test_wave_lookup_ptr);
    }


    catch (...) {
        delete test_wave_ptr;
        delete test_wave_lookup_ptr;
        
        printGold(" ........ ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_wave_ptr;
    delete test_wave_lookup_ptr;

    printGold(" ........ ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //

