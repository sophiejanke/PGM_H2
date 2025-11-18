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
/// \file example_project.cpp
///
/// \brief An example PGMcpp project, accessed natively in C++.
///
/// This is an example PGMcpp project. See comments below for guidance.
///


#include "../header/Model.h"


int main(int argc, char** argv)
{
    /*
     *  1. construct Model object
     *
     *  This block constructs a Model object, which is the central container for the 
     *  entire microgrid model.
     *
     *  The fist argument that must be provided to the Model constructor is a valid
     *  path (either relative or absolute) to a time series of electrical load data.
     *  For an example of the expected format, see 
     *
     *  data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv
     *
     *  Note that the length of the given electrical load time series defines the
     *  modelled project life (so if you want to model n years of microgrid operation,
     *  then you must pass a path to n years worth of electrical load data). In addition,
     *  the given electrical load time series defines which points in time are modelled.
     *  As such, all subsequent time series data which is passed in must (1) be of the
     *  same length as the electrical load time series, and (2) provide data for the
     *  same set of points in time. Of course, the electrical load time series can be
     *  of arbitrary length, and it need not be a uniform time series.
     *
     *  The second argument that one can provide is the desired disptach control mode.
     *  If nothing is given here, then the model will default to simple load following
     *  control. However, one can stipulate which control mode to use by altering the
     *  control_mode attribute of the ModelInputs structure. In this case, the
     *  cycle charging control mode is being set.
     */
    
    std::string path_2_electrical_load_time_series =
        "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";
    
    ModelInputs model_inputs;
    
    model_inputs.path_2_electrical_load_time_series = 
        path_2_electrical_load_time_series;
    
    model_inputs.control_mode = ControlMode :: CYCLE_CHARGING;
    
    Model model(model_inputs);



    /*
     *  2. add Diesel objects to Model
     *
     *  This block defines and adds a set of diesel generators to the Model object.
     *
     *  In this example, a single DieselInputs structure is used to define and add 
     *  three diesel generators to the model.
     *
     *  The first diesel generator is defined as a 300 kW generator (which shows an 
     *  example of how to access and alter an encapsulated attribute of DieselInputs).
     *  In addition, the diesel generator is taken to be a sunk cost (and so no capital
     *  cost is incurred in the first time step; the opposite is true for non-sunk
     *  assets).
     *
     *  The last two diesel generators are defined as 150 kW each. Likewise, they are
     *  also sunk assets (since the same DieselInputs structure is being re-used without
     *  overwriting the is_sunk attribute).
     *
     *  For more details on the various attributes of DieselInputs, refer to the
     *  PGMcpp manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     */
    
    DieselInputs diesel_inputs;
    
    //  2.1. add 1 x 300 kW diesel generator (since mean load is ~250 kW)
    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 300;
    diesel_inputs.combustion_inputs.production_inputs.is_sunk = true;
    
    model.addDiesel(diesel_inputs);
    
    //  2.2. add 2 x 150 kW diesel generators (since max load is 500 kW)
    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 150;
    
    model.addDiesel(diesel_inputs);
    model.addDiesel(diesel_inputs);



    /*
     *  3. add renewable resources to Model
     *
     *  This block adds a set of renewable resource time series to the Model object.
     *
     *  The first resource added is a solar resource time series, which gives 
     *  horizontal irradiance [kW/m2] at each point in time. Again, remember that all
     *  given time series must align with the electrical load time series (i.e., same
     *  length, same points). For an example of the expected format, see
     *
     *  data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv
     *
     *  Finally, note the declaration of a solar resource key. This variable will be
     *  re-used later to associate a solar PV array object with this particular solar
     *  resource. This method of key association between resource and asset allows for
     *  greater flexibility in modelling production assets that are exposed to different
     *  renewable resources (due to being geographically separated, etc.).
     *
     *  The second resource added is a tidal resource time series, which gives tidal
     *  stream speed [m/s] at each point in time. For an example of the expected format,
     *  see
     *
     *  data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv
     *
     *  Again, note the tidal resource key.
     *
     *  The third resource added is a wave resource time series, which gives significant
     *  wave height [m] and energy period [s] at each point in time. For an example of
     *  the expected format, see
     *
     *  data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv
     *
     *  Again, note the wave resource key.
     *
     *  The fourth resource added is a wind resource time series, which gives wind speed
     *  [m/s] at each point in time. For an example of the expected format, see
     *
     *  data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv
     *
     *  Again, note the wind resource key.
     *
     *  The fifth resource added is a hydro resource time series, which gives inflow
     *  rate [m3/hr] at each point in time. For an example of the expected format, see
     *
     *  data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv
     *
     *  Again, note the hydro resource key.
     */
    
    //  3.1. add solar resource time series
    int solar_resource_key = 0;
    std::string path_2_solar_resource_data =
        "data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv";
    
    model.addResource(
        RenewableType :: SOLAR,
        path_2_solar_resource_data,
        solar_resource_key
    );
    
    //  3.2. add tidal resource time series
    int tidal_resource_key = 1;
    std::string path_2_tidal_resource_data =
        "data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv";
    
    model.addResource(
        RenewableType :: TIDAL,
        path_2_tidal_resource_data,
        tidal_resource_key
    );
    
    //  3.3. add wave resource time series
    int wave_resource_key = 2;
    std::string path_2_wave_resource_data =
        "data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv";
    
    model.addResource(
        RenewableType :: WAVE,
        path_2_wave_resource_data,
        wave_resource_key
    );
    
    //  3.4. add wind resource time series
    int wind_resource_key = 3;
    std::string path_2_wind_resource_data =
        "data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv";
    
    model.addResource(
        RenewableType :: WIND,
        path_2_wind_resource_data,
        wind_resource_key
    );
    
    //  3.5. add hydro resource time series
    int hydro_resource_key = 4;
    std::string path_2_hydro_resource_data =
        "data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv";
    
    model.addResource(
        NoncombustionType :: HYDRO,
        path_2_hydro_resource_data,
        hydro_resource_key
    );



    /*
     *  4. add Hydro object to Model
     *
     *  This block defines and adds a hydroelectric asset to the Model object.
     *
     *  In this example, a 300 kW hydroelectric station with a 10,000 m3 reservoir
     *  is defined. The initial reservoir state is set to 50% (so half full), and the
     *  hydroelectric asset is taken to be a sunk asset (so no capital cost incurred
     *  in the first time step). Note the association with the previously given hydro
     *  resource series by way of the hydro resource key.
     *
     *  For more details on the various attributes of HydroInputs, refer to the
     *  PGMcpp manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     */
    
    HydroInputs hydro_inputs;
    hydro_inputs.noncombustion_inputs.production_inputs.capacity_kW = 300;
    hydro_inputs.reservoir_capacity_m3 = 10000;
    hydro_inputs.init_reservoir_state = 0.5;
    hydro_inputs.noncombustion_inputs.production_inputs.is_sunk = true;
    hydro_inputs.resource_key = hydro_resource_key;
    
    model.addHydro(hydro_inputs);



    /*
     *  5. add Renewable objects to Model
     *
     *  This block defines and adds a set of renewable production assets to the Model
     *  object.
     *
     *  The first block defines and adds a solar PV array to the Model object. In this
     *  example, the installed solar capacity is set to 250 kW. Note the association
     *  with the previously given solar resource series by way of the solar resource
     *  key. Also, note that this asset is not taken as sunk (as the is_sunk attribute
     *  of the SolarInputs structure is unchanged and thus defaults to true). As such,
     *  this asset will incur a capital cost in the first time step.
     *
     *  For more details on the various attributes of SolarInputs, refer to the PGMcpp
     *  manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     *
     *  The second block defines and adds a tidal turbine to the Model object. In this
     *  example, the installed tidal capacity is set to 120 kW. In addition, the design
     *  speed of the asset (i.e., the speed at which the rated capacity is achieved) is
     *  set to 2.5 m/s. Note the association with the previously given tidal resource
     *  series by way of the tidal resource key.
     *
     *  For more details on the various attributes of TidalInputs, refer to the PGMcpp
     *  manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     *
     *  The third block defines and adds a wind turbine to the Model object. In this
     *  example, the installed wind capacity is set to 150 kW. In addition, the design
     *  speed of the asset is not given, and so will default to 8 m/s. Note the
     *  association with the previously given tidal resource series by way of the wind
     *  resource key.
     *
     *  For more details on the various attributes of WindInputs, refer to the PGMcpp
     *  manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     *
     *  The fourth block defines and adds a wave energy converter to the Model object.
     *  In this example, the installed wave capacity is set to 100 kW. Note the
     *  association with the previously given wave resource series by way of the wave
     *  resource key.
     *
     *  For more details on the various attributes of WaveInputs, refer to the PGMcpp
     *  manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     */
    
    //  5.1. add 1 x 250 kW solar PV array
    SolarInputs solar_inputs;
    
    solar_inputs.renewable_inputs.production_inputs.capacity_kW = 250;
    solar_inputs.resource_key = solar_resource_key;
    
    model.addSolar(solar_inputs);
    
    //  5.2. add 1 x 120 kW tidal turbine
    TidalInputs tidal_inputs;
    
    tidal_inputs.renewable_inputs.production_inputs.capacity_kW = 120;
    tidal_inputs.design_speed_ms = 2.5;
    tidal_inputs.resource_key = tidal_resource_key;
    
    model.addTidal(tidal_inputs);
    
    //  5.3. add 1 x 150 kW wind turbine
    WindInputs wind_inputs;
    
    wind_inputs.renewable_inputs.production_inputs.capacity_kW = 150;
    wind_inputs.resource_key = wind_resource_key;
    
    model.addWind(wind_inputs);
    
    //  5.4. add 1 x 100 kW wave energy converter
    WaveInputs wave_inputs;
    
    wave_inputs.renewable_inputs.production_inputs.capacity_kW = 100;
    wave_inputs.resource_key = wave_resource_key;
    
    model.addWave(wave_inputs);



    /*
     *  6. add LiIon object to Model
     *
     *  This block defines and adds a lithium ion battery energy storage system to the
     *  Model object.
     *
     *  In this example, a battery energy storage system with a 500 kW power capacity
     *  and a 1050 kWh energy capacity (which represents about four hours of mean load
     *  autonomy) is defined.
     *
     *  For more details on the various attributes of LiIonInputs, refer to the PGMcpp
     *  manual. For instance, note that no economic inputs are given; in this
     *  example, the default values apply.
     */
    
    //  6.1. add 1 x (500 kW, ) lithium ion battery energy storage system
    LiIonInputs liion_inputs;
    
    liion_inputs.storage_inputs.power_capacity_kW = 500;
    liion_inputs.storage_inputs.energy_capacity_kWh = 1050;
    
    model.addLiIon(liion_inputs);



    /*
     *  7. run and write results
     *
     *  This block runs the model and then writes results to the given output path
     *  (either relative or absolute). Note that the writeResults() will create the
     *  last directory on the given path, but not any in-between directories, so be
     *  sure those exist before calling out to this method.
     */
    
    model.run();
    
    model.writeResults("projects/example_cpp");
    
    return 0;
}   /* main() */
