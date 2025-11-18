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
/// \file Storage.h
///
/// \brief Header file for the Storage class.
///


#ifndef STORAGE_H
#define STORAGE_H

#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"

#include "../Interpolator.h"


///
/// \enum StorageType
///
/// \brief An enumeration of the types of Storage asset supported by PGMcpp
///

enum StorageType {
    LIION, ///< A system of lithium ion batteries.
    H2_SYS, ///< A regenerative green hydrogen energy storage system. 
    N_STORAGE_TYPES ///< A simple hack to get the number of elements in StorageType
};


///
/// \struct StorageInputs
///
/// \brief A structure which bundles the necessary inputs for the Storage constructor.
///     Provides default values for every necessary input.
///

struct StorageInputs {
    bool print_flag = false; ///< A flag which indicates whether or not object construct/destruction should be verbose.
    bool is_sunk = false; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
    
    double power_capacity_kW = 100; ///< The rated power capacity [kW] of the asset.
    double energy_capacity_kWh = 1000; ///< The rated energy capacity [kWh] of the asset.
    
    double nominal_inflation_annual = 0.02; ///< The nominal, annual inflation rate to use in computing model economics.
    double nominal_discount_annual = 0.08; ///< The nominal, annual discount rate to use in computing model economics.
};


///
/// \class Storage
///
/// \brief The base class of the Storage hierarchy. This hierarchy contains derived
///     classes which model the storage of energy.
///

class Storage {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(int, double, StorageInputs);
        
        double __computeRealDiscountAnnual(double, double);
        
        virtual void __writeSummary(std::string) {return;}
        virtual void __writeTimeSeries(std::string, std::vector<double>*, int = -1) {return;}
        
        
    public:
        //  1. attributes
        StorageType type; ///< The type (StorageType) of the asset.
        
        Interpolator interpolator; ///< Interpolator component of Storage.
        
        bool print_flag; ///< A flag which indicates whether or not object construct/destruction should be verbose.
        bool is_depleted; ///< A boolean which indicates whether or not the asset is currently considered depleted.
        bool is_sunk; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
        bool external_hydrogen_load_included; ///< A boolean which indicates if the (hydrogen) asset contains an external load component
        bool making_hydrogen_for_external_load; ///< A boolean which indicates if a (hydrogen) asset is currently servign an external load

        int n_points; ///< The number of points in the modelling time series.
        int n_replacements; ///< The number of times the asset has been replaced.
        
        double n_years; ///< The number of years being modelled.
        
        double power_capacity_kW; ///< The rated power capacity [kW] of the asset.
        double energy_capacity_kWh; ///< The rated energy capacity [kWh] of the asset.
        
        double charge_kWh; ///< The energy [kWh] stored in the asset.
        double power_kW; ///< The power [kW] currently being charged/discharged by the asset.
        
        double nominal_inflation_annual; ///< The nominal, annual inflation rate to use in computing model economics.
        double nominal_discount_annual; ///< The nominal, annual discount rate to use in computing model economics.
        double real_discount_annual; ///< The real, annual discount rate used in computing model economics. Is computed from the given nominal inflation and discount rates.
        double capital_cost; ///< The capital cost of the asset (undefined currency).
        double operation_maintenance_cost_kWh; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy charged/discharged.
        
        double net_present_cost; ///< The net present cost of this asset.
        double total_discharge_kWh; ///< The total energy discharged [kWh] over the Model run.
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy [1/kWh] (undefined currency) of this asset. This metric considers only discharge.
        
        std::string type_str; ///< A string describing the type of the asset.
        
        std::vector<double> charge_vec_kWh; ///< A vector of the charge state [kWh] at each point in the modelling time series.
        std::vector<double> charging_power_vec_kW; ///< A vector of the charging power [kW] at each point in the modelling time series.
        std::vector<double> discharging_power_vec_kW; ///< A vector of the discharging power [kW] at each point in the modelling time series.
        
        std::vector<double> capital_cost_vec; ///< A vector of capital costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        std::vector<double> operation_maintenance_cost_vec; ///< A vector of operation and maintenance costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        
        
        //  2. methods
        Storage(void);
        Storage(int, double, StorageInputs);
        virtual void handleReplacement(int);
        
        void computeEconomics(std::vector<double>*);
        
        virtual double getAvailablekW(int, double) {return 0;}
        virtual double getAcceptablekW(int, double) {return 0;}
        virtual double getMinELCapacitykW(double) {return 0;}
        virtual double getMinFCCapacitykW() {return 0;}
        virtual bool EL_minruntime(int){return 0;}
        virtual bool FC_minruntime(int){return 0;}
        virtual void commitCurtailmentHydrogen(int, double, double){return;}
        virtual void commit_SelfDischarge(int,double){return;}
        virtual double commitExternalHydrogenLoadkg(int, double){return 0;}
        
        virtual void commitCharge(int, double, double) {return;}
        virtual double commitDischarge(int, double, double, double) {return 0;}
        virtual void commitElectrolysis(int, double, double) {return;}
        virtual double commitFuelCell(int, double, double, double) {return 0;}
        virtual double getThermalOutput(int,double){return 0;}
        virtual double getMcp(int){return 0;}
        
        void writeResults(std::string, std::vector<double>*, int, int = -1);
        
        virtual ~Storage(void);
        
};  /* Storage */


#endif  /* STORAGE_H */
