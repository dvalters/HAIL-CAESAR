Output Files
============

HAIL-CAESAR will output the same types of output data as the CAESAR-Lisflood model. These are:

#. Time series file of water discharges and sediment outputs at each time series save interaval
#. DEMs or 'rasters' of variables of interest, e.g. Water Depth, Erosion Amount,

Time Series File
----------------

This will be a single file and the filename is set by the parameter file option `write_fname`. It can be any name you like with any extension.

The file contains 14 columns of text. The number of rows is dependent on the length of the run and the interval you have set to save the time series data.

The `timeseries_save_interval` option determines how often the model will write to this file. The value is in modelled minutes, or simulated minutes. Setting this to 5 would make HAIL-CAESAR write output here every 5 minutes of simulated time until the end of the model run.

The columns contain data as follows:

#. Timeseries save interval number. This is just a numbering of the rows at each save interval. It is *not* the model timestep or any actual 'time' value. To convert to an actual time, you would multiply each number by `timeseries_save_interval`, example, which would give you the number of simulated minutes elapsed since the start of the run.
#. Actual discharge. (Cumecs, or cubic metres per second). This is the instantaneous rate of water discarge at the outlet(s) of the catchment DEM, for that output timestep. (Calculated by LISFLOD
#. Expected discharge. (Cumecs, or cubic metres per second). This value is based on the simpler TOPMODEL estimation of discharge. (See TOPMODEL literature).
#. Remaining columns (9) - these are sediment outputs for each grain size fraction. (1-9). The sediment outputs are *totals* in **cubic metres** for that time interval (in contrast to the water discharge values which are instantaneous rates.

*Note that the format of this file currently mirrors that of CAESAR-Lisflood v1.8, so results from the two models are compatible and you should be able to use any existing plotting scripts you have written for the two models interchangeably.*
