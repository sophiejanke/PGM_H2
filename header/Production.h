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
/// \file Production.h
///
/// \brief Header file for the Production class.
///


#ifndef PRODUCTION_H
#define PRODUCTION_H


#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"

#include "../Interpolator.h"


///
/// \struct ProductionInputs
///
/// \brief A structure which bundles the necessary inputs for the Production constructor.
///     Provides default values for every necessary input.
///

struct ProductionInputs {
    bool print_flag = false; ///< A flag which indicates whether or not object construct/destruction should be verbose.
    bool is_sunk = false; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
    
    double capacity_kW = 100; ///< The rated production capacity [kW] of the asset.
    
    double nominal_inflation_annual = 0.02; ///< The nominal, annual inflation rate to use in computing model economics.
    double nominal_discount_annual = 0.04; ///< The nominal, annual discount rate to use in computing model economics.
    
    double replace_running_hrs = 90000; ///< The number of running hours after which the asset must be replaced.
    
    std::string path_2_normalized_production_time_series = ""; ///< A string defining the path (either relative or absolute) to the given normalized production time series.
};


///
/// \class Production
///
/// \brief The base class of the Production hierarchy. This hierarchy contains derived
///     classes which model the production of energy, be it renewable or otherwise.
///

class Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(int, double, ProductionInputs);
        
        void __checkTimePoint(double, double);
        void __throwLengthError(void);
        void __checkNormalizedProduction(double);
        void __readNormalizedProductionData(std::vector<double>*);
        
        
    public:
        //  1. attributes
        Interpolator interpolator; ///< Interpolator component of Production.
        
        bool print_flag; ///< A flag which indicates whether or not object construct/destruction should be verbose.
        bool is_running; ///< A boolean which indicates whether or not the asset is running.
        bool is_sunk; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
        bool normalized_production_series_given; ///< A boolen which indicates whether or not a normalized production time series is given
        
        int n_points; ///< The number of points in the modelling time series.
        int n_starts; ///< The number of times the asset has been started.
        int n_replacements; ///< The number of times the asset has been replaced.
        
        double n_years; ///< The number of years being modelled.
        
        double running_hours; ///< The number of hours for which the assset has been operating.
        double replace_running_hrs; ///< The number of running hours after which the asset must be replaced.
        
        double capacity_kW; ///< The rated production capacity [kW] of the asset.
        
        double nominal_inflation_annual; ///< The nominal, annual inflation rate to use in computing model economics.
        double nominal_discount_annual; ///< The nominal, annual discount rate to use in computing model economics.
        double real_discount_annual; ///< The real, annual discount rate used in computing model economics. Is computed from the given nominal inflation and discount rates.
        double capital_cost; ///< The capital cost of the asset (undefined currency).
        double operation_maintenance_cost_kWh; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced.
        
        double net_present_cost; ///< The net present cost of this asset.
        double total_dispatch_kWh; ///< The total energy dispatched [kWh] over the Model run.
        double total_stored_kWh; ///< The total energy stored [kWh] over the Model run.
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy [1/kWh] (undefined currency) of this asset. This metric considers only dispatch.
        
        std::string type_str; ///< A string describing the type of the asset.
        std::string path_2_normalized_production_time_series; ///< A string defining the path (either relative or absolute) to the given normalized production time series.
        
        std::vector<bool> is_running_vec; ///< A boolean vector for tracking if the asset is running at a particular point in time.
        
        std::vector<double> normalized_production_vec; ///< A vector of normalizd production [ ] at each point in the modelling time series.
        std::vector<double> production_vec_kW; ///< A vector of production [kW] at each point in the modelling time series.
        std::vector<double> dispatch_vec_kW; ///< A vector of dispatch [kW] at each point in the modelling time series. Dispatch is the amount of production that is sent to the grid to satisfy load.
        std::vector<double> storage_vec_kW; ///< A vector of storage [kW] at each point in the modelling time series. Storage is the amount of production that is sent to storage.
        std::vector<double> curtailment_vec_kW; ///< A vector of curtailment [kW] at each point in the modelling time series. Curtailment is the amount of production that can be neither dispatched nor stored, and is hence curtailed.
        
        std::vector<double> capital_cost_vec; ///< A vector of capital costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        std::vector<double> operation_maintenance_cost_vec; ///< A vector of operation and maintenance costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        
        
        //  2. methods
        Production(void);
        Production(int, double, ProductionInputs, std::vector<double>*);
        virtual void handleReplacement(int);
        
        double computeRealDiscountAnnual(double, double);
        virtual void computeEconomics(std::vector<double>*);
        
        double getProductionkW(int);
        virtual double commit(int, double, double, double);
        
        virtual ~Production(void);
        
};  /* Production */


#endif  /* PRODUCTION_H */
