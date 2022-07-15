// LSDCatchmentModel.hpp
//
// Header file for the LSDCatchmentModel

#include <vector>
#include <cmath>
#include <string>
#include <array>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>

// Include for OpenMP
#include <omp.h>

// Topotools includes for stats tools and raster handling
#include "topotools/LSDRaster.hpp"
#include "topotools/LSDStatsTools.hpp"

#include "LSDGrainMatrix.hpp"
#include "LSDRainfallRunoff.hpp"

#include "TNT/tnt.h"   // Template Numerical Toolkit library: used for 2D Arrays.

#ifndef LSDCatchmentModel_H
#define LSDCatchmentModel_H

/// @brief This object is used to model the hydrology, sediment transport and
/// evolution of individual basins.
/// @details The object is (for now) just a rough and ready translation of the
/// CAESAR-Lisflood model - a hydrologically explicit landscape evolution model. It
/// models landscape evolution and hydro-geomorphic processes at the basin scale.
class LSDCatchmentModel: public LSDRaster
{
public:

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //
  // Constructors
  //
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  /// @brief The default constructor.
  /// @details this asks for a pathname and a filename of the parameter file
  /// It then opens the paramter file and ingests the information
  /// @author DAV
  /// @date 2015-01-16
  LSDCatchmentModel()
  {
    create();
  }

  /// @brief this constructor just reads the param file given by the path and
  /// filename. You must give the parameter file extension!
  /// @param pname the pathname to the parameter file
  /// @param fname the filename of the parameter file !!INCLUDING EXTENSION!!
  /// @author DAV
  /// @date 2015-01-16
  LSDCatchmentModel(string pname, string pfname)
  {
    std::cout << "The constructor has been called..." << std::endl;
    create(pname, pfname);
  }

  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // MODEL DOMAIN METHODS
  // Set up of model domain, initialisation
  // of arrays.
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  /// @brief Initialises the size of the arrays holding the various
  /// model fields such as elevation, water depth etc.
  void initialise_model_domain_extents();

  /// @brief Checks that there is a real terrain point on at least one side of the DEM
  /// and also counts the number of actual grid cells in the catchment.
  /// @details This only currently checks for an edge that is not NODATA on at least one side
  /// It does not check that the DEM has its lowest point on this edge. This
  /// should probably be added.
  void check_DEM_edge_condition();

  /// @brief reads data values from the parameter file into the relevant maps
  /// @return
  void initialise_variables(std::string pname, std::string pfname);

  /// @brief initialises array sizes based on DEM dimensions
  /// @details also sets 'hard-coded' parameters to start the model
  void initialise_arrays();

  int get_imax() const { return imax; }
  int get_jmax() const { return jmax; }

  // MODEL OPERATION FLAGS

  /// @brief Is this a hydrology only simulation?
  /// I.e. no erosion methods.
  bool is_hydro_only() const { return hydro_only; }

  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // INPUT/OUTPUT
  // Methods for loading and manipulating files
  // (should probably go in separate class/file)
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  /// @brief Loads the required data files based on the parameters set in the parameter file
  /// @author dav
  void load_data();

  /// Prints the initial values set by user from param file
  /// as well as those default initial values in the code.
  void print_parameters();

  /// @brief Loads the rainfall data file which is in a special format (headerless text file)
  /// @author DAV
  /// @details Rainfall data file is not too big, so think it's okay to use the vector<vector>
  /// method instead of TNT array. Easier to dynamically resize as the rainfall data contains
  /// no header and can vary in size. Saves the user having to count the rows and cols. Reads
  /// in the rainfall data file specified in the parameter list file as floats.
  /// @return Returns a vector of vector<float>. (A 2D-like vector).
  std::vector< std::vector<float> > read_rainfalldata(std::string FILENAME);

  /// @brief Reads in the grain data file, note, that this is not a raster and in
  /// a special format like the rainfall file.
  /// @author DAV
  /// @details The grain data file consists of multiple columns of data in a text file
  /// that store the grain size fractions for the surface and the subsurface strata. Also
  /// contains an index number and the x y location of each grid cell containing grain data.
  void ingest_graindata_from_file(std::string FILENAME);

