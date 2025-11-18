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
/// \file Diesel.h
///
/// \brief Header file for the Diesel class.
///


#ifndef DIESEL_H
#define DIESEL_H


#include "Combustion.h"


///
/// \struct DieselInputs
///
/// \brief A structure which bundles the necessary inputs for the Diesel constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates CombustionInputs.
///
/// Ref: \cite HOMER_fuel_curve\n
/// Ref: \cite HOMER_generator_fuel_curve_intercept_coefficient\n
/// Ref: \cite HOMER_generator_fuel_curve_slope\n
/// Ref: \cite DieselEmissions_2014\n
/// Ref: \cite DieselEmissions_2008\n
///

struct DieselInputs {
    CombustionInputs combustion_inputs; ///< An encapsulated CombustionInputs instance.
    
    double replace_running_hrs = 30000; ///< The number of running hours after which the asset must be replaced. Overwrites the ProductionInputs attribute.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    double fuel_cost_L = 1.70; ///< The cost of fuel [1/L] (undefined currency).
    
    double minimum_load_ratio = 0.4; ///< The minimum load ratio of the asset. That is, when the asset is producing, it must produce at least this ratio of its rated capacity.
    double minimum_runtime_hrs = 4; ///< The minimum runtime [hrs] of the asset. This is the minimum time that must elapse between successive starts and stops.
    
    double linear_fuel_slope_LkWh = -1; ///< The slope [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    double linear_fuel_intercept_LkWh = -1; ///< The intercept [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    
    double CO2_emissions_intensity_kgL = 2.7; ///< Carbon dioxide (CO2) emissions intensity [kg/L].
    double CO_emissions_intensity_kgL = 0.0178; ///< Carbon monoxide (CO) emissions intensity [kg/L].
    double NOx_emissions_intensity_kgL = 0.0014; ///< Nitrogen oxide (NOx) emissions intensity [kg/L].
    double SOx_emissions_intensity_kgL = 0.0042; ///< Sulfur oxide (SOx) emissions intensity [kg/L].
    double CH4_emissions_intensity_kgL = 0.0007; ///< Methane (CH4) emissions intensity [kg/L].
    double PM_emissions_intensity_kgL = 0.0001; ///< Particulate Matter (PM) emissions intensity [kg/L].
};


///
/// \class Diesel
///
/// \brief A derived class of the Combustion branch of Production which models
///     production using a diesel generator.
///

class Diesel : public Combustion {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(DieselInputs);
        void __handleStartStop(int, double, double);
        
        double __getGenericFuelSlope(void);
        double __getGenericFuelIntercept(void);
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            int = -1
        );
        
        
    public:
        //  1. attributes
        double minimum_load_ratio; ///< The minimum load ratio of the asset. That is, when the asset is producing, it must produce at least this ratio of its rated capacity.
        double minimum_runtime_hrs; ///< The minimum runtime [hrs] of the asset. This is the minimum time that must elapse between successive starts and stops.
        double time_since_last_start_hrs; ///< The time that has elapsed [hrs] since the last start of the asset.
        
        
        //  2. methods
        Diesel(void);
        Diesel (int, double, DieselInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double requestProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Diesel(void);
        
};  /* Diesel */


#endif  /* DIESEL_H */
