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
/// \file Renewable.h
///
/// \brief Header file for the Renewable class.
///


#ifndef RENEWABLE_H
#define RENEWABLE_H


#include "../Production.h"


///
/// \enum RenewableType
///
/// \brief An enumeration of the types of Renewable asset supported by PGMcpp
///

enum RenewableType {
    SOLAR, ///< A solar photovoltaic (PV) array
    TIDAL, ///< A tidal stream turbine (or tidal energy converter, TEC)
    WAVE, ///< A wave energy converter (WEC)
    WIND, ///< A wind turbine
    N_RENEWABLE_TYPES ///< A simple hack to get the number of elements in RenewableType
};


///
/// \struct RenewableInputs
///
/// \brief A structure which bundles the necessary inputs for the Renewable constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates ProductionInputs.
///

struct RenewableInputs {
    ProductionInputs production_inputs; ///< An encapsulated ProductionInputs instance.
};


///
/// \class Renewable
///
/// \brief The root of the Renewable branch of the Production hierarchy. This branch 
///     contains derived classes which model the renewable production of energy
///

class Renewable : public Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(RenewableInputs);
        void __handleStartStop(int, double, double);
        
        virtual void __writeSummary(std::string) {return;}
        virtual void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            std::map<int, std::vector<double>>*,
            std::map<int, std::vector<std::vector<double>>>*,
            int = -1
        ) {return;}
        
        
    public:
        //  1. attributes
        RenewableType type; ///< The type (RenewableType) of the asset.
        
        int resource_key; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
        
        double firmness_factor; ///< A factor [0, 1] which defines how firm the production from this asset is.
        
        
        //  2. methods
        Renewable(void);
        Renewable(int, double, RenewableInputs, std::vector<double>*);
        virtual void handleReplacement(int);
        
        void computeEconomics(std::vector<double>*);
        
        virtual double computeProductionkW(int, double, double) {return 0;}
        virtual double computeProductionkW(int, double, double, double) {return 0;}
        virtual double commit(int, double, double, double);
        
        void writeResults(
            std::string,
            std::vector<double>*,
            std::map<int, std::vector<double>>*,
            std::map<int, std::vector<std::vector<double>>>*,
            int,
            int = -1
        );
        
        virtual ~Renewable(void);
        
};  /* Renewable */


#endif  /* RENEWABLE_H */
