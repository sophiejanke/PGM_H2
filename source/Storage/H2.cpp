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
/// \file H2.cpp
///
/// \brief Implementation file for the H2 class.
///
/// A derived class of Storage which models energy storage by way of a regenerative green hydrogen system.
///


#include "../../header/Storage/H2.h" 


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: __checkInputs(H2Inputs H2inputs)
///
/// \brief Helper method to check inputs to the H2 constructor.
///
/// \param H2inputs A structure of H2 constructor inputs.
///

void H2 :: __checkInputs(H2Inputs h2_inputs) 
{

    //  Check inputs & status of H2 system 
    
    //  1. check init_SOC
    
    if (h2_inputs.init_SOC < 0 or h2_inputs.init_SOC > 1) {
        std::string error_str = "ERROR:  H2():  init_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check min_SOC
    if (h2_inputs.min_SOC < 0 or h2_inputs.min_SOC > 1) {
        std::string error_str = "ERROR:  H2():  min_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check hysteresis_SOC
    if (h2_inputs.hysteresis_SOC < 0 or h2_inputs.hysteresis_SOC > 1) {
        std::string error_str = "ERROR:  H2():  hysteresis_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check max_SOC
    if (h2_inputs.max_SOC < 0 or h2_inputs.max_SOC > 1) {
        std::string error_str = "ERROR:  H2():  max_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }

    
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic hydrogen energy storage system
///      capital cost.
///
/// This model was obtained by way of surveying an assortment of published hydrogen 
/// energy storage system costs, and then constructing a best fit model. Note
/// that this model expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the hydrogen energy storage system
///     [CAD].
///


double H2 :: __getGenericCapitalCost(void)
{
    double el_capital_cost = electrolyzer.__getGenericCapitalCost(this->el_capacity_kW);
    double fc_capital_cost = fuelcell.__getGenericCapitalCost(fc_capacity_kW);

    double capital_cost_h2_system = (this->h2_tank_capacity_kg * this->h2_tank_cost_kg) + el_capital_cost + fc_capital_cost;

    double compressor_cost = (this->el_capacity_kW * compressor_cap_cost_kW);
    double water_treatment_cost = (this->el_capacity_kW * this->water_treatment_cap_cost);

    if(not this->compression_included){
        compressor_cost = 0;
    }

    if(not this->water_treatment_included){
        water_treatment_cost = 0;
    }

    capital_cost_h2_system = capital_cost_h2_system + compressor_cost + water_treatment_cost;
    
    return capital_cost_h2_system;

}   // __getGenericCapitalCost() 

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: __getGenericOpMaintCost(void) 
///
/// \brief Helper method to generate a generic hydrogen energy storage system
///     operation and maintenance cost. This is a cost incurred per unit energy
///     charged/discharged.
///
/// \return A generic operation and maintenance cost, per unit energy
///     charged/discharged, for the hydrogen energy storage system [CAD/kWh].
///

double H2 :: __getGenericOpMaintCost(void)
{
    // can remove these methods if we are just considering one O&M cost for charging/discharging
    double el_operation_maintenance_cost_kWh = electrolyzer.__getGenericOpMaintCost();
    double fc_operation_maintenance_cost_kWh = fuelcell.__getGenericOpMaintCost();

    double h2_OpMainCost = this->h2system_operation_maintenance_cost_kWh;

    return h2_OpMainCost;
}   // __getGenericOpMaintCost() 

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: __toggleDepleted(void)
///
/// \brief Helper method to toggle the is_depleted attribute of H2.
///

void H2 :: __toggleDepleted(void) 
{

    double min_charge_kg = this->h2_tank_capacity_kg*this->min_SOC;
        
    if (this->tank_level_kg <= min_charge_kg) {
        this->is_depleted = true;
    }
    else{
        this->is_depleted = false;
    }
    
    return;
}   // __toggleDepleted() 

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: __handleDegradation(
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
///


void H2 :: __handleDegradation(
    int timestep,
    double dt_hrs
)
{
    //  1. model degradation
    this->__modelDegradation(timestep,dt_hrs);
    
    //  2. update and record
    this->SOH_el_vec[timestep] = this->SOH_el;
    this->SOH_fc_vec[timestep] = this->SOH_fc;

    if (this->SOH_el <= this->replace_SOH_el) {
        this->handleReplacement_el(timestep);
    }

    if (this->SOH_fc <= this->replace_SOH_fc) {
        this->handleReplacement_fc(timestep);
    }
    
    return;
}   /* __handleDegradation() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void H2Ion :: __modelDegradation(double dt_hrs, double charging_discharging_kW)
///
/// \brief Helper method to model energy capacity degradation as a function of operating
///     state.
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///

void H2 :: __modelDegradation(
    int timestep,
    double dt_hrs
)
{
    // get approximate soh loss for electrochemical components
    this->SOH_fc = this->fuelcell.fc_degradation(timestep,dt_hrs,this->fc_runtime_hrs);
    this->SOH_el = this->electrolyzer.el_degradation(timestep,dt_hrs,this->el_runtime_hrs,this->el_capacity_kW);

    this->fc_spec_consumption_kg = fc_spec_consumption_kg_initial + (fc_spec_consumption_kg_initial*(1-this->SOH_fc));
    this->el_spec_consumption_kWh = el_spec_consumption_kWh_initial + (el_spec_consumption_kWh_initial*(1-this->SOH_el));

    return;
}   /* __modelDegradation() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for H2.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void H2 :: __writeSummary(std::string write_path)
{
        // temp outputs for fort chip model results
    // std::cout << "Curtiailed Hydrogen: " << this->total_curtailed_hydrogen_potential << std::endl;
    // std::cout << "Water Demand: " << this->total_water_demand_kg << std::endl;
    
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->el_capacity_kW))); // Electrolyzer rated power [kW]
    ofs << " kW ";
    ofs << std::to_string(int(ceil(this->fc_capacity_kW))); // Fuel Cell rated power [kW]
    ofs << " kW ";
    ofs << std::to_string(int(ceil(this->h2_tank_capacity_kg))); // H2 Tank capacity [kg]
    ofs << " kg H2 Summary Results\n";
    ofs << "\n--------\n\n"; 
    
    //  2.1. Hydrogen Storage attributes
    ofs << "## Storage Attributes\n";
    ofs << "\n";
    ofs << "Electrolyzer Capacity: " << this->el_capacity_kW << " kW  \n"; // Electrolyzer rated power [kW]
    ofs << "Hydrogen Tank Capacity: " << this->h2_tank_capacity_kg << " kg  \n"; // H2 Tank capacity [kg]
    ofs << "Fuel Cell Capacity: " << this->fc_capacity_kW<< " kW  \n"; // Fuel Cell rated power [kW]
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
    
    //  2.2. H2 attributes
    ofs << "## H2 Attributes\n";
    ofs << "\n";
    
    ofs << "Charging Efficiency: " << this->n_electrolyzer << "  \n";
    ofs << "Discharging Efficiency: " << this->n_fuelcell << "  \n";
    ofs << "\n";
    
    ofs << "Initial State of Charge: " << this->init_SOC << "  \n";
    ofs << "Minimum State of Charge: " << this->min_SOC << "  \n";
    ofs << "Hyteresis State of Charge: " << this->hysteresis_SOC << "  \n";
    ofs << "Maximum State of Charge: " << this->max_SOC << "  \n";
    ofs << "\n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. H2 Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    double el_capital_cost = electrolyzer.__getGenericCapitalCost(this->el_capacity_kW);
    double fc_capital_cost = fuelcell.__getGenericCapitalCost(this->fc_capacity_kW);

    double compressor_cost = (el_capacity_kW * compressor_cap_cost_kW);
    double water_treatment_cost = (el_capacity_kW * 200);

    if(not this->compression_included){
        compressor_cost = 0;
    }

    if(not this->water_treatment_included){
        water_treatment_cost = 0;
    }

    ofs << "Water Treatement System Capital Cost: " << water_treatment_cost << "  \n";
    ofs << "Electrolyzer Capital Cost: " << el_capital_cost << "  \n";
    ofs << "Compression Capital Cost: " << compressor_cost << "  \n";
    ofs << "Storage Vessel Capital Cost: " << (this->h2_tank_capacity_kg * this->h2_tank_cost_kg) << "  \n";
    ofs << "Fuel Cell Capital Cost: " << fc_capital_cost << "  \n";
    ofs << "\n";

    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched  \n";
    ofs << "\n";

    ofs << "Total Hydrogen Produced [kg]: " << this->total_h2_produced_kg << "  \n";
    ofs << "Total Water Demand [kg]: " << this->total_water_demand_kg << "  \n";
    ofs << "Total Electrolyzer Consumption [kWh]: " << this->total_el_consumption_kWh << "  \n";
    ofs << "Total Fuel Cell Energy Generation  [kWh]: " << this->total_discharge_kWh << "  \n";
    ofs << "Total Electrolyzer runtime [hrs]: " << this->el_runtime_hrs_total << "  \n";
    ofs << "Total Fuel Cell runtime [hrs]: " << this->fc_runtime_hrs_total << "  \n";
    ofs << "Number of times Electrolyzer runtime limit was enforced: " << this->el_runtime_enforced_total << "  \n";
    ofs << "Number of times Fuel Cell runtime limit was enforced: " << this->fc_runtime_enforced_total << "  \n";
    ofs << "Total Potential Curtailed Hydrogen [kg]: " << this->total_curtailed_hydrogen_potential << "  \n";
    ofs << "Total External Hydrogen Load met [kg]: " << this->total_external_hydrogen_load_met << "  \n";
    ofs << "Electrolyzer Replacements: " << this->el_replacements << "  \n";
    ofs << "Fuel Cell Replacements: " << this->fc_replacements << "  \n";
    ofs << "Initial Round Trip Efficiency: " << this->n_h2_overall << "  \n";
    double final_n_h2_overall = this->n_h2_overall * this->SOH_fc * this->SOH_el;
    ofs << "Final Round Trip Efficiency: " << final_n_h2_overall << "  \n";

    ofs << "\n--------\n\n";
    ofs.close();
    return;
}   // __writeSummary() 

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for H2.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the
///     ElectricalLoad.
///
/// \param max_lines The maximum number of lines of output to write.
///

void H2 :: __writeTimeSeries(
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
    ofs << "Fuel Cell Production [kW],";
    ofs << "Electrolyzer Consumption [kW],";
    ofs << "Stored Hydrogen [kg],";
    ofs << "External Hydrogen Load [kg],";
    ofs << "Curtiailed Hydrogen Potential [kg],";
    ofs << "Compression Power [kW],";
    ofs << "Compression Thermal Output [kW],";
    ofs << "Water Demand [kg],";   
    ofs << "Electrolyzer H2 Production [kg],";
    ofs << "Charge (at end of timestep) [kWh],";
    ofs << "Fuel Cell Consumption [kg],";
    ofs << "Electrolyzer State of Health (at end of timestep) [ ],";
    ofs << "Fuel Cell State of Health (at end of timestep) [ ],";
    ofs << "Electrolyzer Operational Efficiency [ ],";
    ofs << "Fuel Cell State Operational Efficiency [ ],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        ofs << this->fuelcell.fc_output_vec_kW[i] << ","; 
        ofs << this->charging_power_vec_kW[i] << ","; 
        ofs << this->tank_level_vec_kg[i] << ","; 
        ofs << this->hydrogen_load_vec_kg[i] << ","; 
        ofs << this->curtailed_hydrogen_vec_kg[i] << ",";
        ofs << this->compression_power_vec_kW[i] << ","; 
        ofs << this->compression_Q_vec_kW[i] << ","; 
        ofs << this->water_demand_vec_kg[i] << ","; 
        ofs << this->electrolyzer.el_output_vec_kg[i] << ","; 
        ofs << this->charge_vec_kWh[i] << ",";
        ofs << this->fuelcell.fc_consumption_vec_kg[i] << ","; 
        ofs << this->SOH_el_vec[i] << ","; 
        ofs << this->SOH_fc_vec[i] << ","; 
        ofs << this->n_electrolyzer_vec[i] << ","; 
        ofs << this->n_fuelcell_vec[i] << ","; 
        ofs << this->capital_cost_vec[i] << ",";
        ofs << this->operation_maintenance_cost_vec[i] << ",";
        ofs << "\n";
    }
    
    ofs.close();
    return;
}   /* __writeTimeSeries() 

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn H2 :: H2(void)
///
/// \brief Constructor (dummy) for the H2 class.
///
*/

H2 :: H2(void)
{
    this->fuelcell = FuelCell();
    this->electrolyzer = Electrolyzer();

    return;
}   /* H2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn H2 :: H2(
///         int n_points,
///         double n_years,
///         H2Inputs H2inputs
///     )
///
/// \brief Constructor (intended) for the H2 class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param H2inputs A structure of H2 constructor inputs.
///

H2 :: H2(
    int n_points,
    double n_years,
    H2Inputs h2_inputs

) :
Storage(
    n_points,
    n_years,
    h2_inputs.storage_inputs
)
{
    //  1. check inputs
    this->__checkInputs(h2_inputs);

    //  2. set attributes 

    this->type = StorageType :: H2_SYS; 
    this->type_str = "H2_SYS"; 

    this->electrolyzer = Electrolyzer(h2_inputs.electrolyzer_inputs);
    this->fuelcell = FuelCell(h2_inputs.fuelcell_inputs);
    h2_inputs.electrolyzer_inputs.n_points = n_points;
    h2_inputs.fuelcell_inputs.n_points = n_points;

    this->external_hydrogen_load_included = h2_inputs.external_hydrogen_load_included;

    if (external_hydrogen_load_included){
        this->path_2_external_hydrogen_load_time_series = h2_inputs.path_2_external_hydrogen_load_time_series;
        this->getExternalHydrogenLoadkg(path_2_external_hydrogen_load_time_series);
    }

    this->tank_level_vec_kg.resize(this->n_points, 0);
    this->compression_power_vec_kW.resize(this->n_points, 0);
    this->water_demand_vec_kg.resize(this->n_points, 0);
    this->hydrogen_load_vec_kg.resize(this->n_points);
    this->curtailed_hydrogen_vec_kg.resize(this->n_points, 0);
    this->SOH_el_vec.resize(this->n_points, 0);
    this->SOH_fc_vec.resize(this->n_points, 0);
    this->n_fuelcell_vec.resize(this->n_points, 0);
    this->n_electrolyzer_vec.resize(this->n_points, 0);
    this->compression_Q_vec_kW.resize(this->n_points, 0);

    this->capital_cost = h2_inputs.capital_cost;
    this->operation_maintenance_cost_kWh = h2_inputs.operation_maintenance_cost_kWh;
    this->h2system_operation_maintenance_cost_kWh = h2_inputs.h2system_operation_maintenance_cost_kWh;

    this->init_SOC = h2_inputs.init_SOC;
    this->min_SOC = h2_inputs.min_SOC;
    this->hysteresis_SOC = h2_inputs.hysteresis_SOC;
    this->max_SOC = h2_inputs.max_SOC;
    this->kWh_kg_conversion = h2_inputs.kWh_kg_conversion;
    this->fc_min_load_ratio = h2_inputs.fc_min_load_ratio;
    this->el_min_load_ratio = h2_inputs.el_min_load_ratio;

    this->fc_min_runtime = h2_inputs.fc_min_runtime;
    this->el_min_runtime = h2_inputs.el_min_runtime;
    this->fc_ramp_loss = h2_inputs.fc_ramp_loss;
    this->el_ramp_loss = h2_inputs.el_ramp_loss;
    this->el_is_running = false;
    this->fc_is_running = false;
    this->making_hydrogen_for_external_load = false;
    this->total_external_hydrogen_load_met = 0;

    this->el_capacity_kW = h2_inputs.el_capacity_kW;
    this->el_quantity = h2_inputs.el_quantity;
    this->el_spec_consumption_kWh = h2_inputs.el_spec_consumption_kWh;
    el_spec_consumption_kWh_initial = this->el_spec_consumption_kWh;
    this->fc_capacity_kW = h2_inputs.fc_capacity_kW;
    this->fc_quantity = h2_inputs.fc_quantity;
    this->fc_spec_consumption_kg = h2_inputs.fc_spec_consumption_kg;
    fc_spec_consumption_kg_initial = this->fc_spec_consumption_kg;
    this->h2_tank_capacity_kg = h2_inputs.h2_tank_capacity_kg;
    this->h2_tank_cost_kg = h2_inputs.h2_tank_cost_kg;
    this->compressor_spec_consumption_kWh = h2_inputs.compressor_spec_consumption_kWh;
    this->n_compressor = h2_inputs.n_compressor;
    this->compression_included = h2_inputs.compression_included;
    this->compressor_cap_cost_kW = h2_inputs.compressor_cap_cost_kW;
    this->water_treatment_included = h2_inputs.water_treatment_included;
    this->excess_hydrogen_potential_included = h2_inputs.excess_hydrogen_potential_included; 
    this->water_treatment_cap_cost = h2_inputs.water_treatment_cap_cost;
    this->water_demand_L = h2_inputs.water_demand_L;

    this->p_air = h2_inputs.p_air;
    this->cp_air = h2_inputs.cp_air;
    this->cp_el = h2_inputs.cp_el;
    this->cp_fc = h2_inputs.cp_fc;
    this->p_el = h2_inputs.p_el;
    this->p_fc = h2_inputs.p_fc;
    this->v_housing = h2_inputs.v_housing;

    this->n_fuelcell = (1/(this->kWh_kg_conversion*this->fc_spec_consumption_kg)); 
    this->n_electrolyzer = (this->kWh_kg_conversion/this->el_spec_consumption_kWh); 
    this->n_h2_overall = this->n_fuelcell * this->n_electrolyzer;

    this->energy_capacity_kWh = kWh_kg_conversion * this->h2_tank_capacity_kg * this->n_fuelcell; 
    this->power_capacity_kW = this->fc_capacity_kW * this->fc_quantity; 

    this->dynamic_h2_energy_capacity_kWh = this->energy_capacity_kWh;
    this->dynamic_h2_power_capacity_kW = this->power_capacity_kW;

    this->SOH_el = 1;
    this->SOH_fc = 1;
    this->is_depleted = false;
    this->power_degradation_flag = h2_inputs.power_degradation_flag;
    this->replace_SOH_el = h2_inputs.replace_SOH_el;
    this->replace_SOH_fc = h2_inputs.replace_SOH_fc;
    this->el_replacements = 0;
    this->fc_replacements = 0;
    this->el_runtime_hrs = 0;
    this->el_runtime_hrs_total = 0;
    this->el_runtime_current_hrs = 0;
    this->fc_runtime_hrs = 0;
    this->fc_runtime_hrs_total = 0;
    this->fc_runtime_current_hrs = 0;
    this->el_runtime_enforced_total = 0;
    this->fc_runtime_enforced_total = 0;
    this->total_curtailed_hydrogen_potential = 0;

    this->charge_kWh = this->init_SOC * this->energy_capacity_kWh;
    this->tank_level_kg = this->init_SOC * this->h2_tank_capacity_kg;
    
    if (h2_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = h2_inputs.capital_cost;
    }
    
    if (h2_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            h2_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  4. construction print
    if (this->print_flag) {
        std::cout << "H2 object constructed at " << this << std::endl;
    }
    
    return;
}   /* H2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: handleReplacement_el(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable, of the electrolyzer.
///
/// \param timestep The current time step of the Model run.
///

void H2 :: handleReplacement_el(int timestep)
{
    //  1. reset attributes
    this->SOH_el = 1;
    this->el_replacements = this->el_replacements + 1;
    this->el_runtime_hrs = 0;
    this->el_is_running = false;
    
    // 2. correct attributes
    this->charge_kWh = this->init_SOC * this->dynamic_h2_energy_capacity_kWh;
    this->is_depleted = false;
    
    return;
}   /* __handleReplacement_el() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: handleReplacement_fc(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable, of the fuel cell.
///
/// \param timestep The current time step of the Model run.
///

void H2 :: handleReplacement_fc(int timestep)
{
    //  1. reset attributes
    this->SOH_fc = 1;
    this->fc_replacements = this->fc_replacements + 1;
    this->fc_runtime_hrs = 0;
    this->fc_is_running = false;
    
    // 2. correct attributes
    this->charge_kWh = this->init_SOC * this->dynamic_h2_energy_capacity_kWh;
    this->is_depleted = false;
    
    return;
}   /* __handleReplacement_fc() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: EL_minruntime(
///         int timestep
///     )
///
/// \brief Helper method (private) to handle the starting/stopping of the H2 asset
///
/// \param timestep The current time step of the Model run.
///
///

bool H2 :: EL_minruntime(int timestep)
{
    // check if electrolyser is currently running (in previous timestep)
    el_min_runtime_enforced = false;

    if(timestep > 1){
        if (this->charging_power_vec_kW[timestep-1] > 0){
            // if yes, update el_is_running indicator 
            this->el_is_running = true;
        } else {
            // if not, set el_is_running to false and reset current running hours
            this->el_runtime_current_hrs = 0;
            this->el_is_running = false;
        }
        // if it was running in the previous timestep, check if current runtime is less than the minimum
        if (this->el_is_running){
            if (this->el_runtime_current_hrs < this->el_min_runtime) {
                // if not, enforce minimum runtime contraint
                el_min_runtime_enforced = true;
                this->el_runtime_enforced_total += 1;
            } else {
                el_min_runtime_enforced = false;
            }
        }
    }
    return el_min_runtime_enforced;
}   /* EL_minruntime() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: FC_minruntime(
///         int timeste
///     )
///
/// \brief Helper method (private) to handle the starting/stopping of the H2 asset
///
/// \param timestep The current time step of the Model run.
///

bool H2 :: FC_minruntime(int timestep)
{
    // Helper method (private) to handle the starting/stopping of the hydrogen asset. 
    fc_min_runtime_enforced = false;

    if (timestep > 1){
        if (this->fuelcell.fc_output_vec_kW[timestep-1] > 0){
            // if yes, update fc_is_running indicator 
            this->fc_is_running = true;
        } else {
            // if not, set fc_is_running to false and reset current running hours
            this->fc_runtime_current_hrs = 0;
            this->fc_is_running = false;
        }
        // if it was running in the previous timestep, check if current runtime is less than the minimum
        if (this->fc_is_running){
            if (this->fc_runtime_current_hrs < this->fc_min_runtime) {
                // if not, enforce minimum runtime contraint
                fc_min_runtime_enforced = true;
                this->fc_runtime_enforced_total += 1;
            } else {
                fc_min_runtime_enforced = false;
            }
        }
    }
    return fc_min_runtime_enforced;
}   /* FC_minruntime() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //
///
/// \fn double H2 :: getMinLoadkW(double dt_hrs)
///
/// \brief Method to get the minimum allowable operating power of the electrolyzer
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The minimum allowable operating power of the asset [kW]
///

double H2 :: getMinELCapacitykW(double dt_hrs) // ---------------------------------------------------------------------------------- //
{
    // get extra power neede for compression phase
    double compression_consumption_kW = this->getCompressorPowerkW(dt_hrs);

    double min_acceptable_power = (this->el_capacity_kW*this->el_quantity*this->el_min_load_ratio) + compression_consumption_kW;
    return min_acceptable_power;
}
// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //
///
/// \fn double H2 :: getAcceptablekW( double dt_hrs)
///
/// \brief Method to get the charge power currently acceptable by the asset.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The charging power [kW] currently acceptable by the asset.
///

double H2 :: getAcceptablekW(int timestep, double dt_hrs) 
{   
    //  1. compute acceptable energy and compression power
    double max_charge_kWh = this->max_SOC * this->h2_tank_capacity_kg * this->el_spec_consumption_kWh;
    double compression_consumption_kW = this->getCompressorPowerkW(dt_hrs);
    
    //  2. compute acceptable power
    double acceptable_power_kW = (max_charge_kWh - (this->tank_level_kg * this->el_spec_consumption_kWh)) / dt_hrs;

    //  3. apply power constraint (upper)
    double power_constraint_upper_kW = this->el_capacity_kW * this->el_quantity; 
    if (acceptable_power_kW > power_constraint_upper_kW) {
        acceptable_power_kW = power_constraint_upper_kW;
    }
    acceptable_power_kW = acceptable_power_kW + compression_consumption_kW;
    
    //  4. account for power already being sent to this asset in this timestep
    acceptable_power_kW -= this->power_kW;
    
    return acceptable_power_kW;
}   /* getAcceptablekW( */ 

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

/// \fn double H2 :: getMinLoadkW(double dt_hrs)
///
/// \brief Method to get the minimum allowable operating power of the electrolyzer
///
/// \return The minimum allowable operating power of the asset [kW]
///

double H2 :: getMinFCCapacitykW() // ---------------------------------------------------------------------------------- //
{
    double min_available_power = this->dynamic_h2_power_capacity_kW * this->fc_min_load_ratio; 
    return min_available_power;
}
// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: getAvailablekW(double dt_hrs)
///
/// \brief Method to get the discharge power currently available from the asset.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep. 
///
/// \return The discharging power [kW] currently available from the asset. 
///

double H2 :: getAvailablekW(int timestep, double dt_hrs) // get available energy in tank
{
    
    //  1. get min charge (kWh)
    double min_charge_kWh = this->min_SOC * this->dynamic_h2_energy_capacity_kWh;
    
    //  2. compute total available power 
    double available_kW = (this->charge_kWh - min_charge_kWh) / dt_hrs;

    if (available_kW <= 0) {
        return 0;
    }

    //  3. apply power constraint
    if (available_kW > this->dynamic_h2_power_capacity_kW) {
        available_kW = this->dynamic_h2_power_capacity_kW;
    }    

    // 4. check that tank level is sufficient given current fuel cell specific consumption
    //    comparing tank level, and the actual kW that computes to given performance of fuel cell technology
    double actual_available_kW = this->tank_level_kg / this->fc_spec_consumption_kg / dt_hrs; 

    if(available_kW > actual_available_kW){
        return 0;
    }

    //  4. apply ramping rate loss - fc ramp up quite quickly but can still include it for model flexibility. 
    available_kW -= this->power_kW;

    return available_kW;
}   /* getAvailablekW() */ 

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: commitElectrolysis(
///         int timestep,
///         double dt_hrs,
///         double charge_kWh
///     )
///
/// \brief Method which takes in the charging (aka electrolysis in the case of hydrogen) power for the current timestep and
///     records.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_kW The charging power [kw] being sent to the asset.
/// 

void H2 :: commitElectrolysis( 
    int timestep,
    double dt_hrs,
    double charging_kW
)
{
    //  1. get additional power input needed for compression 
    double compression_consumption_kW = this->getCompressorPowerkW(dt_hrs);

    //  2. record power left available to make hydrogen; charging power - compression requirement
    //  NEEDS WORK: What if the charging power is greater than the charge power and charging_power_vec_kW becomes negative?
    if(charging_kW>0){
        this->compression_power_vec_kW[timestep] = compression_consumption_kW;
        this->charging_power_vec_kW[timestep] = charging_kW - compression_consumption_kW;
    }
    else{
        this->charging_power_vec_kW[timestep] = charging_kW;
    }

    //  3. compute efficiency lost in ramping up of Electrolysis
    //      get previous timestep power capacity 
    double previous_power_capacity_kW = this->charging_power_vec_kW[timestep - 1];
    //      check if electrolyzer is ramping up 
    if(this->charging_power_vec_kW[timestep] > previous_power_capacity_kW){
        // computer % capacity ramp 
        double ramping_ratio = (this->charging_power_vec_kW[timestep] - previous_power_capacity_kW) / (this->el_capacity_kW * this->el_quantity);
        double ramping_loss = this->el_ramp_loss * ramping_ratio; 
        this->charging_power_vec_kW[timestep] = this->charging_power_vec_kW[timestep] - (this->charging_power_vec_kW[timestep] * ramping_loss);
    }

    // 5. initate electrolysis methods and record 
    this->electrolyzer.commitCharge(timestep,dt_hrs,this->el_spec_consumption_kWh,this->charging_power_vec_kW[timestep],this->el_capacity_kW); 
    this->el_output_kg = this->electrolyzer.el_output_vec_kg[timestep];
    this->electrolyzer.commitDraw(timestep,dt_hrs,this->charging_power_vec_kW[timestep]);

    this->electrolyzer.getQ_el(timestep,dt_hrs,this->electrolyzer.el_draw_vec_kW[timestep],this->el_output_kg);
    this->getWaterDemand(timestep,this->el_output_kg);

    // 6. model degradation and compute current operational efficiency
    this->__handleDegradation(timestep,dt_hrs);
    if (charging_kW>0){
        this->n_electrolyzer_vec[timestep] = (this->kWh_kg_conversion/this->el_spec_consumption_kWh); 
    }
    // 7. update totals
    this->total_h2_produced_kg += this->el_output_kg;
    if (charging_kW>0){
        this->total_el_consumption_kWh += this->electrolyzer.el_draw_vec_kW[timestep]*dt_hrs;
    }
    if (charging_kW>0){
        this->el_runtime_hrs += dt_hrs;
        this->el_runtime_hrs_total += dt_hrs;
        this->el_is_running = true;
    }

    if (this->el_is_running) {
        this->el_runtime_current_hrs += dt_hrs;
    }

    // 8. update tank level and record
    this->tank_level_kg += this->el_output_kg;
    if(this->tank_level_kg > this->h2_tank_capacity_kg){
        this->tank_level_kg = this->h2_tank_capacity_kg;
    }

    this->tank_level_vec_kg[timestep] = this->tank_level_kg;

    // 9. update charge and record
    this->charge_kWh = this->tank_level_vec_kg[timestep]* kWh_kg_conversion;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    // 10. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  11. capture operation and maintenance costs (if applicable)
    if (charging_kW > 0) {
        this->operation_maintenance_cost_vec[timestep] = charging_kW * dt_hrs *
            this->operation_maintenance_cost_kWh;
    }

    this->power_kW = 0;
    
    return;
}   /* commitElectrolysis() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //
///
/// \fn double H2 :: commitFuelcell(
///         int timestep,
///         double dt_hrs,
///         double discharging_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in the discharging (aka Fuel Cell operation in the case of hydrogen) power for the current timestep and
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

double H2 :: commitFuelCell(
    int timestep,
    double dt_hrs,
    double discharging_kW,
    double load_kW
)
{
    //  1. record discharging power
    this->discharging_power_vec_kW[timestep] = discharging_kW;
    this->total_discharge_kWh += discharging_kW * dt_hrs;
    
    // check that minimum operating cap is not being exceeded - to be removed later
    double available_power = this->dynamic_h2_power_capacity_kW*this->fc_min_load_ratio;
    if(discharging_kW<available_power && discharging_kW>0){
        std::cout << "FC Lower capacity limit was ignored:  " << discharging_kW << " At timestep: " << timestep << std::endl;
    }

    // 2. initiate fuel cell methods
    this->fuelcell.commitDischarge(timestep,dt_hrs,discharging_kW,this->dynamic_h2_power_capacity_kW); 
    this->fc_output_kW = this->fuelcell.fc_output_vec_kW[timestep];
    this->fuelcell.commitDraw(timestep,dt_hrs,this->fc_spec_consumption_kg, this->fc_quantity,this->fc_output_kW,this->fc_capacity_kW); 
    this->fc_consumption_kg = this->fuelcell.fc_consumption_vec_kg[timestep]; 

    this->fuelcell.getQ_fc(timestep,dt_hrs,this->fc_consumption_kg,this->fc_output_kW);

    // 3. model degradation and compute current operational efficiency
    this->__handleDegradation(timestep,dt_hrs);
    if(discharging_kW>0){
        this->n_fuelcell_vec[timestep] = (1/(this->kWh_kg_conversion*this->fc_spec_consumption_kg)); 
    }
    // 4. update totals
    
    if(discharging_kW>0){
        this->fc_runtime_hrs += dt_hrs;
        this->fc_runtime_hrs_total += dt_hrs;
        this->fc_is_running = true;
    }

    if (this->fc_is_running) {
        this->fc_runtime_current_hrs += dt_hrs;
    }

    // 5. update tank level and record
    this->tank_level_kg -= this->fc_consumption_kg;
    this->tank_level_vec_kg[timestep] = this->tank_level_kg;

    // 6. update charge and record
    this->charge_kWh = this->tank_level_kg * kWh_kg_conversion;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    //  7. update load
    load_kW -= discharging_kW;
    
    //  8. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  9. capture operation and maintenance costs (if applicable)
    if (discharging_kW > 0) {
        this->operation_maintenance_cost_vec[timestep] = discharging_kW * dt_hrs *
            this->operation_maintenance_cost_kWh;
    }
    
    
    this->power_kW = 0;
    return load_kW;
}   /* commitDischarge() */

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: getThermalOutput(int);
///
/// \brief Method to get the net thermal energy of the electrolyzer and fuel cell
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The thermal enery output of the electrolyzer and fuelcell assets [kW]
///

double H2 :: getThermalOutput(
    int timestep,
    double dt_hrs
) 
{
    // get  fuel cell thermal energy output for thermal energy modelling
    double thermal_output_kW  = this->fuelcell.Q_fc_vec_kW[timestep] + this->electrolyzer.Q_el_vec_kW[timestep];

    if(this->compression_included && this->electrolyzer.Q_el_vec_kW[timestep] > 0){
        double compression_Q_kW = (compressor_spec_consumption_kWh * (this->el_capacity_kW/this->el_spec_consumption_kWh)) / (1 + (1 - this->n_compressor));
        thermal_output_kW += compression_Q_kW;
        compression_Q_vec_kW[timestep] = compression_Q_kW;
    }


    return thermal_output_kW;
}

// ---------------------------------------------------------------------------------- //

///
/// \fn double H2 :: getMcp(int);
///
/// \brief Method to get the m * cp of the electorlyzer and fuel cell
///
/// \param timestep The current time step of the Model run.
///
/// \return The m * cp of the electorlyzer and fuel cell [J/K]
///

double H2 :: getMcp(
    int timestep
) 
{
    // calculate m * cp for electrolyzer for thermal energy modelling
    double mcp = (this->p_el * this->el_capacity_kW * this->cp_el) + (this->p_fc * this->fc_capacity_kW * this->cp_fc);

    return mcp;
}

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //
///
/// \fn void H2 :: getCompressionPower(int timestep, double dt_hrs)
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \brief A method to compute the energy needed to compress the output hydrogen at a given timestep
///

double H2 :: getCompressorPowerkW(double dt_hrs)
{
    double compression_power_kW = compressor_spec_consumption_kWh * (this->el_capacity_kW/this->el_spec_consumption_kWh);

    if(not this->compression_included){
        compression_power_kW = 0;
    }
    return compression_power_kW;
}   /* getCompressorPowerkW() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //
///
/// \fn void H2 :: getExternalHydrogenLoadkg(int timestep, double dt_hrs)
///
/// \brief A method to read in the external hydrogen load 
///

void H2 :: getExternalHydrogenLoadkg(std::string path_2_external_hydrogen_load_time_series)
{

    io::CSVReader<2> CSV(path_2_external_hydrogen_load_time_series);

    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Hydrogen Load [kg]"
    );

    this->path_2_external_hydrogen_load_time_series = path_2_external_hydrogen_load_time_series;

    //  3. read in temperature data, 
    this->n_points = 0;
    double time_hrs = 0;
    double hydrogen_load = 0;
    
    while (CSV.read_row(time_hrs, hydrogen_load)) {       
        this->hydrogen_load_vec_kg.push_back(hydrogen_load);        
        this->n_points++;
    }

    return;
}   /* getExternalHydrogenLoadkg() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //
///
/// \fn void H2 :: commitExternalHydrogenLoadkg(int timestep, double dt_hrs)
///
/// \brief a helper method to withdraw the external hydrogen load from the hydrogen tank
///

double H2 :: commitExternalHydrogenLoadkg(int timestep, double dt_hrs)
{
    double charging_kW = 0;
    this->making_hydrogen_for_external_load = false;

    if(this->hydrogen_load_vec_kg[timestep]> 0){
        // 1. check if hydrogen tank has enough to meet hydrogen load
        if(this->tank_level_kg >= this->hydrogen_load_vec_kg[timestep]){
            // if yes, deplete tank amount meet hydrogen load
            this->tank_level_kg -= this->hydrogen_load_vec_kg[timestep];
            // update tank vector
            this->tank_level_vec_kg[timestep] = this->tank_level_kg;
        }

        else { 
            // 2. if not, compute charging power needed, neglecting compression
            charging_kW = (this->hydrogen_load_vec_kg[timestep] * this->el_spec_consumption_kWh / dt_hrs);
            // check that hydrogen load does not exceed electrolyzer power capacity 
            double power_constraint_upper_kW = this->el_capacity_kW * this->el_quantity; 
            if (charging_kW > power_constraint_upper_kW) {
                std::cout << "hydrogen load exceeds electrolyzer capacity at timestep: " << timestep << std::endl;
                charging_kW = 0;
            }

            // initiate electrolyzer methods to make hydrogen for external load
            this->electrolyzer.commitCharge(timestep,dt_hrs,this->el_spec_consumption_kWh,charging_kW,this->el_capacity_kW);
            this->el_output_kg = this->electrolyzer.el_output_vec_kg[timestep];
            this->electrolyzer.getQ_el(timestep,dt_hrs,charging_kW,this->el_output_kg);
            this->getWaterDemand(timestep,this->el_output_kg);
            this->__handleDegradation(timestep,dt_hrs);

            // indicate that electrolyzer is running for external load
            this->el_is_running = true; 
            this->el_runtime_current_hrs += dt_hrs;
            this->making_hydrogen_for_external_load = true;
            this->el_runtime_hrs += dt_hrs;
            this->el_runtime_hrs_total += dt_hrs;
        }
        this->total_external_hydrogen_load_met += this->hydrogen_load_vec_kg[timestep];
    }
    return charging_kW;
}   /* commitExternalHydrogenLoadkg() */

// ---------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------- //
///
/// \fn void H2 :: commitCurtailmentHydrogen(int timestep, double dt_hrs)
///
/// \brief A method to compute potential for hydrogen generation for immediate use from curtailed energy 
///

void H2 :: commitCurtailmentHydrogen(int timestep, double dt_hrs, double unused_curtailment)
{

    // run electrolysis methods if we are considering curtailment hydrogen potential AND Tank is already full
    if(excess_hydrogen_potential_included && (this->tank_level_kg >= (this->max_SOC * this->h2_tank_capacity_kg))){

        double max_charging_kW = this->el_capacity_kW * this->el_quantity;
        double min_charging_kW = this->el_capacity_kW*this->el_quantity*this->el_min_load_ratio;
        double charging_kW = unused_curtailment;
    
        if (unused_curtailment > max_charging_kW){
            charging_kW = max_charging_kW;
        } else if (unused_curtailment < min_charging_kW){
            charging_kW = 0;
        }

        // run electrolysis methods 
        this->electrolyzer.commitCharge(timestep,dt_hrs,this->el_spec_consumption_kWh,charging_kW,this->el_capacity_kW);
        this->el_output_kg = this->electrolyzer.el_output_vec_kg[timestep];
        this->electrolyzer.getQ_el(timestep,dt_hrs,charging_kW,this->el_output_kg);
        this->getWaterDemand(timestep,this->el_output_kg);
        this->__handleDegradation(timestep,dt_hrs); 
        
        // update vector and total
        curtailed_hydrogen_vec_kg[timestep] = this->el_output_kg;
        this->total_curtailed_hydrogen_potential += this->el_output_kg;

    } 

    return;
}   /* commitCurtailmentHydrogen() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void H2 :: getCompressionPower(int timestep, double dt_hrs)
///
/// \brief A method to compute the energy needed to compress the output hydrogen at a given timestep
///

void H2 :: getWaterDemand(
    int timestep,
    double el_output_kg
    )
{
    double water_demand_kg = el_output_kg*this->water_demand_L;
    this->water_demand_vec_kg[timestep] = water_demand_kg;
    this->total_water_demand_kg += water_demand_kg;
    
    return;
}   /* getTemperature() */

// ---------------------------------------------------------------------------------- //

///
/// \fn H2 :: ~H2(void)
///
/// \brief Destructor for the H2 class.
///  

H2 :: ~H2(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "H2 object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~H2() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
