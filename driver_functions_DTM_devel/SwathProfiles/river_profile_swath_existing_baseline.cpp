//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// chi_map_sensitivity_analysis
//
// This program takes two arguments, the path name and the driver name
// It then runs the chi analysis for the parameter space defined in the driver
// file, allowing the user to examine the effects of changing m/n value,
// number of target nodes, minimum segment length, sigma and target skip value.
// At present it just spits out an output file for each iteration.
// Now also kicks out a table with the parameters and best fit m/n.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// 18/03/2014
// David Milodowski
// University of Edinburgh
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../../LSDStatsTools.hpp"
#include "../../LSDChiNetwork.hpp"
#include "../../LSDRaster.hpp"
#include "../../LSDRasterSpectral.hpp"
#include "../../LSDIndexRaster.hpp"
#include "../../LSDFlowInfo.hpp"
#include "../../LSDJunctionNetwork.hpp"
#include "../../LSDIndexChannelTree.hpp"
#include "../../LSDSwathProfile.hpp"
#include "../../LSDShapeTools.hpp"


int main (int nNumberofArgs,char *argv[])
{
	//Test for correct input arguments
	if (nNumberofArgs!=3)
	{
		cout << "FATAL ERROR: wrong number inputs. The program needs the path name, the driver file name" << endl;
		exit(EXIT_SUCCESS);
	}

	string path_name = argv[1];
	string f_name = argv[2];

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

	string DEM_ID;
	string RASTER_NAME;
	string input_path;
	string output_path;
  string dem_ext = "_dem";
  string sources_ext = "_CH";
  string flt_extension = "flt";
  string txt_extension = ".txt";
  string Swath_ext = "_swath_trans";
  string Long_Swath_ext = "_swath_long";
  
  // initialise variables to be assigned from .driver file
  int junction_number,NormaliseTransProfile,NormaliseLongProfile,FilterTopo;
	float pruning_threshold = 1;
	float HalfWidth,BinWidth,Minimum_Slope,histogram_bin_width,knickpoint_position;
  string temp;
                                                            
	file_info_in >> temp >> DEM_ID
               >> temp >> RASTER_NAME
               >> temp >> input_path
               >> temp >> output_path
               >> temp >> junction_number
               >> temp >> Minimum_Slope
               >> temp >> HalfWidth
               >> temp >> BinWidth
               >> temp >> NormaliseTransProfile
               >> temp >> NormaliseLongProfile
               >> temp >> FilterTopo
               >> temp >> histogram_bin_width
               >> temp >> knickpoint_position;
                   
	string jn_name = itoa(junction_number);
	string uscore = "_";
	jn_name = uscore+jn_name;
	file_info_in.close();

	// set no flux boundary conditions
	vector<string> boundary_conditions(4);
	boundary_conditions[0] = "No";
	boundary_conditions[1] = "no flux";
	boundary_conditions[2] = "no flux";
	boundary_conditions[3] = "No flux";

	// load the  DEM
// 	LSDRaster topo_test((input_path+DEM_ID+dem_ext), flt_extension);  
//   
//   if(FilterTopo == 1)
//   {
//     // filter
//     LSDRasterSpectral SpectralRaster(topo_test);
//     topo_test = SpectralRaster.fftw2D_wiener();
//     int border_width = 100;
//     topo_test = topo_test.border_with_nodata(border_width);    
//   }
//   // fill
//   LSDRaster filled_topo_test = topo_test.fill(Minimum_Slope);
//   cout << "\t Flow routing..." << endl;
// 	// get a flow info object
//  	LSDFlowInfo FlowInfo(boundary_conditions,filled_topo_test);
// 	
//   // calcualte the distance from outlet
// 	LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();
// 
// 	cout << "\t Loading Sources..." << endl;
// 	// load the sources
// 	string CH_name = "_CH";
// 	vector<int> sources = FlowInfo.Ingest_Channel_Heads((input_path+DEM_ID+dem_ext+CH_name), "csv",1);
//   cout << "\t Got sources!" << endl;
// 	// now get the junction network
// 	LSDJunctionNetwork ChanNetwork(sources, FlowInfo);
//   
//   string SO_ext = "_SO";
//   string JI_ext = "_JI";
//   LSDIndexRaster SOArray = ChanNetwork.StreamOrderArray_to_LSDIndexRaster();
//  	LSDIndexRaster JIArray = ChanNetwork.JunctionIndexArray_to_LSDIndexRaster();
// 
// 	SOArray.write_raster((output_path+DEM_ID+SO_ext),flt_extension);
//   JIArray.write_raster((output_path+DEM_ID+JI_ext),flt_extension);
// 	// now get a junction and look for the longest channel upstream
// 	cout << "\t creating main stem" << endl;
// 	LSDIndexChannel main_stem = ChanNetwork.generate_longest_index_channel_in_basin(junction_number, FlowInfo, DistanceFromOutlet);
//         cout << "\t\t got main stem channel, with n_nodes " << main_stem.get_n_nodes_in_channel() <<  endl;
// 
// 	// now create ChannelTree object that can be ingested by the swath profile tool
	string Chan_fname = "_ChanNet";
	string Chan_ext = ".chan";
	string Chan_for_swath_profile_fname = output_path+DEM_ID+Chan_fname+jn_name+Chan_ext;
// 	int organization_switch = 1;                                                  
// 	int pruning_switch = 1;     // get mainstem only
// 	LSDIndexChannelTree ChannelTree(FlowInfo, ChanNetwork, junction_number, organization_switch,
//                                         DistanceFromOutlet, pruning_switch, pruning_threshold);
// 	ChannelTree.print_LSDChannels_for_chi_network_ingestion(FlowInfo, filled_topo_test, DistanceFromOutlet, Chan_for_swath_profile_fname);
// 	ChannelTree.convert_chan_file_for_ArcMap_ingestion(Chan_for_swath_profile_fname);

  // NOW GET SWATH PROFILE
  cout << "\t SWATH PROFILES :-D" << endl;
  cout << "\t loading baseline points" << endl;
  PointData BaselinePoints = LoadChannelTree(Chan_for_swath_profile_fname);
  
  cout << "\t creating swath template" << endl;
  LSDRaster RasterTemplate((input_path+RASTER_NAME), flt_extension);  
  LSDSwath TestSwath(BaselinePoints, RasterTemplate, HalfWidth);
  vector<float> percentiles;
  percentiles.push_back(2.5);
  percentiles.push_back(25);
  percentiles.push_back(50);
  percentiles.push_back(75);
  percentiles.push_back(97.5);
  
  cout << "\n\t writing output \n\t\t - transverse profile" << endl;
  TestSwath.write_transverse_profile_to_file(RasterTemplate, percentiles, BinWidth, output_path+RASTER_NAME,NormaliseTransProfile);
  
  cout << "\t - longitudinal profile" << endl;
  TestSwath.write_longitudinal_profile_to_file(RasterTemplate, percentiles, BinWidth, output_path+RASTER_NAME,NormaliseLongProfile);
  
  cout << "\t - profile templates" << endl;
  LSDRaster Swath(RasterTemplate.get_NRows(),RasterTemplate.get_NCols(),RasterTemplate.get_XMinimum(),RasterTemplate.get_YMinimum(),
                  RasterTemplate.get_DataResolution(),RasterTemplate.get_NoDataValue(),TestSwath.get_DistanceToBaselineArray());  
  string output_file = output_path+RASTER_NAME+Swath_ext;
  Swath.write_raster(output_file.c_str(),flt_extension);
  
  LSDRaster Long_Swath(RasterTemplate.get_NRows(),RasterTemplate.get_NCols(),RasterTemplate.get_XMinimum(),RasterTemplate.get_YMinimum(),
                  RasterTemplate.get_DataResolution(),RasterTemplate.get_NoDataValue(),TestSwath.get_DistanceAlongBaselineArray());
  string output_file2 = output_path+RASTER_NAME+Long_Swath_ext;
  Long_Swath.write_raster(output_file2.c_str(),flt_extension);
  
  // Now get histogram of stats above and below knickpoint
  vector<float> data_above, data_below;
  float val;
  for(int i = 0; i<RasterTemplate.get_NRows(); ++i)
  {
    for(int j = 0; j<RasterTemplate.get_NCols(); ++j)
    {
      if(RasterTemplate.get_data_element(i,j)!=RasterTemplate.get_NoDataValue() && Long_Swath.get_data_element(i,j)!=Long_Swath.get_NoDataValue())
      {
        if(RasterTemplate.get_data_element(i,j) < 0) val=0;
        else val = RasterTemplate.get_data_element(i,j);
        if(Long_Swath.get_data_element(i,j)<knickpoint_position) data_above.push_back(val);
        else data_below.push_back(val); 
      }
    }
  }
  cout << "\t Getting histogram for region above knickpoint" << endl;  
  cout << data_above.size() << endl;
  print_histogram(data_above, histogram_bin_width, output_path+RASTER_NAME+"aboveKP.txt");
  cout << "\t Getting histogram for region below knickpoint" << endl;
  cout << data_below.size() << endl;
  print_histogram(data_below, histogram_bin_width, output_path+RASTER_NAME+"belowKP.txt");
}
