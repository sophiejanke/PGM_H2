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
/// \file Solar.h
///
/// \brief Header file for the Solar class.
///


#ifndef SOLAR_H
#define SOLAR_H


#include "Renewable.h"


///
/// \enum SolarProductionModel
///
/// \brief An enumeration of the various solar power production models supported by
///     PGMcpp.
///

enum SolarPowerProductionModel {
    SOLAR_POWER_SIMPLE, ///< A simple "HOMER-like" power production model
    SOLAR_POWER_DETAILED, ///< A more detailed "PVWatts/SAM-like" production model
    N_SOLAR_POWER_PRODUCTION_MODELS ///< A simple hack to get the number of elements in SolarPowerProductionModel
};


///
/// \struct SolarInputs
///
/// \brief A structure which bundles the necessary inputs for the Solar constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct SolarInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double firmness_factor = 0.2; ///< A factor [0, 1] which defines how firm the production from this asset is.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double derating = 0.8; ///< The derating of the solar PV array (i.e., shadowing, soiling, etc.).
    
    double julian_day = 0; ///< The number of days (including partial days) since 12:00 on 1 Jan 2000.
    
    double latitude_deg = 0; ///< The latitude of the solar PV array [deg].
    double longitude_deg = 0; ///< The longitude of the solar PV array [deg].
    
    double panel_azimuth_deg = 0; ///< The azimuth angle of the panels [deg], relative to north.
    double panel_tilt_deg = 0; ///< The tilt angle of the panels [deg], relative to ground.
    
    double albedo_ground_reflectance = 0.5; ///< The albedo (ground reflectance) to be applied in modelling the solar PV array.
    
    SolarPowerProductionModel power_model = SolarPowerProductionModel :: SOLAR_POWER_SIMPLE; ///< The solar power production model to be applied.
};


///
/// \class Solar
///
/// \brief A derived class of the Renewable branch of Production which models solar
///     production.
///

class Solar : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(SolarInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        double __getMeanLongitudeDeg(void);
        double __getMeanAnomalyRad(void);
        
        double __getEclipticLongitudeRad(double, double);
        double __getObliquityOfEclipticRad(void);
        
        double __getGreenwichMeanSiderialTimeHrs(void);
        double __getLocalMeanSiderialTimeHrs(double);
        
        double __getRightAscensionRad(double, double);
        double __getDeclinationRad(double, double);
        double __getHourAngleRad(double, double);
        
        double __getSolarAltitudeRad(double, double);
        double __getSolarAzimuthRad(double, double);
        double __getSolarZenithRad(double, double);
        
        double __getDiffuseHorizontalIrradiancekWm2(double);
        double __getDirectNormalIrradiancekWm2(double, double, double);
        
        double __getAngleOfIncidenceRad(double, double);
        
        double __getBeamIrradiancekWm2(double, double);
        double __getDiffuseIrradiancekWm2(double);
        double __getGroundReflectedIrradiancekWm2(double);
        
        double __getPlaneOfArrayIrradiancekWm2(int, double, double);
        
        double __computeSimpleProductionkW(int, double, double);
        double __computeDetailedProductionkW(int, double, double);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            std::map<int, std::vector<double>>*,
            std::map<int, std::vector<std::vector<double>>>*,
            int = -1
        );
        
        
    public:
        //  1. attributes
        double derating; ///< The derating of the solar PV array (i.e., shadowing, soiling, etc.).
        
        double julian_day; ///< The number of days (including partial days) since 12:00 on 1 Jan 2000.
        
        double latitude_deg; ///< The latitude of the solar PV array [deg].
        double longitude_deg; ///< The longitude of the solar PV array [deg].
        
        double latitude_rad; ///< The latitude of the solar PV array [rad].
        double longitude_rad; ///< The longitude of the solar PV array [rad].
        
        double panel_azimuth_deg; ///< The azimuth angle of the panels [deg], relative to north.
        double panel_tilt_deg; ///< The tilt angle of the panels [deg], relative to ground.
        
        double panel_azimuth_rad; ///< The azimuth angle of the panels [rad], relative to north.
        double panel_tilt_rad; ///< The tilt angle of the panels [rad], relative to ground.
        
        double albedo_ground_reflectance; ///< The albedo (ground reflectance) to be applied in modelling the solar PV array.
        
        SolarPowerProductionModel power_model; ///< The solar power production model to be applied.
        std::string power_model_string; ///< A string describing the active power production model.
        
        //  2. methods
        Solar(void);
        Solar(int, double, SolarInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double computeProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Solar(void);
        
};  /* Solar */


#endif  /* SOLAR_H */