  /// @brief Prints the contents of the rainfall data for checking
  /// @author DAV
  /// @details Uses iterators <iterator> header to iterate through
  /// the vector of vectors that is raingrid.
  /// @details Actually, this is a generic function for printing a 2D vector
  void print_rainfall_data();

  void print_reach_data();

  /// @brief Calls the various save functions depending on the data types to be saved (Raster Output)
  /// @author DAV
  /// @details dependent on the LSDRaster class calling the overloaded write_raster func. If you are looking
  /// for the function that writes the hydrograph/sediment time series, see the write_output() function.
  void save_raster_data(double tempcycle);

  /// @brief Writes the timeseries file for current timestep.
  /// @detail Writes discharge and sediment flux according to
  /// the same format as found in the CAESAR-Lisflood catchmetn
  void write_output_timeseries(runoffGrid& runoff);

  void save_raster_output();

  /// @brief Writes the time series of catchment output data.
  void output_data();

  /// @brief Writes the time series of catchment output data.
  /// @details Overloaded to take a reference to a runoff object
  /// to allow calculation from the OOP method.-
  /// @author DAV
  void output_data(double temptotal, runoffGrid& runoff);


  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // MODEL TIMING CONTROL
  // Routines for incrementing counters,
  // setting the timestep etc.
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  void set_time_counters();

  void set_loop_cycle();

  double courant_friedrichs_lewy_condition();

  /// @brief Calculates and sets the maximum time step (also the erosion timestep)
  void set_maximum_timestep();

  /// @brief calculates the timestep for the hydro/flow model
  double get_flow_timestep();

  void increment_counters();

  void print_cycle();

  double get_cycle() const { return cycle; }
  int get_maxcycle() const { return maxcycle; }

  /// @brief Zeros certain arrays which have to be reset every timestep
  /// or every certain number of timesteps.
  void zero_values();

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // CATCHMENT MORPHOMETRICS
  // Methods that return information about
  // catchment size, drainage area, wetted area
  // etc.
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  /// @brief Counts the number of cells within the catchment boundary.
  /// @details For
  /// spatially variable rainfall, this counts the number of cells
  /// within each hydroindex region that are within the boundary. It
  /// modifies nActualGridCells.
  void count_catchment_gridcells();

  /// @brief Calculate which cells within the catchment are
  /// underwater.
  void check_wetted_area(int scan_area_interval_iter);

  void initialise_drainage_area();

  /// @brief Calculates the area and calls area4().
  /// @details This is basically a wrapper function now. It sets area = 0,
  ///  and area_depth = 1 where there is actual elevation data. Then calls
  /// get_area4() which does the actual work.
  /// @author Translated by DAV
  void zero_and_calc_drainage_area();

  /// @brief Calculates the drainage area, after being called by get_area()
  /// @author Translated by DAV
  void drainage_area_D8();

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // EROSION COMPONENTS
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  /// @brief Wrapper function that calls the main erosion method
  /// @return
  void call_erosion();

  /// @brief Calls the lateral erosion method
  /// @return
  void call_lateral();

  /// @brief Sorts the acrive erosion layer in terms of its
  /// grainsizes.
  void sort_active(int x,int y);

  /// @brief Calculate the d50 grain size (median grainsize)
  double d50(int index1);

  /// @brief Calculates the sand fraction component "F_s" for the
  /// Wilcock sediment transport law.
  /// @return The sand fraction amount
  double sand_fraction(int index1);

  /// @brief Apportions the erosion amount to the grain
  /// size distribution fractions.
  void addGS(int x, int y);

  /// @brief Determines which grain size fractions are moved to
  /// neigbouring cells during erosion routines.
  void slide_GS(int x,int y, double amount,int x2, int y2);

  /// @brief (Used only in lateral channel erosion - under test)
  /// @returns "elevtot" or zero.
  double mean_ws_elev(int x, int y);

  /// @brief The main erosion routine
  /// @details Erosion only takes place above certain threshold.
  /// Contains calls to subroutines such as addGS() and d50().
  /// Needs re-factoring to extract seprate methods (bedrock vs loose sedi erosion etc.)
  /// @author DAV
  double erode(double mult_factor);

