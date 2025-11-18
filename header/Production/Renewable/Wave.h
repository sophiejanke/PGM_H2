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
/// \file Wave.h
///
/// \brief Header file for the Wave class.
///


#ifndef WAVE_H
#define WAVE_H


#include "Renewable.h"


///
/// \enum WaveProductionModel
///
/// \brief An enumeration of the various wave power production models supported by
///     PGMcpp.
///

enum WavePowerProductionModel {
    WAVE_POWER_GAUSSIAN, ///< A Gaussian power production model
    WAVE_POWER_PARABOLOID, ///< A paraboloid power production model
    WAVE_POWER_LOOKUP, ///< Lookup from a given performance matrix
    N_WAVE_POWER_PRODUCTION_MODELS ///< A simple hack to get the number of elements in WavePowerProductionModel
};


///
/// \struct WaveInputs
///
/// \brief A structure which bundles the necessary inputs for the Wave constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct WaveInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double firmness_factor = 0.8; ///< A factor [0, 1] which defines how firm the production from this asset is.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double design_significant_wave_height_m = 3; ///< The significant wave height [m] at which the wave energy converter achieves its rated capacity.
    double design_energy_period_s = 10; ///< The energy period [s] at which the wave energy converter achieves its rated capacity.
    
    WavePowerProductionModel power_model = WavePowerProductionModel :: WAVE_POWER_PARABOLOID; ///< The wave power production model to be applied.
    
    std::string path_2_normalized_performance_matrix = ""; ///< A path (either relative or absolute) to a normalized performance matrix for the wave energy converter.
};


///
/// \class Wave
///
/// \brief A derived class of the Renewable branch of Production which models wave
///     production.
///

class Wave : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(WaveInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        double __computeGaussianProductionkW(int, double, double, double);
        double __computeParaboloidProductionkW(int, double, double, double);
        double __computeLookupProductionkW(int, double, double, double);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            std::map<int, std::vector<double>>*,
            std::map<int, std::vector<std::vector<double>>>*,
            int = -1
        );
        
        
    public:
        //  1. attributes
        double design_significant_wave_height_m; ///< The significant wave height [m] at which the wave energy converter achieves its rated capacity.
        double design_energy_period_s; ///< The energy period [s] at which the wave energy converter achieves its rated capacity.
        
        WavePowerProductionModel power_model; ///< The wave power production model to be applied.
        std::string power_model_string; ///< A string describing the active power production model.
        
        
        //  2. methods
        Wave(void);
        Wave(int, double, WaveInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double computeProductionkW(int, double, double, double);
        double commit(int, double, double, double);
        
        ~Wave(void);
        
};  /* Wave */


#endif  /* WAVE_H */
