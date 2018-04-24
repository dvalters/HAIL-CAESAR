/// LSDCatchmentModel.cpp

/*
 * SUMMARY
 *
 * LSDCatchmentModel is a 2.5D numerical model of landscape evolution that
 * simulates the processes and evolution of catchments (river basins) and
 * their hydrological and sedimentological process over timescales of days
 * to thousands of years.
 *
 * Background
 *
 * This is a C++ implementation of the CAESAR-Lisflood model (Coulthard et al, 2013).
 * It is essentially a light, fast, and somewhat stripped-down non-GUI version
 * of the CASESAR-Lisflood model.
 *
 * The main additions where it differs are the inclusion of a separate
 * rainfall-runoff module (see LSDRainfallRunoff.cpp) and a separate grain-size
 * module (see LSDGrainmatrix.cpp)
 *
 * Not all the functionallity of CL is implemented in this model. There is
 * no intention to maintain this code in parallel with CAESAR-Lisflood at this
 * stage. Its development is separate from hereon. It was forked from the
 * CL version 1.8a. (The updates in 1.9 were mirrored here too - DAV 2016)
 *
 * It is integrated with the LSDTopoTools package and makes use of several
 * of the LSDTopoTools objects, such as LSDRaster in particular, for reading
 * and writing raster data to and from the model. You might wish to use some
 * of the topographic analysis tools to analyse your model output.
 *
 * The hydrological component of the model is based on the Bates et al (2010)
 * algorithm of non-steady surface water flow, to represent the variation
 * in hydrological flow in a landscape under non-steady state hydrological
 * inputs. This is different to most Landscape Evolution Models, which tend
 * to use some approximation of steady-state discharge based on contributing
 * drainage area.
 *
 *
 * @author Declan Valters
 * @date  2014-2017
 * @comment 4 years (and counting) of my life developing this model...
 * University of Manchester
 * @contact declan.valters@gmail.com
 * @version 1.0
 *
 * Released under the GNU v3 Public License
 *
 */

#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator> // For the printing vector method
#include <sys/stat.h> // For errors
#include <cstdio> // Only for the debug macro

#include "catchmentmodel/LSDCatchmentModel.hpp"
#include "catchmentmodel/LSDUtils.hpp"

#ifndef LSDCatchmentModel_CPP
#define LSDCatchmentModel_CPP

#define COURANT_FRIEDRICH_LEVY_CONDITION = courant_number * (DX / std::sqrt(gravity * (maxdepth)))

using namespace LSDUtils;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// CREATE FUCNTIONS
// These define what happens when an LSDCatchmentModel object is created
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::create()
{
  std::cout << "You are trying to create an LSDCatchmentModel object with no"
            << "supplied files or parameters." << std::endl
            << "Exiting..." << std::endl;
  exit(EXIT_FAILURE);
}

void LSDCatchmentModel::create(string pname, string pfname)
{
  std::cout << "Creating an instance of LSDCatchmentModel.." << std::endl;
  // Using the parameter file
  initialise_variables(pname, pfname);
  std::cout << "The user-defined parameters have been"
            << " ingested from the param file." << std::endl;
}

