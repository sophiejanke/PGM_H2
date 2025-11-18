/*
 *  PGMcpp : PRIMED Grid Modelling (in C++)
 *
 *  Sophie Janke
 *  email:  sophiejanke@uvic.ca
 *  github: sophiejanke
 *
 *  See license terms
 *
 */


///
/// \file FuelCell.cpp
///
/// \brief Implementation file for the FuelCell class.
///
/// A class that models a fuel cell within the regenerative green hydrogen system. 
///

#include "../../header/Storage/FuelCell.h"
#include <cmath> // For exp() and log()

// Temporarily... 
// Consuming 2.5% of the energy in the process


FuelCell :: FuelCell(

    FuelCellInputs fuelcell_inputs

) {
    //  1. check inputs
    this->n_points = fuelcell_inputs.n_points;
    this->fc_capital_cost_per_kW = fuelcell_inputs.fc_capital_cost_per_kW;
    this->fc_operation_maintenance_cost_kWh = fuelcell_inputs.fc_operation_maintenance_cost_kWh;
    this->B_capacity_factor = fuelcell_inputs.B_capacity_factor;

    //  2. set attributes
    this->fc_output_vec_kW.resize(this->n_points, 0);
    this->fc_draw_vec_kW.resize(this->n_points, 0);
    this->fc_consumption_vec_kg.resize(this->n_points, 0);
    this->fc_operation_capacity_vec.resize(this->n_points, 0);
    this->Q_fc_vec_kW.resize(this->n_points, 0);
    this->N_start_stop_vec.resize(this->n_points, 0);  
    this->avg_operating_capacity_ratio_vec.resize(this->n_points, 0); 
    this->n_cap_vec.resize(this->n_points, 0); 

    this->sum_capacity_ratio = 0;      
    this->fc_SOH = 1;
    this->N_start_stop = 1;
    this->k1 = fuelcell_inputs.k1;
    this->k2 = fuelcell_inputs.k2; 
    this->k3 = fuelcell_inputs.k3; 
    this->k4 = fuelcell_inputs.k4;
    

    
    return;
}   /* FuelCell() */


// ---------------------------------------------------------------------------------- //

///
/// \fn FuelCell :: ~FuelCell(void)
///
/// \brief Destructor for the FuelCell class.
///

FuelCell :: ~FuelCell(void)
{
    //  1. destruction print
    if (0) {
        std::cout << "H2 object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~FuelCell() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //
///
/// \fn FuelCell :: FuelCell(void)
///
/// \brief Constructor (dummy) for the FuelCell class.
///

FuelCell :: FuelCell(void)
{

    return;
}   /* FuelCell() */
///
/// \fn double FuelCell :: commitDischarge(
///         int timestep,
///         double dt_hrs,
///     )
///
/// \brief Method which computes the amount of energy produced by the fuelcell at a given timestep
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
///

void FuelCell :: commitDischarge(
    int timestep,
    double dt_hrs,
    double discharging_kW,
    double fc_power_capacity
)
{
    //  1. record ouput power
    fc_output_kW = discharging_kW; 
    this->fc_output_vec_kW[timestep] = fc_output_kW;
    this->fc_operation_capacity_vec[timestep] = fc_output_kW / (fc_power_capacity);
    
    return; 
}   /* commitDischarge() */ 

/// \fn double FuelCell :: commitDraw(
///         int timestep,
///         double dt_hrs,
///     )
///
/// \brief Method which computes the amount of hydrogen consumed by the fuelcell 
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.

// ---------------------------------------------------------------------------------- //


void FuelCell :: commitDraw(
    int timestep,
    double dt_hrs,
    double fc_spec_consumption_kg, 
    double fc_quantity,
    double fc_output_kW, 
    double fc_capacity
)
{
    double fc_capacity_factor = fc_output_kW / fc_capacity;
    double n_100_capacity = (1/33.3)/fc_spec_consumption_kg;
    double n_capacity_factor = n_100_capacity * (1 + (this->B_capacity_factor * (1 - fc_capacity_factor)));
    double consumption_factor = (n_capacity_factor - n_100_capacity) / n_100_capacity;

    n_cap_vec[timestep] = n_capacity_factor;
    fc_consumption_kg = fc_spec_consumption_kg*fc_quantity*fc_output_kW*(1-consumption_factor)*dt_hrs; 
    this->fc_consumption_vec_kg[timestep] = fc_consumption_kg;
    
    return; 
}   /* commitDraw() */

// ---------------------------------------------------------------------------------- //

/// \brief Method which computes the amount of thermal energy produced 
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///

double FuelCell :: getQ_fc(
    int timestep,
    double dt_hrs,
    double fc_consumption_kg,
    double fc_output_kW
)
{
    double Q_fc = 0;

    if (fc_consumption_kg > 0){
        
        double n_fc = fc_output_kW / (fc_consumption_kg * 33.3 / dt_hrs);   
        Q_fc = fc_consumption_kg * 33.3 / dt_hrs * (1 - n_fc);
    }

    this->Q_fc_vec_kW[timestep] = Q_fc;

    return Q_fc_vec_kW[timestep];
}   /* getQ_fc() */ 

// ---------------------------------------------------------------------------------- //

/// \fn double FuelCell :: __getGenericCapitalCost(double)
///
/// \brief Helper method to generate fuel cell
///      capital cost.
///
///
/// \return A generic fuel cell capital cost.
/// 
///


double FuelCell :: __getGenericCapitalCost(double fc_capacity_kW)
{
    double fc_capital_cost = fc_capital_cost_per_kW*fc_capacity_kW;
    
    return fc_capital_cost;

}   // __getGenericCapitalCost() 

// ---------------------------------------------------------------------------------- //

/// \fn double FuelCell :: __getGenericCapitalCost(double)
///
/// \brief Helper method to generate fuel cell
///      capital cost.
///
///
/// \return A generic fuel cell capital cost.
/// 
///

double FuelCell :: __getGenericOpMaintCost(void)
{
    return fc_operation_maintenance_cost_kWh;

}   // __getGenericCapitalCost() 


// ---------------------------------------------------------------------------------- //

/// \fn double FuelCell :: fc_degradation(int)
///
/// \brief Helper method to estimate fc degradation 
///
///
/// \return A generic fuel cell capital cost.
/// 
///

double FuelCell :: fc_degradation(int timestep, double dt_hrs, double runtime_hrs)
{

    double dD_dt = 0;

    if(this->fc_output_vec_kW[timestep] > 0){
        
        // Check for startup
        if(this->fc_output_vec_kW[timestep-1] == 0){
        this->N_start_stop += 1;
        }

        // Get average operating capacity
        this->sum_capacity_ratio += this->fc_operation_capacity_vec[timestep];
        this->avg_operating_capacity_ratio_vec[timestep] = (this->sum_capacity_ratio / (runtime_hrs + (1 * dt_hrs)));

        this->N_start_stop_vec[timestep] = this->N_start_stop;

        dD_dt = (runtime_hrs + (1 * dt_hrs)) * this->k1 + this->N_start_stop_vec[timestep] * this->k2 + (1 - this->avg_operating_capacity_ratio_vec[timestep]) * this->k3;

        this->fc_SOH = 1 - dD_dt;
    }

    return fc_SOH;

}   // __getGenericCapitalCost() 

