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
/// \file Combustion.cpp
///
/// \brief Implementation file for the Combustion class.
///
/// The root of the Combustion branch of the Production hierarchy. This branch 
/// contains derived classes which model the production of energy by way of
/// combustibles.
///


#include "../../../header/Production/Combustion/Combustion.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: __checkInputs(CombustionInputs combustion_inputs)
///
/// \brief Helper method to check inputs to the Combustion constructor.
///
/// \param combustion_inputs A structure of Combustion constructor inputs.
///

void Combustion :: __checkInputs(CombustionInputs combustion_inputs)
{
    // 1. if FUEL_MODE_LOOKUP, check that path is given
    if (
        combustion_inputs.fuel_mode == FuelMode :: FUEL_MODE_LOOKUP and
        combustion_inputs.path_2_fuel_interp_data.empty()
    ) {
        std::string error_str = "ERROR:  Combustion()  fuel mode was set to ";
        error_str += "FuelMode::FUEL_MODE_LOOKUP, but no path to fuel interpolation ";
        error_str += "data was given";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    // 2. cycle charging setpoint
    if (
        combustion_inputs.cycle_charging_setpoint < 0 or
        combustion_inputs.cycle_charging_setpoint > 1
    ) {
        std::string error_str = "ERROR:  Combustion()  cycle charging set point ";
        error_str += "must be in the closed interval [0, 1].";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: Combustion(void)
///
/// \brief Constructor (dummy) for the Combustion class.
///

Combustion :: Combustion(void)
{
    return;
}   /* Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: Combustion(
///         int n_points,
///         double n_years,
///         CombustionInputs combustion_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Combustion class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param combustion_inputs A structure of Combustion constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Combustion :: Combustion(
    int n_points,
    double n_years,
    CombustionInputs combustion_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Production(
    n_points,
    n_years,
    combustion_inputs.production_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(combustion_inputs);
    
    //  2. set attributes
    this->fuel_mode = combustion_inputs.fuel_mode;
    
    switch (this->fuel_mode) {
        case (FuelMode :: FUEL_MODE_LINEAR): {
            this->fuel_mode_str = "FUEL_MODE_LINEAR";
            
            break;
        }
        
        case (FuelMode :: FUEL_MODE_LOOKUP): {
            this->fuel_mode_str = "FUEL_MODE_LOOKUP";
            
            this->interpolator.addData1D(
                0,
                combustion_inputs.path_2_fuel_interp_data
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Combustion():  ";
            error_str += "fuel mode ";
            error_str += std::to_string(this->fuel_mode);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    this->fuel_cost_L = 0;
    this->nominal_fuel_escalation_annual =
        combustion_inputs.nominal_fuel_escalation_annual;
    
    this->real_fuel_escalation_annual = this->computeRealDiscountAnnual(
        combustion_inputs.nominal_fuel_escalation_annual,
        combustion_inputs.production_inputs.nominal_discount_annual
    );
    
    this->linear_fuel_slope_LkWh = 0;
    this->linear_fuel_intercept_LkWh = 0;
    
    this->cycle_charging_setpoint = combustion_inputs.cycle_charging_setpoint;
    
    this->CO2_emissions_intensity_kgL = 0;
    this->CO_emissions_intensity_kgL = 0;
    this->NOx_emissions_intensity_kgL = 0;
    this->SOx_emissions_intensity_kgL = 0;
    this->CH4_emissions_intensity_kgL = 0;
    this->PM_emissions_intensity_kgL = 0;
    
    this->total_fuel_consumed_L = 0;
    
    this->fuel_consumption_vec_L.resize(this->n_points, 0);
    this->fuel_cost_vec.resize(this->n_points, 0);
    
    this->CO2_emissions_vec_kg.resize(this->n_points, 0);
    this->CO_emissions_vec_kg.resize(this->n_points, 0);
    this->NOx_emissions_vec_kg.resize(this->n_points, 0);
    this->SOx_emissions_vec_kg.resize(this->n_points, 0);
    this->CH4_emissions_vec_kg.resize(this->n_points, 0);
    this->PM_emissions_vec_kg.resize(this->n_points, 0);
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Combustion object constructed at " << this << std::endl;
    }
    
    return;
}   /* Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Combustion :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Production :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: computeFuelAndEmissions(void)
///
/// \brief Helper method to compute the total fuel consumption and emissions over the
///     Model run.
///

void Combustion :: computeFuelAndEmissions(void)
{
    for (int i = 0; i < n_points; i++) {
        this->total_fuel_consumed_L += this->fuel_consumption_vec_L[i];
        
        this->total_emissions.CO2_kg += this->CO2_emissions_vec_kg[i];
        this->total_emissions.CO_kg += this->CO_emissions_vec_kg[i];
        this->total_emissions.NOx_kg += this->NOx_emissions_vec_kg[i];
        this->total_emissions.SOx_kg += this->SOx_emissions_vec_kg[i];
        this->total_emissions.CH4_kg += this->CH4_emissions_vec_kg[i];
        this->total_emissions.PM_kg += this->PM_emissions_vec_kg[i];
    }
    
    return;
}   /* computeFuelAndEmissions() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Helper method to compute key economic metrics for the Model run.
///
/// Ref: \cite HOMER_discount_factor\n
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///

void Combustion :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
{
    //  1. account for fuel costs in net present cost
    double t_hrs = 0;
    double real_fuel_escalation_scalar = 0;
    
    for (int i = 0; i < this->n_points; i++) {
        t_hrs = time_vec_hrs_ptr->at(i);
        
        real_fuel_escalation_scalar = 1.0 / pow(
            1 + this->real_fuel_escalation_annual,
            t_hrs / 8760
        );
        
        this->net_present_cost += real_fuel_escalation_scalar * this->fuel_cost_vec[i];
    }
    
    //  2. invoke base class method
    Production :: computeEconomics(time_vec_hrs_ptr);
    
    return;
}   /* computeEconomics() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Combustion :: commit(
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

double Combustion :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Production :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    
    if (this->is_running) {
        //  2. compute and record fuel consumption
        double fuel_consumed_L = this->getFuelConsumptionL(dt_hrs, production_kW);
        this->fuel_consumption_vec_L[timestep] = fuel_consumed_L;
        
        //  3. compute and record emissions
        Emissions emissions = this->getEmissionskg(fuel_consumed_L);
        this->CO2_emissions_vec_kg[timestep] = emissions.CO2_kg;
        this->CO_emissions_vec_kg[timestep] = emissions.CO_kg;
        this->NOx_emissions_vec_kg[timestep] = emissions.NOx_kg;
        this->SOx_emissions_vec_kg[timestep] = emissions.SOx_kg;
        this->CH4_emissions_vec_kg[timestep] = emissions.CH4_kg;
        this->PM_emissions_vec_kg[timestep] = emissions.PM_kg;
        
        //  4. incur fuel costs
        this->fuel_cost_vec[timestep] = fuel_consumed_L * this->fuel_cost_L;
    }
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Combustion :: getFuelConsumptionL(double dt_hrs, double production_kW)
///
/// \brief Method which takes in production and returns volume of fuel burned over the
///     given interval of time.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \return The volume of fuel consumed [L].
///

double Combustion :: getFuelConsumptionL(double dt_hrs, double production_kW)
{
    double fuel_consumed_L = 0;
    
    switch (this->fuel_mode) {
        case (FuelMode :: FUEL_MODE_LINEAR): {
            fuel_consumed_L = (
                this->linear_fuel_slope_LkWh * production_kW +
                this->linear_fuel_intercept_LkWh * this->capacity_kW
            ) * dt_hrs;
            
            break;
        }
        
        case (FuelMode :: FUEL_MODE_LOOKUP): {
            double load_ratio = production_kW / this->capacity_kW;
            
            fuel_consumed_L = this->interpolator.interp1D(0, load_ratio) * dt_hrs;
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Combustion::getFuelConsumptionL():  ";
            error_str += "fuel mode ";
            error_str += std::to_string(this->fuel_mode);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return fuel_consumed_L;
} /* getFuelConsumptionL() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Emissions Combustion :: getEmissionskg(double fuel_consumed_L)
///
/// \brief Method which takes in volume of fuel consumed and returns mass spectrum of
///     resulting emissions.
///
/// \param fuel_consumed_L The volume of fuel consumed [L].
///
/// \return A structure containing the mass spectrum of resulting emissions.
///

Emissions Combustion :: getEmissionskg(double fuel_consumed_L) {
    Emissions emissions;
    
    emissions.CO2_kg = this->CO2_emissions_intensity_kgL * fuel_consumed_L;
    emissions.CO_kg = this->CO_emissions_intensity_kgL * fuel_consumed_L;
    emissions.NOx_kg = this->NOx_emissions_intensity_kgL * fuel_consumed_L;
    emissions.SOx_kg = this->SOx_emissions_intensity_kgL * fuel_consumed_L;
    emissions.CH4_kg = this->CH4_emissions_intensity_kgL * fuel_consumed_L;
    emissions.PM_kg = this->PM_emissions_intensity_kgL * fuel_consumed_L;
    
    return emissions;
}   /* getEmissionskg() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: writeResults(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int combustion_index,
///         int max_lines
///     )
///
/// \brief Method which writes Combustion results to an output directory.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param combustion_index An integer which corresponds to the index of the
///     Combustion asset in the Model.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written. If =0, then only summary results are written.
///

void Combustion :: writeResults(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    int combustion_index,
    int max_lines
)
{
    //  1. handle sentinel
    if (max_lines < 0) {
        max_lines = this->n_points;
    }
    
    //  2. create subdirectories
    write_path += "Production/";
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    
    write_path += "Combustion/";
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    
    write_path += this->type_str;
    write_path += "_";
    write_path += std::to_string(int(ceil(this->capacity_kW)));
    write_path += "kW_idx";
    write_path += std::to_string(combustion_index);
    write_path += "/";
    std::filesystem::create_directory(write_path);
    
    //  3. write summary
    this->__writeSummary(write_path);
    
    //  4. write time series
    if (max_lines > this->n_points) {
        max_lines = this->n_points;
    }
    
    if (max_lines > 0) {
        this->__writeTimeSeries(write_path, time_vec_hrs_ptr, max_lines);
    }
    
    return;
}   /* writeResults() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: ~Combustion(void)
///
/// \brief Destructor for the Combustion class.
///

Combustion :: ~Combustion(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Combustion object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Combustion() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
