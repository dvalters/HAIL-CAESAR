
**TL;DR**: The HAIL-CAESAR _(High-performance Architecture Independent Lisflood-CAESAR model)_ is a 2.5D 'hydrodynamic' landscape evolution model that simulates flooding and erosional processes at the catchment scale, on the order of hours to 100s of years. 

![Model output image](https://github.com/dvalters/HAIL-CAESAR/blob/master/docs/banner.png)

## Installation
1. Download the software, either as a zip/tar file, or by cloning the latest version from github. 
2. The makefile is currently set up to use the gcc compiler, edit `Makefile` if you wish to change this, otherwise:
3. Run `make` in the same folder as `Makefile`.
4. Wait for the code to compile, when finished it will have produced an executable called `HAIL-CAESAR.exe`.
5. You are now ready to run the model!

## Introduction

Numerical models are useful tools for testing hypotheses of geomorphic processes and landscape evolution. Long term landscape evolution takes place over extensive geological time periods, but is controlled by the dynamics of water flow, and sediment transport within river catchments on much shorter, day-to-day time scales. The HAIL-CAESAR software is a numerical model for simulating catchment processes at short-term scales, from hours to years and decades. [Full documentation can be found here.](http://lsdtopotools.github.io/LSDTT_book/#_hydrological_and_erosion_modelling)

This readme documents how to use the HAIL-CAESAR model to set up and run simulations of hydrology and catchment-scale erosion and evolution. LSDCatchmentModel is a 2.5D numerical model of landscape evolution based on the CAESAR-Lisflood model [(Coulthard et al., 2013)](http://onlinelibrary.wiley.com/doi/10.1002/esp.3478/abstract). The model is a [Cellular Automaton](http://natureofcode.com/book/chapter-7-cellular-automata/) model, whereby the grid cells in the model domain each have their own parameters associated with them (such as elevation, water depth, water velocity, sediment load, etc.) and the change in these parameters is determined by the state of the neighbouring grid cells. 

Unlike many models of landscape evolution, this model features an explicit calculation of water flow through the landscape based on a highly simplified form of the shallow water equations [(Bates et al., 2010)](http://www.sciencedirect.com/science/article/pii/S0022169410001538.) Most other models, in order to simulate long term evolution, rely on approximations for water discharge based on drainage area, and assume instantaneous run-off of water under hydrological steady-state conditions. The LSDCatchmentModel is therefore suited to modelling both flooding and erosional processes. It can be run as a standalone hydrological model (no erosion) if so desired.

The model is also developed to run on parallel computing architectures (Multi-core/cluster computers).


********************************************************
### Overview of running CatchmentModel if you have used it before

  1. Prepare your DEM so it has an outlet point touching the side of the DEM bounds. (You may need to rotate it or draw on a channel outlet)
  2. Make sure the DEM is in ascii (.asc) format. 
  3. Prepare a rainfall input file (A list of rain rates per time step, e.g. hourly rainfall). Just a plain text file will do.
  4. Put all your input files and the paramter file in the same folder. Make sure you specify this path in the parameter file as well.
  5. If not already done so, compile the code with the makefile: `make` in the top level directory of the project. Double check that all your parameters are set correctly in the parameter file.
  6. Run the program with `./HAIL-CAESAR.exe /path_to_data/ parameter_file.params`
  7. View the timeseries data and output rasters with a scripting language or GIS of your choice.
