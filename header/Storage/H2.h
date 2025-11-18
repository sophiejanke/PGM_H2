/*
 *  PGMcpp : PRIMED Grid Modelling (in C++)
 *
 *  Sophie Janke
 *  email:  sophiejanke@uvic.ca
 *  github: sophiejanke
 *
 *  See license terms
 *
 */


///
/// \file H2.h
///
/// \brief Header file for the H2 class.
///


#ifndef H2_H
#define H2_H


#include "Storage.h"
#include "Electrolyzer.h"
#include "FuelCell.h"

#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"
#include "../Interpolator.h"

#include <iostream>
#include <vector>


///
/// \struct H2Inputs
///
/// \brief A structure which bundles the necessary inputs for the H2 constructor.
///     Provides default values for every necessary input. 
///
///

struct H2Inputs {
    StorageInputs storage_inputs; ///< An encapsulated StorageInputs instance. 
    ElectrolyzerInputs electrolyzer_inputs; ///< An encapsulated ElectrolyzerInputs instance. 
    FuelCellInputs fuelcell_inputs; ///< An encapsulated FuelCellInputs instance.

    std::string path_2_external_hydrogen_load_time_series; ///< A string defining the path to the external hydrogen load csv
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy charged/discharged. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    double h2system_operation_maintenance_cost_kWh = 0.06;

    double init_SOC = 0.5; ///< The initial state of charge of the asset.
    double min_SOC = 0.01; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
    double hysteresis_SOC = 0.1; ///< The state of charge the asset must achieve to toggle is_depleted.
    double max_SOC = 1.0; ///< The maximum state of charge of the asset. 
    double replace_SOH_el = 0.9; ///< The replacement state of health fo the electrolyzer component.
    double replace_SOH_fc = 0.9; ///< The replacement state of health fo the fue cell component.
    double power_degradation_flag = false; ///< A flag which indicates whether or not power degradation should be modelled.

    double kWh_kg_conversion = 33.3; ///< kWh per kg of hydrogen 
    double fc_min_load_ratio = 0.25; ///< The fuel cell minimum load ratio.
    double el_min_load_ratio = 0.1; ///< The electrolyzer minimum load ratio.
    double fc_min_runtime = 0.1; ///< 5 minute runtime constraint 
    double el_min_runtime = 0.1; ///< 5 minute runtime constraint 
    double fc_ramp_loss = 0.0; ///< default a is no ramping limit on fuel cell
    double el_ramp_loss = 0.1; ///< default is 10% x ramping ratio; ramping from 50-100kW = 0.1*0.5 = 5% charging power lost during ramping

    double el_capacity_kW = 200; ///< The electrolyzer power capacity [kW] of the asset. 
    double el_quantity = 1; ///< The quantity of electrolyzer units (FOR FUTURE DEV)
    double el_spec_consumption_kWh = 60; ///< kWh consumed per kg produced
    double fc_capacity_kW = 100; ///< The fuel cell power capacity [kW] of the asset. 
    double fc_quantity = 1; ///< The quantity of fuel cell units (FOR FUTURE DEV)
    double fc_spec_consumption_kg = 0.055; ///< kg consumed per kWh produced.
    double h2_tank_capacity_kg = 200; ///< The hydrogen tank capacity [kg] of the asset.
    double h2_tank_cost_kg = 1200; ///< The capital cost per kg of the hydrogen storage tank.
    double compressor_spec_consumption_kWh = 2.5; ///< the specific consumption of the compression system [kWh/kg hydrogen]
    double n_compressor = 0.7; ///< The efficiency of the compressor.
    double compressor_cap_cost_kW = 2700; ///< capital cost per kW of electrolyzer of the compressor.
    bool compression_included = true; ///< indicator of if we want to include compression in system design or not.
    bool water_treatment_included = true; ///< An indicator whether or not to include water treatment considerations.
    bool external_hydrogen_load_included = false; ///< An indicator whether or not to consider an external hydrogen load.
    bool excess_hydrogen_potential_included = false; ///< An indicator whether or not to consider hydrogen production for curtailment.
    double water_treatment_cap_cost = 200; ///< The capital cost per kg of water treatement.
    double water_demand_L = 17.2; ///< Water demand per kg of hydrogen in L.

