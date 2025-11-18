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
/// \file Controller.cpp
///
/// \brief Implementation file for the Controller class.
///
/// A class which contains a various dispatch control logic. Intended to serve as a
/// component class of Controller.
///

#include "../header/Controller.h"

// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __computeRenewableProduction(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to compute and record Renewable production, net load.
///
/// The net load at a given point in time is defined as the load at that point in time,
/// minus the sum of all Renewable production at that point in time. Therefore, a
/// negative net load indicates a surplus of Renewable production, and a positive
/// net load indicates a deficit of Renewable production.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///

void Controller :: __computeRenewableProduction(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    double dt_hrs = 0;
    double load_kW = 0;
    double net_load_kW = 0;
    double production_kW = 0;
    
    Renewable* renewable_ptr;
    
    for (int timestep = 0; timestep < electrical_load_ptr->n_points; timestep++) {
        dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
        load_kW = electrical_load_ptr->load_vec_kW[timestep];
        net_load_kW = load_kW;
        
        for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            
            production_kW = this->__getRenewableProduction(
                timestep,
                dt_hrs,
                renewable_ptr,
                resources_ptr
            );
            
            renewable_ptr->production_vec_kW[timestep] = production_kW;
        
            net_load_kW -= production_kW;
        }
        
        this->net_load_vec_kW[timestep] = net_load_kW;
    }
    
    return;
}   /* __computeRenewableProduction() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __constructCombustionMap(
///         std::vector<Combustion*>* combustion_ptr_vec_ptr
///     )
///
/// \brief Helper method to construct a Combustion map, for use in determining
//      the optimal dispatch of Combustion assets in each time step of the Model run.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///

void Controller :: __constructCombustionMap(
    std::vector<Combustion*>* combustion_ptr_vec_ptr
)
{
    std::string print_str = "Controller :: __constructCombustionMap()  ";
    print_str += "constructing combustion map (dispatch)  ";
    
    //  1. get state table dimensions
    unsigned int n_cols = combustion_ptr_vec_ptr->size();
    unsigned long int n_rows = pow(2, n_cols);
    
    //  2. walk through all possible operating states (on/off) and populate combustion
    //     map, keeping only states with minimum number of assets running.
    for (unsigned long int row = 0; row < n_rows; row++) {
        std::vector<bool> state_vec(n_cols, false);
        
        unsigned int asset_count = 0;
        unsigned long int x = row;
        double total_capacity_kW = 0;
        
        for (unsigned int i = 0; i < n_cols; i++) {
            if (x <= 0) {
                break;
            }
            
            if (x % 2 != 0) {
                state_vec[i] = true;
                total_capacity_kW += combustion_ptr_vec_ptr->at(i)->capacity_kW;
                asset_count++;
            }
            
            x /= 2;
        }
        
        if (this->combustion_map.count(total_capacity_kW) == 0) {
            this->combustion_map[total_capacity_kW] = state_vec;
        }
        
        else {
            unsigned int incumbent_asset_count = 0;
            
            for (unsigned int i = 0; i < n_cols; i++) {
                if (this->combustion_map[total_capacity_kW][i]) {
                    incumbent_asset_count++;
                }
            }
            
            if (asset_count < incumbent_asset_count) {
                this->combustion_map[total_capacity_kW] = state_vec;
            }
        }
        
        if (n_cols >= 14) {
            std::cout << print_str << row + 1 << " / " << n_rows << "\r";
        }
    }
    
    if (n_cols >= 14) {
        std::cout << print_str << n_rows << " / " << n_rows << "  DONE" << std::endl;
    }
    
    //  3. sort combustion map by key value (ascending order)
    /*
     *  Not necessary, since std::map is automatically sorted by key value on insertion.
     *  See https://en.cppreference.com/w/cpp/container/map, namely "std::map is a
     *  sorted associative container that contains key-value pairs with unique keys.
     *  Keys are sorted by using the comparison function Compare."
     */
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl << std::endl;
    
    std::cout << "\t\t";
    for (size_t i = 0; i < combustion_ptr_vec_ptr->size(); i++) {
        std::cout << combustion_ptr_vec_ptr->at(i)->capacity_kW << "\t";
    }
    std::cout << std::endl;
    
    std::map<double, std::vector<bool>>::iterator iter;
    for (
        iter = this->combustion_map.begin();
        iter != this->combustion_map.end();
        iter++
    ) {
        std::cout << iter->first << ":\t{\t";
        
        for (size_t i = 0; i < iter->second.size(); i++) {
            std::cout << iter->second[i] << "\t";
        }
        std::cout << "}" << std::endl;
    }
    
    
    // ==== END TEST PRINT ==== //
    */
    
    return;
}   /* __constructCombustionTable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Controller :: __getRenewableProduction(
///         int timestep,
///         double dt_hrs,
///         Renewable* renewable_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to compute the production from the given Renewable asset at
///     the given point in time.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param renewable_ptr A pointer to the Renewable asset.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \return The production [kW] of the Renewable asset.
///

double Controller :: __getRenewableProduction(
    int timestep,
    double dt_hrs,
    Renewable* renewable_ptr,
    Resources* resources_ptr
)
{
    double production_kW = 0;
    
    switch (renewable_ptr->type) {
        case (RenewableType :: SOLAR): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        case (RenewableType :: TIDAL): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        case (RenewableType :: WAVE): {
            double significant_wave_height_m = 0;
            double energy_period_s = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                significant_wave_height_m =
                    resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][0];
                
                energy_period_s =
                    resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][1];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (RenewableType :: WIND): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Controller::__getRenewableProduction():  ";
            error_str += "renewable type ";
            error_str += std::to_string(renewable_ptr->type);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return production_kW;
}   /* __getRenewableProduction() */

