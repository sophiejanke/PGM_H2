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
/// \file LiIon.cpp
///
/// \brief Implementation file for the LiIon class.
///
/// A derived class of Storage which models energy storage by way of lithium-ion
/// batteries.
///


#include "../../header/Storage/LiIon.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __checkInputs(LiIonInputs liion_inputs)
///
/// \brief Helper method to check inputs to the LiIon constructor.
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

void LiIon :: __checkInputs(LiIonInputs liion_inputs)
{
    //  1. check replace_SOH
    if (liion_inputs.replace_SOH < 0 or liion_inputs.replace_SOH > 1) {
        std::string error_str = "ERROR:  LiIon():  replace_SOH must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check init_SOC
    if (liion_inputs.init_SOC < 0 or liion_inputs.init_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  init_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check min_SOC
    if (liion_inputs.min_SOC < 0 or liion_inputs.min_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  min_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check hysteresis_SOC
    if (liion_inputs.hysteresis_SOC < 0 or liion_inputs.hysteresis_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  hysteresis_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  5. check max_SOC
    if (liion_inputs.max_SOC < 0 or liion_inputs.max_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  max_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  6. check charging_efficiency
    if (liion_inputs.charging_efficiency <= 0 or liion_inputs.charging_efficiency > 1) {
        std::string error_str = "ERROR:  LiIon():  charging_efficiency must be in the ";
        error_str += "half-open interval (0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  7. check discharging_efficiency
    if (
        liion_inputs.discharging_efficiency <= 0 or
        liion_inputs.discharging_efficiency > 1
    ) {
        std::string error_str = "ERROR:  LiIon():  discharging_efficiency must be in the ";
        error_str += "half-open interval (0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  8. check degradation_alpha
    if (liion_inputs.degradation_alpha <= 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_alpha must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  9. check degradation_beta
    if (liion_inputs.degradation_beta <= 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_beta must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  10. check degradation_B_hat_cal_0
    if (liion_inputs.degradation_B_hat_cal_0 <= 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_B_hat_cal_0 must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  11. check degradation_r_cal
    if (liion_inputs.degradation_r_cal < 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_r_cal must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  12. check degradation_Ea_cal_0
    if (liion_inputs.degradation_Ea_cal_0 <= 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_Ea_cal_0 must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  13. check degradation_a_cal
    if (liion_inputs.degradation_a_cal < 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_a_cal must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  14. check degradation_s_cal 
    if (liion_inputs.degradation_s_cal < 0) {
        std::string error_str = "ERROR:  LiIon():  degradation_s_cal must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  15. check gas_constant_JmolK
    if (liion_inputs.gas_constant_JmolK <= 0) {
        std::string error_str = "ERROR:  LiIon():  gas_constant_JmolK must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  16. check temperature_K
    if (liion_inputs.temperature_K < 0) {
        std::string error_str = "ERROR:  LiIon():  temperature_K must be >= 0";
        
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
/// \fn double LiIon :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic lithium ion battery energy storage system
///      capital cost.
///
/// This model was obtained by way of surveying an assortment of published lithium ion
/// battery energy storage system costs, and then constructing a best fit model. Note
/// that this model expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the lithium ion battery energy storage system
///     [CAD].
///

double LiIon :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kWh = 250 * pow(this->energy_capacity_kWh, -0.15) + 650;
    
    return capital_cost_per_kWh * this->energy_capacity_kWh;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic lithium ion battery energy storage system
///     operation and maintenance cost. This is a cost incurred per unit energy
///     charged/discharged.
///
/// This model was obtained by way of surveying an assortment of published lithium ion
/// battery energy storage system costs, and then constructing a best fit model. Note
/// that this model expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// \return A generic operation and maintenance cost, per unit energy
///     charged/discharged, for the lithium ion battery energy storage system [CAD/kWh].
///

double LiIon :: __getGenericOpMaintCost(void)
{
    return 0.01;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __toggleDepleted(void)
///
/// \brief Helper method to toggle the is_depleted attribute of LiIon.
///

void LiIon :: __toggleDepleted(void)
{
    if (this->is_depleted) {
        double hysteresis_charge_kWh = this->hysteresis_SOC * this->energy_capacity_kWh;
    
        if (hysteresis_charge_kWh > this->dynamic_energy_capacity_kWh) {
            hysteresis_charge_kWh = this->dynamic_energy_capacity_kWh;
        }
        
        if (this->charge_kWh >= hysteresis_charge_kWh) {
            this->is_depleted = false;
        }
    }
    
    else {
        double min_charge_kWh = this->min_SOC * this->energy_capacity_kWh;
        
        if (this->charge_kWh <= min_charge_kWh) {
            this->is_depleted = true;
        }
    }
    
    return;
}   /* __toggleDepleted() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __handleDegradation(
///         int timestep,
///         double dt_hrs,
///         double charging_discharging_kW
///     )
///
/// \brief Helper method to apply degradation modelling and update attributes.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_discharging_kW The charging/discharging power [kw] being sent to
///     the asset.
///


void LiIon :: __handleDegradation(
    int timestep,
    double dt_hrs,
    double charging_discharging_kW
)
{
    //  1. model degradation
    this->__modelDegradation(dt_hrs, charging_discharging_kW);
    
    //  2. update and record
    this->SOH_vec[timestep] = this->SOH;
    this->dynamic_energy_capacity_kWh = this->SOH * this->energy_capacity_kWh;
    
    if (this->power_degradation_flag) {
        this->dynamic_power_capacity_kW = this->SOH * this->power_capacity_kW;
    }
    
    return;
}   /* __handleDegradation() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __modelDegradation(double dt_hrs, double charging_discharging_kW)
///
/// \brief Helper method to model energy capacity degradation as a function of operating
///     state.
///
/// Ref: \cite BatteryDegradation_2023\n
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_discharging_kW The charging/discharging power [kw] being sent to
///     the asset.
///

void LiIon :: __modelDegradation(double dt_hrs, double charging_discharging_kW)
{
    //  1. compute SOC
    double SOC = this->charge_kWh / this->energy_capacity_kWh;
    
    //  2. compute C-rate and corresponding acceleration factor
    double C_rate = charging_discharging_kW / this->power_capacity_kW;
    
    double C_acceleration_factor =
        1 + this->degradation_alpha * pow(C_rate, this->degradation_beta);
    
    //  3. compute dSOH / dt
    double B_cal = __getBcal(SOC);
    double Ea_cal = __getEacal(SOC);
    
    double dSOH_dt = B_cal *
        exp((-1 * Ea_cal) / (this->gas_constant_JmolK * this->temperature_K));
    
    dSOH_dt *= dSOH_dt;
    dSOH_dt *= 1 / (2 * this->SOH);
    dSOH_dt *= C_acceleration_factor;
    
    //  4. update state of health
    this->SOH -= dSOH_dt * dt_hrs;
    
    return;
}   /* __modelDegradation() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: __getBcal(double SOC)
///
/// \brief Helper method to compute and return the base pre-exponential factor for a 
///     given state of charge.
///
/// Ref: \cite BatteryDegradation_2023\n
///
/// \param SOC The current state of charge of the asset.
///
/// \return The base pre-exponential factor for the given state of charge.
///

double LiIon :: __getBcal(double SOC)
{
    double B_cal = this->degradation_B_hat_cal_0 *
        exp(this->degradation_r_cal * SOC);
    
    return B_cal;
}   /* __getBcal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: __getEacal(double SOC)
///
/// \brief Helper method to compute and return the activation energy value for a given
///     state of charge.
///
/// Ref: \cite BatteryDegradation_2023\n
///
/// \param SOC The current state of charge of the asset.
///
/// \return The activation energy value for the given state of charge.
///

double LiIon :: __getEacal(double SOC)
{
    double Ea_cal = this->degradation_Ea_cal_0;
    
    Ea_cal -= this->degradation_a_cal *
        (exp(this->degradation_s_cal * SOC) - 1);
    
    return Ea_cal;
}   /* __getEacal( */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for LiIon.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void LiIon :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->power_capacity_kW)));
    ofs << " kW ";
    ofs << std::to_string(int(ceil(this->energy_capacity_kWh)));
    ofs << " kWh LIION Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Storage attributes
    ofs << "## Storage Attributes\n";
    ofs << "\n";
    ofs << "Power Capacity: " << this->power_capacity_kW << " kW  \n";
    ofs << "Energy Capacity: " << this->energy_capacity_kWh << " kWh  \n";
    ofs << "\n";
    
    ofs << "Sunk Cost (N = 0 / Y = 1): " << this->is_sunk << "  \n";
    ofs << "Capital Cost: " << this->capital_cost << "  \n";
    ofs << "Operation and Maintenance Cost: " << this->operation_maintenance_cost_kWh
        << " per kWh charged/discharged  \n";
    ofs << "Nominal Inflation Rate (annual): " << this->nominal_inflation_annual
        << "  \n";
    ofs << "Nominal Discount Rate (annual): " << this->nominal_discount_annual
        << "  \n";
    ofs << "Real Discount Rate (annual): " << this->real_discount_annual << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.2. LiIon attributes
    ofs << "## LiIon Attributes\n";
    ofs << "\n";
    
    ofs << "Charging Efficiency: " << this->charging_efficiency << "  \n";
    ofs << "Discharging Efficiency: " << this->discharging_efficiency << "  \n";
    ofs << "\n";
    
    ofs << "Initial State of Charge: " << this->init_SOC << "  \n";
    ofs << "Minimum State of Charge: " << this->min_SOC << "  \n";
    ofs << "Hyteresis State of Charge: " << this->hysteresis_SOC << "  \n";
    ofs << "Maximum State of Charge: " << this->max_SOC << "  \n";
    ofs << "\n";
    
    ofs << "Replacement State of Health: " << this->replace_SOH << "  \n";
    ofs << "\n";
    
    ofs << "Degradation Acceleration Coeff.: " << this->degradation_alpha << "  \n";
    ofs << "Degradation Acceleration Exp.: " << this->degradation_beta << "  \n";
    ofs << "Degradation Base Pre-Exponential Factor: "
        << this->degradation_B_hat_cal_0 << " 1/sqrt(hrs)  \n";
    ofs << "Degradation Dimensionless Constant (r_cal): "
        << this->degradation_r_cal << "  \n";
    ofs << "Degradation Base Activation Energy: "
        << this->degradation_Ea_cal_0 << " J/mol  \n";
    ofs << "Degradation Pre-Exponential Factor: "
        << this->degradation_a_cal << " J/mol  \n";
    ofs << "Degradation Dimensionless Constant (s_cal): "
        << this->degradation_s_cal << "  \n";
    ofs << "Universal Gas Constant: " << this->gas_constant_JmolK
        << " J/mol.K  \n";
    ofs << "Absolute Environmental Temperature: " << this->temperature_K << " K  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. LiIon Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Discharge: " << this->total_discharge_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched  \n";
    ofs << "\n";
    
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    ofs << "\n--------\n\n";
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for LiIon.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the
///     ElectricalLoad.
///
/// \param max_lines The maximum number of lines of output to write.
///

void LiIon :: __writeTimeSeries(
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
    ofs << "Charging Power [kW],";
    ofs << "Discharging Power [kW],";
    ofs << "Charge (at end of timestep) [kWh],";
    ofs << "State of Health (at end of timestep) [ ],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        ofs << this->charging_power_vec_kW[i] << ",";
        ofs << this->discharging_power_vec_kW[i] << ",";
        ofs << this->charge_vec_kWh[i] << ",";
        ofs << this->SOH_vec[i] << ",";
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
/// \fn LiIon :: LiIon(void)
///
/// \brief Constructor (dummy) for the LiIon class.
///

LiIon :: LiIon(void)
{
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: LiIon(
///         int n_points,
///         double n_years,
///         LiIonInputs liion_inputs
///     )
///
/// \brief Constructor (intended) for the LiIon class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

LiIon :: LiIon(
    int n_points,
    double n_years,
    LiIonInputs liion_inputs
) :
Storage(
    n_points,
    n_years,
    liion_inputs.storage_inputs
)
{
    //  1. check inputs
    this->__checkInputs(liion_inputs);
    
    //  2. set attributes
    this->type = StorageType :: LIION;
    this->type_str = "LIION";
    
    this->dynamic_energy_capacity_kWh = this->energy_capacity_kWh;
    this->dynamic_power_capacity_kW = this->power_capacity_kW;
    
    this->SOH = 1;
    this->power_degradation_flag = liion_inputs.power_degradation_flag;
    this->replace_SOH = liion_inputs.replace_SOH;
    
    this->degradation_alpha = liion_inputs.degradation_alpha;
    this->degradation_beta = liion_inputs.degradation_beta;
    this->degradation_B_hat_cal_0 = liion_inputs.degradation_B_hat_cal_0;
    this->degradation_r_cal = liion_inputs.degradation_r_cal;
    this->degradation_Ea_cal_0 = liion_inputs.degradation_Ea_cal_0;
    this->degradation_a_cal = liion_inputs.degradation_a_cal;
    this->degradation_s_cal = liion_inputs.degradation_s_cal;
    this->gas_constant_JmolK = liion_inputs.gas_constant_JmolK;
    this->temperature_K = liion_inputs.temperature_K;
    this->p_liion = liion_inputs.p_liion;
    this->cp_liion = liion_inputs.cp_liion;
    this->self_discharge_rate = liion_inputs.self_discharge_rate;
    
    this->init_SOC = liion_inputs.init_SOC;
    this->charge_kWh = this->init_SOC * this->energy_capacity_kWh;

    this->min_SOC = liion_inputs.min_SOC;
    this->hysteresis_SOC = liion_inputs.hysteresis_SOC;
    this->max_SOC = liion_inputs.max_SOC;
    
    this->charging_efficiency = liion_inputs.charging_efficiency;
    this->discharging_efficiency = liion_inputs.discharging_efficiency;
    
    if (liion_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = liion_inputs.capital_cost;
    }
    
    if (liion_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            liion_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    this->SOH_vec.resize(this->n_points, 0);
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "LiIon object constructed at " << this << std::endl;
    }
    
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void LiIon :: handleReplacement(int timestep)
{
    //  1. reset attributes
    this->dynamic_energy_capacity_kWh = this->energy_capacity_kWh;
    this->dynamic_power_capacity_kW = this->power_capacity_kW;
    this->SOH = 1;
    
    // 2. invoke base class method
    Storage::handleReplacement(timestep);
    
    //  3. correct attributes
    this->charge_kWh = this->init_SOC * this->energy_capacity_kWh;
    this->is_depleted = false;
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: commit_SelfDischarge(int timestep, double dt_hrs)
///
/// \brief Method to get account for self-discharge of battery asset
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param timestep The current time step of the Model run.
///
/// \return The discharging power [kW] currently available from the asset.
///

void LiIon :: commit_SelfDischarge(int timestep, double dt_hrs)
{
    //  account for self-discharge if asset is not discharging or charging 
    this->charge_kWh -= (this->charge_kWh * this->self_discharge_rate / 720 * dt_hrs);
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    return;
}   /* getAvailablekW() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAvailablekW(int timestep, double dt_hrs)
///
/// \brief Method to get the discharge power currently available from the asset.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param timestep The current time step of the Model run.
///
/// \return The discharging power [kW] currently available from the asset.
///

double LiIon :: getAvailablekW(int timestep, double dt_hrs)
{
    //  1. get min charge
    double min_charge_kWh = this->min_SOC * this->energy_capacity_kWh;
    
    //  2. compute available power
    double available_kW =
        ((this->charge_kWh - min_charge_kWh) * this->discharging_efficiency) /
        dt_hrs;
    
    //  3. apply power constraint
    if (available_kW > this->dynamic_power_capacity_kW) {
        available_kW = this->dynamic_power_capacity_kW;
    }
    
    //  4. account for power already being discharged
    available_kW -= this->power_kW;
    
    if (available_kW <= 0) {
        return 0;
    }
    
    return available_kW;
}   /* getAvailablekW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAcceptablekW(double dt_hrs)
///
/// \brief Method to get the charge power currently acceptable by the asset.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The charging power [kW] currently acceptable by the asset.
///

double LiIon :: getAcceptablekW(int timestep, double dt_hrs)
{
    //  1. get max charge
    double max_charge_kWh = this->max_SOC * this->energy_capacity_kWh;
    
    if (max_charge_kWh > this->dynamic_energy_capacity_kWh) {
        max_charge_kWh = this->dynamic_energy_capacity_kWh;
    }
    
    //  2. compute acceptable power
    double acceptable_kW = 
        (max_charge_kWh - this->charge_kWh) /
        (this->charging_efficiency * dt_hrs);
    
    //  3. apply power constraint
    if (acceptable_kW > this->dynamic_power_capacity_kW) {
        acceptable_kW = this->dynamic_power_capacity_kW;
    }
    
    //  4. account for power already being charged
    acceptable_kW -= this->power_kW;
    
    if (acceptable_kW <= 0) {
        return 0;
    }
    
    return acceptable_kW;
}   /* getAcceptablekW( */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: commitCharge(
///         int timestep,
///         double dt_hrs,
///         double charge_kW
///     )
///
/// \brief Method which takes in the charging power for the current timestep and
///     records.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_kW The charging power [kw] being sent to the asset.
///

void LiIon :: commitCharge(
    int timestep,
    double dt_hrs,
    double charging_kW
)
{
    //  1. record charging power
    this->charging_power_vec_kW[timestep] = charging_kW;
    
    //  2. update charge and record
    this->charge_kWh += this->charging_efficiency * charging_kW * dt_hrs;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    //  3. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  4. model degradation
    this->__handleDegradation(timestep, dt_hrs, charging_kW);
    
    //  5. trigger replacement (if applicable)
    if (this->SOH <= this->replace_SOH) {
        this->handleReplacement(timestep);
    }
    
    //  6. capture operation and maintenance costs (if applicable)
    if (charging_kW > 0) {
        this->operation_maintenance_cost_vec[timestep] = charging_kW * dt_hrs *
            this->operation_maintenance_cost_kWh;
    }
    
    this->power_kW= 0;
    return;
}   /* commitCharge() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: commitDischarge(
///         int timestep,
///         double dt_hrs,
///         double discharging_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in the discharging power for the current timestep and
///     records. Returns the load remaining after discharge.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param discharging_kW The discharging power [kw] being drawn from the asset.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the discharge is deducted from it.
///

double LiIon :: commitDischarge(
    int timestep,
    double dt_hrs,
    double discharging_kW,
    double load_kW
)
{
    //  1. record discharging power, update total
    this->discharging_power_vec_kW[timestep] = discharging_kW;
    this->total_discharge_kWh += discharging_kW * dt_hrs;
    
    //  2. update charge and record
    this->charge_kWh -= (discharging_kW * dt_hrs) / this->discharging_efficiency;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    //  3. update load
    load_kW -= discharging_kW;
    
    //  4. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  5. model degradation
    this->__handleDegradation(timestep, dt_hrs, discharging_kW);
    
    //  6. trigger replacement (if applicable)
    if (this->SOH <= this->replace_SOH) {
        this->handleReplacement(timestep);
    }
    
    //  7. capture operation and maintenance costs (if applicable)
    if (discharging_kW > 0) {
        this->operation_maintenance_cost_vec[timestep] = discharging_kW * dt_hrs *
            this->operation_maintenance_cost_kWh;
    }
    
    this->power_kW = 0;
    return load_kW;
}   /* commitDischarge() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: getThermalOutput(int);
///
/// \brief Method to get the net thermal energy of the electrolyzer
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The thermal enery output of the fuel cell asset [kW]
///

double LiIon :: getThermalOutput(
    int timestep,
    double dt_hrs
) 
{
    double thermal_output_kWh = 0;
    // get  fuel cell thermal energy output for thermal energy modelling
    if (this->charging_power_vec_kW[timestep] > 0){
        thermal_output_kWh = this->charging_power_vec_kW[timestep] * (1 - this->charging_efficiency);
    }

    if (this->discharging_power_vec_kW[timestep] > 0){
        thermal_output_kWh = this->discharging_power_vec_kW[timestep] * (1 - this->discharging_efficiency);
    }

    return thermal_output_kWh;
}

// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: getLiIon_mcp(int);
///
/// \brief Method to get the m * cp of the lithium ion battery 
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The m * cp of the lithium ion battery [J/K]
///

double LiIon :: getMcp(
    int timestep
) 
{
    // calculate m * cp for electrolyzer for thermal energy modelling
    double mcp = this->energy_capacity_kWh * this->p_liion * this->cp_liion;

    return mcp;
}

// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: ~LiIon(void)
///
/// \brief Destructor for the LiIon class.
///

LiIon :: ~LiIon(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "LiIon object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~LiIon() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
