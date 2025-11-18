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
/// \file PYBIND11_Noncombustion.cpp
///
/// \brief Bindings file for the Noncombustion class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Noncombustion
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<NoncombustionType>(m, "NoncombustionType")
    .value("HYDRO", NoncombustionType::HYDRO)
    .value("N_NONCOMBUSTION_TYPES", NoncombustionType::N_NONCOMBUSTION_TYPES);


pybind11::class_<NoncombustionInputs>(m, "NoncombustionInputs")
    .def_readwrite("production_inputs", &NoncombustionInputs::production_inputs)
    
    .def(pybind11::init());


pybind11::class_<Noncombustion>(m, "Noncombustion")
    .def_readwrite("type", &Noncombustion::type)
    .def_readwrite("resource_key", &Noncombustion::resource_key)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, NoncombustionInputs, std::vector<double>*>())
    .def("handleReplacement", &Noncombustion::handleReplacement)
    .def("computeEconomics", &Noncombustion::computeEconomics)
    .def(
        "requestProductionkW",
        pybind11::overload_cast<int, double, double>(&Noncombustion::requestProductionkW)
    )
    .def(
        "requestProductionkW",
        pybind11::overload_cast<int, double, double, double>(&Noncombustion::requestProductionkW)
    )
    .def(
        "commit",
        pybind11::overload_cast<int, double, double, double>(&Noncombustion::commit)
    )
    .def(
        "commit",
        pybind11::overload_cast<int, double, double, double, double>(&Noncombustion::commit)
    )
    .def("writeResults", &Noncombustion::writeResults);
