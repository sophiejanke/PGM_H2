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
/// \file ElectricalLoad.h
///
/// \brief Header file for the ElectricalLoad class.
///


#ifndef ELECTRICALLOAD_H
#define ELECTRICALLOAD_H


// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


///
/// \class ElectricalLoad
///
/// \brief A class which contains time and electrical load data. Intended to serve as
///     a component class of Model.
///

class ElectricalLoad {
    public:
        //  1. attributes
        int n_points; ///< The number of points in the modelling time series.
        
        double n_years; ///< The number of years being modelled (inferred from time_vec_hrs).
        
        double min_load_kW; ///< The minimum [kW] of the given electrical load time series.
        double mean_load_kW; ///< The mean, or average, [kW] of the given electrical load time series.
        double max_load_kW; ///< The maximum [kW] of the given electrical load time series.
        
        std::string path_2_electrical_load_time_series; ///< A string defining the path (either relative or absolute) to the given electrical load time series.
        
        std::vector<double> time_vec_hrs; ///< A vector to hold a given sequence of model times [hrs]. This defines the modelling time series.
        std::vector<double> dt_vec_hrs; ///< A vector to hold a sequence of model time deltas [hrs].
        
        std::vector<double> load_vec_kW; ///< A vector to hold a given sequence of electrical load values [kW].
        
        
        //  2. methods
        ElectricalLoad(void);
        ElectricalLoad(std::string);
        
        void readLoadData(std::string);
        void clear(void);
        
        ~ElectricalLoad(void);
        
};  /* ElectricalLoad */


#endif  /* ELECTRICALLOAD_H */
