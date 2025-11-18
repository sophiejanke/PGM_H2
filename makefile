#
#   PGMcpp : PRIMED Grid Modelling (in C++)
#
#   Anthony Truelove MASc, P.Eng.
#   email:  gears1763@tutanota.com
#   github: gears1763-2
#
#   See license terms
#
#   Makefile for PGMcpp: PRIMED Grid Modelling (in C++)
#


## ======== USER INPUT ============================================================== ##

PROJECT_NAME = example

SRC_PROJECT = projects/$(PROJECT_NAME).cpp
OUT_PROJECT = bin/$(PROJECT_NAME).out

## ======== END USER INPUT ========================================================== ##



## ======== COMPILER FLAGS ========================================================== ##

CXX = g++ -O3 -std=c++17

#CXXFLAGS =  -Wall -g -p -fPIC
CXXFLAGS = -Wall -fPIC

LIBS = -lpthread

## ======== END COMPILER FLAGS ====================================================== ##



## ======== BUILD =================================================================== ##

#### ==== Testing Utilities ==== ####

SRC_TESTING_UTILS = test/utils/testing_utils.cpp
OBJ_TESTING_UTILS = test/object/testing_utils.o

.PHONY: testing_utils
testing_utils: $(SRC_TESTING_UTILS)
	$(CXX) $(CXXFLAGS) -c $(SRC_TESTING_UTILS) -o $(OBJ_TESTING_UTILS)


#### ==== Interpolator ==== ####

SRC_INTERPOLATOR = source/Interpolator.cpp
OBJ_INTERPOLATOR = object/Interpolator.o

.PHONY: interpolator
interpolator: $(SRC_INTERPOLATOR)
	$(CXX) $(CXXFLAGS) -c $(SRC_INTERPOLATOR) -o $(OBJ_INTERPOLATOR)


#### ==== Production Hierarchy <-- Combustion ==== ####

SRC_PRODUCTION = source/Production/Production.cpp
OBJ_PRODUCTION = object/Production/Production.o

.PHONY: Production
Production: $(SRC_PRODUCTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_PRODUCTION) -o $(OBJ_PRODUCTION)


SRC_COMBUSTION = source/Production/Combustion/Combustion.cpp
OBJ_COMBUSTION = object/Production/Combustion/Combustion.o

.PHONY: Combustion
Combustion: $(SRC_COMBUSTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_COMBUSTION) -o $(OBJ_COMBUSTION)


SRC_DIESEL = source/Production/Combustion/Diesel.cpp
OBJ_DIESEL = object/Production/Combustion/Diesel.o

.PHONY: Diesel
Diesel: $(SRC_DIESEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIESEL) -o $(OBJ_DIESEL)


COMBUSTION_HIERARCHY = Production \
                       Combustion \
                       Diesel

OBJ_COMBUSTION_HIERARCHY = $(OBJ_PRODUCTION) \
                           $(OBJ_COMBUSTION) \
                           $(OBJ_DIESEL)


#### ==== Production Hierarchy <-- Noncombustion ==== ####

SRC_NONCOMBUSTION = source/Production/Noncombustion/Noncombustion.cpp
OBJ_NONCOMBUSTION = object/Production/Noncombustion/Noncombustion.o

.PHONY: Noncombustion
Noncombustion: $(SRC_NONCOMBUSTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_NONCOMBUSTION) -o $(OBJ_NONCOMBUSTION)


SRC_HYDRO = source/Production/Noncombustion/Hydro.cpp
OBJ_HYDRO = object/Production/Noncombustion/Hydro.o

.PHONY: Hydro
Hydro: $(SRC_HYDRO)
	$(CXX) $(CXXFLAGS) -c $(SRC_HYDRO) -o $(OBJ_HYDRO)


NONCOMBUSTION_HIERARCHY = Noncombustion \
                          Hydro

OBJ_NONCOMBUSTION_HIERARCHY = $(OBJ_NONCOMBUSTION) \
                              $(OBJ_HYDRO)


#### ==== Production Hierarchy <-- Renewable ==== ####

SRC_RENEWABLE = source/Production/Renewable/Renewable.cpp
OBJ_RENEWABLE = object/Production/Renewable/Renewable.o

.PHONY: Renewable
Renewable: $(SRC_RENEWABLE)
	$(CXX) $(CXXFLAGS) -c $(SRC_RENEWABLE) -o $(OBJ_RENEWABLE)


