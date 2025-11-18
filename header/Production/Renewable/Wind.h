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
/// \file Wind.h
///
/// \brief Header file for the Wind class.
///


#ifndef WIND_H
#define WIND_H


#include "Renewable.h"


///
/// \enum WindProductionModel
///
/// \brief An enumeration of the various wind power production models supported by
///     PGMcpp.
///

enum WindPowerProductionModel {
    WIND_POWER_CUBIC, ///< A cubic power production model
    WIND_POWER_EXPONENTIAL, ///< An exponential power production model
    WIND_POWER_LOOKUP, ///< Lookup from a given set of power curve data
    N_WIND_POWER_PRODUCTION_MODELS ///< A simple hack to get the number of elements in WindPowerProductionModel
};


///
/// \struct WindInputs
///
/// \brief A structure which bundles the necessary inputs for the Wind constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct WindInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double firmness_factor = 0.5; ///< A factor [0, 1] which defines how firm the production from this asset is.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double design_speed_ms = 14; ///< The wind speed [m/s] at which the wind turbine achieves its rated capacity.
    
    WindPowerProductionModel power_model = WindPowerProductionModel :: WIND_POWER_CUBIC; ///< The wind power production model to be applied.
};


///
/// \class Wind
///
/// \brief A derived class of the Renewable branch of Production which models wind
///     production.
///

class Wind : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(WindInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        double __computeCubicProductionkW(int, double, double);
        double __computeExponentialProductionkW(int, double, double);
        double __computeLookupProductionkW(int, double, double);
        
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
        double design_speed_ms; ///< The wind speed [m/s] at which the wind turbine achieves its rated capacity.
        
        WindPowerProductionModel power_model; ///< The wind power production model to be applied.
        std::string power_model_string; ///< A string describing the active power production model.
        
        //  2. methods
        Wind(void);
        Wind(int, double, WindInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double computeProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Wind(void);
        
};  /* Wind */


#endif  /* WIND_H */
