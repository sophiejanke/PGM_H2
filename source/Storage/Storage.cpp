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
/// \file Storage.cpp
///
/// \brief Implementation file for the Storage class.
///
/// The base class of the Storage hierarchy. This hierarchy contains derived classes
/// which model the storage of energy.
///


#include "../../header/Storage/Storage.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Storage :: __checkInputs(int n_points, StorageInputs storage_inputs)
///
/// \brief Helper method to check inputs to the Storage constructor.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param storage_inputs A structure of Storage constructor inputs.
///

void Storage :: __checkInputs(
    int n_points, 
    double n_years, 
    StorageInputs storage_inputs
)
{
    //  1. check n_points
    if (n_points <= 0) {
        std::string error_str = "ERROR:  Storage():  n_points must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check n_years
    if (n_years <= 0) {
        std::string error_str = "ERROR:  Storage():  n_years must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check power_capacity_kW
    if (storage_inputs.power_capacity_kW <= 0) {
        std::string error_str = "ERROR:  Storage():  ";
        error_str += "StorageInputs::power_capacity_kW must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check energy_capacity_kWh
    if (storage_inputs.energy_capacity_kWh <= 0) {
        std::string error_str = "ERROR:  Storage():  ";
        error_str += "StorageInputs::energy_capacity_kWh must be > 0";
        
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
/// \fn double Storage :: __computeRealDiscountAnnual(
///         double nominal_inflation_annual,
///         double nominal_discount_annual
///     )
///
/// \brief Helper method to compute the real, annual discount rate to be used
///     in computing model economics. This enables application of the discount factor
///     approach.
///
/// Ref: \cite HOMER_real_discount_rate\n
/// Ref: \cite HOMER_discount_factor\n
///
/// \param nominal_inflation_annual The nominal, annual inflation rate to use in
///     computing model economics.
///
/// \param nominal_discount_annual The nominal, annual discount rate to use in
///     computing model economics.
///
/// \return The real, annual discount rate to use in computing model economics.
///

double Storage :: __computeRealDiscountAnnual(
    double nominal_inflation_annual,
    double nominal_discount_annual
)
{
    double real_discount_annual = nominal_discount_annual - nominal_inflation_annual;
    real_discount_annual /= 1 + nominal_inflation_annual;
    
    return real_discount_annual;
}   /* __computeRealDiscountAnnual() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn Storage :: Storage(void)
///
/// \brief Constructor (dummy) for the Storage class.
///

Storage :: Storage(void)
{
    return;
}   /* Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Storage :: Storage(
///         int n_points,
///         double n_years,
///         StorageInputs storage_inputs
///     )
///
/// \brief Constructor (intended) for the Storage class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param storage_inputs A structure of Storage constructor inputs.
///

Storage :: Storage(
    int n_points,
    double n_years,
    StorageInputs storage_inputs
)
{
    //  1. check inputs
    this->__checkInputs(n_points, n_years, storage_inputs);
    
    //  2. set attributes
    this->print_flag = storage_inputs.print_flag;
    this->is_depleted = false;
    this->is_sunk = storage_inputs.is_sunk;
    
    this->n_points = n_points;
    this->n_replacements = 0;
    
    this->n_years = n_years;
    
    this->power_capacity_kW = storage_inputs.power_capacity_kW;
    this->energy_capacity_kWh = storage_inputs.energy_capacity_kWh;
    
    this->charge_kWh = 0;
    this->power_kW = 0;
    
    this->nominal_inflation_annual = storage_inputs.nominal_inflation_annual;
    this->nominal_discount_annual = storage_inputs.nominal_discount_annual;
    
    this->real_discount_annual = this->__computeRealDiscountAnnual(
        storage_inputs.nominal_inflation_annual,
        storage_inputs.nominal_discount_annual
    );
    
    this->capital_cost = 0;
    this->operation_maintenance_cost_kWh = 0;
    this->net_present_cost = 0;
    this->total_discharge_kWh = 0;
    this->levellized_cost_of_energy_kWh = 0;
    
    this->charge_vec_kWh.resize(this->n_points, 0);
    this->charging_power_vec_kW.resize(this->n_points, 0);
    this->discharging_power_vec_kW.resize(this->n_points, 0);
    
    this->capital_cost_vec.resize(this->n_points, 0);
    this->operation_maintenance_cost_vec.resize(this->n_points, 0);
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Storage object constructed at " << this << std::endl;
    }
    
    return;
}   /* Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Storage :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Storage :: handleReplacement(int timestep)
{
    //  1. reset attributes
    this->charge_kWh = 0;
    this->power_kW = 0;
    
    //  2. log replacement
    this->n_replacements++;
    
    //  3. incur capital cost in timestep
    this->capital_cost_vec[timestep] = this->capital_cost;
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Storage :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Helper method to compute key economic metrics for the Model run.
///
/// Ref: \cite HOMER_discount_factor\n
/// Ref: \cite HOMER_levelized_cost_of_energy\n
/// Ref: \cite HOMER_total_annualized_cost\n
/// Ref: \cite HOMER_capital_recovery_factor\n
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the
///     ElectricalLoad.
///

void Storage :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
{
    //  1. compute net present cost
    double t_hrs = 0;
    double real_discount_scalar = 0;
    
    for (int i = 0; i < this->n_points; i++) {
        t_hrs = time_vec_hrs_ptr->at(i);
        
        real_discount_scalar = 1.0 / pow(
            1 + this->real_discount_annual,
            t_hrs / 8760
        );
        
        this->net_present_cost += real_discount_scalar * this->capital_cost_vec[i];
        
        this->net_present_cost +=
            real_discount_scalar * this->operation_maintenance_cost_vec[i];
    }
    
    /// 2. compute levellized cost of energy (per unit discharged)
    //     assuming 8,760 hours per year
    if (this->total_discharge_kWh <= 0) {
        this->levellized_cost_of_energy_kWh = this->net_present_cost;
    }
    
    else {
        double n_years = time_vec_hrs_ptr->at(this->n_points - 1) / 8760;
    
        double capital_recovery_factor = 
            (this->real_discount_annual * pow(1 + this->real_discount_annual, n_years)) / 
            (pow(1 + this->real_discount_annual, n_years) - 1);

        double total_annualized_cost = capital_recovery_factor *
            this->net_present_cost;
        
        this->levellized_cost_of_energy_kWh =
            (n_years * total_annualized_cost) /
            this->total_discharge_kWh;
    }
    
    return;
}   /* computeEconomics() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Storage :: writeResults(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int storage_index,
///         int max_lines
///     )
///
/// \brief Method which writes Storage results to an output directory.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the
///     ElectricalLoad.
///
/// \param storage_index An integer which corresponds to the index of the Storage
///     asset in the Model.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written. If =0, then only summary results are written.
///

void Storage :: writeResults(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    int storage_index,
    int max_lines
)
{
    //  1. handle sentinel
    if (max_lines < 0) {
        max_lines = this->n_points;
    }
    
    //  2. create subdirectories
    write_path += "Storage/";
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    

    std::string storage_subfolder;

    std::string h2_module = "H2_SYS";

    if (this->type_str == h2_module) {
        storage_subfolder = "H2/";
    } else if(this->type_str == "LIION") {
        storage_subfolder = "BESS/";
    }
    
    write_path += storage_subfolder;
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    
    write_path += this->type_str;
    write_path += "_";
    write_path += std::to_string(int(ceil(this->power_capacity_kW)));
    write_path += "kW_";
    write_path += std::to_string(int(ceil(this->energy_capacity_kWh)));
    write_path += "kWh_idx";
    write_path += std::to_string(storage_index);
    write_path += "/";
    std::filesystem::create_directory(write_path);
    
    //  3. write summary
    this->__writeSummary(write_path);
    
    //  4. write time series
    if (max_lines > this->n_points) {
        max_lines = this->n_points;
    }
    
    if (max_lines > 0) {
        this->__writeTimeSeries(
            write_path,
            time_vec_hrs_ptr,
            max_lines
        );
    }
    
    return;
}   /* writeResults() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Storage :: ~Storage(void)
///
/// \brief Destructor for the Storage class.
///

Storage :: ~Storage(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Storage object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Storage() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