SRC_SOLAR = source/Production/Renewable/Solar.cpp
OBJ_SOLAR = object/Production/Renewable/Solar.o

.PHONY: Solar
Solar: $(SRC_SOLAR)
	$(CXX) $(CXXFLAGS) -c $(SRC_SOLAR) -o $(OBJ_SOLAR)


SRC_TIDAL = source/Production/Renewable/Tidal.cpp
OBJ_TIDAL = object/Production/Renewable/Tidal.o

.PHONY: Tidal
Tidal: $(SRC_TIDAL)
	$(CXX) $(CXXFLAGS) -c $(SRC_TIDAL) -o $(OBJ_TIDAL)


SRC_WAVE = source/Production/Renewable/Wave.cpp
OBJ_WAVE = object/Production/Renewable/Wave.o

.PHONY: Wave
Wave: $(SRC_WAVE)
	$(CXX) $(CXXFLAGS) -c $(SRC_WAVE) -o $(OBJ_WAVE)


SRC_WIND = source/Production/Renewable/Wind.cpp
OBJ_WIND = object/Production/Renewable/Wind.o

.PHONY: Wind
Wind: $(SRC_WIND)
	$(CXX) $(CXXFLAGS) -c $(SRC_WIND) -o $(OBJ_WIND)


RENEWABLE_HIERARCHY = Renewable \
                      Solar \
                      Tidal \
                      Wave \
                      Wind

OBJ_RENEWABLE_HIERARCHY = $(OBJ_RENEWABLE) \
                          $(OBJ_SOLAR) \
                          $(OBJ_TIDAL) \
                          $(OBJ_WAVE) \
                          $(OBJ_WIND)


#### ==== Storage Hierarchy ==== ####

SRC_STORAGE = source/Storage/Storage.cpp
OBJ_STORAGE = object/Storage/Storage.o

.PHONY: Storage
Storage: $(SRC_STORAGE)
	$(CXX) $(CXXFLAGS) -c $(SRC_STORAGE) -o $(OBJ_STORAGE)


SRC_LIION = source/Storage/LiIon.cpp
OBJ_LIION = object/Storage/LiIon.o

.PHONY: LiIon
LiIon: $(SRC_LIION)
	$(CXX) $(CXXFLAGS) -c $(SRC_LIION) -o $(OBJ_LIION)


SRC_ELECTROLYZER = source/Storage/Electrolyzer.cpp
OBJ_ELECTROLYZER = object/Storage/Electrolyzer.o

.PHONY: Electrolyzer
Electrolyzer: $(SRC_ELECTROLYZER)
	$(CXX) $(CXXFLAGS) -c $(SRC_ELECTROLYZER) -o $(OBJ_ELECTROLYZER)


SRC_FUELCELL = source/Storage/FuelCell.cpp
OBJ_FUELCELL = object/Storage/FuelCell.o

.PHONY: FuelCell
FuelCell: $(SRC_FUELCELL)
	$(CXX) $(CXXFLAGS) -c $(SRC_FUELCELL) -o $(OBJ_FUELCELL)


SRC_H2 = source/Storage/H2.cpp
OBJ_H2 = object/Storage/H2.o

.PHONY: H2
H2: $(SRC_H2)
	$(CXX) $(CXXFLAGS) -c $(SRC_H2) -o $(OBJ_H2)


STORAGE_HIERARCHY = Storage \
                    LiIon \
                    Electrolyzer \
                    FuelCell \
                    H2

OBJ_STORAGE_HIERARCHY = $(OBJ_STORAGE) \
                        $(OBJ_LIION) \
                        $(OBJ_ELECTROLYZER) \
                        $(OBJ_FUELCELL) \
                        $(OBJ_H2)


#### ==== Model and Components ==== ####

SRC_CONTROLLER = source/Controller.cpp
OBJ_CONTROLLER = object/Controller.o

.PHONY: Controller
Controller: $(SRC_CONTROLLER)
	$(CXX) $(CXXFLAGS) -c $(SRC_CONTROLLER) -o $(OBJ_CONTROLLER)


SRC_ELECTRICALLOAD = source/ElectricalLoad.cpp
OBJ_ELECTRICALLOAD = object/ElectricalLoad.o

.PHONY: ElectricalLoad
ElectricalLoad: $(SRC_ELECTRICALLOAD)
	$(CXX) $(CXXFLAGS) -c $(SRC_ELECTRICALLOAD) -o $(OBJ_ELECTRICALLOAD)