  /// @brief carries out the lateral bank erosion.
  /// @details This is quite computationall expensive and may be
  /// turned off in environments not susceptible to meandering. (bedrock
  /// mountain/upland channels for example.
  void lateral_channel_erode();

  /// @brief Sets the fall velocities of suspended sediment.
  /// @details These are hard coded. Should be set up to read from
  /// an sediment properties input file at a later stage.
  /// @author DAV
  void set_fall_velocities();

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // SLOPE PROCESS COMPONENTS
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  /// @brief Prevents overdeepening of channels through downcutting from
  /// the main erode function.
  /// @detail I.e. removes material in channel
  /// when neighbouring pixels have gradient between them greater
  /// than critical angle.
  void in_channel_landsliding();

  /// @brief Checks whether slopes have exceded critical failure angle
  /// and if so, does a simple landslide routine.
  /// @details Lowers elevation of higher cell and deposits on lower cell
  /// until gradient between two is below the critical angle.
  void global_landsliding();

  void call_global_landsliding(int global_landsliding_interval_hours);

  void call_channel_landsliding(int in_channel_landslide_iter_interval);

  // Slope creep is a wrapper that calls the creep (main soil creep
  // method.)
  void slope_creep(int creep_time_interval_days, double creep_coeff);

  void creep( double );

  void soil_erosion( double time );

  void soil_development();

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // HYDROLOGY COMPONENTS
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  /// @brief Initialises the rainfall runoff grid if using
  /// spatially complex rainfall runoff object.
  void initialise_rainfall_runoff(runoffGrid& runoff);

  /// @brief Updates the water depths (and susp sedi concentrations)
  void depth_update();

  /// @brief Performs the water routing method
  /// @details Uses the Bates et al. (2010) simplification of the St. Venant
  /// shallow water equations. Does not assume steady state flow across the
  /// landscape. (i.e. this is the LISFLOOD algorithm)
  void flow_route();

  /// @brief Wrapper that determines which water input routine to call,
  /// either the default one, or the object-based one with spatially complex
  /// runoff paterns.
  /// @todo This logic needs simplifying, why bother creating a runoff object
  /// if it is never used for the simple runoff case (which is most uses.)
  void catchment_waterinputs(runoffGrid& runoff);

  /// @brief Calculates the amount of runoff on a grid-cell from the rainfall
  /// timeseries input
  void catchment_water_input_and_hydrology( double flow_timestep);

  /// @brief Overloaded function is for when u<a href="#datastructures">Understanding sing the fully distriuted/complex
  /// rainfall patterns option in the model. Takes a reference to the runoffGrid
  /// object.
  void catchment_water_input_and_hydrology( double flow_timestep, runoffGrid& runoff);

  /// @brief Calculates the hydrological inputs using just reach mode
  void reach_water_and_sediment_input();

  std::vector<std::vector<float> > read_reachfile(std::string REACHINPUTFILE);

  /// @brief Gets the number of catchment cells that have water input to them
  /// @detail Calculates which cells contain a discharge greater than MIN_Q
  /// and lower than MIN_Q_MAXVAL multiplies by a parameter related to the
  /// contributng draingage area and the BASEFLOW parameter.
  void get_catchment_input_points();

  /// @brief Same as method above but uses the runoff object-based approach
  /// @detail This version is still experimental as of 2017 -DAV TODO
  void get_catchment_input_points(runoffGrid& runoff);

  /// Calculates the amount of water entering grid cells from the rainfall timeseries
  /// and hydroindex if spatially variable rainfall is used.
  /// @details Based on the semi-dsitributed TOPMODEL rainfall runoff model
  void topmodel_runoff( double cycle);

  /// Calculates amount of water entering grid cells when using the fully-distributed
  /// rainfall runoff model (i.e. where every single cell can have diffferent rainfall
  /// and saturation levels.
  /// @details Based on TOPMODEL, modified to fully 2D distributed version
  void topmodel_runoff(double cycle, runoffGrid& runoff);

  /// @brief Calculates the hydrograph values (TOPMODEL) for printing to
  /// the output timeseries file.
  void calchydrograph( double time);