    double cp_air = 1005; ///< constant heat capacity of air [J/kg⋅K]
    double cp_el = 800; ///< constant heat capacity of the electrolyzer [J/kg⋅K]
    double cp_fc = 800; ///< constant heat capacity of the fuel cell [J/kg⋅K]
    double p_el = 2; ///< energy density of the electrolyzer in kg/kW 
    double p_fc = 2; ///< energy density of the electrolyzer in kg/kW 
    double p_air = 1.225; ///< density of air [kg/m^3]
    double v_housing = 15.94; ///< volume of a 10 ft shipping container [m^3]
};

///
/// \class H2
///
/// \brief A derived class of Storage which models energy storage by way of a regenerative green hydrogen cycle. 
///

class H2 : public Storage {
    private:
        //  1. attributes 
        
        //  2. methods - functions
        void __checkInputs(H2Inputs); 
        
        double __getGenericCapitalCost(void); // computes capital cost
        double __getGenericOpMaintCost(void); // computes operation and maintenence cost
        void __toggleDepleted(); // flags asset as depleted
        void __handleDegradation(int,double); // applies degradation modelling and update attributes 
        void __modelDegradation(int,double); // models average energy capacity degradation of full system
        
        void __writeSummary(std::string); 
        void __writeTimeSeries(std::string, std::vector<double>*, int = -1); 
        
        
    public:
        //  1. attributes, a variable belonging to the class, can be accessed from outside the class (because public)
        
        // Add an object for each sub class (el, fc, tank)        
        Electrolyzer electrolyzer;
        FuelCell fuelcell; 

        std::string path_2_external_hydrogen_load_time_series; ///< A string defining the path to the external hydrogen load csv

        double h2system_operation_maintenance_cost_kWh; ///< total hydrogen storage system O&M cost per kWh

        double dynamic_h2_energy_capacity_kWh; ///< The dynamic (i.e. degrading) energy capacity [kWh] of the asset. 
        double dynamic_h2_power_capacity_kW; ///< The dynamic (i.e. degrading) power capacity [kW] of the asset.
        
        double SOH_el; ///< electrolyzer state of health
        double SOH_fc; ///< fuel cell state of health
        double replace_SOH_el; ///< The replacement state of health fo the electrolyzer component.
        double replace_SOH_fc; ///< The replacement state of health fo the fuel cell component.
        double power_degradation_flag; ///< A flag which indicates whether or not power degradation should be modelled.
        double el_replacements; ///< Number of electrolyzer replacements.
        double fc_replacements; ///< Number of fuel cell replacements.

        double el_capacity_kW; ///< The electrolyzer power capacity [kW] of the asset. 
        double el_quantity;
        double el_spec_consumption_kWh; 
        double el_spec_consumption_kWh_initial;
        double el_output_kg;
        double fc_capacity_kW; ///< The fuel cell power capacity [kW] of the asset. 
        double fc_quantity; 
        double fc_output_kW;
        double fc_consumption_kg;
        double fc_spec_consumption_kg; 
        double fc_spec_consumption_kg_initial;
        double h2_tank_capacity_kg; ///< The hydrogen tank capacity [kg] of the asset. 
        double h2_tank_cost_kg;
        double tank_level_kg; ///< The current amount of hydrogen [kg] in the asset.
        double compressor_spec_consumption_kWh; 
        double n_compressor;
        double compressor_cap_cost_kW;
        bool compression_included;
        bool water_treatment_included;
        bool external_hydrogen_load_included;
        bool excess_hydrogen_potential_included;
        double water_treatment_cap_cost;
        double water_demand_L;

