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
/// \file Electrolyzer.h
///
/// \brief Header file for the Electrolyzer class.
///


#ifndef Electrolyzer_H
#define Electrolyzer_H

#include <iostream>
#include <vector>

///
/// \struct ElectrolyzerInputs
///
/// \brief A structure which bundles the necessary inputs for the Electrolyzer constructor.
///     Provides default values for every necessary input. 
///
///

struct ElectrolyzerInputs {

    double n_points = 8760*25;  // 25 years, project lifetime
    double el_capital_cost_per_kW = 1600; // default capital cost per kW
    double el_operation_maintenance_cost_kWh = 0.06; // default O&M cost per kWh
    double B_capacity_factor = 0.1; // capacity factor lower limit
    double k1 = 0.00001; // degradation constant 1, runtime hours multiplier
    double k2 = 0.0000207; // degradation constant 2, stop-start cycles multiplier
    double k3 = 0.00001; // degradation constant 3, average load ratio multiplier
    double k4 = 0; // degradation constant 4, operating temperature

};

///
/// \class Electrolyzer
///
/// \brief A class that models an electrolyzer within the regenerative green hydrogen system. 
///

class Electrolyzer {
    private:
        //  1. attributes 
        
        //  2. methods - functions
       
        
    public:
        // 1. attributes 
        
        double el_capital_cost_per_kW; // capital cost per kW
        double el_output_kg; // output of the electrolyzer in kg of hydrogen
        double el_draw_kW; // draw of the electrolyzer in kW 
        double charging_efficiency; // charging efficiency of electrolyzer 
        double n_points; // number of timeseries data points
        double el_operation_maintenance_cost_kWh; // O&M cost per kW
        double B_capacity_factor; // capacity factor lower limit
        double el_SOH; // electrolyzer state of health
        double N_start_stop; // number of stop-start instances
        double sum_capacity_ratio; // sum for computation of average capacity ratio
        double k1; // degradation constant 1, runtime hours multiplier
        double k2; // degradation constant 2, stop-start cycles multiplier
        double k3; // degradation constant 3, average load ratio multiplier
        double k4; // degradation constant 4, operating temperature

        std::vector<double> el_output_vec_kg; // a vector of the electrolyzer in kg of hydrogen
        std::vector<double> Q_el_vec_kW; // a vector of the electrolyzer thermal energy generation in kW
        std::vector<double> el_draw_vec_kW; // a vector of the draw of the electrolyzer in kW
        std::vector<double> N_start_stop_vec; // a vector of the number of stop-start instances
        std::vector<double> operating_capacity_ratio_vec; // a vector of the operating capacity ratio
        std::vector<double> avg_operating_capacity_ratio_vec; // a vector of the average operating capacity ratio
        std::vector<double> n_cap_vec; // a vector of the new operating capacity ratio

        //  2. methods, more functions 
        double __getGenericCapitalCost(double); // computes capital cost
        double __getGenericOpMaintCost(void); // computes operation and maintenence cost
        
        void commitCharge(int, double, double, double, double); // initiate electrolyzer/fuelcell process and store/deplete h2 
        void commitDraw(int, double, double); // computes energy/hydrogen drawn 
        
        double el_degradation(int, double, double, double); // computes component capacity degradation  
        double getQ_el(int, double, double, double); // computer net thermal energy of component

        Electrolyzer(void);
        Electrolyzer(ElectrolyzerInputs);
        ~Electrolyzer(void);
        
};  /* Electrolyzer */

#endif  /* Electrolyzer_H */
