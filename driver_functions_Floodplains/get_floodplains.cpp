//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// get_floodplain_mask
//
// This program takes two arguments, the path name and the driver name
// It gets a mask of the floodplain from a DEM. User has the option to filter the DEM
// using a Perona-Malik filter before getting the floodplain. After the inital floodplain mask
// is found, patches identified that are not connected to the channel network are removed.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// 19/10/16
// Fiona J. Clubb
// University of Edinburgh
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "../LSDStatsTools.hpp"
#include "../LSDChiNetwork.hpp"
#include "../LSDRaster.hpp"
#include "../LSDIndexRaster.hpp"
#include "../LSDFlowInfo.hpp"
#include "../LSDJunctionNetwork.hpp"
#include "../LSDIndexChannelTree.hpp"
#include "../LSDShapeTools.hpp"
#include "../LSDFloodplain.hpp"
#include "../LSDParameterParser.hpp"


int main (int nNumberofArgs,char *argv[])
{
	//start the clock
	clock_t begin = clock();

	//Test for correct input arguments
	if (nNumberofArgs!=3)
	{
    cout << "=========================================================" << endl;
    cout << "|| Welcome to the LSDTopoTools floodplain extraction!  ||" << endl;
    cout << "|| This program extracts floodplains using slope and	  ||" << endl;
		cout << "|| relief thresholds. This program was developed by    ||" << endl;
    cout << "|| Fiona J. Clubb												              ||" << endl;
    cout << "|| at the University of Edinburgh                      ||" << endl;
    cout << "=========================================================" << endl;
    cout << "This program requires two inputs: " << endl;
    cout << "* First the path to the parameter file." << endl;
    cout << "* Second the name of the param file (see below)." << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Then the command line argument will be, for example: " << endl;
    cout << "In linux:" << endl;
    cout << "./get_floodplains.out /LSDTopoTools/Topographic_projects/Test_data/ LSDTT_floodplains.param" << endl;
    cout << "=========================================================" << endl;
    exit(EXIT_SUCCESS);
	}

	string path_name = argv[1];
	string f_name = argv[2];

	// maps for setting default parameters
	map<string,int> int_default_map;
	map<string,float> float_default_map;
	map<string,bool> bool_default_map;
	//map<string,string> string_default_map;

	// set default int parameters
	int_default_map["Threshold_SO"] = 3;
	int_default_map["Relief lower percentile"] = 25;
	int_default_map["Relief upper percentile"] = 75;
	int_default_map["Slope lower percentile"] = 25;
	int_default_map["Slope upper percentile"] = 75;
	int_default_map["Min patch size"] = 1000;

	// set default float parameters
	float_default_map["surface_fitting_window_radius"] = 6;
	float_default_map["Min slope filling"] = 0.0001;
	float_default_map["QQ threshold"] = 0.005;

	// set default bool parameters
	bool_default_map["Filter topography"] = true;

	// Use the parameter parser to get the maps of the parameters required for the
	// analysis
	// load parameter parser object
	LSDParameterParser LSDPP(path_name,f_name);
	LSDPP.force_bil_extension();

	LSDPP.parse_all_parameters(float_default_map, int_default_map, bool_default_map,string_default_map);
	map<string,float> this_float_map = LSDPP.get_float_parameters();
	map<string,int> this_int_map = LSDPP.get_int_parameters();
	map<string,bool> this_bool_map = LSDPP.get_bool_parameters();
	map<string,string> this_string_map = LSDPP.get_string_parameters();

	// Now print the parameters for bug checking
	LSDPP.print_parameters();

	// location of the files
	string DATA_DIR =  LSDPP.get_read_path();
	string DEM_ID =  LSDPP.get_read_fname();
	string OUT_DIR = LSDPP.get_write_path();
	string OUT_ID = LSDPP.get_write_fname();
	string DEM_extension =  LSDPP.get_dem_read_extension();
	vector<string> boundary_conditions = LSDPP.get_boundary_conditions();
	string CHeads_file = LSDPP.get_CHeads_file();

	// some error checking
	if (CHeads_file.empty())
	{
		cout << "FATAL ERROR: I can't find your channel heads file. Check your spelling!! \n The parameter key needs to be 'channel heads fname'" << endl;
		exit(EXIT_SUCCESS);
	}

	cout << "I've parsed all your parameters...now I'm going to get you some FLOODPLAINS :)" << endl;

	LSDRaster filled_topo_test;

  if(this_bool_map["Filter topography"])
  {
		// load the DEM
		cout << "Loading the DEM..." << endl;
		LSDRaster load_DEM((DATA_DIR+DEM_ID), DEM_extension);
		filled_topo_test = load_DEM;

		// filter using Perona Malik
		int timesteps = 50;
		float percentile_for_lambda = 90;
		float dt = 0.1;
		filled_topo_test = filled_topo_test.PeronaMalikFilter(timesteps, percentile_for_lambda, dt);

		// fill
		filled_topo_test = filled_topo_test.fill(this_float_map["Min slope filling"]);
		string fill_name = "_filtered";
		filled_topo_test.write_raster((DATA_DIR+DEM_ID+fill_name), DEM_extension);
  }
  else
  {
    //previously done the filtering and filling, just load the filled DEM
		LSDRaster load_DEM((DATA_DIR+DEM_ID+"_filtered"), DEM_extension);
    filled_topo_test = load_DEM;
  }


  cout << "\t Flow routing..." << endl;
	// get a flow info object
 	LSDFlowInfo FlowInfo(boundary_conditions,filled_topo_test);

	cout << "\t Loading Sources..." << endl;
	// load the sources
	vector<int> sources = FlowInfo.Ingest_Channel_Heads((DATA_DIR+CHeads_file), 0);
	cout << "\t Got sources!" << endl;

	// now get the junction network
	LSDJunctionNetwork ChanNetwork(sources, FlowInfo);
  cout << "\t Got the channel network" << endl;

  //calculate the channel relief
  cout << "\t Getting relief relative to channel" << endl;
  cout << "\t Threshold stream order = " << this_int_map["Threshold_SO"] << endl;
  LSDRaster ChannelRelief = ChanNetwork.calculate_relief_from_channel(filled_topo_test, FlowInfo, this_int_map["Threshold_SO"]);
  string relief_name = "_channel_relief";
  ChannelRelief.write_raster((DATA_DIR+DEM_ID+relief_name), DEM_extension);
  cout << "\t Got the relief!" << endl;

  //get the slope
  cout << "\t Calculating slope..." << endl;
  vector<LSDRaster> surface_fitting;
  LSDRaster Slope;
  vector<int> raster_selection(8, 0);
  raster_selection[1] = 1;             // this means you want the slope
  surface_fitting = filled_topo_test.calculate_polyfit_surface_metrics(this_float_map["surface_fitting_window_radius"], raster_selection);
  Slope = surface_fitting[1];
  cout << "\t Done!" << endl;
  string slope_name = "_slope";
  Slope.write_raster((DATA_DIR+DEM_ID+slope_name), DEM_extension);

  // get the channel relief and slope threshold using quantile-quantile plots
  cout << "Getting channel relief threshold from QQ plots" << endl;
  string qq_fname = DATA_DIR+DEM_ID+"_qq_relief.txt";
  float relief_threshold_from_qq = ChannelRelief.get_threshold_for_floodplain_QQ(qq_fname, this_float_map["QQ threshold"], this_int_map["Relief lower percentile"], this_int_map["Relief upper percentile"]);

  cout << "Getting slope threshold from QQ plots" << endl;
  string qq_slope = path_name+DEM_ID+"_qq_slope.txt";
  float slope_threshold_from_qq = Slope.get_threshold_for_floodplain_QQ(qq_slope, this_float_map["QQ threshold"], this_int_map["Slope lower percentile"], this_int_map["Slope upper percentile"]);

	cout << "Relief threshold: " << relief_threshold_from_qq << " Slope threshold: " << slope_threshold_from_qq << endl;

	// get the distance from outlet
	LSDRaster DistFromOutlet = FlowInfo.distance_from_outlet();

	// get the floodplain object
	cout << "Getting the floodplain object" << endl;
	LSDFloodplain Floodplain(ChannelRelief, Slope, ChanNetwork, FlowInfo, relief_threshold_from_qq, slope_threshold_from_qq, this_int_map["Min patch size"], this_int_map["Threshold_SO"];

	// get the relief relative to nearest channel
	Floodplain.Get_Relief_of_Nearest_Channel(ChanNetwork, FlowInfo, filled_topo_test, DistFromOutlet, this_int_map["Threshold_SO"]);

	LSDIndexRaster BinaryRaster = Floodplain.print_BinaryRaster();
	string bin_ext = "_FP";
	BinaryRaster.write_raster((DATA_DIR+DEM_ID+bin_ext), DEM_extension);

	// Done, check how long it took
	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << "DONE, Time taken (secs): " << elapsed_secs << endl;
}