SRC_RESOURCES = source/Resources.cpp
OBJ_RESOURCES = object/Resources.o

.PHONY: Resources
Resources: $(SRC_RESOURCES)
	$(CXX) $(CXXFLAGS) -c $(SRC_RESOURCES) -o $(OBJ_RESOURCES)


SRC_THERMALMODEL = source/ThermalModel.cpp
OBJ_THERMALMODEL = object/ThermalModel.o

.PHONY: ThermalModel
ThermalModel: $(SRC_THERMALMODEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_THERMALMODEL) -o $(OBJ_THERMALMODEL)


SRC_MODEL = source/Model.cpp
OBJ_MODEL = object/Model.o

.PHONY: Model
Model: $(SRC_MODEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_MODEL) -o $(OBJ_MODEL)


MODEL = Controller \
        ElectricalLoad \
        Resources \
		ThermalModel \
        Model

OBJ_MODEL_COMPONENTS = $(OBJ_CONTROLLER) \
                       $(OBJ_ELECTRICALLOAD) \
                       $(OBJ_RESOURCES) \
					   $(OBJ_THERMALMODEL) \
                       $(OBJ_MODEL)


#### ==== Tests ==== ####

OBJ_ALL = $(OBJ_TESTING_UTILS) \
          $(OBJ_INTERPOLATOR)\
          $(OBJ_COMBUSTION_HIERARCHY) \
          $(OBJ_NONCOMBUSTION_HIERARCHY) \
          $(OBJ_RENEWABLE_HIERARCHY) \
          $(OBJ_STORAGE_HIERARCHY) \
          $(OBJ_MODEL_COMPONENTS)


## == Test: Interpolator == ##

SRC_TEST_INTERPOLATOR = test/source/test_Interpolator.cpp
OUT_TEST_INTERPOLATOR = test/bin/test_Interpolator.out

.PHONY: test_Interpolator
test_Interpolator: $(SRC_TEST_INTERPOLATOR)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_INTERPOLATOR) $(OBJ_ALL) \
-o $(OUT_TEST_INTERPOLATOR) $(LIBS)


## == Test: Production Hierarchy <-- Combustion == ##

SRC_TEST_PRODUCTION = test/source/Production/test_Production.cpp
OUT_TEST_PRODUCTION = test/bin/Production/test_Production.out

.PHONY: test_Production
test_Production: $(SRC_TEST_PRODUCTION)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_PRODUCTION) $(OBJ_ALL) \
-o $(OUT_TEST_PRODUCTION) $(LIBS)


SRC_TEST_COMBUSTION = test/source/Production/Combustion/test_Combustion.cpp
OUT_TEST_COMBUSTION = test/bin/Production/Combustion/test_Combustion.out

.PHONY: test_Combustion
test_Combustion: $(SRC_TEST_COMBUSTION)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_COMBUSTION) $(OBJ_ALL) \
-o $(OUT_TEST_COMBUSTION) $(LIBS)


SRC_TEST_DIESEL = test/source/Production/Combustion/test_Diesel.cpp
OUT_TEST_DIESEL = test/bin/Production/Combustion/test_Diesel.out

.PHONY: test_Diesel
test_Diesel: $(SRC_TEST_DIESEL)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_DIESEL) $(OBJ_ALL) \
-o $(OUT_TEST_DIESEL) $(LIBS)


## == Test: Production Hierarchy <-- Noncombustion == ##

SRC_TEST_NONCOMBUSTION = test/source/Production/Noncombustion/test_Noncombustion.cpp
OUT_TEST_NONCOMBUSTION = test/bin/Production/Noncombustion/test_Noncombustion.out

.PHONY: test_Noncombustion
test_Noncombustion: $(SRC_TEST_NONCOMBUSTION)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_NONCOMBUSTION) $(OBJ_ALL) \
-o $(OUT_TEST_NONCOMBUSTION) $(LIBS)


SRC_TEST_HYDRO = test/source/Production/Noncombustion/test_Hydro.cpp
OUT_TEST_HYDRO = test/bin/Production/Noncombustion/test_Hydro.out

.PHONY: test_Hydro
test_Hydro: $(SRC_TEST_HYDRO)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_HYDRO) $(OBJ_ALL) \
-o $(OUT_TEST_HYDRO) $(LIBS)


