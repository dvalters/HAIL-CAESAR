
The **HAIL-CAESAR** model _(High-performance Architecture Independent Lisflood-CAESAR model)_ is a hydrodynamic landscape evolution model that simulates flooding and erosional processes within river catchments, on timescales of hours to 100s of years. It is based on a port of the [CAESAR-Lisflood](https://sourceforge.net/projects/caesar-lisflood/) model for Unix systems.

![Model output image](https://raw.githubusercontent.com/dvalters/HAIL-CAESAR/master/docs/banner.png)

**Documentation can be found here**: [![Documentation Status](https://readthedocs.org/projects/hail-caesar/badge/?version=latest)](http://hail-caesar.readthedocs.io/en/latest/?badge=latest)

## Installation
1. Download the software, either as a zip/tar file, or by cloning the latest version from github. 
2. The makefile is currently set up to use the gcc compiler, edit `Makefile` if you wish to change this, otherwise:
3. Run `make` in the same folder as `Makefile`.
4. Wait for the code to compile, when finished it will have produced an executable called `HAIL-CAESAR.exe`.
5. You are now ready to run the model!

## Introduction

HAIL-CAESAR was developed out of a need to run ensemble-style simulations on a variety of software architectures, such as HPC systems and desktop workstations. It has been tested on Linux and Mac OS platforms, and should run on Windows too, provided you have a recent Microsoft C++ compiler. 

This readme documents how to use the HAIL-CAESAR model to set up and run simulations of hydrology and catchment-scale erosion and evolution. The model is based on the CAESAR-Lisflood model [(Coulthard et al., 2013)](http://onlinelibrary.wiley.com/doi/10.1002/esp.3478/abstract). The model is a [Cellular Automaton](http://natureofcode.com/book/chapter-7-cellular-automata/) model, whereby the grid cells in the model domain each have their own parameters associated with them (such as elevation, water depth, water velocity, sediment load, etc.) and the change in these parameters is determined by the state of the neighbouring grid cells. 

Unlike many models of landscape evolution, this model features an explicit calculation of water flow through the landscape based on a highly simplified form of the shallow water equations [(Bates et al., 2010)](http://www.sciencedirect.com/science/article/pii/S0022169410001538.) Most other models, in order to simulate long term evolution, rely on approximations for water discharge based on drainage area, and assume instantaneous run-off of water under hydrological steady-state conditions. The HAIL-CAESAR model is therefore suited to modelling both flooding and erosional processes. It can be run as a standalone hydrological model (no erosion) if so desired.

The model is also developed to run on parallel computing architectures (Multi-core/cluster computers) using OpenMP.


********************************************************
### Overview of running HAIL-CAESAR

  1. Prepare your DEM so it has an outlet point touching the side of the DEM bounds. (You may need to rotate it or draw on a channel outlet)
  2. Make sure the DEM is in ascii (.asc) format. 
  3. Prepare a rainfall input file (A list of rain rates per time step, e.g. hourly rainfall). Just a plain text file will do.
  4. Put all your input files and the paramter file in the same folder. Make sure you specify this path in the parameter file as well.
  5. If not already done so, compile the code with the makefile: `make` in the top level directory of the project. Double check that all your parameters are set correctly in the parameter file.
  6. Run the program with `./HAIL-CAESAR.exe /path_to_data/ parameter_file.params`
  7. View the timeseries data and output rasters with a scripting language or GIS of your choice. I recommend QGIS or the excellent LSDMappingTools.