void LSDCatchmentModel::initialise_model_domain_extents()
{
  std::string FILENAME = read_path + "/" + read_fname + "." \
                          + dem_read_extension;

  if (!does_file_exist(FILENAME))
  {
    std::cout << "No terrain DEM found by name of: " << FILENAME << std::endl
              << "You must supply a correct path and filename in "
              << "the input parameter file" << std::endl;
    std::cout << "The model domain cannot be intitialised for real topography\n\
                 without a valid DEM to read from" << std::endl;
                 exit(EXIT_FAILURE);
  }
  try
  {
    std::cout << "\n\nLoading DEM header info, the filename is "
              << FILENAME << std::endl;

    // open the data file
    std::ifstream data_in(FILENAME.c_str());

    //Read in raster data
    std::string str;            // a temporary string for discarding text

    // read the georeferencing data and metadata
    data_in >> str >> jmax;
    std::cout << "NCols: " << jmax << " str: " << std::endl;
    data_in >> str >> imax;
    std::cout << "NRows: " << imax << " str: " << std::endl;
    data_in >> str >> xll
            >> str >> yll
            >> str >> DX // cell size or grid resolution
            >> str >> no_data_value;
  }
  catch(...)
  {
    std::cout << "Something is wrong with your initial elevation raster file."
              << std::endl
              << "Common causes are: " << std::endl
              << "1) Data type is not correct"
              << std::endl << "2) Non standard raster format" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "The model domain has been set from "
            << "reading the elevation DEM header." << std::endl;
}

void LSDCatchmentModel::load_data()
{
  LSDRaster elevR;
  /// Hydroindex LSDRaster: tells rainfall input where to be distributed
  LSDRaster hydroindexR;
  /// Bedrock LSDRaster object
  LSDRaster bedrockR;
  std::string DEM_FILENAME = read_path + "/" + read_fname + "." \
                              + dem_read_extension;

  if (!does_file_exist(DEM_FILENAME))
  {
    std::cout << "No terrain DEM found by name of: " << DEM_FILENAME
              << std::endl
              << "You must supply a correct path and filename "
              << "in the input parameter file" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read in the elevation raster data from file, setting the elevation LSDRaster
  // object, 'elevR'
  try
  {
    elevR.read_ascii_raster(DEM_FILENAME);
    // You have now read in all the headers and the raster data
    // Headers are accessed by elevR.get_Ncols(), elevR.get_NRows() etc
    // Raster is accessed by elevR.get_RasterData_dbl() (type: TNT::Array2D<double>)

    // Load the raw ascii raster data
    TNT::Array2D<double> raw_elev = elevR.get_RasterData_dbl();

    // We want an edge pixel of zeros surrounding the raster data
    // So start the counters at one, rather than zero, this
    // will ensure that elev[0][n] is not written to and left set to zero.
    // remember this data member is set with dim size equal to jmax + 2 to
    // allow the border of zeros

    for (unsigned i=0; i<imax; i++)
    {
      for (unsigned j=0; j<jmax; j++)
      {
        elev[i+1][j+1] = raw_elev[i][j];
      }
    }

    // Check that there is an outlet for the catchment water
    check_DEM_edge_condition();

    // deep copy needed? -- DAV 2/12/2015
    init_elevs = elev;

  }
  catch(...)
  {
    std::cout << "Something is wrong with your initial elevation raster file."
              << std::endl
              << "Common causes are: " << std::endl
              << "1) Data type is not correct" << std::endl
              << "2) Non standard raster format" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Load the HYDROINDEX DEM
  if (spatially_var_rainfall == true)
  {
    std::string HYDROINDEX_FILENAME = read_path + "/" + hydroindex_fname;
    // Check for the file first of all
    if (!does_file_exist(HYDROINDEX_FILENAME))
    {
      std::cout << "No hydroindex DEM found by name of: " << HYDROINDEX_FILENAME
                << std::endl
                << "You specified the spatially variable rainfall option, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    try
    {
      hydroindexR.read_ascii_raster_integers(HYDROINDEX_FILENAME);
      TNT::Array2D<int> raw_rfarea = hydroindexR.get_RasterData_int();
      // Solves padding issues
      for (unsigned i=0; i<imax; i++)
      {
        for (unsigned j=0; j<jmax; j++)
        {
          rfarea[i+1][j+1] = raw_rfarea[i][j];
        }
      }

      std::cout << "The hydroindex: " << HYDROINDEX_FILENAME
                << " was successfully read." << std::endl;
    }
    catch (...)
    {
      std::cout << "Something is wrong with your hydroindex file." << std::endl
                << "Common causes are: " << std::endl
                << "1) Data type is not integer" <<
                   std::endl << "2) Non standard ASCII data format" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  std::cout << "The terrain array and supplementary input data has been loaded."
            << std::endl;

  // This has to go after the loading of the hydroindex,
  // otherwise your rfareas will all be 1!
  count_catchment_gridcells();

  // Load the BEDROCK DEM
  if (bedrock_layer_on == true)
  {
    std::string BEDROCK_FILENAME = read_path + "/" + bedrock_data_file;
    // Check for the file first of all
    if (!does_file_exist(BEDROCK_FILENAME))
    {
      std::cout << "No bedrock DEM found by name of: " << BEDROCK_FILENAME
                << std::endl
                << "You specified to use a separate bedrock layer option, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    try
    {
      bedrockR.read_ascii_raster(BEDROCK_FILENAME);
      bedrock = bedrockR.get_RasterData_dbl();
      std::cout << "The bedrock file: " << BEDROCK_FILENAME
                << " was successfully read." << std::endl;
    }
    catch (...)
    {
      std::cout << "Something is wrong with your bedrock file." << std::endl
                << "Common causes are: " << std::endl
                << "1) Data type is not correct" <<
                   std::endl << "2) Non standard ASCII data format" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Load the RAINDATA file
  // Remember the format is not the same as a standard ASCII DEM...
  if (rainfall_data_on==true)
  {
    std::string RAINFALL_FILENAME = read_path + "/" + rainfall_data_file;
    // Check for the file first of all
    if (!does_file_exist(RAINFALL_FILENAME))
    {
      std::cout << "No rainfall data file found by name of: "
                << RAINFALL_FILENAME << std::endl
                << "You specified to use a rainfall input file, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    std::cout << "Ingesting rainfall data file: " << RAINFALL_FILENAME
              << " into hourly_rain_data" << std::endl;
    hourly_rain_data = read_rainfalldata(RAINFALL_FILENAME);

    // debug
    #ifdef DEBUG
    print_rainfall_data();
    #endif

  }
  // Read grainsize data for restart runs
  if (graindata_from_file == true)
  {
    std::string GRAINDATA_FILENAME = read_path + "/" + grain_data_file;
    if (does_file_exist(GRAINDATA_FILENAME))
    {
      ingest_graindata_from_file(GRAINDATA_FILENAME);
    }
    else
    {
      std::cout << "No grain data file found by name of: "
                << GRAINDATA_FILENAME << std::endl
                << "You specified to use a graindata input file,"
                << "\n but no matching file was found. Try again." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

// Reads in grain data from the grain data file,
// and initialises the grain_array_tot variable,
// the index, grain, and strata arrays
void LSDCatchmentModel::ingest_graindata_from_file(
  std::string GRAINDATA_FILENAME)
{
  std::cout << "\n Loading graindata from the the graindata file: "
            << GRAINDATA_FILENAME << std::endl;

  // open the grain data file
  // Note that c_str() will return a const char* whereas,
  // strip function expects char*...
  std::ifstream infile(GRAINDATA_FILENAME);

  // Index coordinates
  unsigned x1=0, y1=0;

  grain_array_tot = 0;

  std::string line;

  // read a line at a time from infile
  while(std::getline(infile, line))
  {
    // check not an empty string (getline can return '' if
    // first character of line is \n or similar)

    // A string to hold each line of the text file as we iterate
    std::vector<std::string> line_vector;
    // Strip using the function in LSDStatsTools
    split_delimited_string(line, ' ', line_vector);

    unsigned col_counter = 1;
    grain_array_tot++;

    for (unsigned x=0; x<=line_vector.size()-1; x++ )
    {
      //std::cout << "LINE VECTOR IS: " << line_vector[x] << std::endl;
      if (col_counter==1) x1 = std::stoi(line_vector[x]);
      if (col_counter==2) y1 = std::stoi(line_vector[x]);

      // Prevent grains being added that are outside the grid.
      if (x1 > imax) x1 = imax;
      if (y1 > jmax) y1 = jmax;

      if (col_counter == 3)
      {
        index[x1][y1] = grain_array_tot;
      }

      // Next bunch of columns are grain fractions (surface). Update them.
      for(unsigned n=0; n<=G_MAX; n++)
      {
        if (col_counter==4+n)
        {
          grain[grain_array_tot][n] = std::stod(line_vector[x]);
        }
      }

      // Now the fractions for the subsuface strata,
      // note that this is currently hard coded as 10 layers
      // so when you update to have user-defined no. of strata, recompile
      for(int z=0; z<=9; z++)
      {
        for (unsigned n=0; n<=(G_MAX-2); n++)
        {
          if (col_counter == (4+G_MAX+n+1) + (z*9))
          {
            strata[grain_array_tot][z][n] = std::stod(line_vector[x]);
          }
        }
      }
      col_counter++; // move on to the next column (this seems really inefficient...)
    }
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Load the rainfall data from the rainfall text file
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Generic function for reading rainfal data
// This is used by the LSDCatchmentModel.
std::vector< std::vector<float> > LSDCatchmentModel::read_rainfalldata(
  string FILENAME)
{
  std::cout << "\n\n Loading Spatially Distributed Rainfall File, \
                the filename is: "
            << FILENAME << std::endl;

  // open the data file
  std::ifstream infile(FILENAME.c_str());

  std::string line;
  int i = 0;

  while (std::getline(infile, line))
  {
    float value;
    std::stringstream ss(line);

    raingrid.push_back(std::vector<float>());

    while (ss >> value)
    {
      raingrid[i].push_back(value);
    }
    ++i;
  }
  return raingrid;
}

// This is just for sanity checking the rainfall input really
void LSDCatchmentModel::print_rainfall_data()
{
  std::vector< std::vector<float> > vector2d = hourly_rain_data;
  std::vector<std::vector<float> >::iterator itr = vector2d.begin();
  std::vector<std::vector<float> >::iterator end = vector2d.end();

  while (itr!=end)
  {
    std::vector<float>::iterator it1=itr->begin(),end1=itr->end();
    std::copy(it1,end1,std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
    ++itr;
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function gets all the data from a parameter file
//
// Update: It also intialises the other params that are set internally
// Some functions have been taken out of mainloop()
//
// DAV - this is a bit of a clunky method - perhaps replace it with the
//   paramter ingestion method used in CHILD one day?
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::initialise_variables(std::string pname,
                                             std::string pfname)
{
  std::cout << "Initialising the model parameters..." << std::endl;
  // Concatenate the path and paramter file name to get the full file address
  string full_name = pname+pfname;

  std::ifstream infile;
  // Open the parameter file
  infile.open(full_name.c_str());
  string parameter, value, lower, lower_val;
  string bc;

  std::cout << "Parameter filename is: " << full_name << std::endl;

  // now ingest parameters
  while (infile.good())
  {
    parse_line(infile, parameter, value);
    lower = parameter;
    if (parameter == "NULL")
      continue;
    for (unsigned int i=0; i<parameter.length(); ++i)
    {
      lower[i] = std::tolower(parameter[i]);  // converts to lowercase
    }

    // get rid of control characters
    value = RemoveControlCharactersFromEndOfString(value);

    if (lower == "dem_read_extension")
    {
      dem_read_extension = value;
      dem_read_extension = RemoveControlCharactersFromEndOfString(
                              dem_read_extension);
      std::cout << "dem_read_extension: " << dem_read_extension << std::endl;
    }
    else if (lower == "dem_write_extension")
    {
      dem_write_extension = value;
      dem_write_extension = RemoveControlCharactersFromEndOfString(
                              dem_write_extension);
      std::cout << "dem_write_extension: " << dem_write_extension << std::endl;
    }
    else if (lower == "write_path")
    {
      write_path = value;
      write_path = RemoveControlCharactersFromEndOfString(write_path);
      std::cout << "output write path: " << write_path << std::endl;
    }
    else if (lower == "write_fname")
    {
      write_fname = value;
      write_fname = RemoveControlCharactersFromEndOfString(write_fname);
      std::cout << "write_fname: " << write_fname << std::endl;
    }
    else if (lower == "read_path")
    {
      read_path = value;
      read_path = RemoveControlCharactersFromEndOfString(read_path);
      std::cout << "read_path: " << read_path << std::endl;
    }
    else if (lower == "read_fname")
    {
      read_fname = value;
      read_fname = RemoveControlCharactersFromEndOfString(read_fname);
      std::cout << "read_fname: " << read_fname << std::endl;
    }

    //=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // parameters for landscape numerical modelling
    // (LSDCatchmentModel: DAV 2015-01-14)
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Supplementary Input Files
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "hydroindex_file")
    {
      hydroindex_fname = value;
      std::cout << "hydroindex_file: " << hydroindex_fname << std::endl;
    }
    else if (lower == "rainfall_data_file")
    {
      rainfall_data_file = value;
      std::cout << "rainfall_data_file: " << rainfall_data_file << std::endl;
    }
    else if (lower == "grain_data_file")
    {
      grain_data_file = value;
      std::cout << "grain data file: " << grain_data_file << std::endl;
    }
    else if (lower == "bedrock_data_file")
    {
      bedrock_data_file = value;
      std::cout << "bedrock data file: " << bedrock_data_file << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Numerical
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "no_of_iterations")
    {
      no_of_iterations = atoi(value.c_str());
      std::cout << "no of iterations: " << no_of_iterations << std::endl;
    }
    else if (lower == "min_time_step")
    {
      min_time_step = atoi(value.c_str());
      std::cout << "min time step: " << min_time_step << std::endl;
    }
    else if (lower == "max_time_step")
    {
      max_time_step = atoi(value.c_str());
      std::cout << "max time step: " << max_time_step << std::endl;
    }
    else if (lower == "run_time_start")
    {
      run_time_start = atof(value.c_str()); //?
      std::cout << "run time start: " << run_time_start << std::endl;
    }
    else if (lower == "max_run_duration")
    {
      maxcycle = atoi(value.c_str()); //?
      std::cout << "max_run_duration: " << maxcycle << std::endl;
    }
    else if (lower == "memory_limit")
    {
      LIMIT = atoi(value.c_str());
      std::cout << "memory LIMIT: " << LIMIT << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Output and Save Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "timeseries_save_interval")
    {
      output_file_save_interval = atoi(value.c_str());
      std::cout << "timeseries save interval: " << output_file_save_interval << std::endl;
    }
    else if (lower == "raster_output_interval")
    {
      saveinterval = atof(value.c_str());
      std::cout << "raster_output_interval: " << saveinterval << std::endl;
    }
    else if (lower == "write_elevation_file")
    {
      elev_fname = value;
      std::cout << "write_elev_fname: " << elev_fname << std::endl;
    }
    else if (lower == "write_elev_file")
    {
      write_elev_file = (value == "yes") ? true : false;
      std::cout << "write_elev_file_on: " << write_elev_file << std::endl;
    }
    else if (lower == "grainsize_file")
    {
      grainsize_fname = value;
      std::cout << "grainsize_fname: " << grainsize_fname << std::endl;
    }
    else if (lower == "write_grainsize_file")
    {
      write_grainsz_file = (value == "yes") ? true : false;
      std::cout << "write_grainsz_file: " << write_grainsz_file << std::endl;
    }
    else if (lower == "parameters_file")
    {
      params_fname = value;
      std::cout << "params_fname: " << params_fname << std::endl;
    }
    else if (lower == "write_parameters_file")
    {
      write_params_file = (value == "yes") ? true : false;
      std::cout << "write_params_file: " << write_params_file << std::endl;
    }
    else if (lower == "flowvelocity_file")
    {
      flowvel_fname = value;
      std::cout << "flowvel_fname: " << flowvel_fname << std::endl;
    }
    else if (lower == "write_flowvelocity_file")
    {
      write_flowvel_file = (value == "yes") ? true : false;
      std::cout << "write_flowvel_file: " << write_flowvel_file << std::endl;
    }
    else if (lower == "waterdepth_outfile_name")
    {
      waterdepth_fname = value;
      std::cout << "waterdepth_outfile_name: " << waterdepth_fname << std::endl;
    }
    else if (lower == "write_waterdepth_file")
    {
      write_waterd_file = (value == "yes") ? true : false;
      std::cout << "write_waterd_file: " << write_waterd_file << std::endl;
    }
    else if (lower == "timeseries_file")
    {
      timeseries_fname = value;
      std::cout << "timeseries_fname: " << timeseries_fname << std::endl;
    }
    else if (lower == "elevdiff_outfile_name")
    {
      elevdiff_fname = value;
      std::cout << "elevdiff_outfile_name: " << elevdiff_fname << std::endl;
    }
    else if (lower == "write_elevdiff_file")
    {
      write_elevdiff_file = (value == "yes") ? true : false;
      std::cout << "write_elevdiff_file_on: " << write_elevdiff_file
                << std::endl;
    }
    else if (lower == "raingrid_fname_out")
    {
      raingrid_fname = value;
      std::cout << "raingrid_outfile_name: " << raingrid_fname << std::endl;
    }
    else if (lower == "runoffgrid_fname")
    {
      runoffgrid_fname = value;
      std::cout << "runoffgrid_outfile_name: " << runoffgrid_fname << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Sediment
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "read_in_graindata_from_file")
    {
      graindata_from_file = (value == "yes") ? true : false;
      std::cout << "read in grain data from file: " << graindata_from_file
                << std::endl;
    }

    else if (lower == "bedrock_layer_on")
    {
      bedrock_layer_on = (value == "yes") ? true : false;
      std::cout << "bedrock_layer_on: " << bedrock_layer_on << std::endl;
    }
    else if (lower == "transport_law")
    {
      if (value == "wilcock")
      {
        wilcock = true;
        einstein = false;
      }
      else if (value == "einstein")
      {
        einstein = true;
        wilcock = false;
      }
      else
      {
        std::cout << "WARNING!: No sediment transport law "
                  << "specified in parameter file..." << std::endl;
        std::cout << "You must specify a transport law: "
                  << "either 'einstein' or 'wilcock'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        exit(EXIT_FAILURE);
      }

      std::cout << "wilcock: " << wilcock << std::endl;
      std::cout << "einstein: " << einstein << std::endl;
    }

    else if (lower == "max_tau_velocity")
    {
      max_vel = atof(value.c_str());
      std::cout << "max_vel (to calculate tau): " << max_vel << std::endl;
    }
    // max velocity used to calculate tau
    else if (lower == "active_layer_thickness")
    {
      active = atof(value.c_str());
      std::cout << "active: " << active << std::endl;
    }
    else if (lower == "recirculate_proportion")
    {
      recirculate_proportion = atof(value.c_str());
      std::cout << "recirculate_proportion: " << recirculate_proportion
                << std::endl;
    }
    else if (lower == "chann_lateral_erosion")
    {
      chann_lateral_erosion = atof(value.c_str());
      std::cout << "In channel lateral erosion proportion: "
                << chann_lateral_erosion << std::endl;
    }
    else if (lower == "erosion_limit")
    {
      ERODEFACTOR = atof(value.c_str());
      std::cout << "erosion limit per timestep: " << ERODEFACTOR << std::endl;
    }

    /// LATERAL EROSION ROUTINE PARAMETERS
    else if (lower == "lateral_erosion_on")
    {
      lateral_erosion_on = (value == "yes") ? true : false;
      std::cout << "lateral_erosion_on: " << lateral_erosion_on << std::endl;
    }
    else if (lower == "edge_smoothing_passes")
    {
      edge_smoothing_passes = atof(value.c_str());
      std::cout << "smoothing_times: " << edge_smoothing_passes << std::endl;
    }
    else if (lower == "lateral_erosion_const")
    {
      lateral_constant = atof(value.c_str());
      std::cout << "Lateral erosion constant: " << lateral_constant
                << std::endl;
    }
    else if (lower == "downstream_cell_shift")
    {
      downstream_shift = atof(value.c_str());
      std::cout << "downstream_shift: " << downstream_shift << std::endl;
    }
    else if (lower == "lateral_cross_chan_smoothing")
    {
      lateral_cross_channel_smoothing = atof(value.c_str());
      std::cout << "lateral_cross_channel_smoothing: "
                << lateral_cross_channel_smoothing << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Grain Size Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "suspended_sediment_on")
    {
      suspended_opt = (value == "yes") ? true : false;
      std::cout << "suspended_opt: " << suspended_opt << std::endl;
    }

    else if (lower == "grain_size_frac_file")
    {
      grain_data_file = value;
      std::cout << "grain_data_file: " << grain_data_file << std::endl;
    }
    //=-=-=-=-=-=-=-=-=-=-=-=
    // Bedrock Erosion
    //=-=-=-=-=-=-=-=-=-=-=-=

    else if (lower == "bedrock_erosion_threshold")
    {
      bedrock_erosion_threshold = atof(value.c_str());
      std::cout << "bedrock erode threshold (Pa): "
                << bedrock_erosion_threshold << std::endl;
    }
    else if (lower == "stream_power_pb")
    {
      p_b = atof(value.c_str());
      std::cout << "bedrock stream power p_b: " << p_b << std::endl;
    }
    else if (lower == "stream_power_ke")
    {
      bedrock_erodibility_coeff_ke = atof(value.c_str());
      std::cout << "bedrock stream power k_e: " << bedrock_erodibility_coeff_ke
                << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Hydrology and Flow
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "hydro_model_only")
    {
      hydro_only = (value == "yes") ? true : false;
      std::cout << "run hydro model only (NO EROSION): "
                << hydro_only << std::endl;
    }

    else if (lower == "rainfall_data_on")
    {
      rainfall_data_on = (value == "yes") ? true : false;
      std::cout << "rainfall_data_on: " << rainfall_data_on << std::endl;
    }

    else if (lower == "topmodel_m_value")
    {
      M = atof(value.c_str());
      std::cout << "topmodel m value: " << M << std::endl;
    }

    else if (lower == "num_unique_rain_cells")
    {
      rfnum = atoi(value.c_str());
      std::cout << "Number of unique rain cells: " << rfnum << std::endl;
    }

    else if (lower == "rain_data_time_step")
    {
      rain_data_time_step = atof(value.c_str());
      std::cout << "rainfall data time step: "
                << rain_data_time_step << std::endl;
    }

    else if (lower == "spatial_var_rain")
    {
      spatially_var_rainfall = (value == "yes") ? true : false;
      std::cout << "Spatially variable rainfall: "
                << spatially_var_rainfall << std::endl;
    }

    else if (lower == "in_out_difference")
    {
      in_out_difference_allowed = atof(value.c_str());
      std::cout << "in-output difference allowed (cumecs): "
                << in_out_difference_allowed << std::endl;
    }

    else if (lower == "min_q_for_depth_calc")
    {
      MIN_Q = atof(value.c_str());
      std::cout << "minimum discharge for depth calculation: "
                << MIN_Q << std::endl;
    }

    else if (lower == "max_q_for_depth_calc")
    {
      MIN_Q_MAXVAL = atof(value.c_str());
      std::cout << "max discharge for depth calc: "
                << MIN_Q_MAXVAL << std::endl;
    }

    else if (lower == "hflow_threshold")
    {
      hflow_threshold = atof(value.c_str());
      std::cout << "Horizontal flow threshold: "
                << hflow_threshold << std::endl;
    }

    else if (lower == "water_depth_erosion_threshold")
    {
      hflow_threshold = atof(value.c_str());
      std::cout << "Water depth for erosion threshold: "
                << water_depth_erosion_threshold << std::endl;
    }

    else if (lower == "slope_on_edge_cell")
    {
      edgeslope = atof(value.c_str());
      std::cout << "Slope on model domain edge: "
                << edgeslope << std::endl;
    }

    else if (lower == "evaporation_rate")
    {
      k_evap = atof(value.c_str());
      std::cout << "Evaporation rate: " << k_evap << std::endl;
    }

    else if (lower == "courant_number")
    {
      courant_number = atof(value.c_str());
      std::cout << "Courant number: " << courant_number << std::endl;
    }

    else if (lower == "froude_num_limit")
    {
      froude_limit = atof(value.c_str());
      std::cout << "Froude number limit: " << froude_limit << std::endl;
    }

    else if (lower == "mannings_n")
    {
      mannings = atof(value.c_str());
      std::cout << "mannings: " << mannings << std::endl;
    }
    else if (lower == "spatially_complex_rainfall_on")
    {
      spatially_complex_rainfall = (value == "yes") ? true : false;
      std::cout << "Spatially complex rainfall option: "
                << spatially_complex_rainfall << std::endl;
    }
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Vegetation
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "vegetation_on")
    {
      vegetation_on = (value == "yes") ? true : false;
      std::cout << "Grow veggies: " << vegetation_on << std::endl;
    }

    else if (lower == "grass_grow_rate")
    {
      grow_grass_time = atof(value.c_str());
      std::cout << "Veggie grow rate: " << grow_grass_time << std::endl;
    }

    else if (lower == "vegetation_crit_shear")
    {
      vegTauCrit = atof(value.c_str());
      std::cout << "Vegggie crit shear: " << vegTauCrit << std::endl;
    }

    else if (lower == "veg_erosion_prop")
    {
      veg_lat_restriction = atof(value.c_str());
      std::cout << "Veggie erosion proportion: "
                << veg_lat_restriction << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Hillslope
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "creep_rate")
    {
      CREEP_RATE = atof(value.c_str());
      std::cout << "Hillslope creep rate: " << CREEP_RATE << std::endl;
    }

    else if (lower == "slope_failure_thresh")
    {
      failureangle = atof(value.c_str());
      std::cout << "Slope critical failure angle: "
                << failureangle << std::endl;
    }

    else if (lower == "soil_erosion_rate")
    {
      SOIL_RATE = atof(value.c_str());
      std::cout << "Soil erosion rate: " << SOIL_RATE << std::endl;
    }

    else if (lower == "soil_j_mean_depends_on")
    {
      soil_j_mean_depends_on = (value == "yes") ? true:false;
      std::cout << "Soil erosion rate depends on J mean: "
                << soil_j_mean_depends_on << std::endl;
    }

    // Debugging
    else if (lower == "debug_print_cycle")
    {
      DEBUG_print_cycle_on = (value == "yes") ? true:false;
      std::cout << "Will print current cycle to screen: "
                << DEBUG_print_cycle_on << std::endl;
    }
    else if (lower == "debug_write_raingrid")
    {
      DEBUG_write_raingrid = (value == "yes") ? true:false;
      std::cout << "Will write raingrid raster every calc_J: "
                << DEBUG_write_raingrid << std::endl;
    }
    else if (lower == "debug_write_runoffgrid")
    {
      DEBUG_write_runoffgrid = (value == "yes") ? true:false;
      std::cout << "Will write runoff grid to file for debugging: "
                << DEBUG_write_runoffgrid << std::endl;
    }

  }

  std::cout << "No other parameters found, paramter ingestion complete."
            << std::endl;
  std::cout << "Initialising hard coded-constants." << std::endl;

  // Initialise the other parameters (those not set by param file)
  tx = output_file_save_interval;

  if (spatially_var_rainfall == false)
  {
    std::cout << "Making sure no of rain cells is set to 1, "
              << "for uniform rainfall input.."
              << std::endl;
    rfnum = 1;
  }

}
// Initialise the arrays (as done in initialise() )
// Not sure the point of having them declared in header file if you
// can't resize them...surely this is duplicating array creation?? DV
// TO DO DAV - address above comment

// Initialise the relevant arrays
void LSDCatchmentModel::initialise_arrays()
{
  std::cout << "Cartesian imax (no. of rows): " << imax << \
               " Cartesian jmax (no. of cols): " << jmax << std::endl;

  // Need to change this so it does not waste memory assigning arrays
  // when running in hydro mode etc.
  elev = TNT::Array2D<double> (imax+2,jmax+2, -9999);
  water_depth = TNT::Array2D<double> (imax+2,jmax+2, 0.0);

  // Cast to int and then double, what?
  //old_j_mean_store = new double[(int)((maxcycle*60)/input_time_step)+10];
  old_j_mean_store = std::vector<double>
    (static_cast<int>((maxcycle*60)/input_time_step)+10);

  qx = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);
  qy = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);

  qxs = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);
  qys = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);

  Vel = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);

  area = TNT::Array2D<double> (imax+2, jmax + 2, 0.0);
  index = TNT::Array2D<int> (imax +2, jmax + 2, 0);
  elev_diff = TNT::Array2D<double> (imax + 2, jmax + 2);

  bedrock = TNT::Array2D<double> (imax+2, jmax+2, -9999);
  tempcreep = TNT::Array2D<double> (imax+2,jmax+2);
  init_elevs = TNT::Array2D<double> (imax+2,jmax+2, -9999);

  vel_dir = TNT::Array3D<double> (imax+2, jmax+2, 9, 0.0);

  Vsusptot = TNT::Array2D<double> (imax+2,jmax+2, 0.0);

  // Will come back to this later - DAV
  //if (vegetation_on)
  //{
    veg = TNT::Array3D<double> (imax+1, jmax+1, 4, 0.0);
  //}


  //cross_scan = TNT::Array2D<int> (imax+2,jmax+2, 0);
  down_scan = TNT::Array2D<int> (jmax+2, imax+2, 0);

  // line to stop max time step being greater than rain time step
  if (rain_data_time_step < 1) rain_data_time_step = 1;
  if (max_time_step / 60 > rain_data_time_step)
  {
    max_time_step = static_cast<int>(rain_data_time_step) * 60;
  }
  // see StackOverflow for how to set size of nested vector
  // http://stackoverflow.com/questions/2665936/
  //     is-there-a-way-to-specify-the-dimensions-of-a-nested-stl-vector-c
  // DEBUG

  #ifdef DEBUG
  long int xdim = static_cast<int>(maxcycle * (60 / rain_data_time_step)) + 100;
  std::cout << "Size of rain vector: " << xdim << std::endl;

  int short_xdim = static_cast<int>(maxcycle * (60 / rain_data_time_step)) \
                    + 100;
  std::cout << "Int size of rain vector: " << short_xdim << std::endl;
  #endif

  hourly_rain_data = std::vector< std::vector<float> >
    ( (static_cast<int>(maxcycle * (60 / rain_data_time_step)) + 100),
                        vector<float>(rfnum+1) );

  hourly_m_value = std::vector<double>
    (static_cast<int>(maxcycle * (60 / rain_data_time_step)) + 100);

  inputpointsarray = TNT::Array2D <bool> (imax + 2, jmax + 2);

  edge = TNT::Array2D<double> (imax+1,jmax+1, 0.0);
  edge2 = TNT::Array2D<double> (imax+1,jmax+1, 0.0);

  Tau = TNT::Array2D<double> (imax+2,jmax+2, 0.0);

  catchment_input_x_coord = std::vector<int> (jmax * imax, 0);
  catchment_input_y_coord = std::vector<int> (jmax * imax, 0);

  area_depth = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);

  // Grain Arrays
  sum_grain = std::vector<double> (G_MAX+1);
  sum_grain2 = std::vector<double> (G_MAX + 1);
  old_sum_grain = std::vector<double> (G_MAX+1);
  old_sum_grain2 = std::vector<double> (G_MAX + 1);
  Qg_step = std::vector<double> (G_MAX+1);
  Qg_hour = std::vector<double> (G_MAX+1);
  Qg_over = std::vector<double> (G_MAX+1);
  Qg_last = std::vector<double> (G_MAX+1);
  Qg_step2 = std::vector<double> (G_MAX + 1);
  Qg_hour2 = std::vector<double> (G_MAX + 1);
  Qg_over2 = std::vector<double> (G_MAX + 1);
  Qg_last2 = std::vector<double> (G_MAX + 1);

  // Distributed Hydrological Model Arrays
  // j and jo need a very small initial value to get them going
  j = std::vector<double> (rfnum + 1, 0.000000001);
  jo = std::vector<double> (rfnum + 1, 0.000000001);
  j_mean = std::vector<double> (rfnum + 1);
  // std::vectors will be default initalised to 0, unless specified
  old_j_mean = std::vector<double> (rfnum + 1);
  new_j_mean = std::vector<double> (rfnum + 1);
  rfarea = TNT::Array2D<int> (imax + 2, jmax + 2, 0);
  nActualGridCells = std::vector<int> (rfnum + 1);
  catchment_input_counter = std::vector<int> (rfnum + 1);

  // Only need these ones for erosion-enabled simulation runs
  if (!hydro_only)
  {
    sr = TNT::Array3D<double> (imax + 2, jmax + 2, 10, 0.0);
    sl = TNT::Array3D<double> (imax + 2, jmax + 2, 10, 0.0);
    su = TNT::Array3D<double> (imax + 2, jmax + 2, 10, 0.0);
    sd = TNT::Array3D<double> (imax + 2, jmax + 2, 10, 0.0);
    ss = TNT::Array2D<double> (imax + 2, jmax + 2, 0.0);

    strata = TNT::Array3D<double> ( ((imax+2)*(jmax+2))/LIMIT , 10, G_MAX+1, 0.0);
    grain = TNT::Array2D<double> ( ((2+imax)*(jmax+2))/LIMIT, G_MAX+1 , 0.0);
    temp_grain = std::vector<double> (G_MAX+1, 0.0);
  }
  // Initialise suspended fraction vector
  // Tells program whether sediment is suspended fraction or not
  isSuspended = std::vector<bool>(G_MAX+1, false);
  fallVelocity = std::vector<double>(G_MAX+1, 0.0);
  set_fall_velocities();
  // Not entirely sure this is necessary? - TODO DAV
  zero_values();
}

// Hard coded! - Should make this read from sediment details file.
// DAV 3/12/2015
// TODO Allow user to set these values.
void LSDCatchmentModel::set_fall_velocities()
{
  fallVelocity[0] = 0.0;
  fallVelocity[1] = 0.033;
  fallVelocity[2] = 0.109;
  fallVelocity[3] = 0.164;
  fallVelocity[4] = 0.237;
  fallVelocity[5] = 0.338;
  fallVelocity[6] = 0.479;
  fallVelocity[7] = 0.678;
  fallVelocity[8] = 0.959;
  fallVelocity[9] = 1.357;
}

void LSDCatchmentModel::set_time_counters()
{
  save_time = cycle;
  creep_time = cycle;
  creep_time2 = cycle;
  grass_grow_interval = cycle;
  soil_erosion_time = cycle;
  soil_development_time = cycle;
  time_1 = cycle;
}

void LSDCatchmentModel::set_loop_cycle()
{
  previous = cycle;
  old_cycle = std::fmod(cycle, output_file_save_interval);
}

void LSDCatchmentModel::set_inputoutput_diff()
{
  input_output_difference = std::abs(waterinput - waterOut);
}

double LSDCatchmentModel::courant_friedrichs_lewy_condition()
{
  return courant_number * (DX / std::sqrt(gravity * (maxdepth)))
}

// Sets the maximum timestep for the current iteration
void LSDCatchmentModel::set_maximum_timestep()
{  
  if (maxdepth <= 0.1)
  {
    maxdepth = 0.1;
  }
  // The max time factor should not be less than the maximum prescribed
  // by the Courant Friedrich Levy Condition (CFL)
  if (time_step < courant_friedrichs_lewy_condition())
  {
    time_step = courant_friedrichs_lewy_condition();
  }
  // If the water input/output difference threshold is exceeded, AND the max time
  // step exceeds the CFL (Courant Friedrich Levy) condition, then reduce the 
  // time factor to the CFL-prescribed value.
  if (input_output_difference > in_out_difference_allowed && time_step > \
     (courant_friedrichs_lewy_condition())
  {
    time_step = courant_friedrichs_lewy_condition();
  }
}

double LSDCatchmentModel::set_local_timestep()
{
  // Start with the maximum time factor
  double local_time_factor = time_step;
  // Scale it back to the CFL condition if it exceeds CFL.
  if (local_time_factor > courant_friedrichs_lewy_condition())
  {
    local_time_factor = courant_friedrichs_lewy_condition();
  }
  return local_time_factor;
}

void LSDCatchmentModel::increment_counters()
{
  counter++;
  cycle += time_step / 60;
  // cycle is minutes, time_step is seconds
  // cycle is the actual simulated time elapsed in the model
  // counter is the iteration number
  new_cycle = std::fmod(cycle, output_file_save_interval);
}

void LSDCatchmentModel::save_raster_output()
{
  if (cycle >= save_time)
  {
    save_raster_data(std::abs(cycle));
    save_time += saveinterval;
  }
}

void LSDCatchmentModel::print_cycle()
{
  if (DEBUG_print_cycle_on==true)
  {
  std::cout << "Cycle: " << cycle << "                  \r" << std::flush;
  }
}

void LSDCatchmentModel::write_output_timeseries(runoffGrid& runoff)
{
  temptotal = temptot;

  if (spatially_complex_rainfall ==true)
  {
    // uses the runoff object
    output_data(temptotal, runoff);
  }
  else
  {
    // uses the global array
    output_data();
  }
}

void LSDCatchmentModel::call_channel_landsliding(int in_channel_landslide_iter_interval)
{
  if (!hydro_only && ((counter % in_channel_landslide_iter_interval) == 0))
  {
    in_channel_landsliding();
  }
}

void LSDCatchmentModel::slope_creep(int creep_time_interval_days,
                                    double creep_coeff)
{
  if (!hydro_only && (cycle > creep_time))
  {
    creep_time += creep_time_interval_days; // Add 10 days
    creep(creep_coeff);  // Make this number user selectable
  }
}

void LSDCatchmentModel::call_global_landsliding(
  int global_landsliding_interval_hours)
{
  if (!hydro_only && cycle > creep_time2)
  {
    // evaporate(1440);
    creep_time2 += global_landsliding_interval_hours; // Add 1 day in hours
    // TODO DAV - This is a stupid name:
    global_landsliding();
  }
}

void LSDCatchmentModel::grow_vegetation(int vegetation_growth_interval_hours)
{
  if (!hydro_only && vegetation_on && (cycle > grass_grow_interval))
  {
    // Add 1 day in hours
    grass_grow_interval += vegetation_growth_interval_hours;
    grow_grass(1 / (grow_grass_time * 365));
  }
}

void LSDCatchmentModel::check_wetted_area(int scan_area_interval_iter)
{
  if ((counter % scan_area_interval_iter) == 0)
  {
    scan_area();
  }
}

void LSDCatchmentModel::catchment_waterinputs(runoffGrid& runoff)
{
  waterinput = 0;
  double local_time_factor = set_local_timefactor();
  if (spatially_complex_rainfall == true)
  {
    catchment_water_input_and_hydrology(local_time_factor, runoff);
  }
  else
  {
    catchment_water_input_and_hydrology(local_time_factor);
  }
}

void LSDCatchmentModel::initialise_rainfall_runoff(runoffGrid& runoff)
{
  std::cout << "Initialising rainfall runoff for first time..." << std::endl;
  if (spatially_complex_rainfall == true)
  {
    // Create a runoff object same size as model domains
    topmodel_runoff(1.0, runoff);
  }
  else
  {
    topmodel_runoff(1.0);
  }
}

void LSDCatchmentModel::initialise_drainage_area()
{
  std::cout << "Initialising drainage area for first time..." << std::endl;
  // calculate the contributing drainage area

  // There's no need to intitialise the wetted area or catchment input
  // points for the distributed topmodel case, since every cell is checked.
  if (spatially_complex_rainfall == false)
  {
  zero_and_calc_drainage_area();
  // is this needed for spatially complex case? no, but soil erosion uses it.
  std::cout << "Initialising catchment input points for first time..." << std::endl;
  get_catchment_input_points();
  }
}

void LSDCatchmentModel::zero_and_calc_drainage_area()
{
  // Zeros the area and are-depth arrays
  // Sets area_depth to ones as a flag but
  // zeros the ones outisde the catchment
  for(unsigned i=1; i<=imax; i++)
  {
    for(unsigned j=1; j<=jmax; j++)
    {
      area_depth[i][j]=1;
      area[i][j] = 0;
      if (elev[i][j] == -9999)
      {
        area_depth[i][j] = 0.0;
      }
    }
  }
  drainage_area_D8();   // This refers to a newer area getting method in CaesarLisflood
}

void LSDCatchmentModel::drainage_area_D8()
{
  // new routine for determining drainage area 4/10/2010
  // instead of using sweeps this sorts all the elevations then works frmo the
  // highest to lowest - calculating drainage area - D-infinity basically.

  // zero load of arrays
  std::vector<double> tempvalues((imax+2) *(jmax+2));
  std::vector<double> tempvalues2((imax+2) * (jmax+2));
  std::vector<double> xkey((imax+2)*(jmax+2));
  std::vector<double> ykey((imax+2)*(jmax+2));

  // then create temp array based on elevs then also one for x values.
  int inc = 1;
  for (unsigned i = 1; i <= imax; i++)
  {
    for (unsigned j = 1; j <= jmax; j++)
    {
      // tempvalues is just a list of all the elevations in the grid.
      tempvalues[inc] = elev[i][j];  // bad name choice 'temp'! TO DO: DAV change later
      // xkey is the xcoordinates collated
      xkey[inc] = i; // because [row][column] order, so j is the x-coord
      inc++;
    }
  }

  // then sorts according to elevations - but also sorts the key (xkey) according to
  // these too..

  // Comment about the original CAESAR-Lisflood code
  // DAV note 09/01/2014 This seems strange at first because the C# syntax is to have
  // keys first, then values:
  // Array.sort(keys,values) but here it is Array.sort(values, keys) which seems like
  // a mistake at first.
  // So in effect this is *sorting the keys* (1,2,3,4,5,...,n) **according to the
  // elevations**.
  //Array.Sort(tempvalues, xkey); (Only available in the C# libraries)

  // DAV 2014-01-12
  // Written up a template that should roughly replicate the behaviour
  // Template comparator will function on a std::pair, so you need to compress
  // the two vectors into a std::pair object first. std::sort is then used with the
  // comparator, (the template): sort_pair_second. As the name indicates, it sorts
  // std::pair objects by the second value in the pair. (You could change this if
  // needed. - thank you stackoverflow!

  // Pair the vectors up
  std::vector<std::pair<double,double> >
      x_keys_elevs_paired(
        xkey.size() < tempvalues.size() ? xkey.size() : tempvalues.size() );

  for (unsigned k=0; k < x_keys_elevs_paired.size(); k++)
  {
    x_keys_elevs_paired[k] = std::make_pair(xkey[k],tempvalues[k]);
  }

  // Now do the sort
  std::sort(x_keys_elevs_paired.begin(),
            x_keys_elevs_paired.end(),
            sort_pair_second<double,double>() );

  // now does the same for y values, creating a temporary array for the ykeys etc.
  inc = 1;
  for (unsigned i = 1; i <= imax; i++)
  {
    for (unsigned j = 1; j <= jmax; j++)
    {
      tempvalues2[inc] = elev[i][j];
      ykey[inc] = j;  // row-major, so i is the y coordinate (the row of NRows)
      inc++;
    }
  }
  // Create a suitably sized vector to hold the pairs
  std::vector<std::pair<double,double> >
      y_keys_elevs_paired(
        ykey.size() < tempvalues2.size() ? ykey.size() : tempvalues2.size() );


  // Pair the vectors of keys and tempvalues (elevs) up.
  for (unsigned k=0; k < y_keys_elevs_paired.size(); k++)
  {
    y_keys_elevs_paired[k] = std::make_pair(ykey[k],tempvalues2[k]);
  }

  // Now do the sort
  // DAV- this implentation needs double-checking 1/12/2015
  std::sort(y_keys_elevs_paired.begin(),
            y_keys_elevs_paired.end(),
            sort_pair_second<double,double>() );

  // then works through the list of x and y co-ordinates from highest to lowest
  for (unsigned n = (jmax * imax); n >= 1; n--)
  {
    int i = static_cast<int>(std::get<0>(x_keys_elevs_paired[n]) );
    int j = static_cast<int>(std::get<0>(y_keys_elevs_paired[n]) );

    // I.e. If we are in the catchment (area_depth = 0 in NODATA)
    if (area_depth[i][j] > 0)
    {
      // update area if area_depth is higher
      // That is, set the area to a value if in catchment
      if (area_depth[i][j] > area[i][j]) area[i][j] = area_depth[i][j];

      double difftot = 0;

      // work out sum of +ve slopes in all 8 directions
      for (int dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
      {
        int j2 = j + deltaY[dir];
        int i2 = i + deltaX[dir];

        if (j2 < 1) j2 = 1;
        if (i2 < 1) i2 = 1;
        if (j2 > jmax) j2 = jmax;
        if (i2 > imax) i2 = imax;

        // swap comment lines below for drainage area from D8 or Dinfinity
        // Calculates the total drop (difference) in elevation of surrounding
        // cells cumulatively.
        // D8
        if (dir % 2 != 0)
        {
          if (elev[i2][j2] < elev[i][j])
          {
            difftot += elev[i][j] - elev[i2][j2];
          }
        }
        else
        {
          if (elev[i2][j2] < elev[i][j])
          {
            difftot += (elev[i][j] - elev[i2][j2]) / 1.414;
          }
        }
        //if(elev[x][y]-elev[x2][y2]>difftot) difftot=elev[x][y]-elev[x2][y2];
      }

      // Will there be any flow distribution?
      if (difftot > 0)
      {
        // if so, then distribute to all 8 neighbour cells...
        for (int dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
        {
          // Calculate the adjacent coordinates
          int i2 = i + deltaX[dir];
          int j2 = j + deltaY[dir];

          // Make sure we don't go over the edges of the model domain
          if (j2 < 1) j2 = 1;
          if (i2 < 1) i2 = 1;
          if (j2 > jmax) j2 = jmax;
          if (i2 > imax) i2 = imax;

          // swap comment lines below for drainage area from D8 or Dinfinity
          if (dir % 2 != 0)
          {
            if (elev[i2][j2] < elev[i][j])
            {
              area_depth[i2][j2] += area_depth[i][j] \
                * ((elev[i][j] - elev[i2][j2]) / difftot);
            }
          }
          else
          {
            if (elev[i2][j2] < elev[i][j])
            {
              area_depth[i2][j2] += area_depth[i][j] \
                * (((elev[i][j] - elev[i2][j2])/1.414) / difftot);
            }
          }
          //if (elev[x][y] - elev[x2][y2] == difftot)
          //  { area_depth[x2][y2] += area_depth[x][y]; }
        }

      }
      // finally zero the area depth for the old cell (i.e. centre cell in D8)
      area_depth[i][j] = 0;
    }
  }

}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// WRAPPER FUNCTIONS TO CARRY OUT EROSION ETC
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// CALC EROSION MULTIPLIER
void LSDCatchmentModel::call_erosion()
{
  if (counter >= erode_call)
  // erode_call is by default zero,
  // so don't call erosion before water has been routed.
  {
    erode_mult = static_cast<int>(ERODEFACTOR / erode(erode_mult) );
    if (erode_mult < 1)
    {
      erode_mult = 1;
    }
    if (erode_mult > 5)
    {
      erode_mult = 5;
    }
    erode_call = counter + erode_mult;
  }
}

// Carry out the lateral erosion
// not sure about the necessity of this call...
void LSDCatchmentModel::call_lateral()
{
  if (counter >= lateralcounter)
  {
    lateral_channel_erode();   // call the actual erosion function
    lateralcounter = counter + (50 * erode_mult);
  }
}

//=-=-=-=-=-=-=-=-=-=
// DATA OUTPUTS ETC.
//=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::output_data()
// this was part of erodep() in CL but I felt it should have its own method call
{
  unsigned int n;
  Qw_newvol += temptotal*((cycle - previous)*60); // 60 seconds per min

  for (unsigned nn = 1; nn <= rfnum; nn++)
  {
    Jw_newvol += (j_mean[nn] * DX * DX * nActualGridCells[nn]) * ((cycle - previous)*60);
  }

  // Catch all the timesteps that pass one or more hour marks
  if ((new_cycle < old_cycle) || (cycle - previous >= output_file_save_interval))
  {
    while (( tx > previous ) && (cycle >= tx))
    {
      hours++;

      // Step 1: Calculate hourly total sediment Q (m3)
      Qs_step = globalsediq - old_sediq;
      Qs_over = Qs_step*((cycle - tx)/(cycle - tlastcalc));
      Qs_hour = Qs_step - Qs_over + Qs_last;

      // reset Qs_last and old_sediq for large time steps
      if (cycle >= tx + output_file_save_interval)
      {
        Qs_last = 0;
        old_sediq = globalsediq - Qs_over;
      }

      // reset Qs_last and old_sediq for small time steps
      if (cycle < tx+output_file_save_interval)
      {
        Qs_last = Qs_over;
        old_sediq = globalsediq;
      }
      // Step 2: Calculate grain size Qgs, also calculate contaminated amounts
      for (n=1; n<=G_MAX-1; n++)
      {
        // calculate timestep Qgs
        Qg_step[n] = sum_grain[n] - old_sum_grain[n];
        Qg_step2[n] = sum_grain2[n] - old_sum_grain2[n];
        // Interpolate Qgs beyond time tx
        Qg_over[n] = Qg_step[n]*((cycle - tx)/(cycle - tlastcalc));
        Qg_over2[n] = Qg_step2[n] * ((cycle - tx) / (cycle - tlastcalc));
        // and calculate hourly Qgs
        Qg_hour[n] = Qg_step[n] - Qg_over[n] + Qg_last[n];
        Qg_hour2[n] = Qg_step2[n] - Qg_over2[n] + Qg_last2[n];
        // Reset Qg_last[n] and old_sum_grain[n]for large time steps
        if (cycle >= tx + output_file_save_interval)
        {
          Qg_last[n] = 0;
          Qg_last2[n] = 0;
          old_sum_grain[n] = sum_grain[n] - Qg_over[n];
          old_sum_grain2[n] = sum_grain2[n] - Qg_over2[n];
        }
        // Reset Qg_last[n] and old_sum_grain[n] for small time steps
        if (cycle < tx + output_file_save_interval)
        {
          Qg_last[n] = Qg_over[n];
          Qg_last2[n] = Qg_over2[n];
          old_sum_grain[n] = sum_grain[n];
          old_sum_grain2[n] = sum_grain2[n];
        }
      }

      // Step 3: Calculate hourly mean water discharge
      // Qw_overvol = temptotal*((cycle-tx)*output_file_save_interval);
      // replaced by line below MJ 25/01/05
      Qw_overvol = temptotal*((cycle - tx)*60);   // 60 secs per min
      Qw_stepvol = Qw_newvol - Qw_oldvol;
      Qw_hourvol = Qw_stepvol - Qw_overvol + Qw_lastvol;
      Qw_hour = Qw_hourvol/(60*output_file_save_interval);
      // convert hourly water volume to cumecs

      // same for Jw (j_mean contribution)  MJ 14/03/05
      for (unsigned int nn=1; nn<=rfnum; nn++)
      {
        Jw_overvol += (j_mean[nn] * DX * DX \
                        * nActualGridCells[nn])*((cycle - tx)*60);
        // fixed MJ 29/03/05
        // TO DO: DV - is this right? Doesn't this overwrite JwOvervol each iter?
        // should be += - DAV fixed this 2016
      }
      Jw_stepvol = Jw_newvol - Jw_oldvol;
      Jw_hourvol = Jw_stepvol - Jw_overvol + Jw_lastvol;
      Jw_hour = Jw_hourvol/(60*output_file_save_interval);


      // reset Qw_lastvol and Qw_oldvol for large time steps
      if (cycle >= tx + output_file_save_interval)
      {
        Qw_lastvol = 0;
        Qw_oldvol = Qw_newvol - Qw_overvol;

        // same for Jw (j_mean contribution)  MJ 14/03/05
        Jw_lastvol = 0;
        Jw_oldvol = Jw_newvol - Jw_overvol;
      }

      // reset Qw_lastvol and Qw_oldvol for small time steps
      if (cycle < tx + output_file_save_interval)
      {
        Qw_lastvol = Qw_overvol;
        Qw_oldvol = Qw_newvol;

        // same for Jw (j_mean contribution)  MJ 14/03/05
        Jw_lastvol = Jw_overvol;
        Jw_oldvol = Jw_newvol;
      }

      Tx = tx;
      tx = Tx + output_file_save_interval;

      // Step: 4 ((C++) needs rewriting)
      // Need to process each double as stringstream to set fixed precision
      // Then convert stringstream to new string and append to line of output.
      std::string output;

      // 1st Column: TIME (hours)
      std::stringstream hours_format;
      hours_format << std::fixed << std::setprecision(0) << hours;
      output = hours_format.str();

      // 2nd Column: Actual Discharge (cumecs)
      std::stringstream Qw_hour_format;
      Qw_hour_format << std::fixed << std::setprecision(6) << Qw_hour;
      output = output + " " + Qw_hour_format.str();

      // 3rd Column: Expected discharge (based on TOPMODEL?/drainage area?)
      std::stringstream Jw_hour_format;
      Jw_hour_format << std::fixed << std::setprecision(6) << Jw_hour;
      output = output + " " + Jw_hour_format.str();

      // Not used anymore(?)
      // Only included here for compatibilty with CAESAR-Lisflood output files
      // Basiaclly this column should be all zeros
      std::stringstream sand_out_format;
      sand_out_format << std::fixed << std::setprecision(6) << sand_out;
      output = output + " " + sand_out_format.str();
      sand_out = 0; // reset sand

      // Total Sediment discharge (m^3)
      std::stringstream Qs_hour_format;
      Qs_hour_format << std::fixed << std::setprecision(10) << Qs_hour;
      output = output + " " + Qs_hour_format.str();

      // Output the grain size fractions (m^3)
      for (n=1; n<=G_MAX-1; n++)
      {
        std::stringstream Qg_hour_format;
        Qg_hour_format << std::fixed << std::setprecision(10) << Qg_hour[n];
        output = output + " " + Qg_hour_format.str();
      }
      // Open the catchment time series file in append mode (ios_base::app)
      // Open it in write mode (ios_base::out)
      // write_fname is called "catchment.dat" by default (see the .hpp file)
      std::string OUTPUT_FILE = write_path + "/" + write_fname;
      std::ofstream timeseriesf(OUTPUT_FILE, std::ios_base::app | std::ios_base::out);

      // write the current timestep output to the time series file
      timeseriesf << output << std::endl;

      //close the file, although should you really do this if just opening it again in the next loop?
      timeseriesf.close();
    }
    tlastcalc = cycle;
  }
}

// Overloaded function for the fully distributed rainfall and runoff objects
void LSDCatchmentModel::output_data(double temptotal, runoffGrid& runoff)
{
  unsigned int n;
  Qw_newvol += temptotal*((cycle - previous)*60); // 60 seconds per min

  //for (int nn = 1; nn <= rfnum; nn++)
  for (unsigned i=1; i<=imax; i++)
  {
    for (unsigned j=1; j<=jmax; j++)
    {
      if (elev[i][j] > no_data_value)
      {
        Jw_newvol += (runoff.get_j_mean(i,j) * DX * DX ) * ((cycle - previous)*60);
      }
      // originally j_mean[nn] * DX*DX* nActualGridCells[nn] ...
      // needs checking because you don't want to include grid cells that are
      // actually outside the catchment and shouldnt be contributing to the
      // runoff output.
      //
      // The difference will be small since there will be hardly any runoff
      // here, but nontheless it's a bug that should fixed - TODO: DAV
    }
  }

  // Catch all the timesteps that pass one or more hour marks
  if ((new_cycle < old_cycle) || (cycle - previous >= output_file_save_interval))
  {
    while (( tx > previous ) && (cycle >= tx))
    {
      hours++;

      // Step 1: Calculate hourly total sediment Q (m3)
      Qs_step = globalsediq - old_sediq;
      Qs_over = Qs_step*((cycle - tx)/(cycle - tlastcalc));
      Qs_hour = Qs_step - Qs_over + Qs_last;

      // reset Qs_last and old_sediq for large time steps
      if (cycle >= tx + output_file_save_interval)
      {
        Qs_last = 0;
        old_sediq = globalsediq - Qs_over;
      }

      // reset Qs_last and old_sediq for small time steps
      if (cycle < tx+output_file_save_interval)
      {
        Qs_last = Qs_over;
        old_sediq = globalsediq;
      }

      // Step 2: Calculate grain size Qgs, also calculate contaminated amounts
      for (n=1; n<=G_MAX-1; n++)
      {
        // calculate timestep Qgs
        Qg_step[n] = sum_grain[n] - old_sum_grain[n];
        Qg_step2[n] = sum_grain2[n] - old_sum_grain2[n];
        // Interpolate Qgs beyond time tx
        Qg_over[n] = Qg_step[n]*((cycle - tx)/(cycle - tlastcalc));
        Qg_over2[n] = Qg_step2[n] * ((cycle - tx) / (cycle - tlastcalc));
        // and calculate hourly Qgs
        Qg_hour[n] = Qg_step[n] - Qg_over[n] + Qg_last[n];
        Qg_hour2[n] = Qg_step2[n] - Qg_over2[n] + Qg_last2[n];
        // Reset Qg_last[n] and old_sum_grain[n]for large time steps
        if (cycle >= tx + output_file_save_interval)
        {
          Qg_last[n] = 0;
          Qg_last2[n] = 0;
          old_sum_grain[n] = sum_grain[n] - Qg_over[n];
          old_sum_grain2[n] = sum_grain2[n] - Qg_over2[n];
        }
        // Reset Qg_last[n] and old_sum_grain[n] for small time steps
        if (cycle < tx + output_file_save_interval)
        {
          Qg_last[n] = Qg_over[n];
          Qg_last2[n] = Qg_over2[n];
          old_sum_grain[n] = sum_grain[n];
          old_sum_grain2[n] = sum_grain2[n];
        }
      }

      // Step 3: Calculate hourly mean water discharge
      // Qw_overvol = temptotal*((cycle-tx)*output_file_save_interval); // replaced by line below MJ 25/01/05
      Qw_overvol = temptotal*((cycle - tx)*60);   // 60 secs per min
      Qw_stepvol = Qw_newvol - Qw_oldvol;
      Qw_hourvol = Qw_stepvol - Qw_overvol + Qw_lastvol;
      Qw_hour = Qw_hourvol/(60*output_file_save_interval); // convert hourly water volume to cumecs

      // same for Jw (j_mean contribution)  MJ 14/03/05
      //for (int nn=1; nn<=rfnum; nn++)
      //{
      for (unsigned i=1; i<=imax; i++)
      {
        for (unsigned j=1; j<=jmax; j++)
        {
          if (elev[i][j] > no_data_value)
          {
            Jw_overvol += (runoff.get_j_mean(i,j) * DX * DX )*((cycle - tx)*60);
          }
          // DAV, as above, taken out this: "* nActualGridCells[nn]" after last DX,
          // but this will calclate over all grid cells which is inieffiient and
          // potentially buggy
        }
      }
      Jw_stepvol = Jw_newvol - Jw_oldvol;
      Jw_hourvol = Jw_stepvol - Jw_overvol + Jw_lastvol;
      Jw_hour = Jw_hourvol/(60*output_file_save_interval);


      // reset Qw_lastvol and Qw_oldvol for large time steps
      if (cycle >= tx + output_file_save_interval)
      {
        Qw_lastvol = 0;
        Qw_oldvol = Qw_newvol - Qw_overvol;

        // same for Jw (j_mean contribution)  MJ 14/03/05
        Jw_lastvol = 0;
        Jw_oldvol = Jw_newvol - Jw_overvol;
      }

      // reset Qw_lastvol and Qw_oldvol for small time steps
      if (cycle < tx + output_file_save_interval)
      {
        Qw_lastvol = Qw_overvol;
        Qw_oldvol = Qw_newvol;

        // same for Jw (j_mean contribution)  MJ 14/03/05
        Jw_lastvol = Jw_overvol;
        Jw_oldvol = Jw_newvol;
      }

      Tx = tx;
      tx = Tx + output_file_save_interval;

      // Step: 4 ((C++) needs rewriting)
      // Need to process each double as stringstream to set fixed precision
      // Then convert stringstream to new string and append to line of output.
      std::string output;

      // 1st Column: TIME (hours)
      std::stringstream hours_format;
      hours_format << std::fixed << std::setprecision(0) << hours;
      output = hours_format.str();

      // 2nd Column: Actual Discharge (cumecs)
      std::stringstream Qw_hour_format;
      Qw_hour_format << std::fixed << std::setprecision(6) << Qw_hour;
      output = output + " " + Qw_hour_format.str();

      // 3rd Column: Expected discharge (based on TOPMODEL?/drainage area?)
      std::stringstream Jw_hour_format;
      Jw_hour_format << std::fixed << std::setprecision(6) << Jw_hour;
      output = output + " " + Jw_hour_format.str();

      // Not used anymore(?)
      // Only included here for compatibilty with CAESAR-Lisflood output files
      // Basiaclly this column should be all zeros
      std::stringstream sand_out_format;
      sand_out_format << std::fixed << std::setprecision(6) << sand_out;
      output = output + " " + sand_out_format.str();
      sand_out = 0; // reset sand

      // Total Sediment discharge (m^3)
      std::stringstream Qs_hour_format;
      Qs_hour_format << std::fixed << std::setprecision(10) << Qs_hour;
      output = output + " " + Qs_hour_format.str();

      // Output the grain size fractions (m^3)
      for (n=1; n<=G_MAX-1; n++)
      {
        std::stringstream Qg_hour_format;
        Qg_hour_format << std::fixed << std::setprecision(10) << Qg_hour[n];
        output = output + " " + Qg_hour_format.str();
      }

      // Open the catchment time series file in append mode (ios_base::app)
      // Open it in write mode (ios_base::out)
      // write_fname is called "catchment.dat" by default (see the .hpp file)
      std::string OUTPUT_FILE = write_path + "/" + write_fname;
      std::ofstream timeseriesf(OUTPUT_FILE, std::ios_base::app | std::ios_base::out);

      // write the current timestep output to the time series file
      timeseriesf << output << std::endl;

      // close the file, although should you really do this if just opening
      // it again in the next loop?
      timeseriesf.close();
    }
    tlastcalc = cycle;
  }
}

void LSDCatchmentModel::save_raster_data(double tempcycle)
{
  // Write Water_depth raster
  if (write_waterd_file == true)
  {
    // TO DO
    // This will actually write out the border cells
    // which is incorrect technically
    // Find a way to trim these off.
    LSDRaster water_depthR(imax+2, jmax+2, xll, yll, DX,
                          no_data_value, water_depth);

    // Strip the padding of zeros round the edge
    water_depthR.strip_raster_padding();

    // Use the LSDRaster class's own method
    std::string current_water_depth_filename = waterdepth_fname + \
      std::to_string((int)tempcycle);

    std::string OUTPUT_WATERD_FILE = write_path + "/" + \
      current_water_depth_filename;

    water_depthR.write_double_raster(OUTPUT_WATERD_FILE, dem_write_extension);
  }

  // Write Elevation raster
  if (write_elev_file == true)
  {
    LSDRaster elev_outR(imax+2, jmax+2, xll, yll, DX, no_data_value, elev);
    // Get rid of the zeros padding the edges of the domain
    elev_outR.strip_raster_padding();

    std::string OUTPUT_ELEV_FILE = write_path + "/" + elev_fname + \
      std::to_string((int)tempcycle);

    elev_outR.write_double_raster(OUTPUT_ELEV_FILE, dem_write_extension);
  }

  // Write Grain File
  if (write_grainsz_file == true)
  {
    std::cout << "Entering the GRAINMATRIX..." << std::endl;
    LSDGrainMatrix grainsz_outR(imax, jmax, \
                                no_data_value, G_MAX, \
                                index, grain, strata);

    std::string OUTPUT_GRAIN_FILE = write_path + "/" + grainsize_fname + \
      std::to_string((int)tempcycle);

    grainsz_outR.write_grainMatrix_to_ascii_file(OUTPUT_GRAIN_FILE,
                                                 dem_write_extension);
  }

  // Write the elev diff file
  if (write_elevdiff_file == true)
  {
    TNT::Array2D<double> elevdiff_now = init_elevs - elev;

    LSDRaster elevdiff_outR(imax+2, jmax+2, xll, yll,
                            DX, no_data_value, elevdiff_now);
    elevdiff_outR.strip_raster_padding();

    std::string OUTPUT_ELEVDIFF_FILE = write_path + "/" + elevdiff_fname + \
      std::to_string((int)tempcycle);

    elevdiff_outR.write_double_raster(OUTPUT_ELEVDIFF_FILE,
                                      dem_write_extension);
  }

  // TODO
  // Make separate methods in future...

  // Write d50 top (surface) layer

  // write d50 for given layer

  // write specific fraction volume, on a given layer

  // write soil saturation raster
}

// This only currently checks for an edge that is not NODATA on at least one side
// It does not check that the DEM has its lowest point on this edge. This
// should probably be added.
void LSDCatchmentModel::count_catchment_gridcells()
{
  std::cout << "Counting number of actual grid cells in domain (non-NODATA)"
            << std::endl;
  for (unsigned i = 1; i < imax; i++)
  {
    for (unsigned j = 1; j < jmax; j++)
    {
      if (elev[i][j] > -9999) nActualGridCells[rfarea[i][j]]++;
    }
  }

  // Sum up all the catchment cells
  int totalCatchmentCells = 0;
  for (int n : nActualGridCells)
  {
    totalCatchmentCells += n;
  }
  // This is an odd construction, since nActualGridCells[0] is surely always 0?
  // needs to sum up all the values in the vector, not just in [1]
  std::cout << "Total number of grid cells within catchment: "
            << totalCatchmentCells << std::endl;

#ifdef DEBUG
  for (int i=0; i<=rfnum; i++)
  {
    std::cout << "Number of grid cells within hydroindex area " << i
              << " is: " << nActualGridCells[i] << std::endl;
  }
#endif
}

void LSDCatchmentModel::check_DEM_edge_condition()
{
  // Originally part of the Ur-loop (buttonclick2 or something)
  //nActualGridCells = 0;

  std::cout << "Checking edge cells for suitable catchment outlet point..."
            << std::endl;
  //check for -9999's on RH edge of DEM
  double nodata = -9999;  // TO DO change to ingest from data input
  double temp = -9999;

  unsigned maxcols = jmax;
  unsigned maxrows = imax;

  // start at 1 because zeroth elements are zeroed previously.
  // (i.e. like a zero border surrounding.
  // [1][1] is the first true elev data value.
  for (unsigned n = 1; n <= maxcols; n++)
  {
    // Check bottom edge (row major!)
    if (elev[maxrows][n] > nodata) temp = elev[maxrows][n];
    // check top edge
    if (elev[1][n] > nodata) temp = elev[1][n];
  }
  for (unsigned n = 1; n <= maxrows; n++)
  {
    // check LH edge
    if (elev[n][1] > nodata) temp = elev[n][1];
    // check RH edge
    if (elev[n][maxcols] > nodata) temp = elev[n][maxcols];
  }

  if (temp < -10)
  {
    std::cout << "DEM EDGE CONDITION ERROR: LSDCatchmentModel may not function \
                 properly, as the edges of the DEM are all nodata (-9999) \
                 values. This will prevent any water or sediment from leaving \
                 the edge of the model domain (DEM)" << std::endl;
        exit(EXIT_FAILURE);
  }
  else
  {
    std::cout << "Suitable outlet point on edge found. " << std::endl;
  }
}

void LSDCatchmentModel::zero_values()
{
  for(unsigned i=0; i <= imax+1; i++)
  {
    for(unsigned j=0; j <= jmax+1; j++)
    {
      Vel[i][j] = 0;
      area[i][j] = 0;
      elev[i][j] = -9999;
      bedrock[i][j] = -9999;
      init_elevs[i][j] = elev[i][j];
      water_depth[i][j] = 0;
      index[i][j] = -9999;
      inputpointsarray[i][j] = false;

      qx[i][j] = 0;
      qy[i][j] = 0;

      qxs[i][j] = 0;
      qys[i][j] = 0;

      for(int n=0; n<=8; n++)
      {
        vel_dir[i][j][n]=0;
      }

      Vsusptot[i][j] = 0;
      rfarea[i][j] = 1;
    }
  }

  // These arrays are different dimensions
  // So they need to be zeroed differently
  for (unsigned i=0; i<=imax; i++)
  {
    for(unsigned j=0; j<=jmax; j++)
    {
      if (vegetation_on)
      {
        veg[i][j][0] = 0;// elevation
        veg[i][j][1] = 0; // densitj
        veg[i][j][2] = 0; // jw density
        veg[i][j][3] = 0; // height
      }
      edge[i][j] = 0;
      edge2[i][j] = 0;
    }
  }

  for(unsigned i=1; i<((jmax*imax)/LIMIT); i++)
  {
    if (!hydro_only)
    {
      for(unsigned j=0; j<=G_MAX; j++)
      {
        grain[i][j] = 0;
      }
      for(int z=0; z <= 9; z++)
      {
        for(unsigned j=0; j <= G_MAX-2; j++)
        {
          strata[i][z][j] =0;
        }
      }
    }

    catchment_input_x_coord[i] = 0;
    catchment_input_y_coord[i] = 0;
  }

  // DAV - Don't think this is necessary now, these are std::vectors
  // and can be intitialised to zero or whatever when you create them in
  // initailise_arrays()
  // TO DO: Remove this loop.
  // It is duplicating what is done in initialise_arrays()
  for (unsigned i = 1; i <= rfnum; i++)
  {
    j[i] = 0.000000001;
    jo[i] = 0.000000001;
    j_mean[i] = 0;
    old_j_mean[i] = 0;
    new_j_mean[i] = 0;
  }

  // TODO
  // DAV - Hard coded in that the 1st fraction is suspended: needs
  // to be read from a separate input file at later date.
  isSuspended[1] = true;

}

// __________________________________________
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// HYDROLOGICAL METHODS
// A series of methods for doing the
// hydro routing, water-depths, etc
//
// CONTENTS:
// 1. water_flux_out
// 2. flow_route
// 3. depth_update
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// WATER FLUXES OUT OF CATCHMENT
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculate the water coming out and zero any water depths at the edges
// This will actually set it to the minimum water depth
// This must be done so that water can still move sediment to the edge of the catchment
// and hence remove it from the catchment. (otherwise you would get sediment build
// up around the edges.
void LSDCatchmentModel::water_flux_out()
// Extracted as a seprate method from erodepo()
{
  double local_time_factor = time_step;
  // Zero the water, but then we set it to the minimum depth - DV
  temptot = 0;
  for (unsigned i = 1; i <= imax; i++)
  {
    // RH Edge
    if (water_depth[i][jmax] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[i][jmax] - water_depth_erosion_threshold) \
        * DX * DX / local_time_factor;
      water_depth[i][jmax] = water_depth_erosion_threshold;
    }
    // LH Edge
    if (water_depth[i][1] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[i][1] - water_depth_erosion_threshold) \
        * DX * DX / local_time_factor;
      water_depth[i][1] = water_depth_erosion_threshold;
    }
  }

  for (unsigned j = 1; j <= jmax; j++)
  {
    // Top Edge
    if (water_depth[1][j] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[1][j] - water_depth_erosion_threshold) \
        * DX * DX / local_time_factor;
      water_depth[1][j] = water_depth_erosion_threshold;
    }
    // Bottom Edge
    if (water_depth[imax][j] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[imax][j] - water_depth_erosion_threshold) \
        * DX * DX / local_time_factor;
      water_depth[imax][j] = water_depth_erosion_threshold;
    }
  }
  waterOut = temptot;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// THE WATER ROUTING ALGORITHM: LISFLOOD-FP
//
// Originally derived in Bates et al (2010) Journal of Hydrology
// The Lisflood-FP algorithm is a non steady state water flow
// algorithm that uses a simplified version of the Saint-Venant
// shallow water equations (see Hydrology textbooks) to calculate
// water transport across the model.
//
// This algorithm was originally implemented in the CAESAR-Lisflood
// model (2013), and remains broadly unchanged (except it's in C++)
//
//
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::flow_route()
{
  double local_time_factor = set_local_timefactor();

  #pragma omp parallel for
  for (unsigned y=1; y<=jmax; y++)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      // This was a totally pointless idea but I leave it in to warn
      // others of my folly... (DAV)
////      __builtin_prefetch(&water_depth[down_scan[y][inc+1]-1][y-1]);
////      __builtin_prefetch(&water_depth[down_scan[y][inc+1]-1][y]);
////      __builtin_prefetch(&water_depth[down_scan[y][inc+1]][y-1]);
////      __builtin_prefetch(&water_depth[down_scan[y][inc+1]][y]);
////      __builtin_prefetch(&elev[down_scan[y][inc+1]-1][y]);
////      __builtin_prefetch(&elev[down_scan[y][inc+1]-1][y-1]);
////      __builtin_prefetch(&elev[down_scan[y][inc+1]][y]);
////      __builtin_prefetch(&elev[down_scan[y][inc+1]][y-1]);

      unsigned x = down_scan[y][inc];
      inc++;
      if (elev[x][y] > -9999) // to stop moving water in to -9999's on elev
      {
        // routing in x direction
        if ((water_depth[x][y] > 0 || water_depth[x - 1][y] > 0) \
          && elev[x - 1][y] > -9999)
          // need to check water and not -9999 on elev?
        {
          double hflow = std::max(elev[x][y] + water_depth[x][y],
                                  elev[x - 1][y] + water_depth[x - 1][y]) \
                                  - std::max(elev[x - 1][y], elev[x][y]);

          if (hflow > hflow_threshold)
          {
            double tempslope = (((elev[x - 1][y] + water_depth[x - 1][y])) -
                (elev[x][y] + water_depth[x][y])) / DX;

            if (x == imax) tempslope = edgeslope;
            if (x <= 2) tempslope = 0 - edgeslope;

            //double oldqx = qx[x][y];
            qx[x][y] = ((qx[x][y] - (gravity * hflow \
                          * local_time_factor * tempslope)) \
                          / (1 + gravity * hflow * local_time_factor \
                          * (mannings * mannings) * std::abs(qx[x][y]) \
                          / std::pow(hflow, (10 / 3))));
            //if (oldqx != 0) qx[x][y] = (oldqx + qx[x][y]) / 2;

            // need to have these lines to stop too much water moving from
            // one cell to another - resulting in negative discharges
            // which causes a large instability to develop
            // - only in steep catchments really

            // FROUDE NUBER CHECKS
            if (qx[x][y] > 0 && (qx[x][y]
                / hflow) / std::sqrt(gravity * hflow) > froude_limit )
            {
              qx[x][y] = hflow * (std::sqrt(gravity*hflow) * froude_limit );
            }
            // If the discahrge is now negative and above the froude_limit...
            if (qx[x][y] < 0 && std::abs(qx[x][y] / hflow)
                / std::sqrt(gravity * hflow) > froude_limit )
            {
              qx[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit ));
            }

            // DISCHARGE MAGNITUDE/TIMESTEP CHECKS
            // If the discharge is too high for this timestep, scale back...
            if (qx[x][y] > 0 && (qx[x][y] * local_time_factor / DX)
                > (water_depth[x][y] / 4))
            {
              qx[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;
            }
            // If the discharge is negative and too large, scale back...
            if (qx[x][y] < 0 && std::abs(qx[x][y] * local_time_factor / DX)
              > (water_depth[x - 1][y] / 4))
            {
              qx[x][y] = 0 - ((water_depth[x - 1][y] * DX) / 5) / local_time_factor;
            }

            // Update suspended fraction discharges
            if (isSuspended[1])
            {
              if (qx[x][y] > 0)
              {
                qxs[x][y] = qx[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
              }
              if (qx[x][y] < 0)
              {
                qxs[x][y] = qx[x][y] \
                  * (Vsusptot[x - 1][y] / water_depth[x - 1][y]);
              }

              if (qxs[x][y] > 0 && qxs[x][y] * local_time_factor
                > (Vsusptot[x][y] * DX) / 4)
              {
                qxs[x][y] = ((Vsusptot[x][y] * DX) / 5) / local_time_factor;
              }

              if (qxs[x][y] < 0 && std::abs(qxs[x][y] * local_time_factor)
                > (Vsusptot[x - 1][y] * DX) / 4)
              {
                qxs[x][y] = 0 - ((Vsusptot[x - 1][y] * DX) / 5) / local_time_factor;
              }
            }

            // calc velocity now
            if (qx[x][y] > 0)
            {
              vel_dir[x][y][7] = qx[x][y] / hflow;
            }
            if (qx[x][y] < 0)
            {
              vel_dir[x - 1][y][3] = (0- qx[x][y]) / hflow;
            }

          }
          else
          {
            qx[x][y] = 0;
            qxs[x][y] = 0;
          }
        }

        //routing in the y direction
        if ((water_depth[x][y] > 0 || water_depth[x][y - 1] > 0) && elev[x][y - 1] > -9999)
        {
          double hflow = std::max(elev[x][y] + water_depth[x][y], elev[x][y - 1] + water_depth[x][y - 1]) -
              std::max(elev[x][y], elev[x][y - 1]);

          if (hflow > hflow_threshold)
          {
            double tempslope = (((elev[x][y - 1] + water_depth[x][y - 1])) -
                (elev[x][y] + water_depth[x][y])) / DX;
            if (y == jmax) tempslope = edgeslope;
            if (y <= 2 ) tempslope = 0 - edgeslope;

            //double oldqy = qy[x][y];
            qy[x][y] = ((qy[x][y] - (gravity * hflow * local_time_factor * tempslope)) /
                        (1 + gravity * hflow * local_time_factor * (mannings * mannings) * std::abs(qy[x][y]) /
                         std::pow(hflow, (10 / 3))));
            //if (oldqy != 0) qy[x][y] = (oldqy + qy[x][y]) / 2;

            // need to have these lines to stop too much water moving from one cellt o another - resulting in -ve discharges
            // whihc causes a large instability to develop - only in steep catchments really
            if (qy[x][y] > 0 && (qy[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit ) qy[x][y] = hflow * (std::sqrt(gravity * hflow) * froude_limit );
            if (qy[x][y] < 0 && std::abs(qy[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit ) qy[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit));

            if (qy[x][y] > 0 && (qy[x][y] * local_time_factor / DX) > (water_depth[x][y] / 4)) qy[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;
            if (qy[x][y] < 0 && std::abs(qy[x][y] * local_time_factor / DX) > (water_depth[x][y - 1] / 4)) qy[x][y] = 0 - ((water_depth[x][y - 1] * DX) / 5) / local_time_factor;


            if (isSuspended[1])
            {

              if (qy[x][y] > 0) qys[x][y] = qy[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
              if (qy[x][y] < 0) qys[x][y] = qy[x][y] * (Vsusptot[x][y - 1] / water_depth[x][y - 1]);

              if (qys[x][y] > 0 && qys[x][y] * local_time_factor > (Vsusptot[x][y] * DX) / 4) qys[x][y] = ((Vsusptot[x][y] * DX) / 5) / local_time_factor;
              if (qys[x][y] < 0 && std::abs(qys[x][y] * local_time_factor) > (Vsusptot[x][y - 1] * DX) / 4) qys[x][y] = 0 - ((Vsusptot[x][y - 1] * DX) / 5) / local_time_factor;

            }

            // calc velocity now
            if (qy[x][y] > 0) vel_dir[x][y][1] = qy[x][y] / hflow;
            if (qy[x][y] < 0) vel_dir[x][y - 1][5] = (0 - qy[x][y]) / hflow;
          }
          else
          {
            qy[x][y] = 0;
            qys[x][y] = 0;
          }
        }

      }
    }
  }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DEPTH UPDATE
//
// This function scans the discharge arrays, qx, and qy, and updates
// the cell's water depth based on the difference between water discahrges
// in the x and y directions. Note that it only checks x+1 and y+1 because
// the down_scan array (see the scan_area() routine) tells it which cells
// flow into which.
//
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::depth_update()
{
  double local_time_factor = set_local_timefactor();

  maxdepth = 0;
  double l_maxdepth = maxdepth;
  #pragma omp parallel for reduction(max:l_maxdepth)
  for (unsigned y = 1; y<= jmax; y++)
  {
    int inc = 1;
    double tempmaxdepth = 0;
    while (down_scan[y][inc] > 0)
    {
      unsigned x = down_scan[y][inc];
      inc++;

      // update water depths
      water_depth[x][y] += local_time_factor * (qx[x + 1][y] - qx[x][y] + qy[x][y + 1] - qy[x][y]) / DX;
      // now update SS concs
      if (isSuspended[1])
      {
        Vsusptot[x][y] += local_time_factor * (qxs[x + 1][y] - qxs[x][y] + qys[x][y + 1] - qys[x][y]) / DX;
      }

      if (water_depth[x][y] > 0)
      {
        // line to remove any water depth on nodata cells (that shouldnt get there!)
        if (elev[x][y] == -9999) water_depth[x][y] = 0;
        // calc max flow depth for time step calc
        if (water_depth[x][y] > tempmaxdepth) tempmaxdepth = water_depth[x][y];
      }
    }
    if (tempmaxdepth > l_maxdepth)
    {
      l_maxdepth = tempmaxdepth;
    }
  }
  maxdepth = l_maxdepth;
  // reduction for later parallelism implementation DAV
  //for (unsigned x = 1; y <= jmax; y++) if (tempmaxdepth2[y] > maxdepth) maxdepth = tempmaxdepth2[y];
}

// DAV - This can be split into subfunctions
void LSDCatchmentModel::catchment_water_input_and_hydrology( double local_time_factor)
{
  // This was added in CL 1.8f but not present in 1.8a:
  for (unsigned i = 1; i<=rfnum; i++)
  {
    waterinput += j_mean[i] * nActualGridCells[i] * DX * DX;
  }

  for (int z=1; z <= totalinputpoints; z++)
  {
    int i = catchment_input_x_coord[z];
    int j = catchment_input_y_coord[z];
    double water_add_amt = (j_mean[rfarea[i][j]] * nActualGridCells[rfarea[i][j]]) /
        (catchment_input_counter[rfarea[i][j]]) * local_time_factor;    //
    if (water_add_amt > ERODEFACTOR)
    {
      water_add_amt = ERODEFACTOR;
    }

    // Removed now as done above
    // waterinput += (water_add_amt / local_time_factor) * DX * DX;
    water_depth[i][j] += water_add_amt;
  }
  // if the input type flag is 1 then the discharge is input from the hydrograph
  if (cycle >= time_1)
  {
    do
    {
      time_1++;
      topmodel_runoff(time_1);  // calc_J is based on the rainfall rate supplied to the cell

      if (time_step > max_time_step) // && new_j_mean[1] > (0.2 / (jmax * imax * DX * DX)))
      {
        // Find the current maximum runoff amount
        double j_mean_max_temp = 0;
        for (unsigned n = 1; n <= rfnum; n++)
        {
          if (new_j_mean[n] > j_mean_max_temp)
          {
            j_mean_max_temp = new_j_mean[n];
          }
        }

        // check after the variable rainfall area has been added
        // stops code going too fast when there is actual flow in the channels greater than 0.2cu
        if (j_mean_max_temp > (0.2 / (imax * jmax * DX * DX)))
        {
          cycle = time_1 + (max_time_step / 60);
          time_step = max_time_step;
        }
      }
    } while (time_1 < cycle);
  }

  calchydrograph(time_1 - cycle);

  double jmeanmax =0;
  for (unsigned n=1; n <= rfnum; n++)
  {
    if (j_mean[n] > jmeanmax)
    {
      jmeanmax = j_mean[n];
    }
  }

  // DV - This is for reading the dsicharge direct from an input file
  if (jmeaninputfile_opt == true)
  {
    j_mean[1] = ((hourly_rain_data[(static_cast<int>(cycle / rain_data_time_step))][0] //check in original
        / std::pow(DX, 2)) / nActualGridCells[1]);
  }

  if (jmeanmax >= baseflow) // > baseflow
  {
    baseflow = baseflow * 3;    // Magic number 3!? - DAV
    zero_and_calc_drainage_area();         // Could this come from one of the LSDobject files? - DAV
    get_catchment_input_points();
  }

  if (baseflow > (jmeanmax * 3) && baseflow > 0.000001) // DV reverted to match CL 1.8f (formerly 10^-7)
  // Could make the baseflow threshold a parameter in param file?
  {
    baseflow = jmeanmax * 1.25;   // Where do these magic numbers come from? DAV
    zero_and_calc_drainage_area();
    get_catchment_input_points();
  }
}

// DAV - This can be split into subfunctions
void LSDCatchmentModel::catchment_water_input_and_hydrology( double local_time_factor,
                                                                 runoffGrid& runoff)
{
  for (unsigned i = 1; i<imax; i++)
  {
    for (unsigned j =1; j<jmax; j++)
    {
      waterinput += runoff.get_j_mean(i,j) * DX * DX;
    }
  }

  // Not all compilers support reduction of class member variable,
  // so you'll need to fix it (GCC 6.2 seeps to support it)
  // workaround by just creating local copy of waterinput here
  //#pragma omp parallel for reduction(+:waterinput)
  for (unsigned i=1; i<imax; i++)
  {
    for (unsigned j=1; j<jmax; j++)
    {
      double water_add_amt = runoff.get_j_mean(i,j) * local_time_factor;    //

      if (water_add_amt > ERODEFACTOR)
      {
        water_add_amt = ERODEFACTOR;
      }

      waterinput += (water_add_amt / local_time_factor) * DX * DX;

      water_depth[i][j] += water_add_amt;
    }
  }

  // DAV - testing methodf for new_jmeanmax
  double new_jmeanmax = 0;
  for (unsigned m=1; m <= imax; m++)
  {
    for (unsigned n=1; n<=jmax; n++)
    {
      if (runoff.get_new_j_mean(m,n) > new_jmeanmax)
      {
        new_jmeanmax = runoff.get_new_j_mean(m,n);
      }
    }
  }

  // if the input type flag is 1 then the discharge is input from the hydrograph
  if (cycle >= time_1)
  {
    do
    {
      time_1++;
      topmodel_runoff(time_1, runoff);  // calc_J is based on the rainfall rate supplied to the cell
      if (time_step > max_time_step && new_jmeanmax > (0.2 / (jmax * imax * DX * DX)))
        // check after the variable rainfall area has been added
        // stops code going too fast when there is actual flow in the channels greater than 0.2cu
      {
        cycle = time_1 + (max_time_step / 60);
        time_step = max_time_step;
      }
    } while (time_1 < cycle);
  }

  calchydrograph(time_1 - cycle, runoff;
}

// Fully distributed TOPMODEL
void LSDCatchmentModel::topmodel_runoff(double cycle, runoffGrid& runoff)
{
  // UNique fiulename for raingrids
  //std::cout << "Calculating J for spatially complex rainfall..." << std::endl;
  // For later use with the rain grid object
  int current_rainfall_timestep = static_cast<int>(cycle / rain_data_time_step);

  // Create a raingrid object from the rainfall timeseries data and hydroindex
  // Only to be used with spatially var rainfall.

  // Populate rainfall data grid from input text file
  rainGrid current_raingrid(hourly_rain_data, rfarea,
                          imax, jmax,
                          current_rainfall_timestep,
                          rfnum);
  // Calculate runoff for this rainfall grid at this timestep
  runoff.calculate_runoff(rain_factor, M, jmax, imax, current_raingrid, elev);

  // For checking purposes

  if (DEBUG_write_raingrid == true)
  {
    std::string OUTPUT_RAINGRID_FILE = write_path + "/" + raingrid_fname + std::to_string((int)cycle);
    current_raingrid.write_rainGrid_to_raster_file(xll, yll, DX,
                                               OUTPUT_RAINGRID_FILE,
                                               dem_write_extension);
  }
  if (DEBUG_write_runoffgrid == true)
  {
    std::string OUTPUT_RUNOFF_FILE = write_path + "/" + runoffgrid_fname + std::to_string((int)cycle);
    runoff.write_runoffGrid_to_raster_file(xll, yll, DX,
                                           OUTPUT_RUNOFF_FILE,
                                           dem_write_extension);
  }
}

// Calculates the rainfall input to each cell per time step
// Jmean is the local rainfall inputed into the cell at each timestep
// Classic TOPMODEL formulation - semi-distributed
void LSDCatchmentModel::topmodel_runoff(double cycle)
{
  // UNique fiulename for raingrids?

  // For later use with the rain grid object
  int current_rainfall_timestep = static_cast<int>(cycle / rain_data_time_step);

  // Case for uniform OR non-interpolated rainfall

  for (unsigned n=1; n <= rfnum; n++)
  // rfnum is the rainfall number int = 2 to begin with
  {
    double local_rain_fall_rate = 0;   // in metres per second
    double local_time_step = 60; // in seconds

    old_j_mean[n] = new_j_mean[n];

    // jo[n] and j[n] are pre-initialised to some very small value in
    // the zero_values() function
    jo[n] = j[n];

    // Get the M value from the files if one is specified
    if (variable_m_value_flag == 1)
    {
      M = hourly_m_value[1 + current_rainfall_timestep];
    }

    local_rain_fall_rate = 0;

    // DAV -
    // for spatially uniform rainfall. You actualy want:
    // hourly_rain_data[hour][0]  (n starts at 1 here)
    // double cur_rain_rate = hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n];
    // std::cout << cur_rain_rate << std::endl;
    // DAV - I replaced [n] with [n-1] here as the rainfall data vector dimensions are correct.
    if (hourly_rain_data[current_rainfall_timestep][n-1] > 0)
    {
      local_rain_fall_rate = rain_factor * ((hourly_rain_data[current_rainfall_timestep][n-1] / 1000) / 3600);
      // divide by 1000 to make m/hr, then by 3600 for m/sec
    }

    if (local_rain_fall_rate == 0)
    {
      j[n] = jo[n] / (1 + ((jo[n] * local_time_step) / M));

      new_j_mean[n] = M / local_time_step *
          std::log(1 + ((jo[n] * local_time_step) / M));
    }

    if (local_rain_fall_rate > 0)
    {
      j[n] = local_rain_fall_rate / (((local_rain_fall_rate - jo[n]) / jo[n])
          * std::exp((0 - local_rain_fall_rate) * local_time_step / M) + 1);

      new_j_mean[n] = (M / local_time_step)
          * std::log(((local_rain_fall_rate - jo[n]) + jo[n] *
                      std::exp((local_rain_fall_rate * local_time_step) / M))
                     / local_rain_fall_rate);
    }
    if (new_j_mean[n] < 0)
    {
      new_j_mean[n] = 0;
    }
    // (M is the TOPMODEL m value)
    // DAV - Make a seperate function?
  }
}

// Calculates the storm hydrograph
void LSDCatchmentModel::calchydrograph(double time)
{
  for (unsigned n=1; n <= rfnum; n++)
  {
    j_mean[n] = old_j_mean[n] + (( (new_j_mean[n] - old_j_mean[n]) / 2) * (2 - time));
  }
}

// In this version you have to update every grid cell, as they all could possibly have different
// rainfall inputs.
void LSDCatchmentModel::calchydrograph(double time, runoffGrid& runoff)
{
  for (unsigned m=1; m<= imax; m++)
  {
    for (unsigned n=1; n <= jmax; n++)
    {
      double cell_j_mean = runoff.get_old_j_mean(m,n) + (( (runoff.get_new_j_mean(m,n) - runoff.get_old_j_mean(m,n)) / 2) * (2 - time));
      // set the calculated j_mean value for the current cell in the loop
      runoff.set_j_mean(m, n, cell_j_mean);
    }
  }
}

void LSDCatchmentModel::get_catchment_input_points()
{
  std::cout << "Calculating catchment input points... Total: ";

  totalinputpoints = 0;
  for (unsigned n=1; n <= rfnum; n++)
  {
    catchment_input_counter[n] = 0;
  }
  for (unsigned i=1; i <= imax; i++)
  {
    for (unsigned j=1; j <= jmax; j++)
    {
      if ((area[i][j] * baseflow * 3 * DX * DX) > MIN_Q \
          && (area[i][j] * baseflow * 3 * DX * DX) < MIN_Q_MAXVAL)
      {
        totalinputpoints++; // DAV - swapped back 28/10/2016
        catchment_input_x_coord[totalinputpoints] = i; // TO DO DAV - is this right wayround j?
        catchment_input_y_coord[totalinputpoints] = j;
        catchment_input_counter[rfarea[i][j]]++;
      }
    }
  }
  if (totalinputpoints == 0) totalinputpoints = 1;
  // Debug
  std::cout << totalinputpoints << std::endl;
}

// Unnecessary for complex runoff-rainfall?? - check DV
// This is an experimental version for working with
// the spatially variable runoff object.
void LSDCatchmentModel::get_catchment_input_points(runoffGrid& runoff)
{
  std::cout << "Calculating catchment input points... Total: ";

  totalinputpoints = 1;
  for (unsigned i=1; i <= imax; i++)
  {
    for (unsigned j=1; j <= jmax; j++)
    {
      if ((area[i][j] * baseflow * 3 * DX * DX) > MIN_Q \
        && (area[i][j] * baseflow * 3 * DX * DX) < MIN_Q_MAXVAL)
      {
        catchment_input_x_coord[totalinputpoints] = i;
        catchment_input_y_coord[totalinputpoints] = j;

        totalinputpoints++;
      }
    }
  }
  // Debug
  std::cout << totalinputpoints << std::endl;
}

void LSDCatchmentModel::evaporate(double time)
{
  double evap_amount = k_evap * (time / 1440);
  // now reduce if greater than erodedepo - to prevent instability
  if (evap_amount > ERODEFACTOR)
  {
    evap_amount = ERODEFACTOR;
  }

  for (unsigned y=1; y < imax; y++)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      unsigned x = down_scan[y][inc];
      inc++;
      // removes water in rate of mm per day..
      if (water_depth[x][y] > 0)
      {
        water_depth[x][y] -= evap_amount;
        if (water_depth[x][y] < 0) water_depth[x][y] = 0;
      }
    }
  }//);
}

void LSDCatchmentModel::scan_area()
{
  #pragma omp parallel for
  for (unsigned j=1; j <= jmax; j++)
  {
    int inc = 1;
    for (unsigned i=1; i <= imax; i++)
    {
      // zero scan bit..
      down_scan[j][i] = 0;
      // and work out scanned area. // TO DO (DAV) there is some out-of-bounds indexing going on here, check carefully!
      if (water_depth[i][j] > 0
          || water_depth[i][j - 1] > 0
          || water_depth[i][j + 1] > 0
          || water_depth[i - 1][j] > 0
          || water_depth[i - 1][j - 1] > 0
          || water_depth[i - 1][j + 1] > 0
          || water_depth[i + 1][j - 1] > 0
          || water_depth[i + 1][j + 1] > 0
          || water_depth[i + 1][j] > 0
          )
      {
        down_scan[j][inc] = i;
        inc++;
        // inc will increment everytime there is water found in a cell
      }
    }
  }
}


// __________________________________________
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// EROSIONAL METHODS
// A series of methods for doing the
// eroding, landsliding etc
//
// CONTENTS:
//  1) sort_active
//  2) addGS
//  3) sand_fraction
//  4) d50
//  5) slide_GS
//  6) mean_ws_elev
//  7) erode
//  8) lateral_channel_erode
//  9) in_channel_landsliding
//  10) global_landsliding
//  11) creep
//  12) soil_erosion
//  13) soil_development
//
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCatchmentModel::sort_active(int x,int y)
{
  int xyindex;
  double total;
  double amount;
  double coeff;

  if (index[x][y] == -9999)
  {
    addGS(x,y);
  }

  xyindex = index[x][y];

  total = 0.0;
  for (unsigned n = 0;n<=G_MAX;n++)
  {
    total += grain[xyindex][n];
  }

  if (total > (active*1.5)) // depositing - create new strata layer and remove bottom one..
  {
    // start from bottom
    // remove bottom active layer
    // then move all from layer above into one below, up to the top layer
    for(unsigned z=9;z>=1;z--)
    {
      for(unsigned n=0;n<=G_MAX-2;n++)
      {
        strata[xyindex][z][n]=strata[xyindex][z-1][n];
      }
    }

    // then remove strata thickness from grain - and add to top strata layer
    coeff = active / total;
    for (unsigned n=1; n<=(G_MAX-1); n++)
    {
      if ((grain[xyindex][n] > 0.0))
      {
        amount = coeff * (grain[xyindex][n]);
        strata[xyindex][0][n-1] = amount;
        grain[xyindex][n] -= amount;
      }
    }
  }

  if (total < (active/4)) // eroding - eat into existing strata layer & create new one at bottom
  {
    // Start at top
    // Add top strata to grain
    for (unsigned n=1;n<=(G_MAX-1);n++)
    {
      grain[xyindex][n] += strata[xyindex][0][n-1];
    }

    // then from top down add lower strata into upper
    for(unsigned z=0;z<=8;z++)
    {
      for(unsigned n=0;n<=G_MAX-2;n++)
      {
        strata[xyindex][z][n] = strata[xyindex][z+1][n];
      }
    }

    // add new layer at the bottom
    amount = active;
    int z = 9;
    for (unsigned n=1; n<=G_MAX-1; n++)
    {
        strata[xyindex][z][n-1] = amount * dprop[n];
    }
  }

}
 // NEW METHOD for N number of grain sizes
void LSDCatchmentModel::addGS(int x, int y)
{
  #pragma omp critical(addGS)
  {

  grain_array_tot++;
  index[x][y] = grain_array_tot;

  grain[grain_array_tot][0] = 0;
  for (unsigned n = 1; n <= G_MAX - 1;n++ )
  {
      grain[grain_array_tot][n] = active * dprop[n];
  }
  grain[grain_array_tot][G_MAX] = 0;


  for (unsigned n = 0; n <= 9; n++) // Do we always need 9 strata? Future improvement?
  {
      for (unsigned n2 = 0; n2 <= G_MAX-2; n2++ )
      {
          strata[grain_array_tot][n][n2] = (active) * dprop[n2+1];
      }


      if (elev[x][y] - (active * (n + 1)) < (bedrock[x][y] - active))
      {
          for (unsigned q = 0; q <= (G_MAX - 2); q++)
          {
              strata[grain_array_tot][n][q] = 0;
          }
      }
  }
  sort_active(x, y);
  }
}

double LSDCatchmentModel::sand_fraction(int index1)
{

  double active_thickness=0;
  double sand_total=0;
  for(unsigned n =1;n<=G_MAX;n++)
  {
    active_thickness+=(grain[index1][n]);
  }

  for(unsigned n=1;n<=2;n++) // number of sand fractions...
  {
    sand_total+=(grain[index1][n]);
  }

  if(active_thickness<0.0001)
  {
    return(0.0);
  }
  else
  {
    return(sand_total/active_thickness);
  }

}

double LSDCatchmentModel::mean_ws_elev(int x, int y)
{
  double elevtot = 0;
  int counter = 0;

  for (int dir = 1; dir <= 8; dir++)
  {
    int x2, y2;
    x2 = x + deltaX[dir];
    y2 = y + deltaY[dir];

    if (water_depth[x2][y2] > water_depth_erosion_threshold)
    {
      elevtot += water_depth[x2][y2] + elev[x2][y2];
      counter++;
    }

  }
  if (counter > 0)
  {
    elevtot /= counter;
    return elevtot;
  }

  else return 0;
}

double LSDCatchmentModel::d50(int index1)
{

  double active_thickness=0;
  double Dfifty=0,max=0,min=0;
  std::array<double, 20> cum_tot{};  // std::array only C++11

  for(unsigned n=1;n<=G_MAX;n++)
  {
    for(unsigned z=0;z<=(0);z++)
    {
      active_thickness+=(grain[index1][n]);
      cum_tot[n]+=active_thickness;
    }
  }


  int i=1;
  while(cum_tot[i]<(active_thickness*0.5) && i<=9)
  {
    i++;
  }

  if(i==1){min=std::log(d1);max=std::log(d1);}
  if(i==2){min=std::log(d1);max=std::log(d2);}
  if(i==3){min=std::log(d2);max=std::log(d3);}
  if(i==4){min=std::log(d3);max=std::log(d4);}
  if(i==5){min=std::log(d4);max=std::log(d5);}
  if(i==6){min=std::log(d5);max=std::log(d6);}
  if(i==7){min=std::log(d6);max=std::log(d7);}
  if(i==8){min=std::log(d7);max=std::log(d8);}
  if(i==9){min=std::log(d8);max=std::log(d9);}

  Dfifty = std::exp(max - ((max - min) * ((cum_tot[i] - (active_thickness * 0.5)) / (cum_tot[i] - cum_tot[i - 1]))));
  if(active_thickness<0.0000001)Dfifty=0;
  return Dfifty;

}

void LSDCatchmentModel::slide_GS(int x,int y, double amount,int x2, int y2)
{

  // Ok, heres how it works, x and y are ones material moved from,
  //    x2 and y2 are ones material moved to...
  //    amd amount is the amount shifted.


  double total = 0;

  // do only for cells where both have grainsize..
  if (index[x][y] != -9999 && index[x2][y2] != -9999)
  {
    for (unsigned n = 1; n <= (G_MAX - 1); n++)
    {
      if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
    }

    if (amount > total)
    {
      for (unsigned n=1; n<=G_MAX-1; n++)
      {
        grain[index[x2][y2]][n] += (amount - total) * dprop[n];
      }
      amount = total;
    }

    if (total > 0)
    {
      for (unsigned n = 1; n <= (G_MAX - 1); n++)
      {
        double transferamt = amount * (grain[index[x][y]][n] / total);
        grain[index[x2][y2]][n] += transferamt;
        grain[index[x][y]][n] -= transferamt;
        if (grain[index[x][y]][n] < 0) grain[index[x][y]][n] = 0;
      }

    }

    // then to set active layer to correct depth before erosion
    sort_active(x, y);
    sort_active(x2, y2);
    return;
  }

  // now do for cells where only recieving cells have grainsize
  // just adds amount to reviving cells of normal..
  if (index[x][y] == -9999 && index[x2][y2] != -9999)
  {
    for (unsigned n = 1; n <= G_MAX - 1; n++)
    {
      grain[index[x2][y2]][n] += (amount) * dprop[n];
    }

    // then to set active layer to correct depth before erosion
    sort_active(x2, y2);
    return;
  }

  // now for cells whre dontaing cell has grainsize
  if (index[x][y] != -9999 && index[x2][y2] == -9999)
  {

    addGS(x2, y2); // add grainsize array for recieving cell..

    if (amount > active)
    {
      for (unsigned n = 1; n <= G_MAX - 1; n++)
      {
        grain[index[x2][y2]][n] += amount * dprop[n];
      }
      amount = active;
    }

    for (unsigned n = 1; n <= (G_MAX - 1); n++)
    {
      if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
    }

    for (unsigned n = 1; n <= (G_MAX - 1); n++)
    {
      if (total > 0)
      {
        grain[index[x2][y2]][n] += amount * (grain[index[x][y]][n] / total);
        if (grain[index[x][y]][n] > 0.0001) grain[index[x][y]][n] -= amount * (grain[index[x][y]][n] / total);
        if (grain[index[x][y]][n] < 0) grain[index[x][y]][n] = 0;
      }
    }

    /* then to set active layer to correct depth before erosion, */
    sort_active(x, y);
    sort_active(x2, y2);
    return;
  }
}

// Interlude. Here's a relaxing picture of a lighthouse:
//
//      ----------.................._____________  _  .-.
//                                      _____.. . .   | |
//                   _____....------""""             uuuuu
// ____....------""""                                |===|
//                                                   |===|
//                                                   |===|
//                                                   |===|
// _a:f____________________________________________ .[__N]. _______


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// EROSION ROUTINE(S)
//
// This huge (sorry - will be refactored eventually!)
// block of code does the erosion
// in the model. It iterates over every cell
// in the domain and calculates amounts to be
// entrained into bedload and suspended load,
// and the amount to erode or deposit. As you
// may imagine, it's probably the most
// compuationally expensive part of the code.
// Especially when using multiple grain size
// fracions.
//
// The Wilcock or Einstein models of sediment
// transport can be used. Bedrock incision is
// calculated using the simple stream power law.
//
// TO DO
// It really ought to be split into smaller
// functions, but it is what it is for now.
//
// Based on the erode function in CAESAR-Lisflood
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
double LSDCatchmentModel::erode(double mult_factor)
{

  const double rho = 1000.0;
  double tempbmax = 0;
  float gtot2[20] = {};
  //double local_time_factor = 0;

  time_step = time_step * 1.5;

  // Deal with erosion timestep
//  switch (erode_timestep_type)
//  {
//    case 0:  // Erosion controls global time step and thus next hydro timestep
//      time_step = time_step * 1.5;
//      if (time_step > max_time_step) time_step = max_time_step;
//      local_time_factor = time_step;
//      break;

//    case 1:  // Erosion uses local timestep: global timestep and thus hydro timestep not affected
//      local_time_factor = time_step * 1.5;
//      if (local_time_factor > max_time_step) local_time_factor = max_time_step;
//      break;
//  }

  do
  {
  tempbmax = 0;
#pragma omp parallel for reduction(max:tempbmax) \
//  private(tempdir, temp_dist, temptot2, veltot, vel, qtot, tau, velnum, slopetot) schedule(runtime)
    for (unsigned int y = 1; y < jmax; ++y)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        unsigned x = down_scan[y][inc];
        inc++;


        // zero vels.
        Vel[x][y] = 0;
        Tau[x][y] = 0;

        for (unsigned n = 0; n < G_MAX; n++)
        {
          sr[x][y][n] = 0;
          sl[x][y][n] = 0;
          su[x][y][n] = 0;
          sd[x][y][n] = 0;
        }
        ss[x][y] = 0;

        if (water_depth[x][y] > water_depth_erosion_threshold)
        {

          double temptot2 = 0;
          double veltot = 0;
          double vel = 0;
          double qtot = 0;
          double tau = 0;
          double velnum = 0;
          double slopetot = 0;

          double tempdir[11] = {};
          double temp_dist[11] = {};

//          for (int i =0; i<11; ++i)
//          {
//            tempdir[i] =0;
//            temp_dist[i] =0;
//          }

          // add spatial mannings here
          //if (SpatVarMannings == true) mannings = spat_var_mannings[x][y];

          // check to see if index for that cell...
          if (index[x][y] == -9999) addGS(x, y);

          // now tot up velocity directions, velocities and edge directions.
          for (int p = 1; p <= 8; p+=2)
          {
            int x2 = x + deltaX[p];
            int y2 = y + deltaY[p];
            if (water_depth[x2][y2] > water_depth_erosion_threshold)
            {
              if (edge[x][y] > edge[x2][y2])
              {
                temptot2 += (edge[x][y] - edge[x2][y2]);
              }

              if (vel_dir[x][y][p] > 0 )
              {
                // first work out velocities in each direction (for sedi distribution)
                vel = vel_dir[x][y][p];
                if (vel > max_vel)
                {
                  vel = max_vel; // if vel too high cut it
                }
                tempdir[p] = vel * vel;
                veltot += tempdir[p];
                velnum++;
                qtot += (vel * vel);
                //slopetot += ((elev[x][y] - elev[x2][y2]) / DX);
                slopetot += ((elev[x][y] - elev[x2][y2]) / DX) * vel;
              }
            }
          }

          if (qtot > 0)
          {
            vel = (std::sqrt(qtot));
            Vel[x][y] = vel;

            if (vel > max_vel) vel = max_vel; // if vel too high cut it
            double ci = gravity * (mannings * mannings) * std::pow(water_depth[x][y], -0.33);
            //tauvel = 1000 * ci * vel * vel;
            if (slopetot > 0) slopetot = 0;
            //tauvel = 1000 * ci * vel * vel * (1 + (1 * (slopetot)));
            tau = 1000 * ci * vel * vel * (1 + (1 * (slopetot / vel)));
            Tau[x][y] = tau;
          }

          // now do some erosion
          if (tau > 0)
          {
            double d_50 = 0;
            double Fs = 0;
            double Di = 0;
            double graintot = 0;
            if (wilcock == 1)
            {
              d_50 = d50(index[x][y]);
              if (d_50 < d1) d_50 = d1;
              Fs = sand_fraction(index[x][y]);
              for (unsigned n = 1; n <= G_MAX; n++)graintot += (grain[index[x][y]][n]);
            }

            double temptot1 = 0;

            for (unsigned int n = 1; n <= G_MAX-1; n++)
            {
              switch (n)
              {
                case 1: Di = d1; break;
                case 2: Di = d2; break;
                case 3: Di = d3; break;
                case 4: Di = d4; break;
                case 5: Di = d5; break;
                case 6: Di = d6; break;
                case 7: Di = d7; break;
                case 8: Di = d8; break;
                case 9: Di = d9; break;
              }

              // Wilcock and Crowe/Curran

              if (wilcock == 1)
              {
                double tau_ri = 0, U_star, Wi_star;
                tau_ri = (0.021 + (0.015 * std::exp(-20 * Fs))) * (rho * gravity * d_50) * std::pow((Di / d_50), (0.67 / (1 + std::exp(1.5 - (Di / d_50)))));
                U_star = std::pow(tau / rho, 0.5);
                double Fi = grain[index[x][y]][n] / graintot;

                if ((tau / tau_ri) < 1.35)
                {
                  Wi_star = 0.002 * std::pow(tau / tau_ri, 7.5);
                }
                else
                {
                  Wi_star = 14 * std::pow(1 - (0.894 / std::pow(tau / tau_ri, 0.5)), 4.5);
                }
                //maybe should divide by DX as well..
                temp_dist[n] = mult_factor * time_step *
                               ((Fi * (U_star * U_star * U_star)) / ((2.65 - 1) * gravity)) * Wi_star / DX;
              }
              // Einstein sed tpt eqtn
              if (einstein == 1)
              {
                // maybe should divide by DX as well..
                temp_dist[n] = mult_factor * time_step * (40 * std::pow((1 / (((2650 - 1000) * Di) / (tau / gravity))), 3))
                               / std::sqrt(1000 / ((2250 - 1000) * gravity * (Di * Di * Di))) / DX;
              }

              //if (temp_dist[n] < 0.0000000000001) temp_dist[n] = 0;

              // first check to see that theres not too little sediment in a cell to be entrained
              if (temp_dist[n] > grain[index[x][y]][n])
              {
                temp_dist[n] = grain[index[x][y]][n];
              }
              // then check to see if this would make SS levels too high.. and if so reduce
              if (isSuspended[n] && n == 1)
              {
                if ((temp_dist[n] + Vsusptot[x][y]) / water_depth[x][y] > Csuspmax)
                {
                  //work out max amount of sediment that can be there (waterdepth * csuspmax) then subtract whats already there
                  // (Vsusptot) to leave what can be entrained. Check if < 0 after.
                  temp_dist[n] = (water_depth[x][y] * Csuspmax) - Vsusptot[x][y];
                }
              }
              if (temp_dist[n] < 0) temp_dist[n] = 0;

              // nwo placed here speeding up reduction of erode repeats.
              temptot1 += temp_dist[n];
            }

            //check if this makes it below bedrock
            if (elev[x][y] - temptot1 <= bedrock[x][y])
            {
              // now remove from proportion that can be eroded..
              // we can do this as we have the prop (in temptot) that is there to be eroded.
              double elevdiff = elev[x][y] - bedrock[x][y];
              double temptot3 = temptot1;
              temptot1 = 0;
              for (unsigned int n = 1; n <= G_MAX-1; n++)
              {
                if (elev[x][y] <= bedrock[x][y])
                {
                  temp_dist[n] = 0;
                }
                else
                {
                  temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
                  if (temp_dist[n] < 0) temp_dist[n] = 0;
                }
                temptot1 += temp_dist[n];
              }

              // here insert bedrock erosion routine?
              if (tau > bedrock_erosion_threshold)
              {
                double amount = 0; // amount is amount of erosion into the bedrock.
                amount = std::pow(bedrock_erosion_rate * tau, 1.5) * time_step * mult_factor * 0.000000317; // las value to turn it into erosion per year (number of years per second)
                bedrock[x][y] -= amount;
                // now add amount of bedrock eroded into sediment proportions.
                for (unsigned int n2 = 1; n2 <= G_MAX - 1; n2++)
                {
                  grain[index[x][y]][n2] += amount * dprop[n2];
                }
              }
            }


            // veg components
            // here to erode the veg layer..
            if (veg[x][y][1] > 0 && tau > vegTauCrit)
            {
              // now to remove from veg layer..
              veg[x][y][1] -= mult_factor * time_step * std::pow(tau - vegTauCrit, 0.5) * 0.00001;
              if (veg[x][y][1] < 0) veg[x][y][1] = 0;
            }

            // now to determine if movement should be restricted due to veg... or because of bedrock...
            if (veg[x][y][1] > 0.25)
            {
              // now checks if this removed from the cell would put it below the veg layer..
              if (elev[x][y] - temptot1 <= veg[x][y][0])
              {
                // now remove from proportion that can be eroded..
                // we can do this as we have the prop (in temptot) that is there to be eroded.
                double elevdiff = 0;
                elevdiff = elev[x][y] - veg[x][y][0];
                if (elevdiff < 0) elevdiff = 0;
                double temptot3 = temptot1;
                temptot1 = 0;
                for (unsigned n = 1; n <= G_MAX-1; n++)
                {
                  temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
                  if (elev[x][y] <= veg[x][y][0]) temp_dist[n] = 0;
                  temptot1 += temp_dist[n];
                }
                //temptot1 -= elevdiff;
                if (temptot1 < 0) temptot1 = 0;
              }
            }

            if (temptot1 > tempbmax)
            {
              tempbmax = temptot1;
            }

            // now work out what portion of bedload has to go where...
            // only allow actual transfer of sediment if there is flow in a direction - i.e. some sedeiment transport
            if(temptot1>0)
            {
              for (int p = 1; p <= 8; p += 2)
              {
                int x2 = x + deltaX[p];
                int y2 = y + deltaY[p];

                if (water_depth[x2][y2] > water_depth_erosion_threshold)
                {
                  if (index[x2][y2] == -9999) addGS(x2, y2);
                  double factor = 0;

                  // vel slope
                  if (vel_dir[x][y][p] > 0)
                  {
                    factor += 0.75 * tempdir[p] / veltot;
                  }
                  // now for lateral gradient.
                  if (edge[x][y] > edge[x2][y2])
                  {
                    factor += 0.25 * ((edge[x][y] - edge[x2][y2]) / temptot2);
                  }

                  // now loop through grainsizes
                  for (unsigned n = 1; n <= G_MAX-1; n++)
                  {
                    if (temp_dist[n] > 0)
                    {
                      if (n == 1 && isSuspended[n])
                      {
                        // put amount entrained by ss in to ss[,]
                        ss[x][y] = temp_dist[n];
                      }
                      else
                      {
                        switch (p)
                        {
                          case 1: su[x][y][n] = temp_dist[n] * factor; break;
                          case 3: sr[x][y][n] = temp_dist[n] * factor; break;
                          case 5: sd[x][y][n] = temp_dist[n] * factor; break;
                          case 7: sl[x][y][n] = temp_dist[n] * factor; break;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    if (tempbmax > ERODEFACTOR)
    {
      time_step *= (ERODEFACTOR / tempbmax) * 0.5;
    }
//      switch (erode_timestep_type)
//      {
//        case 0:
//          time_step *= (ERODEFACTOR / tempbmax) * 0.5;
//          local_time_factor = time_step;
//          break;
//        case 1:
//          local_time_factor *= (ERODEFACTOR / tempbmax) * 0.5;
//          break;
//      }
//    }
  } while(tempbmax > ERODEFACTOR);

  TNT::Array2D<double> erodetot(imax+2, jmax+2, 0.0);
  TNT::Array2D<double> erodetot3(imax+2, jmax+2, 0.0);

#pragma omp parallel for
  for (unsigned y = 2; y < jmax; ++y)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      unsigned x = down_scan[y][inc];
      inc++;

      if (water_depth[x][y] > water_depth_erosion_threshold && x < imax && x > 1)
      {
        if (index[x][y] == -9999) addGS(x, y);
        for (unsigned int n = 1; n <= G_MAX-1; n++)
        {
          if (n == 1 && isSuspended[n])
          {
            // updating entrainment of SS
            Vsusptot[x][y] += ss[x][y];
            grain[index[x][y]][n] -= ss[x][y];
            erodetot[x][y] -= ss[x][y];

            // this next part is unusual. You have to stop susp sed deposition on the input cells, otherwies
            // it drops sediment out, but cannot entrain as ss levels in input are too high leading to
            // little mountains of sediment. This means a new array in order to check whether a cell is an
            // input point or not..
            if (!inputpointsarray[x][y])
            {
              // now calc ss to be dropped
              double coeff = (fallVelocity[n] * time_step) / water_depth[x][y];
              if (coeff > 1) coeff = 1;
              double Vpdrop = coeff * Vsusptot[x][y];
              if (Vpdrop > 0.001) Vpdrop = 0.001; //only allow 1mm to be deposited per iteration
              grain[index[x][y]][n] += Vpdrop;
              erodetot[x][y] += Vpdrop;
              Vsusptot[x][y] -= Vpdrop;
              //if (Vsusptot[x][y] < 0) Vsusptot[x][y] = 0; NOT this line.
            }
          }
          else
          {
            //else update grain and elevations for bedload.
            double val1 = (su[x][y][n] + sr[x][y][n] + sd[x][y][n] + sl[x][y][n]);
            double val2 = (su[x][y + 1][n] + sd[x][y - 1][n] + sl[x + 1][y][n] + sr[x - 1][y][n]);
            grain[index[x][y]][n] += val2 - val1;
            erodetot[x][y] += val2 - val1;
            erodetot3[x][y] += val1;
          }
        }

        elev[x][y] += erodetot[x][y];
        if (erodetot[x][y] < 0)
        {
          sort_active(x, y);
        }
        //
        // test lateral code...
        //

        if (erodetot3[x][y] > 0)
        {
          double elev_update = 0;

          if (elev[x - 1][y] > elev[x][y] && x > 2)
          {
            double amt = 0;

            if (water_depth[x - 1][y] < water_depth_erosion_threshold)
            {
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x - 1][y] * time_step /DX;
            }
            else
            {
              amt = chann_lateral_erosion * erodetot3[x][y] * (elev[x - 1][y] - elev[x][y]) / DX * 0.1;
            }
            if (amt > 0)
            {
              amt *= 1 - (veg[x - 1][y][1] * (1 - veg_lat_restriction));
              if ((elev[x - 1][y] - amt) < bedrock[x - 1][y] || x - 1 == 1) amt = 0;
              if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
              //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
              elev_update += amt;
              elev[x - 1][y] -= amt;
              slide_GS(x - 1, y, amt, x, y);
            }
          }
          if (elev[x + 1][y] > elev[x][y] && x < imax-1)
          {
            double amt = 0;
            if (water_depth[x + 1][y] < water_depth_erosion_threshold)
            {
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x + 1][y] * time_step / DX;
            }
            else
            {
              amt = chann_lateral_erosion * erodetot3[x][y] * (elev[x + 1][y] - elev[x][y]) / DX * 0.1;
            }
            if (amt > 0)
            {
              amt *= 1 - (veg[x + 1][y][1] * (1 - veg_lat_restriction));
              if ((elev[x + 1][y] - amt) < bedrock[x + 1][y] || x + 1 == imax) amt = 0;
              if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
              //if (amt > erodetot2 /2) amt = erodetot2 /2;
              elev_update += amt;
              elev[x + 1][y] -= amt;
              slide_GS(x + 1, y, amt, x, y);
            }
          }

          elev[x][y] += elev_update;
        }
      }
    }
  }

#pragma omp parallel for
  for (unsigned y = 2; y < jmax; ++y)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      unsigned x = down_scan[y][inc];
      inc++;
      {
        if (erodetot3[x][y] > 0)
        {
          double elev_update = 0;

          if (elev[x][y - 1] > elev[x][y])
          {
            double amt = 0;
            if (water_depth[x][y - 1] < water_depth_erosion_threshold)
            {
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y - 1] * time_step / DX;
            }
            else
            {
              amt = chann_lateral_erosion * erodetot3[x][y] * (elev[x][y - 1] - elev[x][y]) / DX *0.1;
            }
            if (amt > 0)
            {
              amt *= 1 - (veg[x][y - 1][1] * (1 - veg_lat_restriction));
              if ((elev[x][y - 1] - amt) < bedrock[x][y - 1] || y - 1 == 1) amt = 0;
              if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
              //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
              elev_update += amt;
              elev[x][y - 1] -= amt;
              slide_GS(x, y - 1, amt, x, y);
            }
          }
          if (elev[x][y + 1] > elev[x][y])
          {
            double amt = 0;
            if (water_depth[x][y + 1] < water_depth_erosion_threshold)
            {
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y + 1] * time_step / DX;
            }
            else
            {
              amt = chann_lateral_erosion * erodetot3[x][y] * (elev[x][y + 1] - elev[x][y]) / DX * 0.1;
            }

            if (amt > 0)
            {
              amt *= 1 - (veg[x][y + 1][1] * (1 - veg_lat_restriction));
              if ((elev[x][y + 1] - amt) < bedrock[x][y + 1] || y + 1 == jmax) amt = 0;
              if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
              //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
              elev_update += amt;
              elev[x][y + 1] -= amt;
              slide_GS(x, y + 1, amt, x, y);
            }
          }

          elev[x][y] += elev_update;
        }
      }
    }
  }


// now calculate sediment outputs from all four edges...
#ifndef __INTEL_COMPILER   // OpenMP 4.5 array reduction not yet supported by intel
  #if (__GNUC__ >= 6 && __GNUC_MINOR__ > 1)
#pragma omp parallel for reduction(+:gtot2[:20])
  #endif
#endif
  for (unsigned y = 2; y < jmax; y++)
  {
    if (water_depth[imax][y] > water_depth_erosion_threshold || Vsusptot[imax][y] > 0)
    {
      for (unsigned int n = 1; n <= G_MAX-1; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[imax][y];
          Vsusptot[imax][y] = 0;
        }
        else
        {
          gtot2[n] += sr[imax - 1][y][n];
        }
      }
    }
    if (water_depth[1][y] > water_depth_erosion_threshold || Vsusptot[1][y] > 0)
    {
      for (unsigned int n = 1; n <= G_MAX-1; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[1][y];
          Vsusptot[1][y] = 0;
        }
        else
        {
          gtot2[n] += sl[2][y][n];
        }
      }
    }
  }

#ifndef __INTEL_COMPILER // OpenMP 4.5 array reduction not yet supported by intel
  #if (__GNUC__ >= 6 && __GNUC_MINOR__ > 1)
#pragma omp parallel for reduction(+:gtot2[:20])
  #endif
#endif
  for (unsigned x = 2; x < imax; x++)
  {
    if (water_depth[x][jmax] > water_depth_erosion_threshold || Vsusptot[x][jmax] > 0)
    {
      for (unsigned int n = 1; n <= G_MAX-1; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[x][jmax];
          Vsusptot[x][jmax] = 0;
        }
        else
        {
          gtot2[n] += sd[x][jmax - 1][n];
        }
      }
    }
    if (water_depth[x][1] > water_depth_erosion_threshold || Vsusptot[x][1] > 0)
    {
      for (unsigned int n = 1; n <= G_MAX-1; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[x][1];
          Vsusptot[x][1] = 0;
        }
        else
        {
          gtot2[n] += su[x][2][n];
        }
      }
    }
  }

  /// now update files for outputing sediment and re-circulating...
  ///

  sediQ = 0;
  for (unsigned int n = 1; n <= G_MAX; n++)
  {
    if (temp_grain[n] < 0) temp_grain[n] = 0;
    //if (/* DISABLES CODE */ (0))
    //    temp_grain[n] += gtot2[n] * recirculate_proportion; // important to divide input by time factor, so it can be reduced if re-circulating too much...
    sediQ += gtot2[n] * DX * DX;
    globalsediq += gtot2[n] * DX * DX;
    sum_grain[n] += gtot2[n] * DX * DX; // Gez
  }

  return tempbmax;

}


// Does the lateral erosion
// NOT TESTED YET - IN PROGRESS
void LSDCatchmentModel::lateral_channel_erode()
{
  // declare arrays and initialise the size of them
  TNT::Array2D<double> edge_temp(jmax + 1, imax + 1, 0.0);
  TNT::Array2D<double> edge_temp2(jmax + 1, imax + 1, 0.0);
  TNT::Array2D<double> water_depth2(jmax + 1, imax + 1, 0.0);

  TNT::Array2D<int> upscale( (jmax + 1)*2, (imax + 1)*2, 0);
  TNT::Array2D<int> upscale_edge( (jmax + 1)*2, (imax + 1)*2, 0);


  // first make water depth2 equal to water depth then remove single wet cells frmo water depth2 that have an undue influence..
  double mft = 0.1;// water_depth_erosion_threshold;//MIN_Q;// vel_dir threshold
  for (unsigned y = 2; y < jmax; y++)   // fix dav 2016 - should it start from 1 though?
  {

    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      unsigned x = down_scan[y][inc];

      edge_temp[x][y] = 0;
      if (x == 1) x++;
      if (x == jmax) x--;
      inc++;

      if (Tau[x][y] > mft)
      {
        water_depth2[x][y] = Tau[x][y];
        int tempcounter = 0;
        for (int dir = 1; dir <= 8; dir++)
        {
          int x2, y2;
          x2 = x + deltaX[dir];
          y2 = y + deltaY[dir];
          if (Tau[x2][y2] < mft) tempcounter++;
        }
        if (tempcounter > 6) water_depth2[x][y] = 0;
      }
    }
  }

  // first determine which cells are at the edge of the channel

  for (unsigned y = 2; y < imax; y++)
  {

    for (unsigned x = 2; x < jmax; x++)
    {

      edge[x][y] = -9999;

      if (water_depth2[x][y] < mft)
      {
        // if water depth < threshold then if its next to a wet cell then its an edge cell
        if (water_depth2[x][y - 1] > mft ||
            water_depth2[x - 1][y] > mft ||
            water_depth2[x + 1][y] > mft ||
            water_depth2[x][y + 1] > mft)
        {
          edge[x][y] = 0;
        }

        // unless its a dry cell surrounded by wet...
        if (water_depth2[x][y - 1] > mft &&
            water_depth2[x - 1][y] > mft &&
            water_depth2[x + 1][y] > mft &&
            water_depth2[x][y + 1] > mft)
        {
          edge[x][y] = -9999;
          edge2[x][y] = -9999;
        }

        // then update upscaled grid..
        upscale[(x * 2)][(y * 2)] = 0; // if dry
        upscale[(x * 2)][(y * 2) - 1] = 0;
        upscale[(x * 2) - 1][(y * 2)] = 0;
        upscale[(x * 2) - 1][(y * 2) - 1] = 0;
      }

      // update upscaled grid with wet cells (if wet)
      if (water_depth2[x][y] >= mft)
      {
        upscale[(x * 2)][(y * 2)] = 1; // if wet
        upscale[(x * 2)][(y * 2) - 1] = 1;
        upscale[(x * 2) - 1][(y * 2)] = 1;
        upscale[(x * 2) - 1][(y * 2) - 1] = 1;
      }
    }
  }

  // now determine edge cells on the new grid..
  for (unsigned y = 2; y < imax * 2; y++)
  {
    for (unsigned x = 2; x < jmax * 2; x++)
    {
      upscale_edge[x][y] = 0;
      if (upscale[x][y] == 0)
      {
        if (upscale[x][y - 1] == 1 ||
            upscale[x - 1][y] == 1 ||
            upscale[x + 1][y] == 1 ||
            upscale[x][y + 1] == 1)
        {
          upscale[x][y] = 2;
        }
      }
    }
  }

  // now tall up inside and outside on upscaled grid
  for (unsigned y = 2; y < imax * 2; y++)
  {
    for (unsigned x = 2; x < jmax * 2; x++)
    {
      if (upscale[x][y] == 2)
      {
        int wetcells = 0;
        int drycells = 0;
        int water = 0;
        int edge_cell_counter = 1;

        // sum up dry cells and edge cells -
        // now manhattan neighbors
        for (int dir = 1; dir <= 7; dir += 2)
        {
          int x2, y2;
          x2 = x + deltaX[dir];
          y2 = y + deltaY[dir];

          if (upscale[x2][y2] == 1) wetcells += 1;
          if (upscale[x2][y2] == 0) drycells += 1;
          if (upscale[x2][y2] == 2) edge_cell_counter += 1;
        }

        if (edge_cell_counter > 3) drycells += edge_cell_counter - 2;

        water = wetcells - drycells;
        upscale_edge[x][y] = water;
      }
    }
  }

  // now update normal edge array..
  for (unsigned x = 1; x <= jmax; x++)
  {
    for (unsigned y = 1; y <= imax; y++)
    {
      if (edge[x][y] == 0)
      {
        edge[x][y] = (double)(upscale_edge[(x * 2)][(y * 2)] +
            upscale_edge[(x * 2)][(y * 2) - 1] +
            upscale_edge[(x * 2) - 1][(y * 2)] +
            upscale_edge[(x * 2) - 1][(y * 2) - 1]);
        if (edge[x][y] > 2) edge[x][y] = 2; // important line to stop too great inside bends...
        if (edge[x][y] < -2) edge[x][y] = -2;

      }
    }
  }

  for (int n = 1; n <= edge_smoothing_passes+downstream_shift; n++)
  {
    #pragma omp parallel for
    for (unsigned y=2; y<imax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        unsigned x = down_scan[y][inc];

        edge_temp[x][y] = 0;
        if (x == 1) x++;
        if (x == jmax) x--;
        if (y == 1) break;
        if (y == imax) break;
        inc++;

        if (edge[x][y] > -9999)
        {
          double mean = 0;
          double num = 0;
          double water_flag = 0;

          // add in cell itself..
          mean += edge[x][y];
          num++;


          for (int dir = 1; dir <= 8; dir++)
          {
            int x2, y2;
            x2 = x + deltaX[dir];
            y2 = y + deltaY[dir];
            if (water_depth2[x2][y2] > mft) water_flag++;

            if ( n > edge_smoothing_passes && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft && mean_ws_elev(x2,y2)>mean_ws_elev(x,y))
            {
              //now to mean manhattan neighbours - only if they share a wet diagonal neighbour
              if ((std::abs(deltaX[dir]) + std::abs(deltaY[dir])) != 2)
              {
                if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
                    (water_depth2[x + 1][y - 1] > mft ||
                     water_depth2[x + 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
                    (water_depth2[x + 1][y + 1] > mft ||
                     water_depth2[x - 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
                    (water_depth2[x - 1][y - 1] > mft ||
                     water_depth2[x - 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
                    (water_depth2[x - 1][y - 1] > mft ||
                     water_depth2[x + 1][y - 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
              }

              //now non manahttan neighbours, with concected by a dry cell checked..
              else
              {
                if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
                    (water_depth2[x][y - 1] < mft ||
                     water_depth2[x - 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
                    (water_depth2[x][y - 1] < mft ||
                     water_depth2[x + 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
                    (water_depth2[x + 1][y] < mft ||
                     water_depth2[x][y + 1] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
                    (water_depth2[x][y + 1] < mft ||
                     water_depth2[x - 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
              }
            }

            else if ( n <= edge_smoothing_passes && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft)
            {
              //now to mean manhattan neighbours - only if they share a wet diagonal neighbour
              if ((std::abs(deltaX[dir]) + std::abs(deltaY[dir])) != 2)
              {
                if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
                    (water_depth2[x + 1][y - 1] > mft ||
                     water_depth2[x + 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
                    (water_depth2[x + 1][y + 1] > mft ||
                     water_depth2[x - 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
                    (water_depth2[x - 1][y - 1] > mft ||
                     water_depth2[x - 1][y + 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
                    (water_depth2[x - 1][y - 1] > mft ||
                     water_depth2[x + 1][y - 1] > mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
              }
              //now non manahttan neighbours, with concected by a dry cell checked..
              else
              {
                if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
                    (water_depth2[x][y - 1] < mft ||
                     water_depth2[x - 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
                    (water_depth2[x][y - 1] < mft ||
                     water_depth2[x + 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
                    (water_depth2[x + 1][y] < mft ||
                     water_depth2[x][y + 1] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
                if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
                    (water_depth2[x][y + 1] < mft ||
                     water_depth2[x - 1][y] < mft))
                {
                  mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
                  num++;
                }
              }
            }
          }
          if (mean != 0) edge_temp[x][y] = mean / num;

          //remove edge effects
          if (x < 3 || x > (jmax - 3)) edge_temp[x][y] = 0;
          if (y < 3 || y > (imax - 3)) edge_temp[x][y] = 0;

        }
      }
    }

    for (unsigned y = 2; y < imax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        unsigned x = down_scan[y][inc];
        //if (x == 1) x++;
        //if (x == jmax) x--;
        inc++;
        if (edge[x][y] > -9999)
        {
          edge[x][y] = edge_temp[x][y];
        }
      }
    }
  }

  // trial line to remove too high inside bends,,
  for (unsigned x = 1; x <= jmax; x++)
  {
    for (unsigned y = 1; y <= imax; y++)
    {
      if (edge[x][y] > -9999)
      {
        if (edge[x][y] > 0) edge[x][y] = 0;
        //if (edge[x][y] < -0.25) edge[x][y] = -0.25;
        edge[x][y] = 0 - edge[x][y];
        edge[x][y] = 1 / ((2.131 * std::pow(edge[x][y], -1.0794)) * DX);
        //if (edge[x][y] > (1 / (DX * 3))) edge[x][y] = 1 / (DX * 3);
        //edge[x][y] = 1 / edge[x][y];

      }
      if (water_depth[x][y] > water_depth_erosion_threshold && edge[x][y] == -9999) edge[x][y] = 0;
    }
  }

  //// now smooth across the channel..
  double tempdiff = 0;
  double counter = 0;
  do
  {
    counter++;
    // Parallelise outer loop
    for (unsigned y = 2; y < imax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        unsigned x = down_scan[y][inc];

        edge_temp[x][y] = 0;
        if (x == 1) x++;
        if (x == jmax) x--;
        inc++;
        if (water_depth2[x][y] > mft && edge[x][y] == -9999) edge[x][y] = 0;

        if (edge[x][y] > -9999 && water_depth2[x][y] > mft)
        {
          double mean = 0;
          int num = 0;
          for (int dir = 1; dir <= 8; dir+=2)
          {
            int x2, y2;
            x2 = x + deltaX[dir];
            y2 = y + deltaY[dir];

            if (water_depth2[x2][y2] > mft && edge[x2][y2] == -9999) edge[x2][y2] = 0;
            if (edge[x2][y2] > -9999)
            {
              mean += (edge[x2][y2]);
              num++;
            }
          }
          edge_temp[x][y] = mean / num;
        }
      }
    }

    tempdiff = 0;
    for (unsigned y = 2; y < imax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        unsigned x = down_scan[y][inc];
        if (x == 1) x++;
        if (x == jmax) x--;
        inc++;
        if (edge[x][y] > -9999 && water_depth2[x][y] > mft)
        {
          if (std::abs(edge[x][y] - edge_temp[x][y]) > tempdiff) tempdiff = std::abs(edge[x][y] - edge_temp[x][y]);
          edge[x][y] = edge_temp[x][y];
        }
      }
    }
  } while (tempdiff > lateral_cross_channel_smoothing); //this makes it loop until the averaging across the stream stabilises
  // so that the difference between the old and new values are < 0.0001

}

void LSDCatchmentModel::creep(double time)
{
  // creep rate is 10*-2 * slope per year, so inputs time jump in years*/
  // very important differnece here is that slide_GS() is called only if
  //      BOTH cells are not -9999 therfore if both have grainsize then do additions.
  //      this is to stop the progressive spread of selected cells upslope */

  unsigned  x,y;
  double temp;

  for(x=1;x<=imax;x++)
  {
    for(y=1;y<=jmax;y++)
    {
      tempcreep[x][y]=0;
    }
  }


  for(x=2;x<imax;x++)
  {
    for(y=2;y<jmax;y++)
    {
      if(elev[x][y]>bedrock[x][y])
      {
        if(elev[x][y-1]<elev[x][y]&&elev[x][y-1]> -9999)
        {
          temp = ((elev[x][y] - elev[x][y-1]) / DX) * CREEP_RATE * time / DX;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x][y-1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x, y - 1);
          }
        }
        if(elev[x+1][y-1]<elev[x][y]&&elev[x+1][y-1]> -9999)
        {
          temp=((elev[x][y]-elev[x+1][y-1])/ root)*CREEP_RATE*time/ root;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x+1][y-1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x+1, y - 1);
          }

        }
        if(elev[x+1][y]<elev[x][y]&&elev[x+1][y]> -9999)
        {
          temp=((elev[x][y]-elev[x+1][y])/ DX)*CREEP_RATE*time/ DX;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x+1][y]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x+1, y);
          }
        }
        if(elev[x+1][y+1]<elev[x][y]&&elev[x+1][y+1]> -9999)
        {
          temp=((elev[x][y]-elev[x+1][y+1])/ root)*CREEP_RATE*time/ root;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x+1][y+1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x+1, y + 1);
          }
        }
        if(elev[x][y+1]<elev[x][y]&&elev[x][y+1]> -9999)
        {
          temp=((elev[x][y]-elev[x][y+1])/ DX)*CREEP_RATE*time/ DX;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x][y+1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x, y + 1);
          }
        }
        if(elev[x-1][y+1]<elev[x][y]&&elev[x-1][y+1]> -9999)
        {
          temp=((elev[x][y]-elev[x-1][y+1])/ root)*CREEP_RATE*time/ root;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x-1][y+1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x-1, y + 1);
          }
        }
        if(elev[x-1][y]<elev[x][y]&&elev[x-1][y]> -9999)
        {
          temp=((elev[x][y]-elev[x-1][y])/ DX)*CREEP_RATE*time/ DX;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x-1][y]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x-1, y );
          }
        }
        if(elev[x-1][y-1]<elev[x][y]&&elev[x-1][y-1]> -9999)
        {
          temp=((elev[x][y]-elev[x-1][y-1])/ root)*CREEP_RATE*time/ root;
          if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
          if(temp<0)temp=0;
          tempcreep[x][y]-=temp;
          tempcreep[x-1][y-1]+=temp;
          if(index[x][y]!=-9999)
          {
            slide_GS(x, y, temp, x-1, y - 1);
          }
        }
      }
    }
  }

  for(x=1;x<=imax;x++)
  {
    for(y=1;y<=jmax;y++)
    {
      elev[x][y]+=tempcreep[x][y];
    }
  }

}

void LSDCatchmentModel::in_channel_landsliding()
{
  unsigned x,y,inc;
  double wet_factor;
  // This is a runtime constant, calculated every function call..!
  double factor=std::tan((failureangle*(3.141592654/180)))*DX;
  double diff=0;

  for(y=2;y<jmax;y++)
  {

    inc=1;
    // Only do on wetted cells and their neigbours
    while(down_scan[y][inc]>0)
    {

      x=down_scan[y][inc];
      if(x==imax)x=imax-1;
      if(x==1)x=2;

      inc++;
      // check to see if under water
      wet_factor=factor;
      //if(water_depth[x][y]>0.01)wet_factor=factor/2;
      if(elev[x][y]<=(bedrock[x][y]+active))wet_factor=10000;

      // chexk landslides in channel slowly

      if(((elev[x][y]-elev[x+1][y+1])/1.41)>wet_factor&&elev[x+1][y+1]> -9999)
      {
        diff=((elev[x][y]-elev[x+1][y+1])/1.41)-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x+1][y+1]+=diff;
        slide_GS(x,y,diff,x+1,y+1);
      }
      if((elev[x][y]-elev[x][y+1])>wet_factor&&elev[x][y+1]> -9999)
      {
        diff=(elev[x][y]-elev[x][y+1])-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x][y+1]+=diff;
        slide_GS(x,y,diff,x,y+1);
      }
      if(((elev[x][y]-elev[x-1][y+1])/1.41)>wet_factor&&elev[x-1][y+1]> -9999)
      {
        diff=((elev[x][y]-elev[x-1][y+1])/1.41)-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x-1][y+1]+=diff;
        slide_GS(x,y,diff,x-1,y+1);
      }
      if((elev[x][y]-elev[x-1][y])>wet_factor&&elev[x-1][y]> -9999)
      {
        diff=(elev[x][y]-elev[x-1][y])-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x-1][y]+=diff;
        slide_GS(x,y,diff,x-1,y);
      }

      if(((elev[x][y]-elev[x-1][y-1])/1.41)>wet_factor&&elev[x-1][y-1]> -9999)
      {
        diff=((elev[x][y]-elev[x-1][y-1])/1.41)-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x-1][y-1]+=diff;
        slide_GS(x,y,diff,x-1,y-1);
      }
      if((elev[x][y]-elev[x][y-1])>wet_factor&&elev[x][y-1]> -9999)
      {
        diff=(elev[x][y]-elev[x][y-1])-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x][y-1]+=diff;
        slide_GS(x,y,diff,x,y-1);
      }
      if(((elev[x][y]-elev[x+1][y-1])/1.41)>wet_factor&&elev[x+1][y-1]> -9999)
      {
        diff=((elev[x][y]-elev[x+1][y-1])/1.41)-wet_factor;
        if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
        elev[x][y]-=diff;
        elev[x+1][y-1]+=diff;
        slide_GS(x,y,diff,x+1,y-1);
      }

      if ((elev[x][y] - elev[x + 1][y]) > wet_factor && elev[x + 1][y] > -9999)
      {
        diff = (elev[x][y] - elev[x + 1][y]) - wet_factor;
        if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
        elev[x][y] -= diff;
        elev[x + 1][y] += diff;
        slide_GS(x, y, diff, x + 1, y);
      }

    }
  }

}

void LSDCatchmentModel::global_landsliding()
{
  unsigned x, y, inc=0;
  double wet_factor;
  double factor = std::tan((failureangle * (3.141592654 / 180))) * DX;
  double diff = 0;
  double total = 0;

  if (dunes_opt == true)
  {
    for (x = 1; x <= imax; x++)
    {
      for (y = 1; y <= jmax; y++)
      {
        elev[x][y] -= sand[x][y];
      }
    }
  }


  do
  {
    total = 0;
    inc++;
    for (x = 2; x < imax; x++)
    {
      for (y = 2; y < jmax; y++)
      {

        wet_factor = factor;
        if (elev[x][y] <= (bedrock[x][y] + active)) wet_factor = 10 * DX;

        // chexk landslides in channel slowly

        if (((elev[x][y] - elev[x + 1][y + 1]) / 1.41) > wet_factor && elev[x + 1][y + 1]> -9999)
        {
          diff = ((elev[x][y] - elev[x + 1][y + 1]) / 1.41) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x + 1][y + 1] += diff;
          total += diff;
        }
        if ((elev[x][y] - elev[x][y + 1]) > wet_factor && elev[x][y + 1]> -9999)
        {
          diff = (elev[x][y] - elev[x][y + 1]) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x][y + 1] += diff;
          total += diff;
        }
        if (((elev[x][y] - elev[x - 1][y + 1]) / 1.41) > wet_factor && elev[x - 1][y + 1]> -9999)
        {
          diff = ((elev[x][y] - elev[x - 1][y + 1]) / 1.41) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x - 1][y + 1] += diff;
          total += diff;
        }
        if ((elev[x][y] - elev[x - 1][y]) > wet_factor && elev[x - 1][y]> -9999)
        {
          diff = (elev[x][y] - elev[x - 1][y]) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x - 1][y] += diff;
          total += diff;
        }

        if (((elev[x][y] - elev[x - 1][y - 1]) / 1.41) > wet_factor && elev[x - 1][y - 1]> -9999)
        {
          diff = ((elev[x][y] - elev[x - 1][y - 1]) / 1.41) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x - 1][y - 1] += diff;
          total += diff;
        }
        if ((elev[x][y] - elev[x][y - 1]) > wet_factor && elev[x][y - 1]> -9999)
        {
          diff = (elev[x][y] - elev[x][y - 1]) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x][y - 1] += diff;
          total += diff;
        }
        if (((elev[x][y] - elev[x + 1][y - 1]) / 1.41) > wet_factor && elev[x + 1][y - 1]> -9999)
        {
          diff = ((elev[x][y] - elev[x + 1][y - 1]) / 1.41) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x + 1][y - 1] += diff;
          total += diff;
        }



        if ((elev[x][y] - elev[x + 1][y]) > wet_factor && elev[x + 1][y]> -9999)
        {
          diff = (elev[x][y] - elev[x + 1][y]) - wet_factor;
          if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
          if (diff > ERODEFACTOR) diff = ERODEFACTOR;
          elev[x][y] -= diff;
          elev[x + 1][y] += diff;
          total += diff;
        }

      }
    }
  } while (total > 0 && inc<200);

  if (dunes_opt == true)
  {
    for (x = 1; x <= imax; x++)
    {
      for (y = 1; y <= jmax; y++)
      {
        elev[x][y] += sand[x][y];
      }
    }
  }
}

void LSDCatchmentModel::soil_erosion(double time)
{
  // creep rate is 10*-2 * slope per year, so inputs time jump in years*/
  // very important differnece here is that slide_GS() is called only if
  //      BOTH cells are not -9999 therfore if both have grainsize then do additions.
  //      this is to stop the progressive spread of selected cells upslope */

  unsigned x, y;
  double temp;


  for (x = 1; x <= jmax; x++)
  {
    for (y = 1; y <= imax; y++)
    {
      tempcreep[x][y] = 0;
    }
  }

  for (x = 2; x < jmax; x++)
  {
    for (y = 2; y < imax; y++)
    {
      if (elev[x][y] > bedrock[x][y])
      {
        if (elev[x][y - 1] < elev[x][y] && elev[x][y - 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x][y - 1]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y]*DX*DX,0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x][y - 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x, y - 1);
          }
        }
        if (elev[x + 1][y - 1] < elev[x][y] && elev[x + 1][y - 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x + 1][y - 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x + 1][y - 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x + 1, y - 1);
          }

        }
        if (elev[x + 1][y] < elev[x][y] && elev[x + 1][y] > -9999)
        {
          temp = ((elev[x][y] - elev[x + 1][y]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x + 1][y] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x + 1, y);
          }
        }
        if (elev[x + 1][y + 1] < elev[x][y] && elev[x + 1][y + 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x + 1][y + 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x + 1][y + 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x + 1, y + 1);
          }
        }
        if (elev[x][y + 1] < elev[x][y] && elev[x][y + 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x][y + 1]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x][y + 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x, y + 1);
          }
        }
        if (elev[x - 1][y + 1] < elev[x][y] && elev[x - 1][y + 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x - 1][y + 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x - 1][y + 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x - 1, y + 1);
          }
        }
        if (elev[x - 1][y] < elev[x][y] && elev[x - 1][y] > -9999)
        {
          temp = ((elev[x][y] - elev[x - 1][y]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x - 1][y] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x - 1, y);
          }
        }
        if (elev[x - 1][y - 1] < elev[x][y] && elev[x - 1][y - 1] > -9999)
        {
          temp = ((elev[x][y] - elev[x - 1][y - 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
          if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
          if (temp < 0) temp = 0;
          tempcreep[x][y] -= temp;
          tempcreep[x - 1][y - 1] += temp;
          if (index[x][y] != -9999)
          {
            slide_GS(x, y, temp, x - 1, y - 1);
          }
        }
      }
    }
  }

  for (unsigned x = 1; x <= jmax; x++)
  {
    for (unsigned y = 1; y <= imax; y++)
    {
      elev[x][y] += tempcreep[x][y];
    }
  }
}

