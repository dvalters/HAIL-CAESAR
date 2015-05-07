//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// chi_m_over_n_analysis.cpp
//
// This program is used to extract statistics about the m/n
// ratio for a channel network
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Copyright (C) 2013 Simon M. Mudd 2013
//
// Developed by Simon Mudd
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
#include "../LSDStatsTools.hpp"
#include "../LSDChiNetwork.hpp"

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

	// create the chi network
	string Chan_fname = "_ChanNet";
	string Chan_ext = ".chan";
	string Chan_for_chi_ingestion_fname = path_name+DEM_name+Chan_fname+jn_name+Chan_ext;

	cout << "Channel file name is: " << Chan_for_chi_ingestion_fname << endl;

	LSDChiNetwork ChiNetwork(Chan_for_chi_ingestion_fname);
	LSDChiNetwork ChiNetwork_extended(Chan_for_chi_ingestion_fname);
	ChiNetwork_extended.extend_tributaries_to_outlet();

	// get the best fit m over n ratio for this basin
	float bf_cum_movn_ms, bf_colinear_movn_breaks;
	string bfchi_fname = "_BFmovern";
	string fpt_ext = ".movern";

	// get a string with some paramter values
  	string sigma_str;
  	string skip_str;
  	string msl_str;
  	string tn_str;
  	string param_str;
	sigma_str = static_cast<ostringstream*>( &(ostringstream() << sigma) )->str();
	skip_str = static_cast<ostringstream*>( &(ostringstream() << target_skip) )->str();
	msl_str = static_cast<ostringstream*>( &(ostringstream() << minimum_segment_length) )->str();
	tn_str = static_cast<ostringstream*>( &(ostringstream() << target_nodes) )->str();

	param_str = uscore+sigma_str+uscore+skip_str+uscore+msl_str+uscore+tn_str;


	vector<float> m_over_n_values;
	vector<float> AICc_mean_breaks;
	vector<float> AICc_stdd_breaks;
	vector< vector<float> > AICc_vals;
	vector< vector<float> > AICc_stddev;

	// get the best fit m over n in two different ways
	int Monte_Carlo_switch = 1;
	bf_colinear_movn_breaks = ChiNetwork.search_for_best_fit_m_over_n_colinearity_test_with_breaks(A_0,
						 n_movern, d_movern,start_movern,
						 minimum_segment_length, sigma, target_skip, target_nodes, n_iterations,
						 m_over_n_values, AICc_mean_breaks, AICc_stdd_breaks,
						 Monte_Carlo_switch);

	bf_cum_movn_ms = ChiNetwork_extended.search_for_best_fit_m_over_n_individual_channels_with_breaks_monte_carlo(A_0,
	                     n_movern, d_movern,start_movern,
						 minimum_segment_length, sigma, target_skip, target_nodes, n_iterations,
						 m_over_n_values, AICc_vals, AICc_stddev);

	// now print the results from the m_over_n analysis
	ofstream mn_analysis_out;
	string mn_fo_fname = (path_name+DEM_name+bfchi_fname+param_str+jn_name+fpt_ext);
	mn_analysis_out.open(mn_fo_fname.c_str());
	mn_analysis_out << "-99 ";
	for (int i = 0; i< n_movern; i++)
	{
		mn_analysis_out << m_over_n_values[i] << " ";
	}
	mn_analysis_out << endl << "-99 ";
	for (int i = 0; i< n_movern; i++)
	{
		mn_analysis_out << AICc_mean_breaks[i] << " ";
	}
	mn_analysis_out << endl << "-99 ";
	for (int i = 0; i< n_movern; i++)
	{
		mn_analysis_out << AICc_stdd_breaks[i] << " ";
	}
	mn_analysis_out << endl;


	for(int chan = 0; chan<  ChiNetwork.get_n_channels(); chan++)
	{
		mn_analysis_out << chan << " ";
		for (int i = 0; i< n_movern; i++)
		{
			mn_analysis_out << AICc_vals[chan][i] << " ";
		}
		mn_analysis_out << endl;
		mn_analysis_out << chan << " ";
		for (int i = 0; i< n_movern; i++)
		{
			mn_analysis_out << AICc_stddev[chan][i] << " ";
		}
		mn_analysis_out << endl;
	}
	mn_analysis_out.close();
	cout << "best fit m over n collinear: " << bf_colinear_movn_breaks
       << " and mainstem: " << bf_cum_movn_ms << endl;


}