// ---------------------------------------------------------------------------------- //


/*
// ---------------------------------------------------------------------------------- //'
// **** DEPRECATED ****
///
/// \fn LoadStruct Controller :: __handleStorageDischarging(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the discharging of available Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets.
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleStorageDischarging(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. get total available power
    double total_available_power_kW = 0;
    std::vector<double> available_power_vec_kW(storage_ptr_vec_ptr->size(), 0);

    Storage* storage_ptr;
    
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  1.1. get pointer
        storage_ptr = storage_ptr_vec_ptr->at(asset);

        
        //  1.2. check if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }
        
        //  1.3. log available, increment total available
        available_power_vec_kW[asset] = storage_ptr->getAvailablekW(dt_hrs); 

        // --------------------------------- FC Lower Operation Limit Constraint --------------------------- //
        if (storage_ptr->type == StorageType::H2_SYS) {
            double min_fc_load = storage_ptr->getMinFCCapacitykW();
            double h2_discharge_power_kW =
                (((load_struct.load_kW - load_struct.total_renewable_production_kW) / total_available_power_kW) *
                available_power_vec_kW[asset]);

            if (h2_discharge_power_kW < min_fc_load) {
                available_power_vec_kW[asset] = 0;
            }
        }

        total_available_power_kW += available_power_vec_kW[asset];

    }
    
    //  2. set total discharge power
    double total_discharge_power_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_discharge_power_kW < load_struct.required_firm_dispatch_kW) {
        total_discharge_power_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_discharge_power_kW > total_available_power_kW) {
        total_discharge_power_kW = total_available_power_kW;
    }
    
    //  3. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_discharge_power_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  4. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -=
        (total_available_power_kW - total_discharge_power_kW);
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  5. commit total discharge power
    double asset_discharge_power_kW = 0;
    
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  5.1. get pointer
        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        //  5.2. check if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }
        
        //  5.3. get asset discharge power (proportional to available)
        if (total_available_power_kW <= 0) {
            asset_discharge_power_kW = 0;
        }

        else {
            asset_discharge_power_kW =
                (total_discharge_power_kW / total_available_power_kW) *
                available_power_vec_kW[asset];
        }   
    
        // 5.4 commit discharge
        if (asset_discharge_power_kW > 0) {
            switch (storage_ptr->type) {
                case StorageType :: H2_SYS: {
                    load_struct.load_kW = storage_ptr->commitFuelCell(
                        timestep,
                        dt_hrs,
                        asset_discharge_power_kW,
                        load_struct.load_kW
                    );
                    break;
                }
                default:{
                    load_struct.load_kW = storage_ptr->commitDischarge(
                        timestep,
                        dt_hrs,
                        asset_discharge_power_kW,
                        load_struct.load_kW
                    );
                    break;
                }
            }
            
            this->storage_discharge_bool_vec[asset] = true;
        }
    }
    
    return load_struct;
}   // __handleStorageDischarging() 

// ---------------------------------------------------------------------------------- //

*/

// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleStorageDischarging(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the discharging of available Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets.
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleStorageDischarging(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. set target discharge 
    double total_discharge_power_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_discharge_power_kW < load_struct.required_firm_dispatch_kW) {
        total_discharge_power_kW = load_struct.required_firm_dispatch_kW;
    }

    double total_available_power_kW = 0;
    double asset_discharge_power_kW = 0;

    //  2. get available power from each asset 
    std::vector<double> available_power_vec_kW(storage_ptr_vec_ptr->size(), 0);

    Storage* storage_ptr;

    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  2.1. get pointer
        storage_ptr = storage_ptr_vec_ptr->at(asset);

        //  2.2. check if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }

        //  2.3. log available
        available_power_vec_kW[asset] = storage_ptr->getAvailablekW(timestep, dt_hrs); 

        if (total_discharge_power_kW >= available_power_vec_kW[asset]){
            asset_discharge_power_kW = available_power_vec_kW[asset];
        }
        else{
            asset_discharge_power_kW = total_discharge_power_kW;
        }

        // if hydrogen storage asset, check that power is not below minimum operating capacity and if minimum runtime constraint should be enforced
        if (storage_ptr->type == StorageType::H2_SYS) {
            double min_fc_load_kW = storage_ptr->getMinFCCapacitykW();
            bool min_runtime_enforced = storage_ptr->FC_minruntime(timestep);

            // what if it is forced instead to output excess energy instead of just being off/unavailable? 
            if (asset_discharge_power_kW < min_fc_load_kW) {
                // if (asset_discharge_power_kW > 0 ){
                //     std::cout << "fc MLR enforced at: "  << timestep << std::endl;}
                available_power_vec_kW[asset] = 0;
                asset_discharge_power_kW = 0;
            }

            if (min_runtime_enforced && asset_discharge_power_kW == 0) {
                asset_discharge_power_kW = min_fc_load_kW;
                available_power_vec_kW[asset] = min_fc_load_kW;
            } 
        }

        //  2.4. update total discharge power and total available 
        total_discharge_power_kW -= asset_discharge_power_kW;
        total_available_power_kW += available_power_vec_kW[asset];

        if (asset_discharge_power_kW > 0) {
            switch (storage_ptr->type) {
                case StorageType :: H2_SYS: {
                    load_struct.load_kW = storage_ptr->commitFuelCell(
                        timestep,
                        dt_hrs,
                        asset_discharge_power_kW,
                        load_struct.load_kW
                    );
                    break;
                }
                default:{
                    load_struct.load_kW = storage_ptr->commitDischarge(
                        timestep,
                        dt_hrs,
                        asset_discharge_power_kW,
                        load_struct.load_kW
                    );
                    break;
                }
            }
            this->storage_discharge_bool_vec[asset] = true;
        }

    }

    //  3. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_discharge_power_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  4. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -=
        (total_available_power_kW - total_discharge_power_kW);
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }

    return load_struct;
}   /* __handleStorageDischarging() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleNoncombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to handle the dispatch of Noncombustion assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer
///     vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleNoncombustionDispatch(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    //  1. get total available production
    double total_available_production_kW = 0;
    std::vector<double> available_production_vec_kW(
        noncombustion_ptr_vec_ptr->size(), 0
    );
    Noncombustion* noncombustion_ptr;
    
    for (
        size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++
    ) {
        //  1.1. get pointer
        noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
        
        //  1.2. log available, increment total available
        switch (noncombustion_ptr->type) {
            case (NoncombustionType :: HYDRO): {
                double resource_value = 0;
                
                if (
                    not noncombustion_ptr->normalized_production_series_given
                ) {
                    resource_value =
                        resources_ptr->resource_map_1D[
                            noncombustion_ptr->resource_key
                        ][timestep];
                }
                
                available_production_vec_kW[asset] =
                    noncombustion_ptr->requestProductionkW(
                        timestep,
                        dt_hrs,
                        noncombustion_ptr->capacity_kW,
                        resource_value
                    );
                
                total_available_production_kW +=
                    available_production_vec_kW[asset];
                
                break;
            }
            
            default: {
                available_production_vec_kW[asset] =
                    noncombustion_ptr->requestProductionkW(
                        timestep,
                        dt_hrs,
                        noncombustion_ptr->capacity_kW
                    );
                
                total_available_production_kW +=
                    available_production_vec_kW[asset];
                
                break;
            }
        }
    }
    
    //  2. set total production
    double total_production_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_production_kW < load_struct.required_firm_dispatch_kW) {
        total_production_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_production_kW > total_available_production_kW) {
        total_production_kW = total_available_production_kW;
    }
    
    //  3. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_production_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  4. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -= 
        (total_available_production_kW - total_production_kW);
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  5. commit total production
    double asset_production_kW = 0;
    
    for (
        size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++
    ) {
        //  5.1. get pointer
        noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
        
        //  5.2. get asset production (proportional to available)
        if (total_available_production_kW <= 0) {
            asset_production_kW = 0;
        }
        
        else {
            asset_production_kW =
                (total_production_kW / total_available_production_kW) *
                available_production_vec_kW[asset];
        }
        
        //  5.3. commit production, log
        switch (noncombustion_ptr->type) {
            case (NoncombustionType :: HYDRO): {
                double resource_value = 0;
                
                if (
                    not noncombustion_ptr->normalized_production_series_given
                ) {
                    resource_value =
                        resources_ptr->resource_map_1D[
                            noncombustion_ptr->resource_key
                        ][timestep];
                }
                
                load_struct.load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    asset_production_kW,
                    load_struct.load_kW,
                    resource_value
                );
                
                break;
            }
            
            default: {
                load_struct.load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    asset_production_kW,
                    load_struct.load_kW
                );
                
                break;
            }
        }
    }
    
    return load_struct;
}   /* __handleNoncombustionDispatch() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleCombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         bool is_cycle_charging
///     )
///
/// \brief Helper method to handle the dispatch of Combustion assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of
///     the Model.
///
/// \param is_cycle_charging A flag which indicates whether the Combustion assets are 
///     running in cycle charging mode (true) or load following mode (false).
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleCombustionDispatch(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    bool is_cycle_charging
)
{
    //  1. determine allocation
    double allocation_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    

    if (allocation_kW<0.001){
        allocation_kW = 0;
    }

    if (allocation_kW < load_struct.required_firm_dispatch_kW) {
        allocation_kW = load_struct.required_firm_dispatch_kW;
    }  

    if (load_struct.required_spinning_reserve_kW > 0) {
        allocation_kW += load_struct.required_spinning_reserve_kW;
    }  
    
    //  2. allocate Combustion assets
    double allocated_capacity_kW = 0;
    std::map<double, std::vector<bool>>::iterator iter =
        this->combustion_map.begin();
    
    while (iter != std::prev(this->combustion_map.end(), 1)) {
        if (allocation_kW <= allocated_capacity_kW) {
            break;
        }
        
        iter++;
        allocated_capacity_kW = iter->first;
    }
    
    //  3. set total production
    double total_production_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_production_kW < load_struct.required_firm_dispatch_kW) {
        total_production_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_production_kW > allocated_capacity_kW) {
        total_production_kW = allocated_capacity_kW;
    }
    
    //  4. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_production_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  5. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -=
        (allocated_capacity_kW - total_production_kW);
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  6. commit Combustion assets
    //     sharing load proportionally to individual rated capacities
    //     force starts of allocated assets even if production is zero
    //     (to satisfy spinning reserve requirement)
    double asset_production_kW = 0;
    Combustion* combustion_ptr;
    
    for (
        size_t asset = 0;
        asset < this->combustion_map[allocated_capacity_kW].size();
        asset++
    ) {
        //  6.1. get pointer
        combustion_ptr = combustion_ptr_vec_ptr->at(asset);
        
        //  6.2. get asset production
        if (allocated_capacity_kW <= 0) {
            asset_production_kW = 0;
        }
        
        else {
            asset_production_kW =
                int(this->combustion_map[allocated_capacity_kW][asset]) *
                (combustion_ptr->capacity_kW / allocated_capacity_kW) *
                total_production_kW;
        }
        
        if (is_cycle_charging and asset_production_kW > 0) {
            if (
                asset_production_kW <
                    combustion_ptr->cycle_charging_setpoint *
                    combustion_ptr->capacity_kW
            ) {
                asset_production_kW =
                    combustion_ptr->cycle_charging_setpoint *
                    combustion_ptr->capacity_kW;
            }
        }
        
        //  6.3. force start (if applicable), commit production, log
        if (
            allocated_capacity_kW > 0 and
            this->combustion_map[allocated_capacity_kW][asset] and
            not combustion_ptr->is_running and
            asset_production_kW == 0
        ) {
            switch (combustion_ptr->type) {
                case (CombustionType :: DIESEL): {
                    Diesel* diesel_ptr = (Diesel*)combustion_ptr;
                    
                    diesel_ptr->is_running = true;
                    diesel_ptr->n_starts++;
                    diesel_ptr->time_since_last_start_hrs = 0;
                    
                    break;
                }
                
                default: {
                    // do nothing!
                    
                    break;
                }
            }
        }

        asset_production_kW = combustion_ptr->requestProductionkW(
            timestep,
            dt_hrs,
            asset_production_kW
        );
        
        load_struct.load_kW = combustion_ptr->commit(
            timestep,
            dt_hrs,
            asset_production_kW,
            load_struct.load_kW
        );
    }
    
    return load_struct;
}   /* __handleCombustionDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Controller :: __handleRenewableDispatch(
///         int timestep,
///         double dt_hrs,
///         double remaining_load_kW,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the dispatch of Renewable assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param remaining_load_kW The load remaining [kW] before dispatch.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of
///     the Model.
///
/// \return The net load [kW] remaining after the dispatch is deducted from it.
///

double Controller :: __handleRenewableDispatch(
    int timestep,
    double dt_hrs,
    double remaining_load_kW,
    std::vector<Renewable*>* renewable_ptr_vec_ptr
)
{
    //  1. set target dispatch
    double target_dispatch_kW = remaining_load_kW;
    
    if (target_dispatch_kW < 0) {
        target_dispatch_kW = 0;
    }
    
    //  2. dispatch Renewable assets
    Renewable* renewable_ptr;
    double production_kW = 0;
    
    for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
        renewable_ptr = renewable_ptr_vec_ptr->at(asset);
        
        production_kW = renewable_ptr->production_vec_kW[timestep];
        
        target_dispatch_kW = renewable_ptr->commit(
            timestep,
            dt_hrs,
            production_kW,
            target_dispatch_kW
        );
    }
    
    //  3. log impact of dispatch
    remaining_load_kW = target_dispatch_kW;
    
    return remaining_load_kW;
}   /* __handleRenewableDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __handleStorageCharging(
///         int timestep,
///         double dt_hrs,
///         std::vector<Storage*>* storage_ptr_vec_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the charging of available Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets
///     that are to be charged.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
///     the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///

void Controller :: __handleStorageCharging(
    int timestep,
    double dt_hrs,
    std::vector<Storage*>* storage_ptr_vec_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr
)
{
    double acceptable_kW = 0;
    double curtailment_kW = 0;
    double unused_curtailment = 0;
    
    Storage* storage_ptr;
    Combustion* combustion_ptr;
    Noncombustion* noncombustion_ptr;
    Renewable* renewable_ptr;
    
    for (
        size_t storage_asset = 0;
        storage_asset < storage_ptr_vec_ptr->size();
        storage_asset++
    ) { 
        // 1. check if already discharging and get pointer to asset
        bool is_discharging = this->storage_discharge_bool_vec[storage_asset];
        storage_ptr = storage_ptr_vec_ptr->at(storage_asset);

        if (storage_ptr->type == StorageType::H2_SYS){
            if (storage_ptr->making_hydrogen_for_external_load){
                continue;
            }
        }

        //  2. if hydrogen asset, check if it needs to be forced on due to minimum runtime constraint
        if (storage_ptr->type == StorageType::H2_SYS){

            bool min_runtime_enforced = storage_ptr->EL_minruntime(timestep);
            // otherwise, check if we are already discharging 
            if (!min_runtime_enforced && is_discharging) {
                continue;
            }
        } else {
            // For all other storage types: if discharging, skip charging 
            if (is_discharging) {
                continue;  // 
            }
        }
 
        //  3. attempt to charge from Combustion curtailment first
        for (size_t asset = 0; asset < combustion_ptr_vec_ptr->size(); asset++) {
            combustion_ptr = combustion_ptr_vec_ptr->at(asset);
            curtailment_kW = combustion_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }
            
            acceptable_kW = storage_ptr->getAcceptablekW(timestep, dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            } 
            // track unused curtailment
            if(curtailment_kW > acceptable_kW){
                unused_curtailment += (curtailment_kW - acceptable_kW);
            }
            
            combustion_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            combustion_ptr->storage_vec_kW[timestep] += acceptable_kW;
            combustion_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  4. attempt to charge from Noncombustion curtailment second
        for (size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++) {
            noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
            curtailment_kW = noncombustion_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }

            acceptable_kW = storage_ptr->getAcceptablekW(timestep, dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            }
            // track unused curtailment
            if(curtailment_kW > acceptable_kW){
                unused_curtailment += (curtailment_kW - acceptable_kW);
            }
            
            noncombustion_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            noncombustion_ptr->storage_vec_kW[timestep] += acceptable_kW;
            noncombustion_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  5. attempt to charge from Renewable curtailment third
        for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            curtailment_kW = renewable_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }
            
            acceptable_kW = storage_ptr->getAcceptablekW(timestep, dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            }
            // track unused curtailment
            if(curtailment_kW > acceptable_kW){
                unused_curtailment += (curtailment_kW - acceptable_kW);
            }
            
            renewable_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            renewable_ptr->storage_vec_kW[timestep] += acceptable_kW;
            renewable_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;

        }
        
        // if storage asset is type LIION and is not discharging or charging, run self-discharge method
        if (storage_ptr->type == StorageType::LIION){
            if (storage_ptr->power_kW == 0){
                storage_ptr->commit_SelfDischarge(timestep, dt_hrs);
            }
        }

        //  7. commit charge
        switch (storage_ptr->type) {
            case StorageType :: H2_SYS: {

                // check minimum load ratio and update power to 0 if too low
                double min_el_load_kW = storage_ptr->getMinELCapacitykW(dt_hrs);
                if (storage_ptr->power_kW < min_el_load_kW) {
                    // if (storage_ptr->power_kW > 0 ){
                    //     std::cout << "el MLR enforced at: "  << timestep << std::endl;
                    // }
                    storage_ptr->power_kW = 0;
                }
                // check if minimum runtime constraint is not met, force electrolyzer to stay on if necessary 
                bool min_runtime_enforced = storage_ptr->EL_minruntime(timestep);
                if (min_runtime_enforced) {
                    storage_ptr->power_kW = min_el_load_kW;
                } 

                storage_ptr->commitElectrolysis(
                    timestep,
                    dt_hrs,
                    storage_ptr->power_kW
                );

                // call method to produce hydrogen with extra unused curtailment, if (a) tank is full and (b) excess_hydrogen_potential_included = true 
                if (unused_curtailment > 0 && storage_ptr->power_kW == 0) {
                    storage_ptr->commitCurtailmentHydrogen(timestep, dt_hrs, unused_curtailment);
                }

                break;
            }
            default: {
                storage_ptr->commitCharge(
                    timestep,
                    dt_hrs,
                    storage_ptr->power_kW
                );
                break;
            }
        }

    }
    
    return;
}   /* __handleStorageCharging() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __handleThermalTracking(
///         int timestep,
///         double dt_hrs,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the calling of the thermal model.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets
///
///

void Controller :: __handleThermalTracking(
    int timestep,
    double dt_hrs,
    std::vector<Storage*>* storage_ptr_vec_ptr,
    ThermalModel* thermal_model_ptr
)
{

    thermal_model_ptr->commitH2ThermalTracking(timestep,dt_hrs,storage_ptr_vec_ptr);
    thermal_model_ptr->commitLiIonThermalTracking(timestep,dt_hrs,storage_ptr_vec_ptr);
    thermal_model_ptr->commitThermalBalance(timestep,dt_hrs);


    return;
}   /* __handleThermalTracking() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: Controller(void)
///
/// \brief Constructor for the Controller class.
///

Controller :: Controller(void)
{
    return;
}   /* Controller() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setControlMode(ControlMode control_mode)
///
/// \brief Method to set control mode of Controller.
///
/// \param control_mode The ControlMode which is to be active in the Controller.
///

void Controller :: setControlMode(ControlMode control_mode)
{
    this->control_mode = control_mode;
    
    switch(control_mode) {
        case (ControlMode :: LOAD_FOLLOWING): {
            this->control_string = "LOAD_FOLLOWING";
            
            break;
        }
        
        case (ControlMode :: CYCLE_CHARGING): {
            this->control_string = "CYCLE_CHARGING";
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Controller :: setControlMode():  ";
                error_str += "control mode ";
                error_str += std::to_string(control_mode);
                error_str += " not recognized";
                
                #ifdef _WIN32
                    std::cout << error_str << std::endl;
                #endif

                throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return;
}   /* setControlMode() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setFirmDispatchRatio(double max_operating_reserve_factor)
///
/// \brief Method to set Controller firm_dispatch_ratio attribute.
///
/// \param firm_dispatch_ratio The ratio [0, 1] of the load in each time step
///     that must be dispatched from firm assets.
///

void Controller :: setFirmDispatchRatio(double firm_dispatch_ratio)
{
    this->firm_dispatch_ratio = firm_dispatch_ratio;
    
    return;
}   /* setFirmDispatchRatio() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setLoadReserveRatio(double max_operating_reserve_factor)
///
/// \brief Method to set Controller firm_dispatch_ratio attribute.
///
/// \param load_reserve_ratio The ratio [0, 1] of the load in each time step
///     that must be included in the required spinning reserve.
///

void Controller :: setLoadReserveRatio(double load_reserve_ratio)
{
    this->load_reserve_ratio = load_reserve_ratio;
    
    return;
}   /* setLoadReserveRatio() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
///  \fn void Controller :: init(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr
///     )
///
/// \brief Method to initialize the Controller component of the Model.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///

void Controller :: init(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr
)
{
    //  1. init vector attributes
    this->net_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    this->missed_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    this->missed_firm_dispatch_vec_kW.resize(electrical_load_ptr->n_points, 0);
    this->missed_spinning_reserve_vec_kW.resize(electrical_load_ptr->n_points, 0);
    
    //  2. compute Renewable production
    this->__computeRenewableProduction(
        electrical_load_ptr,
        renewable_ptr_vec_ptr,
        resources_ptr
    );
    
    //  3. construct Combustion table
    this->__constructCombustionMap(combustion_ptr_vec_ptr);
    
    return;
}   /* init() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: applyDispatchControl(
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Nonombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Method to apply dispatch control at every point in the modelling time series.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
///     the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param storage_ptr_vec_ptr A pointer to the Storage pointer vector of the Model.
///

void Controller :: applyDispatchControl(
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr,
    ThermalModel* thermal_model_ptr
)
{
    double dt_hrs = 0;
    double load_kW = 0;
    double required_firm_dispatch_kW = 0;
    double total_renewable_production_kW = 0;
    double required_spinning_reserve_kW = 0;
    
    Renewable* renewable_ptr;
    
    LoadStruct load_struct;
    
    this->storage_discharge_bool_vec.clear();
    this->storage_discharge_bool_vec.resize(storage_ptr_vec_ptr->size(), false);
    
    for (
        int timestep = 0; timestep < electrical_load_ptr->n_points; timestep++
    ) {
        //  1. get load and dt_hrs
        load_kW = electrical_load_ptr->load_vec_kW[timestep];
        dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];

        // 1.1 check if there is an added load from the external hydrogen load or if we have added hydrogen load
        for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
            //  get pointer
            Storage* storage_ptr;
            storage_ptr = storage_ptr_vec_ptr->at(asset);
            //  check if asset is hydrogen energy storage
            if (storage_ptr->type == StorageType::H2_SYS) {
                if ((storage_ptr->external_hydrogen_load_included)){
                    double hydrogen_load_kW = storage_ptr->commitExternalHydrogenLoadkg(timestep, dt_hrs);
                    load_kW += hydrogen_load_kW;
                }

                // adding load if we need to force on electrolyzer...
                bool min_runtime_enforced = storage_ptr->EL_minruntime(timestep);
                if ((min_runtime_enforced)){
                    double min_el_load_kW = storage_ptr->getMinELCapacitykW(dt_hrs);
                    load_kW += min_el_load_kW;
                }
            }
        }

        //  2. compute required firm dispatch
        required_firm_dispatch_kW = this->firm_dispatch_ratio * load_kW;
        
        //  3. compute total renewable production and required spinning reserve
        total_renewable_production_kW = 0;
        required_spinning_reserve_kW = this->load_reserve_ratio * load_kW;

        for (
            size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++
        ) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            
            total_renewable_production_kW +=
                renewable_ptr->production_vec_kW[timestep];
            
            required_spinning_reserve_kW += 
                (1 - renewable_ptr->firmness_factor) *
                renewable_ptr->production_vec_kW[timestep];
        }
        
        if (required_spinning_reserve_kW > load_kW) {
            required_spinning_reserve_kW = load_kW;
        }
        
        //  4. init load structure
        load_struct.load_kW = load_kW;
        load_struct.total_renewable_production_kW =
            total_renewable_production_kW;
        load_struct.required_firm_dispatch_kW = required_firm_dispatch_kW;
        load_struct.required_spinning_reserve_kW =
            required_spinning_reserve_kW;
        
        //  5. handle Noncombustion dispatch
        load_struct = this->__handleNoncombustionDispatch(
            timestep,
            dt_hrs,
            load_struct,
            noncombustion_ptr_vec_ptr,
            resources_ptr
        );
        
        //  6. handle Storage discharge
        load_struct = this->__handleStorageDischarging(
            timestep,
            dt_hrs,
            load_struct,
            storage_ptr_vec_ptr
        );
        
        //  7. handle Combustion dispatch
        switch(this->control_mode) {
            case (ControlMode :: LOAD_FOLLOWING): {
                load_struct = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_struct,
                    combustion_ptr_vec_ptr,
                    false
                );
                
                break;
            }
            
            case (ControlMode :: CYCLE_CHARGING): {
                bool is_cycle_charging = false;
                
                for (
                    size_t asset = 0;
                    asset < storage_ptr_vec_ptr->size();
                    asset++
                ) {
                    if (not this->storage_discharge_bool_vec[asset]) {
                        is_cycle_charging = true;
                        break;
                    }
                }
                
                load_struct = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_struct,
                    combustion_ptr_vec_ptr,
                    is_cycle_charging
                );
                
                break;
            }
            
            default: {
                std::string error_str = "ERROR:  Controller :: setControlMode():  ";
                    error_str += "control mode ";
                    error_str += std::to_string(control_mode);
                    error_str += " not recognized";
                    
                    #ifdef _WIN32
                        std::cout << error_str << std::endl;
                    #endif

                    throw std::runtime_error(error_str);
                
                break;
            }
        }
        
        //  8. handle Renewable dispatch
        load_struct.load_kW = this->__handleRenewableDispatch(
            timestep,
            dt_hrs,
            load_struct.load_kW,
            renewable_ptr_vec_ptr
        );
        
        //  9. handle Storage charging
        this->__handleStorageCharging(
            timestep,
            dt_hrs,
            storage_ptr_vec_ptr,
            combustion_ptr_vec_ptr,
            noncombustion_ptr_vec_ptr,
            renewable_ptr_vec_ptr
        );

        // 10. handle thermal modelling, if it exists
        if (thermal_model_ptr!= NULL){
            this->__handleThermalTracking(
                timestep,
                dt_hrs,
                storage_ptr_vec_ptr,
                thermal_model_ptr
            );
        }
        
        //  11. log missed load, firm dispatch, and/or spinning reserve, if any
        if (load_struct.load_kW > 1e-6) {
            this->missed_load_vec_kW[timestep] = load_struct.load_kW;
        }
        
        if (load_struct.required_firm_dispatch_kW > 1e-6) {
            this->missed_firm_dispatch_vec_kW[timestep] =
                load_struct.required_firm_dispatch_kW;
        }
        
        if (load_struct.required_spinning_reserve_kW > 1e-6) {
            this->missed_spinning_reserve_vec_kW[timestep] =
                load_struct.required_spinning_reserve_kW;
        }

        //  11. reset storage_discharge_bool_vec
        for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
            this->storage_discharge_bool_vec[asset] = false;
        }
    }
    
    return;
}   /* applyDispatchControl() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: clear(void)
///
/// \brief Method to clear all attributes of the Controller object.
///

void Controller :: clear(void)
{
    this->net_load_vec_kW.clear();
    this->missed_load_vec_kW.clear();
    this->missed_firm_dispatch_vec_kW.clear();
    this->missed_spinning_reserve_vec_kW.clear();
    this->combustion_map.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: ~Controller(void)
///
/// \brief Destructor for the Controller class.
///

Controller :: ~Controller(void)
{
    this->clear();
    
    return;
}   /* ~Controller() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
