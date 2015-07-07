//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// get_topographic_metrics.cpp
// A driver function for use with the Land Surace Dynamics Topo Toolbox
// This program calculates channel heads using a chi method described in
// Clubb et al. (manuscript in prep)
//
// Developed by:
//  Simon M. Mudd
//
// Copyright (C) 2013 Simon M. Mudd 2013
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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../LSDRaster.hpp"
#include "../LSDStatsTools.hpp"
#include "../LSDFlowInfo.hpp"
#include "../LSDJunctionNetwork.hpp"
#include "../LSDStrahlerLinks.hpp"
#include "../LSDBasin.hpp"
#include "../LSDParticle.hpp"
#include "../LSDCRNParameters.hpp"
#include "../LSDShapeTools.hpp"
#include "../LSDCosmoData.hpp"
using namespace std;



int main (int nNumberofArgs,char *argv[])
{

  // some paramters
  //Test for correct input arguments
  if (nNumberofArgs!=4)
  {
    cout << "FATAL ERROR: wrong number inputs. The program needs the path name and the file name" << endl;
    exit(EXIT_SUCCESS);
  }

  string path_name = argv[1];
  string f_name = argv[2];
  string s_threshold = argv[3];
  
  string path_to_atmospheric_data = "./";

  int threshold = atoi(s_threshold.c_str());

  cout << "The path is: " << path_name << " and the filename is: " << f_name 
       << " and thresh is " << threshold << endl;

  string full_name = path_name+f_name;

  //string DEM_name;
  string fill_ext = "_fill";

  // get some file names
  string DEM_f_name = path_name+f_name;
  string DEM_bil_extension = "bil";
  string DEM_flt_extension = "flt";

  // load the DEM
  LSDRaster topo_test(DEM_f_name, DEM_bil_extension);
  
  float sea_threshold = 1.0;
  // remove the seas
  topo_test.mask_to_nodata_below_threshold(sea_threshold);

  // now trim the raster
  LSDRaster trimmed = topo_test.RasterTrimmerSpiral();
  
  string Trim_name = "_TRIM";
  trimmed.write_raster((DEM_f_name+Trim_name),DEM_bil_extension);
 
  // now get the filled raster
  float min_slope = 0.0001;
  LSDRaster filled_raster = trimmed.fill(min_slope);
  
  // get the flow info 
  vector<string> boundary_conditions(4);
  boundary_conditions[0] = "No";
  boundary_conditions[1] = "n";
  boundary_conditions[2] = "no flux";
  boundary_conditions[3] = "n";
  
  cout << "Filled raster, getting flow info" << endl;
  LSDFlowInfo FlowInfo(boundary_conditions, filled_raster);

  // get contributing pixels (needed for junction network)
  LSDIndexRaster ContributingPixels = FlowInfo.write_NContributingNodes_to_LSDIndexRaster();

  // get the sources
  vector<int> sources;
  sources = FlowInfo.get_sources_index_threshold(ContributingPixels, threshold);

  // now get the junction network
  cout << "Initializing Channel network" << endl;
  LSDJunctionNetwork JNetwork(sources, FlowInfo);
  cout << "got junction_network" << endl;

  // get the number of junctions
  int NJuncs = JNetwork.get_NJunctions();
  cout << "The number of junctions is: " << NJuncs << endl;

  // load the cosmogenic data file
  string cosmo_fname = "cosmo_data.txt";
  cosmo_fname = path_name+cosmo_fname;

  ifstream cosmo_in;
  cosmo_in.open(cosmo_fname.c_str());
  cout << "Cosmo fname is: " << cosmo_fname << endl;

  // variables for ingesting data
  string temp_string;
  float latitude;
  float longitude;
  float x_loc;
  float y_loc;

  // the cosmo data
  vector<float> x_vec;
  vector<float> y_vec;
  vector<double> lat_dd;
  vector<double> long_dd;
  
  //double convert_x
  //double convert_y;

  // get the header
  cosmo_in >> temp_string >> temp_string >> temp_string >> temp_string >> temp_string;
  cout << "temp_string is: " << temp_string << endl;

  LSDCoordinateConverterLLandUTM Converter;
  
  // set to southern hemisphere
  //bool isNorth = false;

  // now get the data
  while (cosmo_in >> temp_string >> latitude >> longitude >> x_loc >> y_loc)
  {
    //cout << "x: " << x_loc << " y: " << y_loc << endl;
    x_vec.push_back(x_loc);
    y_vec.push_back(y_loc);

    //int eId = 22;
    //int Zone = 19;
    //double Lat, Long;
    //Converter.UTMtoLL(eId, double(y_loc), double(x_loc), Zone, isNorth, Lat, Long);
    
    //cout << "lat_data: " << latitude << " Lat converted: " << Lat << endl;
    //cout << "long data: "<< longitude<< " Long converted: " << Long << endl;
    
    //double Northing;
    //double Easting;
    
    //Converter.LLtoUTM(eId, double(latitude ), double(longitude),Northing, Easting, Zone);
    //cout << "xinL " << x_loc << " y_in: " << y_loc << " Easting out: " << Easting << " Northing_out: " << Northing << " Zone out: " << Zone << endl;
    //cout << "Northing min tenmil: " << 10000000-Northing << endl;




  }
  cosmo_in.close();

  vector<int> valid_cosmo_points;
  vector<int> snapped_node_indices;
  vector<int> snapped_junction_indices;
  
  int search_radius_nodes = 1;
  int threshold_stream_order = 1;

  // get the stream orders and the junctions
  //LSDIndexRaster SOArray = JNetwork.StreamOrderArray_to_LSDIndexRaster();
  //LSDIndexRaster JIArray = JNetwork.JunctionIndexArray_to_LSDIndexRaster();  
  //LSDIndexRaster NIArray = FlowInfo.write_NodeIndex_to_LSDIndexRaster();

  //string SO_name = "_SO";
  //string JI_name = "_JI";
  //string NI_name = "_NI";
  //SOArray.write_raster((DEM_f_name+SO_name),DEM_bil_extension);
  //JIArray.write_raster((DEM_f_name+JI_name),DEM_bil_extension);
  //NIArray.write_raster((DEM_f_name+NI_name),DEM_bil_extension); 

  JNetwork.snap_point_locations_to_channels(x_vec, y_vec, 
            search_radius_nodes, threshold_stream_order, FlowInfo, 
            valid_cosmo_points, snapped_node_indices, snapped_junction_indices);
  
  
  vector<LSDBasin> BasinVec;
  
  double test_N10 = 100000;
  double test_dN10 = 5000;
  double test_N26 = 100000;
  double test_dN26 = 5000;  
            
  int n_valid_nodes = int(valid_cosmo_points.size());
  cout << "The number of valid points are: " << n_valid_nodes << endl;
  
  string this_csv_prefix = "Node_prefix_";
  string this_samp_number;
 
 
 
/* 
  
  // get the topographic shielding
  // the values of theta and phi step are based on testing by S. Grieve 
  // Note that Codilian reccomends 5,5 but 10,15 leads to minimal errors
  int theta_step = 30;
  int phi_step = 30;
  cout << "LINE 963, LSDBasin, performing shielding" << endl;
  LSDRaster T_shield = filled_raster.TopoShield(theta_step, phi_step);
  cout << "LINE 965, LSDBasin, did shielding" << endl;
  
  // write the shielding raster
  string TS_name = "_TShield";
  T_shield.write_raster((DEM_f_name+TS_name),DEM_bil_extension);
      
  for(int samp = 0; samp<n_valid_nodes; samp++)
  //for(int samp = 0; samp<1; samp++)
  {
    cout << "Valid point is: " << valid_cosmo_points[samp] << " X: " 
         << x_vec[valid_cosmo_points[samp]] << " Y: "
         << y_vec[valid_cosmo_points[samp]] << endl;
    cout << "Node index is: " <<  snapped_node_indices[samp] << " and junction is: " 
         << snapped_junction_indices[samp] << endl;
    LSDCosmoBasin thisBasin(snapped_junction_indices[samp],FlowInfo, JNetwork,
                            test_N10,test_dN10, test_N26,test_dN26);
    
    // populate the scaling vectors
    thisBasin.populate_scaling_vectors(FlowInfo, filled_raster, T_shield,
                                       path_to_atmospheric_data);
    
    // get the atmospheric pressure for bug checking. THis will print to screen
    thisBasin.get_atmospheric_pressure(FlowInfo, filled_raster, path_to_atmospheric_data);
    
    BasinVec.push_back(thisBasin);
    
    // now print the data to file
    this_samp_number = itoa(samp);
    string this_filename = this_csv_prefix+this_samp_number;
    
    thisBasin.print_particle_csv(path_name, this_filename, FlowInfo, filled_raster,
                                 T_shield, path_to_atmospheric_data);
    
    // now get the cosmo erosion rate
    cout << "Line 272, cosmo snapping; getting erate, 10Be conc: " << test_N10 << endl;
    //double erate;
    string String_10Be = "Be10";
    string String_26Al = "Al26";
    double prod_uncert_factor = 1;
    //erate = thisBasin.predict_CRN_erosion(test_N10, String_10Be,prod_uncert_factor);
    
    string Muon_scaling = "Braucher";
    //bool data_from_outlet_only = true;
    double rho = 2650;
    //double erosion_rate_in_cm_kyr = 10;
    //if (samp == 0)
    //{
    //  erosion_rate_in_cm_kyr = 100;
    //}
    //else if (samp == 1)
    //{
    //  erosion_rate_in_cm_kyr = 3;
    //}    
    
    // Some routines for testing the forward model
    //double erosion_rate = erosion_rate_in_cm_kyr*1.0e-5;    // in m/yr
    //double eff_erosion_rate = rho*0.1*erosion_rate;
    //double predicted_N;
    //cout << endl << endl << "LINE 298 Predicting N" << endl;
    //predicted_N = thisBasin.predict_mean_CRN_conc(eff_erosion_rate, String_10Be,prod_uncert_factor, 
    //                      Muon_scaling, data_from_outlet_only);
    //cout << endl << endl << "Line 301 Testing predicted N, erosion rate: " <<  erosion_rate_in_cm_kyr 
    //     << " and N: " << predicted_N  << endl;
                          
    
    // now test the Newton-Raphson routine
    //double effective_erate;
    //double production_uncertainty;

         
    cout << "=======================================================" << endl;
    cout << "And now for the full analysis" << endl;
    vector<double> erate_analysis = thisBasin.full_CRN_erosion_analysis(test_N10, 
                                        String_10Be, test_dN10, prod_uncert_factor,
                                        Muon_scaling);
    cout << "LINE 323, cosmo_snapping.cpp, the effective erate is: " 
         <<  erate_analysis[0] << endl << "The erosion rate for rho = 2650 is: " 
         << erate_analysis[0]*10/rho << "m/yr and " 
         << erate_analysis[0]*1e6/rho << "cm/kyr" << endl;
    cout << "LINE 323, cosmo_snapping.cpp, the error is: " 
         <<  erate_analysis[1] << endl << "The erosion rate error for rho = 2650 is: " 
         << erate_analysis[1]*10/rho << "m/yr and " 
         << erate_analysis[1]*1e6/rho << "cm/kyr" << endl;
    cout << "============================================================" << endl;
    cout << endl << endl << endl;     
    
  }

*/

  double snow_eff_depth = 0;
  double self_eff_depth = 0;
  
  int theta_step = 30;
  int phi_step = 30;
  cout << "LINE 963, LSDBasin, performing shielding" << endl;
  LSDRaster T_shield = filled_raster.TopographicShielding(theta_step, phi_step);

  // test the self sheilding and snow shielding
  for(int samp = 0; samp<n_valid_nodes; samp++)
  //for(int samp = 0; samp<1; samp++)
  {
    cout << "Valid point is: " << valid_cosmo_points[samp] << " X: " 
         << x_vec[valid_cosmo_points[samp]] << " Y: "
         << y_vec[valid_cosmo_points[samp]] << endl;
    cout << "Node index is: " <<  snapped_node_indices[samp] << " and junction is: " 
         << snapped_junction_indices[samp] << endl;
    LSDCosmoBasin thisBasin(snapped_junction_indices[samp],FlowInfo, JNetwork,
                            test_N10,test_dN10, test_N26,test_dN26);
    
    // populate the scaling vectors
    thisBasin.populate_scaling_vectors(FlowInfo, filled_raster, T_shield,
                                       path_to_atmospheric_data);
                                       
    // now get the snow and self shielding
    thisBasin.populate_snow_and_self_eff_depth_vectors(snow_eff_depth, self_eff_depth);
    
    // some variables
   string String_10Be = "Be10";
    string String_26Al = "Al26";

    double prod_uncert_factor = 1;
    //erate = thisBasin.predict_CRN_erosion(test_N10, String_10Be,prod_uncert_factor);
    
    double production_uncertainty; 
    double average_production;
    bool is_production_uncertainty_plus_on = false;
    bool is_production_uncertainty_minus_on = false;
    bool data_from_outlet_only = true;
    
    string Muon_scaling = "Braucher";
    //bool data_from_outlet_only = true;
    // test the shielding calculations
    double eff_erosion_rate = 0.000001;
    cout << "LINE 380 main Predicting concentration"  << endl;
    thisBasin.predict_mean_CRN_conc_with_snow_and_self(eff_erosion_rate, 
                                            String_10Be,
                                            prod_uncert_factor, Muon_scaling,
                                            data_from_outlet_only, production_uncertainty, 
                                            average_production,
                                            is_production_uncertainty_plus_on,
                                            is_production_uncertainty_minus_on);
    cout << "Predicted concentration"  << endl;

    BasinVec.push_back(thisBasin);
    
    // now print the data to file
    this_samp_number = itoa(samp);
    string this_filename = this_csv_prefix+this_samp_number;
    
    //thisBasin.print_particle_csv(path_name, this_filename, FlowInfo, filled_raster,
    //                             T_shield, path_to_atmospheric_data);
/*    
    // now get the cosmo erosion rate
    cout << "Line 272, cosmo snapping; getting erate, 10Be conc: " << test_N10 << endl;
    //double erate;


    double rho = 2650;

         
    cout << "=======================================================" << endl;
    cout << "And now for the full analysis" << endl;
    vector<double> erate_analysis = thisBasin.full_CRN_erosion_analysis(test_N10, 
                                        String_10Be, test_dN10, prod_uncert_factor,
                                        Muon_scaling);
    cout << "LINE 323, cosmo_snapping.cpp, the effective erate is: " 
         <<  erate_analysis[0] << endl << "The erosion rate for rho = 2650 is: " 
         << erate_analysis[0]*10/rho << "m/yr and " 
         << erate_analysis[0]*1e6/rho << "cm/kyr" << endl;
    cout << "LINE 323, cosmo_snapping.cpp, the error is: " 
         <<  erate_analysis[1] << endl << "The erosion rate error for rho = 2650 is: " 
         << erate_analysis[1]*10/rho << "m/yr and " 
         << erate_analysis[1]*1e6/rho << "cm/kyr" << endl;
    cout << "============================================================" << endl;
    cout << endl << endl << endl;     
*/    
  }


/*

  // Test the LSDCosmoData object
  string detailed_cosmo_file = "Test_basin_data.csv";
  string cosmo_file = path_name+detailed_cosmo_file;
  string filetype = "csv";
  
  cout << "File is: " << cosmo_file << endl;
  
  LSDCosmoData CosmoData(cosmo_file,filetype);
  CosmoData.print_data_to_screen();
  
  CosmoData.convert_to_UTM(filled_raster);

  cout << endl << endl << endl << endl << endl << "------" << endl
       << "Now using LSDCosmoData to run analysis" << endl;

  CosmoData.basic_cosmogenic_analysis(search_radius_nodes, 
                            threshold_stream_order, filled_raster,
                            T_shield,
                            FlowInfo, JNetwork);
*/






  
  // now test some functions in the particle
  //int startType = 0; 
  //int samp = 0;
  //double Xloc = double( x_vec[valid_cosmo_points[samp]] );
  //double Yloc = double( y_vec[valid_cosmo_points[samp]] );
  //double  startdLoc = 0.0;
  //double  start_effdloc = 0.0;
  //double startzLoc = 100;
  
  //LSDCRNParticle test_particle(startType, Xloc, Yloc,
  //                             startdLoc, start_effdloc, startzLoc);
  //LSDCRNParameters LSDCRNP;
  
  //double test_scaling;
  //vector<bool> scalings(4,true);
  
  //for (int i = 0; i<5; i++)
  //{
    //test_scaling = 0.4+ double(i)*0.3;
    //cout << endl << endl << endl << "Scaling is: " << test_scaling << endl;
    //LSDCRNP.set_Schaller_parameters();
    //LSDCRNP.scale_F_values(test_scaling);
    //LSDCRNP.set_Schaller_parameters();
    //LSDCRNP.scale_F_values(test_scaling,scalings);
  //}
  
  //string path_to_files = "/exports/csce/datastore/geos/users/smudd/devel_projects/Cosmo_scripts/";
  //string path_to_files = "c:/code/CRONUS_calculators/";
    
  //LSDCRNP.load_parameters_for_atmospheric_scaling(path_to_files);
  
  /*
  double elev;
  do
  {
    cout << "enter the latitude: " << endl;
    cin >> Xloc;
    cout << "enter the longitude: " << endl;
    cin >> Yloc;
    cout << "enter the elevation: " << endl;
    cin >>  elev;
    
    LSDCRNP.NCEPatm_2(Xloc, Yloc, elev);

  } while(Xloc > -100);
  */
  /*
  //double lat;
  //double P;
  double h; 
  //double z;
  //double Rc;
  //double H;
  //double tolerance = 1e-4;
  //double Fsp = 0.978;
  double rho = 1890;
  double lat;
  double d;
  double snow_scale = 0.9;
  double topo_scale = 0.8;
  double N_10Be = 150000;
  double N_26Al = 60000;
  //double e_10, e_26;
  lat = 30;
  double sample_del10, sample_del26;
  
  do
  {
    cout << "enter the h (atmospheric pressure): " << endl;
    cin >> h;
    cout << "enter the soil thickness: " << endl;
    cin >> d;
    cout << "enter the latitude: " << endl;
    cin >> lat;
    cout << "enter the N10Be: " << endl;
    cin >> N_10Be;
    cout << "enter N10Be uncertainty:" << endl;
    cin  >> sample_del10;
    cout << "enter the N_26Al: " << endl;
    cin >> N_26Al;
    cout << "enter the N26 uncertainty: " << endl;
    cin >> sample_del26;
    
      
    // test the CRONUS calculator
    test_particle.calculate_effective_depth_one_layer(d, rho);
    
    // get the CRONUS erosion results
    vector<double> erosion_results = 
      test_particle.CRONUS_get_Al_Be_erosion(LSDCRNP, h,lat, rho, N_10Be, N_26Al, 
                      sample_del10, sample_del26, topo_scale, snow_scale);
    test_particle.CRONUS_screen_report(erosion_results);      
    
    // now test the analytical solution
    // first get the stone scaling factor
    double Fsp = 1;
    double scaling  =  test_particle.stone2000sp(lat,h, Fsp);
    LSDCRNP.set_scaling(scaling, topo_scale, snow_scale);
    
    // now set to stone parameters
    LSDCRNP.set_CRONUS_stone_parameters();
    
    test_particle.update_10Be_SSfull(erosion_results[0],LSDCRNP);
    test_particle.update_26Al_SSfull(erosion_results[6],LSDCRNP);
                
  

  } while(h > -100);  
  */

  

}


                                                                            