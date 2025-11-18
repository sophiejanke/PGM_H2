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
/// \file ThermalModel.cpp
///
/// \brief Implementation file for the ThermalModel class.
///
/// A container class which forms the centre of PGMcpp. The ThermalModel class is
/// intended to serve as the primary user interface with the functionality of PGMcpp,
/// and as such it contains all other classes.
///


#include "../header/ThermalModel.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void ThermalModel :: __checkInputs(ThermalModelInputs model_inputs)
///
/// \brief Helper method (private) to check inputs to the ThermalModel constructor.
///
/// \param model_inputs A structure of ThermalModel constructor inputs.
///

void ThermalModel :: __checkInputs(ThermalModelInputs thermal_model_inputs)
{
    //  1. check path_2_environmental_temperature_time_series
    if (thermal_model_inputs.path_2_environmental_temperature_time_series.empty()) {
        std::string error_str = "ERROR:  ThermalModel():  ";
        error_str += "ThermalModelInputs::path_2_environmental_temperature_time_series cannot be empty";
        
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
/// \fn void ThermalModel:: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void ThermalModel :: __writeSummary(std::string write_path)
{
    //  1. create subdirectory
    write_path += "ThermalModel/";
    std::filesystem::create_directory(write_path);
    
    //  2. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  3. write summary results (markdown)
    ofs << "# ThermalModel Summary Results\n";
    ofs << "\n--------\n\n";

    // EDITING: add outputs for summary results like total heating and cooling needed, avg T, etc.
    
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void  ThermalModel:: __writeTimeSeries(std::string write_path, int max_lines)
///
/// \brief Helper method to write time series results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param max_lines The maximum number of lines of output to write.
///

void ThermalModel :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    int max_lines)
{
    //  1. create filestream
    write_path += "thermal_model_timeseries.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results header (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Environmental Temperature [C],";
    ofs << "Hydrogen Storage Housing Temperature [C],";
    ofs << "Battery Storage Housing Temperature [C],";
    ofs << "Hydrogen System Thermal Generation [kW],";
    ofs << "Heating Output Potential [kW],";
    ofs << "Storage Heating Load [kW],";
    ofs << "H2 heat sent to BESS [kW]";
    ofs << "\n";

    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        ofs << this->T_env_vec_C[i] << ","; 
        ofs << this->T_room_h2_vec_C[i] << ","; 
        ofs << this->T_room_liion_vec_C[i] << ","; 
        ofs << this->H2_thermal_out_vec_kW[i] << ","; 
        ofs << this->Q_heating_external_vec_kW[i] << ",";
        ofs << this->Q_load_vec_kW[i] << ",";
        ofs << this->Q_h2_to_liion_vec_kW[i] << ",";
        ofs << "\n";
    }

    ofs << "\n";
    
    ofs.close();

    return;
}   /* __writeTimeSeries() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn  ThermalModel:: Model(void)
///
/// \brief Constructor (dummy) for the Thermal Modelclass.
///

ThermalModel :: ThermalModel(void)
{
    return;
}   /* Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn  ThermalModel:: Model(ModelInputs model_inputs)
///
/// \brief Constructor (intended) for the Thermal Modelclass.
///
/// \param model_inputs A structure of Thermal Modelconstructor inputs.
///

ThermalModel :: ThermalModel(ThermalModelInputs thermal_model_inputs)
{
    //  1. check inputs

    this->__checkInputs(thermal_model_inputs);

    this->R_storage_housing = thermal_model_inputs.R_storage_housing;
    this->A_storage_housing = thermal_model_inputs.A_storage_housing;
    this->V_storage_housing = thermal_model_inputs.V_storage_housing;
    this->cp_air = thermal_model_inputs.cp_air;
    this->x_air = thermal_model_inputs.x_air;
    this->p_air = thermal_model_inputs.p_air;
    this->n_heat_transfer = thermal_model_inputs.n_heat_transfer;
    this->n_storage_heat_transfer = thermal_model_inputs.n_storage_heat_transfer;

    this->m_air = this->p_air * this->x_air * this->V_storage_housing;

    this->T_room_h2 = thermal_model_inputs.T_room_initial;
    this->T_room_liion = thermal_model_inputs.T_room_initial;
    this->T_storage_housing_min_h2 = thermal_model_inputs.T_storage_housing_min_h2;
    this->T_storage_housing_max_h2 = thermal_model_inputs.T_storage_housing_max_h2;
    this->T_storage_housing_min_liion = thermal_model_inputs.T_storage_housing_min_liion;
    this->T_storage_housing_max_liion = thermal_model_inputs.T_storage_housing_max_liion;

    this->path_2_environmental_temperature_time_series = thermal_model_inputs.path_2_environmental_temperature_time_series;

    this->sending_Q_to_liion = false;

    // if (not thermal_model_inputs.path_2_environmental_temperature_time_series.empty()) {
    //     this->path_2_environmental_temperature_time_series = true;
        
    //     this->path_2_environmental_temperature_time_series = 
    //     thermal_model_inputs.path_2_environmental_temperature_time_series;
        
    //     this->__readTemperatureData();
    // }

    this->__readTemperatureData(path_2_environmental_temperature_time_series);

    this->T_room_h2_vec_C.resize(this->n_points, 0);
    this->H2_thermal_out_vec_kW.resize(this->n_points, 0);
    this->T_room_liion_vec_C.resize(this->n_points, 0);
    this->T_env_vec_C.resize(this->n_points, 0);
    this->Q_req_liion_vec_kW.resize(this->n_points, 0);
    this->Q_req_h2_vec_kW.resize(this->n_points, 0);
    this->Q_req_remaining_vec_kW.resize(this->n_points, 0);
    this->Q_heating_external_vec_kW.resize(this->n_points, 0);
    this->Q_load_vec_kW.resize(this->n_points, 0);
    this->Q_h2_to_liion_vec_kW.resize(this->n_points, 0);

    return;
}   /* ThermalModel() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

//
/// \fn void  ThermalModel:: commitH2ThermalTracking
///
/// \brief Method to track the thermal energy balance in the hydrogen storage housing.
/// Method should be called on any time a charging or discharging phase occurs
///
/// \param h2_inputs A structure of ThermalModel constructor inputs.
///

void ThermalModel :: commitH2ThermalTracking(
    int timestep,
    double dt_hrs,
    std::vector<Storage*>* storage_ptr_vec_ptr
) 
{
    Storage* storage_ptr;

    // reset method parameters
    double mcp = 0;
    double storage_thermal_generation_kW = 0; 
    this->Q_req_h2_kW = 0;
    double Q_req_h2_ON = false;


    // Loop through assets and extract mcp and thermal generation of each for that timestep
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {

        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        // only taking thermal outputs from hydrogen assets
        if (storage_ptr->type == StorageType::H2_SYS){
            
            // reset asset terms
            double asset_mcp = 0;
            double asset_thermal_generation = 0; 

            //  get pointer
            storage_ptr = storage_ptr_vec_ptr->at(asset); 

            // get the thermal energy output and mcp 
            asset_thermal_generation = storage_ptr->getThermalOutput(timestep,dt_hrs);
            asset_mcp = storage_ptr->getMcp(timestep);

            storage_thermal_generation_kW += asset_thermal_generation;
            mcp += asset_mcp;

        }

    }
    // track thermal output of hydrogen 
    this->H2_thermal_out_vec_kW[timestep] = storage_thermal_generation_kW;
    // get the environmental temperature 
    double T_env = this->T_env_vec_C[timestep];

    // add the air in the housing to the mcp term
    mcp += (this->m_air * this->cp_air);

    // compute the energy lost thorugh the walls of the storage housing 
    double Q_loss_kW = ((this->A_storage_housing * (this->T_room_h2 - T_env)) / this->R_storage_housing) / 1000;

    //  calculate the resulting change in T_room
    double dT_room_dt = ( 1 / (mcp * 0.00000027778)) * (storage_thermal_generation_kW - Q_loss_kW) * dt_hrs;

    // compare resulting change in temperature 
    double T_room_check = this->T_room_h2 + dT_room_dt;

    // check if activity results in T_room falling outside of the operating limits 
    // if yes, flag Q_req_ON = true, otherwise Q_req_ON stays negative and the T_room undergoes the change.
    if ( T_room_check < this->T_storage_housing_min_h2 ) {
        Q_req_h2_ON = true;
        this->T_room_h2_vec_C[timestep] = this->T_storage_housing_min_h2;
    }
    else if (T_room_check > this->T_storage_housing_max_h2 ){
        Q_req_h2_ON = true; 
        this->T_room_h2_vec_C[timestep] = this->T_storage_housing_max_h2;
    }
    else{
        this->T_room_h2_vec_C[timestep] = T_room_check; 
    }

    // if Q_req_ON is true, calculate how much additional heat/cooling is required for the system
    if (Q_req_h2_ON == true){ 
        this->Q_req_h2_kW = 
        (((mcp * 0.00000027778) * (this->T_room_h2-this->T_room_h2_vec_C[timestep])) - storage_thermal_generation_kW + Q_loss_kW)* dt_hrs;
    }

    this->Q_req_h2_vec_kW[timestep] = this->Q_req_h2_kW;

    // update T_room with new value (will act as previous timestep for next call of funciton)
    this->T_room_h2 = this->T_room_h2_vec_C[timestep];
    
    return;
}   /* commitH2ThermalTracking() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

//
/// \fn void  ThermalModel:: commitLiIonThermalTracking
///
/// \brief Method to track the thermal energy balance in the battery storage housing.
/// Method should be called on any time a charging or discharging phase occurs
///
/// \param h2_inputs A structure of ThermalModel constructor inputs.
///

void ThermalModel :: commitLiIonThermalTracking(
    int timestep,
    double dt_hrs,
    std::vector<Storage*>* storage_ptr_vec_ptr
) 
{
    Storage* storage_ptr;

    // reset method parameters
    double mcp = 0;
    double storage_thermal_generation_kW = 0; 
    this->Q_req_liion_kW = 0;
    double Q_req_liion_ON = false;


    // Loop through assets and extract mcp and thermal generation of each for that timestep
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {

        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        // only taking thermal outputs from battery assets
        if (storage_ptr->type == StorageType::LIION){
        
            // reset asset terms
            double asset_mcp = 0;
            double asset_thermal_generation = 0; 

            //  get pointer
            storage_ptr = storage_ptr_vec_ptr->at(asset); 

            // get the thermal energy output and mcp 
            asset_thermal_generation = storage_ptr->getThermalOutput(timestep,dt_hrs);
            asset_mcp = storage_ptr->getMcp(timestep);

            storage_thermal_generation_kW += asset_thermal_generation;
            mcp += asset_mcp;
        }

    }
    
    // get the environmental temperature 
    double T_env = this->T_env_vec_C[timestep];

    // add the air in the housing to the mcp term
    mcp += (this->m_air * this->cp_air);

    // compute the energy lost thorugh the walls of the storage housing 
    double Q_loss_kW = ((this->A_storage_housing * (this->T_room_liion - T_env)) / this->R_storage_housing) / 1000;

    //  calculate the resulting change in T_room
    double dT_room_dt = ( 1 / (mcp * 0.00000027778)) * (storage_thermal_generation_kW - Q_loss_kW) * dt_hrs;

    // compare resulting change in temperature 
    double T_room_check = this->T_room_liion + dT_room_dt;

    // check activity reults in T_room falling outside of the operating limits
    // if yes, flag Q_req_ON = true, otherwise Q_req_ON stays negative and the T_room undergoes the change.
    if ( T_room_check < this->T_storage_housing_min_liion ) {
        Q_req_liion_ON = true;
        this->T_room_liion_vec_C[timestep] = this->T_storage_housing_min_liion;
    }
    else if (T_room_check > this->T_storage_housing_max_liion ){
        Q_req_liion_ON = true; 
        this->T_room_liion_vec_C[timestep] = this->T_storage_housing_max_liion;
    }
    else{
        this->T_room_liion_vec_C[timestep] = T_room_check; 
    }

    // if Q_req_ON is true, calculate how much additional heat/cooling is required for the system
    if (Q_req_liion_ON == true){ 
        this->Q_req_liion_kW = 
        (((mcp * 0.00000027778) * (this->T_room_liion-this->T_room_liion_vec_C[timestep])) - storage_thermal_generation_kW + Q_loss_kW)* dt_hrs;
    }

    this->Q_req_liion_vec_kW[timestep] = this->Q_req_liion_kW;

    // update T_room with new value (will act as previous timestep for next call of funciton)
    this->T_room_liion = this->T_room_liion_vec_C[timestep];
    
    return;
}   /* commitLiIonThermalTracking() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

//
/// \fn void  ThermalModel:: commitThermalBalance
///
/// \brief Method to track the overall thermal energy balance of the storage assets
/// Method should be called on any time a charging or discharging phase occurs
///
/// \param h2_inputs A structure of ThermalModel constructor inputs.
///

void ThermalModel :: commitThermalBalance(
    int timestep,
    double dt_hrs
) 
{
    double Q_req_remaining_kW = 0;
    double Q_req_liion_new_kW = 0;
    double Q_req_h2_new_kW = 0;
    // check if we can should send thermal energy from hydrogen to battery system 
    if ((this->Q_req_liion_kW>0)&&(this->Q_req_h2_kW<0)){

        this->Q_h2_to_liion_kW = this->Q_req_liion_kW*this->n_storage_heat_transfer;
    } 
    else{
        this->Q_h2_to_liion_kW = 0;
    }
    // update battery and hydrogen required thermal loads and compute remaining total
    Q_req_liion_new_kW = (this->Q_req_liion_kW - this->Q_h2_to_liion_kW);
    Q_req_h2_new_kW = (this->Q_req_h2_kW + this->Q_h2_to_liion_kW);
    Q_req_remaining_kW = Q_req_h2_new_kW + Q_req_liion_new_kW;

    this->Q_h2_to_liion_vec_kW[timestep] = this->Q_h2_to_liion_kW;
    this->Q_req_remaining_vec_kW[timestep] = Q_req_remaining_kW;
    
    if (Q_req_remaining_kW < 0){
        this->Q_heating_external_vec_kW[timestep] = this->n_heat_transfer*Q_req_remaining_kW*-1;
    } 
    else {
        this->Q_load_vec_kW[timestep] = Q_req_remaining_kW;
    }

    return;
}   /* commitThermalBalance() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //
///
/// \fn void Temperature :: __readTemperatureData(
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Helper method to read in a given time series of normalized temperature.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

// ---------------------------------------------------------------------------------- //
void ThermalModel::__readTemperatureData(std::string path_2_environmental_temperature_time_series) 
{
    //  1. clear
    // this->clear();

    //  2. init CSV reader, record path
    io::CSVReader<2> CSV(path_2_environmental_temperature_time_series);

    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Ambient Temperature [C]"
    );

    this->path_2_environmental_temperature_time_series = path_2_environmental_temperature_time_series;

    //  3. read in temperature data, 
    this->n_points = 0;
    double time_hrs = 0;
    double T_outside = 0;
    
    while (CSV.read_row(time_hrs, T_outside)) {        //  write to normalized temperature vector, increment n_points
        this->T_env_vec_C.push_back(T_outside);         // OR this->T_env_vec_C[n_points] = T_env;
        this->n_points++;
    }

    return;
}
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void  ThermalModel:: clear(void)
///
/// \brief Method to clear all attributes of the Thermal Modelobject.
///

void ThermalModel :: clear(void)
{
    //  1. reset
    this->path_2_environmental_temperature_time_series.clear();
    this->n_points = 0;
    this->T_room_h2_vec_C.clear();
    this->H2_thermal_out_vec_kW.clear();
    this->T_room_liion_vec_C.clear();
    this->T_env_vec_C.clear();
    this->Q_req_liion_vec_kW.clear();
    this->Q_req_h2_vec_kW.clear();
    this->Q_req_remaining_vec_kW.clear();
    this->Q_h2_to_liion_vec_kW.clear();
    this->Q_heating_external_vec_kW.clear();
    this->Q_load_vec_kW.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn  ThermalModel:: ~ThermalModel(void)
///
/// \brief Destructor for the Thermal ThermalModelclass.
///

ThermalModel :: ~ThermalModel(void)
{
    this->clear();
    return;
}   /* ~ThermalModel() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
