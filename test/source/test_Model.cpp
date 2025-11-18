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
/// \file test_Model.cpp
///
/// \brief Testing suite for Model class.
///
/// A suite of tests for the Model class.
///


#include "../utils/testing_utils.h"
#include "../../header/Model.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Model* testConstruct_Model(void)
///
/// \brief A function to construct an Model object.
///
/// \param test_model_inputs A ModelInputs structure for the Model constructor.
///
/// \return A pointer to a test Model object.
///

Model* testConstruct_Model(ModelInputs test_model_inputs)
{
    Model* test_model_ptr = new Model(test_model_inputs);
    
    testTruth(
        test_model_ptr->electrical_load.path_2_electrical_load_time_series ==
        test_model_inputs.path_2_electrical_load_time_series,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model_ptr->controller.firm_dispatch_ratio,
        0.1,
        __FILE__,
        __LINE__
    );
    
    // DEPRECATED
    /*
    testFloatEquals(
        test_model_ptr->controller.load_operating_reserve_factor,
        0.2,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model_ptr->controller.max_operating_reserve_factor,
        1,
        __FILE__,
        __LINE__
    );
    */

    return test_model_ptr;
}   /* testConstruct_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

/// 
/// \fn void testBadConstruct_Model(void)
///
/// \brief Function to check if passing bad ModelInputs to the Model constructor is
///     handled appropriately.
///

void testBadConstruct_Model(void)
{
    bool error_flag = true;
    
    try {
        ModelInputs bad_model_inputs;   // path_2_electrical_load_time_series left empty
        
        Model bad_model(bad_model_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    try {
        ModelInputs bad_model_inputs;
        bad_model_inputs.path_2_electrical_load_time_series =
            "data/test/electrical_load/bad_path_";
        bad_model_inputs.path_2_electrical_load_time_series += std::to_string(rand());
        bad_model_inputs.path_2_electrical_load_time_series += ".csv";
        
        Model bad_model(bad_model_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testPostConstructionAttributes_Model(Model* test_model_ptr)
///
/// \brief A function to check the values of various post-construction attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testPostConstructionAttributes_Model(Model* test_model_ptr)
{
    testFloatEquals(
        test_model_ptr->electrical_load.n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->electrical_load.n_years,
        0.999886,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->electrical_load.min_load_kW,
        82.1211213927802,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->electrical_load.mean_load_kW,
        258.373472633202,
        __FILE__,
        __LINE__
    );


    testFloatEquals(
        test_model_ptr->electrical_load.max_load_kW,
        500,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testPostConstructionAttributes_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testElectricalLoadData_Model(Model* test_model_ptr)
///
/// \brief Function to check the values read into the ElectricalLoad component of the
///     test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testElectricalLoadData_Model(Model* test_model_ptr)
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
            test_model_ptr->electrical_load.dt_vec_hrs[i],
            expected_dt_vec_hrs[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_model_ptr->electrical_load.time_vec_hrs[i],
            expected_time_vec_hrs[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_model_ptr->electrical_load.load_vec_kW[i],
            expected_load_vec_kW[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testElectricalLoadData_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddSolarResource_Model(
///         Model* test_model_ptr,
///         std::string path_2_solar_resource_data,
///         int solar_resource_key
///     )
///
/// \brief Function to test adding a solar resource and then check the values read into
///     the Resources component of the test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_solar_resource_data A path (either relative or absolute) to the solar
///     resource data.
///
/// \param solar_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddSolarResource_Model(
    Model* test_model_ptr,
    std::string path_2_solar_resource_data,
    int solar_resource_key
)
{
    test_model_ptr->addResource(
        RenewableType :: SOLAR,
        path_2_solar_resource_data,
        solar_resource_key
    );

    std::vector<double> expected_solar_resource_vec_kWm2 = {
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
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0.000141055102242,
        0.00084525014743,
        0.024893647822702,
        0.091245556190749,
        0.158722176731637,
        0.152859680515876,
        0.149922903895116,
        0.13049996570866,
        0.03081254222795,
        0.001218928911125,
        0.000206092647423,
        0,
        0,
        0,
        0,
        0,
        0
    };

    for (size_t i = 0; i < expected_solar_resource_vec_kWm2.size(); i++) {
        testFloatEquals(
            test_model_ptr->resources.resource_map_1D[solar_resource_key][i],
            expected_solar_resource_vec_kWm2[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testAddSolarResource_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddTidalResource_Model(
///         Model* test_model_ptr,
///         std::string path_2_tidal_resource_data,
///         int tidal_resource_key
///     )
///
/// \brief Function to test adding a tidal resource and then check the values read into
///     the Resources component of the test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_tidal_resource_data A path (either relative or absolute) to the tidal
///     resource data.
///
/// \param tidal_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddTidalResource_Model(
    Model* test_model_ptr,
    std::string path_2_tidal_resource_data,
    int tidal_resource_key
)
{
    test_model_ptr->addResource(
        RenewableType :: TIDAL,
        path_2_tidal_resource_data,
        tidal_resource_key
    );
    
    std::vector<double> expected_tidal_resource_vec_ms = {
        0.347439913040533,
        0.770545522195602,
        0.731352084836198,
        0.293389814389542,
        0.209959110813115,
        0.610609623896497,
        1.78067162013604,
        2.53522775118089,
        2.75966627832024,
        2.52101111143895,
        2.05389330201031,
        1.3461515862445,
        0.28909254878384,
        0.897754086048563,
        1.71406453837407,
        1.85047408742869,
        1.71507908595979,
        1.33540349705416,
        0.434586143463003,
        0.500623815700637,
        1.37172172646733,
        1.68294125491228,
        1.56101300975417,
        1.04925834219412,
        0.211395463930223,
        1.03720048903385,
        1.85059536356448,
        1.85203242794517,
        1.4091471616277,
        0.767776539039899,
        0.251464906990961,
        1.47018469375652,
        2.36260493698197,
        2.46653750048625,
        2.12851908739291,
        1.62783753197988,
        0.734594890957439,
        0.441886297300355,
        1.6574418350918,
        2.0684558286637,
        1.87717416992136,
        1.58871262337931,
        1.03451227609235,
        0.193371305159817,
        0.976400122458815,
        1.6583227369707,
        1.76690616570953,
        1.54801328553115
    };

    for (size_t i = 0; i < expected_tidal_resource_vec_ms.size(); i++) {
        testFloatEquals(
            test_model_ptr->resources.resource_map_1D[tidal_resource_key][i],
            expected_tidal_resource_vec_ms[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testAddTidalResource_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddWaveResource_Model(
///         Model* test_model_ptr,
///         std::string path_2_wave_resource_data,
///         int wave_resource_key
///     )
///
/// \brief Function to test adding a wave resource and then check the values read into
///     the Resources component of the test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_wave_resource_data A path (either relative or absolute) to the wave
///     resource data.
///
/// \param wave_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddWaveResource_Model(
    Model* test_model_ptr,
    std::string path_2_wave_resource_data,
    int wave_resource_key
)
{
    test_model_ptr->addResource(
        RenewableType :: WAVE,
        path_2_wave_resource_data,
        wave_resource_key
    );
    
    std::vector<double> expected_significant_wave_height_vec_m = {
        4.26175222125028,
        4.25020976167872,
        4.25656524330349,
        4.27193854786718,
        4.28744955711233,
        4.29421815278154,
        4.2839937266082,
        4.25716982457976,
        4.22419391611483,
        4.19588925217606,
        4.17338788587412,
        4.14672746914214,
        4.10560041173665,
        4.05074966447193,
        3.9953696962433,
        3.95316976150866,
        3.92771018142378,
        3.91129562488595,
        3.89558312094911,
        3.87861093931749,
        3.86538307240754,
        3.86108961027929,
        3.86459448853189,
        3.86796474016882,
        3.86357412779993,
        3.85554872014731,
        3.86044266668675,
        3.89445961915999,
        3.95554798115731,
        4.02265508610476,
        4.07419587011404,
        4.10314247143958,
        4.11738045085928,
        4.12554995596708,
        4.12923992001675,
        4.1229292327442,
        4.10123955307441,
        4.06748827895363,
        4.0336230651344,
        4.01134236393876,
        4.00136570034559,
        3.99368787690411,
        3.97820924247644,
        3.95369335178055,
        3.92742545608532,
        3.90683362771686,
        3.89331520944006,
        3.88256045801583
    };
    
    std::vector<double> expected_energy_period_vec_s = {
        10.4456008226821,
        10.4614151137651,
        10.4462827795433,
        10.4127692097884,
        10.3734397942723,
        10.3408599227669,
        10.32637292093,
        10.3245412676322,
        10.310409818185,
        10.2589529840966,
        10.1728100603103,
        10.0862908658929,
        10.03480243813,
        10.023673635806,
        10.0243418565116,
        10.0063487117653,
        9.96050302286607,
        9.9011999635568,
        9.84451822125472,
        9.79726875879626,
        9.75614594835158,
        9.7173447961368,
        9.68342904390577,
        9.66380508567062,
        9.6674009575699,
        9.68927134575103,
        9.70979984863046,
        9.70967357906908,
        9.68983025704562,
        9.6722855524805,
        9.67973599910003,
        9.71977125328293,
        9.78450442291421,
        9.86532355233449,
        9.96158937600019,
        10.0807018356507,
        10.2291022504937,
        10.39458528356,
        10.5464393581004,
        10.6553277500484,
        10.7245553190084,
        10.7893127285064,
        10.8846512240849,
        11.0148158739075,
        11.1544325654719,
        11.2772785848343,
        11.3744362756187,
        11.4533643503183
    };
    
    for (size_t i = 0; i < expected_energy_period_vec_s.size(); i++) {
        testFloatEquals(
            test_model_ptr->resources.resource_map_2D[wave_resource_key][i][0],
            expected_significant_wave_height_vec_m[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_model_ptr->resources.resource_map_2D[wave_resource_key][i][1],
            expected_energy_period_vec_s[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testAddWaveResource_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddWindResource_Model(
///         Model* test_model_ptr,
///         std::string path_2_wind_resource_data,
///         int wind_resource_key
///     )
///
/// \brief Function to test adding a wind resource and then check the values read into
///     the Resources component of the test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_wind_resource_data A path (either relative or absolute) to the wind
///     resource data.
///
/// \param wind_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddWindResource_Model(
    Model* test_model_ptr,
    std::string path_2_wind_resource_data,
    int wind_resource_key
)
{
    test_model_ptr->addResource(
        RenewableType :: WIND,
        path_2_wind_resource_data,
        wind_resource_key
    );
    
    std::vector<double> expected_wind_resource_vec_ms = {
        6.88566688469997,
        5.02177105466549,
        3.74211715899568,
        5.67169579985362,
        4.90670669971858,
        4.29586955031368,
        7.41155377205065,
        10.2243290476943,
        13.1258696725555,
        13.7016198628274,
        16.2481482330233,
        16.5096744355418,
        13.4354482206162,
        14.0129230731609,
        14.5554549260515,
        13.4454539065912,
        13.3447169512094,
        11.7372615098554,
        12.7200070078013,
        10.6421127908149,
        6.09869498990661,
        5.66355596602321,
        4.97316966910831,
        3.48937138360567,
        2.15917470979169,
        1.29061103587027,
        3.43475751425219,
        4.11706326260927,
        4.28905275747408,
        5.75850263196241,
        8.98293663055264,
        11.7069822941315,
        12.4031987075858,
        15.4096570910089,
        16.6210843829552,
        13.3421219142573,
        15.2112831900548,
        18.350864533037,
        15.8751799822971,
        15.3921198799796,
        15.9729192868434,
        12.4728950178772,
        10.177050481096,
        10.7342247355551,
        8.98846695631389,
        4.14671169124739,
        3.17256452697149,
        3.40036336968628
    };

    for (size_t i = 0; i < expected_wind_resource_vec_ms.size(); i++) {
        testFloatEquals(
            test_model_ptr->resources.resource_map_1D[wind_resource_key][i],
            expected_wind_resource_vec_ms[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testAddWindResource_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddHydroResource_Model(
///         Model* test_model_ptr,
///         std::string path_2_hydro_resource_data,
///         int hydro_resource_key
///     )
///
/// \brief Function to test adding a hydro resource and then check the values read into
///     the Resources component of the test Model object.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_hydro_resource_data A path (either relative or absolute) to the hydro
///     resource data.
///
/// \param hydro_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddHydroResource_Model(
    Model* test_model_ptr,
    std::string path_2_hydro_resource_data,
    int hydro_resource_key
)
{
    test_model_ptr->addResource(
        NoncombustionType :: HYDRO,
        path_2_hydro_resource_data,
        hydro_resource_key
    );
    
    std::vector<double> expected_hydro_resource_vec_ms = {
        2167.91531556942,
        2046.58261560569,
        2007.85941123153,
        2000.11477247929,
        1917.50527264453,
        1963.97311577093,
        1908.46985899809,
        1886.5267112678,
        1965.26388854254,
        1953.64692935289,
        2084.01504296306,
        2272.46796101188,
        2520.29645627096,
        2715.203242423,
        2720.36633563203,
        3130.83228077221,
        3289.59741021591,
        3981.45195965772,
        5295.45929491303,
        7084.47124360523,
        7709.20557708454,
        7436.85238642936,
        7235.49173429668,
        6710.14695517339,
        6015.71085806577,
        5279.97001316337,
        4877.24870889801,
        4421.60569340303,
        3919.49483690424,
        3498.70270322341,
        3274.10813058883,
        3147.61233529349,
        2904.94693324343,
        2805.55738101,
        2418.32535637171,
        2398.96375630723,
        2260.85100182222,
        2157.58912702878,
        2019.47637254377,
        1913.63295220712,
        1863.29279076589,
        1748.41395678279,
        1695.49224555317,
        1599.97501375715,
        1559.96103873397,
        1505.74855473274,
        1438.62833664765,
        1384.41585476901
    };

    for (size_t i = 0; i < expected_hydro_resource_vec_ms.size(); i++) {
        testFloatEquals(
            test_model_ptr->resources.resource_map_1D[hydro_resource_key][i],
            expected_hydro_resource_vec_ms[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testAddHydroResource_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddHydro_Model(
///         Model* test_model_ptr,
///         int hydro_resource_key
///     )
///
/// \brief Function to test adding a hydroelectric asset to the test Model object, and
///     then spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param hydro_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddHydro_Model(
    Model* test_model_ptr,
    int hydro_resource_key
)
{
    HydroInputs hydro_inputs;
    hydro_inputs.noncombustion_inputs.production_inputs.capacity_kW = 300;
    hydro_inputs.reservoir_capacity_m3 = 100000;
    hydro_inputs.init_reservoir_state = 0.5;
    hydro_inputs.noncombustion_inputs.production_inputs.is_sunk = true;
    hydro_inputs.resource_key = hydro_resource_key;

    test_model_ptr->addHydro(hydro_inputs);

    testFloatEquals(
        test_model_ptr->noncombustion_ptr_vec.size(),
        1,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->noncombustion_ptr_vec[0]->type,
        NoncombustionType :: HYDRO,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->noncombustion_ptr_vec[0]->resource_key,
        hydro_resource_key,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddHydro_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddDiesel_Model(Model* test_model_ptr)
///
/// \brief Function to test adding a suite of diesel generators to the test Model object,
///     and then spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testAddDiesel_Model(Model* test_model_ptr)
{
    DieselInputs diesel_inputs;
    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 100;
    diesel_inputs.combustion_inputs.production_inputs.is_sunk = true;

    test_model_ptr->addDiesel(diesel_inputs);

    testFloatEquals(
        test_model_ptr->combustion_ptr_vec.size(),
        1,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->combustion_ptr_vec[0]->type,
        CombustionType :: DIESEL,
        __FILE__,
        __LINE__
    );

    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 150;

    test_model_ptr->addDiesel(diesel_inputs);

    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 250;

    test_model_ptr->addDiesel(diesel_inputs);

    testFloatEquals(
        test_model_ptr->combustion_ptr_vec.size(),
        3,
        __FILE__,
        __LINE__
    );

    std::vector<int> expected_diesel_capacity_vec_kW = {100, 150, 250};

    for (int i = 0; i < 3; i++) {
        testFloatEquals(
            test_model_ptr->combustion_ptr_vec[i]->capacity_kW,
            expected_diesel_capacity_vec_kW[i],
            __FILE__,
            __LINE__
        );
    }

    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 100;

    for (int i = 0; i < 2 * ((double)rand() / RAND_MAX); i++) {
        test_model_ptr->addDiesel(diesel_inputs);
    }
    
    return;
}   /* testAddDiesel_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddSolar_Model(
///         Model* test_model_ptr,
///         int solar_resource_key
///     )
///
/// \brief Function to test adding a solar PV array to the test Model object and then 
///     spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param solar_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddSolar_Model(
    Model* test_model_ptr,
    int solar_resource_key
)
{
    SolarInputs solar_inputs;
    solar_inputs.resource_key = solar_resource_key;

    test_model_ptr->addSolar(solar_inputs);

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec.size(),
        1,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec[0]->type,
        RenewableType :: SOLAR,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddSolar_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddSolar_productionOverride_Model(
///         Model* test_model_ptr,
///         std::string path_2_normalized_production_time_series
///     )
///
/// \brief Function to test adding a solar PV array to the test Model object using the
///     production override feature, and then spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param path_2_normalized_production_time_series A path (either relative or absolute)
///     to the given normalized production time series data.
///

void testAddSolar_productionOverride_Model(
    Model* test_model_ptr,
    std::string path_2_normalized_production_time_series
)
{
    SolarInputs solar_inputs;
    solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = 
        path_2_normalized_production_time_series;

    test_model_ptr->addSolar(solar_inputs);
    
    testFloatEquals(
        test_model_ptr->renewable_ptr_vec.size(),
        2,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model_ptr->renewable_ptr_vec[1]->type,
        RenewableType :: SOLAR,
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model_ptr->renewable_ptr_vec[1]->normalized_production_series_given,
        __FILE__,
        __LINE__
    );
    
    testTruth(
        test_model_ptr->renewable_ptr_vec[1]->path_2_normalized_production_time_series ==
        path_2_normalized_production_time_series,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddSolar_productionOverride_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddTidal_Model(
///         Model* test_model_ptr,
///         int tidal_resource_key
///     )
///
/// \brief Function to test adding a tidal turbine to the test Model object and then 
///     spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param tidal_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddTidal_Model(
    Model* test_model_ptr,
    int tidal_resource_key
)
{
    TidalInputs tidal_inputs;
    tidal_inputs.resource_key = tidal_resource_key;

    test_model_ptr->addTidal(tidal_inputs);

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec.size(),
        3,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec[2]->type,
        RenewableType :: TIDAL,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddTidal_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddWave_Model(
///         Model* test_model_ptr,
///         int wave_resource_key
///     )
///
/// \brief Function to test adding a wave energy converter to the test Model object and
///     then spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param wave_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddWave_Model(
    Model* test_model_ptr,
    int wave_resource_key
)
{
    WaveInputs wave_inputs;
    wave_inputs.resource_key = wave_resource_key;

    test_model_ptr->addWave(wave_inputs);

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec.size(),
        4,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec[3]->type,
        RenewableType :: WAVE,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddWave_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddWind_Model(
///         Model* test_model_ptr,
///         int wind_resource_key
///     )
///
/// \brief Function to test adding a wind turbine to the test Model object and then 
///     spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///
/// \param wind_resource_key A key used to index into the Resources component of the 
///     test Model object.
///

void testAddWind_Model(
    Model* test_model_ptr,
    int wind_resource_key
)
{
    WindInputs wind_inputs;
    wind_inputs.resource_key = wind_resource_key;

    test_model_ptr->addWind(wind_inputs);

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec.size(),
        5,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->renewable_ptr_vec[4]->type,
        RenewableType :: WIND,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddWind_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testAddLiIon_Model(Model* test_model_ptr)
///
/// \brief Function to test adding a lithium ion battery energy storage system to the
///     test Model object and then spot check some post-add attributes.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testAddLiIon_Model(Model* test_model_ptr)
{
    LiIonInputs liion_inputs;

    test_model_ptr->addLiIon(liion_inputs);

    testFloatEquals(
        test_model_ptr->storage_ptr_vec.size(),
        1,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_model_ptr->storage_ptr_vec[0]->type,
        StorageType :: LIION,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testAddLiIon_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testLoadBalance_Model(Model* test_model_ptr)
///
/// \brief Function to check that the post-run load data is as expected. That is, the
///     added renewable, production, and storage assets are handled by the Controller
///     as expected.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testLoadBalance_Model(Model* test_model_ptr)
{
    double load_kW = 0;

    Combustion* combustion_ptr;
    Noncombustion* noncombustion_ptr;
    Renewable* renewable_ptr;
    Storage* storage_ptr;

    for (int i = 0; i < test_model_ptr->electrical_load.n_points; i++) {
        testLessThanOrEqualTo(
            test_model_ptr->controller.net_load_vec_kW[i],
            test_model_ptr->electrical_load.max_load_kW,
            __FILE__,
            __LINE__
        );
        
        load_kW = test_model_ptr->electrical_load.load_vec_kW[i];
        
        for (size_t j = 0; j < test_model_ptr->combustion_ptr_vec.size(); j++) {
            combustion_ptr = test_model_ptr->combustion_ptr_vec[j];
            
            testGreaterThanOrEqualTo(
                combustion_ptr->production_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                combustion_ptr->dispatch_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                combustion_ptr->curtailment_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                combustion_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testFloatEquals(
                combustion_ptr->production_vec_kW[i] -
                combustion_ptr->dispatch_vec_kW[i] -
                combustion_ptr->curtailment_vec_kW[i] -
                combustion_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            load_kW -= combustion_ptr->dispatch_vec_kW[i];
        }
        
        for (size_t j = 0; j < test_model_ptr->noncombustion_ptr_vec.size(); j++) {
            noncombustion_ptr = test_model_ptr->noncombustion_ptr_vec[j];
            
            testGreaterThanOrEqualTo(
                noncombustion_ptr->production_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                noncombustion_ptr->dispatch_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                noncombustion_ptr->curtailment_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                noncombustion_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testFloatEquals(
                noncombustion_ptr->production_vec_kW[i] -
                noncombustion_ptr->dispatch_vec_kW[i] -
                noncombustion_ptr->curtailment_vec_kW[i] -
                noncombustion_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            load_kW -= noncombustion_ptr->dispatch_vec_kW[i];
        }
        
        for (size_t j = 0; j < test_model_ptr->renewable_ptr_vec.size(); j++) {
            renewable_ptr = test_model_ptr->renewable_ptr_vec[j];
            
            testGreaterThanOrEqualTo(
                renewable_ptr->production_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                renewable_ptr->dispatch_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                renewable_ptr->curtailment_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                renewable_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testFloatEquals(
                renewable_ptr->production_vec_kW[i] -
                renewable_ptr->dispatch_vec_kW[i] -
                renewable_ptr->curtailment_vec_kW[i] -
                renewable_ptr->storage_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            load_kW -= renewable_ptr->dispatch_vec_kW[i];
        }
        
        for (size_t j = 0; j < test_model_ptr->storage_ptr_vec.size(); j++) {
            storage_ptr = test_model_ptr->storage_ptr_vec[j];
            
            testGreaterThanOrEqualTo(
                storage_ptr->charging_power_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testGreaterThanOrEqualTo(
                storage_ptr->discharging_power_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
            
            testTruth(
                not (
                    storage_ptr->charging_power_vec_kW[i] > 0 and
                    storage_ptr->discharging_power_vec_kW[i] > 0
                ),
                __FILE__,
                __LINE__
            );
            
            load_kW -= storage_ptr->discharging_power_vec_kW[i];
        }
        
        testLessThanOrEqualTo(
            load_kW,
            1e-6,
            __FILE__,
            __LINE__
        );
        
        testLessThanOrEqualTo(
            test_model_ptr->controller.missed_load_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    
    return;
}   /* testLoadBalance_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testOperatingReserve_Model(Model* test_model_ptr)
///
/// \brief Function to check that the post-run state is consistent with the intended 
///     operating reserve (or "spinning reserve") logic.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testOperatingReserve_Model(Model* test_model_ptr)
{
    // DEPRECATED
    /*
    double load_kW = 0;
    double operating_reserve_kW = 0;

    Combustion* combustion_ptr;
    Noncombustion* noncombustion_ptr;
    Renewable* renewable_ptr;
    Storage* storage_ptr;

    for (int i = 0; i < test_model_ptr->electrical_load.n_points; i++) {
        //  1. compute operating reserve
        load_kW = test_model_ptr->electrical_load.load_vec_kW[i];
        
        operating_reserve_kW =
            test_model_ptr->controller.load_operating_reserve_factor * load_kW;
        
        for (size_t j = 0; j < test_model_ptr->renewable_ptr_vec.size(); j++) {
            renewable_ptr = test_model_ptr->renewable_ptr_vec[j];
            
            operating_reserve_kW += (1 - renewable_ptr->firmness_factor) *
                renewable_ptr->production_vec_kW[i];
            
            testGreaterThanOrEqualTo(
                renewable_ptr->production_vec_kW[i],
                0,
                __FILE__,
                __LINE__
            );
        }
        
        if (
            operating_reserve_kW >
                test_model_ptr->controller.max_operating_reserve_factor * load_kW
        ) {
            operating_reserve_kW =
                test_model_ptr->controller.max_operating_reserve_factor * load_kW;
        }
        
        testGreaterThanOrEqualTo(
            operating_reserve_kW,
            0,
            __FILE__,
            __LINE__
        );
        
        //  2. deduct Storage discharge from operating reserve
        for (size_t j = 0; j < test_model_ptr->storage_ptr_vec.size(); j++) {
            storage_ptr = test_model_ptr->storage_ptr_vec[j];
            
            operating_reserve_kW -= storage_ptr->discharging_power_vec_kW[i];
        }
        
        //  3. deduct Noncombustion dispatch from operating reserve
        for (size_t j = 0; j < test_model_ptr->noncombustion_ptr_vec.size(); j++) {
            noncombustion_ptr = test_model_ptr->noncombustion_ptr_vec[j];
            
            operating_reserve_kW -= noncombustion_ptr->dispatch_vec_kW[i];
        }
        
        //  4. deduct Combustion dispatch from operating reserve
        for (size_t j = 0; j < test_model_ptr->combustion_ptr_vec.size(); j++) {
            combustion_ptr = test_model_ptr->combustion_ptr_vec[j];
            
            operating_reserve_kW -= combustion_ptr->dispatch_vec_kW[i];
        }
        
        //  5. assert remaining operating reserve is zero (+/- tolerance)
        testLessThanOrEqualTo(
            operating_reserve_kW,
            1e-6,
            __FILE__,
            __LINE__
        );
    }
    */
    return;
}   /* testOperatingReserve_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEconomics_Model(Model* test_model_ptr)
///
/// \brief Function to check that the modelled economic metrics are > 0.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testEconomics_Model(Model* test_model_ptr)
{
    testGreaterThan(
        test_model_ptr->net_present_cost,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->levellized_cost_of_energy_kWh,
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testEconomics_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testFuelConsumptionEmissions_Model(Model* test_model_ptr)
///
/// \brief Function to check that the modelled fuel consumption and emissions are > 0.
///
/// \param test_model_ptr A pointer to the test Model object.
///

void testFuelConsumptionEmissions_Model(Model* test_model_ptr)
{
    testGreaterThan(
        test_model_ptr->total_fuel_consumed_L,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.CO2_kg,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.CO_kg,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.NOx_kg,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.SOx_kg,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.CH4_kg,
        0,
        __FILE__,
        __LINE__
    );

    testGreaterThan(
        test_model_ptr->total_emissions.PM_kg,
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testFuelConsumptionEmissions_Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Model");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    std::cout << std::flush;
    
    srand(time(NULL));
    
    
    std::string path_2_electrical_load_time_series =
        "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

    ModelInputs test_model_inputs;
    test_model_inputs.path_2_electrical_load_time_series =
        path_2_electrical_load_time_series;
    
    Model* test_model_ptr = testConstruct_Model(test_model_inputs);
    
    
    try {
        testBadConstruct_Model();
        testPostConstructionAttributes_Model(test_model_ptr);
        testElectricalLoadData_Model(test_model_ptr);
        
        
        int solar_resource_key = 0;
        std::string path_2_solar_resource_data =
            "data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv";
        
        testAddSolarResource_Model(
            test_model_ptr,
            path_2_solar_resource_data,
            solar_resource_key
        );
        
        
        int tidal_resource_key = 1;
        std::string path_2_tidal_resource_data =
            "data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv";
        
        testAddTidalResource_Model(
            test_model_ptr,
            path_2_tidal_resource_data,
            tidal_resource_key
        );
        
        
        int wave_resource_key = 2;
        std::string path_2_wave_resource_data =
            "data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv";
        
        testAddWaveResource_Model(
            test_model_ptr,
            path_2_wave_resource_data,
            wave_resource_key
        );
        
        
        int wind_resource_key = 3;
        std::string path_2_wind_resource_data =
            "data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv";
        
        testAddWindResource_Model(
            test_model_ptr,
            path_2_wind_resource_data,
            wind_resource_key
        );
        
        
        int hydro_resource_key = 4;
        std::string path_2_hydro_resource_data =
            "data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv";
        
        testAddHydroResource_Model(
            test_model_ptr,
            path_2_hydro_resource_data,
            hydro_resource_key
        );
        
        
        std::string path_2_normalized_production_time_series =
                "data/test/normalized_production/normalized_solar_production.csv";
        
        // looping solely for the sake of profiling (also tests reset(), which is
        // needed for wrapping PGMcpp in an optimizer)
        int n_times = 1000;
        for (int i = 0; i < n_times; i++) {
            test_model_ptr->reset();
            
            testAddHydro_Model(test_model_ptr, hydro_resource_key);
            testAddDiesel_Model(test_model_ptr);
            testAddSolar_Model(test_model_ptr, solar_resource_key);
            
            testAddSolar_productionOverride_Model(
                test_model_ptr,
                path_2_normalized_production_time_series
            );
            
            testAddTidal_Model(test_model_ptr, tidal_resource_key);
            testAddWave_Model(test_model_ptr, wave_resource_key);
            testAddWind_Model(test_model_ptr, wind_resource_key);
            
            testAddLiIon_Model(test_model_ptr);
            
            test_model_ptr->run();
        }
        
        
        testLoadBalance_Model(test_model_ptr);
        testOperatingReserve_Model(test_model_ptr);
        testEconomics_Model(test_model_ptr);
        testFuelConsumptionEmissions_Model(test_model_ptr);
        
        test_model_ptr->writeResults("test/test_results/");
    }


    catch (...) {
        delete test_model_ptr;
        
        printGold(" .................................... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_model_ptr;

    printGold(" .................................... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;
}   /* main() */

// ---------------------------------------------------------------------------------- //
