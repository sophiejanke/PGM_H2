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
/// \file Noncombustion.cpp
///
/// \brief Implementation file for the Noncombustion class.
///
/// The root of the Noncombustion branch of the Production hierarchy. This branch 
/// contains derived classes which model controllable production which is not based
/// on combustion.
///


#include "../../../header/Production/Noncombustion/Noncombustion.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Noncombustion :: __checkInputs(NoncombustionInputs noncombustion_inputs)
///
/// \brief Helper method to check inputs to the Noncombustion constructor.
///
/// \param noncombustion_inputs A structure of Noncombustion constructor inputs.
///

void Noncombustion :: __checkInputs(NoncombustionInputs noncombustion_inputs)
{
    //...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Noncombustion :: __handleStartStop(
///         int timestep,
///         double dt_hrs,
///         double production_kW
///     )
///
/// \brief Helper method to handle the starting/stopping of the Noncombustion asset.
///

void Noncombustion :: __handleStartStop(
    int timestep,
    double dt_hrs,
    double production_kW
)
{
    if (this->is_running) {
        // handle stopping
        if (production_kW <= 0) {
            this->is_running = false;
        }
    }
    
    else {
        // handle starting
        if (production_kW > 0) {
            this->is_running = true;
            this->n_starts++;
        }
    }
    
    return;
}   /* __handleStartStop() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Noncombustion :: Noncombustion(void)
///
/// \brief Constructor (dummy) for the Noncombustion class.
///

Noncombustion :: Noncombustion(void)
{
    return;
}   /* Noncombustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Noncombustion :: Noncombustion(
///         int n_points,
///         double n_years,
///         NoncombustionInputs noncombustion_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Noncombustion class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param noncombustion_inputs A structure of Noncombustion constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Noncombustion :: Noncombustion(
    int n_points,
    double n_years,
    NoncombustionInputs noncombustion_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Production(
    n_points,
    n_years,
    noncombustion_inputs.production_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(noncombustion_inputs);
    
    //  2. set attributes
    //...
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Noncombustion object constructed at " << this << std::endl;
    }
    
    return;
}   /* Noncombustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Noncombustion :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Noncombustion :: handleReplacement(int timestep)
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
/// \fn void Noncombustion :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Helper method to compute key economic metrics for the Model run.
///
/// Ref: \cite HOMER_discount_factor\n
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///

void Noncombustion :: computeEconomics(std::vector<double>* time_vec_hrs_ptr)
{
    //  1. invoke base class method
    Production :: computeEconomics(time_vec_hrs_ptr);
    
    return;
}   /* computeEconomics() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Noncombustion :: commit(
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

double Noncombustion :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. handle start/stop
    this->__handleStartStop(timestep, dt_hrs, production_kW);
    
    //  2. invoke base class method
    load_kW = Production :: commit(
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
/// \fn void Noncombustion :: writeResults(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int combustion_index,
///         int max_lines
///     )
///
/// \brief Method which writes Noncombustion results to an output directory.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param noncombustion_index An integer which corresponds to the index of the
///     Noncombustion asset in the Model.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written. If =0, then only summary results are written.
///

void Noncombustion :: writeResults(
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
    
    write_path += "Noncombustion/";
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
/// \fn Noncombustion :: ~Noncombustion(void)
///
/// \brief Destructor for the Noncombustion class.
///

Noncombustion :: ~Noncombustion(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Noncombustion object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Noncombustion() */

// ---------------------------------------------------------------------------------- //


// ======== END PUBLIC ============================================================== //
