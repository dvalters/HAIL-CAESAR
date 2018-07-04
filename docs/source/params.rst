Parameters
==========

The parameter file (``HailCaesar.params``) is read by the program when it starts, setting the variables for various components of the model, as well as the names of the DEM files and supplementary input files needed for the model simulation. It is a plain text file and can be called anything you like. By convention, the example parameter file supplied with the code has the suffix ``.params``, but you may use whatever you like.

Anything in the parameter file preceded by a ``#`` will be treated as a
comment. You are free to comment away in the parameter file to your heart’s content.

The order of the parameters in the parameter file is not strict, but we
stick to the convention used in the sample file for reference here. The
parameter names must not be changed, and must be lowercase.

Parameter File
++++++++++++++

**This section contains explanations on each of the parameters**

File Information
----------------

``read_fname``
~~~~~~~~~~~~~~
The name of the DEM (digital elevation model) file that contains the terrain surface/topography.
**No file extension** should be included here.


``dem_read_extension``
~~~~~~~~~~~~~~~~~~~~~~
The extension and format of the DEM file. The format is inferred from the extension given.

Options
^^^^^^^

 - asc (ASCII Grid/ESRI Grid)

``dem_write_extension``
~~~~~~~~~~~~~~~~~~~~~~~

The extension of the output rasters. This extension determines the format of the files as well, so specifying ``flt`` for example will also write them out in binary format as well as appending the extension itself.

Options
^^^^^^^
  - asc
  - flt
  - bil

``read_path``
~~~~~~~~~~~~~

The full path that the input files will be read from.
e.g. DEMs, rainfall files, grain data files.

``write_path``
~~~~~~~~~~~~~~

The full path that the output files will be written to.


``write_fname``
~~~~~~~~~~~~~~~

The output name for the timeseries output. (e.g. Hydrograph and Sedigraph.)
Will be a plain text file.

``timeseries_save_interval``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The interval that data is written to the timeseries file. Should be in model minutes, I.e. simulated minutes. (Integer)


Supplementary Files
-------------------

``hydroindex_file``
~~~~~~~~~~~~~~~~~~~

When using the spatially variable rainfall configuration. The hydroindex file name (with extension). Only ascii (asc) currently supported. This file is a DEM the same resolution and extent as the main terrain DEM, but the cell values are integers from 1, marking each region that will receive a different rainfall input record.

``rainfall_data_file``
~~~~~~~~~~~~~~~~~~~~~~

Name of the text file timeseries containing the rainfall rate at each rainfall input timestep in mm/hr.

``grain_data_file``
~~~~~~~~~~~~~~~~~~~

Name of the file that contains the graindata information. I.e. if you are prepopulating the model domain with grain size and grain fraction information, you need tto set this file. More details are given in the input files section.


``bedrock_data_file``
~~~~~~~~~~~~~~~~~~~~~

Name of the DEM that will be used to represent the bedrock layer, if the model is configured with this mode. (``bedrock_layer_on``). If this file is specified, then the main terrain DEM, (``read_fname``) will still act as the uppermost terrain surface, with the bedrock DEM situated below it - but you must have correctly set the elevations on the bedrock DEM file to be less than the surface DEM. (i.e. the model currently cannot do this for you.)


Run time and timestep
---------------------

These parameters control things like time stepping and how long the model runs for.

``min_time_step``
~~~~~~~~~~~~~~~~~

The minimum timestep used in the model.

 - **Units, data type**: Model seconds, integer
 - **Default**: 1 (second)

``max_time_step``
~~~~~~~~~~~~~~~~~

The maximum timestep used in the model

 - **Units, data type**: Model seconds, integer
 - **Default value**: 3600 (seconds)

``run_time_start``
~~~~~~~~~~~~~~~~~~

The model run start time - only zero is currently supported as the restart function is currently implemented.

 - **Units, data type**: Model hours, integer
 - **Default value**: 0

``max_run_duration``
~~~~~~~~~~~~~~~~~~~~

The maximum run duration of the model. I.e. the model stops when it reaches this point.

 - **Units, data type**: Model hours, integer
 - (No default value)

``memory_limit``
~~~~~~~~~~~~~~~~

This can be ignored, it was a feature of CAESAR-Lisflood to restrict the size of the arrays such as the huge grain size array in memory. It is set to 1.


Sediment Transport
------------------

``transport_law``
~~~~~~~~~~~~~~~~~

Determines which sediment transport law is used by the model. The options are Wilcock and Crowe or Einstein-Brown. See the pages in the documentation for more detail.

Options
^^^^^^^

 - ``wilcock``
 - ``einstein``

``max_tau_velocity``
~~~~~~~~~~~~~~~~~~~~