## == Test: Production Hierarchy <-- Renewable == ##

SRC_TEST_RENEWABLE = test/source/Production/Renewable/test_Renewable.cpp
OUT_TEST_RENEWABLE = test/bin/Production/Renewable/test_Renewable.out

.PHONY: test_Renewable
test_Renewable: $(SRC_TEST_RENEWABLE)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_RENEWABLE) $(OBJ_ALL) \
-o $(OUT_TEST_RENEWABLE) $(LIBS)


SRC_TEST_SOLAR = test/source/Production/Renewable/test_Solar.cpp
OUT_TEST_SOLAR = test/bin/Production/Renewable/test_Solar.out

.PHONY: test_Solar
test_Solar: $(SRC_TEST_SOLAR)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_SOLAR) $(OBJ_ALL) \
-o $(OUT_TEST_SOLAR) $(LIBS)


SRC_TEST_TIDAL = test/source/Production/Renewable/test_Tidal.cpp
OUT_TEST_TIDAL = test/bin/Production/Renewable/test_Tidal.out

.PHONY: test_Tidal
test_Tidal: $(SRC_TEST_TIDAL)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_TIDAL) $(OBJ_ALL) \
-o $(OUT_TEST_TIDAL) $(LIBS)


SRC_TEST_WAVE = test/source/Production/Renewable/test_Wave.cpp
OUT_TEST_WAVE = test/bin/Production/Renewable/test_Wave.out

.PHONY: test_Wave
test_Wave: $(SRC_TEST_WAVE)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_WAVE) $(OBJ_ALL) \
-o $(OUT_TEST_WAVE) $(LIBS)


SRC_TEST_WIND = test/source/Production/Renewable/test_Wind.cpp
OUT_TEST_WIND = test/bin/Production/Renewable/test_Wind.out

.PHONY: test_Wind
test_Wind: $(SRC_TEST_WIND)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_WIND) $(OBJ_ALL) \
-o $(OUT_TEST_WIND) $(LIBS)


## == Test: Storage Hierarchy == ##

SRC_TEST_STORAGE = test/source/Storage/test_Storage.cpp
OUT_TEST_STORAGE = test/bin/Storage/test_Storage.out

.PHONY: test_Storage
test_Storage: $(SRC_TEST_STORAGE)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_STORAGE) $(OBJ_ALL) \
-o $(OUT_TEST_STORAGE) $(LIBS)


SRC_TEST_LIION = test/source/Storage/test_LiIon.cpp
OUT_TEST_LIION = test/bin/Storage/test_LiIon.out

.PHONY: test_LiIon
test_LiIon: $(SRC_TEST_LIION)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_LIION) $(OBJ_ALL) \
-o $(OUT_TEST_LIION) $(LIBS)


SRC_TEST_H2 = test/source/Storage/test_H2.cpp
OUT_TEST_H2 = test/bin/Storage/test_H2.out

.PHONY: test_H2
test_H2: $(SRC_TEST_H2)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_H2) $(OBJ_ALL) \
-o $(OUT_TEST_H2) $(LIBS)


## == Test: Model and Components == ##

SRC_TEST_CONTROLLER = test/source/test_Controller.cpp
OUT_TEST_CONTROLLER = test/bin/test_Controller.out

.PHONY: test_Controller
test_Controller: $(SRC_TEST_CONTROLLER)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_CONTROLLER) $(OBJ_ALL) \
-o $(OUT_TEST_CONTROLLER) $(LIBS)


SRC_TEST_ELECTRICALLOAD = test/source/test_ElectricalLoad.cpp
OUT_TEST_ELECTRICALLOAD = test/bin/test_ElectricalLoad.out

.PHONY: test_ElectricalLoad
test_ElectricalLoad: $(SRC_TEST_ELECTRICALLOAD)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_ELECTRICALLOAD) $(OBJ_ALL) \
-o $(OUT_TEST_ELECTRICALLOAD) $(LIBS)


SRC_TEST_RESOURCES = test/source/test_Resources.cpp
OUT_TEST_RESOURCES = test/bin/test_Resources.out

.PHONY: test_Resources
test_Resources: $(SRC_TEST_RESOURCES)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_RESOURCES) $(OBJ_ALL) \
-o $(OUT_TEST_RESOURCES) $(LIBS)


SRC_TEST_MODEL = test/source/test_Model.cpp
OUT_TEST_MODEL = test/bin/test_Model.out

