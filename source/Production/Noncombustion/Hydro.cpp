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
/// \file Hydro.cpp
///
/// \brief Implementation file for the Hydro class.
///
/// A derived class of the Noncombustion branch of Production which models production
/// using a hydroelectric asset (either with reservoir or not).
///


#include "../../../header/Production/Noncombustion/Hydro.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __checkInputs(HydroInputs hydro_inputs)
///
/// \brief Helper method to check inputs to the Hydro constructor.
///
/// \param hydro_inputs A structure of Hydro constructor inputs.
///

void Hydro :: __checkInputs(HydroInputs hydro_inputs)
{
    //  1. check fluid_density_kgm3
    if (hydro_inputs.fluid_density_kgm3 <= 0) {
        std::string error_str = "ERROR:  Hydro():  fluid_density_kgm3 must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check net_head_m
    if (hydro_inputs.net_head_m <= 0) {
        std::string error_str = "ERROR:  Hydro():  net_head_m must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check reservoir_capacity_m3
    if (hydro_inputs.reservoir_capacity_m3 < 0) {
        std::string error_str = "ERROR:  Hydro():  reservoir_capacity_m3 must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check init_reservoir_state
    if (
        hydro_inputs.init_reservoir_state < 0 or
        hydro_inputs.init_reservoir_state > 1
    ) {
        std::string error_str = "ERROR:  Hydro():  init_reservoir_state must be in ";
        error_str += "the closed interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __initInterpolator(void)
///
/// \brief Helper method to set up turbine and generator efficiency interpolation
///
/// Ref: \cite Hydro_2023\n
///

void Hydro :: __initInterpolator(void)
{
    //  1. set up generator efficiency interpolation
    InterpolatorStruct1D generator_interp_struct_1D;
    
    generator_interp_struct_1D.n_points = 12;
    
    generator_interp_struct_1D.x_vec = {
        0,   0.1, 0.2,  0.3, 0.4, 0.5,
        0.6, 0.7, 0.75, 0.8, 0.9, 1
    };
    
    generator_interp_struct_1D.min_x = 0;
    generator_interp_struct_1D.max_x = 1;
    
    generator_interp_struct_1D.y_vec = {
        0.000, 0.800, 0.900, 0.913,
        0.925, 0.943, 0.947, 0.950,
        0.953, 0.954, 0.956, 0.958
    };
    
    this->interpolator.interp_map_1D.insert(
        std::pair<int, InterpolatorStruct1D>(
            HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY,
            generator_interp_struct_1D
        )
    );
    
    //  2. set up turbine efficiency interpolation
    InterpolatorStruct1D turbine_interp_struct_1D;
    
    turbine_interp_struct_1D.n_points = 11;
    
    turbine_interp_struct_1D.x_vec = {
        0,   0.1, 0.2, 0.3, 0.4,
        0.5, 0.6, 0.7, 0.8, 0.9,
        1
    };
    
    turbine_interp_struct_1D.min_x = 0;
    turbine_interp_struct_1D.max_x = 1;
    
    std::vector<double> efficiency_vec;
    
    switch (this->turbine_type) {
        case(HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            efficiency_vec = {
                0.000, 0.780, 0.855, 0.875, 0.890,
                0.900, 0.908, 0.913, 0.918, 0.908,
                0.880
            };
            
            break;
        }
        
        case(HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            efficiency_vec = {
                0.000, 0.400, 0.625, 0.745, 0.810,
                0.845, 0.880, 0.900, 0.910, 0.900,
                0.850
            };
            
            break;
        }
        
        case(HydroTurbineType :: HYDRO_TURBINE_KAPLAN): {
            efficiency_vec = {
                0.000, 0.265, 0.460, 0.550, 0.650,
                0.740, 0.805, 0.845, 0.900, 0.880,
                0.850
            };
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Hydro():  turbine type ";
            error_str += std::to_string(this->turbine_type);
            error_str += " not recognized";
        
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    turbine_interp_struct_1D.y_vec = efficiency_vec;
    
    this->interpolator.interp_map_1D.insert(
        std::pair<int, InterpolatorStruct1D>(
            HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY,
            turbine_interp_struct_1D
        )
    );
    
    //  3. set up flow to power interpolation
    InterpolatorStruct1D flow_to_power_interp_struct_1D;
    
    double power_ratio = 0.1;
    std::vector<double> power_ratio_vec (91, 0);
    
    for (size_t i = 0; i < power_ratio_vec.size(); i++) {
        power_ratio_vec[i] = power_ratio;
        
        power_ratio += 0.01;
        
        if (power_ratio < 0) {
            power_ratio = 0;
        }
        
        else if (power_ratio > 1) {
            power_ratio = 1;
        }
    }
    
    flow_to_power_interp_struct_1D.n_points = power_ratio_vec.size();
    
    std::vector<double> flow_vec_m3hr;
    std::vector<double> power_vec_kW;
    flow_vec_m3hr.resize(power_ratio_vec.size(), 0);
    power_vec_kW.resize(power_ratio_vec.size(), 0);
    
    for (size_t i = 0; i < power_ratio_vec.size(); i++) {
        flow_vec_m3hr[i] = this->__powerToFlow(power_ratio_vec[i] * this->capacity_kW);
        power_vec_kW[i] = power_ratio_vec[i] * this->capacity_kW;
        /*
        std::cout << flow_vec_m3hr[i] << "\t" << power_vec_kW[i] << " (" <<
            power_ratio_vec[i] << ")" << std::endl;
        */
    }
    
    flow_to_power_interp_struct_1D.x_vec = flow_vec_m3hr;
    
    flow_to_power_interp_struct_1D.min_x = flow_vec_m3hr[0];
    flow_to_power_interp_struct_1D.max_x = flow_vec_m3hr[flow_vec_m3hr.size() - 1];
    
    flow_to_power_interp_struct_1D.y_vec = power_vec_kW;
    
    this->interpolator.interp_map_1D.insert(
        std::pair<int, InterpolatorStruct1D>(
            HydroInterpKeys :: FLOW_TO_POWER_INTERP_KEY,
            flow_to_power_interp_struct_1D
        )
    );
    
    return;
}   /* __initInterpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic hydroelectric capital cost.
///
/// This model was obtained by way of ...
///
/// \return A generic capital cost for the hydroelectric asset [CAD].
///

double Hydro :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 1000; //<-- WIP: need something better here!
    
    return capital_cost_per_kW * this->capacity_kW + 15000000; //<-- WIP: need something better here!
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getGenericOpMaintCost(void)
///
/// \brief Helper method (private) to generate a generic hydroelectric operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of ...
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     hydroelectric asset [CAD/kWh].
///

double Hydro :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0.05;  //<-- WIP: need something better here!
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getEfficiencyFactor(double power_kW)
///
/// \brief Helper method to compute the efficiency factor (product of turbine and
///     generator efficiencies).
///
/// Ref: \cite Hydro_2023\n
///
/// \param power_kW The power requested of the hydro plant.
///
/// \return The product of the turbine and generator efficiencies.
///

double Hydro :: __getEfficiencyFactor(double power_kW)
{
    //  1. return on zero
    if (power_kW <= 0) {
        return 0;
    }
    
    //  2. compute power ratio (clip to [0, 1])
    double power_ratio = power_kW / this->capacity_kW;
    
    if (power_ratio < 0) {
        power_ratio = 0;
    }
    
    else if (power_ratio > 1) {
        power_ratio = 1;
    }
    
    
    //  3. init efficiency factor to the turbine efficiency
    double efficiency_factor = this->interpolator.interp1D(
        HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY,
        power_ratio
    );
    
    //  4. include generator efficiency
    efficiency_factor *= this->interpolator.interp1D(
        HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY,
        power_ratio
    );
    
    return efficiency_factor;
}   /* __getEfficiencyFactor() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getMinimumFlowm3hr(void)
///
/// \brief Helper method to compute and return the minimum required flow for production,
///     based on turbine type.
///
/// This helper method assumes that the minimum flow is that which is associated with
/// a power ratio of 0.1. See constructor for initialization of minimum_power_kW.
///
/// Ref: \cite Hydro_2023\n
///
/// \return The minimum required flow [m3/hr] for production.
///

double Hydro :: __getMinimumFlowm3hr(void)
{
    return this->__powerToFlow(this->minimum_power_kW);
}   /* __getMinimumFlowm3hr() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getMaximumFlowm3hr(void)
///
/// \brief Helper method to compute and return the maximum productive flow, based on
///     turbine type.
///
/// This helper method assumes that the maximum flow is that which is associated with
/// a power ratio of 1.
///
/// Ref: \cite Hydro_2023\n
///
/// \return The maximum productive flow [m3/hr].
///

double Hydro :: __getMaximumFlowm3hr(void)
{
    return this->__powerToFlow(this->capacity_kW);
}   /* __getMaximumFlowm3hr() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __flowToPower(double flow_m3hr)
///
/// \brief Helper method to translate a given flow into a corresponding power output.
///
/// Ref: \cite Hydro_2023\n
///
/// \param flow_m3hr The flow [m3/hr] through the turbine.
///
/// \return The power output [kW] corresponding to a given flow [m3/hr].
///

double Hydro :: __flowToPower(double flow_m3hr)
{
    //  1. return on less than minimum flow
    if (flow_m3hr < this->minimum_flow_m3hr) {
        return 0;
    }
    
    //  2. interpolate flow to power
    double power_kW = this->interpolator.interp1D(
        HydroInterpKeys :: FLOW_TO_POWER_INTERP_KEY,
        flow_m3hr
    );
    
    return power_kW;
}   /* __flowToPower() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __powerToFlow(double power_kW)
///
/// \brief Helper method to translate a given power output into a corresponding flow.
///
/// Ref: \cite Hydro_2023\n
///
/// \param power_kW The power output [kW] of the hydroelectric generator.
///
/// \return
///

double Hydro :: __powerToFlow(double power_kW)
{
    //  1. return on zero power 
    if (power_kW <= 0) {
        return 0;
    }
    
    //  2. get efficiency factor
    double efficiency_factor = this->__getEfficiencyFactor(power_kW);
    
    //  3. compute flow
    double flow_m3hr = 3600 * 1000 * power_kW;
    flow_m3hr /= efficiency_factor * this->fluid_density_kgm3 * 9.81 * this->net_head_m;
    
    return flow_m3hr;
}   /* __powerToFlow() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getAvailableFlow(double dt_hrs, double hydro_resource_m3hr)
///
/// \brief Helper method to determine what flow is currently available to the turbine.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///
/// \return The flow [m3/hr] currently available through the turbine.
///

double Hydro :: __getAvailableFlow(double dt_hrs, double hydro_resource_m3hr)
{
    //  1. init to flow available from stored volume in reservoir
    double flow_m3hr = this->stored_volume_m3 / dt_hrs;
    
    //  2. add flow available from resource
    flow_m3hr += hydro_resource_m3hr;
    
    //  3. cap at maximum flow
    if (flow_m3hr > this->maximum_flow_m3hr) {
        flow_m3hr = this->maximum_flow_m3hr;
    }
    
    return flow_m3hr;
}   /* __getAvailableFlow() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getAcceptableFlow(double dt_hrs)
///
/// \brief Helper method to determine what flow is currently acceptable by the
///     reservoir.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The flow [m3/hr] currently acceptable by the reservoir.
///

double Hydro :: __getAcceptableFlow(double dt_hrs)
{
    //  1. if no reservoir, return
    if (this->reservoir_capacity_m3 <= 0) {
        return 0;
    }
    
    //  2. compute acceptable based on room in reservoir
    double acceptable_m3hr = (this->reservoir_capacity_m3 - this->stored_volume_m3) /
        dt_hrs;
    
    return acceptable_m3hr;
}   /* __getAcceptableFlow() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __updateState(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double hydro_resource_m3hr
///     )
///
/// \brief Helper method to update and log flow and reservoir state.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///

void Hydro :: __updateState(
    int timestep,
    double dt_hrs,
    double production_kW,
    double hydro_resource_m3hr
)
{
    //  1. get turbine flow, log
    double flow_m3hr = 0;
    
    if (production_kW >= this->minimum_power_kW) {
        flow_m3hr = this->__powerToFlow(production_kW);
    }
    
    double available_flow_m3hr = this->__getAvailableFlow(dt_hrs, hydro_resource_m3hr);
    
    if (flow_m3hr > available_flow_m3hr) {
        flow_m3hr = available_flow_m3hr;
    }
    
    this->turbine_flow_vec_m3hr[timestep] = flow_m3hr;
    
    //  3. compute net reservoir flow
    double net_flow_m3hr = hydro_resource_m3hr - flow_m3hr;
    
    //  4. compute flow acceptable by reservoir
    double acceptable_flow_m3hr = this->__getAcceptableFlow(dt_hrs);
    
    //  5. compute spill, update net flow (if applicable), log
    double spill_m3hr = 0;
    
    if (acceptable_flow_m3hr < net_flow_m3hr) {
        spill_m3hr = net_flow_m3hr - acceptable_flow_m3hr;
        net_flow_m3hr = acceptable_flow_m3hr;
    }
    
    this->spill_rate_vec_m3hr[timestep] = spill_m3hr;
    
    //  6. update reservoir state, log
    this->stored_volume_m3 += net_flow_m3hr * dt_hrs;
    this->stored_volume_vec_m3[timestep] = this->stored_volume_m3;
    
    return;
}   /* __updateState() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Hydro.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Hydro :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write to summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW HYDRO Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Production attributes
    ofs << "## Production Attributes\n";
    ofs << "\n";
    
    ofs << "Capacity: " << this->capacity_kW << " kW  \n";
    ofs << "\n";
    
    ofs << "Production Override: (N = 0 / Y = 1): "
        << this->normalized_production_series_given << "  \n";
    if (this->normalized_production_series_given) {
        ofs << "Path to Normalized Production Time Series: "
            << this->path_2_normalized_production_time_series << "  \n";
    }
    ofs << "\n";
    
    ofs << "Sunk Cost (N = 0 / Y = 1): " << this->is_sunk << "  \n";
    ofs << "Capital Cost: " << this->capital_cost << "  \n";
    ofs << "Operation and Maintenance Cost: " << this->operation_maintenance_cost_kWh
        << " per kWh produced  \n";
    ofs << "Nominal Inflation Rate (annual): " << this->nominal_inflation_annual
        << "  \n";
    ofs << "Nominal Discount Rate (annual): " << this->nominal_discount_annual
        << "  \n";
    ofs << "Real Discount Rate (annual): " << this->real_discount_annual << "  \n";
    ofs << "\n";
    
    ofs << "Replacement Running Hours: " << this->replace_running_hrs << "  \n";
    ofs << "\n--------\n\n";
    
    //  2.2. Noncombustion attributes
    ofs << "## Noncombustion Attributes\n";
    ofs << "\n";
    
    //...
    
    ofs << "\n--------\n\n";
    
    //  2.3. Hydro attributes
    ofs << "## Hydro Attributes\n";
    ofs << "\n";
    
    ofs << "Fluid Density: " << this->fluid_density_kgm3 << " kg/m3  \n";
    ofs << "Net Head: " << this->net_head_m << " m  \n";
    ofs << "\n";
    
    ofs << "Reservoir Volume: " << this->reservoir_capacity_m3 << " m3  \n";
    ofs << "Reservoir Initial State: " << this->init_reservoir_state << "  \n";
    ofs << "\n";
    
    ofs << "Turbine Type: ";
    switch(this->turbine_type) {
        case(HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            ofs << "PELTON";
            
            break;
        }
        
        case(HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            ofs << "FRANCIS";
            
            break;
        }
        
        case(HydroTurbineType :: HYDRO_TURBINE_KAPLAN): {
            ofs << "KAPLAN";
            
            break;
        }
        
        default: {
            // write nothing!
            
            break;
        }
    }
    ofs << "  \n";
    ofs << "\n";
    ofs << "Minimum Flow: " << this->minimum_flow_m3hr << " m3/hr  \n";
    ofs << "Maximum Flow: " << this->maximum_flow_m3hr << " m3/hr  \n";
    ofs << "\n";
    ofs << "Minimum Production: " << this->minimum_power_kW << " kW  \n";
    ofs << "\n";
    
    ofs << "\n--------\n\n";
    
    //  2.4. Hydro Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Dispatch: " << this->total_dispatch_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched  \n";
    ofs << "\n";
    
    ofs << "Running Hours: " << this->running_hours << "  \n";
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    //...
    
    ofs << "\n--------\n\n";

    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Hydro.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Hydro :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    int max_lines
)
{
    //  1. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Is Running (N = 0 / Y = 1),";
    ofs << "Turbine Flow [m3/hr],";
    ofs << "Spill Rate [m3/hr],";
    ofs << "Stored Volume [m3],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
        ofs << this->is_running_vec[i] << ",";
        ofs << this->turbine_flow_vec_m3hr[i] << ",";
        ofs << this->spill_rate_vec_m3hr[i] << ",";
        ofs << this->stored_volume_vec_m3[i] << ",";
        ofs << this->capital_cost_vec[i] << ",";
        ofs << this->operation_maintenance_cost_vec[i] << ",";
        ofs << "\n";
    }

    ofs.close();
    return;
}   /* __writeTimeSeries() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: Hydro(void)
///
/// \brief Constructor (dummy) for the Hydro class.
///

Hydro :: Hydro(void)
{
    return;
}   /* Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: Hydro(
///         int n_points,
///         double n_years,
///         HydroInputs hydro_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Hydro class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param hydro_inputs A structure of Hydro constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Hydro :: Hydro(
    int n_points,
    double n_years,
    HydroInputs hydro_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Noncombustion(
    n_points,
    n_years,
    hydro_inputs.noncombustion_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(hydro_inputs);
    
    //  2. set attributes
    this->type = NoncombustionType :: HYDRO;
    this->type_str = "HYDRO";
    
    this->resource_key = hydro_inputs.resource_key;
    
    this->turbine_type = hydro_inputs.turbine_type;
    
    this->fluid_density_kgm3 = hydro_inputs.fluid_density_kgm3;
    this->net_head_m = hydro_inputs.net_head_m;
    
    this->reservoir_capacity_m3 = hydro_inputs.reservoir_capacity_m3;
    this->init_reservoir_state = hydro_inputs.init_reservoir_state;
    this->stored_volume_m3 =
        hydro_inputs.init_reservoir_state * hydro_inputs.reservoir_capacity_m3;
    
    this->minimum_power_kW = 0.1 * this->capacity_kW;   // <-- NEED TO DOUBLE CHECK THAT THIS MAKES SENSE IN GENERAL
    
    this->__initInterpolator();
    
    this->minimum_flow_m3hr = this->__getMinimumFlowm3hr();
    this->maximum_flow_m3hr = this->__getMaximumFlowm3hr();
    
    this->turbine_flow_vec_m3hr.resize(this->n_points, 0);
    this->spill_rate_vec_m3hr.resize(this->n_points, 0);
    this->stored_volume_vec_m3.resize(this->n_points, 0);
    
    if (hydro_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = hydro_inputs.capital_cost;
    }
    
    if (hydro_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            hydro_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    return;
}   /* Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Hydro :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Noncombustion :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: requestProductionkW(
///         int timestep,
///         double dt_hrs,
///         double request_kW,
///         double hydro_resource_m3hr
///     )
///
/// \brief Method which takes in production request, and then returns what the asset can
///     deliver (subject to operating constraints, etc.).
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param request_kW The requested production [kW].
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///
/// \return The production [kW] delivered by the hydro generator.
///

double Hydro :: requestProductionkW(
    int timestep,
    double dt_hrs,
    double request_kW,
    double hydro_resource_m3hr
)
{
    //  0. given production time series override
    if (this->normalized_production_series_given) {
        double production_kW = Production :: getProductionkW(timestep);
        
        return production_kW;
    }
    
    //  1. return on request of zero
    if (request_kW <= 0) {
        return 0;
    }
    
    //  2. if request is less than minimum power, set to minimum power
    if (request_kW < this->minimum_power_kW) {
        request_kW = this->minimum_power_kW;
    }
    
    //  3. check available flow, return if less than minimum flow
    double available_flow_m3hr = this->__getAvailableFlow(dt_hrs, hydro_resource_m3hr);
    
    if (available_flow_m3hr < this->minimum_flow_m3hr) {
        return 0;
    }
    
    //  4. init production to request, enforce capacity constraint (which also accounts
    //     for maximum flow constraint).
    double production_kW = request_kW;
    
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    //  5. map production to flow
    double flow_m3hr = this->__powerToFlow(production_kW);
    
    //  6. if flow is in excess of available, then adjust production accordingly
    if (flow_m3hr > available_flow_m3hr) {
        production_kW = this->__flowToPower(available_flow_m3hr);
    }
    
    return production_kW;
}   /* requestProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: commit(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in production and load for the current timestep, computes
///     and records dispatch and curtailment, and then returns remaining load.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the dispatch is deducted from it.
///

double Hydro :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW,
    double hydro_resource_m3hr
)
{
    //  1. invoke base class method
    load_kW = Noncombustion :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    //  2. update state and record
    this->__updateState(
        timestep,
        dt_hrs,
        production_kW,
        hydro_resource_m3hr
    );
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: ~Hydro(void)
///
/// \brief Destructor for the Hydro class.
///

Hydro :: ~Hydro(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Hydro object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Hydro() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
