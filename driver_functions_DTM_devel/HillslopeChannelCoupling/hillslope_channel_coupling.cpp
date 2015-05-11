//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// threshold_quartile_quantile.cpp
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// David T Milodowski
// University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include "../../LSDStatsTools.hpp"
#include "../../LSDRaster.hpp"
#include "../../LSDFlowInfo.hpp"
#include "../../LSDIndexRaster.hpp"
#include "../../LSDChiNetwork.hpp"
#include "../../LSDJunctionNetwork.hpp"
#include "../../LSDIndexChannel.hpp"
#include "../../LSDIndexChannelTree.hpp"
#include "../../LSDChannel.hpp"
#include "../../TNT/tnt.h"
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

  string path, Raster_name, Output_ID, ChannelHeads_name;
  int basin_junction, n_metrics, timesteps;  
  float pruning_threshold,A0,m_ov_n;
  string DEM_extension = "flt";
  string temp;
  vector<string> metric_filenames;
  file_info_in >> temp >> path
               >> temp >> Raster_name
               >> temp >> ChannelHeads_name
               >> temp >> Output_ID
               >> temp >> basin_junction
               >> temp >> pruning_threshold
               >> temp >> A0
               >> temp >> m_ov_n
               >> temp >> timesteps
               >> temp >> n_metrics >> temp;
  for (int i = 0; i<n_metrics; ++i)
  {
    file_info_in >> temp;
    metric_filenames.push_back(temp);
  }
  file_info_in.close();
  // Now create the raster selection vector based on user's selection
  // Elevation
  cout << "\t Load DEM" << endl;
  LSDRaster raster(path+Raster_name, DEM_extension);
  int NRows = raster.get_NRows();
  int NCols = raster.get_NCols();
  float NoDataValue = raster.get_NoDataValue();
  float XMin = raster.get_XMinimum();
  float YMin = raster.get_YMinimum();
  float DataRes = raster.get_DataResolution();
  // filter - for zero filtering, use 0 timesteps. simples!
  cout << "\t Perona Malik Filter" << endl;
  string pm_ext = "_pmfilt";
  string hs_ext = "_hs";
  float slope_percentile = 90;
  float dt = 0.1;
  raster = raster.PeronaMalikFilter(timesteps,slope_percentile,dt);
  raster.write_raster((path+Raster_name+pm_ext),DEM_extension);
  LSDRaster HS = raster.hillshade();    
  HS.write_raster((path+Raster_name+pm_ext+hs_ext),DEM_extension);
  // Fill raster 
  cout << "\t Fill DEM" << endl;
  float min_slope = 0.0001;
  raster = raster.fill(min_slope);
  
  // D-Inf flow area calculation
  cout << "\t Calculate D-Inf Flow Directions" << endl;
  Array2D<float> D_inf_temp = raster.D_inf_FlowDir();
  LSDRaster D_inf_dir = raster.write_dinf_flowdir_to_LSDRaster(D_inf_temp);
  
  // Create FlowInfo object 
  cout << "\t Calculate FlowInfo" << endl;
  vector<string> boundary_conditions(4);
  boundary_conditions[0] = "No";
  boundary_conditions[1] = "no flux";
  boundary_conditions[2] = "no flux";
  boundary_conditions[3] = "No flux";
  LSDFlowInfo FlowInfo(boundary_conditions,raster);
  LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();
  
  // Create channel network from sources - print some rasters for checking
  cout << "\t Calculating Junction Network" << endl;
  vector<int> sources = FlowInfo.Ingest_Channel_Heads((path+ChannelHeads_name), "csv",2);
  LSDJunctionNetwork JunctionNetwork(sources, FlowInfo);
  string SO_ext = "_SO";
  string JI_ext = "_JI";
  LSDIndexRaster SOArray = JunctionNetwork.StreamOrderArray_to_LSDIndexRaster();
  LSDIndexRaster JIArray = JunctionNetwork.JunctionIndexArray_to_LSDIndexRaster();
  SOArray.write_raster((path+Raster_name+SO_ext),DEM_extension);
  JIArray.write_raster((path+Raster_name+JI_ext),DEM_extension);
  
  // Now create a LSDChannelTree object
  int organization_switch = 1;                                                  // Decide how I want to thin the dataset
  int pruning_switch = 0;
  LSDIndexChannelTree ChannelTree(FlowInfo, JunctionNetwork, basin_junction, organization_switch, DistanceFromOutlet, pruning_switch, pruning_threshold);
  // print a file that can be ingested bt the chi fitting algorithm
  string Chan_fname = "_ChanNet";
  string Chan_ext = ".chan";
  string path_ext = "SensitivityAnalysis/";
  string jn_name = "_" + itoa(basin_junction);
  string Chan_for_chi_ingestion_fname = path+Raster_name+Chan_fname+jn_name+Chan_ext;
  ChannelTree.print_LSDChannels_for_chi_network_ingestion(FlowInfo, raster, DistanceFromOutlet, Chan_for_chi_ingestion_fname);
  ChannelTree.convert_chan_file_for_ArcMap_ingestion(Chan_for_chi_ingestion_fname);