.PHONY: test_Model
test_Model: $(SRC_TEST_MODEL)
	$(CXX) $(CXXFLAGS) $(SRC_TEST_MODEL) $(OBJ_ALL) \
-o $(OUT_TEST_MODEL) $(LIBS)


TESTS = test_Interpolator\
        test_Production \
        test_Combustion \
        test_Diesel \
        test_Noncombustion \
        test_Hydro \
        test_Renewable \
        test_Solar \
        test_Tidal \
        test_Wave \
        test_Wind \
        test_Storage \
        test_LiIon \
        test_H2 \
        test_Controller \
        test_ElectricalLoad \
        test_Resources \
        test_Model


OUT_TESTS = $(OUT_TEST_INTERPOLATOR) &&\
            $(OUT_TEST_PRODUCTION) &&\
            $(OUT_TEST_COMBUSTION) &&\
            $(OUT_TEST_DIESEL) &&\
            $(OUT_TEST_NONCOMBUSTION) &&\
            $(OUT_TEST_HYDRO) &&\
            $(OUT_TEST_RENEWABLE) &&\
            $(OUT_TEST_SOLAR) &&\
            $(OUT_TEST_TIDAL) &&\
            $(OUT_TEST_WAVE) &&\
            $(OUT_TEST_WIND) &&\
            $(OUT_TEST_STORAGE) &&\
            $(OUT_TEST_LIION) &&\
            $(OUT_TEST_H2) &&\
            $(OUT_TEST_CONTROLLER) &&\
            $(OUT_TEST_ELECTRICALLOAD) &&\
            $(OUT_TEST_RESOURCES) &&\
            $(OUT_TEST_MODEL)


#### ==== Project ==== ####

.PHONY: PGMcpp_project
PGMcpp_project: $(SRC_PROJECT)
	$(CXX) $(CXXFLAGS) $(SRC_PROJECT) $(OBJ_ALL) -o $(OUT_PROJECT) $(LIBS)

## ======== END BUILD =============================================================== ##



## ======== TARGETS ================================================================= ##


.PHONY: clean
clean:
	rm -frv bin
	rm -frv gmon.*
	rm -frv object
	rm -frv profiling_results
	rm -frv projects/example_cpp
	rm -frv projects/example_py
	rm -frv pybindings/PGMcpp.*
	rm -frv pybindings/test_results/
	rm -frv test/bin
	rm -frv test/object
	rm -frv test/test_results



.PHONY: dirs
dirs:
	mkdir -pv bin/Production/Combustion
	mkdir -pv bin/Production/Noncombustion
	mkdir -pv bin/Production/Renewable
	mkdir -pv bin/Storage
	mkdir -pv object/Production/Combustion
	mkdir -pv object/Production/Noncombustion
	mkdir -pv object/Production/Renewable
	mkdir -pv object/Storage
	mkdir -pv test/bin/Production/Combustion
	mkdir -pv test/bin/Production/Noncombustion
	mkdir -pv test/bin/Production/Renewable
	mkdir -pv test/bin/Storage
	mkdir -pv test/object


.PHONY: docs
docs:
	rm -fv docs/PGMcpp_manual_html.7z
	rm -fv docs/PGMcpp_manual_LaTeX.pdf
	doxygen docs/config.doxygen
	(cd docs/latex && \
     make pdf && \
     mv -fv refman.pdf ../PGMcpp_manual_LaTeX.pdf)
	7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on docs/PGMcpp_manual_html.7z docs/html
	rm -frv docs/html
	rm -frv docs/latex
	rm -frv docs/bibTmpDir
	rm -frv docs/bib2xhtml.pl
	rm -frv docs/doxygen.bst
	rm -frv docs/citelist.doc


.PHONY: PGMcpp
PGMcpp:
	make clean
	make dirs
	make testing_utils
	make interpolator
	make $(COMBUSTION_HIERARCHY)
	make $(NONCOMBUSTION_HIERARCHY)
	make $(RENEWABLE_HIERARCHY)
	make $(STORAGE_HIERARCHY)
	make $(MODEL)
	make $(TESTS)
	clear
	@echo
	$(OUT_TESTS)
	@echo


.PHONY: profile
profile:
	gprof test/bin/test_Model.out > profiling_results


.PHONY: project
project:
	make PGMcpp_project
	$(OUT_PROJECT)

## ======== END TARGETS ============================================================= ##
