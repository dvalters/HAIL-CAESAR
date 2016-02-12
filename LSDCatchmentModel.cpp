/// LSDCatchmentModel.cpp

/* LSDCatchmentModel is a 2D numerical model of landscape evolution that
 * simulates the processes and evolution
 * of catchments (river basins) and their hydrological and sedimentological
 * process over timescales of days to thousands of years.
 *
 * This is a C++ implementation of the original CAESAR-Lisflood model
 * (Coulthard et al. (2013)). This is a non-GUI version of the CL model. You
 * run it from the command line/terminal/console.
 *
 * It is integrated with the LSDTopoTools package and makes use of several
 * of the LSDTopoTools objects, such as LSDRaster in particular for reading
 * and writing raster data to and from the model. You might wish to use some
 * of the topographic analysis tools to analyse your model output.
 *
 * The hydrological component of the model is based on the Bates et al (2010)
 * algorithm of non-steady surface water flow, to represent the variation
 * in hydrological flow in a landscape under non-steady state hydrological
 * inputs.
 *
 * @author Declan Valters
 * @date  2014, 2015
 * University of Manchester
 * @contact declan.valters@manchester.ac.uk
 * @version 0.01
 *
 * Released under the GNU v2 Public License
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

// Only for the debug macro
#include <cstdio>

#include "LSDCatchmentModel.hpp"

// DV - One day, I'd like to integrate this more into the LSDTopoTools,
// particulalrly the LSDBasin object using it to 'cut out' basins,
// run hydrology sims for the catchment (hundreds - thousands yrs).
// and then perform topo analysis on the model run output
using std::string;

int DEBUG = 0;

#ifndef LSDCatchmentModel_CPP
#define LSDCatchmentModel_CPP

// ingest data tools
// DAV: I've copied these here for now to make the model self-contained for testing purposes
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Line parser for parameter files - JAJ 08/01/2014
// This might be better off somewhere else
//
// To be used on a parameter file of the format:
//  Name: 100       comments etc.
// Which sets parameter as "Name" and value as "100"
//
// This just does one line at a time; you need a wrapper function to get all
// the information out of the file
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::parse_line(std::ifstream &infile, string &parameter, string &value)
{
  char c;
  char buff[256];
  int pos = 0;
  int word = 0;

  while ( infile.get(c) )
  {
    if (pos >= 256)
    {
      std::cout << "Buffer overrun, word too long in parameter line: " << std::endl;
      std::string line;
      getline(infile, line);
      std::cout << "\t" << buff << " ! \n" << line << std::endl;
      exit(1);
    }
    // preceeding whitespace
    if (c == '#')
    {
      if (word == 0)
      {
        parameter = "NULL";
        value = "NULL";
      }
      if (word == 1)
        value = "NULL";
      word = 2;
    }

    if ((c == ' ' || c == '\t') && pos == 0)
      continue;
    else if ( (c == ':' && word == 0) || ( (c == ' ' || c == '\n' || c == '\t') && word == 1))
    {
      while (buff[pos-1] == ' ' || buff[pos-1] == '\t')
        --pos;      // Trailing whitespace
      buff[pos] = '\0';   // Append Null char
      if (word == 0)
        parameter = buff;   // Assign buffer contents
      else if (word == 1)
        value = buff;
      ++word;
      pos = 0;        // Rewind buffer
    }
    else if ( c == '\n' && word == 0 )
    {
      parameter = "NULL";
      buff[pos] = '\0';
      value = buff;
      ++word;
    }
    else if (word < 2)
    {
      buff[pos] = c;
      ++pos;
    }

    if (c == '\n')
      break;
  }
  if (word == 0)
  {
    parameter = "NULL";
    value = "NULL";
  }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
// This function removes control characters from the end of a string
// These get introduced if you use the DOS format in your parameter file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
std::string LSDCatchmentModel::RemoveControlCharactersFromEndOfString(std::string toRemove)
{
  int len =  toRemove.length();
  if(len != 0)
  {
    if (iscntrl(toRemove[len-1]))
    {
      //cout << "Bloody hell, here is another control character! Why Microsoft? Why?" << endl;
      toRemove.erase(len-1);
    }
  }
  return toRemove;
}

// Wee function to check if file exists
// Not sure if this works in Windows...must test sometime
inline bool LSDCatchmentModel::does_file_exist(const std::string &filename)
{
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) ==0);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// CREATE FUCNTIONS
// These define what happens when an LSDCatchmentModel object is created
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::create()
{
  std::cout << "You are trying to create an LSDCatchmentModel object with no supplied files or parameters." << std::endl << "Exiting..." << std::endl;
  exit(EXIT_FAILURE);
}

void LSDCatchmentModel::create(string pname, string pfname)
{
  std::cout << "Creating an instance of LSDCatchmentModel.." << std::endl;
  // Using the parameter file
  initialise_variables(pname, pfname);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Load the data from the text file
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Generic function for reading rainfal data
// This is used by the LSDCatchmentModel.
// Need to think about this...the number of rows and cols are not known beforehand
std::vector< std::vector<float> > LSDCatchmentModel::read_rainfalldata(string FILENAME)
{
  std::cout << "\n\n Loading Spatially Distributed Rainfall File, the filename is "
            << FILENAME << std::endl;

  // open the data file
  std::ifstream infile(FILENAME.c_str());

  std::string line;
  //std::vector< std::vector<float> > raingrid; //declared in .hpp now
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
  return raingrid;  // You(I) forgot the return statement! - DV
  // It's odd that the compiler allows no return statement.
}
/*
std::vector<float> LSDCatchmentModel::read_rainfall_uniform(string FILENAME)
{
  std::cout << "\n\n Loading Uniform Rainfall File, the filename is "
            << FILENAME << std::endl;

  std::ifstream infile(FILENAME.c_str());

  if (infile)
  {
    float value;
    while (infile >> value))
    {
      raingrid.push_back(value)
    }
  }
  else
  {
    std::cout << "There was an error opening your uniform rainfall file." <<
    std::endl;
  }
}
*/

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

void LSDCatchmentModel::initialise_model_domain_extents()
{
  std::string FILENAME = read_fname + dem_read_extension;

  if (!does_file_exist(FILENAME))
  {
    std::cout << "No terrain DEM found by name of: " << FILENAME << std::endl
              << "You must supply a correct path and filename in the input parameter file" << std::endl;
    std::cout << "The model domain cannot be intitialised for real topography\n \
                 without a valid DEM to read from" << std::endl;
                 exit(EXIT_FAILURE);
  }
  try
  {
    std::cout << "\n\nLoading DEM header info, the filename is " << FILENAME << std::endl;

    // open the data file
    std::ifstream data_in(FILENAME.c_str());

    //Read in raster data
    std::string str;            // a temporary string for discarding text

    // read the georeferencing data and metadata
    data_in >> str >> xmax;
    std::cout << "NCols: " << xmax << " str: " << std::endl;
    data_in >> str >> ymax;
    std::cout << "NRows: " << ymax << " str: " << std::endl;
    data_in >> str >> xll
            >> str >> yll
            >> str >> DX // cell size or grid resolution
            >> str >> no_data_value;
  }
  catch(...)
  {
    std::cout << "Something is wrong with your initial elevation raster file." << std::endl
              << "Common causes are: " << std::endl << "1) Data type is not correct" <<
                 std::endl << "2) Non standard raster format" << std::endl;
    exit(EXIT_FAILURE);
  }

}

