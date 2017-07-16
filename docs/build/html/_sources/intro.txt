Introduction
============

Hydrological and Erosion Modelling
----------------------------------

.. image:: _static/images/flood_depth_fig_crop.png

Numerical models are useful tools for testing hypotheses of geomorphic processes and landscape evolution. Long term landscape evolution takes place over extensive geological time periods, but is controlled by the dynamics of water flow, and sediment transport within river catchments on much shorter, day-to-day time scales. The HAIL-CAESAR package provides a model for simulating catchment processes at short-term scales.

This chapter documents how to use the HAIL-CAESAR model to set up and run simulations of hydrology and catchment-scale erosion and evolution. HAIL-CAESAR is a 2.5D numerical model of landscape evolution based on the `CAESAR-Lisflood model (Coulthard et al., 2013) <http://onlinelibrary.wiley.com/doi/10.1002/esp.3478/abstract>`_. The model is a `cellular automaton <http://natureofcode.com/book/chapter-7-cellular-automata/>`_ model, whereby the grid cells in the model domain each have their own parameters associated with them (such as elevation, water depth, water velocity, sediment load, etc.) and the change in these parameters is determined by the state of the neighbouring grid cells. 

Unlike many models of landscape evolution, this model features an explicit calculation of water flow through the landscape based on a highly simplified form of the shallow water equations `(Bates et al., 2010) <http://www.sciencedirect.com/science/article/pii/S0022169410001538>`_. Most other models, in order to simulate long term evolution, rely on approximations for water discharge based on drainage area, and assume instantaneous run-off of water under hydrological steady-state conditions. The HAIL-CAESAR model is therefore suited to modelling both flooding and erosional processes. It can be run as a standalone hydrological model (no erosion) if so desired.

The model is also developed to run on parallel computing architectures (Multi-core/cluster computers).

Quick guide if you know roughly what you are doing
***************************************************
Overview of running HAIL-CAESAR if you have used it before:

#. Prepare your DEM so it has an outlet point touching the side of the DEM bounds. (You may need to rotate it or draw on a channel outlet)
#. Make sure the DEM is in ascii (.asc) format. 
#. Prepare a rainfall input file (A list of rain rates per time step, e.g. hourly rainfall). Just a plain text file will do.
#. Put all your input files and the paramter file in the same folder. Make sure you specify this path in the parameter file as well.
#. If not already done so, compile the code with the makefile: `make`, when in the top-level directory.
#. Double check that all your parameters are set correctly in the parameter file.
#. Run the program with `./HAIL-CAESAR.exe /path/to/data/ parameter_file.params`
#. View the timeseries data and output rasters with a scripting language or GIS of your choice.



