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
/// \file Production.cpp
///
/// \brief Implementation file for the Production class.
///
/// The base class of the Production hierarchy. This hierarchy contains derived
/// classes which model the production of energy, be it renewable or otherwise.
///


#include "../../header/Production/Production.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Production :: __checkInputs(int n_points, ProductionInputs production_inputs)
///
/// \brief Helper method to check inputs to the Production constructor.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param production_inputs A structure of Production constructor inputs.
///

void Production :: __checkInputs(
    int n_points, 
    double n_years, 
    ProductionInputs production_inputs
)
{
    //  1. check n_points
    if (n_points <= 0) {
        std::string error_str = "ERROR:  Production():  n_points must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check n_years
    if (n_years <= 0) {
        std::string error_str = "ERROR:  Production():  n_years must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check capacity_kW
    if (production_inputs.capacity_kW <= 0) {
        std::string error_str = "ERROR:  Production():  ";
        error_str += "ProductionInputs::capacity_kW must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check replace_running_hrs
    if (production_inputs.replace_running_hrs <= 0) {
        std::string error_str = "ERROR:  Production():  ";
        error_str += "ProductionInputs::replace_running_hrs must be > 0";
        
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
/// \fn void Production :: __checkTimePoint(
///         double time_received_hrs,
///         double time_expected_hrs
///     )
///
/// \brief Helper method to check received time point against expected time point. The
///     given time series should align point-wise with the previously given electrical
///     load time series.
///
/// \param time_received_hrs The point in time received from the given data.
///
/// \param time_expected_hrs The point in time expected (this comes from the electrical
///     load time series).
///

void Production :: __checkTimePoint(
    double time_received_hrs,
    double time_expected_hrs
)
{
    if (time_received_hrs != time_expected_hrs) {
        std::string error_str = "ERROR:  Production():  ";
        error_str += "the given normalized production time series at ";
        error_str += this->path_2_normalized_production_time_series;
        error_str += " does not align with the ";
        error_str += "previously given electrical load time series";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::runtime_error(error_str);
    }
    
    return;
}   /* __checkTimePoint() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
///
/// \fn void Production :: __throwLengthError(void)
///
/// \brief Helper method to throw data length error (if not the same as the given 
///     electrical load time series).
///

void Production :: __throwLengthError(void)
{
    std::string error_str = "ERROR:  Production():  ";
    error_str += "the given normalized production time series at ";
    error_str += this->path_2_normalized_production_time_series;
    error_str += " is not the same length as the previously given electrical";
    error_str += " load time series";
    
    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    
    return;
}   /* __throwLengthError() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Production :: __checkNormalizedProduction(double normalized_production)
///
/// \brief Helper method to check that given data values are everywhere contained in
///     the closed interval [0, 1]. A normalized production time series is expected, so
///     this must be true everywhere.
///
/// \param normalized_production The normalized production value to check
///

void Production :: __checkNormalizedProduction(double normalized_production)
{
    if (normalized_production < 0 or normalized_production > 1) {
        std::string error_str = "ERROR:  Production():  ";
        error_str += "the given normalized production time series at ";
        error_str += this->path_2_normalized_production_time_series;
        error_str += " contains normalized production values outside the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::runtime_error(error_str);
    }
    
    return;
}   /* __throwValueError() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Production :: __readNormalizedProductionData(
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Helper method to read in a given time series of normalized production.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void Production :: __readNormalizedProductionData(
    std::vector<double>* time_vec_hrs_ptr
)
{
    //  1. init CSV reader
    io::CSVReader<2> CSV(this->path_2_normalized_production_time_series);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Normalized Production [ ]"
    );
    
    //  2. read in normalized performance data,
    //     check values and check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double normalized_production = 0;
    
    while (CSV.read_row(time_hrs, normalized_production)) {
        //  2.1. check length of data
        if (n_points > this->n_points) {
            this->__throwLengthError();
        }
    
        //  2.2. check normalized production value
        this->__checkNormalizedProduction(normalized_production);
        
        //  2.3. check time point
        time_expected_hrs = time_vec_hrs_ptr->at(n_points);
        this->__checkTimePoint(time_hrs, time_expected_hrs);
        
        //  2.4. write to normalized production vector, increment n_points
        this->normalized_production_vec[n_points] = normalized_production;
        n_points++;
    }
    
    //  3. check length of data
    if (n_points != this->n_points) {
        this->__throwLengthError();
    }
    
    return;
}   /* __readNormalizedProductionData() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: Production(void)
///
/// \brief Constructor (dummy) for the Production class.
///

Production :: Production(void)
{
    return;
}   /* Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: Production(
///         int n_points,
///         double n_years,
///         ProductionInputs production_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Production class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param production_inputs A structure of Production constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Production :: Production(
    int n_points,
    double n_years,
    ProductionInputs production_inputs,
    std::vector<double>* time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(n_points, n_years, production_inputs);
    
    //  2. set attributes
    this->print_flag = production_inputs.print_flag;
    this->is_running = false;
    this->is_sunk = production_inputs.is_sunk;
    this->normalized_production_series_given = false;
    
    this->n_points = n_points;
    this->n_starts = 0;
    this->n_replacements = 0;
    
    this->n_years = n_years;
    
    this->running_hours = 0;
    this->replace_running_hrs = production_inputs.replace_running_hrs;
    
    this->capacity_kW = production_inputs.capacity_kW;
    
    this->nominal_inflation_annual = production_inputs.nominal_inflation_annual;
    this->nominal_discount_annual = production_inputs.nominal_discount_annual;
    
    this->real_discount_annual = this->computeRealDiscountAnnual(
        production_inputs.nominal_inflation_annual,
        production_inputs.nominal_discount_annual
    );
    
    this->capital_cost = 0;
    this->operation_maintenance_cost_kWh = 0;
    this->net_present_cost = 0;
    this->total_dispatch_kWh = 0;
    this->total_stored_kWh = 0;
    this->levellized_cost_of_energy_kWh = 0;
    this->curtailment_total_kWh = 0;
    
    this->path_2_normalized_production_time_series = "";
    
    this->is_running_vec.resize(this->n_points, 0);
    
    this->normalized_production_vec.resize(this->n_points, 0);
    this->production_vec_kW.resize(this->n_points, 0);
    this->dispatch_vec_kW.resize(this->n_points, 0);
    this->storage_vec_kW.resize(this->n_points, 0);
    this->curtailment_vec_kW.resize(this->n_points, 0);
    
    this->capital_cost_vec.resize(this->n_points, 0);
    this->operation_maintenance_cost_vec.resize(this->n_points, 0);
    
    //  3. read in normalized production time series (if given)
    if (not production_inputs.path_2_normalized_production_time_series.empty()) {
        this->normalized_production_series_given = true;
        
        this->path_2_normalized_production_time_series = 
            production_inputs.path_2_normalized_production_time_series;
        
        this->__readNormalizedProductionData(time_vec_hrs_ptr);
    }
    
    //  4. construction print
    if (this->print_flag) {
        std::cout << "Production object constructed at " << this << std::endl;
    }
    
    return;
}   /* Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Production :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Production :: handleReplacement(int timestep)
{
    //  1. reset attributes
    this->is_running = false;
    
    //  2. log replacement
    this->n_replacements++;
    
    //  3. incur capital cost in timestep
    this->capital_cost_vec[timestep] = this->capital_cost;
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Production :: computeRealDiscountAnnual(
///         double nominal_inflation_annual,
///         double nominal_discount_annual
///     )
///
/// \brief Method to compute the real, annual discount rate to be used
///     in computing model economics. This enables application of the discount factor
///     approach.
///
/// Ref: \cite HOMER_real_discount_rate\n
/// Ref: \cite HOMER_discount_factor\n
///
/// \param nominal_inflation_annual The nominal, annual inflation rate to use in computing model economics.
///
/// \param nominal_discount_annual The nominal, annual discount rate to use in computing model economics.
///
/// \return The real, annual discount rate to use in computing model economics.
///

double Production :: computeRealDiscountAnnual(
    double nominal_inflation_annual,
    double nominal_discount_annual
)
{
    double real_discount_annual = nominal_discount_annual - nominal_inflation_annual;
    real_discount_annual /= 1 + nominal_inflation_annual;
    
    return real_discount_annual;
}   /* __computeRealDiscountAnnual() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Production :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Helper method to compute key economic metrics for the Model run.
///
/// Ref: \cite HOMER_discount_factor\n
/// Ref: \cite HOMER_levelized_cost_of_energy\n
/// Ref: \cite HOMER_total_annualized_cost\n
/// Ref: \cite HOMER_capital_recovery_factor\n
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///

void Production :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
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
    
    /// 2. compute levellized cost of energy (per unit dispatched)
    //     assuming 8,760 hours per year
    if (this->total_dispatch_kWh <= 0) {
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
            this->total_dispatch_kWh;
    }
    
    return;
}   /* computeEconomics() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Production :: getProductionkW(int timestep)
///
/// \brief A method to simply fetch the normalized production at a particular point in
///     the given normalized production time series, multiply by the rated capacity
///     of the asset, and return.
///
/// \return The production [kW] for the asset at the given point in time, as defined by
///     the given normalized production time series.
///

double Production :: getProductionkW(int timestep)
{
    double production_kW =
        this->normalized_production_vec[timestep] * this->capacity_kW;
    
    return production_kW;
}   /* getProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Production :: commit(
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

double Production :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. record production
    this->production_vec_kW[timestep] = production_kW;
    
    //  2. compute and record dispatch and curtailment
    double dispatch_kW = 0;
    double curtailment_kW = 0;
    
    if (production_kW > load_kW) {
        dispatch_kW = load_kW;
        curtailment_kW = production_kW - dispatch_kW;
    }
    
    else {
        dispatch_kW = production_kW;
    }
    
    this->dispatch_vec_kW[timestep] = dispatch_kW;
    this->total_dispatch_kWh += dispatch_kW * dt_hrs;
    this->curtailment_vec_kW[timestep] = curtailment_kW;
    this->curtailment_total_kWh += curtailment_kW * dt_hrs;
    
    //  3. update load
    load_kW -= dispatch_kW;
    
    //  4. update and log running attributes
    if (this->is_running) {
        //  4.1. log running state, running hours
        this->is_running_vec[timestep] = this->is_running;
        this->running_hours += dt_hrs;
        
        //  4.2. incur operation and maintenance costs
        double produced_kWh = production_kW * dt_hrs;
        
        double operation_maintenance_cost =
            this->operation_maintenance_cost_kWh * produced_kWh;
        this->operation_maintenance_cost_vec[timestep] = operation_maintenance_cost;
    }
    
    //  5. trigger replacement, if applicable
    if (this->running_hours >= (this->n_replacements + 1) * this->replace_running_hrs) {
        this->handleReplacement(timestep);
    }
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: ~Production(void)
///
/// \brief Destructor for the Production class.
///

Production :: ~Production(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Production object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Production() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
