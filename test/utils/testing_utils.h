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
/// \file testing_utils.h
///
/// \brief Header file for various PGMcpp testing utilities.
///
/// This is a library of utility functions used throughout the various test suites.
///


#ifndef TESTING_UTILS_H
#define TESTING_UTILS_H


#include "../../header/std_includes.h"

#ifndef FLOAT_TOLERANCE
    ///
    /// \def FLOAT_TOLERANCE
    ///
    /// \brief A tolerance for application to floating point equality tests.
    ///
    
    #define FLOAT_TOLERANCE 1e-6
#endif  /* FLOAT_TOLERANCE */


#ifdef _WIN32
    void activateVirtualTerminal(void);
#endif  /* _WIN32 */


void printGreen(std::string);
void printGold(std::string);
void printRed(std::string);

void testFloatEquals(double, double, std::string, int);
void testGreaterThan(double, double, std::string, int);
void testGreaterThanOrEqualTo(double, double, std::string, int);
void testLessThan(double, double, std::string, int);
void testLessThanOrEqualTo(double, double, std::string, int);
void testTruth(bool, std::string, int);

void expectedErrorNotDetected(std::string, int);


#endif  /* TESTING_UTILS_H */