Limits the maximum velocity used to calculate sediment transport. The default is 5. It is rarely invoked except in very steep slopes.

``active_layer_thickness``
~~~~~~~~~~~~~~~~~~~~~~~~~~

This controls the thickness of layer representing the surface, bedload or subsurface. It should be around 0.1 to 0.2. It must be at least 4 times the erode_limit parameter. (See below)


``chann_lateral_erosion``
~~~~~~~~~~~~~~~~~~~~~~~~~

In channel lateral erosion rate. Prevents overdeepening feedback. See explanation here.

``erode_limit``
~~~~~~~~~~~~~~~

Maximum erosion limit per cell (or deposition). Prevents numerical instabilities by transferring too much between cell to cell. Should be around 0.01 for 10m or less DEMs, slightly higher for coarse DEMs.

``suspended_sediment_on``
~~~~~~~~~~~~~~~~~~~~~~~~~

Turns on suspended sediment for the first fraction only at present. (i.e. the smallest grainsize fraction.

(**yes** | **no**)

``read_in_graindata_from_file``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Reads in the initial grain size data from a file. Normally the initial distribution of grainsizes is uniform across the landscape if this is not specified.

``bedrock_layer_on``
~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)


Lateral Bank Erosion
--------------------

Please note that this feature is untested in HAIL-CAESAR as of yet.

``lateral_erosion_on``
~~~~~~~~~~~~~~~~~~~~~~

Turns on lateral erosion in channels.

(**yes** | **no**)

``lateral_erosion_const``
~~~~~~~~~~~~~~~~~~~~~~~~~

See Coulthard and Van de Wiel (2007) for details. This parameter sets the lateral erosion rates, and ought to be calibrated to a field site. Values can ramge from 0.01-0.001 for braided rivers; 0.0001 for meandering channels. This value is grid cell size independent.

``edge_smoothing_passes``
~~~~~~~~~~~~~~~~~~~~~~~~~

Determines how well smoothed the curvature of the channels will be. Should be set as the frequency of meanders/distance between two meanders in grid cell size.

``downstream_cell_shift``
~~~~~~~~~~~~~~~~~~~~~~~~~

The gradient used to determine lateral erosion rates can be shifted downstream, allowing meander bars and bends to migrate down channel. Suggested values are around 10% of the edge_smoothing_passes parameter. So around 1-5.

``lateral_cross_chan_smoothing``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The maximum difference allowed in the the cross channel smoothing of edge values. After calculating the radius of curvature for the outside of a meander bend, the model must interpolate this value to determine how much sediment can be moved laterally. Smaller values result in better cross channel gradients, but take up more CPU time. Typical default value is 0.0001. If your channels are well resolved by DEM resolution, you may need to reduce by an order of magnitude.

Hydrology
---------

``hydro_model_only``
~~~~~~~~~~~~~~~~~~~~

Runs the model as a hydrological/flood inundation model only if set to yes. Will turn off all the erosion routines and terrain will be fixed. Speeds up run times considerably.

(**yes** | **no**)

``topmodel_m_value``
~~~~~~~~~~~~~~~~~~~~

