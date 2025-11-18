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
/// \file Resources.h
///
/// \brief Header file for the Resources class.
///


#ifndef RESOURCES_H
#define RESOURCES_H


// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"

// components
#include "ElectricalLoad.h"

// production
#include "Production/Noncombustion/Noncombustion.h"
#include "Production/Renewable/Renewable.h"


///
/// \class Resources
///
/// \brief A class which contains renewable resource data. Intended to serve as a
///     component class of Model.
///

class Resources {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkResourceKey1D(int, RenewableType);
        void __checkResourceKey2D(int, RenewableType);
        
        void __checkResourceKey1D(int, NoncombustionType);
        
        void __checkTimePoint(double, double, std::string, ElectricalLoad*);
        void __throwLengthError(std::string, ElectricalLoad*);
        
        void __readHydroResource(std::string, int, ElectricalLoad*);
        
        void __readSolarResource(std::string, int, ElectricalLoad*);
        void __readTidalResource(std::string, int, ElectricalLoad*);
        void __readWaveResource(std::string, int, ElectricalLoad*);
        void __readWindResource(std::string, int, ElectricalLoad*);
        
        
    public:
        //  1. attributes
        std::map<int, std::vector<double>> resource_map_1D; ///< A map <int, vector<double>> of given 1D renewable resource time series.
        std::map<int, std::string> string_map_1D; ///< A map <int, string> of descriptors for the type of the given 1D renewable resource time series.
        std::map<int, std::string> path_map_1D; ///< A map <int, string> of the paths (either relative or absolute) to given 1D renewable resource time series.
        
        std::map<int, std::vector<std::vector<double>>> resource_map_2D; ///< A map <int, vector<vector<double>>> of given 2D renewable resource time series.
        std::map<int, std::string> string_map_2D; ///< A map <int, string> of descriptors for the type of the given 2D renewable resource time series.
        std::map<int, std::string> path_map_2D; ///< A map <int, string> of the paths (either relative or absolute) to given 2D renewable resource time series.
        
        
        //  2. methods
        Resources(void);
        
        void addResource(NoncombustionType, std::string, int, ElectricalLoad*);
        void addResource(RenewableType, std::string, int, ElectricalLoad*);
        
        void clear(void);
        
        ~Resources(void);
        
};  /* Resources */


#endif  /* RESOURCES_H */