  /// @brief Overloaded function for calculating hydrograph when using the fully distributed
  /// model. Takes an extra reference to the runoff object.
  void calchydrograph(double time, runoffGrid& runoff);

  /// @brief Evaporation routine.
  void evaporate(double time);

  /// @brief Calculates which cells have water content and marks these
  /// cells with a flag.
  /// @detail By creating a mask of cells that contain water, later calculations
  /// can be sped up by only iterating over cells that contain water (and there
  /// -fore will have discharges, water to route, erosion occuring etc.)
  /// Interesting to see how useful this is in catchments where majority of
  /// cells contain water, i.e. during flood events, etc. and whether there
  /// is worth to this method which has overhead but ultimately reduces
  /// compute time, especially in situations where there is large areas of
  /// catchment with minimal water content.
  void scan_area();

  /// @brief Calculates water exiting from the catchment boundaries (on all
  /// four sides of the domain, regardles of where 'true' catchment outlet
  /// point is.)
  void water_flux_out();

  /// @brief Calculates the difference between water entering the catchment
  /// and water leaving the catchment.
  /// If this value is below a user-set threshold, the timestep can be increased
  /// during periods of low water flow. (e.g. inter-storm periods.)
  void set_inputoutput_diff();

  // =-=-=-=-=-=-=-=-=-=-=
  // VEGETATION
  // =-=-=-=-=-=-=-=-=-=-=

  /// @brief Wrapper function that determines whehter to call
  /// the main grass growing function "grow_grass"
  void grow_vegetation(int vegetation_growth_interval_hours);

  /// @brief Routine to grow grass for vegetation enabled simulations.
  void grow_grass(double amount3);

  // =-=-=-=-=-=-=-=-=-=-=-=-=
  // GROUNDWATER
  // =-=-=-=-=-=-=-=-=-=-=-=-=
  void groundwater_input();  // wpgw_water_input
  void groundwater_flow();   // Groundwater_Flow
  void clear_Water_partitioning();

private:

  string dem_read_extension;
  string dem_write_extension;
  string write_path;
  string read_path;
  string write_fname = "catchment.dat";
  string read_fname;

  bool uniquefilecheck = false;

  //constants
  const double root = 7.07;
  const double gravity = 9.81;
  const float g = 9.81F;
  const float kappa = 0.4F;
  const int ACTIVE_FACTOR=1;
  const int TRUE=1;
  const int FALSE=0;
  const unsigned int G_MAX=10;
  const std::array<int, 9> deltaX = {{0,  0,  1,  1,  1,  0, -1, -1, -1}};
  const std::array<int, 9> deltaY = {{0, -1, -1,  0,  1,  1,  1,  0, -1}};

  double water_depth_erosion_threshold = 0.01;
  int reach_input_data_timestep = 60;
  int stage_reach_input_data_timestep = 60;
  int number_of_points = 0;
  double globalsediq = 0;
  double time_1 = 1;
  double save_time = 0;
  double creep_time = 1;
  double creep_time2 = 1;
  double creep_time3 = 1;
  double grass_grow_interval = 1;

  double soil_erosion_time = 1;
  double soil_development_time = 1;

  double bedrock_erosion_threshold = 0;
  double bedrock_erosion_rate = 0;
  double p_b = 1.5;  // detach capacity exponent from CHILD
  double bedrock_erodibility_coeff_ke = 0.002;

  ///int tot_number_of_tracer_points=0;
  int input_type_flag=0; // 0 is water input from points, 1 is input from hydrograph or rainfall file.
  double failureangle=45;
  double saveinterval=1000;
  int counter=0;

  double waterinput = 0;
  double waterOut = 0;
  double input_output_difference = 0;
  double in_out_difference_allowed = 0;
  double mannings = 0.04;

  /// no. of rainfall cells
  unsigned rfnum = 1;

  /// set by ncols and nrows
  unsigned int jmax = 0;
  unsigned int imax = 0;
  double xll = 0;
  double yll = 0;
  double no_data_value = -9999.9;

  int maxcycle = 1000;

  double ERODEFACTOR=0.05;
  double DX=5.0;