void LSDCatchmentModel::load_data()
{
  std::string FILENAME = read_fname + dem_read_extension;

  if (!does_file_exist(FILENAME))
  {
    std::cout << "No terrain DEM found by name of: " << FILENAME << std::endl
              << "You must supply a correct path and filename in the input parameter file" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read in the elevation raster data from file, setting the elevation LSDRaster
  // object, 'elevR'
  try
  {
    elevR.read_ascii_raster(FILENAME);
    // You have now read in all the headers and the raster data
    // Headers are accessed by elevR.get_Ncols(), elevR.get_NRows() etc
    // Raster is accessed by elevR.get_RasterData_dbl() (type: TNT::Array2D<double>)

    // Load the raw ascii raster data
    TNT::Array2D<double> raw_elev = elevR.get_RasterData_dbl();

    // We want an edge pixel of zeros surrounding the raster data
    // So start the counters at one, rather than zero, this
    // will ensure that elev[0][n] is not written to and left set to zero.
    // remember this data member is set with dim size equal to xmax + 2 to
    // allow the border of zeros

    for (int i=0; i<ymax; i++)
    {
      for (int j=0; j<xmax; j++)
      {
        elev[i+1][j+1] = raw_elev[i][j];
      }
    }

    // Check that there is an outlet for the catchment water
    check_DEM_edge_condition();

    // deep copy needed? -- DAV 2/12/2015
    init_elevs = elev;

    //ymax = elevR.get_NRows();
    //xmax = elevR.get_NCols();
    //std::cout << "YMAX: " << ymax << std::endl;
    //std::cout << "XMAX: " << xmax << std::endl;
  }
  catch(...)
  {
    std::cout << "Something is wrong with your initial elevation raster file." << std::endl
              << "Common causes are: " << std::endl << "1) Data type is not correct" <<
                 std::endl << "2) Non standard raster format" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Load the HYDROINDEX DEM
  if (spatially_var_rainfall == true)
  {
    FILENAME = hydroindex_fname;
    // Check for the file first of all
    if (!does_file_exist(FILENAME))
    {
      std::cout << "No hydroindex DEM found by name of: " << FILENAME << std::endl << "You specified the spatially variable rainfall option, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    try
    {
      hydroindexR.read_ascii_raster(FILENAME);
      rfarea = hydroindexR.get_RasterData_int();
      std::cout << "The hydroindex: " << FILENAME << " was successfully read." << std::endl;
    }
    catch (...)
    {
      std::cout << "Something is wrong with your hydroindex file." << std::endl
                << "Common causes are: " << std::endl << "1) Data type is not integer" <<
                   std::endl << "2) Non standard ASCII data format" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Load the BEDROCK DEM
  if (bedrock_layer_on == true)
  {
    FILENAME = bedrock_data_file;
    // Check for the file first of all
    if (!does_file_exist(FILENAME))
    {
      std::cout << "No bedrock DEM found by name of: " << FILENAME << std::endl << "You specified to use a separate bedrock layer option, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    try
    {
      bedrockR.read_ascii_raster(FILENAME);
      bedrock = bedrockR.get_RasterData_dbl();
      std::cout << "The bedrock file: " << FILENAME << " was successfully read." << std::endl;
    }
    catch (...)
    {
      std::cout << "Something is wrong with your bedrock file." << std::endl
                << "Common causes are: " << std::endl << "1) Data type is not correct" <<
                   std::endl << "2) Non standard ASCII data format" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Load the RAINDATA file
  // Remember the format is not the same as a standard ASCII DEM...
  if (rainfall_data_on==true)
  {
    FILENAME = rainfall_data_file;
    // Check for the file first of all
    if (!does_file_exist(FILENAME))
    {
      std::cout << "No rainfall data file found by name of: " << FILENAME << std::endl << "You specified to use a rainfall input file, \
                   \n but no matching file was found. Try again." << std::endl;
                   exit(EXIT_FAILURE);
    }
    std::cout << "Ingesting rainfall data file: " << FILENAME << " into hourly_rain_data" << std::endl;
    hourly_rain_data = read_rainfalldata(FILENAME);

    // debug
    if (DEBUG = 1) print_rainfall_data();

  }
  // INCOMPLETE! See the read_rainfalldata() method...

  // DAV: The grainsize file is slightly more complicated...
  // This needs some more thought...
  // Load the GRAINSIZE DEM
  //~ if (CM_support_file_names["grain_data_file"] == true)
  //~ {
  //~ FILENAME = CM_support_file_names["grain_data_file"];
  //~ // Check for the file first of all
  //~ if (!does_file_exist(FILENAME))
  //~ {
  //~ std::cout << "No grain data DEM found by name of: " << FILENAME << std::endl << "You specified to use a grain data option, \
  //~ \n but no matching file was found. Try again." << std::endl;
  //~ exit(EXIT_FAILURE);
  //~ }
  //~ try
  //~ {
  //~ grainR.read_ascii_raster(FILENAME);
  //~ bedrock = grainR.get_RasterData_dbl();
  //~ std::cout << "The grainsize file: " << FILENAME << " was successfully read." << std::endl;
  //~ }
  //~ catch (...)
  //~ {
  //~ std::cout << "Something is wrong with your grainsize file." << std::endl
  //~ << "Common causes are: " << std::endl << "1) Data type is not correct" <<
  //~ std::endl << "2) Non standard ASCII data format" << std::endl;
  //~ exit(EXIT_FAILURE);
  //~ }
  //~ }

}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function gets all the data from a parameter file
//
// Update: It also intialises the other params that are set internally (hard coded)
// Some functions have been taken out of mainloop()
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::initialise_variables(std::string pname, std::string pfname)
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

    //std::cout << "parameter is: " << lower << " and value is: " << value << std::endl;

    // get rid of control characters
    value = RemoveControlCharactersFromEndOfString(value);

    if (lower == "dem_read_extension")
    {
      dem_read_extension = value;
      dem_read_extension = RemoveControlCharactersFromEndOfString(dem_read_extension);
      std::cout << "dem_read_extension: " << dem_read_extension << std::endl;
    }
    else if (lower == "dem_write_extension")
    {
      dem_write_extension = value;
      dem_write_extension = RemoveControlCharactersFromEndOfString(dem_write_extension);
      std::cout << "dem_write_extension: " << dem_write_extension << std::endl;
    }
    else if (lower == "write_path")
    {
      write_path = value;
      write_path = RemoveControlCharactersFromEndOfString(write_path);
      std::cout << "dem_read_extension: " << dem_read_extension << std::endl;
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
    else if (lower == "max_time_step")
    {
      max_time_step = atoi(value.c_str());
      std::cout << "max_time_step: " << max_time_step << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Output and Save Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "timeseries_save_interval")
    {
      saveinterval = atoi(value.c_str());
      std::cout << "timeseries save interval: " << output_file_save_interval << std::endl;
    }
    else if (lower == "raster_output_interval")
    {
      timeseries_interval = atoi(value.c_str());
      std::cout << "raster_output_interval: " << saveinterval << std::endl;
    }
    else if (lower == "elevation_file")
    {
      elev_fname = value;
      std::cout << "elev_fname: " << elev_fname << std::endl;
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

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Sediment
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
      else std::cout << "No sedi transport law specified in parameter file..." << std::endl;

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
      std::cout << "recirculate_proportion: " << recirculate_proportion << std::endl;
    }
    else if (lower == "lateral_erosion_on")
    {
      lateral_erosion_on = (value == "yes") ? true : false;
      std::cout << "lateral_erosion_on: " << lateral_erosion_on << std::endl;
    }
    //else if (lower == "lateral_ero_rate") lateral_ero_rate = atof(value.c_str());
    else if (lower == "edge_filter_passes")
    {
      smoothing_times = atof(value.c_str());
      std::cout << "smoothing_times: " << smoothing_times << std::endl;
    }
    else if (lower == "cells_shift_lat")
    {
      downstream_shift = atof(value.c_str());
      std::cout << "downstream_shift: " << downstream_shift << std::endl;
    }
    else if (lower == "max_diff_cross_chann")
    {
      lateral_cross_channel_smoothing = atof(value.c_str());
      std::cout << "lateral_cross_channel_smoothing: " << lateral_cross_channel_smoothing << std::endl;
    }
    else if (lower == "erosion_limit")
    {
      ERODEFACTOR = atof(value.c_str());
      std::cout << "erosion limit per timestep: " << ERODEFACTOR << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Grain Size Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "suspended_sediment_on")
    {
      suspended_opt = (value == "yes") ? true : false;
      std::cout << "suspended_opt: " << suspended_opt << std::endl;
    }

    // This is a vector of values and needs some thought (different fractions)
    //else if (lower == "fall_velocity") fall_velocity = atof(value.c_str());

    else if (lower == "grain_size_frac_file")
    {
      grain_data_file = value;
      std::cout << "grain_data_file: " << grain_data_file << std::endl;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Hydrology and Flow
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "rainfall_data_on")
    {
      rainfall_data_on = (value == "yes") ? true : false;
      std::cout << "rainfall_data_on: " << rainfall_data_on << std::endl;
    }

    else if (lower == "TOPMODEL_m_value")
    {
      M = atof(value.c_str());
      std::cout << "TOPMODEL m value: " << M << std::endl;
    }

    else if (lower == "num_unique_rain_cells")
    {
      rfnum = atoi(value.c_str()); // not needed now
      std::cout << "Number of unique rain cells: " << rfnum << std::endl;
    }

    else if (lower == "rain_data_time_step")
    {
      rain_data_time_step = atof(value.c_str());
      std::cout << "rainfall data time step: " << rain_data_time_step << std::endl;
    }

    else if (lower == "spatial_var_rain")
    {
      spatially_var_rainfall = (value == "yes") ? true : false;
      std::cout << "Spatially variable rainfall: " << spatially_var_rainfall << std::endl;
    }

    else if (lower == "in_out_diff")
    {
      in_out_difference = atof(value.c_str());
      std::cout << "in-output difference allowed (cumecs): " << in_out_difference << std::endl;
    }

    else if (lower == "min_Q_for_depth_calc")
    {
      MIN_Q = atof(value.c_str());
      std::cout << "minimum discharge for depth calculation: " << MIN_Q << std::endl;
    }

    else if (lower == "max_Q_for_depth_calc")
    {
      MIN_Q_MAXVAL = atof(value.c_str());
      std::cout << "max discharge for depth calc: " << MIN_Q_MAXVAL << std::endl;
    }

    else if (lower == "hflow_threshold")
    {
      hflow_threshold = atof(value.c_str());
      std::cout << "Horizontal flow threshold: " << hflow_threshold << std::endl;
    }

    else if (lower == "water_depth_erosion_threshold")
    {
      hflow_threshold = atof(value.c_str());
      std::cout << "Water depth for erosion threshold: " << water_depth_erosion_threshold << std::endl;
    }

    else if (lower == "slope_on_edge_cell")
    {
      edgeslope = atof(value.c_str());
      std::cout << "Slope on model domain edge: " << edgeslope << std::endl;
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
      std::cout << "Veggie grow rate: " << grain_data_file << std::endl;
    }

    else if (lower == "vegetation_crit_shear")
    {
      vegTauCrit = atof(value.c_str());
      std::cout << "Vegggie crit shear: " << vegTauCrit << std::endl;
    }

    else if (lower == "veg_erosion_prop")
    {
      veg_lat_restriction = atof(value.c_str());
      std::cout << "Veggie erosion proportion: " << veg_lat_restriction << std::endl;
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
      std::cout << "Slope critical failure angle: " << failureangle << std::endl;
    }

    else if (lower == "soil_erosion_rate")
    {
      SOIL_RATE = atof(value.c_str());
      std::cout << "Soil erosion rate: " << SOIL_RATE << std::endl;
    }

    else if (lower == "soil_j_mean_depends_on")
    {
      soil_j_mean_depends_on = (value == "yes") ? true:false;
      std::cout << "Soil erosion rate depends on J mean: " << soil_j_mean_depends_on << std::endl;
    }
    //else if (lower == "call_muddpile_model") call_muddpile_model = atof(value.c_str());

  }

  std::cout << "No other parameters found, paramter ingestion complete." << std::endl;
  std::cout << "Initialising hard coded-constants." << std::endl;

  // Initialise the other parameters (those not set by param file)

}

// Initialise the arrays (as done in initialise() )
// Not sure the point of having them declared on header file if you
// can't resize them...surely this is duplicating array creation?? DV

// These need moving - initialise_params() is called before load_data()
// but load_data() determines array sizes based on DEM hdr info...epic programmer fail - DV
// i.e. don't know xmax and ymax at start of this function call in program flow.
void LSDCatchmentModel::initialise_arrays()
{
  std::cout << "Cartesian ymax (no. of rows): " << ymax << \
               " Cartesian xmax (no. of cols): " << xmax << std::endl;

  elev = TNT::Array2D<double> (ymax+2,xmax+2, 0.0);
  water_depth = TNT::Array2D<double> (ymax+2,xmax+2, 0.0);

  // Cast to int and then double, what?
  //old_j_mean_store = new double[(int)((maxcycle*60)/input_time_step)+10];
  old_j_mean_store = std::vector<double> (static_cast<int>((maxcycle*60)/input_time_step)+10);

  qx = TNT::Array2D<double> (ymax + 2, xmax + 2, 0.0);
  qy = TNT::Array2D<double> (ymax + 2, xmax + 2, 0.0);

  qxs = TNT::Array2D<double> (ymax + 2, xmax + 2, 0.0);
  qys = TNT::Array2D<double> (ymax + 2, xmax + 2, 0.0);

  Vel = TNT::Array2D<double> (ymax + 2, xmax + 2, 0.0);

  area = TNT::Array2D<double> (ymax+2, xmax + 2, 0.0);
  index = TNT::Array2D<int> (ymax +2, xmax + 2, 0);
  elev_diff = TNT::Array2D<double> (ymax + 2, xmax + 2);

  bedrock = TNT::Array2D<double> (ymax+2, xmax+2);
  tempcreep = TNT::Array2D<double> (ymax+2,xmax+2);
  init_elevs = TNT::Array2D<double> (ymax+2,xmax+2);

  vel_dir = TNT::Array3D<double> (ymax+2, xmax+2, 9);
  strata = TNT::Array3D<double> ( ((ymax+2)*(xmax+2))/LIMIT , 10, G_MAX+1);
  veg = TNT::Array3D<double> (ymax+1, xmax+1, 4);

  grain = TNT::Array2D<double> ( ((2+ymax)*(xmax+2))/LIMIT, G_MAX+1 );
  cross_scan = TNT::Array2D<int> (ymax+2,xmax+2, 0);
  down_scan = TNT::Array2D<int> (xmax+2, ymax+2, 0);

  // line to stop max time step being greater than rain time step
  if (rain_data_time_step < 1) rain_data_time_step = 1;
  if (max_time_step / 60 > rain_data_time_step) max_time_step = static_cast<int>(rain_data_time_step) * 60;

  // see StackOverflow for how to set size of nested vector
  // http://stackoverflow.com/questions/2665936/is-there-a-way-to-specify-the-dimensions-of-a-nested-stl-vector-c
  hourly_rain_data = std::vector< std::vector<float> > ( (static_cast<int>(maxcycle * (60 / rain_data_time_step)) + 100), vector<float>(rfnum+1) );

  hourly_m_value = std::vector<double> (static_cast<int>(maxcycle * (60 / rain_data_time_step)) + 100);

  // erm...what? Best just leave it in for now...
  // magic numbers FTW
  // Does this represent 10000 years? - DV
  climate_data = TNT::Array2D<double> (10001, 3);

  temp_grain = std::vector<double> (G_MAX+1);

  inputpointsarray = TNT::Array2D <bool> (ymax + 2, xmax + 2);

  edge = TNT::Array2D<double> (ymax+1,xmax+1);
  edge2 = TNT::Array2D<double> (ymax+1,xmax+1);

  Tau = TNT::Array2D<double> (ymax+2,xmax+2);

  catchment_input_x_coord = std::vector<int> (xmax * ymax);
  catchment_input_y_coord = std::vector<int> (xmax * ymax);

  area_depth = TNT::Array2D<double> (ymax + 2, xmax + 2);

  //dune things - but not implemented in this version
  /*
    dune_mult = (int)(DX)/int.Parse(dune_grid_size_box.Text);
    if (dune_mult < 1) dune_mult = 1;
    if (DuneBox.Checked == false) dune_mult = 1; // needed in order to stop it tripping out the memory

    area_depth = new double[xmax + 2, ymax + 2];
    sand = new double[xmax + 2, ymax + 2];
    elev2 = new double[(xmax * dune_mult) + 2, (ymax*dune_mult) + 2];
    sand2 = new double[(xmax * dune_mult) + 2, (ymax * dune_mult) + 2];
    */

  dischargeinput = TNT::Array2D<double> (1000,5);

  hydrograph = TNT::Array2D<double> ( (maxcycle -(static_cast<int>(cycle/60))) + 1000, 2);

  Vsusptot = TNT::Array2D<double> (ymax+2,xmax+2);

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
  j = std::vector<double> (rfnum + 1); //0.000000001; //start of hydrological model paramneters
  jo = std::vector<double> (rfnum + 1);//0.000000001;
  j_mean = std::vector<double> (rfnum + 1);
  old_j_mean = std::vector<double> (rfnum + 1);
  new_j_mean = std::vector<double> (rfnum + 1);
  rfarea = TNT::Array2D<int> (ymax + 2, xmax + 2);
  nActualGridCells = std::vector<int> (rfnum + 1);
  catchment_input_counter = std::vector<int> (rfnum + 1);

  sr = TNT::Array3D<double> (ymax + 2, xmax + 2, 10);
  sl = TNT::Array3D<double> (ymax + 2, xmax + 2, 10);
  su = TNT::Array3D<double> (ymax + 2, xmax + 2, 10);
  sd = TNT::Array3D<double> (ymax + 2, xmax + 2, 10);
  ss = TNT::Array2D<double> (ymax + 2, xmax + 2);

  // Initialise suspended fraction vector
  // Tells program whether sediment is suspended fraction or not
  isSuspended = std::vector<bool>(G_MAX+1, false);
  fallVelocity = std::vector<double>(G_MAX+1, 0.0);
  set_fall_velocities();

  // Segfaults here because you are trying to zero a load of
  // zero length arrays

  zero_values();
  /*
    time_1=1;

    std::cout << "Calulating J.." << std::endl;
    calc_J(1.0);

    save_time=cycle;
    creep_time=cycle;
    creep_time2 = cycle;
    soil_erosion_time = cycle;
    soil_development_time = cycle;
    time_1=cycle;

    std::cout << "Get_area().." << std::endl;
    get_area();

    std::cout << "Get_catchment_input_points().." << std::endl;
    get_catchment_input_points();
    time_factor = 1;
  **/
}

// Hard coded! - Should make this read from sed details file.
// DAV 3/12/2015
void LSDCatchmentModel::set_fall_velocities()
{
  fallVelocity[0] = 0.0;
  fallVelocity[1] = 0.066;
  fallVelocity[2] = 0.109;
  fallVelocity[3] = 0.164;
  fallVelocity[4] = 0.237;
  fallVelocity[5] = 0.338;
  fallVelocity[6] = 0.479;
  fallVelocity[7] = 0.678;
  fallVelocity[8] = 0.959;
  fallVelocity[9] = 1.357;
}

void LSDCatchmentModel::get_area()
{
  // gets the drainage area
  // Could possible come from one of the other LSDRaster objects

  for(int i=0; i<=ymax; i++)
  {
    for(int j=0; j<=xmax; j++)
    {
      area_depth[i][j]=1;
      area[i][j] = 0;
      if (elev[i][j] == -9999)
      {
        area_depth[i][j] = 0.0;
      }

    }
  }
  get_area4();   // This refers to a newer area getting method in CL
}

void LSDCatchmentModel::get_area4()
{

  // new routine for determining drainage area 4/10/2010
  // instead of using sweeps this sorts all the elevations then works frmo the
  // highest to lowest - calculating drainage area - D-infinity basically.

  int i, j, n, i2, j2, dir;

  // zero load of arrays
  std::vector<double> tempvalues((ymax+2) *(xmax+2));
  std::vector<double> tempvalues2((ymax+2) * (xmax+2));
  std::vector<double> xkey((ymax+2)*(xmax+2));
  std::vector<double> ykey((ymax+2)*(xmax+2));

  // I leave in the old C# syntax for now for reference (note the subtle differences) (DAV)
  //tempvalues = new Double [(xmax+2) *(ymax+2)];
  //tempvalues2 = new Double[(xmax+2) * (ymax+2)];
  //xkey = new Double [(xmax+2)*(ymax+2)];
  //ykey = new Double[(xmax+2) * (ymax+2)];

  // then create temp array based on elevs then also one for x values.
  int inc = 1;
  for (i = 1; i <= ymax; i++)
  {
    for (j = 1; j <= xmax; j++)
    {
      // tempvalues is just a list of all the elevations in the grid.
      tempvalues[inc] = elev[i][j];
      // xkey is the xcoordinates collated
      xkey[inc] = j; // because [row][column] order, so j is the x-coord
      inc++;
    }
  }

  // then sorts according to elevations - but also sorts the key (xkey) according to
  // these too..

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
  // needed.

  // Pair the vectors up
  std::vector<std::pair<double,double> >
      x_keys_elevs_paired(xkey.size() < tempvalues.size() ? xkey.size() : tempvalues.size() );

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
  for (i = 1; i <= ymax; i++)
  {
    for (j = 1; j <= xmax; j++)
    {
      tempvalues2[inc] = elev[i][j];
      ykey[inc] = i;  // row-major, so i is the y coordinate (the row of NRows)
      inc++;
    }
  }
  // Create a suitably sized vector to hold the pairs
  std::vector<std::pair<double,double> >
      y_keys_elevs_paired(ykey.size() < tempvalues2.size() ? ykey.size() : tempvalues2.size() );


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

  //Array.Sort(tempvalues2, ykey);
  // The old C# version
  //(Why write one-line in C# when you can write 8 in C++?)

  // Hang on, do these need to be unpaired now? - DAV 1/12/2015
  // Yes! - you are using the unsorted xkeys/ykeys below


  // then works through the list of x and y co-ordinates from highest to lowest...
  for (n = (xmax * ymax); n >= 1; n--)
  {
    //x = (int)(xkey[n]); 1/12/15
    //this.InfoStatusPanel.Text = Convert.ToString(x);
    //y = (int)(ykey[n]); 1/12/15
    //this.InfoStatusPanel.Text = Convert.ToString(y);

    // need to extract the x/y keys from the pair
    //x = static_cast<int>(x_keys_elevs_paired[n].first );
    //y = static_cast<int>(y_keys_elevs_paired[n].first );

    // 5th attempt using std::get<i>
    j = static_cast<int>(std::get<0>(x_keys_elevs_paired[n]) );
    i = static_cast<int>(std::get<0>(y_keys_elevs_paired[n]) );


    // I.e. If we are in the catchment (area_depth = 0 in NODATA)
    if (area_depth[i][j] > 0)
    {
      // update area if area_depth is higher
      // That is, set the area to a value if in catchment
      if (area_depth[i][j] > area[i][j]) area[i][j] = area_depth[i][j];

      double difftot = 0;

      // work out sum of +ve slopes in all 8 directions
      for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
      {
        j2 = j + deltaX[dir];
        i2 = i + deltaY[dir];

        if (j2 < 1) j2 = 1;
        if (i2 < 1) i2 = 1;
        if (j2 > xmax) j2 = xmax;
        if (i2 > ymax) i2 = ymax;

        // swap comment lines below for drainage area from D8 or Dinfinity
        // Calculates the total drop (difference) in elevation of surrounding cells
        // cumulatively.
        // D8
        if (std::remainder(dir, 2) != 0)
        {
          if (elev[i2][j2] < elev[i][j]) difftot += elev[i][j] - elev[i2][j2];
        }
        else
        {
          if (elev[i2][j2] < elev[i][j]) difftot += (elev[i][j] - elev[i2][j2]) / 1.414;
        }
        //if(elev[x][y]-elev[x2][y2]>difftot) difftot=elev[x][y]-elev[x2][y2];
      }

      // Will there be any flow distribution?
      if (difftot > 0)
      {
        //if (DEBUG = 1) std::cout << "Distribution of flow! : " << difftot << std::endl;
        // then distribute to all 8...
        for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
        {
          // Calculate the adjacent coordinates
          i2 = i + deltaX[dir];
          j2 = j + deltaY[dir];

          // Make sure we don't go over the edges of the model domain
          if (j2 < 1) j2 = 1;
          if (i2 < 1) i2 = 1;
          if (j2 > xmax) j2 = xmax;
          if (i2 > ymax) i2 = ymax;

          // swap comment lines below for drainage area from D8 or Dinfinity

          if (std::remainder(dir, 2) != 0)
          {
            if (elev[i2][j2] < elev[i][j]) area_depth[i2][j2] += area_depth[i][j] * ((elev[i][j] - elev[i2][j2]) / difftot);
          }
          else
          {
            if (elev[i2][j2] < elev[i][j]) area_depth[i2][j2] += area_depth[i][j] * (((elev[i][j] - elev[i2][j2])/1.414) / difftot);
          }

          //if (elev[x][y] - elev[x2][y2] == difftot) area_depth[x2][y2] += area_depth[x][y];
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
// need a bool option here to check if it is a flow only run
// EDIT: No you don't - If flow only, then just don't call the erosion method! (in driver file) Separate qroute/depth etc

// CALC EROSION MULTIPLIER
void LSDCatchmentModel::call_erosion()
{
  if (counter >= erode_call)   // erode_call is by default zero, so don't call erosion before water has been routed.
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
    lateral3();   // call the actual erosion function
    lateralcounter = counter + (50 * erode_mult);
  }
}


// CARRY OUT SOIL CREEP, GRASS-GROWING ETC
//

// BOOL option for flow only...

// CARRY OUT LOCAL LANDSLIDES EVERY X ITERATIONS...
void LSDCatchmentModel::call_landsliding()
{
  if (std::remainder(counter, 10) == 0)
  {
    slide_3();   // slide_3 in CL is the implemented land sliding function
  }
}

// DO THE SOIL CREEP SHUFFLE!
void LSDCatchmentModel::call_soilcreep()
{
  if (cycle > creep_time)
  {
    // update the counter creep_time
    creep_time += 14400;    // 10 days if minutes (or four hours if seconds!)
    // call the creep function
    creep(0.028);
  }
}

// There is a sand dunes function but I am not implementing that here - DAV

// NOW CALL THE SOIL EROSION FUNCTION
void LSDCatchmentModel::call_soil_erosion()
{
  if (SOIL_RATE > 0 && cycle > soil_erosion_time)
  {
    get_area();    // gets the drainage area before doing the maths below. Useful to have fresh D.A.
    soil_erosion_time += 1440;  // do soil erosion daily
    soil_erosion(0.0028);
  }
}

// DEVELOP THE SOIL
void LSDCatchmentModel::call_soil_devel()
{
  if (cycle > soil_development_time)
  {
    soil_development_time += 1440 * 365/12;
    if (soildevoption == true)
      soil_development();
  }
}

// MORE SLOPE PROCESSES
void LSDCatchmentModel::call_evapotrans()
{
  if (cycle > creep_time2)
    evaporate(1440);
  // update the creep_time2
  creep_time2 += 1440; // daily (1440 minutes in a day)
}
void LSDCatchmentModel::call_slide5()   // not exactly sure what slide_5 does differently to slide_3: not commented well in the original code!
{
  // update the creep_time2
  creep_time3 += 1440; // daily (1440 minutes in a day)  // NEED A SEPARATE COUTNER FOR THIS CREEP!
  // BOOL for flow only option...
  slide_5();
}

// GROW SOME BAD-ASS GRASS
//void LSDCatchmentModel::call_grass_growing()
//{
//if (grow_grass_time > 0)
//grow_grass(1 / (grow_grass_time * 365));
//}

//=-=-=-=-=-=-=-=-=-=
// DATA OUTPUTS ETC.
//=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::output_data(double temptotal)
// this was part of erodep() in CL but I felt it should have its own method call - DAV
{
  int n;
  Qw_newvol += temptotal*((cycle - previous)*60); // 60 seconds per min

  for (int nn = 1; nn <= rfnum; nn++)
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
      // Qw_overvol = temptotal*((cycle-tx)*output_file_save_interval); // replaced by line below MJ 25/01/05
      Qw_overvol = temptotal*((cycle - tx)*60);   // 60 secs per min
      Qw_stepvol = Qw_newvol - Qw_oldvol;
      Qw_hourvol = Qw_stepvol - Qw_overvol + Qw_lastvol;
      Qw_hour = Qw_hourvol/(60*output_file_save_interval); // convert hourly water volume to cumecs

      // same for Jw (j_mean contribution)  MJ 14/03/05
      for (int nn=1; nn<=rfnum; nn++)
      {
        Jw_overvol = (j_mean[nn] * DX * DX * nActualGridCells[nn])*((cycle - tx)*60);  // fixed MJ 29/03/05
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

      /* Former C# stuff using string.format
       *
            // Step 4: Output hourly data to file (format for reach model input)
            // changed MJ 18/01/05
            std::string output = string.Format("{0}",hours);
            output = output + string.Format(" {0:F6}",Qw_hour);
            output = output + string.Format(" {0:F6}",Jw_hour);

            // DAV: Took out the SIBERIA sub-model bit here.

            output = output + string.Format(" {0:F6}", sand_out);
            sand_out = 0;

            output = output + string.Format(" {0:F10}", Qs_hour);
      */

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

        //output = output + string.Format(" {0:F10}", Qg_hour[n]);
        //output = output+" "+Qg_hour[n];
      }

      // Open the catchment time series file in append mode (ios_base::app)
      // Open it in write mode (ios_base::out)
      // CATCH_FILE is called "catchment.dat" by default (see the .hpp file)
      std::ofstream timeseriesf(CATCH_FILE, std::ios_base::app | std::ios_base::out);

      // write the current timestep output to the time series file
      timeseriesf << output << std::endl;

      //close the file, although should you really do this if just opening it again in the next loop?
      timeseriesf.close();

      ////C# version of the above
      //StreamWriter sw = File.AppendText(CATCH_FILE);
      //sw.WriteLine(output);
      //sw.Close();
    }
    tlastcalc = cycle;
  }

}

void LSDCatchmentModel::save_data_and_draw_graphics()
{

}

void LSDCatchmentModel::save_data(int typeflag, double tempcycle)
{
  //
}

void LSDCatchmentModel::save_data(double tempcycle)
{
  if(uniquefilecheck==false) tempcycle=0;
  // Write Water_depth raster
  if (write_waterd_file == true)
  {
    // TO DO
    // This will actually write out the border cells which is incorrect technically
    // Find a way to trim these off.
    LSDRaster water_depthR(ymax+2, xmax+2, xll, yll, DX, no_data_value, water_depth);

    // Use the LSDRaster object's own method
    // Woohoo! Some actual object-oriented programming!
    water_depthR.write_double_raster(waterdepth_fname, "asc");
  }

  // Write Elevation raster
  if (write_elev_file == true)
  {
    LSDRaster elev_outR(ymax+2, xmax+2, xll, yll, DX, no_data_value, elev);
    elev_outR.write_double_raster("output_elevTEST.asc", "asc");
  }
}

// This only currently checks for an edge that is not NODATA on at least one side
// It does not check that the DEM has its lowest point on this edge. This
// should probably be added.
void LSDCatchmentModel::check_DEM_edge_condition()
{
  // Originally part of the Ur-loop (buttonclick2 or something)
  //nActualGridCells = 0;
  std::cout << "Counting number of actual grid cells in domain (non-NODATA)" << std::endl;
  for (int i = 1; i <= ymax; i++)
  {
    for (int j = 1; j <= xmax; j++)
    {
      if (elev[i][j] > -9999) nActualGridCells[rfarea[i][j]]++;
    }
  }
  // This is an odd construction, since nActualGridCells[0] is surely always 0?
  std::cout << "Number of grid cells within catchment: " << nActualGridCells[1] << std::endl;

  std::cout << "Checking edge cells for suitable catchment outlet point..." << std::endl;
  //check for -9999's on RH edge of DEM
  double nodata = -9999;
  double temp = -9999;

  int maxcols = xmax;
  int maxrows = ymax;

  // start at 1 because zeroth elements are zeroed previously.
  // (i.e. like a zero border surrounding.
  // [1][1] is the first true elev data value.
  for (int n = 1; n <= maxcols; n++)
  {
    // Check bottom edge (row major!)
    if (elev[maxrows][n] > nodata) temp = elev[maxrows][n];
    // check top edge
    if (elev[1][n] > nodata) temp = elev[1][n];
  }
  for (int n = 1; n <= maxrows; n++)
  {
    // check LH edge
    if (elev[n][1] > nodata) temp = elev[n][1];
    // check RH edge
    if (elev[n][maxcols] > nodata) temp = elev[n][maxcols];
  }

  if (temp < -10)
  {
    std::cout << "DEM EDGE CONDITION ERROR: LSDCatchmentModel may not function \
                 properly, as the edges of the DEM are all nodata (-9999) values. This will \
        prevent any water or sediment from leaving the edge of the model domain (DEM)" << std::endl;
        exit(-1);
  }
  else
  {
    std::cout << "Suitable outlet point on edge found. " << std::endl;
  }
}

// A wrapper method that calls the chief erosional and water routing methods.
void LSDCatchmentModel::run_components()   // originally erodepo() in CL
{
  // Originally main_loop() in CL, but no need (I think) for separete
  // loops here.
  std::cout << "Initialising first iteration..." << std::endl;
  double tempflow = baseflow;
  double ince = cycle + 60;
  time_1 = 1;

  // J is the local rainfall inputed into the cell at each timestep
  std::cout << "Initialising J for first time..." << std::endl;
  calc_J(1.0);

  save_time = cycle;
  creep_time = cycle;
  creep_time2 = cycle;
  soil_erosion_time = cycle;
  soil_development_time = cycle;
  time_1 = cycle;

  std::cout << "Initialising drainage area for first time..." << std::endl;
  // calculate the contributing drainage area
  get_area();

  std::cout << "Initialising catchment input points for first time..." << std::endl;
  get_catchment_input_points();

  time_factor = 1;

  // Originally erodedepo() in CL...
  // Entering the main loop here
  std::cout << "Entering main model loop..." << std::endl;

  // Main iteration loop
  do
  {
    previous = cycle;
    old_cycle = std::fmod(cycle, output_file_save_interval);
    //std::cout << "Old cycle: " << old_cycle << std::endl;
    //std::cout << "Calculate time step-related variables, make sure they don't fall below the threshold values..." << std::endl;
    double input_output_difference = std::abs(waterinput - waterOut);
    // calculate time step-related variables, make sure they don't fall below the threshold values
    if (maxdepth <= 0.1)
      maxdepth = 0.1;
    if (time_factor < (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
      time_factor = (courant_number * (DX / std::sqrt(gravity * (maxdepth))));
    if (input_output_difference > in_out_difference && time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
      time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));

    double local_time_factor = time_factor;
    if (local_time_factor > (courant_number * (DX  / std::sqrt(gravity * (maxdepth)))))
    {
      local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
    }

    // increment the counters
    counter++;
    cycle += time_factor / 60;
    // cycle is minutes, time_factor is seconds
    new_cycle = std::fmod(cycle, output_file_save_interval);

    // WATER ROUTING
    // first zero counter to tally up water inputs
    waterinput = 0;

    // In CL there was an option to set either reach or tidal mode.
    // Only catchment mode is implemented in this spin off version
    //std::cout << "LOCAL TIME FACTOR: " << local_time_factor << std::endl;
    catchment_water_input_and_hydrology(local_time_factor);

    //std::cout << "route the water and update the flow depths\r" << std::flush;
    qroute();
    depth_update();

    // check scan area every 5 iters.. maybe re-visit for reach mode if it causes too much backing up of sed. see code commented below nex if..
    if (std::remainder(counter, 5) == 0)
    {
      scan_area();
    }

    //call_erosion();
    //call_lateral();
    water_flux_out(local_time_factor);

    temptotal = temptot;

    output_data(temptotal);  // not sure if this is the best place to put this, but it needs to be done every timestep? - DAV

    std::cout << "Cycle: " << cycle << "                  \r" << std::flush;

    if (cycle >= save_time)
    {
      // deprecated // save_data_and_draw_graphics(); //similar to above worry?
      save_data(tempcycle);
      save_time += saveinterval;
    }

    // if we have reached the end of the run, kill the cycle
  } while (cycle < maxcycle * 60);
  // here endeth the main erodepo loop!

}

void LSDCatchmentModel::print_initial_values()
{
  // TO DO: (just for debugging, really)
  std::cout << "Printing initial parameter values: " << std::endl;
  //std::cout <<
}



void LSDCatchmentModel::zero_values()
{
  // To do

  for(int i=0; i <= ymax; i++)
  {
    for(int j=0; j <= xmax; j++)
    {
      Vel[i][j] = 0;
      area[i][j] = 0;
      elev[i][j] = 0;
      bedrock[i][j] = -9999;
      init_elevs[i][j] = elev[i][j];
      water_depth[i][j] = 0;
      index[i][j] = -9999;
      inputpointsarray[i][j] = false;
      veg[i][j][0] = 0;// elevation
      veg[i][j][1] = 0; // densitj
      veg[i][j][2] = 0; // jw density
      veg[i][j][3] = 0; // height

      edge[i][j] = 0;
      edge2[i][j] = 0;

      //sand[i][j] = 0;

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

  for(int i=1; i<((xmax*ymax)/LIMIT); i++)
  {
    for(int j=0; j<=G_MAX; j++)
    {
      grain[i][j] = 0;
    }
    for(int z=0; z <= 9; z++)
    {
      for(int j=0; j <= G_MAX-2; j++)
      {
        strata[i][z][j] =0;
      }
    }
    catchment_input_x_coord[i] = 0;
    catchment_input_y_coord[i] = 0;
  }

  for (int i = 1; i <= rfnum; i++)
  {
    j[i] = 0.000000001;
    jo[i] = 0.000000001;
    j_mean[i] = 0;
    old_j_mean[i] = 0;
    new_j_mean[i] = 0;
  }

  // TO DO
  // DAV - Hard coded in that the 1st fraction is suspended: needs
  // to be red from a separate input file at later date.
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
// 2. qroute
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
void LSDCatchmentModel::water_flux_out(double local_time_factor)
// Extracted as a seprate method from erodepo()
{
  //double temptot = 0;    // temporary water total // DV MADE GLOBAL VAR

  // Zero the water, but then we set it to the minimum depth - DV
  temptot = 0;
  for (int i = 1; i <= ymax; i++)
  {
    // RH Edge
    if (water_depth[i][xmax] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[i][xmax] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
      water_depth[i][xmax] = water_depth_erosion_threshold;
    }
    // LH Edge
    if (water_depth[i][1] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[i][1] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
      water_depth[i][1] = water_depth_erosion_threshold;
    }
  }

  for (int j = 1; j <= xmax; j++)
  {
    // Top Edge
    if (water_depth[1][j] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[1][j] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
      water_depth[1][j] = water_depth_erosion_threshold;
    }
    // Bottom Edge
    if (water_depth[ymax][j] > water_depth_erosion_threshold)
    {
      temptot += (water_depth[ymax][j] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
      water_depth[ymax][j] = water_depth_erosion_threshold;
    }
  }
  waterOut = temptot;
}
/*
void LSDCatchmentModel::init_water_routing(int flag, double reach_input_amount, double catchment_input_amount)
{
  int x, y, inc;
  double w = water_depth_erosion_threshold;
}
*/

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// THE WATER ROUTING ALGORITHM: LISFLOOD-FP
//
// Originally derived in Bates et al (2010) Journal of Hydrology
// The Lisflood-FP algorithm is a non steady state water flow
// algorithm that uses a simplified version of the Saint-Venant
// shallow water equations (see Hydrology textbooks) to calculate
// water transport across the model.
//
// Originally implemented in CL in C# by TC
// Translated here to C++ by DAV for LSD
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::qroute()
{
  //std::cout << "qroute\r" << std::flush;
  double local_time_factor = time_factor;
  if (local_time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
  {
    local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
  }

  //std::cout << "local time factor: " << local_time_factor <<"\r" << std::flush;
  // PARALLELISATION  COULD BE INSERTED HERE - DAV
  // #OMP PARALLEL...etc

  int inc = 1;

  // y is less than xmax? change to i,j to avoid confusion?
  for (int y=1; y < xmax+1; y++)
  {
    while (down_scan[y][inc] > 0)
    {
      int x = down_scan[y][inc];
      inc++;

      if (elev[x][y] > -9999.0)   // stops the water moving into NODATA value cells
      {
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=
        // ROUTING IN THE X-DIRECTION
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=
        // check that we are in a cell containing water and not a -9999 cell
        if ((water_depth[x][y] > 0 || water_depth[x-1][y] > 0) && elev[x - 1][y] > -9999)
        {
          // find the amount of horizontal flow by comparing the water heights of two neighbouring cells: x and x-1.
          double hflow = std::max(elev[x][y] + water_depth[x][y], elev[x-1][y] + water_depth[x-1][y]) -
              std::max(elev[x-1][y], elev[x][y]);
          //debug
          //std::cout << "hflow: " << hflow << std::endl;
          // don't bother routing flow if it is so minuscule...
          if (hflow > hflow_threshold)
          {
            // debug
            //std::cout << "caluclating hflow..." << std::endl;
            double tempslope = (((elev[x-1][y] + water_depth[x-1][y])) - (elev[x][y] + water_depth[x][y])) / DX;

            // to do : apply this to all edes
            if (x == xmax) tempslope = edgeslope;
            if (x <= 2) tempslope = 0 - edgeslope; // this deals with the problem of the next-to-edge cells

            // appears to be some variation of the Darcy-Weisbach formula? Should check this - DAV.
            // calculate discharge out of cells in x direction.
            qx[x][y] = ((qx[x][y] - (gravity * hflow * local_time_factor * tempslope)) /
                        (1 + gravity * hflow * local_time_factor * (mannings * mannings) * std::abs(qx[x][y]) /
                         std::pow(hflow, (10/3))));

            // Below, the lines attempt to stop a situation where there would be too much water
            // moving from one cell to another in a single time step, resulting in negative
            // discharges. Large instabilities can develop in steep catchments.
            if (qx[x][y] > 0 && (qx[x][y] / hflow)/std::sqrt(gravity*hflow) > froude_limit)
            {
              qx[x][y] = hflow * (std::sqrt(gravity * hflow) * froude_limit);
            }
            if (qx[x][y] < 0 && std::abs(qx[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit)
            {
              qx[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit));
            }

            if ((qx[x][y] > 0 && qx[x][y] * local_time_factor / DX) > (water_depth[x][y] / 4))
            {
              qx[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;
            }

            if (qx[x][y] < 0 && std::abs(qx[x][y] * local_time_factor / DX) > (water_depth[x-1][y] / 4))
            {
              qx[x][y] = 0 - ((water_depth[x-1][y] * DX) / 5) / local_time_factor;
            }
            // BOOL IF statement about the suspended sediment option
            //if (isSuspended == true)
            if (suspended_opt == true)
            {
              if (qx[x][y] > 0)
              {
                qxs[x][y] = qx[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
              }
              if (qx[x][y] < 0)
              {
                qxs[x][y] = qx[x][y] * (Vsusptot[x-1][y] / water_depth[x-1][y]);
              }
              if (qxs[x][y] > 0 && std::abs(qxs[x][y] * local_time_factor > (Vsusptot[x-1][y] * DX) / 4))
              {
                qxs[x][y] = 0 - ((Vsusptot[x-1][y] * DX) / 5 / local_time_factor);
              }
            }

            // CALCULATE THE VELOCITY OF THE FLOWS
            if (qx[x][y] > 0)
            {
              vel_dir[x][y][7] = qx[x][y] / hflow;
            }
            if (qx[x][y] < 0)
            {
              vel_dir[x-1][y][3] = (0 - qx[x][y]) / hflow;
            }
          }
          // What to do if the water depth is really, really small:
          else
          {
            qx[x][y] = 0;
            qxs[x][y] = 0;
          }
        }// end of the X-direction routing code block

        // =-=-=-=-=-=-=-=-=-=-=-=-=-=
        // ROUTING IN THE Y-DIRECTION
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=
        // check that we are in a cell containing water and not a -9999 cell
        if ((water_depth[x][y] > 0 || water_depth[x][y-1] > 0) && elev[x][y-1] > -9999)
        {
          // find the amount of horizontal flow by comparing the water heights of two neighbouring cells: x and x-1.
          double hflow = std::max(elev[x][y] + water_depth[x][y], elev[x][y-1] + water_depth[x][y-1]) -
              std::max(elev[x][y], elev[x][y-1]);

          if (hflow > hflow_threshold)    // don't bother computing flow if it is so minuscule...
          {
            double tempslope = (((elev[x][y-1] + water_depth[x][y-1])) - (elev[x][y] + water_depth[x][y])) / DX;

            if (y == xmax) tempslope = edgeslope;
            if (y <= 2) tempslope = 0 - edgeslope; // this deals with the problem of the next-to-edge cells

            // The Lisflood-FP algorithm developed in Bates et al 2010 - DAV.
            // calculate discharge out of cells in y direction.
            qy[x][y] = ((qy[x][y] - (gravity * hflow * local_time_factor * tempslope)) /
                        (1 + gravity * hflow * local_time_factor * (mannings * mannings) * std::abs(qy[x][y]) /
                         std::pow(hflow, (10/3))));

            // Below, the lines attempt to stop a situation where there would be too much water
            // moving from one cell to another in a single time step, resulting in negative
            // discharges. Large instabilities can develop in steep catchments.
            if (qy[x][y] > 0 && (qy[x][y] / hflow)/std::sqrt(gravity*hflow) > froude_limit)
              qy[x][y] = hflow * (std::sqrt(gravity * hflow) * froude_limit);

            if (qy[x][y] < 0 && std::abs(qy[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit)
              qy[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit));

            if ((qy[x][y] > 0 && qy[x][y] * local_time_factor / DX) > (water_depth[x][y] / 4))
              qy[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;

            if (qy[x][y] < 0 && std::abs(qy[x][y] * local_time_factor / DX) > (water_depth[x][y-1] / 4))
              qy[x][y] = 0 - ((water_depth[x][y-1] * DX) / 5) / local_time_factor;

            // BOOL IF statement about the suspended sediment option
            if (suspended_opt == true)
            {
              if (qy[x][y] > 0)
              {
                qys[x][y] = qy[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
              }
              if (qy[x][y] < 0)
              {
                qys[x][y] = qy[x][y] * (Vsusptot[x][y-1] / water_depth[x][y-1]);
              }
              if (qys[x][y] > 0 && std::abs(qys[x][y] * local_time_factor > (Vsusptot[x][y-1] * DX) / 4))
              {
                qys[x][y] = 0 - ((Vsusptot[x][y-1] * DX) / 5 / local_time_factor);
              }
            }

            // CALCULATE THE VELOCITY OF THE FLOWS
            if (qy[x][y] > 0)
            {
              vel_dir[x][y][1] = qy[x][y] / hflow;
            }
            if (qy[x][y] < 0)
            {
              vel_dir[x][y-1][5] = (0 - qy[x][y]) / hflow;
            }
          }

          else // What to do if the water depth is really, really small:
          {
            qy[x][y] = 0;
            qys[x][y] = 0;
          }
        }   // end of the y-direction water routing block
      }
    } // endwhile
  } // endfor
}

void LSDCatchmentModel::depth_update()
{
  //std::cout << "depth_update(): " << std::endl;
  double local_time_factor = time_factor;

  if (local_time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
  {
    local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
  }

  std::vector<double> tempmaxdepth2(ymax + 2);
  maxdepth = 0;

  for (int y=1; y<=ymax; y++)
  {
    int inc = 1;
    double tempmaxdepth = 0;
    while (down_scan[y][inc] > 0)
    {
      //debug
      //std::cout << "Entering the depth update loop" <<std::endl;
      int x = down_scan[y][inc];
      inc++;

      // UPDATE THE WATER DEPTHS
      //std::cout << "update water depth: " << inc << "\r" << std::flush;
      water_depth[x][y] += local_time_factor * (qx[x+1][y] - qx[x][y] + qy[x][y+1] - qy[x][y]) / DX;
      //debug
      //std::cout << "incrementing_depth: " << water_depth[x][y] << std::endl;

      // UPDATE THE SUSPENDED SEDIMENT CONCENTRATIONS
      if (suspended_opt == true)
      {
        //std::cout << "update SuspSedi: " << std::endl;
        Vsusptot[x][y] += local_time_factor * (qxs[x + 1][y] - qxs[x][y] + qys[x][y + 1] - qys[x][y]) / DX;
      }

      if (water_depth[x][y] > 0)
      {
        // remove any water depths on NODATA cells (This shouldnae happen!)
        if (elev[x][y] == -9999)
          water_depth[x][y] = 0;

        // calculate the maximum flow depth for this time step.
        if (water_depth[x][y] > tempmaxdepth)
          tempmaxdepth = water_depth[x][y];
      }
    }
    tempmaxdepth2[y] = tempmaxdepth;
  }
  // reduction (if paralellism implemented at later date)
  for (int y = 1; y <= ymax; y++)
  {
    if (tempmaxdepth2[y] > maxdepth)
    {
      maxdepth = tempmaxdepth2[y];
    }
  }
}

void LSDCatchmentModel::catchment_water_input_and_hydrology( double local_time_factor)     // DAV - This can be split into subfunctions
{
  for (int z=1; z <= totalinputpoints; z++)
  {
    int j = catchment_input_x_coord[z];
    int i = catchment_input_y_coord[z];
    double water_add_amt = (j_mean[rfarea[i][j]] * nActualGridCells[rfarea[i][j]]) /
        (catchment_input_counter[rfarea[i][j]]) * local_time_factor;    //
    if (water_add_amt > ERODEFACTOR)
    {
      water_add_amt = ERODEFACTOR;
    }

    waterinput += (water_add_amt / local_time_factor) * DX * DX;

    // debug
    //if (waterinput > 0)
    //{
    //std::cout << "waterinput: " << waterinput << std::endl;
    //}
    //if (water_add_amt > 0)
    //{
    //std::cout << "water_add_amt: " << water_add_amt << std::endl;
    //}


    water_depth[i][j] += water_add_amt;
  }

  // if the input type flag is 1 then the discharge is input from the hydrograph
  if (cycle >= time_1)
  {
    do
    {
      time_1++;
      calc_J(time_1++);  // calc_J is based on the rainfall rate supplied to the cell
      if (time_factor > max_time_step && new_j_mean[1] > (0.2 / (xmax * ymax * DX * DX)))
        // check after the variable rainfall area has been added
        // stops code going too fast when there is actual flow in the channels greater than 0.2cu
      {
        cycle = time_1 + (max_time_step / 60);
        time_factor = max_time_step;
      }
    } while (time_1 < cycle);
  }

  calchydrograph(time_1 - cycle);

  double jmeanmax =0;
  for (int n=1; n <= rfnum; n++)
  {
    if (j_mean[n] > jmeanmax)
    {
      jmeanmax = j_mean[n];
    }
  }

  if (rainfall_data_on == true)
    // Yikes, this was originally hard-coded into the GUI
    // interface in CL... (DAV - Needs a new bool param declaring and setting in the driver file)
  {
    j_mean[1] = ((hourly_rain_data[(static_cast<int>(cycle / rain_data_time_step))][1] //check in original
        / std::pow(DX, 2)) / nActualGridCells[1]);
  }

  if (jmeanmax >= baseflow)
  {
    baseflow = baseflow * 3;    // Magic number 3!? - DAV
    get_area();         // Could this come from one of the LSDobject files? - DAV
    get_catchment_input_points();
  }

  if (baseflow > (jmeanmax * 3) && baseflow > 0.0000001)
  {
    baseflow = jmeanmax * 1.25;   // Where do these magic numbers come from? DAV
    get_area();
    get_catchment_input_points();
  }
}

// Calculates the rainfall input to each cell per time step
// J is the local rainfall inputed into the cell at each timestep
// (Actually that is j_mean)
void LSDCatchmentModel::calc_J(double cycle)
{
  for (int n=1; n <= rfnum; n++)    // rfnum is the rainfall number int = 2 to begin with
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
      M = hourly_m_value[1 + static_cast<int>(cycle / rain_data_time_step)];
    }

    local_rain_fall_rate = 0;

    // DAV- Experimental fix! subtract 1 as n=1 should be outside the vector index
    // for spatially uniform rainfall. You actuall want:
    // hourly_rain_data[hour][0]  (n starts at 1 here)
    double cur_rain_rate = hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n];
    // std::cout << cur_rain_rate << std::endl;

    if (hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n] > 0)
    {
      local_rain_fall_rate = rain_factor * ((hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n] / 1000) / 3600);
      /** divide by 1000 to make m/hr, then by 3600 for m/sec */
    }

    if (local_rain_fall_rate == 0)
    {
      j[n] = jo[n] / (1 + ((jo[n] * local_time_step) / M));
      new_j_mean[n] = M / local_time_step * std::log(1 + ((jo[n] * local_time_step) / M));
    }

    if (local_rain_fall_rate > 0)
    {
      j[n] = local_rain_fall_rate / (((local_rain_fall_rate - jo[n]) / jo[n]) * std::exp((0 - local_rain_fall_rate) * local_time_step / M) + 1);
      new_j_mean[n] = (M / local_time_step) * std::log(((local_rain_fall_rate - jo[n]) + jo[n] * std::exp((local_rain_fall_rate * local_time_step) / M)) / local_rain_fall_rate);
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
// Where is this derivation from? - DAV
void LSDCatchmentModel::calchydrograph(double time)
{
  for (int n=1; n <= rfnum; n++)
  {
    j_mean[n] = old_j_mean[n] + (( (new_j_mean[n] - old_j_mean[n]) / 2) * (2 - time));
  }
}


void LSDCatchmentModel::get_catchment_input_points()
{
  std::cout << "Calculating catchment input points... Total: ";


  totalinputpoints = 1;
  for (int n=1; n <= rfnum; n++)
  {
    catchment_input_counter[n] = 0;
  }
  for (int i=1; i <= ymax; i++)
  {
    for (int j=1; j <= xmax; j++)
    {
      if ((area[i][j] * j_mean[rfarea[i][j]] * 3 * DX * DX) > MIN_Q \
          && (area[i][j] * j_mean[rfarea[i][j]] * 3 * DX * DX) < MIN_Q_MAXVAL)
      {
        catchment_input_x_coord[totalinputpoints] = j;
        catchment_input_y_coord[totalinputpoints] = i;
        catchment_input_counter[rfarea[i][j]]++;
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
  //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
  //Parallel.For(1, ymax, options, delegate(int y)
  for (int y=1; y < ymax; y++)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      int x = down_scan[y][inc];
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

  //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
  //Parallel.For(1, ymax+1, options, delegate(int y)
  for (int i=1; i <= ymax; i++)
  {
    int inc = 1;
    for (int j=1; j <= xmax; j++)
    {
      // zero scan bit..
      down_scan[j][i] = 0;
      // and work out scanned area. // TO DO (DAV) there is some out-of-bounds indexing going on here, check carefully!
      if (water_depth[i][j] > 0
          || water_depth[i - 1][j] > 0
          || water_depth[i - 1][j - 1] > 0
          || water_depth[i - 1][j + 1] > 0
          || water_depth[i + 1][j - 1] > 0
          || water_depth[i + 1][j + 1] > 0
          || water_depth[i][j - 1] > 0
          || water_depth[i + 1][j] > 0
          || water_depth[i][j + 1] > 0)
      {
        down_scan[j][inc] = i;
        inc++;
        // debug
        //std::cout << "set downscan: "<< down_scan[y][inc] << std::endl;
      }
    }
  }//);
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
//  8) lateral3
//  9) slide_3
//  10) slide_5
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
  int n, z;


  if (index[x][y] == -9999) addGS(x,y);  // should not be necessary
  xyindex = index[x][y];

  total = 0.0;
  for (n=0;n<=G_MAX;n++)
  {

    total += grain[xyindex][n];

  }

  if (total > (active*1.5)) // depositing - create new strata layer and remove bottom one..
  {
    // start from bottom
    // remove bottom active layer
    // then move all from layer above into one below, up to the top layer
    for(z=9;z>=1;z--)
    {
      for(n=0;n<=G_MAX-2;n++)
      {

        strata[xyindex][z][n]=strata[xyindex][z-1][n];

      }
    }

    // then remove strata thickness from grain - and add to top strata layer
    coeff = active / total;
    for (n=1;n<=(G_MAX-1);n++)
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
    for (n=1;n<=(G_MAX-1);n++)
    {
      grain[xyindex][n] += strata[xyindex][0][n-1];
    }

    // then from top down add lower strata into upper
    for(z=0;z<=8;z++)
    {
      for(n=0;n<=G_MAX-2;n++)
      {
        strata[xyindex][z][n] = strata[xyindex][z+1][n];
      }
    }

    // add new layer at the bottom
    amount = active;
    z = 9;
    strata[xyindex][z][0] = amount*d1prop; // 0.0;
    strata[xyindex][z][1] = amount*d2prop; // + amount*d1prop;
    strata[xyindex][z][2] = amount*d3prop;
    strata[xyindex][z][3] = amount*d4prop;
    strata[xyindex][z][4] = amount*d5prop;
    strata[xyindex][z][5] = amount*d6prop;
    strata[xyindex][z][6] = amount*d7prop;
    strata[xyindex][z][7] = amount*d8prop;
    strata[xyindex][z][8] = amount*d9prop;

  }

}

void LSDCatchmentModel::addGS(int &x, int &y)
{
  // needs lock statement to stop two being added at the same time...
  //lock (this) // DAV - This is only for paralllism...
  //{
  int n, q;
  grain_array_tot++;
  index[x][y] = grain_array_tot;

  grain[grain_array_tot][0] = 0;
  grain[grain_array_tot][1] = active * d1prop;
  grain[grain_array_tot][2] = active * d2prop;//+(active*d1prop);
  grain[grain_array_tot][3] = active * d3prop;
  grain[grain_array_tot][4] = active * d4prop;
  grain[grain_array_tot][5] = active * d5prop;
  grain[grain_array_tot][6] = active * d6prop;
  grain[grain_array_tot][7] = active * d7prop;
  grain[grain_array_tot][8] = active * d8prop;
  grain[grain_array_tot][9] = active * d9prop;
  grain[grain_array_tot][10] = 0;


  for (n = 0; n <= 9; n++)
  {
    strata[grain_array_tot][n][0] = (active) * d1prop;
    strata[grain_array_tot][n][1] = (active * d2prop);
    strata[grain_array_tot][n][2] = (active) * d3prop;
    strata[grain_array_tot][n][3] = (active) * d4prop;
    strata[grain_array_tot][n][4] = (active) * d5prop;
    strata[grain_array_tot][n][5] = (active) * d6prop;
    strata[grain_array_tot][n][6] = (active) * d7prop;
    strata[grain_array_tot][n][7] = (active) * d8prop;
    strata[grain_array_tot][n][8] = (active) * d9prop;

    if (elev[x][y] - (active * (n + 1)) < (bedrock[x][y] - active))
    {
      for (q = 0; q <= (G_MAX - 2); q++)
      {
        strata[grain_array_tot][n][q] = 0;
      }
    }
  }


  sort_active(x, y);

  //} // this is for the locak statement above

}

double LSDCatchmentModel::sand_fraction(int index1)
{

  int n;
  double active_thickness=0;
  double sand_total=0;
  for(n=1;n<=G_MAX;n++)
  {
    active_thickness+=(grain[index1][n]);
  }

  for(n=1;n<=2;n++) // number of sand fractions...
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
  if (counter > 0) {
    elevtot /= counter;
    return elevtot;
  }

  else return 0;
}

double LSDCatchmentModel::d50(int index1)
{
  int z,n,i;
  double active_thickness=0;
  double Dfifty=0,max=0,min=0;
  std::array<double, 20> cum_tot;  // std::array only C++11

  // The C# way of doing it:
  //double[] cum_tot;
  //cum_tot = new double[20];

  for(n=1;n<=G_MAX;n++)
  {
    for(z=0;z<=(0);z++)
    {
      active_thickness+=(grain[index1][n]);
      cum_tot[n]+=active_thickness;
    }
  }


  i=1;
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
  //if(i==9){min=std::log(d8);max=std::log(d9);}

  Dfifty = std::exp(max - ((max - min) * ((cum_tot[i] - (active_thickness * 0.5)) / (cum_tot[i] - cum_tot[i - 1]))));
  if(active_thickness<0.0000001)Dfifty=0;
  return Dfifty;

}

void LSDCatchmentModel::slide_GS(int x,int y, double amount,int x2, int y2)
{

  /** Ok, heres how it works, x and y are ones material moved from,
      x2 and y2 are ones material moved to...
      amd amount is the amount shifted. */

  int n;
  double total = 0;

  // do only for cells where both have grainsize..

  if (index[x][y] != -9999 && index[x2][y2] != -9999)
  {
    for (n = 1; n <= (G_MAX - 1); n++)
    {
      if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
    }

    if (amount > total)
    {
      grain[index[x2][y2]][1] += (amount - total) * d1prop;
      grain[index[x2][y2]][2] += (amount - total) * d2prop;
      grain[index[x2][y2]][3] += (amount - total) * d3prop;
      grain[index[x2][y2]][4] += (amount - total) * d4prop;
      grain[index[x2][y2]][5] += (amount - total) * d5prop;
      grain[index[x2][y2]][6] += (amount - total) * d6prop;
      grain[index[x2][y2]][7] += (amount - total) * d7prop;
      grain[index[x2][y2]][8] += (amount - total) * d8prop;
      grain[index[x2][y2]][9] += (amount - total) * d9prop;

      amount = total;
    }

    if (total > 0)
    {
      for (n = 1; n <= (G_MAX - 1); n++)
      {
        double transferamt = amount * (grain[index[x][y]][n] / total);
        grain[index[x2][y2]][n] += transferamt;
        grain[index[x][y]][n] -= transferamt;
        if (grain[index[x][y]][n] < 0) grain[index[x][y]][n] = 0;
      }

    }

    /* then to set active layer to correct depth before erosion, */
    sort_active(x, y);
    sort_active(x2, y2);
    return;
  }

  //now do for cells where only recieving cells have grainsize
  // just adds amount to reviving cells of normal..
  if (index[x][y] == -9999 && index[x2][y2] != -9999)
  {
    grain[index[x2][y2]][1] += (amount) * d1prop;
    grain[index[x2][y2]][2] += (amount) * d2prop;
    grain[index[x2][y2]][3] += (amount) * d3prop;
    grain[index[x2][y2]][4] += (amount) * d4prop;
    grain[index[x2][y2]][5] += (amount) * d5prop;
    grain[index[x2][y2]][6] += (amount) * d6prop;
    grain[index[x2][y2]][7] += (amount) * d7prop;
    grain[index[x2][y2]][8] += (amount) * d8prop;
    grain[index[x2][y2]][9] += (amount) * d9prop;

    /* then to set active layer to correct depth before erosion, */
    sort_active(x2, y2);
    return;
  }

  // now for cells whre dontaing cell has grainsize
  if (index[x][y] != -9999 && index[x2][y2] == -9999)
  {

    addGS(x2, y2); // add grainsize array for recieving cell..

    if (amount > active)
    {

      grain[index[x2][y2]][1] += (amount - active) * d1prop;
      grain[index[x2][y2]][2] += (amount - active) * d2prop;
      grain[index[x2][y2]][3] += (amount - active) * d3prop;
      grain[index[x2][y2]][4] += (amount - active) * d4prop;
      grain[index[x2][y2]][5] += (amount - active) * d5prop;
      grain[index[x2][y2]][6] += (amount - active) * d6prop;
      grain[index[x2][y2]][7] += (amount - active) * d7prop;
      grain[index[x2][y2]][8] += (amount - active) * d8prop;
      grain[index[x2][y2]][9] += (amount - active) * d9prop;

      amount = active;
    }


    for (n = 1; n <= (G_MAX - 1); n++)
    {
      if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
    }

    for (n = 1; n <= (G_MAX - 1); n++)
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

double LSDCatchmentModel::erode(double mult_factor)
{
  double rho = 1000.0;
  double tempbmax = 0;

  //std::array<double, 20> gtot2;
  double gtot2[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  for (int n=0; n <= G_MAX; n++)
  {
    gtot2[n] = 0;
  }

  time_factor = time_factor * 1.5;
  if (time_factor > max_time_step)
  {
    time_factor = max_time_step;
  }

  int counter2 = 0;


  // main erosion loop
  // Needs refactoring to smaller functions/methods for maintainability
  do
  {
    counter2++;
    tempbmax = 0;
    std::vector<double> tempbmax2(ymax + 2, 0.0);

    // PARALELLISATION GOES HERE
    // C#
    // var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
    // Parallel.For(1, ymax, options, delegate(int y)

    // C++ with OpenMP
    // #pragma omp parallel for
    for (int y =1; y <= ymax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0) // results in an infinte loop!?
      {
        int x = down_scan[y][inc];
        inc++;

        // zero the velocities
        Vel[x][y] = 0;
        Tau[x][y] = 0;

        for(int n=0; n < G_MAX; n++)
        {
          sr[x][y][n] = 0;
          sl[x][y][n] = 0;
          su[x][y][n] = 0;
          sd[x][y][n] = 0;
        }
        ss[x][y] = 0;

        if (water_depth[x][y] > water_depth_erosion_threshold)
        {
          double temptot1 = 0;
          double temptot2 = 0;
          double veltot = 0;
          double vel = 0;
          double qtot = 0;
          double tau = 0;
          double velnum = 0;
          double slopetot = 0;

          std::vector<double> temp_dist(11, 0.0);    // this vector holds the amountto be removed from the cell in each grain size
          std::vector<double> tempdir(11, 0.0);    // this vector hold the velocity directions temporariliy

          // check to see if the there is a nodata value in the cell
          if (index[x][y] == -9999)
          {
            addGS(x, y);    // add the grain size for that cell
          }

          // now add up the velocity directions, velocities, and edge directions
          for (int p=1; p <= 8; p += 2)   // D8 routing
          {
            int x2 = x + deltaX[p];
            int y2 = y + deltaY[p];

            if (water_depth[x2][y2] > water_depth_erosion_threshold)
            {
              if (edge[x][y] > edge[x2][y2]);
              {
                temptot2 += (edge[x][y] - edge[x2][y2]);
              }


              if (vel_dir[x][y][p] > 0)
              {
                // First, work out the velocities in each direction
                // (For the sediment distribution)
                vel = vel_dir[x][y][p];

                if (vel > max_vel)
                {
                  vel = max_vel;
                  // if the velocity is too high, for model stability reasons
                  // then it should be cut back to the max_vel.
                  // (though this violates cons. of momentum!)
                  // assume transferred to other stores...
                }

                tempdir[p] = vel * vel;
                veltot += tempdir[p];
                velnum++;
                qtot += (vel * vel);
                slopetot += ((elev[x][y] - elev[x2][y2]) / DX) * vel;
              }
            }
          }

          // calculate velocities for each cell
          // calculate tau (stress) for each cell
          if (qtot > 0)
          {
            vel = (std::sqrt(qtot));
            Vel[x][y] = vel;

            if (vel < 0)
            {
              // Not sure what is done computationally here,
              // Original code just had a text box/meessage update?
            }

            // make sure no cells have vels greater than the max vel
            if (vel > max_vel)
            {
              vel = max_vel;
            }

            double ci = gravity * (mannings * mannings) * std::pow(water_depth[x][y], -0.33);

            if (slopetot > 0)
            {
              slopetot = 0;
            }

            tau = 1000 * ci * vel * vel * (1 + (1 * (slopetot / vel)));
            Tau[x][y] = tau;
          }

          // NOW DO SOME EROSION
          // DAV- These ought to be seperate methods really, no need
          // for so many if blocks in a single function.
          // i.e. one method for the Wilcock, one for Einstein.

          // Check we have some shear stress
          if (tau > 0)
          {
            double d_50 = 0;
            double Fs = 0;
            double Di = 0;
            double graintot = 0;

            if (wilcock == true)
            {
              d_50 = d50(index[x][y]);
              if (d_50 < 1)
              {
                d_50 = d1;
              }
              Fs = sand_fraction(index[x][y]);

              for (int n=1; n <= G_MAX; n++)
              {
                graintot += (grain[index[x][y]][n]);
              }
            }

            for (int n=1; n <= 9; n++)
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

              // For the WILCOCK and CROW/CURRAN transport/erosion laws
              // MAKE SEPARATE METHOD
              if (wilcock == true)
              {
                double tau_ri = 0;
                tau_ri = (0.021 + 64504398503);// WHATTT??!?!
                double U_star =  std::pow(tau / rho, 0.5);
                double Wi_star;
                double Fi = grain[index[x][y]][n] / graintot;

                if ((tau / tau_ri) < 1.35)
                {
                  Wi_star = 0.002 * std::pow(tau / tau_ri, 7.5);
                }

                else
                {
                  Wi_star = 14 * std::pow(1 - (0.894 / std::pow(tau / tau_ri, 0.5)), 4.5);
                }

                temp_dist[n] = mult_factor * time_factor * ((Fi * (U_star * U_star * U_star)) / ((2.65 - 1) * gravity)) * Wi_star / DX;
              }

              // EINSTEIN Sediment option
              // MAKE SEPARATE METHOD
              if (einstein == true)
              {
                temp_dist[n] = mult_factor * time_factor * (40 * std::pow(((1 / (((2650 - 1000) * Di) / tau / gravity))), 3)) / std::sqrt(1000 / ((2250 - 1000) * gravity * (Di * Di * Di))) / DX;  // recheck the maths here!
              }

              // check that the amount of susp. sed. is not really really tiny. (if so, zero)
              if (temp_dist[n] < 0.0000000000001) temp_dist[n] = 0;
              // check to see if theres not too little sediment in the cell to be entrained
              if (temp_dist[n] > grain[(index[x][y])][n]) temp_dist[n] = grain[(index[x][y])][n];
              // then check to see if this would make the suspended sediment levels too high
              // reduce if too high
              if (isSuspended[n] && n ==1)
              {
                if ((temp_dist[n] + Vsusptot[x][y]) / water_depth[x][y] > Csuspmax)
                {
                  // work out the max amount of sediment that there can be (water_depth * csuspmax)
                  // then subtract what's already there (Vsusptot) to leave what can
                  // possibly be entrained. Check if < 0 afterwards.
                  temp_dist[n] = (water_depth[x][y] * Csuspmax) - Vsusptot[x][y];
                }
              }
              // Check if < 0.
              if (temp_dist[n] < 0) temp_dist[n] = 0;

              // placed this here to speed up reduction of erode repeats
              temptot1  += temp_dist[n];   // DAV - Not sure exactly what this does yet...
            }

            // Interlude. Here's a relaxing picture of a lighthouse:
            /*
                                  ----------.................._____________  _  .-.
                                                                  _____.. . .   | |
                                               _____....------""""             uuuuu
                             ____....------""""                                |===|
                                                                               |===|
                                                                               |===|
                                                                               |===|
                             _a:f____________________________________________ .[__N]. _______

                            */
            // DAV - really need to flatten some of these for/if/else nested blocks...
            // Use separate small functions

            // CALCULATE THE SED CONC (NON SUSPENDED)
            // check if the substrate erosion makes it below the bedrock
            if (elev[x][y] - temptot1 <= bedrock[x][y])
            {
              // now remove this from the proportion that can be eroded
              // we can do this as we know the proportion (temptot) that is there to be eroded.
              double elevdiff = elev[x][y] - bedrock[x][y];
              double temptot3 = temptot1;
              temptot1 = 0;

              for (int n=1; n <= 9; n++)   // n <= 9 is to do each sediment fraction.
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
                temptot1  += temp_dist[n];   // DAV - Not sure exactly what this does yet...
              }
            }
            // BEDROCK EROSION ROUTINE GOES HERE
            //
            //  "If it weren't for the rocks in its bed,
            //   the stream would have no song."
            //      - Carl Perkins
            //


            // DAV - This should definitely be a separate method.
            if (tau > bedrock_erosion_threshold)
            {
              double amount = 0;   // 'amount' is the amount of erosion into the bedrock
              amount = std::pow(bedrock_erosion_rate * tau, 1.5) * time_factor * mult_factor * 0.000000317;
              // last value is to turn this into erosion per year (number of years in a second)
              bedrock[x][y] -= amount;
              // now add the amount of bedrock eroded into the sediment proportions.
              grain[(index[x][y])][1] += amount * d1prop;
              grain[(index[x][y])][2] += amount * d2prop;
              grain[(index[x][y])][3] += amount * d3prop;
              grain[(index[x][y])][4] += amount * d4prop;
              grain[(index[x][y])][5] += amount * d5prop;
              grain[(index[x][y])][6] += amount * d6prop;
              grain[(index[x][y])][7] += amount * d7prop;
              grain[(index[x][y])][8] += amount * d8prop;
              grain[(index[x][y])][9] += amount * d9prop;
            }
          }

          // VEGETATION COMPONENTS
          //
          // "Vegetation is the basic instrument the creator
          //  uses to set all of nature in motion"
          //      - Antoine Lavoisier
          if (veg[x][y][1] > 0 && tau > vegTauCrit)
          {
            // Remove from the vegetaton layer
            veg[x][y][1] -= mult_factor * time_factor * std::pow(tau - vegTauCrit, 0.5) * 0.00001;
            if (veg[x][y][1] < 0) veg[x][y][1] = 0;
          }

          // Determine if movement should be restricted due to veg, or because
          // of bedrock
          if (veg[x][y][1] > 0.25)   // Arbitrary threshold? - DAV
          {
            // now check if this amount removed from the cell would put it
            // below the veg layer
            if (elev[x][y] - temptot1 <= veg[x][y][0])
            {
              // now remove from the proportion that can be eroded
              double elevdiff = 0;
              elevdiff = elev[x][y] - veg[x][y][0];
              if (elevdiff < 0)  elevdiff = 0;double temptot3 = temptot1;
              temptot1 = 0;

              for (int n = 1; n <= 9; n++)
              {
                temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
                if(elev[x][y] <= veg[x][y][0]) temp_dist[n] = 0;
                temptot1 += temp_dist[n];
              }

              if (temptot1 < 0) temptot1 = 0;
            }
          }  // Here endeth the veg-block

          if (temptot1 > tempbmax2[y]) tempbmax2[y] = temptot1;   // I have no idea what this is for...
          // Too many temp... temp this temp that....need to rename - DAV

          // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
          // BEDLOAD ROUTING
          //
          //
          // Work out what portion of bedload has to go where
          if (temptot1 > 0)
          {
            for (int p=1; p <= 8; p+=2)
            {
              int x2 = x + deltaX[p];
              int y2 = y + deltaY[p];

              if (water_depth[x2][y2] > water_depth_erosion_threshold)
              {
                if (index[x2][y2] == -9999) addGS(x2, y2);
                double factor =0;

                // vel slope
                if (vel_dir[x][y][p] > 0)
                {
                  factor += 0.75 * tempdir[p] / veltot;
                }
                // now for the lateral gradient
                if (edge[x][y] > edge[x2][y2])
                {
                  factor += 0.75 * ((edge[x][y] - edge[x2][y2]) / temptot2);
                }

                // Loop through the grainsizes
                for (int n=1; n<=9; n++)
                {
                  if (temp_dist[n] > 0)
                  {
                    if (n == 1 && isSuspended[n])
                    {
                      // Put the amount entrained by ss into ss[,]
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
                    }       // It's curly braces all the way down...
                  }
                }

              }
            }       // Refuge point for curly brace fatigue...
          }
        }
      }
      // reduction on tempbmax (DAV for parallelism implementation later on)
      for (int y=1; y<=ymax; y++) if (tempbmax2[y] > tempbmax) tempbmax = tempbmax2[y];   // This is the actual reduction bit

      if (tempbmax > ERODEFACTOR)
      {
        time_factor *= (ERODEFACTOR / tempbmax) * 0.5;
      }
    } // ENDFOR
  } while (tempbmax > ERODEFACTOR);




  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //  UPDATING GRAINSIZE ETC
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


  // new temp erode total array.
  TNT::Array2D<double> erodetot(xmax + 2, ymax + 2);
  TNT::Array2D<double> erodetot3(xmax + 2, ymax +2);

  //var options1 = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
  //Parallel.For(2, ymax, options1, delegate(int y)
  for (int y=2; y<ymax; y++)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      int x = down_scan[y][inc];
      inc++;

      if (water_depth[x][y] > water_depth_erosion_threshold && x < xmax && x > 1)
      {
        if (index[x][y] == -9999) addGS(x, y);
        for (int n = 1; n <= 9; n++)
        {
          if (n == 1 && isSuspended[n])
          {
            // updating entrainment of SS
            Vsusptot[x][y] += ss[x][y];
            grain[ index[x][y] ][n] -= ss[x][y];
            erodetot[x][y] -= ss[x][y];

            // this next part is unusual. You have to stop susp sed deposition on the input cells, otherwies
            // it drops sediment out, but cannot entrain as ss levels in input are too high leading to
            // little mountains of sediment. This means a new array in order to check whether a cell is an
            // input point or not..
            if (!inputpointsarray[x][y])
            {
              // now calc ss to be dropped
              double coeff = (fallVelocity[n] * time_factor) / water_depth[x][y];
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
        if (erodetot[x][y] != 0) sort_active(x, y);

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
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x - 1][y] * time_factor /DX;
            else amt = bed_proportion * erodetot3[x][y] * (elev[x - 1][y] - elev[x][y]) / DX * 0.1;

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
          if (elev[x + 1][y] > elev[x][y] && x < xmax-1)
          {
            double amt = 0;
            if (water_depth[x + 1][y] < water_depth_erosion_threshold)
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x + 1][y] * time_factor / DX;
            else amt = bed_proportion * erodetot3[x][y] * (elev[x + 1][y] - elev[x][y]) / DX * 0.1;

            if (amt > 0)
            {
              amt *= 1 - (veg[x + 1][y][1] * (1 - veg_lat_restriction));
              if ((elev[x + 1][y] - amt) < bedrock[x + 1][y] || x + 1 == xmax) amt = 0;
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
  // (PARALLELISM GOES HERE)
  //Parallel.For2(2, ymax, delegate(int y)
  for (int y = 2; y < ymax; y++)
  {
    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      int x = down_scan[y][inc];
      inc++;
      {
        if (erodetot3[x][y] > 0)
        {
          double elev_update = 0;

          if (elev[x][y - 1] > elev[x][y])
          {
            double amt = 0;
            if (water_depth[x][y - 1] < water_depth_erosion_threshold)
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y - 1] * time_factor / DX;
            else amt = bed_proportion * erodetot3[x][y] * (elev[x][y - 1] - elev[x][y]) / DX *0.1;

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
              amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y + 1] * time_factor / DX;
            else amt = bed_proportion * erodetot3[x][y] * (elev[x][y + 1] - elev[x][y]) / DX * 0.1;

            if (amt > 0)
            {
              amt *= 1 - (veg[x][y + 1][1] * (1 - veg_lat_restriction));
              if ((elev[x][y + 1] - amt) < bedrock[x][y + 1] || y + 1 == ymax) amt = 0;
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
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // SEDIMENT OUTPUT AT EDGES
  //
  //
  // now calculate sediment outputs from all four edges...
  for (int y = 2; y < ymax; y++)
  {
    if (water_depth[xmax][y] > water_depth_erosion_threshold || Vsusptot[xmax][y] > 0)
    {
      for (int n = 1; n <= 9; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[xmax][y];
          Vsusptot[xmax][y] = 0;
        }
        else
        {
          gtot2[n] += sr[xmax - 1][y][n];
        }
      }
    }
    if (water_depth[1][y] > water_depth_erosion_threshold || Vsusptot[1][y] > 0)
    {
      for (int n = 1; n <= 9; n++)
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

  for (int x = 2; x < xmax; x++)
  {
    if (water_depth[x][ymax] > water_depth_erosion_threshold || Vsusptot[x][ymax] > 0)
    {
      for (int n = 1; n <= 9; n++)
      {
        if (isSuspended[n])
        {
          gtot2[n] += Vsusptot[x][ymax];
          Vsusptot[x][ymax] = 0;
        }
        else
        {
          gtot2[n] += sd[x][ymax - 1][n];
        }
      }
    }
    if (water_depth[x][1] > water_depth_erosion_threshold || Vsusptot[x][1] > 0)
    {
      for (int n = 1; n <= 9; n++)
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
  for (int n = 1; n <= G_MAX; n++)
  {
    if (temp_grain[n] < 0) temp_grain[n] = 0;
    if (recirculate_opt == true && reach_mode_opt == true)
      temp_grain[n] += gtot2[n] * recirculate_proportion; // important to divide input by time factor, so it can be reduced if re-circulating too much...
    sediQ += gtot2[n] * DX * DX;
    globalsediq += gtot2[n] * DX * DX;
    sum_grain[n] += gtot2[n] * DX * DX; // Gez
  }

  return tempbmax;
}

// Does the lateral erosion
void LSDCatchmentModel::lateral3()
{
  //double[,] edge_temp, edge_temp2, water_depth2;
  //int[,] upscale, upscale_edge;

  // declare arrays and initialise the size of them
  TNT::Array2D<double> edge_temp(xmax + 1, ymax + 1, 0.0);
  TNT::Array2D<double> edge_temp2(xmax + 1, ymax + 1, 0.0);
  TNT::Array2D<double> water_depth2(xmax + 1, ymax + 1, 0.0);

  TNT::Array2D<int> upscale( (xmax + 1)*2, (ymax + 1)*2, 0);
  TNT::Array2D<int> upscale_edge( (xmax + 1)*2, (ymax + 1)*2, 0);

  //edge_temp = new Double[xmax + 1][ymax + 1];
  //edge_temp2 = new Double[xmax + 1][ymax + 1];
  //water_depth2 = new Double[xmax + 1][ymax + 1];
  //upscale = new int[(xmax + 1) * 2][(ymax + 1) * 2];
  //upscale_edge = new int[(xmax + 1) * 2][(ymax + 1) * 2];


  // first make water depth2 equal to water depth then remove single wet cells frmo water depth2 that have an undue influence..
  double mft = 0.1;// water_depth_erosion_threshold;//MIN_Q;// vel_dir threshold
  for (int y = 2; y < ymax; y++)
  {

    int inc = 1;
    while (down_scan[y][inc] > 0)
    {
      int x = down_scan[y][inc];

      edge_temp[x][y] = 0;
      if (x == 1) x++;
      if (x == xmax) x--;
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

  for (int y = 2; y < ymax; y++)
  {

    for (int x = 2; x < xmax; x++)
    {

      edge[x][y] = -9999;
      //std::cout << "water depth2: " << water_depth2[x][y] << std::endl;
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

  for (int y = 2; y < ymax * 2; y++)
  {
    for (int x = 2; x < xmax * 2; x++)
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


  for (int y = 2; y < ymax * 2; y++)
  {
    for (int x = 2; x < xmax * 2; x++)
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
        //
        water = wetcells - drycells;
        upscale_edge[x][y] = water;
      }
    }
  }

  // now update normal edge array..

  for (int x = 1; x <= xmax; x++)
  {
    for (int y = 1; y <= ymax; y++)
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

  //then apply a smoothing filter over the top of this. here its done X number of times -

  //double smoothing_times = double.Parse(avge_smoothbox.Text);
  //double downstream_shift = double.Parse(downstreamshiftbox.Text);

  for (int n = 1; n <= smoothing_times+downstream_shift; n++)
  {
    //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
    //Parallel.For(2, ymax, options, delegate(int y)
    for (int y=2; y<ymax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        int x = down_scan[y][inc];

        edge_temp[x][y] = 0;
        if (x == 1) x++;
        if (x == xmax) x--;
        if (y == 1) y++;
        if (y == ymax) y--;
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

            if ( n > smoothing_times && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft && mean_ws_elev(x2,y2)>mean_ws_elev(x,y))
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

            else if ( n <= smoothing_times && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft)
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

          // removes too many cells - islands etc..

          //if(num>5&&edge[x,y]>0)edge_temp[x,y]=0;
          //if(num+water_flag>7&&edge[x,y]>0)edge_temp[x,y]=0;

          //remove edge effects
          if (x < 3 || x > (xmax - 3)) edge_temp[x][y] = 0;
          if (y < 3 || y > (ymax - 3)) edge_temp[x][y] = 0;

        }
      }
    }//);

    for (int y = 2; y < ymax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        int x = down_scan[y][inc];
        //if (x == 1) x++;
        //if (x == xmax) x--;
        inc++;
        if (edge[x][y] > -9999)
        {
          edge[x][y] = edge_temp[x][y];
        }
      }
    }
  }


  // trial line to remove too high inside bends,,
  for (int x = 1; x <= xmax; x++)
  {
    for (int y = 1; y <= ymax; y++)
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
    //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
    //Parallel.For(2, ymax, options, delegate(int y)
    for (int y = 2; y < ymax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        int x = down_scan[y][inc];

        edge_temp[x][y] = 0;
        if (x == 1) x++;
        if (x == xmax) x--;
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
    }//);

    tempdiff = 0;
    for (int y = 2; y < ymax; y++)
    {
      int inc = 1;
      while (down_scan[y][inc] > 0)
      {
        int x = down_scan[y][inc];
        if (x == 1) x++;
        if (x == xmax) x--;
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
  //tempStatusPanel.Text = Convert.ToString(counter);

}

void LSDCatchmentModel::creep(double time)
{
  /** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
  /** very important differnece here is that slide_GS() is called only if
        BOTH cells are not -9999 therfore if both have grainsize then do additions.
        this is to stop the progressive spread of selected cells upslope */

  int x,y;
  double temp;

  for(x=1;x<=xmax;x++)
  {
    for(y=1;y<=ymax;y++)
    {
      tempcreep[x][y]=0;
    }
  }


  for(x=2;x<xmax;x++)
  {
    for(y=2;y<ymax;y++)
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

  for(x=1;x<=xmax;x++)
  {
    for(y=1;y<=ymax;y++)
    {
      elev[x][y]+=tempcreep[x][y];
    }
  }

}


void LSDCatchmentModel::slide_3()
{
  int x,y,inc;
  double wet_factor;
  double factor=std::tan((failureangle*(3.141592654/180)))*DX;
  double diff=0;

  for(y=2;y<ymax;y++)
  {

    inc=1;
    while(down_scan[y][inc]>0)
    {

      x=down_scan[y][inc];
      if(x==xmax)x=xmax-1;
      if(x==1)x=2;

      inc++;
      /** check to see if under water **/
      wet_factor=factor;
      //if(water_depth[x][y]>0.01)wet_factor=factor/2;
      if(elev[x][y]<=(bedrock[x][y]+active))wet_factor=10000;

      /** chexk landslides in channel slowly */

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

void LSDCatchmentModel::slide_5()
{
  int x, y, inc=0;
  double wet_factor;
  double factor = std::tan((failureangle * (3.141592654 / 180))) * DX;
  //if(landslidesBox.Checked == true) factor = DX * ((-265000 * j_mean) + 1.38);
  double diff = 0;
  double total = 0;

  if (dunes_opt == true)
  {
    for (x = 1; x <= xmax; x++)
    {
      for (y = 1; y <= ymax; y++)
      {
        elev[x][y] -= sand[x][y];
      }
    }
  }


  do
  {
    total = 0;
    inc++;
    for (y = 2; y < ymax; y++)
    {
      for (x = 2; x < xmax; x++)
      {

        wet_factor = factor;
        //if(water_depth[x,y]>0.01)wet_factor=factor/2;
        if (elev[x][y] <= (bedrock[x][y] + active)) wet_factor = 10 * DX;

        /** chexk landslides in channel slowly */

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
    for (x = 1; x <= xmax; x++)
    {
      for (y = 1; y <= ymax; y++)
      {
        elev[x][y] += sand[x][y];
      }
    }
  }

}

void LSDCatchmentModel::soil_erosion(double time)
{
  /** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
  /** very important differnece here is that slide_GS() is called only if
        BOTH cells are not -9999 therfore if both have grainsize then do additions.
        this is to stop the progressive spread of selected cells upslope */

  int x, y;
  double temp;


  for (x = 1; x <= xmax; x++)
  {
    for (y = 1; y <= ymax; y++)
    {
      tempcreep[x][y] = 0;
    }
  }


  for (x = 2; x < xmax; x++)
  {
    for (y = 2; y < ymax; y++)
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

  for (x = 1; x <= xmax; x++)
  {
    for (y = 1; y <= ymax; y++)
    {
      elev[x][y] += tempcreep[x][y];
    }
  }
}

// all based on Van Walleghem et al., 2013 (JGR:ES)
void LSDCatchmentModel::soil_development()
{
  for (int x = 1; x <= xmax; x++)
  {
    for (int y = 1; y <= ymax; y++)
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
          //amt = k1 * std::exp(-c1*depth_below_surface) * (c2/std::log10(particle_size) * time;
          // start from top then work down - means no need to hold values in temp arrays...
          double Di = 0;
          int xyindex = index[x][y];


          for (int n = 2; n <= (G_MAX - 1); n++)
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

          //for (int z = 9; z >= 1; z--)
          //{
          //    for (int n = 0; n <= G_MAX - 2; n++)
          //    {

          //        strata[xyindex][z][n] = strata[xyindex][z - 1][n];

          //    }
          //}

        }

        if (chem_weath_opt == true) // Chemical Weathering
        {
          //amt = k2 * std::exp(-c3 * depth_below_surface) * c4 * Specific_surface_area * time;
        }
      }
    }
  }
}
#endif