// all based on Van Walleghem et al., 2013 (JGR:ES)
void LSDCatchmentModel::soil_development()
{
  for (unsigned x = 1; x <= imax; x++)
  {
    for (unsigned y = 1; y <= jmax; y++)
    {
      if (elev[x][y] > -9999) // ensure it is not a no-data point
      {
        if (index[x][y] == -9999) addGS(x, y); // first ensure that there is grainsize defined for that cell

        if (bedrock_lower_opt == true) // Bedrock lowering
        {
          if (bedrock[x][y] > -9999 && elev[x][y]>=bedrock[x][y])
          {
            double h = elev[x][y] - bedrock[x][y];
            if (h == 0) h = 0.001;
            bedrock[x][y] += - P1 * std::exp(-b1 * (h)) / 12; //  / 12 to make it months
          }
        }

        if (physical_weather_opt == true) // Physical Weathering
        {
          // amt = k1 * std::exp(-c1*depth_below_surface) * (c2/std::log10(particle_size) * time;
          // start from top then work down - means no need to hold values in temp arrays...
          double Di = 0;
          int xyindex = index[x][y];


          for (unsigned n = 2; n <= (G_MAX - 1); n++)
          {
            if ((grain[xyindex][n] > 0.0))
            {
              switch (n)
              {
                case 1: Di = d1; break;
                case 2: Di = d2; break;
                case 3: Di = d3; break;
                case 4: Di = d4; break;
                case 5: Di = d5; break;
                case 6: Di = d6; break;
                case 7: Di = d7; break;
                case 8: Di = d8; break;
                case 9: Di = d9; break;
              }

              double amount = grain[xyindex][n] * ((-(k1 * std::exp(-c1 * active * 0.5) * (c2 / std::log(Di * 0.001)) * 1)) / 12); //  / 12 to make it months
              grain[xyindex][n] -= amount;
              if (n == 2)
              {
                grain[xyindex][n - 1] += amount;
              }
              else
              {
                grain[xyindex][n - 1] += amount * 0.05;
                grain[xyindex][n - 2] += amount * 0.95;
              }

              for (int z = 1; z <= 9; z++)
              {
                // What is this actually needed for? check original implementation
                double amount2 = strata[xyindex][z - 1][n] * ((-(k1 * std::exp(-c1 * active * z) * (c2 / std::log(Di * 0.001)) * 1)) / 12); //  / 12 to make it months

                strata[xyindex][z-1][n] -= amount;
                if (n == 2)
                {
                  strata[xyindex][z-1][n - 1] += amount;
                }
                else
                {
                  strata[xyindex][z-1][n - 1] += amount * 0.05;
                  strata[xyindex][z-1][n - 2] += amount * 0.95;
                }
              }
            }
          }
        }

        if (chem_weath_opt == true) // Chemical Weathering
        {
          // TO DO
        }
      }
    }
  }
}