As well as the water routing sub-model, LSDCatchmentModel also calculates the discharge based on Beven’s TOPMODEL (i.e. discharge approximation based on drainage area and topography. The model contains the infamous m parameter, which varies depending on environment. You should consult the literature for appropriate values.

For catchment mode this is an important variable as it controls the peak and duration of the hydrograph generated by a rain event. It is the same as the ‘m’ value in TOPMODEL, that CAESAR-lisflood’s hydrological model is based on. Typical values for m are from 0.02 (meaning low flood peaks and long duration hydrographs) to 0.005 (flashier peaks) and examples of values used can be found in the CAESAR and TOPMODEL literature.


``in_out_difference``
~~~~~~~~~~~~~~~~~~~~~

If greater than 0, allows the model to run faster in periods of hydrological steady state. If the difference between water entering the catchment and water leaving the catchment is equal to or less than this value, the model will increase the time step. The time step will then be determined by erosional and depositional processes, which are typically much slower acting. Can be set to a low mean annual flow value for the river.

``min_q_for_depth_calc``
~~~~~~~~~~~~~~~~~~~~~~~~

Threshold for calculating flow depths. The model will not calculate flow depths when the discharge at a cell is below this value, avoiding CPU time spent calculating incredibly small flow amounts. Should be set to approximately 10% of grid cell size. e.g 0.5 for a 50m DEM.

``max_q_for_depth_calc``
~~~~~~~~~~~~~~~~~~~~~~~~

An upper discharge threshold that will prevent water being added above the given discharge threshold. Typically 1000.0, but lowering the value will shift the balance of water being added to the headwaters, rather than lower down through the catchment.

``water_depth_erosion_threshold``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If water depths are below this threshold, the model will not calculate erosion for that cell in that timestep. Used to prevent CPU time being spent on incredibly small amounts of erosion calculations.

``slope_on_edge_cell``
~~~~~~~~~~~~~~~~~~~~~~

The slope used to calculate water flow on the edge of the DEM (since there is no neighbouring cell to calculate the downstream gradient. You should set this to approximately the same as the average channel gradient near the outlet of your river.

``evaporation_rate``
~~~~~~~~~~~~~~~~~~~~~

Untested/unimplemented yet.

``courant_number``
~~~~~~~~~~~~~~~~~~

Controls the numerical stability and execution speed of the flow model. See Bates et al (2009). Typical values should be between 0.3 and 0.7. Higher values will speed up the model, but are more unstable. Parameter is dependent on grid cell size. DEMs of 20-50m resolution can use values of 0.7, finer DEMs (e.g. <2m) will need the lowest value of 0.3, but do not set it lower than 0.3.

``froude_num_limit``
~~~~~~~~~~~~~~~~~~~~

Restricts flow between cells per time step, as too much can lead to checkerboarding effects. If this happens the froude number can be lowered. The default value of 0.8 results in subcritical flow - flow dominated by gravitational forces and behving in a slow or stable way. A value of 1 results in critical flow, which may be ok for shallow flows at coarse grid cell resolutions. Note that reducing flow with the Froude number will reduce the speed of a flood wave moving downstream.

``mannings_n``
~~~~~~~~~~~~~~

A roughness coefficient used by the flow model. Values can be looked-up here. http://www.fsl.orst.edu/geowater/FX3/help/8_Hydraulic_Reference/Mannings_n_Tables.htm

``hflow_threshold``
~~~~~~~~~~~~~~~~~~~

This threshold prevents water being routed between adjacent cells when the gradient is incredibly small. A good default value is 0.00001.

Precipitation
-------------

``rainfall_data_on``
~~~~~~~~~~~~~~~~~~~~

Reads in rainfall data from a rainfall.txt file timeseries.

(**yes** | **no**)

``rain_data_time_step``
~~~~~~~~~~~~~~~~~~~~~~~

The timestep of the rainfall inputs. Note: even if you set this to something other than ``60`` (for hourly rainfall), the units are always mm/hr. I.e. this is the instantaneous rainfall rate at each timestep.

``spatial_var_rain``
~~~~~~~~~~~~~~~~~~~~

Use spatially variable rainfall inputs over the model domain. This requires a rainfall.txt file with multiple columns (for each region of the catchment receiving different rainfall rates.) and a hydroindex file to map rainfall columns in the timeseries file to the correct cells in the model domain.

``num_unique_rain_cells``
~~~~~~~~~~~~~~~~~~~~~~~~~

The number of rainfall regions/cells in your rainfall data and hydroindex file.


``spatially_complex_rainfall_on``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Not fully implemented yet.

(**yes** | **no**)

``interpolation_method``
~~~~~~~~~~~~~~~~~~~~~~~~

Not fully implemented yet.

``generate_artificial_rainfall``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Not implemented yet.

(**yes** | **no**)


Vegetation
----------

``vegetation_on``
~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``grass_grow_rate``
~~~~~~~~~~~~~~~~~~~

``vegetation_crit_shear``
~~~~~~~~~~~~~~~~~~~~~~~~~~

``veg_erosion_prop``
~~~~~~~~~~~~~~~~~~~~

Hillslope
---------

``creep_rate``
~~~~~~~~~~~~~~

``slope_failure_thresh``
~~~~~~~~~~~~~~~~~~~~~~~~

``soil_erosion_rate``
~~~~~~~~~~~~~~~~~~~~~

``soil_j_mean_depends``
~~~~~~~~~~~~~~~~~~~~~~~

``call_muddpile_model``
~~~~~~~~~~~~~~~~~~~~~~~


Output Rasters
--------------

``raster_output_interval``
~~~~~~~~~~~~~~~~~~~~~~~~~~

``write_waterdepth_file``
~~~~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``waterdepth_outfile_name``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

``write_elev_file``
~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``write_elevation_file``
~~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``write_grainsize_file``
~~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``grainsize_file``
~~~~~~~~~~~~~~~~~~

``write_elevdiff_file``
~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``elevdiff_outfile_name``
~~~~~~~~~~~~~~~~~~~~~~~~~

``raingrid_fname_out``
~~~~~~~~~~~~~~~~~~~~~~



Debug Options
---------------
