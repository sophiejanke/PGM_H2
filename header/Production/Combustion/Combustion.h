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
/// \file Combustion.h
///
/// \brief Header file for the Combustion class.
///


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../Production.h"


///
/// \enum CombustionType
///
/// \brief An enumeration of the types of Combustion asset supported by PGMcpp
///

enum CombustionType {
    DIESEL, ///< A diesel generator.
    N_COMBUSTION_TYPES ///< A simple hack to get the number of elements in CombustionType
};


///
/// \enum FuelMode
///
/// \brief An enumeration of the fuel modes for the Combustion asset which are
///     supported by PGMcpp
///

enum FuelMode {
    FUEL_MODE_LINEAR, ///< A linearized fuel curve model (i.e., HOMER-like model)
    FUEL_MODE_LOOKUP, ///< Interpolating over a given fuel lookup table
    N_FUEL_MODES ///< A simple hack to get the number of elements in FuelMode
};


///
/// \struct CombustionInputs
///
/// \brief A structure which bundles the necessary inputs for the Combustion constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates ProductionInputs.
///

struct CombustionInputs {
    ProductionInputs production_inputs; ///< An encapsulated ProductionInputs instance.
    
    FuelMode fuel_mode = FuelMode :: FUEL_MODE_LINEAR; ///< The fuel mode to use in modelling fuel consumption.
    
    double nominal_fuel_escalation_annual = 0.05; ///< The nominal, annual fuel escalation rate to use in computing model economics.
    double cycle_charging_setpoint = 0.85; ///< The cycle charging set point (the load ratio at which to produce when running in cycle charging mode).
    
    std::string path_2_fuel_interp_data = ""; ///< A path (either relative or absolute) to a set of fuel consumption data.
};


///
/// \struct Emissions
///
/// \brief A structure which bundles the emitted masses of various emissions chemistries.
///

struct Emissions {
    double CO2_kg = 0; ///< The mass of carbon dioxide (CO2) emitted [kg].
    double CO_kg = 0; ///< The mass of carbon monoxide (CO) emitted [kg].
    double NOx_kg = 0; ///< The mass of nitrogen oxides (NOx) emitted [kg].
    double SOx_kg = 0; ///< The mass of sulfur oxides (SOx) emitted [kg].
    double CH4_kg = 0; ///< The mass of methane (CH4) emitted [kg].
    double PM_kg = 0; ///< The mass of particulate matter (PM) emitted [kg].
};


///
/// \class Combustion
///
/// \brief The root of the Combustion branch of the Production hierarchy. This branch 
///     contains derived classes which model the production of energy by way of
///     combustibles.
///

class Combustion : public Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(CombustionInputs);
        
        virtual void __writeSummary(std::string) {return;}
        virtual void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            int = -1
        ) {return;}
        
        
    public:
        //  1. attributes
        CombustionType type; ///< The type (CombustionType) of the asset.
        FuelMode fuel_mode; ///< The fuel mode to use in modelling fuel consumption.
        Emissions total_emissions; ///< An Emissions structure for holding total emissions [kg].
        
        double fuel_cost_L; ///< The cost of fuel [1/L] (undefined currency).
        double nominal_fuel_escalation_annual; ///< The nominal, annual fuel escalation rate to use in computing model economics.
        double real_fuel_escalation_annual; ///< The real, annual fuel escalation rate used in computing model economics. Is computed from the given nominal inflation and discount rates.
        
        double linear_fuel_slope_LkWh; ///< The slope [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced.
        double linear_fuel_intercept_LkWh; ///< The intercept [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced.
        
        double cycle_charging_setpoint; ///< The cycle charging set point (the load ratio at which to produce when running in cycle charging mode).
        
        double CO2_emissions_intensity_kgL; ///< Carbon dioxide (CO2) emissions intensity [kg/L].
        double CO_emissions_intensity_kgL; ///< Carbon monoxide (CO) emissions intensity [kg/L].
        double NOx_emissions_intensity_kgL; ///< Nitrogen oxide (NOx) emissions intensity [kg/L].
        double SOx_emissions_intensity_kgL; ///< Sulfur oxide (SOx) emissions intensity [kg/L].
        double CH4_emissions_intensity_kgL; ///< Methane (CH4) emissions intensity [kg/L].
        double PM_emissions_intensity_kgL; ///< Particulate Matter (PM) emissions intensity [kg/L].
        
        double total_fuel_consumed_L; ///< The total fuel consumed [L] over a model run.
        
        std::string fuel_mode_str; ///< A string describing the fuel mode of the asset.
    
        std::vector<double> fuel_consumption_vec_L; ///< A vector of fuel consumed [L] over each modelling time step.
        std::vector<double> fuel_cost_vec; ///< A vector of fuel costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        
        std::vector<double> CO2_emissions_vec_kg; ///< A vector of carbon dioxide (CO2) emitted [kg] over each modelling time step.
        std::vector<double> CO_emissions_vec_kg; ///< A vector of carbon monoxide (CO) emitted [kg] over each modelling time step.
        std::vector<double> NOx_emissions_vec_kg; ///< A vector of nitrogen oxide (NOx) emitted [kg] over each modelling time step.
        std::vector<double> SOx_emissions_vec_kg; ///< A vector of sulfur oxide (SOx) emitted [kg] over each modelling time step.
        std::vector<double> CH4_emissions_vec_kg; ///< A vector of methane (CH4) emitted [kg] over each modelling time step.
        std::vector<double> PM_emissions_vec_kg; ///< A vector of particulate matter (PM) emitted [kg] over each modelling time step.
        
        
        //  2. methods
        Combustion(void);
        Combustion(int, double, CombustionInputs, std::vector<double>*);
        virtual void handleReplacement(int);
        
        void computeFuelAndEmissions(void);
        void computeEconomics(std::vector<double>*);
        
        virtual double requestProductionkW(int, double, double) {return 0;}
        virtual double commit(int, double, double, double);
        
        double getFuelConsumptionL(double, double);
        Emissions getEmissionskg(double);
        
        void writeResults(
            std::string,
            std::vector<double>*,
            int,
            int = -1
        );
        
        virtual ~Combustion(void);
        
};  /* Combustion */


#endif  /* COMBUSTION_H */
