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
/// \file Interpolator.cpp
///
/// \brief Implementation file for the Interpolator class.
///
/// A class which contains interpolation data and functionality. Intended to serve as a
/// component of the Production and Storage hierarchies.
///


#include "../header/Interpolator.h"



// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey1D(int data_key)
///
/// \brief Helper method to check if given data key (1D) is already in use.
///
/// \param data_key The key associated with the given 1D interpolation data.
///

void Interpolator :: __checkDataKey1D(int data_key)
{
    if (this->interp_map_1D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData1D()  ";
        error_str += "data key (1D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey2D(int data_key)
///
/// \brief Helper method to check if given data key (2D) is already in use.
///
/// \param data_key The key associated with the given 2D interpolation data.
///

void Interpolator :: __checkDataKey2D(int data_key)
{
    if (this->interp_map_2D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData2D()  ";
        error_str += "data key (2D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkBounds1D(int data_key, double interp_x)
///
/// \brief Helper method to check that the given 1D interpolation value is contained 
///     within the given corresponding data domain. Also checks that the data key has
///     been registered.
///
/// \param data_key A key associated with the given interpolation data.
///
/// \param interp_x The query value to be interpolated.
///

void Interpolator :: __checkBounds1D(int data_key, double interp_x)
{
    //  1. key error
    if (this->interp_map_1D.count(data_key) == 0) {
        std::string error_str = "ERROR:  Interpolator::interp1D()  ";
        error_str += "data key ";
        error_str += std::to_string(data_key);
        error_str += " has not been registered";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. bounds error
    if (
        interp_x < this->interp_map_1D[data_key].min_x or
        interp_x > this->interp_map_1D[data_key].max_x
    ) {
        std::string error_str = "ERROR:  Interpolator::interp1D()  ";
        error_str += "interpolation value ";
        error_str += std::to_string(interp_x);
        error_str += " is outside of the given interpolation data domain [";
        error_str += std::to_string(this->interp_map_1D[data_key].min_x);
        error_str += " , ";
        error_str += std::to_string(this->interp_map_1D[data_key].max_x);
        error_str += "]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkBounds1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkBounds2D(int data_key, double interp_x, double interp_y)
///
/// \brief Helper method to check that the given 2D interpolation value is contained 
///     within the given corresponding data domain. Also checks that the data key has
///     been registered.
///
/// \param data_key A key associated with the given interpolation data.
///
/// \param interp_x The first query value to be interpolated.
///
/// \param interp_y The second query value to be interpolated.
///

void Interpolator :: __checkBounds2D(int data_key, double interp_x, double interp_y)
{
    //  1. key error
    if (this->interp_map_2D.count(data_key) == 0) {
        std::string error_str = "ERROR:  Interpolator::interp2D()  ";
        error_str += "data key ";
        error_str += std::to_string(data_key);
        error_str += " has not been registered";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. bounds error (x_interp)
    if (
        interp_x < this->interp_map_2D[data_key].min_x or
        interp_x > this->interp_map_2D[data_key].max_x
    ) {
        std::string error_str = "ERROR:  Interpolator::interp2D()  ";
        error_str += "interpolation value interp_x = ";
        error_str += std::to_string(interp_x);
        error_str += " is outside of the given interpolation data domain [";
        error_str += std::to_string(this->interp_map_2D[data_key].min_x);
        error_str += " , ";
        error_str += std::to_string(this->interp_map_2D[data_key].max_x);
        error_str += "]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. bounds error (y_interp)
    if (
        interp_y < this->interp_map_2D[data_key].min_y or
        interp_y > this->interp_map_2D[data_key].max_y
    ) {
        std::string error_str = "ERROR:  Interpolator::interp2D()  ";
        error_str += "interpolation value interp_y = ";
        error_str += std::to_string(interp_y);
        error_str += " is outside of the given interpolation data domain [";
        error_str += std::to_string(this->interp_map_2D[data_key].min_y);
        error_str += " , ";
        error_str += std::to_string(this->interp_map_2D[data_key].max_y);
        error_str += "]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkBounds2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __throwReadError(std::string path_2_data, int dimensions)
///
/// \brief Helper method to throw a read error whenever non-numeric data is encountered
///     where only numeric data should be.
///
/// \param path_2_data The path (either relative or absolute) to the given interpolation
///     data.
///
/// \param dimensions The dimensionality of the data being read.
///

void Interpolator :: __throwReadError(std::string path_2_data, int dimensions)
{
    std::string error_str = "ERROR:  Interpolator::addData";
    error_str += std::to_string(dimensions);
    error_str += "D()  ";
    error_str += " failed to read ";
    error_str += path_2_data;
    error_str += " (this is probably a std::stod() error; is there non-numeric ";
    error_str += "data where only numeric data should be?)";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    
    return;
}   /* __throwReadError() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn bool Interpolator :: __isNonNumeric(std::string str)
///
/// \brief Helper method to determine if given string is non-numeric (i.e., contains
//      one or more alpha characters).
///
/// \param str The string being tested.
///
/// \return A boolean indicating if the given string is non-numeric.
///

bool Interpolator :: __isNonNumeric(std::string str)
{
    for (size_t i = 0; i < str.size(); i++) {;
        if (isalpha(str[i])) {
            return true;
        }
    }
    
    return false;
}   /* __isAlpha() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn int Interpolator :: __getInterpolationIndex(
///         double interp_x,
///         std::vector<double>* x_vec_ptr
///     )
///
/// \brief Helper method to get appropriate interpolation index into given vector.
///
/// \param interp_x The query value to be interpolated.
///
/// \param x_vec_ptr A pointer to the given vector of interpolation data.
///
/// \return The appropriate interpolation index into the given vector.
///

int Interpolator :: __getInterpolationIndex(
    double interp_x,
    std::vector<double>* x_vec_ptr
)
{
    int idx = 0;
    while (
        not (interp_x >= x_vec_ptr->at(idx) and interp_x <= x_vec_ptr->at(idx + 1))
    ) {
        idx++;
    }
    
    return idx;
}   /* __getInterpolationIndex() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn std::vector<std::string> Interpolator :: __splitCommaSeparatedString(
///         std::string str,
///         std::string break_str
///     )
///
/// \brief Helper method to split a comma-separated string into a vector of substrings.
///
/// \param str The string to be split.
///
/// \param break_str A string which triggers the function to break. What has been split
///     up to the point of the break is then returned.
///
/// \return A vector of substrings, which follows from splitting the given string in a
///     comma separated manner.
///

std::vector<std::string> Interpolator :: __splitCommaSeparatedString(
    std::string str,
    std::string break_str
)
{
    std::vector<std::string> str_split_vec;
    
    size_t idx = 0;
    std::string substr;
    
    while ((idx = str.find(',')) != std::string::npos) {
        substr = str.substr(0, idx);
        
        if (substr == break_str) {
            break;
        }
        
        str_split_vec.push_back(substr);
        
        str.erase(0, idx + 1);
    }
    
    return str_split_vec;
}   /* __splitCommaSeparatedString() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn std::vector<std::vector<std::string>> Interpolator :: __getDataStringMatrix(
///         std::string path_2_data
///     )
///
/// \brief Helper method to read data into a string matrix (delimiters are commas and
///     newlines).
///
/// \param path_2_data The path (either relative or absolute) to the given interpolation
///     data.
///
/// \return A matrix of string values. Each cell corresponds to a single cell (or
///     element) of a comma separated data file.
///

std::vector<std::vector<std::string>> Interpolator :: __getDataStringMatrix(
    std::string path_2_data
)
{
    //  1. create input file stream
    std::ifstream ifs;
    ifs.open(path_2_data);
    
    //  2. check that open() worked
    if (not ifs.is_open()) {
        std::string error_str = "ERROR:  Interpolator::__getDataStringMatrix()  ";
        error_str += " failed to open ";
        error_str += path_2_data;
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. read file line by line
    bool is_header = true;
    std::string line;
    std::vector<std::string> line_split_vec;
    std::vector<std::vector<std::string>> string_matrix;
    
    while (not ifs.eof()) {
        std::getline(ifs, line);
        
        if (is_header) {
            is_header = false;
            continue;
        }
        
        line_split_vec = this->__splitCommaSeparatedString(line);
        
        if (not line_split_vec.empty()) {
            string_matrix.push_back(line_split_vec);
        }
    }
    
    ifs.close();
    return string_matrix;
}   /* __getDataStringMatrix() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __readData1D(int data_key, std::string path_2_data)
///
/// \brief Helper method to read the given 1D interpolation data into Interpolator.
///
/// \param data_key A key associated with the given interpolation data.
///
/// \param path_2_data The path (either relative or absolute) to the given interpolation
///     data.
///

void Interpolator :: __readData1D(int data_key, std::string path_2_data)
{
    //  1. get string matrix
    std::vector<std::vector<std::string>> string_matrix =
        this->__getDataStringMatrix(path_2_data);

    //  2. read string matrix contents into 1D interpolation struct
    InterpolatorStruct1D interp_struct_1D;
    
    interp_struct_1D.n_points = string_matrix.size();
    interp_struct_1D.x_vec.resize(interp_struct_1D.n_points, 0);
    interp_struct_1D.y_vec.resize(interp_struct_1D.n_points, 0);
    
    for (int i = 0; i < interp_struct_1D.n_points; i++) {
        try {
            interp_struct_1D.x_vec[i] = std::stod(string_matrix[i][0]);
            interp_struct_1D.y_vec[i] = std::stod(string_matrix[i][1]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 1);
        }
    }
    
    interp_struct_1D.min_x = interp_struct_1D.x_vec[0];
    interp_struct_1D.max_x = interp_struct_1D.x_vec[interp_struct_1D.n_points - 1];
    
    //  3. write struct to map
    this->interp_map_1D.insert(
        std::pair<int, InterpolatorStruct1D>(data_key, interp_struct_1D)
    );
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl;
    std::cout << path_2_data << std::endl;
    std::cout << "--------" << std::endl;
    
    std::cout << "n_points: " << this->interp_map_1D[data_key].n_points << std::endl;
    
    std::cout << "x_vec: [";
    for (
        int i = 0;
        i < this->interp_map_1D[data_key].n_points;
        i++
    ) {
        std::cout << this->interp_map_1D[data_key].x_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "y_vec: [";
    for (
        int i = 0;
        i < this->interp_map_1D[data_key].n_points;
        i++
    ) {
        std::cout << this->interp_map_1D[data_key].y_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << std::endl;
    // ==== END TEST PRINT ==== //
    //*/
    
    return;
}   /* __readData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __readData2D(int data_key, std::string path_2_data)
///
/// \brief Helper method to read the given 2D interpolation data into Interpolator.
///
/// \param data_key A key associated with the given interpolation data.
///
/// \param path_2_data The path (either relative or absolute) to the given interpolation
///     data.
///

void Interpolator :: __readData2D(int data_key, std::string path_2_data)
{
    //  1. get string matrix
    std::vector<std::vector<std::string>> string_matrix =
        this->__getDataStringMatrix(path_2_data);

    //  2. read string matrix contents into 2D interpolation map
    InterpolatorStruct2D interp_struct_2D;
    
    interp_struct_2D.n_rows = string_matrix.size() - 1;
    interp_struct_2D.n_cols = string_matrix[0].size() - 1;
    
    interp_struct_2D.x_vec.resize(interp_struct_2D.n_cols, 0);
    interp_struct_2D.y_vec.resize(interp_struct_2D.n_rows, 0);
    
    interp_struct_2D.z_matrix.resize(interp_struct_2D.n_rows, {});
    
    for (int i = 0; i < interp_struct_2D.n_rows; i++) {
        interp_struct_2D.z_matrix[i].resize(interp_struct_2D.n_cols, 0);
    }
    
    for (size_t i = 1; i < string_matrix[0].size(); i++) {
        try {
            interp_struct_2D.x_vec[i - 1] = std::stod(string_matrix[0][i]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 2);
        }
    }
    
    interp_struct_2D.min_x = interp_struct_2D.x_vec[0];
    interp_struct_2D.max_x = interp_struct_2D.x_vec[interp_struct_2D.n_cols - 1];
    
    for (size_t i = 1; i < string_matrix.size(); i++) {
        try {
            interp_struct_2D.y_vec[i - 1] = std::stod(string_matrix[i][0]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 2);
        }
    }
    
    interp_struct_2D.min_y = interp_struct_2D.y_vec[0];
    interp_struct_2D.max_y = interp_struct_2D.y_vec[interp_struct_2D.n_rows - 1];
    
    for (size_t i = 1; i < string_matrix.size(); i++) {
        for (size_t j = 1; j < string_matrix[0].size(); j++) {
            try {
                interp_struct_2D.z_matrix[i - 1][j - 1] = std::stod(string_matrix[i][j]);
            }
            
            catch (...) {
                this->__throwReadError(path_2_data, 2);
            }
        }
    }
    
    //  3. write struct to map
    this->interp_map_2D.insert(
        std::pair<int, InterpolatorStruct2D>(data_key, interp_struct_2D)
    );
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl;
    std::cout << path_2_data << std::endl;
    std::cout << "--------" << std::endl;
    
    std::cout << "n_rows: " << this->interp_map_2D[data_key].n_rows << std::endl;
    std::cout << "n_cols: " << this->interp_map_2D[data_key].n_cols << std::endl;
    
    std::cout << "x_vec: [";
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_cols;
        i++
    ) {
        std::cout << this->interp_map_2D[data_key].x_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "y_vec: [";
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_rows;
        i++
    ) {
        std::cout << this->interp_map_2D[data_key].y_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "z_matrix:" << std::endl;
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_rows;
        i++
    ) {
        std::cout << "\t[";
        
        for (
            int j = 0;
            j < this->interp_map_2D[data_key].n_cols;
            j++
        ) {
            std::cout << this->interp_map_2D[data_key].z_matrix[i][j] << ", ";
        }
        
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    // ==== END TEST PRINT ==== //
    //*/
    
    return;
}   /* __readData2D() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: Interpolator(void)
///
/// \brief Constructor for the Interpolator class.
///

Interpolator :: Interpolator(void)
{
    //...
    
    return;
}   /* Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData1D(int data_key, std::string path_2_data)
///
/// \brief Method to add 1D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 1D
///     interpolation data.
///

void Interpolator :: addData1D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey1D(data_key);
    
    //  2. read data into map
    this->__readData1D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_1D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData2D(int data_key, std::string path_2_data)
///
/// \brief Method to add 2D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 2D
///     interpolation data.
///

void Interpolator :: addData2D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey2D(data_key);
    
    //  2. read data into map
    this->__readData2D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_2D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Interpolator :: interp1D(int data_key, double interp_x)
///
/// \brief Method to perform a 1D interpolation.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param interp_x The query value to be interpolated. If this value is outside the
///     domain of the associated interpolation data, then an error will occur.
///
/// \return An interpolation of the given query value.
///

double Interpolator :: interp1D(int data_key, double interp_x)
{
    //  1. check bounds
    this->__checkBounds1D(data_key, interp_x);
    
    //  2. get interpolation index
    int idx = this->__getInterpolationIndex(
        interp_x,
        &(this->interp_map_1D[data_key].x_vec)
    );
    
    //  3. perform interpolation
    double x_0 = this->interp_map_1D[data_key].x_vec[idx];
    double x_1 = this->interp_map_1D[data_key].x_vec[idx + 1];
    
    double y_0 = this->interp_map_1D[data_key].y_vec[idx];
    double y_1 = this->interp_map_1D[data_key].y_vec[idx + 1];
    
    double interp_y = ((y_1 - y_0) / (x_1 - x_0)) * (interp_x - x_0) + y_0;
    
    return interp_y;
}   /* interp1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Interpolator :: interp2D(int data_key, double interp_x, double interp_y)
///
/// \brief Method to perform a 2D interpolation.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param interp_x The first query value to be interpolated. If this value is outside
///     the domain of the associated interpolation data, then an error will occur.
///
/// \param interp_y The second query value to be interpolated. If this value is outside
///     the domain of the associated interpolation data, then an error will occur.
///
/// \return An interpolation of the given query values.
///

double Interpolator :: interp2D(int data_key, double interp_x, double interp_y)
{
    //  1. check bounds
    this->__checkBounds2D(data_key, interp_x, interp_y);
    
    //  2. get interpolation indices
    int idx_x = this->__getInterpolationIndex(
        interp_x,
        &(this->interp_map_2D[data_key].x_vec)
    );
    
    int idx_y = this->__getInterpolationIndex(
        interp_y,
        &(this->interp_map_2D[data_key].y_vec)
    );
    
    //  3. perform first horizontal interpolation
    double x_0 = this->interp_map_2D[data_key].x_vec[idx_x];
    double x_1 = this->interp_map_2D[data_key].x_vec[idx_x + 1];
    
    double z_0 = this->interp_map_2D[data_key].z_matrix[idx_y][idx_x];
    double z_1 = this->interp_map_2D[data_key].z_matrix[idx_y][idx_x + 1];
    
    double interp_z_0 = ((z_1 - z_0) / (x_1 - x_0)) * (interp_x - x_0) + z_0;
    
    //  4. perform second horizontal interpolation
    z_0 = this->interp_map_2D[data_key].z_matrix[idx_y + 1][idx_x];
    z_1 = this->interp_map_2D[data_key].z_matrix[idx_y + 1][idx_x + 1];
    
    double interp_z_1 = ((z_1 - z_0) / (x_1 - x_0)) * (interp_x - x_0) + z_0;
    
    //  5. perform vertical interpolation
    double y_0 = this->interp_map_2D[data_key].y_vec[idx_y];
    double y_1 = this->interp_map_2D[data_key].y_vec[idx_y + 1];
    
    double interp_z =
        ((interp_z_1 - interp_z_0) / (y_1 - y_0)) * (interp_y - y_0) + interp_z_0;
    
    return interp_z;
}   /* interp2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: ~Interpolator(void)
///
/// \brief Destructor for the Interpolator class.
///

Interpolator :: ~Interpolator(void)
{
    //...
    
    return;
}   /* ~Interpolator() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