  /// memory limit
  int LIMIT=1;
  double MIN_Q=0.01;
  double MIN_Q_MAXVAL=1000.0;
  double CREEP_RATE=0.0025;
  double SOIL_RATE = 0.0025;
  double active=0.2;
  int grain_array_tot =1;

  /// Number of passes for edge smoothing filter
  double edge_smoothing_passes = 100.0;
  /// Number of cells to shift lat erosion downstream
  double downstream_shift= 5.0;
  /// Max difference allowed in cross channel smoothing of edge values
  double lateral_cross_channel_smoothing = 0.0001;
  double lateral_constant=0.0000002;

  double time_step = 1;
  std::vector<double> j, jo, j_mean, old_j_mean, new_j_mean;

  /// TOPMODEL 'm'
  double M = 0.005;
  double baseflow = 0.00000005; //end of hyd model variables usually 0.0000005 changed 2/11/05
  // Reverted to match CL 1.8f 17/08/16 - DV

  double cycle =0;  // can't initalise static vars in header file!
  double rain_factor = 1;
  double sediQ = 0;

  // speed in which vegetation reaches full maturity in year
  double grow_grass_time = 0;
  double duneupdatetime = 0;

  double output_file_save_interval = 60;
  double min_time_step = 0;
  double vegTauCrit = 100;

  int max_time_step = 0;
  int dune_mult = 5;
  double dune_time = 1;
  double max_vel = 5;
  double sand_out = 0;
  double maxdepth = 10;
  double courant_number = 0.7;
  double erode_call = 0;
  double erode_mult = 1;
  double lateralcounter = 1;
  double edgeslope = 0.001;
  double chann_lateral_erosion = 20.0; // formerly 'bed_proportion =0.01'
  double veg_lat_restriction = 0.1;

  double froude_limit = 0.8;
  double recirculate_proportion = 1;

  double Csuspmax = 0.05; // max concentration  of SS allowed in a cell (proportion)
  double hflow_threshold = 0.00001;

  // KAtharine
  int variable_m_value_flag = 0;

  // TO DO: DAV - these could be read from an input file.
  // Swale grainsizes
  double d1=0.000065;
  double d2=0.001;
  double d3=0.002;
  double d4=0.004;
  double d5=0.008;
  double d6=0.016;
  double d7=0.032;
  double d8=0.064;
  double d9=0.128;

  // std::array<double, 11> dprop = {{0.0, 0.144, 0.022, 0.019, 0.029, 0.068, 0.146, 0.22, 0.231, 0.121, 0.0}}; // Default
  double dprop[11] = {0.0, 0.05, 0.05, 0.15, 0.225, 0.25, 0.1, 0.075, 0.05, 0.05, 0.0}; // Swale

  double previous;
  int hours = 0;
  double new_cycle = 0;
  double old_cycle = 0;
  double tx = 60;
  double Tx = 0;
  double tlastcalc = 0;
  double Qs_step = 0;
  double Qs_hour = 0;
  double Qs_over = 0;
  double Qs_last = 0;
  double Qw_newvol = 0;
  double Qw_oldvol = 0;
  double Qw_lastvol = 0;
  double Qw_stepvol = 0;
  double Qw_hourvol = 0;
  double Qw_hour = 0;
  double Qw_overvol = 0;
  double temptotal = 0;
  double old_sediq = 0;

  double temptot = 0;

  std::vector<double> sum_grain, sum_grain2;
  std::vector<double> old_sum_grain,old_sum_grain2;
  std::vector<double> Qg_step, Qg_step2, Qg_hour, Qg_hour2;
  std::vector<double> Qg_over, Qg_over2, Qg_last,Qg_last2;

  TNT::Array2D<double> elev;
  TNT::Array2D<double> bedrock;
  TNT::Array2D<double> init_elevs;
  TNT::Array2D<double> water_depth;
  TNT::Array2D<double> area;
  TNT::Array2D<double> tempcreep;
  TNT::Array2D<double> Tau;
  TNT::Array2D<double> Vel;
  TNT::Array2D<double> qx;
  TNT::Array2D<double> qy;
  TNT::Array2D<double> qxs;
  TNT::Array2D<double> qys;
  // TODO - these are for the dune model which is as of yet unimplemented
  TNT::Array2D<double> area_depth;
  TNT::Array2D<double> sand;
  TNT::Array2D<double> elev2;
  TNT::Array2D<double> sand2;
  TNT::Array2D<double> grain;
  TNT::Array2D<double> elev_diff;
  TNT::Array2D<double> spat_var_mannings;

