//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// roughness_metrics_driver.cpp
// A driver function for use with the Land Surace Dynamics Topo Toolbox
// This program calculates eigenvalues describing local variability of surface
// normal orientation as described by Milodowski et al. (2015).  See also McKean
// and Roering (2004)
//
// References: Milodowski, D. T., S. M. Mudd, E. T. A. Mitchard (2015), 
// Topographic roughness as a signature of the emergence of bedrock in eroding
// landscapes, Earth Surface Dynamics Discussions
//
// McKean, J. and Roering, J. (2004): Objective landslide detection and surface
// morphology mapping using high-resolution airborne laser altimetry,
// Geomorphology, 57(3-4), 331–351, doi:10.1016/S0169-555X(03)00164-8.
//
// Developed by:
//  David T. Milodowski
//  Simon M. Mudd
//
// Copyright (C) 2013 David T. Milodowski and Simon M. Mudd 2013
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
// David T. Milodowski, University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include "../../LSDStatsTools.hpp"
#include "../../LSDRaster.hpp"
#include "../../LSDIndexRaster.hpp"
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

  string DEM_name,PATH;
  string dem_ext = "_dem";
  vector<string> output_extensions;
  output_extensions.push_back("_s1");
  output_extensions.push_back("_s2");
  output_extensions.push_back("_s3");
  string DEM_flt_extension = "flt";
  float value;
  vector<float> surface_fitting_window_radius,normal_variability_radius;
  int n_rad_1, n_rad_2;
  vector<int> raster_selection;  	
  string temp;
  int raster_selector;
  file_info_in >> temp >> PATH
               >> temp >> DEM_name
               >> temp >> n_rad_1;
  file_info_in >> temp;
  for(int i = 0; i < n_rad_1; ++i) 
  {
    file_info_in >> value;
    surface_fitting_window_radius.push_back(value);
  }                         
  file_info_in >> temp >> n_rad_2;
  file_info_in >> temp;
  for(int i = 0; i < n_rad_2; ++i) 
  {
    file_info_in >> value;
    normal_variability_radius.push_back(value);
  }            
  // Now create the raster selection vector based on user's selection
  // s1
  file_info_in >> temp >> raster_selector;
  raster_selection.push_back(raster_selector);
  // s2
  file_info_in >> temp >> raster_selector;
  raster_selection.push_back(raster_selector);
  // s3
  file_info_in >> temp >> raster_selector;
  raster_selection.push_back(raster_selector);
  file_info_in.close();

  LSDRaster dem(PATH+DEM_name+dem_ext, DEM_flt_extension);
  int count = 0;
  for(int i_rad_1 = 0; i_rad_1 < n_rad_1; ++i_rad_1)
  {
    for(int i_rad_2 = 0; i_rad_2 < n_rad_2; ++ i_rad_2)
    {
      cout << "CALCULATING SURFACE ROUGHNESS -> run " << count+1 << "/" << n_rad_1*n_rad_2 << "\n\t lengthscale 1 = " << surface_fitting_window_radius[i_rad_1] << "\n\t lengthscale 2 = " << normal_variability_radius[i_rad_2] << endl;
      ++count;
      vector<LSDRaster> output_rasters;
      output_rasters = dem.calculate_polyfit_roughness_metrics(surface_fitting_window_radius[i_rad_1], normal_variability_radius[i_rad_2], raster_selection);
      for(int i = 0; i<raster_selection.size(); ++i)
      {
        if(raster_selection[i]==1)
        {
          stringstream ss;
          ss << surface_fitting_window_radius[i_rad_1] << ' ' << normal_variability_radius[i_rad_2];
          string lengthscale_1, lengthscale_2; 
          ss >> lengthscale_1 >> lengthscale_2;
          string lengthscale_ext = "_" + lengthscale_1 + "_" + lengthscale_2;
          string output_file = PATH+DEM_name+output_extensions[i]+lengthscale_ext;
          output_rasters[i].write_raster(output_file,DEM_flt_extension);
        }
      }
    }
  }
}
