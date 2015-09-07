// LSDCatchmentModel.hpp
//
// Header file for the LSDCatchmentModel

#include <vector>
#include <cmath>
#include <string>
#include <array>
#include <map>
#include <fstream>
#include <sys/stat.h>

#include "LSDRaster.hpp"

#include "TNT/tnt.h"   // Template Numerical Toolkit library: used for 2D Arrays.

using std::string;

#ifndef LSDCatchmentModel_H
#define LSDCatchmentModel_H

/// @brief Template for sorting a std::pair.
/// @details This template will perform a sort on the std::pair types. It
/// will sort ascending based on the second item in the pair. It is intended
/// to mimic the C# method: sort(Array,Array). Needs further testing, but
/// should work in principle. 
/// @author DAV
template <class T1, class T2, class Pred = std::less<T2> >
struct sort_pair_second
{
	bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right)
	{
		Pred p;
		return p(left.second, right.second);
	}
};



///@brief This object is used to model the hydrology, sediment transport and evolution
///of individual basins.
///@details The object is (for now) just a rough and ready translation of the CAESAR-Lisflood model - a hydrologically explicit landscape evolution model. It 
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
	
	//-------------------------------------
	//MPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPM
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// The Meat and Potatoes of the Class
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//MPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPMPM
	
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Stuff for loading and manipulating files
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	
	/// @brief Loads the required data files based on the parameters set in the parameter file 
	/// @author dav
	void load_data();
	
	/// @brief Loads the rainfall data file which is in a special format (headerless text file)
	/// @author DAV
	/// @details Rainfall data file is not too big, so think it's okay to use the vector<vector> 
	/// method instead of TNT array. Easier to dynamically resize as the rainfall data contains
	/// no header and can vary in size. Saves the user having to count the rows and cols. Reads
	/// in the rainfall data file specified in the parameter list file as floats.
	/// @return Returns a vector of vector<float>. (A 2D-like vector). 
	std::vector< std::vector<float> > read_rainfalldata(std::string FILENAME);
	
	/// @brief Calls the various save functions depending on the data types to be saved
	/// @author DAV
	/// @details dependent on the LSDRaster class calling the overloaded write_raster func
	void save_data(int typeflag, double tempcycle);
	
	/// @brief Checks to see if a file exists
	/// @author DAV (Thanks to StackOverflow)
	/// @return Returns a bool value
	/// @details Uses the <sys/stat.h> include. May not work on Windows? Need to test.
	/// Checks to see if a file exists using the stat function.
	/// @param const std::string &name, where name is the full filename.
	/// @return Returns a true bool value if the file exists.
	inline bool does_file_exist(const std::string &filename);
	
	/// @brief Parses lines in an input file for ingestion
	/// @author Whoever wrote LSDStatsTools - borrowed here by DAV
	/// @note Would fit better in some generic class for file functions etc? LSDFileTools?
	void parse_line(std::ifstream &infile, string &parameter, string &value); 
	
	/// @brief Removes the end-of-line weird character mess that you get in Windows
	/// @author JAJ? SMM? - borrowed here by DAV	
	std::string RemoveControlCharactersFromEndOfString(std::string toRemove);
	
	/// @brief reads data values from the parameter file into the relevant maps
	/// @return 
	void initialise_model(std::string pname, std::string pfname);	
	
	/// @brief Wrapper function that calls the main erosional and depositional methods
	/// @return 
	void run_components();
	
	/// @brief Wrapper function that calls the main erosion method
	/// @return 	
	void call_erosion();
	
	/// @brief Calls the lateral erosion method
	/// @return
	void call_lateral();

	//// @brief Calculates water fluxes out of the catchment
	//// @return	
	////void water_flux_out();
	
	/// @brief Calls the landsliding methods
	/// @return	
	void call_landsliding();
	
	void call_slide5();
	
	/// @brief Calls the soil creep method
	/// @return
	void call_soilcreep();
	
	/// @brief Calls the soil erosion method
	/// @return	
	void call_soil_erosion();

	/// @brief Calls the soil development method
	/// @return	
	void call_soil_devel();
	
	/// @brief Calls the evapo-transpiration method
	/// @return
	void call_evapotrans();

	/// @brief Calls the grass growing method
	/// @return
	void call_grass_growing();
	
	/// @brief Calculates the area, although it also calls
	/// get_area4(), which is slightly confusing
	/// @author Translated by DAV
	void get_area();
	
	/// @brief Calculates the area, after being called by get_area()
	/// @author Translated by DAV	
	void get_area4();
	
	void get_catchment_input_points();
	
	void output_data();
	
	void save_data_and_draw_graphics();
	
	void intialise_params();
	
	void zero_values();
	
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// EROSION COMPONENTS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-	

	/// @brief Carries out the main erosional and depositional methods
	/// @return 
	void sort_active(int x,int y);
	
	double d50(int index1);
	
	double sand_fraction(int index1);
	
	void addGS(int &x, int &y);
	
	void slide_GS(int x,int y, double amount,int x2, int y2);
	
	double mean_ws_elev(int x, int y);
	
	double erode(double mult_factor);
	
	void lateral3();
	
	void slide_3();
	
	void slide_5();
	
	void creep( double );
	
	void soil_erosion( double time );
	
	void soil_development();
	
	
	
	
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// HYDROLOGY COMPONENTS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		
	/// @brief Updates the water depths (and susp sedi concentrations)
	/// @return	nowt (it's a void)
	void depth_update();
	
	/// @brief Performs the water routing method
	/// @return nowt (it's a void)
	void qroute();
	
	void catchment_water_input_and_hydrology( double local_time_factor);
	
	void calc_J( double cycle);
	
	void calchydrograph( double time);
	
	void Cdepth_update();
	
	void evaporate(double time);
	
	void scan_area();
	
	void water_flux_out(double local_time_factor);	
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//
	// Maps holding parameters and switches read from input paramter files
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// DAV - I am not sure whether these should be public or protected data members
	// LSDModelDriver needs read/write access to them, so my guess would be public?
	
	// CM = LSDCatchmentModel specific
	// (RM = LSDRasterModel specific)


	/// This map holds all the possible model switches
	std::map<std::string,bool> CM_model_switches; 

	/// This holds names of methods. For example, if the key is 'sed_transport_law', the std::string is
	/// the method which is used to calculate sediment transport (such as 'wilcock' or 'einstein')
	std::map<std::string,std::string> CM_method_map;

	/// This holds float parameters
	std::map<std::string,float> CM_float_parameters;

	/// This holds integer parameters
	std::map<std::string,int> CM_int_parameters;

	/// This holds names of supporting files, for example files that contain
	/// node of junction indices to be loaded. 
	std::map<std::string,std::string> CM_support_file_names;
	
	string dem_read_extension;
	string dem_write_extension;
	string write_path;
	string read_path;
	string write_fname;
	string read_fname;
	
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// MASSIVE LIST OF VARIABLES USED IN CL
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	
	// DAV: 2014-10-01
	// At the moment I have just copied and pasted all the variable declartions
	// anddefinitions from the CL original source file. Some of them can be thinned out
	// and should also consider where is best to place them (private/public etc)
	
	// TO DO: Convert C-style arrays to std::vector<type>
	
	// HINT:
	// C# Arrays:
	// double[] array_name
	//
	// C++ Arrays:
	// double array_name[]
	//
	// C++ equivalent vector:
	// std::vector<double>(length) {Initial_value1, initial_value2, etc}
	
	// You can also use the TNT arrays (Template Numerical Toolkit)
	// TNT::Array2D<float> {1.0, 2.0, 3.0}
	//
	// Note that C++ vectors do not need tobe intialised with a length or
	// initial values if you don't want them to, they will dynamicaly resize
	// as need. (Unlike the standard practice of initiaising C-style arrays 
	// with new[] etc....

	//static double magnifyValue;// = 0;
	//static int updateClick; // = 0;
	//std::vector<double>[] zoomFactor = { .25, .33, .50, .66, .80, 1, 1.25, 1.5, 2.0, 2.5, 3.0 };
	//double[] contrastFactor = { 1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3 };
	//double contrastMultiplier = 0;
	//int imageCount = 1;
	//int imageCount2 = 0;
	//int coordinateDone = 0;
	//double urfinalLati, urfinalLongi, llfinalLati, llfinalLongi, yurcorner, xurcorner = 0;
	//std::string kml = "";
	//std::string KML_FILE_NAME = "animation\\animation.kml";
	//int save_time2, save_interval2 = 0;
	//std::string startDate, kmlTime;
	
	//std::string[] DateArray;
	//std::string[] DateArray2;
   
	// toms global variables
	// DAV: Note: You are only allowed to initialize non-static variables in C++11.
	bool uniquefilecheck = false;
	
	double gravity = 9.81;
	const float g = 9.81F;
	const float kappa = 0.4F;
	double water_depth_erosion_threshold = 0.01;
	int input_time_step = 60;
	int number_of_points = 0;
	double globalsediq = 0;
	double time_1 = 1;
	double save_time = 0;
	double creep_time = 1;
	double creep_time2 = 1;
	double creep_time3 = 1;
	
	double soil_erosion_time = 1;
	double soil_development_time = 1;

	double bedrock_erosion_threshold = 0;
	double bedrock_erosion_rate = 0;

	//int tot_number_of_tracer_points=0;
	int input_type_flag=0; // 0 is water input from points, 1 is input from hydrograph or rainfall file.
	double failureangle=45;
	double saveinterval=1000;
	int counter=0;

	bool googleoutputflag = false;
	double waterinput = 0;
	double waterOut = 0;
	double in_out_difference = 0;
	double mannings = 0.04;
	int rfnum = 2;

	int xmax, ymax;
	double xll, yll;

	int maxcycle;
	const int ACTIVE_FACTOR=1;
	const int TRUE=1;
	const int FALSE=0;
	double ERODEFACTOR=0.05;
	double DX=5.0;
	double root=7.07;

	int LIMIT=1;
	double MIN_Q=0.01; // PARAM
	double MIN_Q_MAXVAL=1000.0; // PARAM
	double CREEP_RATE=0.0025;
	double SOIL_RATE = 0.0025;
	double active=0.2;
	int G_MAX=10;
	double lateral_constant=0.0000002;
	int grain_array_tot =1 ;
	
	double smoothing_times = 100.0;
	double downstream_shift= 5.0;

	double time_factor = 1;
	std::vector<double> j, jo, j_mean, old_j_mean, new_j_mean;
	double M = 0.005;
	double baseflow = 0.00000005; //end of hyd model variables usually 0.0000005 changed 2/11/05
	double cycle =0;  // can't initalise static vars in header file!
	double rain_factor = 1;
	double sediQ = 0;
	double grow_grass_time = 0;
	double duneupdatetime = 0;
	
	double output_file_save_interval = 60;
	double min_time_step = 0;
	double vegTauCrit = 100;
	int graphics_scale;// = 2; // value that controls the number of bmp pixels per model pixel for the output images.
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
	double bed_proportion = 0.01;
	double veg_lat_restriction = 0.1;
	double lateral_cross_channel_smoothing = 0.0001;
	double froude_limit = 0.8;
	double recirculate_proportion = 1;

	double Csuspmax = 0.05; // max concentration  of SS allowed in a cell (proportion)
	double hflow_threshold = 0.001;

	// KAtharine
	int variable_m_value_flag = 0;

	// grain size variables - the sizes
	double d1=0.0005;
	double d2=0.001;
	double d3=0.002;
	double d4=0.004;
	double d5=0.008;
	double d6=0.016;
	double d7=0.032;
	double d8=0.064;
	double d9=0.128;

	// grain size proportions for each fraction... as a proportion of 1.
	
	double d1prop=0.144;
	double d2prop=0.022;
	double d3prop=0.019;
	double d4prop=0.029;
	double d5prop=0.068;
	double d6prop=0.146;
	double d7prop=0.22;
	double d8prop=0.231;
	double d9prop=0.121;

	// Gez
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
	std::vector<double> sum_grain, sum_grain2, old_sum_grain,old_sum_grain2, Qg_step, Qg_step2, Qg_hour, Qg_hour2, Qg_over, Qg_over2, Qg_last,Qg_last2;
	std::string CATCH_FILE = "catchment.dat";
	// end gez

	// toms global arrays 
	
	// DAV: Move towards using the LSD Objects such as LSDRaster for reading/storing DEMs and LSDBasin
	LSDRaster elevR;
	LSDRaster hydroindexR;
	LSDRaster bedrockR;
	LSDRaster water_depthR;

	/*static*/ TNT::Array2D<double> elev, bedrock, init_elevs, water_depth, area, tempcreep, Tau, Vel, qx, qy, qxs, qys,
		/* dune arrays */ area_depth, sand, elev2, sand2, grain, elev_diff;
	TNT::Array2D<int> index, cross_scan,down_scan, rfarea;
	TNT::Array2D<bool> inputpointsarray;
	std::vector<int> catchment_input_x_coord, catchment_input_y_coord;
	
	TNT::Array3D<double> vel_dir;
	TNT::Array3D<double> strata;
	std::vector<double> hourly_m_value, temp_grain;
	TNT::Array2D<double> hydrograph, dischargeinput;  //, hourly_rain_data; // made this vector below, see if it causes problems?
	std::vector< std::vector<float> > hourly_rain_data;
	TNT::Array3D<double> inputfile;
	TNT::Array2D<int> inpoints;
	TNT::Array3D<double> veg;
	TNT::Array2D<double> edge, edge2; //TJC 27/1/05 array for edges
	std::vector<double> old_j_mean_store;
	TNT::Array2D<double> climate_data;
	TNT::Array3D<double> sr, sl, su, sd;
	TNT::Array2D<double> ss;

	
	// MJ global vars
	std::vector<double> fallVelocity;
	std::vector<bool> isSuspended;
	TNT::Array2D<double> Vsusptot;
	std::array<int, 9> deltaX = {{0, 0, 1, 1 ,1 , 0, -1, -1, -1}};
	std::array<int, 9> deltaY = {{0, -1, -1, 0, 1, 1, 1, 0, -1}};
	//int[] deltaY = new int[9] {0,-1,-1,0,1,1,1,0,-1};   // I leave an old example in for comparison - DAV
	std::vector<int> nActualGridCells;
	double Jw_newvol = 0.0;
	double Jw_oldvol = 0.0;
	double Jw_lastvol = 0.0;
	double Jw_stepvol = 0.0;
	double Jw_hourvol = 0.0;
	double Jw_hour = 0.0;
	double Jw_overvol = 0.0;
	double k_evap = 0.0;

	// JOE global vars
	std::string inputheader;			//Read from ASCII DEM <JOE 20050605>
	TNT::Array2D<double> slopeAnalysis;  // Initially calculated in percent slope, coverted to radians
	TNT::Array2D<double> aspect;		  // Radians
	TNT::Array2D<double> hillshade;	  // 0 to 255
	double hue = 360.0;		// Ranges between 0 and 360 degrees
	double sat = 0.90;		// Ranges between 0 and 1.0 (where 1 is 100%)
	double val = 1.0;		// Ranges between 0 and 1.0 (where 1 is 100%)
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	// siberia submodel parameters
	double m1 = 1.70;
	double n1 = 0.69;
	double Beta3 = 0.000186;
	double m3 = 0.79;
	double Beta1 = 1067;

	// sedi tpt flags   
	int einstein = 0;
	int wilcock = 0;
	int div_inputs = 1;
	double rain_data_time_step = 60; // time step for rain data - default is 60. 


	// lisflood caesar adaptation globals
	std::vector<int> catchment_input_counter;
	int totalinputpoints = 0;

	//JMW Vars
	std::string basetext = "CAESAR - Lisflood 1.8a (21/7/2014)";
	std::string cfgname = "";  //Config file name // Removed NULL string - can't have these in C++
	std::string workdir = "c:\\program files\\Caesar\\work\\";

	// stage/tidal variables
	int fromx, tox, fromy, toy;
	double stage_input_time_step = 1;
	std::vector<double> stage_inputfile;

	// Soil generation variables
	double P1, b1, k1, c1, c2, k2, c3, c4;
	
	// Option Bools
	bool soildevoption;
	bool suspended_opt;
	bool jmeaninputfile_opt;
	bool recirculate_opt;
	bool reach_mode_opt;
	bool dunes_opt;
	bool bedrock_lower_opt;
	bool physical_weather_opt;
	bool chem_weath_opt;
	
	protected:
	std::vector< std::vector<float> > raingrid;	 // this is for the rainfall data file
	
	// Mainly just the definitions of the create() functions go here:
	// The implementations are in the .cpp file.
	private:
	void create();  
	void create(std::string pname, std::string pfname);
};
#endif


