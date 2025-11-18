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
/// \file Electrolyzer.cpp
///
/// \brief Implementation file for the Electrolyzer class.
///
/// A class that models an electrolyzer within the regenerative green hydrogen system. 
///

#include "../../header/Storage/Electrolyzer.h" 

Electrolyzer :: Electrolyzer(

    ElectrolyzerInputs electrolyzer_inputs
) {
    //  1. check inputs
    this->n_points = electrolyzer_inputs.n_points;
    this->el_capital_cost_per_kW = electrolyzer_inputs.el_capital_cost_per_kW;
    this->el_operation_maintenance_cost_kWh = electrolyzer_inputs.el_operation_maintenance_cost_kWh;
    this->B_capacity_factor = electrolyzer_inputs.B_capacity_factor;

    //  2. set attributes
    this->el_output_vec_kg.resize(this->n_points, 0);
    this->Q_el_vec_kW.resize(this->n_points, 0);
    this->el_draw_vec_kW.resize(this->n_points, 0);  
    this->N_start_stop_vec.resize(this->n_points, 0);  
    this->operating_capacity_ratio_vec.resize(this->n_points, 0);  
    this->avg_operating_capacity_ratio_vec.resize(this->n_points, 0); 
    this->n_cap_vec.resize(this->n_points, 0); 

    this->sum_capacity_ratio = 0;      
    this->el_SOH = 1;
    this->N_start_stop = 1;
    this->k1 = electrolyzer_inputs.k1;
    this->k2 = electrolyzer_inputs.k2; 
    this->k3 = electrolyzer_inputs.k3; 
    this->k4 = electrolyzer_inputs.k4;
    
    //  3. construction print
    
    return;
}   /* Electrolyzer() */


// ---------------------------------------------------------------------------------- //

///
/// \fn Electrolyzer :: ~Electrolyzer(void)
///
/// \brief Destructor for the Electrolyzer class.
///

Electrolyzer :: ~Electrolyzer(void)
{
    //  1. destruction print
    if (0) {
        std::cout << "H2 object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Electrolyzer() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //
///
/// \fn Electrolyzer :: Electrolyzer(void)
///
/// \brief Constructor (dummy) for the Electrolyzer class.
///

Electrolyzer :: Electrolyzer(void)
{

    return;
}   /* Electrolyzer() */
///
/// \fn void Electrolyzer :: commitCharge(
///         int timestep,
///         double dt_hrs,
///     )
///
/// \brief Method which computes the amount of hydrogen produced by the electrolyzer
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///

void Electrolyzer :: commitCharge(
    int timestep,
    double dt_hrs,
    double el_spec_consumption_kWh, 
    double charging_kW,
    double el_capacity_kW
)
{
    //  1. record electrolyzer output (kg)
    double n_100_capacity = 33.3 / el_spec_consumption_kWh;
    double n_capacity_factor = n_100_capacity*(1+(this->B_capacity_factor*(1-(charging_kW/el_capacity_kW))));
    el_output_kg = (charging_kW / el_spec_consumption_kWh) * (n_capacity_factor/n_100_capacity) * dt_hrs;

    n_cap_vec[timestep] = n_capacity_factor;
    this->el_output_vec_kg[timestep] = el_output_kg;

}   /* commitCharge() */ 

/// \fn void Electrolyzer :: commitDraw(
///         int timestep,
///         double dt_hrs,
///     )
//
/// \brief Method which computes the amount of energy drawn by the electrolyzer
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.

void Electrolyzer :: commitDraw(
    int timestep,
    double dt_hrs,
    double charging_kW
)   
{
    //  2. record electrolyzer power consumption
    this->el_draw_vec_kW[timestep] = charging_kW;  

}   /* commitDraw() */ 
// ---------------------------------------------------------------------------------- //

/// \brief Method which computes the amount of thermal energy produced by the electrolyzer
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///

double Electrolyzer :: getQ_el(
    int timestep,
    double dt_hrs,
    double el_consumption_kW,
    double el_production_kg
)
{
    double Q_el = 0;

    //  1. record electrolyzer output (kg)
    if (el_consumption_kW > 0){
        
        double n_el = (el_production_kg * 33.3 / dt_hrs) / el_consumption_kW;

        Q_el = el_consumption_kW * (1 - n_el);

    }

    this->Q_el_vec_kW[timestep] = Q_el;

    return Q_el_vec_kW[timestep];
}   /* getQ_el() */ 

// ---------------------------------------------------------------------------------- //

/// \fn double Electrolyzer :: __getGenericCapitalCost(double)
///
/// \brief Helper method to generate electrolyzer
///      capital cost.
///
///
/// \return A generic electrolyzer capital cost.
/// 
///


double Electrolyzer :: __getGenericCapitalCost(double el_capacity_kW)
{
    double el_capital_cost = el_capital_cost_per_kW*el_capacity_kW;
    
    return el_capital_cost;

}   // __getGenericCapitalCost() 

// ---------------------------------------------------------------------------------- //

/// \fn double Electrolyzer :: __getGenericCapitalCost(double)
///
/// \brief Helper method to generate electrolyzer
///      capital cost.
///
///
/// \return A generic electrolyzer capital cost.
/// 
///

double Electrolyzer :: __getGenericOpMaintCost(void)
{
    return el_operation_maintenance_cost_kWh;

}   // __getGenericCapitalCost() 


// ---------------------------------------------------------------------------------- //

/// \fn double FuelCell :: fc_degradation(int)
///
/// \brief Helper method to estimate electrolyzer degradation 
///
///
/// \return A generic electrolyzer capital cost. 
/// 
/// 

double Electrolyzer :: el_degradation(int timestep, double dt_hrs, double runtime_hrs, double el_capacity_kW)
{

    double dD_dt = 0;

    if(this->el_draw_vec_kW[timestep] > 0){
        
        // Check for startup
        if(this->el_draw_vec_kW[timestep-1] == 0){
        this->N_start_stop += 1;
        }

        // Get average operating capacity
        double operating_capacity_ratio = this->el_draw_vec_kW[timestep] / el_capacity_kW; 
        this->sum_capacity_ratio += operating_capacity_ratio;
        this->avg_operating_capacity_ratio_vec[timestep] = (this->sum_capacity_ratio / (runtime_hrs + 1));

        this->N_start_stop_vec[timestep] = this->N_start_stop;

        dD_dt = (runtime_hrs + 1)*this->k1 + this->N_start_stop_vec[timestep]*this->k2 + (1 - this->avg_operating_capacity_ratio_vec[timestep])*this->k3;

        this->el_SOH = 1 - dD_dt;
    }

    return this->el_SOH;

}   // __getGenericCapitalCost() 
