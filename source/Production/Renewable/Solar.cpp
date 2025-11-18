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
/// \file Solar.cpp
///
/// \brief Implementation file for the Solar class.
///
/// A derived class of the Renewable branch of Production which models solar
/// production.
///


#include "../../../header/Production/Renewable/Solar.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __checkInputs(SolarInputs solar_inputs)
///
/// \brief Helper method to check inputs to the Solar constructor.
///

void Solar :: __checkInputs(SolarInputs solar_inputs)
{
    //  1. check derating
    if (
        solar_inputs.derating < 0 or
        solar_inputs.derating > 1
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::derating must be in the closed interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check julian day
    if (solar_inputs.julian_day < 0) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::julian_day must be >= 0 days.";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check latitude
    if (
        solar_inputs.latitude_deg < -90 or
        solar_inputs.latitude_deg > 90
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::latitude_deg must be in the closed interval ";
        error_str += "[-90, 90] degrees";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check longitude
    if (
        solar_inputs.longitude_deg < -180 or
        solar_inputs.longitude_deg > 180
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::longitude_deg must be in the closed interval ";
        error_str += "[-180, 180] degrees";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  5. check panel tilt angle
    if (
        solar_inputs.panel_tilt_deg < 0 or
        solar_inputs.panel_tilt_deg > 90
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::panel_tilt_deg must be in the closed interval ";
        error_str += "[0, 90] degrees";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  6. check albedo ground reflectance
    if (
        solar_inputs.albedo_ground_reflectance < 0 or
        solar_inputs.albedo_ground_reflectance > 1
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::albedo_ground_reflectance must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  7. check firmness_factor
    if (
        solar_inputs.firmness_factor < 0 or
        solar_inputs.firmness_factor > 1
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::firmness_factor must be in the closed interval [0, 1]";
        
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
/// \fn double Solar :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic solar PV array capital cost.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the solar PV array [CAD].
///

double Solar :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 1000 * pow(this->capacity_kW, -0.15) + 3000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic solar PV array operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     solar PV array [CAD/kWh].
///

double Solar :: __getGenericOpMaintCost(void)
{
    return 0.01;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getMeanLongitudeDeg(void)
///
/// \brief Method to compute and return the mean longitude [deg], bound to the half-open
///     interval [0, 360). From eqn (4.7) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \return The mean longitude [deg], bound to the half-open interval [0, 360).
///

double Solar :: __getMeanLongitudeDeg(void)
{
    // compute mean longitude
    double mean_longitude_deg = 280.46 + 0.9856474 * this->julian_day;
    
    // bound to the half-open interval [0, 360) deg
    int mean_longitude_deg_int = int(mean_longitude_deg);
    double mean_longitude_deg_frac = mean_longitude_deg - mean_longitude_deg_int;
    
    mean_longitude_deg = mean_longitude_deg_int % 360;
    mean_longitude_deg += mean_longitude_deg_frac;
    
    return mean_longitude_deg;
}   /* __getMeanLongitudeDeg() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// double Solar :: __getMeanAnomalyRad(void)
///
/// \brief Method to compute and return the mean anomaly [rad], bound to the half-open 
///     interval [0, 2pi). From eqn (4.8) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \return The mean anomaly [rad], bound to the half-open interval [0, 2pi).
///

double Solar :: __getMeanAnomalyRad(void)
{
    // compute mean anomaly
    double mean_anomaly_deg = 357.528 + 0.9856003 * this->julian_day;
    
    // bound to the half-open interval [0, 360) deg.
    int mean_anomaly_deg_int = int(mean_anomaly_deg);
    double mean_anomaly_deg_frac = mean_anomaly_deg - mean_anomaly_deg_int;
    
    mean_anomaly_deg = mean_anomaly_deg_int % 360;
    mean_anomaly_deg += mean_anomaly_deg_frac;
    
    // translate to rads
    double mean_anomaly_rad = (M_PI / 180.0) * mean_anomaly_deg;
    
    return mean_anomaly_rad;
}   /* __getMeanAnomalyRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getEclipticLongitudeRad(
///         double mean_longitude_deg,
///         double mean_anomaly_rad
///     )
///
/// \brief Method to compute and return the ecliptic longitude [rad], bound to the 
///     half-open interval [0, 2pi). From eqn (4.9) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param mean_longitude_deg The mean longitude [deg], bound to the half-open interval 
///     [0, 360) deg.
///
/// \param mean_anomaly_rad The mean anomaly [rad], bound to the half-open interval 
///     [0, 2pi).
///
/// \return The ecliptic longitude [rad], bound to the half-open interval [0, 2pi).
///


double Solar :: __getEclipticLongitudeRad(
    double mean_longitude_deg,
    double mean_anomaly_rad
)
{
    // compute eclioptic longitude 
    double eclong_deg = mean_longitude_deg + 
        1.915 * sin(mean_anomaly_rad) +
        0.02 * sin(2 * mean_anomaly_rad);
    
    // bound to half-open interval [0, 360) deg
    int eclong_deg_int = int(eclong_deg);
    double eclong_deg_frac = eclong_deg - eclong_deg_int;
    
    eclong_deg = eclong_deg_int % 360;
    eclong_deg += eclong_deg_frac;
    
    // translate to rads
    double eclong_rad = (M_PI / 180.0) * eclong_deg;
    
    return eclong_rad;
}   /* __getEclipticLongitudeRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getObliquityOfEclipticRad(void)
///
/// \brief Method to compute and return the obliquity of the ecliptic [rad], bound to 
///     the half-open interval [0, 2pi). From eqn (4.10) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \return The obliquity of the ecliptic [rad], bound to the half-open interval [0, 2pi).
///

double Solar :: __getObliquityOfEclipticRad(void)
{
    // compute obliquity of ecliptic
    double obleq_deg = 23.439 - 0.0000004 * this->julian_day;
    
    // bound to half-open interval [0, 360) deg
    int obleq_deg_int = int(obleq_deg);
    double obleq_deg_frac = obleq_deg - obleq_deg_int;
    
    obleq_deg = obleq_deg_int % 360;
    obleq_deg += obleq_deg_frac;
    
    // translate to rads
    double obleq_rad = (M_PI / 180.0) * obleq_deg;
    
    return obleq_rad;
}   /* __getObliquityOfEclipticRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getGreenwichMeanSiderialTimeHrs(void)
///
/// \brief Method to compute the Greenwich mean siderial time [hrs], bound to the 
///     half-open interval [0, 24) hrs. From eqn (4.13) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \return Greenwich mean siderial time [hrs], bound to the half-open interval 
///     [0, 24) hrs.
///

double Solar :: __getGreenwichMeanSiderialTimeHrs(void)
{
    // compute Greenwich mean siderial time
    double Greenwich_mean_siderial_time_hrs = 6.697375 + 
        0.0657098242 * this->julian_day -
        (this->longitude_deg / 15);
    
    // bound to the half-open interval [0, 24) hrs
    int Greenwich_mean_siderial_time_hrs_int = int(Greenwich_mean_siderial_time_hrs);
    double Greenwich_mean_siderial_time_hrs_frac = Greenwich_mean_siderial_time_hrs -
        Greenwich_mean_siderial_time_hrs_int;
    
    Greenwich_mean_siderial_time_hrs = Greenwich_mean_siderial_time_hrs_int % 24;
    Greenwich_mean_siderial_time_hrs += Greenwich_mean_siderial_time_hrs_frac;
    
    return Greenwich_mean_siderial_time_hrs;
}   /* __getGreenwichMeanSiderialTimeHrs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getLocalMeanSiderialTimeHrs(
///         double Greenwich_mean_siderial_time_hrs
///     )
///
/// \brief Method to compute and return the local mean siderial time [hrs], bound to 
///     the half-open interval [0, 24) hrs. From eqn (4.14) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param Greenwich_mean_siderial_time_hrs The Greenwich mean siderial time [hrs], 
///     bound to the half-open interval [0, 24) hrs.
///
/// \return The local mean siderial time [hrs], bound to the half-open interval
///     [0, 24) hrs.
///

double Solar :: __getLocalMeanSiderialTimeHrs(
    double Greenwich_mean_siderial_time_hrs
)
{
    // compute local mean siderial time
    double local_mean_siderial_time_hrs = Greenwich_mean_siderial_time_hrs +
        (this->longitude_deg / 15);
    
    // bound to the half-open interval [0, 24) hrs
    int local_mean_siderial_time_hrs_int = int(local_mean_siderial_time_hrs);
    double local_mean_siderial_time_hrs_frac = local_mean_siderial_time_hrs -
        local_mean_siderial_time_hrs_int;
    
    local_mean_siderial_time_hrs = local_mean_siderial_time_hrs_int % 24;
    local_mean_siderial_time_hrs += local_mean_siderial_time_hrs_frac;
    
    return local_mean_siderial_time_hrs;
}   /* __getLocalMeanSiderialTimeHrs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getDeclinationRad(
///         double eclong_rad,
///         double obleq_rad
///     )
///
/// \brief Method to compute and return the declination of the sun [rad], bound to the 
///     closed interval [-pi/2, pi/2]. From eqn (4.12) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param eclong_rad The ecliptic longitude [rad], bound to the half-open interval 
///     [0, 2pi).
///
/// \param obleq_rad The obliquity of the ecliptic, bound to the half-open interval 
///     [0, 2pi).
///
/// \return The declination of the sun [rad], bound to the closed interval [-pi/2, pi/2].
///

double Solar :: __getDeclinationRad(
    double eclong_rad,
    double obleq_rad
)
{
    double declination_rad = asin(sin(obleq_rad) * sin(eclong_rad));
    
    return declination_rad;
}   /* __getDeclinationRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getRightAscensionRad(
///         double eclong_rad,
///         double obleq_rad
///     )
///
/// \brief Method to compute and return the right ascension of the sun [rad], bound to
///     the half-open interval [0, 2pi). From eqn (4.11) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param eclong_rad The ecliptic longitude [rad], bound to the half-open interval 
///     [0, 2pi).
///
/// \param obleq_rad The obliquity of the ecliptic, bound to the half-open interval 
///     [0, 2pi).
///
/// \return The right ascension of the sun [rad], bound to the half-open interval 
///     [0, 2pi).
///

double Solar :: __getRightAscensionRad(
    double eclong_rad,
    double obleq_rad
)
{
    // compute right ascension
    double right_ascension_rad = atan(
        (cos(obleq_rad) * sin(eclong_rad)) / cos(eclong_rad)
    );
    
    // bound to half-open interval [0, 2pi)
    if (cos(eclong_rad) < 0) {
        right_ascension_rad += M_PI;
    }
    
    else if (cos(obleq_rad) * sin(eclong_rad) < 0) {
        right_ascension_rad += 2 * M_PI;
    }
    
    return right_ascension_rad;
}   /* __getRightAscensionRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getHourAngleRad(
///         double local_mean_siderial_time_hrs,
///         double right_ascension_rad
///     )
///
/// \brief Method to compute and return the hour angle [rad] of the sun, bound to the 
///     open interval (-pi, pi). From eqn (4.15) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param local_mean_siderial_time_hrs The local mean siderial time [hrs], bound to the 
///     half-open interval [0, 24) hrs.
///
/// \param right_ascension_rad The right ascension of the sun [rad], bound to the 
///     half-open interval [0, 2pi).
///
/// \return The hour angle [rad] of the sun, bound to the open interval (-pi, pi).
///

double Solar :: __getHourAngleRad(
    double local_mean_siderial_time_hrs,
    double right_ascension_rad
)
{
    // compute hour angle
    double b_rad = 15 * (M_PI / 180.0) * local_mean_siderial_time_hrs -
        right_ascension_rad;
    
    double hour_angle_rad = b_rad;
    
    // bound to open interval (-pi, pi)
    if (b_rad < -1 * M_PI) {
        hour_angle_rad += 2 * M_PI;
    }
    
    else if (b_rad > M_PI) {
        hour_angle_rad -= 2 * M_PI;
    }
    
    return hour_angle_rad;
}   /* __getHourAngleRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getSolarAltitudeRad(
///         double declination_rad,
///         double hour_angle_rad
///     )
///
/// \brief Method to compute and return the sun altitude [rad], corrected for refraction 
///     and bound to the closed interval [0, pi/2]. From eqns (4.16) and (4.17) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param declination_rad The declination of the sun [rad], bound to the closed interval 
///     [-pi/2, pi/2].
///
/// \param hour_angle_rad The hour angle of the sun [rad], bound to the open interval 
///     (-pi, pi).
///
/// \return The sun altitude [rad], corrected for refraction and bound to the closed 
///     interval [0, pi/2].
///

double Solar :: __getSolarAltitudeRad(
    double declination_rad,
    double hour_angle_rad
)
{
    // compute un-corrected altitude
    double a = sin(declination_rad) * sin(this->latitude_rad) +
        cos(declination_rad) * cos(this->latitude_rad) * cos(hour_angle_rad);
    
    double altitude_rad = 0;
    
    if (a < -1) {
        altitude_rad = -1 * M_PI_2;
    }
    
    else if (a > 1) {
        altitude_rad = M_PI_2;
    }
    
    else {
        altitude_rad = asin(a);
    }
    
    // correct for refraction
    double altitude_deg = (180.0 / M_PI) * altitude_rad;
    
    double refraction = 0.56;
    
    if (altitude_deg > -0.56) {
        refraction = 3.51567 *
            (0.1594 + 0.0196 * altitude_deg + 0.00002 * pow(altitude_deg, 2)) *
            pow(1 + 0.505 * altitude_deg + 0.0845 * pow(altitude_deg, 2), -1);
    }
    
    double altitude_corrected_rad = 0;
    
    if (altitude_deg + refraction > 90) {
        altitude_corrected_rad = M_PI_2;
    }
    
    else {
        altitude_corrected_rad = (M_PI / 180.0) * (altitude_deg + refraction);
    }
    
    return altitude_corrected_rad;
}   /* __getSolarAltitudeRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getSolarAzimuthRad(
///         double declination_rad,
///         double hour_angle_rad
///     )
///
/// \brief Method to copmute and return the solar azimuth [rad], bound to the closed 
///     interval [-pi, pi]. From eqns (4.16) and (4.18) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param declination_rad The declination of the sun [rad], bound to the closed interval 
///     [-pi/2, pi/2].
///
/// \param hour_angle_rad The hour angle of the sun [rad], bound to the open interval 
///     (-pi, pi).
///
/// \return The solar azimuth [rad], bound to the closed interval [-pi, pi].
///

double Solar :: __getSolarAzimuthRad(
    double declination_rad,
    double hour_angle_rad
)
{
    // compute un-corrected altitude
    double a = sin(declination_rad) * sin(this->latitude_rad) +
        cos(declination_rad) * cos(this->latitude_rad) * cos(hour_angle_rad);
    
    double altitude_rad = 0;
    
    if (a < -1) {
        altitude_rad = -1 * M_PI_2;
    }
    
    else if (a > 1) {
        altitude_rad = M_PI_2;
    }
    
    else {
        altitude_rad = asin(a);
    }
    
    // compute a term
    a = (sin(altitude_rad) * sin(this->latitude_rad) - sin(declination_rad)) /
        (cos(altitude_rad) * cos(this->latitude_rad));
    
    // compute b term
    double b_rad = 0;
    
    if (cos(altitude_rad) == 0 or a < -1) {
        b_rad = M_PI;
    }
    
    else if (a > 1) {
        b_rad = 0;
    }
    
    else {
        b_rad = acos(a);
    }
    
    // compute azimuth
    double azimuth_rad = 0;
    
    if (hour_angle_rad < -1 * M_PI) {
        azimuth_rad = b_rad;
    }
    
    else if (
        (hour_angle_rad >= -1 * M_PI and hour_angle_rad <= 0) or
        hour_angle_rad > M_PI
    ) {
        azimuth_rad = M_PI - b_rad;
    }
    
    else {
        azimuth_rad = M_PI + b_rad;
    }
    
    return azimuth_rad;
}   /* __getSolarAzimuth() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getSolarZenithRad(
///         double declination_rad,
///         double hour_angle_rad
///     )
///
/// \brief Method to compute and return the solar zenith [rad], bound to the open 
///     interval (-pi/2, pi/2). From eqn (4.19) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param declination_rad The declination of the sun [rad], bound to the closed interval 
///     [-pi/2, pi/2].
///
/// \param hour_angle_rad The hour angle of the sun [rad], bound to the open interval 
///     (-pi, pi).
///
/// \return The solar zenith [rad], bound to the open interval (-pi/2, pi/2).
///

double Solar :: __getSolarZenithRad(
    double declination_rad,
    double hour_angle_rad
)
{
    double solar_zenith_rad = M_PI_2 - this->__getSolarAltitudeRad(
        declination_rad,
        hour_angle_rad
    );
    
    return solar_zenith_rad;
}   /* __getSolarZenith() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getDiffuseHorizontalIrradiancekWm2(double solar_resource_kWm2)
///
/// \brief Method which takes in the solar resource at a particular point in time, and 
///     then returns the diffuse horizontal irradiance (DHI) [kW/m2] using a very simple, 
///     empirical model (simply DHI is proportional to GHI).
///
/// Ref: \cite Safaripour_2011\n
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The diffuse horizontal irradiance [kW/m2].
///

double Solar :: __getDiffuseHorizontalIrradiancekWm2(double solar_resource_kWm2)
{
    double GHI_2_DHI = 0.32;
    
    return GHI_2_DHI * solar_resource_kWm2;
}   /* __getDiffuseHorizontalIrradiancekWm2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getDirectNormalIrradiancekWm2(
///         double solar_resource_kWm2,
///         double diffuse_horizontal_irradiance_kWm2,
///         double solar_zenith_rad
///     )
///
/// \brief Method which takes in the solar resource and DHI at a particular point in 
///     time, then the returns the direct normal irradiance (DNI) [kW/m2]. From 
///     definition of global horizontal irradiance (GHI).
///
/// Ref: \cite Gilman_2018\n
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \param diffuse_horizontal_irradiance_kWm2 The DHI [kW/m2].
///
/// \param solar_zenith_rad The solar zenith [rad].
///
/// \return The direct normal irradiance (DNI) [kW/m2].
///

double Solar :: __getDirectNormalIrradiancekWm2(
    double solar_resource_kWm2,
    double diffuse_horizontal_irradiance_kWm2,
    double solar_zenith_rad
)
{
    double direct_normal_irradiance_kWm2 =
        (solar_resource_kWm2 - diffuse_horizontal_irradiance_kWm2) /
        cos(solar_zenith_rad);
    
    return direct_normal_irradiance_kWm2;
}   /* __getDirectNormalIrradiancekWm2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getAngleOfIncidenceRad(
///         double solar_zenith_rad,
///         double solar_azimuth_rad
///     )
///
/// \brief Method to compute and return the angle of incidence [rad] between the solar 
///     beam and the panel normal. From eqn (5.1) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param solar_zenith_rad The solar zenith [rad].
///
/// \param solar_azimuth_rad The solar azimuth [rad].
///
/// \return The angle of incidence [rad] between the solar beam and the panel normal.
///

double Solar :: __getAngleOfIncidenceRad(
    double solar_zenith_rad,
    double solar_azimuth_rad
)
{
    double a =
        sin(solar_zenith_rad) *
        cos(solar_azimuth_rad - this->panel_azimuth_rad) *
        sin(this->panel_tilt_rad) +
        cos(solar_zenith_rad) *
        cos(this->panel_tilt_rad);
    
    double angle_of_incidence_rad = 0;
    
    if (a < -1) {
        angle_of_incidence_rad = M_PI;
    }
    
    else if (a > 1) {
        angle_of_incidence_rad = 0;
    }
    
    else {
        angle_of_incidence_rad = acos(a);
    }
    
    return angle_of_incidence_rad;
}   /* __getAngleOfIncidenceRad() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getBeamIrradiancekWm2(
///         double direct_normal_irradiance_kWm2,
///         double angle_of_incidence_rad
///     )
///
/// \brief Method which computes and returns the beam irradiance normal to the
///     panels [kW/m2]. From eqn (6.1) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param direct_normal_irradiance_kWm2 The DNI [kW/m2].
///
/// \param angle_of_incidence_rad The angle of incidence [rad] between the solar beam
///     and the panel normal.
///
/// \return The beam irradiance normal to the panels [kW/m2].
///

double Solar :: __getBeamIrradiancekWm2(
    double direct_normal_irradiance_kWm2,
    double angle_of_incidence_rad
)
{
    double beam_irradiance_kWm2 = direct_normal_irradiance_kWm2 *
        cos(angle_of_incidence_rad);
    
    return beam_irradiance_kWm2;
}   /* __getBeamIrradiancekWm2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getDiffuseIrradiancekWm2(double diffuse_horizontal_irradiance_kWm2)
///
/// \brief Method which computes and returns the (isotropic) diffuse sky
///     irradiance [kW/m2]. From eqn (6.5) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param diffuse_horizontal_irradiance_kWm2 The DHI [kW/m2]
///
/// \return The (isotropic) diffuse sky irradiance [kW/m2]
///

double Solar :: __getDiffuseIrradiancekWm2(double diffuse_horizontal_irradiance_kWm2)
{
    double diffuse_sky_irradiance_kWm2 = diffuse_horizontal_irradiance_kWm2 *
        cos(this->panel_tilt_rad);
    
    return diffuse_sky_irradiance_kWm2;
}   /* __getDiffuseIrradiancekWm2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //'

///
/// \fn double Solar :: __getGroundReflectedIrradiancekWm2(double solar_resource_kWm2)
///
/// \brief Method to compute and return the ground reflected irradiance [kW/m2]. From
///     eqn (6.21) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The ground reflected irradiance [kW/m2].
///

double Solar :: __getGroundReflectedIrradiancekWm2(double solar_resource_kWm2)
{
    double ground_reflected_irradiance_kWm2 =
        this->albedo_ground_reflectance * solar_resource_kWm2 *
        ((1 - cos(this->panel_tilt_rad)) / 2);
    
    return ground_reflected_irradiance_kWm2;
}   /* __getGroundReflectedIrradiancekWm2() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getPlaneOfArrayIrradiancekWm2(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the nominal plane of array irradiance. From eqn (7.1) of Gilman.
///
/// Ref: \cite Gilman_2018\n
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The nominal plane of array irradiance [kW/m2].
///

double Solar :: __getPlaneOfArrayIrradiancekWm2(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    // get mean longitude and mean anomaly
    double mean_longitude_deg = this->__getMeanLongitudeDeg();
    double mean_anomaly_rad = this->__getMeanAnomalyRad();
    
    
    // get ecliptic longitude and obliquity of the ecliptic
    double eclong_rad = this->__getEclipticLongitudeRad(
        mean_longitude_deg,
        mean_anomaly_rad
    );
    
    double obleq_rad = this->__getObliquityOfEclipticRad();
    
    
    // get local mean siderial time
    double Greenwich_mean_siderial_time_hrs = this->__getGreenwichMeanSiderialTimeHrs();
    
    double local_mean_siderial_time_hrs = this->__getLocalMeanSiderialTimeHrs(
        Greenwich_mean_siderial_time_hrs
    );
    
    
    // get right ascension, declination, and hour angle
    double right_ascension_rad = this->__getRightAscensionRad(eclong_rad, obleq_rad);
    double declination_rad = this->__getDeclinationRad(eclong_rad, obleq_rad);
    
    double hour_angle_rad = this->__getHourAngleRad(
        local_mean_siderial_time_hrs,
        right_ascension_rad
    );
    
    
    // get solar azimuth and zenith
    double solar_azimuth_rad = this->__getSolarAzimuthRad(
        declination_rad,
        hour_angle_rad
    );
    
    double solar_zenith_rad = this->__getSolarZenithRad(
        declination_rad,
        hour_angle_rad
    );
    
    
    // get diffuse horizontal irradiance (DHI) and direct normal irradiance (DNI)
    double diffuse_horizontal_irradiance_kWm2 = this->__getDiffuseHorizontalIrradiancekWm2(
        solar_resource_kWm2
    );
    
    double direct_normal_irradiance_kWm2 = this->__getDirectNormalIrradiancekWm2(
        solar_resource_kWm2,
        diffuse_horizontal_irradiance_kWm2,
        solar_zenith_rad
    );
    
    
    // get angle of incidence
    double angle_of_incidence_rad = this->__getAngleOfIncidenceRad(
        solar_zenith_rad,
        solar_azimuth_rad
    );
    
    
    // compute plane of array irradiance as superposition of beam, diffuse, and ground
    // reflected.
    double plane_of_array_irradiance_kWm2 = 0;
    
    plane_of_array_irradiance_kWm2 += this->__getBeamIrradiancekWm2(
        direct_normal_irradiance_kWm2,
        angle_of_incidence_rad
    );
    
    plane_of_array_irradiance_kWm2 += this->__getDiffuseIrradiancekWm2(
        diffuse_horizontal_irradiance_kWm2
    );
    
    plane_of_array_irradiance_kWm2 += this->__getGroundReflectedIrradiancekWm2(
        solar_resource_kWm2
    );
    
    return plane_of_array_irradiance_kWm2;
}   /* __getPlaneOfArrayIrradiance() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __computeSimpleProductionkW(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the solar PV production at that point in time base on a simple,
///     "HOMER-like" model.
///
/// Ref: \cite HOMER_how_homer_calculates_the_pv_array_power_output\n
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The production [kW] of the solar PV array.
///

double Solar :: __computeSimpleProductionkW(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    // apply simple production model (GHI -> production)
    double production_kW = this->derating * solar_resource_kWm2 * this->capacity_kW;
    
    // cap production at capacity
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    return production_kW;
}   /* __computeSimpleProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __computeDetailedProductionkW(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the solar PV production at that point in time base on a detailed,
///     "PVWatts/SAM-like" model.
///
/// Ref: \cite Gilman_2018\n
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The production [kW] of the solar PV array.
///

double Solar :: __computeDetailedProductionkW(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    // apply detailed production model (POA irradiance -> production)
    double plane_of_array_irradiance_kWm2 = this->__getPlaneOfArrayIrradiancekWm2(
        timestep,
        dt_hrs,
        solar_resource_kWm2
    );
    
    double production_kW =
        this->derating * plane_of_array_irradiance_kWm2 * this->capacity_kW;
    
    // cap production at capacity
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    return production_kW;
}   /* __computeDetailedProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Solar.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Solar :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW SOLAR Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Production attributes
    ofs << "## Production Attributes\n";
    ofs << "\n";
    
    ofs << "Capacity: " << this->capacity_kW << " kW  \n";
    ofs << "\n";
    
    ofs << "Production Override: (N = 0 / Y = 1): "
        << this->normalized_production_series_given << "  \n";
    if (this->normalized_production_series_given) {
        ofs << "Path to Normalized Production Time Series: "
            << this->path_2_normalized_production_time_series << "  \n";
    }
    ofs << "\n";
    
    ofs << "Sunk Cost (N = 0 / Y = 1): " << this->is_sunk << "  \n";
    ofs << "Capital Cost: " << this->capital_cost << "  \n";
    ofs << "Operation and Maintenance Cost: " << this->operation_maintenance_cost_kWh
        << " per kWh produced  \n";
    ofs << "Nominal Inflation Rate (annual): " << this->nominal_inflation_annual
        << "  \n";
    ofs << "Nominal Discount Rate (annual): " << this->nominal_discount_annual
        << "  \n";
    ofs << "Real Discount Rate (annual): " << this->real_discount_annual << "  \n";
    ofs << "\n";
    
    ofs << "Replacement Running Hours: " << this->replace_running_hrs << "  \n";
    ofs << "\n--------\n\n";
    
    //  2.2. Renewable attributes
    ofs << "## Renewable Attributes\n";
    ofs << "\n";
    
    ofs << "Resource Key (1D): " << this->resource_key << "  \n";
    ofs << "Firmness Factor: " << this->firmness_factor << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. Solar attributes
    ofs << "## Solar Attributes\n";
    ofs << "\n";
    
    ofs << "Derating Factor: " << this->derating << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.4. Solar Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Dispatch: " << this->total_dispatch_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched  \n";
    ofs << "\n";

    ofs << "Total Curtailment: " << this->curtailment_total_kWh
        << " kWh  \n";
    ofs << "\n";
    
    ofs << "Running Hours: " << this->running_hours << "  \n";
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    ofs << "\n--------\n\n";
    
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         std::map<int, std::vector<double>>* resource_map_1D_ptr,
///         std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Solar.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param resource_map_1D_ptr A pointer to the 1D map of Resources.
///
/// \param resource_map_2D_ptr A pointer to the 2D map of Resources.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Solar :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    std::map<int, std::vector<double>>* resource_map_1D_ptr,
    std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
    int max_lines
)
{
    //  1. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Solar Resource [kW/m2],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        
        if (not this->normalized_production_series_given) {
            ofs << resource_map_1D_ptr->at(this->resource_key)[i] << ",";
        }
        
        else {
            ofs << "OVERRIDE" << ",";
        }
        
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
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
/// \fn Solar :: Solar(void)
///
/// \brief Constructor (dummy) for the Solar class.
///

Solar :: Solar(void)
{
    //...
    
    return;
}   /* Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: Solar(
///         int n_points,
///         double n_years,
///         SolarInputs solar_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Solar class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param solar_inputs A structure of Solar constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Solar :: Solar(
    int n_points,
    double n_years,
    SolarInputs solar_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Renewable(
    n_points,
    n_years,
    solar_inputs.renewable_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(solar_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: SOLAR;
    this->type_str = "SOLAR";
    
    this->resource_key = solar_inputs.resource_key;
    
    this->firmness_factor = solar_inputs.firmness_factor;
    
    this->derating = solar_inputs.derating;
    
    this->julian_day = solar_inputs.julian_day;
    
    this->latitude_deg = solar_inputs.latitude_deg;
    this->longitude_deg = solar_inputs.longitude_deg;
    
    this->latitude_rad = (M_PI / 180.0) * this->latitude_deg;
    this->longitude_rad = (M_PI / 180.0) * this->longitude_deg;
    
    this->panel_azimuth_deg = solar_inputs.panel_azimuth_deg;
    this->panel_tilt_deg = solar_inputs.panel_tilt_deg;
    
    this->panel_azimuth_rad = (M_PI / 180.0) * this->panel_azimuth_deg;
    this->panel_tilt_rad = (M_PI / 180.0) * this->panel_tilt_deg;
    
    this->albedo_ground_reflectance = solar_inputs.albedo_ground_reflectance;
    
    this->power_model = solar_inputs.power_model;
    
    switch (this->power_model) {
        case (SolarPowerProductionModel :: SOLAR_POWER_SIMPLE): {
            this->power_model_string = "SIMPLE";
            
            break;
        }
        
        case (SolarPowerProductionModel :: SOLAR_POWER_DETAILED): {
            this->power_model_string = "DETAILED";
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Solar():  ";
            error_str += "power production model ";
            error_str += std::to_string(this->power_model);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    if (solar_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = solar_inputs.capital_cost;
    }
    
    if (solar_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            solar_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Solar object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Solar :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Renewable :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the solar PV production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. global horizontal irradiance) [kW/m2].
///
/// \return The production [kW] of the solar PV array.
///

double Solar :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    //  given production time series override
    if (this->normalized_production_series_given) {
        double production_kW = Production :: getProductionkW(timestep);
        
        return production_kW;
    }
    
    // check if no resource
    if (solar_resource_kWm2 <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (SolarPowerProductionModel :: SOLAR_POWER_SIMPLE): {
            production_kW = this->__computeSimpleProductionkW(
                timestep,
                dt_hrs,
                solar_resource_kWm2
            );
            
            break;
        }
        
        case (SolarPowerProductionModel :: SOLAR_POWER_DETAILED): {
            production_kW = this->__computeDetailedProductionkW(
                timestep,
                dt_hrs,
                solar_resource_kWm2
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Solar::computeProductionkW():  ";
            error_str += "power model ";
            error_str += std::to_string(this->power_model);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: commit(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in production and load for the current timestep, computes
///     and records dispatch and curtailment, and then returns remaining load.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the dispatch is deducted from it.
///

double Solar :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Renewable :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    
    //  2. increment julian day
    this->julian_day += dt_hrs / 24;
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: ~Solar(void)
///
/// \brief Destructor for the Solar class.
///

Solar :: ~Solar(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Solar object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Solar() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