void LSDCatchmentModel::grow_grass(double amount3)
{
  for(unsigned x=1; x<=imax; x++)
  {
    for(unsigned y=1; y<=jmax; y++)
    {
      //first check if veg is at 0.. not sure if this is needed now..
      if (veg[x][y][0] == 0)
      {
          veg[x][y][0] = elev[x][y];
      }

      // first check if under water or not..
      if (water_depth[x][y] < water_depth_erosion_threshold)
      {
        // if not then it
        // now adds to the amount of veg there..
        veg[x][y][1] += amount3;

        if(veg[x][y][1] > 1)
        {
          veg[x][y][1] = 1;
        }
      }

      // check if veg below elev! if so raise to elev
      if (veg[x][y][0] < elev[x][y]) // raises the veg level if it gets buried.. but only by a certain amount (0.001m day...)
      {
        veg[x][y][0] += 0.001; // this is an arbitrary amount..
        if (veg[x][y][0] > elev[x][y]) veg[x][y][0] = elev[x][y];
      }

      // check if veg above elev! if so lower to elev
      if (veg[x][y][0] > elev[x][y])
      {
        veg[x][y][0] -= 0.001; // this is an arbitrary amount..
        if (veg[x][y][0] < elev[x][y]) veg[x][y][0] = elev[x][y];
      }

      // trying to see now, if veg is above elev - so if lateral erosion happens - more than 0.1m the veg gets wiped..

      if (veg[x][y][0] - elev[x][y] > 0.1)
      {
        veg[x][y][1] = 0;
        veg[x][y][0] = elev[x][y];
      }

      // now see if veg under water - if so let it die back a bit..
      if(water_depth[x][y] > water_depth_erosion_threshold && veg[x][y][1] > 0)
      {
        veg[x][y][1] -= (amount3 / 2);
        if (veg[x][y][1] < 0)
        {
          veg[x][y][1] = 0;
          veg[x][y][0] = elev[x][y]; // resets elev if veg amt is 0
        }
      }

      // also if it is under sediment - then dies back a bit too...
      if (veg[x][y][0] < elev[x][y])
      {
        veg[x][y][1] -= (amount3 / 2);
        if (veg[x][y][1] < 0)
        {
          veg[x][y][1] = 0;
          veg[x][y][0] = elev[x][y]; // resets elev if veg amt is 0
        }
      }

      // but if it is under sediment, has died back to nearly 0 (0.05) then it resets the elevation
      // to the surface elev.
      if (veg[x][y][0] < elev[x][y] && veg[x][y][1] < 0.05)
      {
        veg[x][y][0] = elev[x][y];
      }
    }
  }
}

