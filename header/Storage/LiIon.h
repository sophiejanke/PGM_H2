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
/// \file LiIon.h
///
/// \brief Header file for the LiIon class.
///


#ifndef LIION_H
#define LIION_H


#include "Storage.h"


///
/// \struct LiIonInputs
///
/// \brief A structure which bundles the necessary inputs for the LiIon constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates StorageInputs.
///
/// Ref: \cite BatteryDegradation_2023\n
///

struct LiIonInputs {
    StorageInputs storage_inputs; ///< An encapsulated StorageInputs instance.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy charged/discharged. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double init_SOC = 0.5; ///< The initial state of charge of the asset.
    
    double min_SOC = 0.15; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
    double hysteresis_SOC = 0.5; ///< The state of charge the asset must achieve to toggle is_depleted.
    double max_SOC = 0.9; ///< The maximum state of charge of the asset.
    
    double charging_efficiency = 0.9; ///< The charging efficiency of the asset.
    double discharging_efficiency = 0.9; ///< The discharging efficiency of the asset.
    
    double replace_SOH = 0.8; ///< The state of health at which the asset is considered "dead" and must be replaced.
    bool power_degradation_flag = false; ///< A flag which indicates whether or not power degradation should be modelled.
    
    double degradation_alpha = 8.935; ///< A dimensionless acceleration coefficient used in modelling energy capacity degradation.
    double degradation_beta = 1; ///< A dimensionless acceleration exponent used in modelling energy capacity degradation.
    double degradation_B_hat_cal_0 = 5.22226e6; ///< A reference (or base) pre-exponential factor [1/sqrt(hrs)] used in modelling energy capacity degradation.
    double degradation_r_cal = 0.4361; ///< A dimensionless constant used in modelling energy capacity degradation.
    double degradation_Ea_cal_0 = 5.279e4; ///< A reference (or base) activation energy [J/mol] used in modelling energy capacity degradation.
    double degradation_a_cal = 100; ///< A pre-exponential factor [J/mol] used in modelling energy capacity degradation.
    double degradation_s_cal = 2; ///< A dimensionless constant used in modelling energy capacity degradation.
    double gas_constant_JmolK = 8.31446; ///< The universal gas constant [J/mol.K].
    double temperature_K = 273 + 20; ///< The absolute environmental temperature [K] of the lithium ion battery energy storage system.

    double p_liion = 3.3; ///< energy density of the electrolyzer in kg/kWh
    double cp_liion = 800; ///< constant heat capacity of the liion [J/kg⋅K]
    double self_discharge_rate = 1.5; ///< monthly self discharge rate [% charge lost per month]
};


///
/// \class LiIon
///
/// \brief A derived class of Storage which models energy storage by way of lithium-ion
///     batteries.
///

class LiIon : public Storage {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(LiIonInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        void __toggleDepleted();
        
        void __handleDegradation(int, double, double);
        void __modelDegradation(double, double);
        
        double __getBcal(double);
        double __getEacal(double);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(std::string, std::vector<double>*, int = -1);
        
        
    public:
        //  1. attributes
        bool power_degradation_flag; ///< A flag which indicates whether or not power degradation should be modelled.
        
        double dynamic_energy_capacity_kWh; ///< The dynamic (i.e. degrading) energy capacity [kWh] of the asset.
        double dynamic_power_capacity_kW; ///< The dynamic (i.e. degrading) power capacity [kW] of the asset.
        
        double SOH; ///< The state of health of the asset.
        double replace_SOH; ///< The state of health at which the asset is considered "dead" and must be replaced.
        
        double degradation_alpha; ///< A dimensionless acceleration coefficient used in modelling energy capacity degradation.
        double degradation_beta; ///< A dimensionless acceleration exponent used in modelling energy capacity degradation.
        double degradation_B_hat_cal_0; ///< A reference (or base) pre-exponential factor [1/sqrt(hrs)] used in modelling energy capacity degradation.
        double degradation_r_cal; ///< A dimensionless constant used in modelling energy capacity degradation.
        double degradation_Ea_cal_0; ///< A reference (or base) activation energy [J/mol] used in modelling energy capacity degradation.
        double degradation_a_cal; ///< A pre-exponential factor [J/mol] used in modelling energy capacity degradation.
        double degradation_s_cal; ///< A dimensionless constant used in modelling energy capacity degradation.
        double gas_constant_JmolK; ///< The universal gas constant [J/mol.K].
        double temperature_K; ///< The absolute environmental temperature [K] of the lithium ion battery energy storage system.
        
        double init_SOC; ///< The initial state of charge of the asset.
    
        double min_SOC; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
        double hysteresis_SOC; ///< The state of charge the asset must achieve to toggle is_depleted.
        double max_SOC; ///< The maximum state of charge of the asset.

        double p_liion; ///< energy density of the electrolyzer in kg/kWh
        double cp_liion; ///< constant heat capacity of the liion [J/kg⋅K]
        double self_discharge_rate; ///< monthly self discharge rate [% charge lost per month]
        
        double charging_efficiency; ///< The charging efficiency of the asset.
        double discharging_efficiency; ///< The discharging efficiency of the asset.
        
        std::vector<double> SOH_vec; ///< A vector of the state of health of the asset at each point in the modelling time series.
        
        //  2. methods
        LiIon(void);
        LiIon(int, double, LiIonInputs);
        void handleReplacement(int);
        
        double getAvailablekW(int, double);
        double getAcceptablekW(int, double);
        
        void commitCharge(int, double, double);
        double commitDischarge(int, double, double, double);

        double getThermalOutput(int,double);
        double getMcp(int);
        void commit_SelfDischarge(int,double);
        
        ~LiIon(void);
        
};  /* LiIon */


#endif  /* LIION_H */
