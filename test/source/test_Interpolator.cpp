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
/// \file test_Interpolator.cpp
///
/// \brief Testing suite for Interpolator class.
///
/// A suite of tests for the Interpolator class.
///


#include "../utils/testing_utils.h"
#include "../../header/Interpolator.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator* testConstruct_Interpolator(void)
///
/// \brief A function to construct an Interpolator object.
///
/// \return A pointer to a test Interpolator object.
///

Interpolator* testConstruct_Interpolator(void)
{
    Interpolator* test_interpolator_ptr = new Interpolator();

    return test_interpolator_ptr;
}   /* testConstruct_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testDataRead1D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key,
///         std::string path_2_data_1D
///     )
///
/// \brief A function to check the 1D data values read into the Interpolator object.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_1D A key used to index into the Interpolator object.
///
/// \param path_2_data_1D A path (either relative or absolute) to the interpolation data.
///

void testDataRead1D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_1D,
    std::string path_2_data_1D
)
{
    test_interpolator_ptr->addData1D(data_key_1D, path_2_data_1D);

    testTruth(
        test_interpolator_ptr->path_map_1D[data_key_1D] == path_2_data_1D,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_1D[data_key_1D].n_points,
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_1D[data_key_1D].x_vec.size(),
        16,
        __FILE__,
        __LINE__
    );

    std::vector<double> expected_x_vec = {
        0,
        0.3,
        0.35,
        0.4,
        0.45,
        0.5,
        0.55,
        0.6,
        0.65,
        0.7,
        0.75,
        0.8,
        0.85,
        0.9,
        0.95,
        1
    };

    std::vector<double> expected_y_vec = {
        4.68079520372916,
        11.1278522361839,
        12.4787834830748,
        13.7808847600209,
        15.0417468303382,
        16.277263,
        17.4612831516442,
        18.6279054806525,
        19.7698039220515,
        20.8893499214868,
        21.955378,
        23.0690535155297,
        24.1323614374927,
        25.1797231192866,
        26.2122451458747,
        27.254952
    };

    for (int i = 0; i < test_interpolator_ptr->interp_map_1D[data_key_1D].n_points; i++) {
        testFloatEquals(
            test_interpolator_ptr->interp_map_1D[data_key_1D].x_vec[i],
            expected_x_vec[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_interpolator_ptr->interp_map_1D[data_key_1D].y_vec[i],
            expected_y_vec[i],
            __FILE__,
            __LINE__
        );
    }

    testFloatEquals(
        test_interpolator_ptr->interp_map_1D[data_key_1D].min_x,
        expected_x_vec[0],
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_1D[data_key_1D].max_x,
        expected_x_vec[expected_x_vec.size() - 1],
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testDataRead1D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadIndexing1D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key_bad
///     )
///
/// \brief A function to check if bad key errors are being handled properly.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_bad A key used to index into the Interpolator object.
///

void testBadIndexing1D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_bad
)
{
    bool error_flag = true;
    
    try {
        test_interpolator_ptr->interp1D(data_key_bad, 0);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadIndexing1D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testInvalidInterpolation1D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key_1D
///     )
///
/// \brief Function to check if attempting to interpolate outside the given 1D data
///     domain is handled properly.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_1D A key used to index into the Interpolator object.
///

void testInvalidInterpolation1D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_1D
)
{
    bool error_flag = true;
    
    try {
        test_interpolator_ptr->interp1D(data_key_1D, -1);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    try {
        test_interpolator_ptr->interp1D(data_key_1D, 2);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    try {
        test_interpolator_ptr->interp1D(data_key_1D, 0 - FLOAT_TOLERANCE);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    try {
        test_interpolator_ptr->interp1D(data_key_1D, 1 + FLOAT_TOLERANCE);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testInvalidInterpolation1D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testInterpolation1D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key_1D
///     )
///
/// \brief Function to check that the Interpolator object is returning the expected 1D
///     interpolation values.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_1D A key used to index into the Interpolator object.
///

void testInterpolation1D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_1D
)
{
    std::vector<double> interp_x_vec = {
        0,
        0.170812859791767,
        0.322739274162545,
        0.369750203682042,
        0.443532869135929,
        0.471567864244626,
        0.536513734479662,
        0.586125806988674,
        0.601101175455075,
        0.658356862575221,
        0.70576929893201,
        0.784069734739331,
        0.805765927542453,
        0.884747873186048,
        0.930870496062112,
        0.979415217694769,
        1
    };

    std::vector<double> expected_interp_y_vec = {
        4.68079520372916,
        8.35159603357656,
        11.7422361561399,
        12.9931187917615,
        14.8786636301325,
        15.5746957307243,
        17.1419229487141,
        18.3041866133728,
        18.6530540913696,
        19.9569217633299,
        21.012354614584,
        22.7142305879957,
        23.1916726441968,
        24.8602332554707,
        25.8172124624032,
        26.8256741279932,
        27.254952
    };

    for (size_t i = 0; i < interp_x_vec.size(); i++) {
        testFloatEquals(
            test_interpolator_ptr->interp1D(data_key_1D, interp_x_vec[i]),
            expected_interp_y_vec[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testInterpolation1D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testDataRead2D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key,
///         std::string path_2_data_2D
///     )
///
/// \brief A function to check the 2D data values read into the Interpolator object.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_2D A key used to index into the Interpolator object.
///
/// \param path_2_data_2D A path (either relative or absolute) to the interpolation data.
///

void testDataRead2D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_2D,
    std::string path_2_data_2D
)
{
    test_interpolator_ptr->addData2D(data_key_2D, path_2_data_2D);

    testTruth(
        test_interpolator_ptr->path_map_2D[data_key_2D] == path_2_data_2D,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].n_rows,
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].n_cols,
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].x_vec.size(),
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].y_vec.size(),
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].z_matrix.size(),
        16,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].z_matrix[0].size(),
        16,
        __FILE__,
        __LINE__
    );

    std::vector<double> expected_x_vec = {
        0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75, 5.25, 5.75, 6.25, 6.75, 7.25, 7.75
    };

    std::vector <double> expected_y_vec = {
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
        20
    };

    for (int i = 0; i < test_interpolator_ptr->interp_map_2D[data_key_2D].n_cols; i++) {
        testFloatEquals(
            test_interpolator_ptr->interp_map_2D[data_key_2D].x_vec[i],
            expected_x_vec[i],
            __FILE__,
            __LINE__
        );
    }

    for (int i = 0; i < test_interpolator_ptr->interp_map_2D[data_key_2D].n_rows; i++) {
        testFloatEquals(
            test_interpolator_ptr->interp_map_2D[data_key_2D].y_vec[i],
            expected_y_vec[i],
            __FILE__,
            __LINE__
        );
    }

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].min_x,
        expected_x_vec[0],
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].max_x,
        expected_x_vec[expected_x_vec.size() - 1],
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].min_y,
        expected_y_vec[0],
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_interpolator_ptr->interp_map_2D[data_key_2D].max_y,
        expected_y_vec[expected_y_vec.size() - 1],
        __FILE__,
        __LINE__
    );

    std::vector<std::vector<double>> expected_z_matrix = {
        {0, 0.129128125, 0.268078125, 0.404253125, 0.537653125, 0.668278125, 0.796128125, 0.921203125, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0.11160375, 0.24944375, 0.38395375, 0.51513375, 0.64298375, 0.76750375, 0.88869375, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0.094079375, 0.230809375, 0.363654375, 0.492614375, 0.617689375, 0.738879375, 0.856184375, 0.969604375, 1, 1, 1, 1, 1, 1, 1},
        {0, 0.076555, 0.212175, 0.343355, 0.470095, 0.592395, 0.710255, 0.823675, 0.932655, 1, 1, 1, 1, 1, 1, 1},
        {0, 0.059030625, 0.193540625, 0.323055625, 0.447575625, 0.567100625, 0.681630625, 0.791165625, 0.895705625, 0.995250625, 1, 1, 1, 1, 1, 1},
        {0, 0.04150625, 0.17490625, 0.30275625, 0.42505625, 0.54180625, 0.65300625, 0.75865625, 0.85875625, 0.95330625, 1, 1, 1, 1, 1, 1},
        {0, 0.023981875, 0.156271875, 0.282456875, 0.402536875, 0.516511875, 0.624381875, 0.726146875, 0.821806875, 0.911361875, 0.994811875, 1, 1, 1, 1, 1},
        {0, 0.0064575, 0.1376375, 0.2621575, 0.3800175, 0.4912175, 0.5957575, 0.6936375, 0.7848575, 0.8694175, 0.9473175, 1, 1, 1, 1, 1},
        {0, 0, 0.119003125, 0.241858125, 0.357498125, 0.465923125, 0.567133125, 0.661128125, 0.747908125, 0.827473125, 0.899823125, 0.964958125, 1, 1, 1, 1},
        {0, 0, 0.10036875, 0.22155875, 0.33497875, 0.44062875, 0.53850875, 0.62861875, 0.71095875, 0.78552875, 0.85232875, 0.91135875, 0.96261875, 1, 1, 1},
        {0, 0, 0.081734375, 0.201259375, 0.312459375, 0.415334375, 0.509884375, 0.596109375, 0.674009375, 0.743584375, 0.804834375, 0.857759375, 0.902359375, 0.938634375, 0.966584375, 0.986209375},
        {0, 0, 0.0631, 0.18096, 0.28994, 0.39004, 0.48126, 0.5636, 0.63706, 0.70164, 0.75734, 0.80416, 0.8421, 0.87116, 0.89134, 0.90264},
        {0, 0, 0.044465625, 0.160660625, 0.267420625, 0.364745625, 0.452635625, 0.531090625, 0.600110625, 0.659695625, 0.709845625, 0.750560625, 0.781840625, 0.803685624999999, 0.816095625, 0.819070625},
        {0, 0, 0.02583125, 0.14036125, 0.24490125, 0.33945125, 0.42401125, 0.49858125, 0.56316125, 0.61775125, 0.66235125, 0.69696125, 0.72158125, 0.73621125, 0.74085125, 0.73550125},
        {0, 0, 0.007196875, 0.120061875, 0.222381875, 0.314156875, 0.395386875, 0.466071875, 0.526211875, 0.575806875, 0.614856875, 0.643361875, 0.661321875, 0.668736875, 0.665606875, 0.651931875},
        {0, 0, 0, 0.0997625, 0.1998625, 0.2888625, 0.3667625, 0.4335625, 0.4892625, 0.5338625, 0.5673625, 0.5897625, 0.6010625, 0.6012625, 0.5903625, 0.5683625}
    };

    for (int i = 0; i < test_interpolator_ptr->interp_map_2D[data_key_2D].n_rows; i++) {
        for (int j = 0; j < test_interpolator_ptr->interp_map_2D[data_key_2D].n_cols; j++) {
            testFloatEquals(
                test_interpolator_ptr->interp_map_2D[data_key_2D].z_matrix[i][j],
                expected_z_matrix[i][j],
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testDataRead2D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testInvalidInterpolation2D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key_2D
///     )
///
/// \brief Function to check if attempting to interpolate outside the given 2D data
///     domain is handled properly.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_2D A key used to index into the Interpolator object.
///

void testInvalidInterpolation2D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_2D
)
{
    bool error_flag = true;
    
    try {
        test_interpolator_ptr->interp2D(data_key_2D, -1, 6);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }

    try {
        test_interpolator_ptr->interp2D(data_key_2D, 99, 6);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    try {
        test_interpolator_ptr->interp2D(data_key_2D, 0.75, -1);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }

    try {
        test_interpolator_ptr->interp2D(data_key_2D, 0.75, 99);
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testInvalidInterpolation2D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testInterpolation2D_Interpolator(
///         Interpolator* test_interpolator_ptr,
///         int data_key_2D
///     )
///
/// \brief Function to check that the Interpolator object is returning the expected 2D
///     interpolation values.
///
/// \param test_interpolator_ptr A pointer to the test Interpolator object.
///
/// \param data_key_2D A key used to index into the Interpolator object.
///

void testInterpolation2D_Interpolator(
    Interpolator* test_interpolator_ptr,
    int data_key_2D
)
{
    std::vector<double> interp_x_vec = {
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

    std::vector<double> interp_y_vec = {
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

    std::vector<std::vector<double>> expected_interp_z_matrix = {
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

    for (size_t i = 0; i < interp_y_vec.size(); i++) {
        for (size_t j = 0; j < interp_x_vec.size(); j++) {
            testFloatEquals(
                test_interpolator_ptr->interp2D(data_key_2D, interp_x_vec[j], interp_y_vec[i]),
                expected_interp_z_matrix[i][j],
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testInterpolation2D_Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\n\tTesting Interpolator");
    
    #ifdef _WIN32
        std::cout << std::endl;
    #endif
    
    srand(time(NULL));
    
    
    Interpolator* test_interpolator_ptr = testConstruct_Interpolator();
    
    
    try {
        int data_key_1D = 1;
        std::string path_2_data_1D =
            "data/test/interpolation/diesel_fuel_curve.csv";

        testDataRead1D_Interpolator(test_interpolator_ptr, data_key_1D, path_2_data_1D);
        testBadIndexing1D_Interpolator(test_interpolator_ptr, -99);
        testInvalidInterpolation1D_Interpolator(test_interpolator_ptr, data_key_1D);
        testInterpolation1D_Interpolator(test_interpolator_ptr, data_key_1D);


        int data_key_2D = 2;
        std::string path_2_data_2D =
            "data/test/interpolation/wave_energy_converter_normalized_performance_matrix.csv";

        testDataRead2D_Interpolator(test_interpolator_ptr, data_key_2D, path_2_data_2D);
        testInvalidInterpolation2D_Interpolator(test_interpolator_ptr, data_key_2D);
        testInterpolation2D_Interpolator(test_interpolator_ptr, data_key_2D);
    }


    catch (...) {
        delete test_interpolator_ptr;
        
        printGold(" ............................. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_interpolator_ptr;

    printGold(" ............................. ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;
}   /* main() */

// ---------------------------------------------------------------------------------- //
