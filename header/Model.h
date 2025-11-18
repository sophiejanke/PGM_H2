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
/// \file Model.h
///
/// \brief Header file for the Model class.
///


#ifndef MODEL_H
#define MODEL_H


//  components
#include "Controller.h"
#include "ElectricalLoad.h"
#include "Resources.h"
#include "ThermalModel.h"

//  production
#include "Production/Combustion/Diesel.h"
#include "Production/Noncombustion/Hydro.h"
#include "Production/Renewable/Solar.h"
#include "Production/Renewable/Tidal.h"
#include "Production/Renewable/Wave.h"
#include "Production/Renewable/Wind.h"

// storage
#include "Storage/LiIon.h"
#include "Storage/H2.h"
#include "Storage/Electrolyzer.h"
#include "Storage/FuelCell.h"
// #include "Storage/Temperature.h"


///
/// \struct ModelInputs
///
/// \brief A structure which bundles the necessary inputs for the Model constructor.
///     Provides default values for every necessary input (except
///     path_2_electrical_load_time_series, for which a valid input must be provided).
///

struct ModelInputs {
    std::string path_2_electrical_load_time_series = ""; ///< A string defining the path (either relative or absolute) to the given electrical load time series.
    
    ControlMode control_mode = ControlMode :: LOAD_FOLLOWING; ///< The control mode to be applied by the Controller object.
    
    // **** DEPRECATED ****
    /*
    double load_operating_reserve_factor = 0.2; //< An operating reserve factor [0, 1] to cover random fluctuations in load.
    double max_operating_reserve_factor = 1; //< A maximum reserve factor [0, 1] that limits the required overall operating reserve to, at most, factor * load_kW.
    */
    
    double firm_dispatch_ratio = 0.1; ///< The ratio [0, 1] of the load in each time step that must be dispatched from firm assets.
    double load_reserve_ratio = 0.1; ///< The ratio [0, 1] of the load in each time step that must be included in the required spinning reserve.
};


///
/// \class Model
///
/// \brief A container class which forms the centre of PGMcpp. The Model class is
///     intended to serve as the primary user interface with the functionality of
///     PGMcpp, and as such it contains all other classes.
///

class Model {
    private:
        //  1. attributes
        void __checkInputs(ModelInputs);
        
        void __computeFuelAndEmissions(void);
        void __computeNetPresentCost(void);
        void __computeLevellizedCostOfEnergy(void);
        void __computeEconomics(void);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(std::string, int = -1);
        
        
        //  2. methods
        //...
        
        
    public:
        //  1. attributes
        double total_fuel_consumed_L; ///< The total fuel consumed [L] over a model run.
        Emissions total_emissions; ///< An Emissions structure for holding total emissions [kg].
        
        double net_present_cost; ///< The net present cost of the Model (undefined currency).
        
        double total_renewable_noncombustion_dispatch_kWh; ///< The total energy dispatched [kWh] by all renewable and non-combustion assets over the Model run.
        double total_renewable_noncombustion_charge_kWh; ///< The total energy stored [kWh] from all renewable and non-combustion assets over the Model run.
        double total_combustion_charge_kWh; ///< The total energy stored [kWh] from all combustion assets over the Model run.
        double total_discharge_kWh; ///< The total energy discharged [kWh] from all storage assets over the Model run.
        double total_dispatch_kWh; ///< The total energy dispatched [kWh] from all production assets over the Model run.
        double renewable_penetration; ///< The total renewable (i.e. non-combustion) penetration over the Model run.
        
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy, per unit energy dispatched/discharged, of the Model [1/kWh] (undefined currency).
        
        Controller controller; ///< Controller component of Model
        ElectricalLoad electrical_load; ///< ElectricalLoad component of Model
        Resources resources; ///< Resources component of Model
        
        ThermalModel* thermal_model_ptr; ///< Pointer to Thermal Model
        
        std::vector<Combustion*> combustion_ptr_vec; ///< A vector of pointers to the various Combustion assets in the Model
        std::vector<Noncombustion*> noncombustion_ptr_vec; ///< A vector of pointers to the various Noncombustion assets in the Model
        std::vector<Renewable*> renewable_ptr_vec;  ///< A vector of pointers to the various Renewable assets in the Model
        std::vector<Storage*> storage_ptr_vec;  ///< A vector of pointers to the various Storage assets in the Model
        
        
        
        //  2. methods
        Model(void);
        Model(ModelInputs);
        
        void addDiesel(DieselInputs);
        
        void addResource(NoncombustionType, std::string, int);
        void addResource(RenewableType, std::string, int);
        
        void addHydro(HydroInputs);
        
        void addSolar(SolarInputs);
        void addTidal(TidalInputs);
        void addWave(WaveInputs);
        void addWind(WindInputs);
        
        void addLiIon(LiIonInputs);
        void addH2(H2Inputs);
        void addThermalModel(ThermalModelInputs);
        
        void run(void);
        
        void reset(void);
        void clear(void);
        
        void writeResults(std::string, int = -1);
        
        ~Model(void);
        
};  /* Model */


#endif  /* Model_H */
