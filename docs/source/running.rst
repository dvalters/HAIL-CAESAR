Running the Model
=================

The model runs from the command line/terminal/console. You specify the
model executable name (HAIL-CAESAR.exe) followed by the path name to
the parameter file and the parameter file itself. The model will print
out updates to the terminal window regularly, keeping you updated to the
stage it is at and if there are any errors. The DEM of your catchment
must be present in the same folder as your parameter file and must be
correctly formatted.

You need a minimum of three input files:

1. Parameter file

2. DEM file of your catchment (currently only ASCII format is supported,
   sorry ``.bil/flt`` fans!)

3. Rainfall time series text file (There is currently no option to
   generate rainfall within the model, but this is coming soon)

Your input files (DEM etc, parameter file) can be in any folder you
like, as the path to the input files is specified in the parameter file.
This means the executable file can be kept separate, once it has been
compiled.

The model is run like so:

.. code:: console

    ./HAIL-CAESAR.exe [PATH-TO-FOLDER-WITH-INPUT-FILES] ParameterFile.txt

As you can see, the executable takes two arguments, the first is the
path where your parameter file and input files can be found, the second
is the name of your parameter file. Note that the names of the input DEM
and rainfall file are specified in the parameter file.

When the model runs, it will print to screen the parameters that have
been read from the parameter file, for a sanity check. The debug version
prints a lot of other information to screen, to help find bugs and
errors. I suggest turning this off and running with the optimised
version unless you are trying to trace a bug. The program will update
you at certain stages of the data ingestion process. (This usually only
takes a few seconds). When the model runs, a counter displays the number
of elapsed minutes in model-time. (There is an option to turn this off
in the parameter file - set ``debug_print_cycle_on`` to ``no``.

The model also prints out when it is writing output raster files, such
as water depths, elevation difference etc. These files are outputted to
the directory specified in the parameter file.

Outputs
-------

HAIL-CAESAR generates similar outputs to CAESAR-Lisflood, i.e. a
timeseries text file of the water discharge, and sediment fluxes; as
well as raster files for water depths, elevations, erosion amounts, and
so on. These can be outputted at an interval specified in the parameter
file. Output files can be saved in ascii (.asc) form only currently.

DEM preparation
---------------

    **Important**

    You will need to check your DEM is correctly formatted before use.
    LSDCatchmentModel has specific requirements about DEM layout.

Currently, you will have to prepare your own DEM as a separate stage in
the workflow. (Using whichever GIS tool you like, or preferably our own
software!). The DEM should be set up so that one side of the catchment
will act as the flow exit point. If you do not have the intended
catchment outlet point touching one of the DEM edges, you will get
unrealistic pooling of water and flood the entire catchment, as water
will not be able to leave the model domain. **In other words: There
should be no 'NODATA' values between the intended outlet cell(s) and the
edge the DEM file.** This is very important for the model to work
correctly. You may have to rotate your DEM or add on a channel
artificially so that your catchment has a suitable outlet point at one
side of the DEM file.

    **Note**

    The model will actually route water off **all** edges of the
    catchment, if the geometry of your catchment allows it. This might
    be fine for your intended use, but note that the discharge
    timeseries file will report total water discharge and sediment
    output as a total from ALL edge cells, not just the ones you think
    are the main catchment outlet point. As a side effect, you can use
    the model to simulate range scale runoff and multiple catchments,
    just be aware that you will get one value for total discharge for
    the whole DEM.

Technically, the DEM doesn’t need to be pit-filled, but it may be
worthwhile to do so as parts of the model can be sped up when the
catchment is in a low-flow or steady-flow state. Again, it depends on
your intended usage of the model.

Model run time controls
-----------------------

A sample parameter file is provided for the Boscastle floods simulation.
This is a 48-hour simulation using a 5m DEM, over a catchment 3km x
5.5km (about 700000 grid cells). It will take about 2-3 hours to run on
a mid-range Desktop machine. (You can dramatically speed this up by
using a coarser DEM.) Number of domain grid cells is the main control on
compute time. With a multi-core machine, the run time can be
significantly reduced, depending on the number of cores you have.

    **Tip**

    If running in a multi-core environment, you can get this down to around 11
    minutes using a 48-core machine.!

Note that some of the parameters in the paramter file will have an
effect on model run time. For example: **in\_out\_difference**,
**courant\_number**, and many of the minimum threshold values for flow
and erosion can all be tweaked to get model speed up in some way. See
the parameter file guide for advice.
