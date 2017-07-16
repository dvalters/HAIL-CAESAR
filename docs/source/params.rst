Parameter File
==============

The parameter file is read by the CatchmentModel program when it starts,
setting the variables for various components of the model, as well as
the names of the DEM files and supplementary input files needed for the
model simulation. It is a plain text file and can be called anything you
like. By convention, the example parameter file supplied with the code
has the suffix ``.params``, but you may use whatever you like.

Anything in the parameter file preceded by a ``#`` will be treated as a
comment. You are free to comment away in the parameter file to your
heart’s content.

The order of the parameters in the parameter file is not strict, but we
stick to the convention used in the sample file for reference here. The
parameter names must not be changed, and must be lowercase.

+--------------------+--------------------+--------------------------------------+
| Parameter Name     | Units/Data Type    | Description                          |
+====================+====================+======================================+
| ``read_fname``     | text               | The name of your DEM that will act   |
|                    |                    | as the terrain surface in the model. |
|                    |                    | Do not give the extension here.      |
+--------------------+--------------------+--------------------------------------+
| ``dem_read_extensi | text               | The extension of the input DEM. Only |
| on``               |                    | ASCII (asc) is currently supported,  |
|                    |                    | we are working on supporting other   |
|                    |                    | input formats soon.                  |
+--------------------+--------------------+--------------------------------------+
| ``dem_write_extens | text               | The format to save output rasters    |
| ion``              |                    | in. Supported choices are: 'asc' or  |
|                    |                    | 'flt' (binary). 'bil' also works but |
|                    |                    | no georeferencing is supported yet.  |
+--------------------+--------------------+--------------------------------------+
| ``read_path``      | text               | The path where your input files can  |
|                    |                    | be found                             |
+--------------------+--------------------+--------------------------------------+
| ``write_path``     | text               | The path where your output files     |
|                    |                    | will be written                      |
+--------------------+--------------------+--------------------------------------+
| ``write_fname``    | text               | A name for the output timeseries     |
|                    |                    | file. You can supply your own        |
|                    |                    | extension, but it will always be a   |
|                    |                    | plain text file.                     |
+--------------------+--------------------+--------------------------------------+
| ``timeseries_save_ | model              | Will record a timeseries record (new |
| interval``         | minutes/integer    | row) every X minutes in model time.  |
|                    |                    | The timeseries output contains the   |
|                    |                    | discharge of water and sediment.     |
+--------------------+--------------------+--------------------------------------+
| ``SUPPLEMENTARY INPUT``                                                        |
+--------------------+--------------------+--------------------------------------+
| ``hydroindex_file``| text               | The hydroindex file name (with       |
|                    |                    | extension). Only ascii (asc)         |
|                    |                    | currently supported.                 |
+--------------------+--------------------+--------------------------------------+
| ``rainfall_data_fi | text               | The rainfall data file name (with    |
| le``               |                    | extension). Must be a plain text     |
|                    |                    | file.                                |
+--------------------+--------------------+--------------------------------------+
| ``grain_data_file``| text               | Name of your grain data input file.  |
|                    |                    |                                      |
+--------------------+--------------------+--------------------------------------+
| ``bedrock_data_fil | text               | Name of the bedrock DEM. If          |
| e``                |                    | specified, this DEM (ascii only)     |
|                    |                    | will be loaded as a solid bedrock    |
|                    |                    | layer beneath your top-layer terrain |
|                    |                    | DEM. It’s up to the use to make sure |
|                    |                    | the elevations are lower than the    |
|                    |                    | top layer!                           |
+--------------------+--------------------+--------------------------------------+
| ``NUMERICAL``                                                                  |
+--------------------+--------------------+--------------------------------------+
| ``min_time_step``  | model seconds,     | The minimum timestep used in the     |
|                    | integer            | model                                |
+--------------------+--------------------+--------------------------------------+
| ``max_time_step``  | model seconds,     | Maximum timestep used by model.      |
|                    | integer            | Should not really be higher than     |
|                    |                    | your rainfall data interval,         |
|                    |                    | otherwise you will skip input data.  |
+--------------------+--------------------+--------------------------------------+
| ``max_run_         | model hours,       | Length of your simulation, minus 1.  |
| duration``         | integer            | (So for a 72 hour simulation, enter  |
|                    |                    | '71' here.) This is a weird quirk    |
|                    |                    | that will be fixed in a later        |
|                    |                    | release.                             |
+--------------------+--------------------+--------------------------------------+
| ``memory_limit``   |                    | Ignore, leave at 1.                  |
+--------------------+--------------------+--------------------------------------+
| ``SEDIMENT``                                                                   |
+--------------------+--------------------+--------------------------------------+
| ``transport_law``  | text               | Can either be **wilcock** or         |
|                    |                    | **einstein**. Determines which       |
|                    |                    | sediment transport law is used by    |
|                    |                    | the model.                           |
+--------------------+--------------------+--------------------------------------+
| ``max_tau_         | meters/second      | Limits the maximum velocity used to  |
| velocity``         |                    | calculate sediment transport. The    |
|                    |                    | default is 5. It is rarely invoked   |
|                    |                    | except in very steep slopes.         |
+--------------------+--------------------+--------------------------------------+
| ``active_layer_thi | metres             | This controls the thickness of layer |
| ckness``           |                    | representing the surface, bedload or |
|                    |                    | subsurface. It should be around 0.1  |
|                    |                    | to 0.2. It must be at least 4 times  |
|                    |                    | the **erode\_limit** parameter. (See |
|                    |                    | below)                               |
+--------------------+--------------------+--------------------------------------+
| ``chann_lateral_er |                    | In channel lateral erosion rate.     |
| osion``            |                    | Prevents overdeepening feedback.     |
|                    |                    | `See explanation                     |
|                    |                    | here <http://www.showme.com/sh/?i=24 |
|                    |                    | 567>`__                              |
+--------------------+--------------------+--------------------------------------+
| ``erode_limit``    | metres             | Maximum erosion limit per cell (or   |
|                    |                    | deposition). Prevents numerical      |
|                    |                    | instabilities by transferring too    |
|                    |                    | much between cell to cell. Should be |
|                    |                    | around 0.01 for 10m or less DEMs,    |
|                    |                    | slightly higher for coarse DEMs.     |
+--------------------+--------------------+--------------------------------------+
| ``suspended_sedime | boolean (yes/no)   | Turns on suspended sediment for the  |
| nt_on``            |                    | first fraction only at present.      |
+--------------------+--------------------+--------------------------------------+
| ``read_in_graindat | boolean (yes/no)   | Reads in the initial grain size data |
| a_from_file``      |                    | from a file. Normally the initial    |
|                    |                    | distribution of grainsizes is        |
|                    |                    | uniform across the landscape if this |
|                    |                    | is not specified.                    |
+--------------------+--------------------+--------------------------------------+
| ``LATERAL EROSION OPTIONS``                                                    |
+--------------------+--------------------+--------------------------------------+
| ``lateral_erosion_ | boolean (yes/no)   | Turns on lateral erosion in channels |
| on``               |                    | (UNTESTED)                           |
+--------------------+--------------------+--------------------------------------+
| ``lateral_erosion_ |                    | See Coulthard and Van de Wiel (2007) |
| const``            |                    | for details. This parameter sets the |
|                    |                    | lateral erosion rates, and ought to  |
|                    |                    | be calibrated to a field site.       |
|                    |                    | Values can ramge from 0.01-0.001 for |
|                    |                    | braided rivers; 0.0001 for           |
|                    |                    | meandering channels. This value is   |
|                    |                    | grid cell size independent.          |
+--------------------+--------------------+--------------------------------------+
| ``edge_smoothing_p | integer            | Determines how well smoothed the     |
| asses``            |                    | curvature of the channels will be.   |
|                    |                    | Should be set as the frequency of    |
|                    |                    | meanders/distance between two        |
|                    |                    | meanders in grid cell size.          |
+--------------------+--------------------+--------------------------------------+
| ``downstream_cell_ | integer            | The gradient used to determine       |
| shift``            |                    | lateral erosion rates can be shifted |
|                    |                    | downstream, allowing meander bars    |
|                    |                    | and bends to migrate down channel.   |
|                    |                    | Suggested values are around 10% of   |
|                    |                    | the **edge\_smoothing\_passes**      |
|                    |                    | parameter. So around 1-5.            |
+--------------------+--------------------+--------------------------------------+
| ``lateral_cross_ch |                    | The maximum difference allowed in    |
| an_smoothing``     |                    | the the cross channel smoothing of   |
|                    |                    | edge values. After calculating the   |
|                    |                    | radius of curvature for the outside  |
|                    |                    | of a meander bend, the model must    |
|                    |                    | interpolate this value to determine  |
|                    |                    | how much sediment can be moved       |
|                    |                    | laterally. Smaller values result in  |
|                    |                    | better cross channel gradients, but  |
|                    |                    | take up more CPU time. Typical       |
|                    |                    | default value is 0.0001. If your     |
|                    |                    | channels are well resolved by DEM    |
|                    |                    | resolution, you may need to reduce   |
|                    |                    | by an order of magnitude.            |
+--------------------+--------------------+--------------------------------------+
| ``HYDROLOGY``                                                                  |
+--------------------+--------------------+--------------------------------------+
| ``hydro_model_     | boolean (yes/no)   | Runs the model as a                  |
| only``             |                    | hydrological/flood inundation model  |
|                    |                    | only if set to yes. Will turn off    |
|                    |                    | all the erosion routines and terrain |
|                    |                    | will be fixed. Speeds up run times   |
|                    |                    | considerably.                        |
+--------------------+--------------------+--------------------------------------+
| ``topmodel_m_      |                    | As well as the water routing         |
| value``            |                    | sub-model, LSDCatchmentModel also    |
|                    |                    | calculates the discharge based on    |
|                    |                    | Beven’s TOPMODEL (i.e. discharge     |
|                    |                    | approximation based on drainage area |
|                    |                    | and topography. The model contains   |
|                    |                    | the infamous **m** parameter, which  |
|                    |                    | varies depending on environment. You |
|                    |                    | should consult the literature for    |
|                    |                    | appropriate values.                  |
|                    |                    |                                      |
|                    |                    |     **Tip**                          |
|                    |                    |                                      |
|                    |                    |     For catchment mode this is an    |
|                    |                    |     important variable as it         |
|                    |                    |     controls the peak and duration   |
|                    |                    |     of the hydrograph generated by a |
|                    |                    |     rain event. It is the same as    |
|                    |                    |     the 'm' value in TOPMODEL, that  |
|                    |                    |     CAESAR-lisflood’s hydrological   |
|                    |                    |     model is based on. Typical       |
|                    |                    |     values for m are from 0.02 (low  |
|                    |                    |     - meaning low flood peaks and    |
|                    |                    |     long duration hydrographs) to    |
|                    |                    |     0.005 (higher, flashier peaks)   |
|                    |                    |     and examples of values used can  |
|                    |                    |     be found in the CAESAR and       |
|                    |                    |     TOPMODEL literature.             |
+--------------------+--------------------+--------------------------------------+
| ``in_out_differenc | cumecs             | If greater than 0, allows the model  |
| e``                |                    | to run faster in periods of          |
|                    |                    | hydrological steady state. If the    |
|                    |                    | difference between water entering    |
|                    |                    | the catchment and water leaving the  |
|                    |                    | catchment is equal to or less than   |
|                    |                    | this value, the model will increase  |
|                    |                    | the time step. The time step will    |
|                    |                    | then be determined by erosional and  |
|                    |                    | depositional processes, which are    |
|                    |                    | typically much slower acting. Can be |
|                    |                    | set to a low mean annual flow value  |
|                    |                    | for the river.                       |
+--------------------+--------------------+--------------------------------------+
| ``min_q_for_depth_ | cumecs             | Threshold for calculating flow       |
| calc``             |                    | depths. The model will not calculate |
|                    |                    | flow depths when the discharge at a  |
|                    |                    | cell is below this value, avoiding   |
|                    |                    | CPU time spent calculating           |
|                    |                    | incredibly small flow amounts.       |
|                    |                    | Should be set to approximately 10%   |
|                    |                    | of grid cell size. e.g 0.5 for a 50m |
|                    |                    | DEM.                                 |
+--------------------+--------------------+--------------------------------------+
| ``max_q_for_depth_ | cumecs             | An upper discharge threshold that    |
| calc``             |                    | will prevent water being added above |
|                    |                    | the given discharge threshold.       |
|                    |                    | Typically 1000.0, but lowering the   |
|                    |                    | value will shift the balance of      |
|                    |                    | water being added to the headwaters, |
|                    |                    | rather than lower down through the   |
|                    |                    | catchment.                           |
+--------------------+--------------------+--------------------------------------+
| ``water_depth_eros | metres             | If water depths are below this       |
| ion_threshold``    |                    | threshold, the model will not        |
|                    |                    | calculate erosion for that cell in   |
|                    |                    | that timestep. Used to prevent CPU   |
|                    |                    | time being spent on incredibly small |
|                    |                    | amounts of erosion calculations.     |
+--------------------+--------------------+--------------------------------------+
| ``slope_on_edge_ce |                    | The slope used to calculate water    |
| ll``               |                    | flow on the edge of the DEM (since   |
|                    |                    | there is no neighbouring cell to     |
|                    |                    | calculate the downstream gradient.   |
|                    |                    | You should set this to approximately |
|                    |                    | the same as the average channel      |
|                    |                    | gradient near the outlet of your     |
|                    |                    | river.                               |
+--------------------+--------------------+--------------------------------------+
| ``evaporation_     |                    | Untested/unimplemented yet           |
| rate``             |                    |                                      |
+--------------------+--------------------+--------------------------------------+
| ``courant_number`` |                    | Controls the numerical stability and |
|                    |                    | execution speed of the flow model.   |
|                    |                    | See Bates et al (2009). Typical      |
|                    |                    | values should be between 0.3 and     |
|                    |                    | 0.7. Higher values will speed up the |
|                    |                    | model, but are more unstable.        |
|                    |                    | Parameter is dependent on grid cell  |
|                    |                    | size. DEMs of 20-50m resolution can  |
|                    |                    | use values of 0.7, finer DEMs (e.g.  |
|                    |                    | <2m) will need the lowest value of   |
|                    |                    | 0.3, but do not set it lower than    |
|                    |                    | 0.3.                                 |
+--------------------+--------------------+--------------------------------------+
| ``froude_num_      |                    | Restricts flow between cells per     |
| limit``            |                    | time step, as too much can lead to   |
|                    |                    | checkerboarding effects. If this     |
|                    |                    | happens the froude number can be     |
|                    |                    | lowered. The default value of 0.8    |
|                    |                    | results in subcritical flow - flow   |
|                    |                    | dominated by gravitational forces    |
|                    |                    | and behving in a slow or stable way. |
|                    |                    | A value of 1 results in critical     |
|                    |                    | flow, which may be ok for shallow    |
|                    |                    | flows at coarse grid cell            |
|                    |                    | resolutions. Note that reducing flow |
|                    |                    | with the Froude number will reduce   |
|                    |                    | the speed of a flood wave moving     |
|                    |                    | downstream.                          |
+--------------------+--------------------+--------------------------------------+
| ``mannings_n``     |                    | A roughness coefficient used by the  |
|                    |                    | flow model. `Values can be looked-up |
|                    |                    | here. <http://www.fsl.orst.edu/geowa |
|                    |                    | ter/FX3/help/8_Hydraulic_Reference/M |
|                    |                    | annings_n_Tables.htm>`__             |
+--------------------+--------------------+--------------------------------------+
| ``hflow_threshold``| metres             | This threshold prevents water being  |
|                    |                    | routed between adjacent cells when   |
|                    |                    | the gradient is incredibly small. A  |
|                    |                    | good default value is 0.00001.       |
+--------------------+--------------------+--------------------------------------+

Table: Guide to the parameter file. Row/parameters can be in any order.
