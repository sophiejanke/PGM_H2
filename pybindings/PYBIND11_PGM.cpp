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
/// \file PYBIND11_PGM.cpp
///
/// \brief Bindings file for PGMcpp.
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for PGMcpp. Only public
/// attributes/methods are bound!
///


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../header/Model.h"


PYBIND11_MODULE(PGMcpp, m) {

    #include "snippets/PYBIND11_Controller.cpp"
    #include "snippets/PYBIND11_ElectricalLoad.cpp"
    #include "snippets/PYBIND11_Interpolator.cpp"
    #include "snippets/PYBIND11_Model.cpp"
    #include "snippets/PYBIND11_Resources.cpp"
    #include "snippets/PYBIND11_ThermalModel.cpp" 
    
    #include "snippets/Production/PYBIND11_Production.cpp"
    
    #include "snippets/Production/Noncombustion/PYBIND11_Noncombustion.cpp"
    #include "snippets/Production/Noncombustion/PYBIND11_Hydro.cpp"
    
    #include "snippets/Production/Combustion/PYBIND11_Combustion.cpp"
    #include "snippets/Production/Combustion/PYBIND11_Diesel.cpp"
    
    #include "snippets/Production/Renewable/PYBIND11_Renewable.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Solar.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Tidal.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Wave.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Wind.cpp"
    
    #include "snippets/Storage/PYBIND11_Storage.cpp"
    #include "snippets/Storage/PYBIND11_LiIon.cpp"
    #include "snippets/Storage/PYBIND11_Electrolyzer.cpp"
    #include "snippets/Storage/PYBIND11_FuelCell.cpp"
    #include "snippets/Storage/PYBIND11_H2.cpp"

}   /* PYBIND11_MODULE() */
