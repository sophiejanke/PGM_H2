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
/// \file Hydro.h
///
/// \brief Header file for the Hydro class.
///


#ifndef HYDRO_H
#define HYDRO_H


#include "Noncombustion.h"


///
/// \enum HydroTurbineType
///
/// \brief An enumeration of the types of hydroelectric turbine supported by PGMcpp
///

enum HydroTurbineType {
    HYDRO_TURBINE_PELTON, ///< A Pelton turbine (impluse)
    HYDRO_TURBINE_FRANCIS, ///< A Francis turbine (reaction)
    HYDRO_TURBINE_KAPLAN, ///< A Kaplan turbine (reaction)
    N_HYDRO_TURBINES ///< A simple hack to get the number of elements in HydroTurbineType
};


///
/// \enum HydroInterpKeys
///
/// \brief An enumeration of the Interpolator keys used by the Hydro asset
///

enum HydroInterpKeys {
    GENERATOR_EFFICIENCY_INTERP_KEY, ///< The key for generator efficiency interpolation
    TURBINE_EFFICIENCY_INTERP_KEY, ///< The key for turbine efficiency interpolation
    FLOW_TO_POWER_INTERP_KEY, ///< The key for flow to power interpolation
    N_HYDRO_INTERP_KEYS ///< A simple hack to get the number of elements in HydroInterpKeys
};


///
/// \struct HydroInputs
///
/// \brief A structure which bundles the necessary inputs for the Hydro constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates NoncombustionInputs.
///

struct HydroInputs {
    NoncombustionInputs noncombustion_inputs; ///< An encapsulated NoncombustionInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double fluid_density_kgm3 = 1000; ///< The density [kg/m3] of the hydroelectric working fluid.
    double net_head_m = 500; ///< The net head [m] of the asset.
    
    double reservoir_capacity_m3 = 0; ///< The capacity [m3] of the hydro reservoir.
    double init_reservoir_state = 0; ///< The initial state of the reservoir (where state is volume of stored fluid divided by capacity).
    
    HydroTurbineType turbine_type = HydroTurbineType :: HYDRO_TURBINE_PELTON; ///< The type of hydroelectric turbine model to use.
};


///
/// \class Hydro
///
/// \brief A derived class of the Noncombustion branch of Production which models
///     production using a hydroelectric asset (either with reservoir or not).
///

class Hydro : public Noncombustion {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(HydroInputs);
        
        void __initInterpolator(void);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        double __getEfficiencyFactor(double);
        
        double __getMinimumFlowm3hr(void);
        double __getMaximumFlowm3hr(void);
        
        double __flowToPower(double);
        double __powerToFlow(double);
        
        double __getAvailableFlow(double, double);
        double __getAcceptableFlow(double);
        
        void __updateState(int, double, double, double);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            int = -1
        );
    
    
    public:
        //  1. attributes
        HydroTurbineType turbine_type; ///< The type of hydroelectric turbine model to use.
        
        double fluid_density_kgm3; ///< The density [kg/m3] of the hydroelectric working fluid.
        double net_head_m; ///< The net head [m] of the asset.
        
        double reservoir_capacity_m3; ///< The capacity [m3] of the hydro reservoir.
        double init_reservoir_state; ///< The initial state of the reservoir (where state is volume of stored fluid divided by capacity).
        double stored_volume_m3; ///< The volume [m3] of stored fluid.
        
        double minimum_power_kW; ///< The minimum power [kW] that the asset can produce. Corresponds to minimum productive flow.
        
        double minimum_flow_m3hr; ///< The minimum required flow [m3/hr] for the asset to produce. Corresponds to minimum power.
        double maximum_flow_m3hr; ///< The maximum productive flow [m3/hr] that the asset can support.
        
        std::vector<double> turbine_flow_vec_m3hr; ///< A vector of the turbine flow [m3/hr] at each point in the modelling time series.
        std::vector<double> spill_rate_vec_m3hr; ///< A vector of the spill rate [m3/hr] at each point in the modelling time series.
        std::vector<double> stored_volume_vec_m3; ///< A vector of the stored volume [m3] in the reservoir at each point in the modelling time series.
        
        
        //  2. methods
        Hydro(void);
        Hydro (int, double, HydroInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double requestProductionkW(int, double, double, double);
        double commit(int, double, double, double, double);
        
        ~Hydro(void);
        
        
};  /* Hydro */


#endif  /* HYDRO_H */