// PRINTS ALL PARAMETERS TO SCREEN FOR CHECKING
void LSDCatchmentModel::print_parameters()
{
  std::cout << "PARAMETERS FOR SIMULATION:" << std::endl;
  std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-="  << std::endl;

  std::cout << "MAX RUN DURATION:              " << maxcycle << std::endl;
  std::cout << "NO ROWS:                       " << imax << std::endl;
  std::cout << "NO COLS:                       " << jmax << std::endl;
  std::cout << "CELL SIZE                      " << DX << std::endl;

  std::cout << "WATER DEPTH EROSION THRESHOLD: " << water_depth_erosion_threshold << std::endl;
  std::cout << "WATER INPUT OUTPUT DIFF:       " << in_out_difference_allowed << std::endl;
  std::cout << "MANNINGS N:                    " << mannings << std::endl;
  std::cout << "NUMBER OF RAINFALL CELLS:      " << rfnum << std::endl;
  std::cout << "RAINDATA TIMESTEP:             " << rain_data_time_step << std::endl;
  std::cout << "TOPMODEL M:                    " << M << std::endl;
  std::cout << "COURANT NUMBER:                " << courant_number << std::endl;
  std::cout << "FROUDE NUMBER:                 " << froude_limit << std::endl;
  std::cout << "HORIZ FLOW THRESHOLD:          " << hflow_threshold << std::endl;

  // Grain distribution
  std::cout << "~~~~~~GRAIN SIZE DETAILS~~~~~~~" << std::endl;
  std::cout << "| PROP |" << " SIZE |" << "| FALL VELOCITY   |" << std::endl;
  std::cout << dprop[1] << " | " << d1 << " | " << fallVelocity[1] << std::endl;
  std::cout << dprop[2] << " | " << d2 << " | " << fallVelocity[2] << std::endl;
  std::cout << dprop[3] << " | " << d3 << " | " << fallVelocity[3] << std::endl;
  std::cout << dprop[4] << " | " << d4 << " | " << fallVelocity[4] << std::endl;
  std::cout << dprop[5] << " | " << d5 << " | " << fallVelocity[5] << std::endl;
  std::cout << dprop[6] << " | " << d6 << " | " << fallVelocity[6] << std::endl;
  std::cout << dprop[7] << " | " << d7 << " | " << fallVelocity[7] << std::endl;
  std::cout << dprop[8] << " | " << d8 << " | " << fallVelocity[8] << std::endl;
  std::cout << dprop[9] << " | " << d9 << " | " << fallVelocity[9] << std::endl;

  std::cout << "SEDIMENT LAW:                  ";
    if (einstein) std::cout << "Einstein" << std::endl;
    if (wilcock) std::cout  << "Wilcock and Crowe" << std::endl;

}
#endif