        double total_water_demand_kg; 
        double total_h2_produced_kg;
        double total_el_consumption_kWh;
        double total_curtailed_hydrogen_potential;

        double fc_runtime_hrs;
        double fc_runtime_current_hrs;
        double fc_runtime_hrs_total;
        double fc_min_runtime;
        bool fc_is_running;
        bool fc_min_runtime_enforced;
        double fc_runtime_enforced_total;

        double el_runtime_hrs;
        double el_runtime_hrs_total;
        double el_runtime_current_hrs; 
        double el_min_runtime;   
        bool el_is_running;  
        bool el_min_runtime_enforced;
        double el_runtime_enforced_total;

        bool making_hydrogen_for_external_load; 
        double total_external_hydrogen_load_met;

        double init_SOC; ///< The initial state of charge of the asset. 
        double min_SOC; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
        double hysteresis_SOC;; ///< The state of charge the asset must achieve to toggle is_depleted.
        double max_SOC;

        double n_fuelcell;
        double n_electrolyzer;
        double n_h2_overall;
        double kWh_kg_conversion;
        double fc_min_load_ratio; 
        double el_min_load_ratio;
    
        double fc_ramp_loss;
        double el_ramp_loss;

        double cp_air; ///< constant heat capacity of air [kJ/kg⋅K]
        double cp_el; ///< constant heat capacity of the electrolyzer [J/kg⋅K]
        double cp_fc; ///< constant heat capacity of the fuel cell [J/kg⋅K]
        double p_el; ///< energy density of the electrolyzer in kW/kg
        double p_fc; ///< energy density of the electrolyzer in kW/kg        
        double p_air; ///< density of air [kg/m^3]
        double v_housing; ///< volume of a 10 ft shipping container [m^3]
        
        std::vector<double> SOH_el_vec; ///< A vector of the state of health of the electrolyzer
        std::vector<double> SOH_fc_vec; ///< A vector of the state of health of the fuel cell
        std::vector<double> tank_level_vec_kg;
        std::vector<double> hydrogen_load_vec_kg;
        std::vector<double> curtailed_hydrogen_vec_kg; 
        std::vector<double> compression_power_vec_kW;
        std::vector<double> water_demand_vec_kg;
        std::vector<double> n_fuelcell_vec;
        std::vector<double> n_electrolyzer_vec;
        std::vector<double> compression_Q_vec_kW;

        //  2. methods, more functions
        H2(void);
        H2(int, double, H2Inputs); 

        void handleReplacement_el(int); // handle replacement and resetting of H2 system components
        void handleReplacement_fc(int); // handle replacement and resetting of H2 system components

        bool EL_minruntime(int); // helper method to enforce EL minimum runtime constraint
        bool FC_minruntime(int); // helper method to enforce FC minimum runtime constraint

        double getAcceptablekW(int, double); // communicates acceptable energy to controller class
        double getAvailablekW(int, double); // communicates available energy to controller class
        double getMinELCapacitykW(double); // communicates the electrolyzer minimum load ratio constraint  
        double getMinFCCapacitykW(void); // communicates the fuel cell minimum load ratio constraint
        void commitElectrolysis(int, double, double); // initiate running of el and fill tank 
        double commitFuelCell(int, double, double, double); // initiate running of fc and depleting tank

        double getThermalOutput(int,double); // get thermal energy produced by the electrolyzer, compressor, and fuel cell
        double getMcp(int); // get the total mcp term for the full storage system

        void getExternalHydrogenLoadkg(std::string); // helper method to extract external hydrogen load and store in a vector
        double commitExternalHydrogenLoadkg(int, double); // a helper method to withdraw the external hydrogen load from the hydrogen tank
        void commitCurtailmentHydrogen(int, double, double);
        double getCompressorPowerkW(double); // computes compression energy demand
        void getWaterDemand(int, double); // computes water demand

        ~H2(void);
        
};  /* H2 */


#endif  /* H2_H */
