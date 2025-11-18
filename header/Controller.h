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
/// \file Controller.h
///
/// \brief Header file for the Controller class.
///


#ifndef CONTROLLER_H
#define CONTROLLER_H

// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"

//  components
#include "ElectricalLoad.h"
#include "Resources.h"
#include "ThermalModel.h"

// production and storage
#include "Production/Combustion/Combustion.h"
#include "Production/Combustion/Diesel.h"
#include "Production/Noncombustion/Noncombustion.h"
#include "Production/Renewable/Renewable.h"
#include "Storage/Storage.h"


///
/// \enum ControlMode
///
/// \brief An enumeration of the types of control modes supported by PGMcpp
///

enum ControlMode {
    LOAD_FOLLOWING, ///< Load following control, with in-order dispatch of non-Combustion assets and optimal dispatch of Combustion assets.
    CYCLE_CHARGING, ///< Cycle charging control, with in-order dispatch of non-Combustion assets and optimal dispatch of Combustion assets.
    N_CONTROL_MODES ///< A simple hack to get the number of elements in ControlMode
};


///
/// \struct LoadStruct
///
/// \brief A structure for holding various inputs/outputs for the Controller
//      __handle*() methods.
///

struct LoadStruct {
    double load_kW = 0;    ///< The load [kW] remaining for the time step.
    double total_renewable_production_kW = 0;   ///< The total production [kW] from all Renewable assets for the time step.
    double required_firm_dispatch_kW = 0; ///< The firm dispatch requirement [kW] remaining for the time step.
    double required_spinning_reserve_kW = 0; ///< The spinning reserve requirement [kW] remaining for the time step.
};


///
/// \class Controller
///
/// \brief A class which contains a various dispatch control logic. Intended to serve
///     as a component class of Model.
///

class Controller {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __computeRenewableProduction(
            ElectricalLoad*,
            std::vector<Renewable*>*,
            Resources*
        );
        
        void __constructCombustionMap(std::vector<Combustion*>*);
        
        double __getRenewableProduction(int, double, Renewable*, Resources*);
        
        // **** DEPRECATED ****
        /*
        double __handleStorageDischarging(int, double, double, std::vector<Storage*>*);
        
        double __handleNoncombustionDispatch(
            int,
            double,
            double,
            std::vector<Noncombustion*>*,
            Resources*
        );
        
        double __handleCombustionDispatch(
            int,
            double,
            double,
            double,
            double,
            double,
            std::vector<Combustion*>*,
            bool
        );
        */
        
        LoadStruct __handleStorageDischarging(
            int,
            double,
            LoadStruct,
            std::vector<Storage*>*
        );
        
        LoadStruct __handleNoncombustionDispatch(
            int,
            double,
            LoadStruct,
            std::vector<Noncombustion*>*,
            Resources*
        );
        
        LoadStruct __handleCombustionDispatch(
            int,
            double,
            LoadStruct,
            std::vector<Combustion*>*,
            bool
        );
        
        double __handleRenewableDispatch(
            int,
            double,
            double,
            std::vector<Renewable*>*
        );
        
        void __handleStorageCharging(
            int,
            double,
            std::vector<Storage*>*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*
        );

        void __handleThermalTracking(
            int,
            double,
            std::vector<Storage*>*,
            ThermalModel*
        );
        
        
    public:
        //  1. attributes
        ControlMode control_mode; ///< The ControlMode that is active in the Model.
        std::string control_string; ///< A string describing the active ControlMode.
        
        // **** DEPRECATED ****
        /*
        double load_operating_reserve_factor; //< An operating reserve factor [0, 1] to cover random fluctuations in load.
        double max_operating_reserve_factor; //< A maximum reserve factor [0, 1] that limits the required overall operating reserve to, at most, factor * load_kW.
        double required_operating_reserve_kW; //< A required operating reserve [kW], to absorb load and Renewable production fluctuations.
        */
        
        double firm_dispatch_ratio; ///< The ratio [0, 1] of the load in each time step that must be dispatched from firm assets.
        double load_reserve_ratio; ///< The ratio [0, 1] of the load in each time step that must be included in the required spinning reserve.
        
        std::vector<bool> storage_discharge_bool_vec; ///< A boolean vector attribute to track which Storage assets have been discharged in each time step.
        
        std::vector<double> net_load_vec_kW; ///< A vector of net load values [kW] at each point in the modelling time series. Net load is defined as load minus all available Renewable production.
        std::vector<double> missed_load_vec_kW; ///< A vector of missed load values [kW] at each point in the modelling time series.
        std::vector<double> missed_firm_dispatch_vec_kW; ///< A vector of missed firm dispatch values [kW] at each point in the modelling time series.
        std::vector<double> missed_spinning_reserve_vec_kW; ///< A vector of missed spinning reserve values [kw] at each point in the modelling time series.
        
        std::map<double, std::vector<bool>> combustion_map; ///< A map of all possible combustion states, for use in determining optimal dispatch. 
        
        
        //  2. methods
        Controller(void);
        
        void setControlMode(ControlMode);
        
        // **** DEPRECATED ****
        /*
        void setLoadOperatingReserveFactor(double);
        void setMaxOperatingReserveFactor(double);
        */
        void setFirmDispatchRatio(double);
        void setLoadReserveRatio(double);
        
        void init(
            ElectricalLoad*,
            std::vector<Renewable*>*,
            Resources*,
            std::vector<Combustion*>*
        );
        
        void applyDispatchControl(
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*,
            ThermalModel*
        );
        
        void clear(void);
        
        ~Controller(void);
        
};  /* Controller */


#endif  /* CONTROLLER_H */
