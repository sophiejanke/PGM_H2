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
/// \file Tidal.cpp
///
/// \brief Implementation file for the Tidal class.
///
/// A derived class of the Renewable branch of Production which models tidal
/// production.
///


#include "../../../header/Production/Renewable/Tidal.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Tidal :: __checkInputs(TidalInputs tidal_inputs)
///
/// \brief Helper method to check inputs to the Tidal constructor.
///
/// Ref: \cite Bir_2011 \n
/// Ref: \cite Lewis_2021 \n
///

void Tidal :: __checkInputs(TidalInputs tidal_inputs)
{
    //  1. check design_speed_ms
    if (tidal_inputs.design_speed_ms <= 0) {
        std::string error_str = "ERROR:  Tidal():  ";
        error_str += "TidalInputs::design_speed_ms must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    else if (tidal_inputs.design_speed_ms < 2) {
        std::string warning_str = "WARNING:  Tidal():  ";
        warning_str += "Setting TidalInputs::design_speed_ms to less than 2 m/s may be ";
        warning_str += "technically unrealistic";
        
        std::cout << warning_str << std::endl;
    }
    
    //  2. check firmness_factor
    if (
        tidal_inputs.firmness_factor < 0 or
        tidal_inputs.firmness_factor > 1
    ) {
        std::string error_str = "ERROR:  Tidal():  ";
        error_str += "TidalInputs::firmness_factor must be in the closed interval [0, 1]";
        
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
/// \fn double Tidal :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic tidal turbine capital cost.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic capital cost for the tidal turbine [CAD].
///

double Tidal :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 2000 * pow(this->capacity_kW, -0.15) + 4000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic tidal turbine operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     tidal turbine [CAD/kWh].
///

double Tidal :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0.05 * pow(this->capacity_kW, -0.2) + 0.05;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: __computeCubicProductionkW(
///         int timestep,
///         double dt_hrs,
///         double tidal_resource_ms
///     )
///
/// \brief Helper method to compute tidal turbine production under a cubic
///     production model.
///
/// Ref: \cite WeiWaiKum_2023 \n
/// Ref: \cite Bir_2011 \n
/// Ref: \cite Lewis_2021 \n
/// Ref: \cite Whitby_2013 \n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param tidal_resource_ms The available tidal stream resource [m/s].
///
/// \return The production [kW] of the tidal turbine, under a cubic model.
///

double Tidal :: __computeCubicProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    double production = 0;
    
    if (
        tidal_resource_ms < 0.15 * this->design_speed_ms or
        tidal_resource_ms > 1.25 * this->design_speed_ms
    ){
        production = 0;
    }
    
    else if (
        0.15 * this->design_speed_ms <= tidal_resource_ms and
        tidal_resource_ms <= this->design_speed_ms
    ) {
        production = (1 / pow(this->design_speed_ms, 3)) * pow(tidal_resource_ms, 3);
    }
    
    else {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeCubicProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: __computeExponentialProductionkW(
///         int timestep,
///         double dt_hrs,
///         double tidal_resource_ms
///     )
///
/// \brief Helper method to compute tidal turbine production under an exponential
///     production model.
///
/// Ref: \cite WindTidalWave_2019 \n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param tidal_resource_ms The available tidal stream resource [m/s].
///
/// \return The production [kW] of the tidal turbine, under an exponential model.
///

double Tidal :: __computeExponentialProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    double production = 0;
    
    double turbine_speed =
        (tidal_resource_ms - this->design_speed_ms) / this->design_speed_ms;
        
    if (turbine_speed < -0.71 or turbine_speed > 0.65) {
        production = 0;
    }
    
    else if (turbine_speed >= -0.71 and turbine_speed <= 0) {
        production = 1.69215 * exp(1.25909 * turbine_speed) - 0.69215;
    }
    
    else {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeExponentialProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: __computeLookupProductionkW(
///         int timestep,
///         double dt_hrs,
///         double tidal_resource_ms
///     )
///
/// \brief Helper method to compute tidal turbine production by way of
///     looking up using given power curve data.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param tidal_resource_ms The available tidal stream resource [m/s].
///
/// \return The interpolated production [kW] of the tidal tubrine.
///

double Tidal :: __computeLookupProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    // *** WORK IN PROGRESS *** //
    
    return 0;
}   /* __computeLookupProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Tidal :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Tidal.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Tidal :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW TIDAL Summary Results\n";
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
    
    //  2.2. Renewable attributes
    ofs << "## Renewable Attributes\n";
    ofs << "\n";
    
    ofs << "Resource Key (1D): " << this->resource_key << "  \n";
    ofs << "Firmness Factor: " << this->firmness_factor << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. Tidal attributes
    ofs << "## Tidal Attributes\n";
    ofs << "\n";
    
    ofs << "Power Production Model: " << this->power_model_string << "  \n";
    ofs << "Design Speed: " << this->design_speed_ms << " m/s  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.4. Tidal Results
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
    
    ofs << "\n--------\n\n";
    
    ofs.close();
    
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Tidal :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         std::map<int, std::vector<double>>* resource_map_1D_ptr,
///         std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Tidal.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param resource_map_1D_ptr A pointer to the 1D map of Resources.
///
/// \param resource_map_2D_ptr A pointer to the 2D map of Resources.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Tidal :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    std::map<int, std::vector<double>>* resource_map_1D_ptr,
    std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
    int max_lines
)
{
    //  1. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Tidal Resource [m/s],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        
        if (not this->normalized_production_series_given) {
            ofs << resource_map_1D_ptr->at(this->resource_key)[i] << ",";
        }
        
        else {
            ofs << "OVERRIDE" << ",";
        }
        
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
        ofs << this->capital_cost_vec[i] << ",";
        ofs << this->operation_maintenance_cost_vec[i] << ",";
        ofs << "\n";
    }
    
    return;
}   /* __writeTimeSeries() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Tidal :: Tidal(void)
///
/// \brief Constructor (dummy) for the Tidal class.
///

Tidal :: Tidal(void)
{
    return;
}   /* Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Tidal :: Tidal(
///         int n_points,
///         double n_years,
///         TidalInputs tidal_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Tidal class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param tidal_inputs A structure of Tidal constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Tidal :: Tidal(
    int n_points,
    double n_years,
    TidalInputs tidal_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Renewable(
    n_points,
    n_years,
    tidal_inputs.renewable_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(tidal_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: TIDAL;
    this->type_str = "TIDAL";
    
    this->resource_key = tidal_inputs.resource_key;
    
    this->firmness_factor = tidal_inputs.firmness_factor;
    
    this->design_speed_ms = tidal_inputs.design_speed_ms;
    
    this->power_model = tidal_inputs.power_model;
    
    switch (this->power_model) {
        case (TidalPowerProductionModel :: TIDAL_POWER_CUBIC): {
            this->power_model_string = "CUBIC";
            
            break;
        }
        
        case (TidalPowerProductionModel :: TIDAL_POWER_EXPONENTIAL): {
            this->power_model_string = "EXPONENTIAL";
            
            break;
        }
        
        case (TidalPowerProductionModel :: TIDAL_POWER_LOOKUP): {
            this->power_model_string = "LOOKUP";
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Tidal():  ";
            error_str += "power production model ";
            error_str += std::to_string(this->power_model);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    if (tidal_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = tidal_inputs.capital_cost;
    }
    
    if (tidal_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            tidal_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Tidal object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Tidal :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Tidal :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Renewable :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double tidal_resource_ms
///     )
///
/// \brief Method which takes in the tidal resource at a particular point in time, and
///     then returns the tidal turbine production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param tidal_resource_ms Tidal resource (i.e. tidal stream speed) [m/s].
///
/// \return The production [kW] of the tidal turbine.
///

double Tidal :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    //  given production time series override
    if (this->normalized_production_series_given) {
        double production_kW = Production :: getProductionkW(timestep);
        
        return production_kW;
    }
    
    // check if no resource
    if (tidal_resource_ms <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (TidalPowerProductionModel :: TIDAL_POWER_CUBIC): {
            production_kW = this->__computeCubicProductionkW(
                timestep,
                dt_hrs,
                tidal_resource_ms
            );
            
            break;
        }
        
        
        case (TidalPowerProductionModel :: TIDAL_POWER_EXPONENTIAL): {
            production_kW = this->__computeExponentialProductionkW(
                timestep,
                dt_hrs,
                tidal_resource_ms
            );
            
            break;
        }
        
        case (TidalPowerProductionModel :: TIDAL_POWER_LOOKUP): {
            production_kW = this->__computeLookupProductionkW(
                timestep,
                dt_hrs,
                tidal_resource_ms
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Tidal::computeProductionkW():  ";
            error_str += "power model ";
            error_str += std::to_string(this->power_model);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: commit(
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

double Tidal :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Renewable :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    
    //...
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Tidal :: ~Tidal(void)
///
/// \brief Destructor for the Tidal class.
///

Tidal :: ~Tidal(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Tidal object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Tidal() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