// Create chi network automatically here, rather than reading in a file
  // create the chi network 
  cout << "\t Calculate Chi" << endl;
  LSDChiNetwork ChiNetwork(Chan_for_chi_ingestion_fname);
  ChiNetwork.calculate_chi(A0, m_ov_n);
  
  // Now do hillslope flow routing
  cout << "\t Hilltop flow routing" << endl;
  vector<int> baselevel_channel_nodes,hillslope_nodes;
  JunctionNetwork.couple_hillslope_nodes_to_channel_nodes(raster, FlowInfo, D_inf_dir, SOArray, basin_junction, hillslope_nodes, baselevel_channel_nodes);
  Array2D<int> test(NRows,NCols,NoDataValue);
  int N_hillslope_nodes = hillslope_nodes.size();
  for(int i = 0; i<N_hillslope_nodes; ++i)
  {
    int row,col;
    FlowInfo.retrieve_current_row_and_col(hillslope_nodes[i],row,col);
    test[row][col] = baselevel_channel_nodes[i];
  } 
  LSDIndexRaster testraster(NRows,NCols,XMin,YMin,DataRes,NoDataValue,test);
  testraster.write_raster(path+Raster_name+jn_name+"_bl_channel","flt");
  // Loop through Chi Network, extracting relevant channel information
  cout << "\t Extracting Metrics" << endl;
  int N = hillslope_nodes.size();
  vector<vector<int> > ChannelTree_nodes = ChiNetwork.get_node_indices();
  vector<vector<float> > ChannelTree_chis = ChiNetwork.get_chis();
  int N_tributaries = ChannelTree_nodes.size();
  Array2D<float> chi_coordinates(NRows,NCols,NoDataValue);
  Array2D<float> channel_number(NRows,NCols,NoDataValue);
  cout << N_tributaries << " tribs" << endl;
  for(int i = 0; i < N_tributaries; ++i)
  {
    int N_nodes_in_trib = ChannelTree_nodes[i].size();
    cout << N_nodes_in_trib << " nodes in tributary " << i << endl;
    int row,col;
    for(int j = 0; j < N_nodes_in_trib; ++j)
    {
      FlowInfo.retrieve_current_row_and_col(ChannelTree_nodes[i][j],row,col);
      chi_coordinates[row][col] = ChannelTree_chis[i][j];
      channel_number[row][col] = float(i);
    }
  }
  LSDRaster chi(NRows,NCols,XMin,YMin,DataRes,NoDataValue,chi_coordinates);
  LSDRaster channel_ID(NRows,NCols,XMin,YMin,DataRes,NoDataValue,channel_number);
  // Attach relevant base level channel info to basin nodes
  cout << "\t\t\t channel number" << endl;
  vector<float> baselevel_channel_number = FlowInfo.get_raster_values_for_nodes(channel_ID, baselevel_channel_nodes); 
  cout << "\t\t\t channel dist" << endl;
  vector<float> baselevel_channel_dist = FlowInfo.get_raster_values_for_nodes(DistanceFromOutlet, baselevel_channel_nodes); 
  cout << "\t\t\t channel chi" << endl;
  vector<float> baselevel_channel_chi = FlowInfo.get_raster_values_for_nodes(chi, baselevel_channel_nodes);
  
  // Now loop through metric rasters, retrieving values for corresponding basin nodes.
  vector< vector<float> > all_metrics_for_basin;
  for(int i =0; i<n_metrics; ++i)
  {
    LSDRaster MetricRaster(path+metric_filenames[i],DEM_extension);
    vector<float> metrics_for_basin = FlowInfo.get_raster_values_for_nodes(MetricRaster, hillslope_nodes);
    all_metrics_for_basin.push_back(metrics_for_basin);  
  }
  
  // Write ouput file
  cout << "\t Writing Output File" << endl;
  string output_filename = path + Output_ID + "_hillslope_channel_coupling.txt";
  ofstream ofs;
  ofs.open(output_filename.c_str());
  
  if(ofs.fail())
  {
    cout << "\nFATAL ERROR: unable to write output_file" << endl;
    exit(EXIT_FAILURE);
  }
  ofs << "BasinNode ChannelNode ChannelNumber ChiCoordinate Slope S3 Biomass\n";
  for(int i = 0; i<N;++i)
  {
    ofs << hillslope_nodes[i] << " " << baselevel_channel_nodes[i] << " " << baselevel_channel_number[i] << " " << baselevel_channel_dist[i] << " " << baselevel_channel_chi[i];
    for(int j=0; j<n_metrics; ++j) ofs << " " << all_metrics_for_basin[j][i];
    ofs << "\n";
  }
  ofs.close();
  cout << "\t DONE" << endl;
}
