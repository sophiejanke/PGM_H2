/*
 * PGMcpp : PRIMED Grid Modelling (in C++)
 * Copyrighad 2023 (C)
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
/// \file PYBIND11_Model.cpp
///
/// \brief Bindings file for the Model class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Model
/// class. Only public attributes/methods are bound!
///


pybind11::class_<ModelInputs>(m, "ModelInputs")
    .def_readwrite(
        "path_2_electrical_load_time_series",
        &ModelInputs::path_2_electrical_load_time_series
    )
    .def_readwrite("control_mode", &ModelInputs::control_mode)
    .def_readwrite("firm_dispatch_ratio", &ModelInputs::firm_dispatch_ratio)
    .def_readwrite("load_reserve_ratio", &ModelInputs::load_reserve_ratio)
    
    .def(pybind11::init());


pybind11::class_<Model>(m, "Model")
    .def_readwrite("total_fuel_consumed_L", &Model::total_fuel_consumed_L)
    .def_readwrite("total_emissions", &Model::total_emissions)
    .def_readwrite("net_present_cost", &Model::net_present_cost)
    .def_readwrite(
        "total_renewable_noncombustion_dispatch_kWh",
        &Model::total_renewable_noncombustion_dispatch_kWh
    )
    .def_readwrite(
        "total_renewable_noncombustion_charge_kWh",
        &Model::total_renewable_noncombustion_charge_kWh
    )
    .def_readwrite("total_combustion_charge_kWh", &Model::total_combustion_charge_kWh)
    .def_readwrite("total_discharge_kWh", &Model::total_discharge_kWh)
    .def_readwrite("total_dispatch_kWh", &Model::total_dispatch_kWh)
    .def_readwrite("renewable_penetration", &Model::renewable_penetration)
    .def_readwrite(
        "levellized_cost_of_energy_kWh",
        &Model::levellized_cost_of_energy_kWh
    )
    .def_readwrite("controller", &Model::controller)
    .def_readwrite("electrical_load", &Model::electrical_load)
    .def_readwrite("resources", &Model::resources)
    .def_readwrite("thermal_model_ptr", &Model::thermal_model_ptr)

    .def_readwrite("combustion_ptr_vec", &Model::combustion_ptr_vec)
    .def_readwrite("noncombustion_ptr_vec", &Model::noncombustion_ptr_vec)
    .def_readwrite("renewable_ptr_vec", &Model::renewable_ptr_vec)
    .def_readwrite("storage_ptr_vec", &Model::storage_ptr_vec)
    
    .def(pybind11::init<>())
    .def(pybind11::init<ModelInputs>())
    .def("addDiesel", &Model::addDiesel)
    .def(
        "addResource",
        pybind11::overload_cast<NoncombustionType, std::string, int>(&Model::addResource)
    )
    .def(
        "addResource",
        pybind11::overload_cast<RenewableType, std::string, int>(&Model::addResource)
    )
    .def("addHydro", &Model::addHydro)
    .def("addSolar", &Model::addSolar)
    .def("addTidal", &Model::addTidal)
    .def("addWave", &Model::addWave)
    .def("addWind", &Model::addWind)
    .def("addLiIon", &Model::addLiIon)
    .def("addH2", &Model::addH2)
    .def("addThermalModel", &Model::addThermalModel)

    .def("run", &Model::run)
    .def("reset", &Model::reset)
    .def("clear", &Model::clear)
    .def(
        "writeResults",
        &Model::writeResults,
        pybind11::arg("write_path"),
        pybind11::arg("max_lines") = -1
    );
