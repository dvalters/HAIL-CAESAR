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

The full path that the output files will be read to.


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

Name fo the text file timeseries containing the rainfall rate at each rainfall input timestep in mm/hr.

``grain_data_file``
~~~~~~~~~~~~~~~~~~~

``bedrock_data_file``
~~~~~~~~~~~~~~~~~~~~~




Run time and timestep
---------------------

These parameters control things like time stepping and how long the model runs for.

``min_time_step``
~~~~~~~~~~~~~~~~~

The minimum timestep used in the model.

 - **Units, data type**: Model seconds, integer

``max_time_step``
~~~~~~~~~~~~~~~~~

The maximum timestep used in the model

 - **Units, data type**: Model seconds, integer
 - **Default value**: 3600

``run_time_start``
~~~~~~~~~~~~~~~~~~

``max_run_duration``
~~~~~~~~~~~~~~~~~~~~

``memory_limit``
~~~~~~~~~~~~~~~~


Sediment Transport
------------------

``transport_law``
~~~~~~~~~~~~~~~~~

``max_tau_velocity``
~~~~~~~~~~~~~~~~~~~~

``active_layer_thickness``
~~~~~~~~~~~~~~~~~~~~~~~~~~

``chann_lateral_erosion``
~~~~~~~~~~~~~~~~~~~~~~~~~

``erode_limit``
~~~~~~~~~~~~~~~

``suspended_sediment_on``
~~~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``read_in_graindata_from_file``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``bedrock_layer_on``
~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)


Lateral Bank Erosion
--------------------

``lateral_erosion_on``
~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``lateral_erosion_const``
~~~~~~~~~~~~~~~~~~~~~~~~~

``edge_smoothing_passes``
~~~~~~~~~~~~~~~~~~~~~~~~~

``downstream_cell_shift``
~~~~~~~~~~~~~~~~~~~~~~~~~

``lateral_cross_chan_smoothing``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hydrology
---------

``hydro_model_only``
~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``topmodel_m_value``
~~~~~~~~~~~~~~~~~~~~

``in_out_difference``
~~~~~~~~~~~~~~~~~~~~~

``min_q_for_depth_calc``
~~~~~~~~~~~~~~~~~~~~~~~~

``max_q_for_depth_calc``
~~~~~~~~~~~~~~~~~~~~~~~~

``water_depth_erosion_threshold``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``slope_on_edge_cell``
~~~~~~~~~~~~~~~~~~~~~~

``evaporation_rate``
~~~~~~~~~~~~~~~~~~~~~

``courant_number``
~~~~~~~~~~~~~~~~~~

``froude_num_limit``
~~~~~~~~~~~~~~~~~~~~

``mannings_n``
~~~~~~~~~~~~~~

``hflow_threshold``
~~~~~~~~~~~~~~~~~~~

Precipitation
-------------

``rainfall_data_on``
~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``rain_data_time_step``
~~~~~~~~~~~~~~~~~~~~~~~

``spatial_var_rain``
~~~~~~~~~~~~~~~~~~~~

``num_unique_rain_cells``
~~~~~~~~~~~~~~~~~~~~~~~~~

``spatially_complex_rainfall_on``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

(**yes** | **no**)

``interpolation_method``
~~~~~~~~~~~~~~~~~~~~~~~~

``generate_artificial_rainfall``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
