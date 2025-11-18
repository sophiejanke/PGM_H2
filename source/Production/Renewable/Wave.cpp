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
/// \file Wave.cpp
///
/// \brief Implementation file for the Wave class.
///
/// A derived class of the Renewable branch of Production which models wave
/// production.
///


#include "../../../header/Production/Renewable/Wave.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Wave :: __checkInputs(WaveInputs wave_inputs)
///
/// \brief Helper method to check inputs to the Wave constructor.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///

void Wave :: __checkInputs(WaveInputs wave_inputs)
{
    //  1. check design_significant_wave_height_m
    if (wave_inputs.design_significant_wave_height_m <= 0) {
        std::string error_str = "ERROR:  Wave():  ";
        error_str += "WaveInputs::design_significant_wave_height_m must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check design_energy_period_s
    if (wave_inputs.design_energy_period_s <= 0) {
        std::string error_str = "ERROR:  Wave():  ";
        error_str += "WaveInputs::design_energy_period_s must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. if WAVE_POWER_LOOKUP, check that path is given
    if (
        wave_inputs.power_model == WavePowerProductionModel :: WAVE_POWER_LOOKUP and
        wave_inputs.path_2_normalized_performance_matrix.empty()
    ) {
        std::string error_str = "ERROR:  Wave()  power model was set to ";
        error_str += "WavePowerProductionModel::WAVE_POWER_LOOKUP, but no path to a ";
        error_str += "normalized performance matrix was given";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check firmness_factor
    if (
        wave_inputs.firmness_factor < 0 or
        wave_inputs.firmness_factor > 1
    ) {
        std::string error_str = "ERROR:  Wave():  ";
        error_str += "WaveInputs::firmness_factor must be in the closed interval [0, 1]";
        
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
/// \fn double Wave :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic wave energy converter capital cost.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic capital cost for the wave energy converter [CAD].
///

double Wave :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 7000 * pow(this->capacity_kW, -0.15) + 5000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic wave energy converter operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic operation and maintenance cost, per unit energy produced, for
///     the wave energy converter [CAD/kWh].
///


double Wave :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0.05 * pow(this->capacity_kW, -0.2) + 0.05;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeGaussianProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production under a Gaussian
///     production model.
///
/// Ref: \cite WindTidalWave_2019 \n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The production [kW] of the wave energy converter, under an exponential
///     model.
///

double Wave:: __computeGaussianProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    double H_s_nondim = 
        (significant_wave_height_m - this->design_significant_wave_height_m) / 
        this->design_significant_wave_height_m;
        
    double T_e_nondim =
        (energy_period_s - this->design_energy_period_s) / 
        this->design_energy_period_s;
        
    double production = exp(
        -2.25119 * pow(T_e_nondim, 2) +
        3.44570 * T_e_nondim * H_s_nondim -
        4.01508 * pow(H_s_nondim, 2)
    );
    
    return production * this->capacity_kW;
}   /* __computeGaussianProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeParaboloidProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production under a paraboloid
///     production model.
///
/// Ref: \cite Robertson_2021\n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The production [kW] of the wave energy converter, under a paraboloid
///     model.
///

double Wave:: __computeParaboloidProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    // first, check for idealized wave breaking (deep water)
    if (significant_wave_height_m >= 0.2184 * pow(energy_period_s, 2)) {
        return 0;
    }
    
    // otherwise, apply generic quadratic performance model
    // (with outputs bounded to [0, 1])
    double production = 
        0.289 * significant_wave_height_m - 
        0.00111 * pow(significant_wave_height_m, 2) * energy_period_s - 
        0.0169 * energy_period_s;
    
    if (production < 0) {
        production = 0;
    } 
    
    else if (production > 1) {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeParaboloidProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeLookupProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production by way of
///     looking up using given performance matrix.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The interpolated production [kW] of the wave energy converter.
///

double Wave:: __computeLookupProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    double prod = this->interpolator.interp2D(
        0,
        significant_wave_height_m,
        energy_period_s
    );
    
    return prod * this->capacity_kW;
}   /* __computeLookupProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Wave :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Wave.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Wave :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW WAVE Summary Results\n";
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
    
    ofs << "Resource Key (2D): " << this->resource_key << "  \n";
    ofs << "Firmness Factor: " << this->firmness_factor << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. Wave attributes
    ofs << "## Wave Attributes\n";
    ofs << "\n";
    
    ofs << "Power Production Model: " << this->power_model_string << "  \n";
    switch (this->power_model) {
        case (WavePowerProductionModel :: WAVE_POWER_GAUSSIAN): {
            ofs << "Design Significant Wave Height: "
                << this->design_significant_wave_height_m << " m  \n";
            
            ofs << "Design Energy Period: " << this->design_energy_period_s << " s  \n";
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_LOOKUP): {
            ofs << "Normalized Performance Matrix: "
                << this->interpolator.path_map_2D[0] << "  \n";
            
            break;
        }
        
        default: {
            // write nothing!
            
            break;
        }
    }
    
    ofs << "\n--------\n\n";
    
    //  2.4. Wave Results
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
/// \fn void Wave :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         std::map<int, std::vector<double>>* resource_map_1D_ptr,
///         std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Wave.
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

void Wave :: __writeTimeSeries(
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
    ofs << "Significant Wave Height [m],";
    ofs << "Energy Period [s],";
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
            ofs << resource_map_2D_ptr->at(this->resource_key)[i][0] << ",";
            ofs << resource_map_2D_ptr->at(this->resource_key)[i][1] << ",";
        }
        
        else {
            ofs << "OVERRIDE" << ",";
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
/// \fn Wave :: Wave(void)
///
/// \brief Constructor (dummy) for the Wave class.
///
Wave :: Wave(void)
{
    return;
}   /* Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Wave :: Wave(
///         int n_points,
///         double n_years,
///         WaveInputs wave_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Wave class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Wave :: Wave(
    int n_points,
    double n_years,
    WaveInputs wave_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Renewable(
    n_points,
    n_years,
    wave_inputs.renewable_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(wave_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: WAVE;
    this->type_str = "WAVE";
    
    this->resource_key = wave_inputs.resource_key;
    
    this->firmness_factor = wave_inputs.firmness_factor;
    
    this->design_significant_wave_height_m =
        wave_inputs.design_significant_wave_height_m;
    this->design_energy_period_s = wave_inputs.design_energy_period_s;
    
    this->power_model = wave_inputs.power_model;
    
    switch (this->power_model) {
        case (WavePowerProductionModel :: WAVE_POWER_GAUSSIAN): {
            this->power_model_string = "GAUSSIAN";
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_PARABOLOID): {
            this->power_model_string = "PARABOLOID";
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_LOOKUP): {
            this->power_model_string = "LOOKUP";
            
            this->interpolator.addData2D(
                0,
                wave_inputs.path_2_normalized_performance_matrix
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Wave():  ";
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
    
    if (wave_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = wave_inputs.capital_cost;
    }
    
    if (wave_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            wave_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Wave object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Wave :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Wave :: handleReplacement(int timestep)
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
/// \fn double Wave :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Method which takes in the wave resource at a particular point in time, and
///     then returns the wave turbine production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param signficiant_wave_height_m The significant wave height (wave statistic) [m].
///
/// \param energy_period_s The energy period (wave statistic) [s].
///
/// \return The production [kW] of the wave turbine.
///

double Wave :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    //  given production time series override
    if (this->normalized_production_series_given) {
        double production_kW = Production :: getProductionkW(timestep);
        
        return production_kW;
    }
    
    // check if no resource
    if (significant_wave_height_m <= 0 or energy_period_s <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (WavePowerProductionModel :: WAVE_POWER_PARABOLOID): {
            production_kW = this->__computeParaboloidProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_GAUSSIAN): {
            production_kW = this->__computeGaussianProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_LOOKUP): {
            production_kW = this->__computeLookupProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Wave::computeProductionkW():  ";
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
/// \fn double Wave :: commit(
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

double Wave :: commit(
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
/// \fn Wave :: ~Wave(void)
///
/// \brief Destructor for the Wave class.
///

Wave :: ~Wave(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Wave object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Wave() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
