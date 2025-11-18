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
/// \file FuelCell.h
///
/// \brief Header file for the FuelCell class.
///


#ifndef FuelCell_H
#define FuelCell_H

#include <iostream>
#include <vector>

///
/// \struct FuelCellInputs
///
/// \brief A structure which bundles the necessary inputs for the FuelCell constructor.
///     Provides default values for every necessary input. 
///
///

struct FuelCellInputs {


    double n_points = 8760*25;  // 25 years, project lifetime
    double fc_capital_cost_per_kW = 2000; // default capital cost per kW
    double fc_operation_maintenance_cost_kWh = 0.06; // default O&M cost per kWh
    double B_capacity_factor = 0.1; // capacity factor lower limit
    double k1 = 0.00001; // degradation constant 1, runtime hours multiplier
    double k2 = 0.0000207; // degradation constant 2, stop-start cycles multiplier
    double k3 = 0.00001; // degradation constant 3, average load ratio multiplier
    double k4 = 0; // degradation constant 4, operating temperature

};

///
/// \class FuelCell
///
/// \brief A class that models a fuel cell within the regenerative green hydrogen system. 
///

class FuelCell { // Should be set up such that FuelCell is a derived class of H2
    private:
        //  1. attributes 
        
        //  2. methods - functions
        
    public:
        // 1. attributes
        double fc_consumption_kg; // draw of the fuel cell in kg of hydrogen 
        double fc_output_kW; // output of the fuel cell in kW
        double n_points; // number of timeseries data points
        double fc_capital_cost_per_kW; // capital cost per kW
        double fc_operation_maintenance_cost_kWh; // O&M cost per kW
        double B_capacity_factor; // capacity factor lower limit
        double fc_SOH; // fuel cell state of health
        double N_start_stop; // number of stop-start instances
        double sum_capacity_ratio; // sum for computation of average capacity ratio
        double k1; // degradation constant 1, runtime hours multiplier
        double k2; // degradation constant 2, stop-start cycles multiplier
        double k3; // degradation constant 3, average load ratio multiplier
        double k4; // degradation constant 4, operating temperature

        std::vector<double> fc_output_vec_kW; // a vector of the output of the fuel cell in kW
        std::vector<double> fc_draw_vec_kW; // a vector of the fuel cell power consumption in kW
        std::vector<double> fc_consumption_vec_kg; // a vector for the draw of the fuel cell in kg of hydrogen 
        std::vector<double> fc_operation_capacity_vec; // a vector of the operating capacity ratio
        std::vector<double> Q_fc_vec_kW; // a vector of the draw of the fuel cell in kW

        std::vector<double> N_start_stop_vec; // a vector of the number of stop-start instances
        std::vector<double> operating_capacity_ratio_vec; // a vector of the operating capacity ratio
        std::vector<double> avg_operating_capacity_ratio_vec; // a vector of the average operating capacity ratio
        std::vector<double> n_cap_vec; // a vector of the new operating capacity ratio
    
        //  2. methods, more functions 
        double __getGenericCapitalCost(double); // computes capital cost
        double __getGenericOpMaintCost(void); // computes operation and maintenence cost
        
        void commitDischarge(int, double, double, double); // initiate electrolyzer/fuelcell process and store/deplete h2
        void commitDraw(int, double, double, double, double, double); // computes energy/hydrogen drawn 

        double fc_degradation(int,double,double); // computes component capacity degradation 
        double getQ_fc(int, double, double, double); // computer net thermal energy of component
        
        FuelCell(void);
        FuelCell(FuelCellInputs); 
        ~FuelCell(void); 
        
};  /* FuelCell */

#endif  /* FuelCell_H */
