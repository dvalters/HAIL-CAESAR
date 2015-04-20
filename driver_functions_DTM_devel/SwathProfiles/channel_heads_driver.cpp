//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// channel_heads_driver.cpp
// A driver function for use with the Land Surace Dynamics Topo Toolbox
// This program calculates channel heads using a chi method described in
// Clubb et al. (2014)
//
// Reference: Clubb, F. J., S. M. Mudd, D. T. Milodowski, M. D. Hurst,
// and L. J. Slater (2014), Objective extraction of channel heads from
// high-resolution topographic data, Water Resour. Res., 50, doi: 10.1002/2013WR015167.
//
// Developed by:
//  Fiona Clubb
//  Simon M. Mudd
//
// Copyright (C) 2013 Fiona Clubb and Simon M. Mudd 2013
//
// Developer can be contacted by simon.m.mudd _at_ ed.ac.uk
//
//    Simon Mudd
//    University of Edinburgh
//    School of GeoSciences
//    Drummond Street
//    Edinburgh, EH8 9XP
//    Scotland
//    United Kingdom
//
// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the
// GNU General Public License along with this program;
// if not, write to:
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301
// USA
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Fiona Clubb, University of Edinburgh
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include "../../LSDStatsTools.hpp"
#include "../../LSDRaster.hpp"
#include "../../LSDIndexRaster.hpp"
#include "../../LSDFlowInfo.hpp"
#include "../../LSDJunctionNetwork.hpp"
#include "../../LSDIndexChannelTree.hpp"
#include "../../LSDChiNetwork.hpp"

int main (int nNumberofArgs,char *argv[])
{

  cout << "Welcome to the channel heads driver program. This program finds channel\n"
       << "heads using the Dreich algorithm (Clubb et al, 2014, WRR)\n"
       << "To run this program you need a driver file with the name of the DEM\n"
       << "WITHOUT extension, the minimum slope, a threshold number of pixels for\n"
       << "intial channel identification, A_0 and m/n values for chi analysis\n"
       << "and the number of connecting nodes that wil be considered a candidate channel\n\n"
       << "Here is an example:\n"
       << "indian_creek\n"
       << "0.0001\n"
       << "250\n"
       << "1000\n"
       << "0.45\n"
       << "10\n\n";

  //Test for correct input arguments
  if (nNumberofArgs!=3)
  {
    cout << "FATAL ERROR: wrong number inputs. The program needs the path name and the file name" << endl;
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

  string DEM_name;
  string fill_ext = "_fill";
  string sources_ext = "_CH";
  file_info_in >> DEM_name;
  int threshold;
  float Minimum_Slope;
  float A_0;
  float m_over_n;
  int no_connecting_nodes;
  float curv_threshold;

  file_info_in >> Minimum_Slope >> threshold >> A_0 >> m_over_n >> no_connecting_nodes >> curv_threshold;

  // get some file names
  string DEM_f_name = DEM_name+fill_ext;
  string DEM_flt_extension = "flt";
  string complete_fname = DEM_name+sources_ext;//".flt";

  // load the DEM
  LSDRaster topo_test(DEM_name, DEM_flt_extension);

  // Set the no flux boundary conditions
  vector<string> boundary_conditions(4);
  boundary_conditions[0] = "No";
  boundary_conditions[1] = "no flux";
  boundary_conditions[2] = "no flux";
  boundary_conditions[3] = "No flux";

  // get the filled file
  cout << "Filling the DEM" << endl;
  LSDRaster filled_topo_test = topo_test.fill(Minimum_Slope);
  filled_topo_test.write_raster((DEM_f_name),DEM_flt_extension);

  //get a FlowInfo object
  LSDFlowInfo FlowInfo(boundary_conditions,filled_topo_test);
  LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();
  LSDIndexRaster ContributingPixels = FlowInfo.write_NContributingNodes_to_LSDIndexRaster();

	//string NI_name = "_NI";
  //LSDIndexRaster NodeIndex = FlowInfo.write_NodeIndex_to_LSDIndexRaster();
	//NodeIndex.write_raster((path_name+DEM_name+NI_name), DEM_flt_extension);

  //get the sources: note: this is only to select basins!
  vector<int> sources;
  sources = FlowInfo.get_sources_index_threshold(ContributingPixels, threshold);

  // now get the junction network
  LSDJunctionNetwork ChanNetwork(sources, FlowInfo);

  // Get the valleys using the contour curvature
  float surface_fitting_window_radius = 6;      // the radius of the fitting window in metres
  vector<LSDRaster> surface_fitting;
  LSDRaster tan_curvature;
  string curv_name = "_tan_curv";
  vector<int> raster_selection(8, 0);
  raster_selection[6] = 1;                      // this indicates you only want the tangential curvature
  surface_fitting = filled_topo_test.calculate_polyfit_surface_metrics(surface_fitting_window_radius, raster_selection);

  // now print the tangential curvature raster to file.
  for(int i = 0; i<int(raster_selection.size()); ++i)
  {
    if(raster_selection[i]==1)
    {
      tan_curvature = surface_fitting[i];
      tan_curvature.write_raster((path_name+DEM_name+curv_name), DEM_flt_extension);
    }
  }
  // Find the valley junctions
  Array2D<float> tan_curv_array = tan_curvature.get_RasterData();
  cout << "got tan curvature array" << endl;
	Array2D<int> valley_junctions = ChanNetwork.find_valleys(FlowInfo, tan_curv_array, sources, no_connecting_nodes,curv_threshold);

	// Write the valley junctions to an LSDIndexRaster
  //string VJ_name = "_VJ";
  //LSDIndexRaster ValleyJunctionsRaster (NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,valley_junctions);
	//ValleyJunctionsRaster.write_raster((path_name+DEM_name+VJ_name),DEM_flt_extension);

	cout << "Got valley junctions, proceeding to chi analysis" << endl;

	// Calculate the channel head nodes
  int MinSegLength = 10;
  vector<int> ChannelHeadNodes = ChanNetwork.GetChannelHeadsChiMethodFromValleys(valley_junctions, MinSegLength,
                                                 A_0, m_over_n, FlowInfo, DistanceFromOutlet, filled_topo_test);

  //write channel_heads to a csv file
  FlowInfo.print_vector_of_nodeindices_to_csv_file(ChannelHeadNodes, complete_fname);


//   string prefix = "/home/smudd/LSDTopoData/LiDAR_datasets/USA/OH/indian_creek2_fill_nodeindices_for_Arc";
  string suffix = "_nodeindices_for_Arc";
  FlowInfo.Ingest_Channel_Heads(complete_fname+suffix,"csv");

  //write channel heads to a raster
  string CH_name = "_CH";
  LSDIndexRaster Channel_heads_raster = FlowInfo.write_NodeIndexVector_to_LSDIndexRaster(ChannelHeadNodes);
  Channel_heads_raster.write_raster((DEM_name+CH_name),DEM_flt_extension);

  //create a channel network based on these channel heads
  LSDJunctionNetwork NewChanNetwork(ChannelHeadNodes, FlowInfo);
  //int n_junctions = NewChanNetwork.get_Number_of_Junctions();
  LSDIndexRaster SOArrayNew = NewChanNetwork.StreamOrderArray_to_LSDIndexRaster();
  string SO_name_new = "_SO_from_CH";

  SOArrayNew.write_raster((DEM_name+SO_name_new),DEM_flt_extension);

}