  TNT::Array2D<int> index;
  TNT::Array2D<int> down_scan;
  TNT::Array2D<int> rfarea;

  // Reach input cell flag switches
  bool reach1_input_on = false;
  bool reach2_input_on = false;
  bool reach3_input_on = false;

  std::string reach1_input_file;
  std::string reach2_input_file;
  std::string reach3_input_file;

  int reach1_x;
  int reach1_y;
  int reach2_x;
  int reach2_y;
  int reach3_x;
  int reach3_y;

  // Arrays for reach mode input points
  TNT::Array2D<int> inpoints;
  TNT::Array2D<bool> inputpointsarray;

  std::vector< std::vector<float> > hourly_rain_data;
  std::vector<std::vector<std::vector<float> > > inputfile;
  //TNT::Array3D<double> inputfile;
  std::vector<double> stage_inputfile;
  // TODO above these all need initialising from read ins.

  double stage_input_time_step = 1;

  std::vector<int> catchment_input_x_coord;
  std::vector<int> catchment_input_y_coord;

  TNT::Array3D<double> vel_dir;
  TNT::Array3D<double> strata;

  std::vector<double> hourly_m_value;
  std::vector<double> temp_grain;
  
  TNT::Array3D<double> veg;
  TNT::Array2D<double> edge, edge2; //TJC 27/1/05 array for edges
  std::vector<double> old_j_mean_store;
  TNT::Array3D<double> sr, sl, su, sd;
  TNT::Array2D<double> ss;

  // MJ global vars
  std::vector<double> fallVelocity;
  std::vector<bool> isSuspended;
  TNT::Array2D<double> Vsusptot;


  std::vector<int> nActualGridCells;
  double Jw_newvol = 0.0;
  double Jw_oldvol = 0.0;
  double Jw_lastvol = 0.0;
  double Jw_stepvol = 0.0;
  double Jw_hourvol = 0.0;
  double Jw_hour = 0.0;
  double Jw_overvol = 0.0;
  double k_evap = 0.0;

  // sedi tpt flags
  bool einstein = false;
  bool wilcock = false;
  int div_inputs = 1;
  double rain_data_time_step = 60; // time step for rain data - default is 60.

  // lisflood caesar adaptation globals
  std::vector<int> catchment_input_counter;
  int totalinputpoints = 0;

  /// Soil generation variables
  double P1, b1, k1, c1, c2, k2, c3, c4;

  /// Option Bools
  bool soildevoption = false;
  bool suspended_opt = false;
  bool jmeaninputfile_opt = false;
  // This is for reading discharge direct from input file - DAV 2/2016
  // (Dear god I've been working on this code for 9 years nearly...)
  bool recirculate_opt = false;
  bool reach_mode_opt = false;
  bool dunes_opt = false;
  bool bedrock_lower_opt = false;
  bool physical_weather_opt = false;
  bool chem_weath_opt = false;
  bool allow_in_out_diff = true;

  bool soil_j_mean_depends_on = false;
  bool rainfall_data_on = false;
  bool water_init_from_raster = false;
  bool hydro_only = false;
  bool vegetation_on = false;
  bool bedrock_layer_on = false;
  bool lateral_erosion_on = false;
  bool spatially_var_rainfall = false;
  bool spatially_var_mannings = false;
  bool graindata_from_file = false;

  bool spatially_complex_rainfall = false;

  bool groundwater_basic = false;
  bool groundwater_SLiM;

  int erode_timestep_type = 0;  // 0 for default based on erosion amount, 1 for basedon hydro timestep
  int hydro_timestep_type = 0;  // 0 for default

