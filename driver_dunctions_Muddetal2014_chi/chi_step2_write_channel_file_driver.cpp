//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Chile_test.cpp
//
// This program is used for testing the LSDRaster object
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Simon M. Mudd
// University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../LSDStatsTools.hpp"
#include "../LSDRaster.hpp"
#include "../LSDIndexRaster.hpp"
#include "../LSDFlowInfo.hpp"
#include "../LSDJunctionNetwork.hpp"
#include "../LSDIndexChannelTree.hpp"

int main (int nNumberofArgs,char *argv[])
{
	//Test for correct input arguments
	if (nNumberofArgs!=3)
	{
		cout << "FATAL ERROR: wrong number inputs. The program needs the path name and the file name" << endl;
		exit(EXIT_SUCCESS);
	}

	string path_name = argv[1];
	
  // make sure there is a slash on the end of the file
  string lchar = path_name.substr(path_name.length()-2,1);
  string slash = "/";      
  if (lchar != slash)
  {
    cout << "You forgot the frontslash at the end of the path. Appending." << endl; 
    path_name = path_name+slash;
  } 		
	
	string f_name = argv[2];

  cout << "\nYou are running the write junctions driver." << endl
       <<"IMPORTANT: this has been updated to load an ENVI DEM, whith extension .bil" << endl
       <<"You can convert your DEM to this file format using gdal_translate, with -of ENVI" << endl
       <<"See documentation at: http://www.geos.ed.ac.uk/~smudd/LSDTT_docs/html/gdal_notes.html" << endl << endl;
       
	cout << "The path is: " << path_name << " and the filename is: " << f_name << endl;

	string full_name = path_name+f_name;

	ifstream file_info_in;
	file_info_in.open(full_name.c_str());
	if( file_info_in.fail() )
	{
		cout << "\nFATAL ERROR: the header file \"" << full_name
		     << "\" doesn't exist" << endl;
		exit(EXIT_FAILURE);
	}

	string DEM_name;
	string fill_ext = "_fill";
	file_info_in >> DEM_name;
	int junction_number;
	float pruning_threshold;
	int threshold;
	float A_0;
	int minimum_segment_length;
	float sigma;
	float start_movern;
	float d_movern;
	float Minimum_Slope;
	int n_movern;
	int target_nodes;
	int n_iterations;
	float fraction_dchi_for_variation;
	float vertical_interval;
	float horizontal_interval;
	float area_thin_frac;
	int target_skip;

	file_info_in >> Minimum_Slope >> threshold >> junction_number
				>> pruning_threshold >> A_0 >> minimum_segment_length >> sigma >> start_movern
				>> d_movern >> n_movern >> target_nodes >> n_iterations >> fraction_dchi_for_variation
				>> vertical_interval >> horizontal_interval >> area_thin_frac >> target_skip;


	cout << "Paramters of this run: " << endl
	     << "junction number: " << junction_number << endl
	     << "pruning_threshold: " << pruning_threshold << endl
	     << "threshold: " << threshold << endl
	     << "A_0: " << A_0 << endl
	     << "minimum_segment_length: " << minimum_segment_length << endl
	     << "sigma: " << sigma << endl
	     << "start_movern " << start_movern << endl
	     << "d_movern: " << d_movern << endl
	     << "n_movern: " << n_movern << endl
	     << "target_nodes: " << target_nodes << endl
	     << "n_iterarions: " << n_iterations << endl
	     << "fraction_dchi_for_variation: " << fraction_dchi_for_variation << endl
	     << "vertical interval: " << vertical_interval << endl
	     << "horizontal interval: " << horizontal_interval << endl
	     << "area thinning fraction for SA analysis: " << area_thin_frac << endl
	     << "target_skip is: " << target_skip << endl;


	string jn_name = itoa(junction_number);
	string uscore = "_";
	jn_name = uscore+jn_name;
	file_info_in.close();

	string DEM_f_name = path_name+DEM_name+fill_ext;
	string DEM_bil_extension = "bil";

	// set no flux boundary conditions
	vector<string> boundary_conditions(4);
	boundary_conditions[0] = "No";
	boundary_conditions[1] = "no flux";
	boundary_conditions[2] = "no flux";
	boundary_conditions[3] = "No flux";


	// load the filled DEM
	LSDRaster filled_topo_test((path_name+DEM_name+fill_ext), DEM_bil_extension);

	// get a flow info object
	LSDFlowInfo FlowInfo(boundary_conditions,filled_topo_test);

	// calcualte the distance from outlet
	LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();
	LSDIndexRaster ContributingPixels = FlowInfo.write_NContributingNodes_to_LSDIndexRaster();

	// get the sources
	vector<int> sources;
	sources = FlowInfo.get_sources_index_threshold(ContributingPixels, threshold);

	// now get the junction network
	LSDJunctionNetwork ChanNetwork(sources, FlowInfo);

	// now get a junction and look for the longest channel upstream
	cout << "creating main stem" << endl;
	LSDIndexChannel main_stem = ChanNetwork.generate_longest_index_channel_in_basin(junction_number, FlowInfo, DistanceFromOutlet);
        cout << "got main stem channel, with n_nodes " << main_stem.get_n_nodes_in_channel() <<  endl;

	string Basin_name = "_basin";
	LSDIndexRaster BasinArray = ChanNetwork.extract_basin_from_junction(junction_number,junction_number,FlowInfo);
	BasinArray.write_raster((path_name+DEM_name+Basin_name+jn_name),DEM_bil_extension);

	// now get the best fit m over n for all the tributaries
	int organization_switch = 1;
	int pruning_switch = 1;
	LSDIndexChannelTree ChannelTree(FlowInfo, ChanNetwork, junction_number, organization_switch,
                                        DistanceFromOutlet, pruning_switch, pruning_threshold);

	// print a file that can be ingested bt the chi fitting algorithm
	string Chan_fname = "_ChanNet";
	string Chan_ext = ".chan";
	string Chan_for_chi_ingestion_fname = path_name+DEM_name+Chan_fname+jn_name+Chan_ext;
	ChannelTree.print_LSDChannels_for_chi_network_ingestion(FlowInfo,
                             filled_topo_test, DistanceFromOutlet, Chan_for_chi_ingestion_fname);
	ChannelTree.convert_chan_file_for_ArcMap_ingestion(Chan_for_chi_ingestion_fname);

}
