/*
 * PGMcpp : PRIMED Grid ThermalModelling (in C++)
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
/// \file ThermalModel.h
///
/// \brief Header file for the ThermalModel class.
///


#ifndef THERMALMODEL_H
#define THERMALMODEL_H

// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"

// storage
#include "Storage/LiIon.h"
#include "Storage/H2.h"
#include "Storage/Electrolyzer.h"
#include "Storage/FuelCell.h"

#include <iostream>
#include <vector>


///
/// \struct ThermalModelInputs
///
/// \brief A structure which bundles the necessary inputs for the ThermalModel constructor.
///

struct ThermalModelInputs {

    std::string path_2_environmental_temperature_time_series = "../data/sanirajak_24h/environmental_temperature/sanirajak_environmental_temperature.csv"; ///< A string defining the path (either relative or absolute) to the given normalized temperature time series.

    double R_storage_housing = 6; /// Thermal resistance in walls of storage housing [m^2.K/W]
    double A_storage_housing = 28; /// Approximate wall area of a 10 foot shipping container [m^2]
    double V_storage_housing = 15.94; /// Approximate volume of a 10 foot shipping container [m^3]
    double cp_air = 1005; /// spceific heat capacity of air [J/kg.K]
    double x_air = 0.7; /// fraction of air in the housing 
    double p_air = 1.225; /// density of air [kg/m^3]
    double T_room_initial = 25;
    double T_storage_housing_min_h2 = 5; /// minimum storage housing temperature [degC] of the hydrogen assets
    double T_storage_housing_max_h2 = 80; /// maximum storage housing temperature [degC] of the hydrogen assets
    double T_storage_housing_min_liion = 15; /// minimum storage housing temperature [degC] of the LiIon asset
    double T_storage_housing_max_liion = 25; /// maximum storage housing temperature [degC] of the LiIon asset
    double n_heat_transfer = 0.8;
    double n_storage_heat_transfer = 0.9;
};

///
/// \class ThermalModel
///

class ThermalModel {
    private:
        //  1. attributes
        
        //  2. methods
        //...
        
        
    public:
        //  1. attributes
        int n_points; ///< The number of points in the modelling time series.
        double R_storage_housing; /// Thermal resistance in walls of storage housing [m^2.K/W]
        double A_storage_housing; /// Approximate wall area of a 10 foot shipping container [m^2]
        double V_storage_housing; /// Approximate volume of a 10 foot shipping container [m^3]
        double cp_air; /// spceific heat capacity of air [J/kg.K]
        double x_air; /// fraction of air in the housing 
        double p_air; /// density of air [kg/m^3]
        double m_air;
        double T_room_initial;
        double T_room_h2;
        double T_room_liion;
        double T_storage_housing_min_h2; /// minimum storage housing temperature [degC] of the hydrogen assets
        double T_storage_housing_max_h2; /// maximum storage housing temperature [degC] of the hydrogen assets
        double T_storage_housing_min_liion; /// minimum storage housing temperature [degC] of the LiIon asset
        double T_storage_housing_max_liion; /// maximum storage housing temperature [degC] of the LiIon asset

        bool sending_Q_to_liion; 
        double Q_h2_to_liion_kW;
        double Q_req_h2_kW;
        double Q_req_liion_kW;
        double Q_external_heating_potential_kW;
        double Q_storage_heating_load_kW;
        double n_heat_transfer;
        double n_storage_heat_transfer;

        std::string path_2_environmental_temperature_time_series; ///< A string defining the path (either relative or absolute) to the given normalized temperature time series.
        
        std::vector<Storage*> storage_ptr_vec;  ///< A vector of pointers to the various Storage assets in the ThermalModel
               
        std::vector<double> T_room_h2_vec_C;       
        std::vector<double> H2_thermal_out_vec_kW;
        std::vector<double> T_room_liion_vec_C;       
        std::vector<double> T_env_vec_C;

        std::vector<double> Q_req_liion_vec_kW;       
        std::vector<double> Q_req_h2_vec_kW;
        std::vector<double> Q_req_remaining_vec_kW;   
        std::vector<double> Q_h2_to_liion_vec_kW;

        std::vector<double> Q_heating_external_vec_kW;
        std::vector<double> Q_load_vec_kW;       

        //  2. methods
        ThermalModel(void);
        ThermalModel(ThermalModelInputs);
        
        void commitLiIonThermalTracking(int,double,std::vector<Storage*>*); // takes the thermal energy balance for LiIon system and computes required heating/cooling
        void commitH2ThermalTracking(int,double,std::vector<Storage*>*); // takes the thermal energy balance for H2 system and computes required heating/cooling
        void commitThermalBalance(int,double); // takes the thermal energy outputs from each asset and computes overall energy balance
        void __readTemperatureData(std::string); // reads in environemtnal temperature data
        void __checkInputs(ThermalModelInputs);
        void __writeSummary(std::string);
        void __writeTimeSeries(std::string, std::vector<double>*, int = -1);
        
        void clear(void);
        
        ~ThermalModel(void);
        
};  /* ThermalModel */


#endif  /* ThermalModel_H */