  // Bools for writing out files
  bool write_elev_file = false;
  bool write_grainsz_file = false;
  bool write_params_file = false;
  bool write_flowvel_file = false;
  bool write_waterd_file = false;
  bool write_elevdiff_file = false;

  /// input file names
  std::string rainfall_data_file = "";
  std::string grain_data_file = "";
  std::string bedrock_data_file = "";
  std::string water_init_raster_file = "";

  /// output file names
  std::string elev_fname = "";
  std::string grainsize_fname = "";
  std::string params_fname = "";
  std::string waterdepth_fname = "";
  std::string flowvel_fname = "";
  std::string hydroindex_fname = "";
  std::string timeseries_fname = "";
  std::string elevdiff_fname = "";
  std::string raingrid_fname = "";
  std::string runoffgrid_fname = "";
  std::string mannings_fname = "";

  bool DEBUG_print_cycle_on = false;
  bool DEBUG_write_raingrid = false;
  bool DEBUG_write_runoffgrid = false;

  int timeseries_interval;
  float run_time_start;
  int no_of_iterations;

  int tempcycle = 0;

  std::vector< std::vector<float> > raingrid;	 // this is for the rainfall data file

  // Mainly just the definitions of the create() functions go here:
  // The implementations are in the .cpp file.

  void create();
  void create(std::string pname, std::string pfname);
};


// "There is no science without fancy and no art without facts", wrote the great
// Russian author Vladimir Nabokov. I include here an ascii rendering of Vincent Van Gogh's
// The Cafe Terrace at Night, (1888, oil on canvas). Thanks go to 'SSt' for the rendering.
//
// You can enjoy it while your code is compiling.
//
/*

|---|| |---|-|     |                           |
'.--'| |---|-|     |(`,         ,      _      ||
 '---' '---'-'     |-._   O           `-`    | |  ___
            ___    |  _`-._       .         _| | |-|-|
           |---|-. | |    |`.              /   | |-|-|
    |      |---|-|_| |____| |    ,-.    , |    | |-|-|
    |`.  _ |---|-|-|.       |    `-'      |    | '-'-'
       `| `|`-.|-|-||       | ,      .  |||    |
`-._     \`.`-|'-|-||       |     ,^.  /  |: : |  ___
    `-._  \ `-.`-|`||.__    |   . |||_|   |    | |-|-|
`-._    |  \   `-._`'._|``.-+._   |,' |   |    | |-|-|
`-._`-._)   \      `-.._``'-||_/_,|  .| ,.|    | |-|-|
-.,':-.,'    \          ``--._/   |: :| ::|    | '-'-'
  \ |  |      \    .-.._     :    |   |   |: : |
  : |-.|       \   '.._/     '    |   |   |    |
  | |  |        \  ,^.      :     |   |   |    |
--| |  |. |`.    `.| |     :      |   |   |,._.--------
  | |  ||_|__|____|`,'----.'   :: |   |   / _|`.``.````
  | |  ||(|,'|)|`.|`.-.-. |       | ::|  ,|`'|-|  |
  | |  |' |  | | || | | | |       |   |  ||`'|-|  |.
  | |  |  |  | | ||_|_|_|_|--__---'--._`-||`'|-|__|
  | |  |  |  | | ,`,.'`,.' \`,.'   .   `-._`.|_|__|____
--| |  |  |  | |__'  `'__`,.'  `__      .  `-----------
  | |  |  |  |_`-.-')_`-.-|| \`-.-'   .      -      -
  | |  |  |,'-.-'^. | |,^|||  \,^._`_ -   -     `-.
  | |  | ,'__,^. ,-.   ___   ,-.< ___ >         -  `-.
  | |  |'< __ >  |_| < ___ > |_|\`.|,' `.   -       - `
  | |  |___\/___ |._._`.|,'_ |._.,'|`. `.`.     -     -
,'| |  |-  )( -: '| | ,'|`.   | |      - `.`.-     -
  | |  |   -  '' -     ::     -     -     -`.`. -     -
  |_|,' -    ::     -  ::  -     -     -     `.`.  -
 ,'  -     -..   -     ::     -     -     -    `.`.   -
'       -   ''-        ::  -     -     -     -   `.`SSt


*/

#endif
