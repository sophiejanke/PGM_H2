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
/// \file Resources.cpp
///
/// \brief Implementation file for the Resources class.
///
/// A class which contains renewable resource data. Intended to serve as a component
/// class of Model.
///


#include "../header/Resources.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __checkResourceKey1D(
///         int resource_key,
///         RenewableType renewable_type
///     )
///
/// \brief Helper method to check if given resource key (1D) is already in use.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param renewable_type The type of renewable resource being added to Resources.
///

void Resources :: __checkResourceKey1D(
    int resource_key,
    RenewableType renewable_type
)
{
    if (this->resource_map_1D.count(resource_key) > 0) {
        std::string error_str = "ERROR:  Resources::addResource(";
        
        switch (renewable_type) {
            case (RenewableType :: SOLAR): {
                error_str += "SOLAR):  ";
                
                break;
            }
            
            case (RenewableType :: TIDAL): {
                error_str += "TIDAL):  ";
                
                break;
            }
            
            case (RenewableType :: WIND): {
                error_str += "WIND):  ";
                
                break;
            }
            
            default: {
                error_str += "UNDEFINED_TYPE):  ";
                
                break;
            }
        }
        
        error_str += "resource key (1D) ";
        error_str += std::to_string(resource_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /*  __checkResourceKey1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __checkResourceKey1D(
///         int resource_key,
///         NoncombustionType noncombustion_type
///     )
///
/// \brief Helper method to check if given resource key (1D) is already in use.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param noncombustion_type The type of renewable resource being added to Resources.
///

void Resources :: __checkResourceKey1D(
    int resource_key,
    NoncombustionType noncombustion_type
)
{
    if (this->resource_map_1D.count(resource_key) > 0) {
        std::string error_str = "ERROR:  Resources::addResource(";
        
        switch (noncombustion_type) {
            case (NoncombustionType :: HYDRO): {
                error_str += "HYDRO):  ";
                
                break;
            }
            
            default: {
                error_str += "UNDEFINED_TYPE):  ";
                
                break;
            }
        }
        
        error_str += "resource key (1D) ";
        error_str += std::to_string(resource_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /*  __checkResourceKey1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __checkResourceKey2D(
///         int resource_key,
///         RenewableType renewable_type
///     )
///
/// \brief Helper method to check if given resource key (2D) is already in use.
///
/// \param resource_key The key associated with the given renewable resource.
///

void Resources :: __checkResourceKey2D(
    int resource_key,
    RenewableType renewable_type
)
{
    if (this->resource_map_2D.count(resource_key) > 0) {
        std::string error_str = "ERROR:  Resources::addResource(";
        
        switch (renewable_type) {
            case (RenewableType :: WAVE): {
                error_str += "WAVE):  ";
                
                break;
            }
            
            default: {
                error_str += "UNDEFINED_TYPE):  ";
                
                break;
            }
        }
        
        error_str += "resource key (2D) ";
        error_str += std::to_string(resource_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /*  __checkResourceKey2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __checkTimePoint(
///         double time_received_hrs,
///         double time_expected_hrs,
///         std::string path_2_resource_data,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to check received time point against expected time point. The
///     given time series should align point-wise with the previously given electrical
///     load time series.
///
/// \param time_received_hrs The point in time received from the given data.
///
/// \param time_expected_hrs The point in time expected (this comes from the electrical
///     load time series).
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __checkTimePoint(
    double time_received_hrs,
    double time_expected_hrs,
    std::string path_2_resource_data,
    ElectricalLoad* electrical_load_ptr
)
{
    if (time_received_hrs != time_expected_hrs) {
        std::string error_str = "ERROR:  Resources::addResource():  ";
        error_str += "the given resource time series at ";
        error_str += path_2_resource_data;
        error_str += " does not align with the ";
        error_str += "previously given electrical load time series at ";
        error_str += electrical_load_ptr->path_2_electrical_load_time_series;
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::runtime_error(error_str);
    }
    
    return;
}   /* __checkTimePoint() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __throwLengthError(
///         std::string path_2_resource_data,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to throw data length error (if not the same as the given 
///     electrical load time series).
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __throwLengthError(
    std::string path_2_resource_data,
    ElectricalLoad* electrical_load_ptr
)
{
    std::string error_str = "ERROR:  Resources::addResource():  ";
    error_str += "the given resource time series at ";
    error_str += path_2_resource_data;
    error_str += " is not the same length as the previously given electrical";
    error_str += " load time series at ";
    error_str += electrical_load_ptr->path_2_electrical_load_time_series;
    
    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    
    return;
}   /* __throwLengthError() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __readHydroResource(
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to handle reading a hydro resource time series into Resources.
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __readHydroResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    //  1. init CSV reader, record path and type
    io::CSVReader<2> CSV(path_2_resource_data);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Hydro Inflow [m3/hr]"
    );
    
    this->path_map_1D.insert(
        std::pair<int, std::string>(resource_key, path_2_resource_data)
    );
    
    this->string_map_1D.insert(std::pair<int, std::string>(resource_key, "HYDRO"));
    
    //  2. init map element
    this->resource_map_1D.insert(
        std::pair<int, std::vector<double>>(resource_key, {})
    );
    this->resource_map_1D[resource_key].resize(electrical_load_ptr->n_points, 0);
    
    
    //  3. read in resource data, check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double hydro_resource_m3hr = 0;
    
    while (CSV.read_row(time_hrs, hydro_resource_m3hr)) {
        if (n_points > electrical_load_ptr->n_points) {
            this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
        }
        
        time_expected_hrs = electrical_load_ptr->time_vec_hrs[n_points];
        this->__checkTimePoint(
            time_hrs,
            time_expected_hrs,
            path_2_resource_data,
            electrical_load_ptr
        );
        
        this->resource_map_1D[resource_key][n_points] = hydro_resource_m3hr;
        
        n_points++;
    }
    
    //  4. check data length
    if (n_points != electrical_load_ptr->n_points) {
        this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
    }
    
    return;
}   /* __readHydroResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __readSolarResource(
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to handle reading a solar resource time series into Resources.
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __readSolarResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    //  1. init CSV reader, record path and type
    io::CSVReader<2> CSV(path_2_resource_data);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Solar GHI [kW/m2]"
    );
    
    this->path_map_1D.insert(
        std::pair<int, std::string>(resource_key, path_2_resource_data)
    );
    
    this->string_map_1D.insert(std::pair<int, std::string>(resource_key, "SOLAR"));
    
    //  2. init map element
    this->resource_map_1D.insert(
        std::pair<int, std::vector<double>>(resource_key, {})
    );
    this->resource_map_1D[resource_key].resize(electrical_load_ptr->n_points, 0);
    
    
    //  3. read in resource data, check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double solar_resource_kWm2 = 0;
    
    while (CSV.read_row(time_hrs, solar_resource_kWm2)) {
        if (n_points > electrical_load_ptr->n_points) {
            this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
        }
        
        time_expected_hrs = electrical_load_ptr->time_vec_hrs[n_points];
        this->__checkTimePoint(
            time_hrs,
            time_expected_hrs,
            path_2_resource_data,
            electrical_load_ptr
        );
        
        this->resource_map_1D[resource_key][n_points] = solar_resource_kWm2;
        
        n_points++;
    }
    
    //  4. check data length
    if (n_points != electrical_load_ptr->n_points) {
        this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
    }
    
    return;
}   /* __readSolarResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __readTidalResource(
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to handle reading a tidal resource time series into Resources.
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __readTidalResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    //  1. init CSV reader, record path and type
    io::CSVReader<2> CSV(path_2_resource_data);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Tidal Speed (hub depth) [m/s]"
    );
    
    this->path_map_1D.insert(
        std::pair<int, std::string>(resource_key, path_2_resource_data)
    );
    
    this->string_map_1D.insert(std::pair<int, std::string>(resource_key, "TIDAL"));
    
    //  2. init map element
    this->resource_map_1D.insert(
        std::pair<int, std::vector<double>>(resource_key, {})
    );
    this->resource_map_1D[resource_key].resize(electrical_load_ptr->n_points, 0);
    
    
    //  3. read in resource data, check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double tidal_resource_ms = 0;
    
    while (CSV.read_row(time_hrs, tidal_resource_ms)) {
        if (n_points > electrical_load_ptr->n_points) {
            this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
        }
        
        time_expected_hrs = electrical_load_ptr->time_vec_hrs[n_points];
        this->__checkTimePoint(
            time_hrs,
            time_expected_hrs,
            path_2_resource_data,
            electrical_load_ptr
        );
        
        this->resource_map_1D[resource_key][n_points] = tidal_resource_ms;
        
        n_points++;
    }
    
    //  4. check data length
    if (n_points != electrical_load_ptr->n_points) {
        this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
    }
    
    return;
}   /* __readTidalResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __readWaveResource(
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to handle reading a wave resource time series into Resources.
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __readWaveResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    //  1. init CSV reader, record path and type
    io::CSVReader<3> CSV(path_2_resource_data);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Significant Wave Height [m]",
        "Energy Period [s]"
    );
    
    this->path_map_2D.insert(
        std::pair<int, std::string>(resource_key, path_2_resource_data)
    );
    
    this->string_map_2D.insert(std::pair<int, std::string>(resource_key, "WAVE"));
    
    //  2. init map element
    this->resource_map_2D.insert(
        std::pair<int, std::vector<std::vector<double>>>(resource_key, {})
    );
    this->resource_map_2D[resource_key].resize(electrical_load_ptr->n_points, {0, 0});
    
    
    //  3. read in resource data, check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double significant_wave_height_m = 0;
    double energy_period_s = 0;
    
    while (CSV.read_row(time_hrs, significant_wave_height_m, energy_period_s)) {
        if (n_points > electrical_load_ptr->n_points) {
            this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
        }
        
        time_expected_hrs = electrical_load_ptr->time_vec_hrs[n_points];
        this->__checkTimePoint(
            time_hrs,
            time_expected_hrs,
            path_2_resource_data,
            electrical_load_ptr
        );
        
        this->resource_map_2D[resource_key][n_points][0] = significant_wave_height_m;
        this->resource_map_2D[resource_key][n_points][1] = energy_period_s;
        
        n_points++;
    }
    
    //  4. check data length
    if (n_points != electrical_load_ptr->n_points) {
        this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
    }
    
    return;
}   /* __readWaveResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: __readWindResource(
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief Helper method to handle reading a wind resource time series into Resources.
///
/// \param path_2_resource_data The path (either relative or absolute) to the given 
///     resource time series.
///
/// \param resource_key The key associated with the given renewable resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: __readWindResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    //  1. init CSV reader, record path and type
    io::CSVReader<2> CSV(path_2_resource_data);
    
    CSV.read_header(
        io::ignore_extra_column,
        "Time (since start of data) [hrs]",
        "Wind Speed (hub height) [m/s]"
    );
    
    this->path_map_1D.insert(
        std::pair<int, std::string>(resource_key, path_2_resource_data)
    );
    
    this->string_map_1D.insert(std::pair<int, std::string>(resource_key, "WIND"));
    
    //  2. init map element
    this->resource_map_1D.insert(
        std::pair<int, std::vector<double>>(resource_key, {})
    );
    this->resource_map_1D[resource_key].resize(electrical_load_ptr->n_points, 0);
    
    
    //  3. read in resource data, check against time series (point-wise and length)
    int n_points = 0;
    double time_hrs = 0;
    double time_expected_hrs = 0;
    double wind_resource_ms = 0;
    
    while (CSV.read_row(time_hrs, wind_resource_ms)) {
        if (n_points > electrical_load_ptr->n_points) {
            this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
        }
        
        time_expected_hrs = electrical_load_ptr->time_vec_hrs[n_points];
        this->__checkTimePoint(
            time_hrs,
            time_expected_hrs,
            path_2_resource_data,
            electrical_load_ptr
        );
        
        this->resource_map_1D[resource_key][n_points] = wind_resource_ms;
        
        n_points++;
    }
    
    //  4. check data length
    if (n_points != electrical_load_ptr->n_points) {
        this->__throwLengthError(path_2_resource_data, electrical_load_ptr);
    }
    
    return;
}   /* __readWindResource() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Resources :: Resources(void)
///
/// \brief Constructor for the Resources class.
///

Resources :: Resources(void)
{
    return;
}   /* Resources() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: addResource(
///         NoncombustionType noncombustion_type,
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief A method to add a renewable resource time series to Resources. Checks if
///     given resource key is already in use. The associated helper methods also
///     check against ElectricalLoad to ensure that all added time series align with the
///     electrical load time series (both in terms of length and which points in time
///     are included).
///
/// \param noncombustion_type The type of renewable resource being added to Resources.
///
/// \param path_2_resource_data A string defining the path (either relative or absolute) to the given resource time series.
///
/// \param resource_key A key used to index into the Resources object, used to associate Renewable assets with the corresponding resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: addResource(
    NoncombustionType noncombustion_type,
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    switch (noncombustion_type) {
        case (NoncombustionType :: HYDRO): {
            this->__checkResourceKey1D(resource_key, noncombustion_type);
            
            this->__readHydroResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Resources :: addResource(:  ";
            error_str += "noncombustion type ";
            error_str += std::to_string(noncombustion_type);
            error_str += " has no associated resource";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return;
}   /* addResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: addResource(
///         RenewableType renewable_type,
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief A method to add a renewable resource time series to Resources. Checks if
///     given resource key is already in use. The associated helper methods also
///     check against ElectricalLoad to ensure that all added time series align with the
///     electrical load time series (both in terms of length and which points in time
///     are included).
///
/// \param renewable_type The type of renewable resource being added to Resources.
///
/// \param path_2_resource_data A string defining the path (either relative or absolute) to the given resource time series.
///
/// \param resource_key A key used to index into the Resources object, used to associate Renewable assets with the corresponding resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: addResource(
    RenewableType renewable_type,
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    switch (renewable_type) {
        case (RenewableType :: SOLAR): {
            this->__checkResourceKey1D(resource_key, renewable_type);
            
            this->__readSolarResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: TIDAL): {
            this->__checkResourceKey1D(resource_key, renewable_type);
            
            this->__readTidalResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: WAVE): {
            this->__checkResourceKey2D(resource_key, renewable_type);
            
            this->__readWaveResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: WIND): {
            this->__checkResourceKey1D(resource_key, renewable_type);
            
            this->__readWindResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Resources :: addResource(:  ";
            error_str += "renewable type ";
            error_str += std::to_string(renewable_type);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return;
}   /* addResource() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: clear(void)
///
/// \brief Method to clear all attributes of the Resources object.
///

void Resources :: clear(void)
{
    this->resource_map_1D.clear();
    this->string_map_1D.clear();
    this->path_map_1D.clear();
    
    this->resource_map_2D.clear();
    this->string_map_2D.clear();
    this->path_map_2D.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn Resources :: ~Resources(void)
///
/// \brief Destructor for the Resources class.
///

Resources :: ~Resources(void)
{
    this->clear();
    return;
}   /* ~Resources() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
