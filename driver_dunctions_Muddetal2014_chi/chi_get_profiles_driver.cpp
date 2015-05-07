//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// chi_get_profiles
//
// This program takes two arguments, the path name and the driver name
// It then reads the .chan file with the prefix listed in the first row of
// the driver file and creates a number of chi profiles that are forced
// with the m/n ratios determied by parameters in teh driver file on rows
// 9-11.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
// Simon M. Mudd
// Declan Valters     (implemented m/n loop)
// University of Edinburgh
//
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
		 << "DEM name: " << DEM_name << endl
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

	// test if infilef works:
	ifstream file_info_in2;
	file_info_in2.open(Chan_for_chi_ingestion_fname.c_str());
	if( file_info_in2.fail() )
	{
		cout << "\nFATAL ERROR: the file \"" << Chan_for_chi_ingestion_fname
		     << "\" doesn't exist" << endl;
		exit(EXIT_FAILURE);
	}
	file_info_in2.close();

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

	// create the chi network
	LSDChiNetwork ChiNetwork(Chan_for_chi_ingestion_fname);
	LSDChiNetwork ChiNetwork_extended(Chan_for_chi_ingestion_fname);
	ChiNetwork_extended.extend_tributaries_to_outlet();

	//=-=-=-=-=-=-=-=-=-=-=-=-
	// now caluculate the slope area data
	//=-=-=-=-=-=-=-=-=-=-=-=-
	string SA_vert_fname = "SAVert";
	string SA_ext = ".SAdata";
	ChiNetwork.slope_area_extraction_vertical_intervals(vertical_interval, area_thin_frac,
	                                              (path_name+DEM_name+SA_vert_fname+jn_name+SA_ext));

	string SA_horiz_fname = "SAHoriz";
	ChiNetwork.slope_area_extraction_horizontal_intervals(horizontal_interval, area_thin_frac,
	                                              (path_name+DEM_name+SA_horiz_fname+jn_name+SA_ext));

	//=-=-=-=-=-=-=-=-=
	///////////////////
	// Now do the segment finding
	///////////////////
	//=-=-=-=-=-=-=-=-=
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// loop through MULTIPLE M/N vlaues
  // produces separate profile files for each value of m/n
    for(int i = 0; i<n_movern; i++)
	  {
		  float this_movern = start_movern + float(i)*d_movern;

		  cout << "This m/n is: " << this_movern << endl;

		  string fpt_ext = ".tree";

		  //label each profile file with the value of movn used
		  //string prefix_movn = std::to_string(this_movern);

		  // convert the m/n ratio to a string for the output filename
		  string prefix_movn = static_cast<ostringstream*>( &(ostringstream() << this_movern) )->str();

  		// get the breaks of all the channels
	   	ChiNetwork_extended.split_all_channels(A_0, this_movern, n_iterations,
		  					target_skip, target_nodes, minimum_segment_length, sigma);

		  // monte carlo sample all channels
		  ChiNetwork_extended.monte_carlo_sample_river_network_for_best_fit_after_breaks(A_0, this_movern, n_iterations,
								target_skip, minimum_segment_length, sigma);

		  string fpt_mc = "_fullProfileMC_forced_" + prefix_movn+param_str;

		  ChiNetwork_extended.print_channel_details_to_file_full_fitted((path_name+DEM_name+fpt_mc+jn_name+fpt_ext));
		  cout << "Printing the scv file" << endl;
      ChiNetwork_extended.print_channel_details_to_file_full_fitted_for_ArcMap((path_name+DEM_name+fpt_mc+jn_name+fpt_ext));

    }

 }
