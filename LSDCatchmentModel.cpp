/// LSDCatchmentModel.cpp
///
/// This is basically the C++ implementation of the CAESAR-Lisflood (T. Coulthard) model. 
/// 
/// It is designed to be integrated with the LSD Topo Tools package, but it should
/// also be able to run as a standalone program with a driver file and input data.
///
/// WHY CAESAR-Lisflood?
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// The key features of the CAESAR-Lisflood (CL) model are the simplified physical-based
/// treatment of water flow across the landscape (Lisflood). The hydrological aspect of the
/// model is essentially nonsteady state. (Unlike most other landscape evolution models such
/// as CHILD, the old version of CAESAR etc..) The timestep is much shorter than a typical 
/// landscape evolution model, so it is more suited to shorter time scales from individual 
/// storms, to years, decades, or Holocene landscape evolution. Though in theory there is no
/// upper limit to the model run length, it will just take a helluva long time. It is designed for
/// individual catchment simulation (though the catchements could in theory be quite large, but
/// there are caveats to using large catchments or high resolution DEMs)
///
/// Why not use the original, C#, Windows version, with the neat GUI?
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// You can do, if you are happy to run on Windows, and would prefer to use a model in GUI-mode
/// then CAESAR-Lisflood is a good choice for this. This branch of the model is designed to 
/// be cross platform, though with an emphasis on running on a linux environment, and hopefully
/// on a supercomputer if the resources are available. One of the drawbacks to the original model
/// is that you need a dedicated Windows box to do your simulations, you can't send them off to
/// a cluster computer type facility. 
///
/// With this version you can also perform topographic analysis within the same
/// LSDTopoTools environment, switching eaasily between modelling and landscape analysis.
///
/// How does LSDCatchmentModel fit in with LSDRasterModel?
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// It should compliment the LSDRasterModel, which is better suited to large-scale, 
/// topographic evolution over long geological time periods, and has a much faster solver based
/// on Jean Braun's FastScape algorithm.
///
/// References:
/// =-=-=-=-=-=-=
/// Coulthard, T. J., Neal, J. C., Bates, P. D., Ramirez, J., Almeida, G. A., & Hancock, G. R. (2013).
/// Integrating the LISFLOOD‐FP 2D hydrodynamic model with the CAESAR model: implications for modelling 
/// landscape evolution. Earth Surface Processes and Landforms, 38(15), 1897-1906.
///
/// Bates, P. D., Horritt, M. S., & Fewtrell, T. J. (2010). A simple inertial formulation of the 
/// shallow water equations for efficient two-dimensional flood inundation modelling. 
/// Journal of Hydrology, 387(1), 33-45.
/// 
///
/// Declan Valters, 2014
/// Version 0.0.1 alpha 
/// January 2015
///

#include "LSDCatchmentModel.hpp"

#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>

using std::string;

// ingest data tools
// DAV: I've copied these here for now to make the model self-contained for testing purposes
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Line parser for parameter files - JAJ 08/01/2014
// This might be better off somewhere else
//
// To be used on a parameter file of the format:
// 	Name: 100		comments etc.
// Which sets parameter as "Name" and value as "100"
//
// This just does one line at a time; you need a wrapper function to get all
// the information out of the file
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void parse_line(std::ifstream &infile, string &parameter, string &value)
{
	char c;
	char buff[128];
	int pos = 0;
	int word = 0;

	while ( infile.get(c) )	
	{
		if (pos >= 128)
		{
			std::cout << "Buffer overrun, word too long in parameter line: " << std::endl;
			string line;
			getline(infile, line);
			std::cout << "\t" << buff << " ! \n" << line << std::endl;
			exit(1);
		}
		// preceeding whitespace
		if (c == '#')
		{
			if (word == 0)
			{
				parameter = "NULL";
				value = "NULL";
			}
			if (word == 1)
				value = "NULL";
			word = 2;
		}

		if ((c == ' ' || c == '\t') && pos == 0)
			continue;
		else if ( (c == ':' && word == 0) || ( (c == ' ' || c == '\n' || c == '\t') && word == 1))
		{
			while (buff[pos-1] == ' ' || buff[pos-1] == '\t')
				--pos;		// Trailing whitespace
			buff[pos] = '\0';	// Append Null char
			if (word == 0)
				parameter = buff;	// Assign buffer contents
			else if (word == 1)
				value = buff;
			++word;
			pos = 0;		// Rewind buffer
		}
		else if ( c == '\n' && word == 0 )
		{
			parameter = "NULL";
			buff[pos] = '\0';
			value = buff;
			++word;
		}
		else if (word < 2)
		{
			buff[pos] = c;
			++pos;
		}

		if (c == '\n')
			break;
	}
	if (word == 0)
	{
		parameter = "NULL";
		value = "NULL";
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
// This function removes control characters from the end of a string
// These get introduced if you use the DOS format in your parameter file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
string RemoveControlCharactersFromEndOfString(string toRemove)
{
  int len =  toRemove.length();  
  if(len != 0)  
  {  
    if (iscntrl(toRemove[len-1]))  
    { 
      //cout << "Bloody hell, here is another control character! Why Microsoft? Why?" << endl; 
      toRemove.erase(len-1);  
    }  
  }  
  return toRemove;
}

//=-=-=-=-=-=-=-=-=-=-=
// CREATE FUCNTIONS
// This define what happens when an LSDCatchmentModel object is created
//=-=-=-=-=-=-=-=-=-=-=
// Not got round to a default value model yet - DAV
//void LSDCatchmentModel::create()
//{
	//// Do you want to maybe initialise some default values?
	//initialise_model();
//}

void LSDCatchmentModel::create(string pname, string pfname)
{
	// Using the parameter file
	initialise_model(pname, pfname);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function gets all the data from a parameter file
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::initialise_model(string pname, string pfname)
{
  // the full name of the file
  string full_name = pname+pfname;

  std::ifstream infile;
  infile.open(full_name.c_str());
  string parameter, value, lower, lower_val;
  string bc;

  std::cout << "Parameter filename is: " << full_name << std::endl;

  // now ingest parameters
  while (infile.good())
  {
    parse_line(infile, parameter, value);
    lower = parameter;
    if (parameter == "NULL")
      continue;
    for (unsigned int i=0; i<parameter.length(); ++i)
    {
      lower[i] = std::tolower(parameter[i]);  // converts to lowercase
    }

    std::cout << "parameter is: " << lower << " and value is: " << value << std::endl;

    // get rid of control characters
    value = RemoveControlCharactersFromEndOfString(value);

    if (lower == "dem read extension")
    {
      dem_read_extension = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      dem_read_extension = RemoveControlCharactersFromEndOfString(dem_read_extension);
    }
    else if (lower == "dem write extension")
    {
      dem_write_extension = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      dem_write_extension = RemoveControlCharactersFromEndOfString(dem_write_extension);
    }
    else if (lower == "write path")
    {
      write_path = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      write_path = RemoveControlCharactersFromEndOfString(write_path);
    }
    else if (lower == "write fname")
    {
      write_fname = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      write_fname = RemoveControlCharactersFromEndOfString(write_fname);
      //std::cout << "Got the write name, it is: "  << write_fname << std::endl;
    }
    else if (lower == "read path")
    {
      read_path = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      read_path = RemoveControlCharactersFromEndOfString(read_path);
      //std::cout << "Got the write name, it is: "  << write_fname << std::endl;
    }
    else if (lower == "read fname")
    {
      read_fname = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      read_fname = RemoveControlCharactersFromEndOfString(read_fname);
      //std::cout << "Got the read name, it is: " << read_fname << std::endl;
    }
    //=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // parameters for landscape numerical modelling
    // (LSDCatchmentModel: DAV 2015-01-14)
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    
    // The File Information is read at the start of this method
    // no need to duplicate it here
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Supplementary Input Files
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "hydroindex_file")
    {
      CM_support_file_names["hydroindex_file"] = atof(value.c_str());
    }
    else if (lower == "rainfall_data_file")
    {
      CM_support_file_names["rainfall_data_file"] = atof(value.c_str());
    }
    else if (lower == "grain_data_file")
    {
      CM_support_file_names["grain_data_file"] = atof(value.c_str());
    }
    else if (lower == "bedrock_data_file")
    {
      CM_support_file_names["bedrock_data_file"] = atof(value.c_str());
    }
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Numerical
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "no_of_iterations")
    {
      CM_float_parameters["no_of_iterations"] = atof(value.c_str());
    }
    else if (lower == "min_time_step")
    {
      CM_float_parameters["min_time_step"] = atof(value.c_str());
    }
    else if (lower == "max_time_step")
    {
      CM_float_parameters["max_time_step"] = atof(value.c_str());
    }
    else if (lower == "run_time_start")
    {
      CM_float_parameters["run_time_start"] = atof(value.c_str());
    }
    else if (lower == "max_run_duration")
    {
      CM_float_parameters["max_run_duration"] = atof(value.c_str());
    }
    else if (lower == "memory_limit")
    {
      CM_float_parameters["memory_limit"] = atof(value.c_str());
    }
    else if (lower == "max_time_step")
    {
      CM_float_parameters["max_time_step"] = atof(value.c_str());
    }
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Output and Save Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "save_interval")
    {
      CM_int_parameters["save_interval"] = atof(value.c_str());
    }
    else if (lower == "time_series_interval")
    {
      CM_int_parameters["time_series_interval"] = atof(value.c_str());
    }
    else if (lower == "elevation_file")
    {
      //bool temp_bool = (value == "true") ? true : false;
      CM_model_switches["write_elevation_file"] = atof(value.c_str()); //temp_bool;
    }
    else if (lower == "grainsize_file")
    {
      CM_model_switches["write_grainsize_file"] = atof(value.c_str());
    }
    else if (lower == "parameters_file")
    {
      CM_model_switches["write_parameters_file"] = atof(value.c_str());
    }    
    else if (lower == "flowvelocity_file")
    {
      CM_model_switches["write_flowvelocity_file"] = atof(value.c_str());
    }   
    else if (lower == "waterdepth_file")
    {
      CM_model_switches["write_waterdepth_file"] = atof(value.c_str());
    }   
    else if (lower == "timeseries_file")
    {
      CM_model_switches["write_timeseries_file"] = atof(value.c_str());
    }
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Sediment
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "transport_law")
    {
      CM_method_map["transport_law"] = atof(value.c_str());
    }
    else if (lower == "max_tau_velocity")
    {
      CM_float_parameters["max_tau_velocity"] = atof(value.c_str());
    } 
    else if (lower == "active_layer_thickness")
    {
      CM_float_parameters["active_layer_thickness"] = atof(value.c_str());
    }     
    else if (lower == "recirculate_proportion")
    {
      CM_float_parameters["recirculate_proportion"] = atof(value.c_str());
    }     
    else if (lower == "lateral_erosion_on")
    {
      CM_model_switches["lateral_erosion_on"] = atof(value.c_str());
    }     
    else if (lower == "lateral_ero_rate")
    {
      CM_float_parameters["lateral_ero_rate"] = atof(value.c_str());
    }     
    else if (lower == "edge_filter_passes")
    {
      CM_float_parameters["edge_filter_passes"] = atof(value.c_str());
    }     
    else if (lower == "cells_shift_lat")
    {
      CM_float_parameters["cells_shift_lat"] = atof(value.c_str());
    }     
    else if (lower == "max_diff_cross_chann")
    {
      CM_float_parameters["max_diff_cross_chan"] = atof(value.c_str());
    }    
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Grain Size Options
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "suspended_sediment_on")
    {
      CM_model_switches["suspended_sediment_on"] = atof(value.c_str());
    }     
    else if (lower == "fall_velocity")
    {
      CM_float_parameters["fall_velocity"] = atof(value.c_str());
    }    
    else if (lower == "grain_size_frac_file")
    {
      CM_support_file_names["grain_size_frac_file"] = atof(value.c_str());
    }      
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Hydrology and Flow
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "TOPMODEL_m_value")
    {
      CM_float_parameters["TOPMODEL_m_value"] = atof(value.c_str());
    } 
    else if (lower == "rain_data_time_step")
    {
      CM_int_parameters["rain_data_time_step"] = atof(value.c_str());
    } 
    else if (lower == "spatial_var_rain")
    {
      CM_model_switches["spatial_var_rain"] = atof(value.c_str());
    } 
    else if (lower == "input_output_diff")
    {
      CM_float_parameters["input_output_diff"] = atof(value.c_str());
    }     
    else if (lower == "min_Q_for_depth_calc")
    {
      CM_float_parameters["min_Q_for_depth_calc"] = atof(value.c_str());
    }     
    else if (lower == "max_Q_for_depth_calc")
    {
      CM_float_parameters["max_Q_for_depth_calc"] = atof(value.c_str());
    }     
    else if (lower == "depth_ero_threshold")
    {
      CM_float_parameters["depth_ero_threshold"] = atof(value.c_str());
    } 
    else if (lower == "slope_on_edge_cell")
    {
      CM_float_parameters["slope_on_edge_cell"] = atof(value.c_str());
    } 
    else if (lower == "evaporation_rate")
    {
      CM_float_parameters["evaporation_rate"] = atof(value.c_str());
    } 
    else if (lower == "courant_number")
    {
      CM_float_parameters["courant_number"] = atof(value.c_str());
    } 
    else if (lower == "hflow_threshold")
    {
      CM_float_parameters["hflow_threshold"] = atof(value.c_str());
    } 
    else if (lower == "froude_num_limit")
    {
      CM_float_parameters["froude_num_limit"] = atof(value.c_str());
    } 
    else if (lower == "mannings_n")
    {
      CM_int_parameters["mannings_n"] = atof(value.c_str());
    } 
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Vegetation
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "vegetation_on")
    {
      CM_model_switches["vegetation_on"] = atof(value.c_str());
    }    
    else if (lower == "grass_grow_rate")
    {
      CM_float_parameters["grass_grow_rate"] = atof(value.c_str());
    }   
    else if (lower == "vegetation_crit_shear")
    {
      CM_float_parameters["vegetation_crit_shear"] = atof(value.c_str());
    }   
    else if (lower == "veg_erosion_prop")
    {
      CM_float_parameters["veg_erosion_prop"] = atof(value.c_str());
    } 
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // Hillslope
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "creep_rate")
    {
      CM_float_parameters["creep_rate"] = atof(value.c_str());
    } 
    else if (lower == "slope_failure_thresh")
    {
      CM_float_parameters["slope_failure_thresh"] = atof(value.c_str());
    } 
    else if (lower == "soil_erosion_rate")
    {
      CM_float_parameters["soil_erosion_rate"] = atof(value.c_str());
    } 
    else if (lower == "soil_j_mean_depends_on")
    {
      CM_model_switches["soil_j_mean_depends_on"] = atof(value.c_str());
    } 
    else if (lower == "call_muddpile_model")
    {
      CM_model_switches["call_muddpile_model"] = atof(value.c_str());
    } 
}
}

void LSDCatchmentModel::get_area()
{
	// TO DO
	// gets the drainage area 
	// Could possible come from one of the other LSDRaster objects?
	int x,y;

	for(x=1;x<=xmax;x++)
	{
		for(y=1;y<=ymax;y++)
		{
			area_depth[x][y]=1;
			area[x][y] = 0;
			if (elev[x][y] == -9999)
			{
				area_depth[x][y] = 0.0;
			}
			
		}
	}
	get_area4();   // This refers to a newer area getting method in CL
}

void LSDCatchmentModel::get_area4()
{

	// new routine for determining drainage area 4/10/2010
	// instead of using sweeps this sorts all the elevations then works frmo the
	// highest to lowest - calculating drainage area - D-infinity basically.

	int x, y, n, x2, y2, dir;

	// zero load of arrays
	std::vector<double> tempvalues((xmax+2) *(ymax+2)), tempvalues2((xmax+2) * (ymax+2)), xkey((xmax+2)*(ymax+2)), ykey((xmax+2) * (ymax+2));
	
	// I leave in the old C# syntax for now for reference (note the subtle differences) (DAV)
	//tempvalues = new Double [(xmax+2) *(ymax+2)];
	//tempvalues2 = new Double[(xmax+2) * (ymax+2)];
	//xkey = new Double [(xmax+2)*(ymax+2)];
	//ykey = new Double[(xmax+2) * (ymax+2)];

	// then create temp array based on elevs then also one for x values. 
	int inc = 1;
	for (y = 1; y <= ymax; y++)
	{
		for (x = 1; x <= xmax; x++)    
		{
			tempvalues[inc] = elev[x][y];
			xkey[inc] = x;
			inc++;
		}
	}
	
	// then sorts according to elevations - but also sorts the key (xkey) according to 
	// these too..
	
	// DAV note 09/01/2014 This seems strange at first because the C# syntax is to have 
	// keys first, then values:
	// Array.sort(keys,values) but here it is Array.sort(values, keys) which seems like 
	// a mistake at first. 
	// So in effect this is *sorting the keys* (1,2,3,4,5,...,n) **according to the 
	// elevations**. 
	//Array.Sort(tempvalues, xkey); (Only available in the C# libraries)
	
	// DAV 2014-01-12 
	// Written up a template that should roughly replicate the behaviour
	// Template comparator will function on a std::pair, so you need to compress
	// the two vectors into a std::pair object first. std::sort is then used with the 
	// comparator, (the template): sort_pair_second. As the name indicates, it sorts
	// std::pair objects by the second value in the pair. (You could change this if 
	// needed.
	
	// Pair the vectors up
	std::vector<std::pair<double,double> > x_keys_elevs_paired(xkey.size() < tempvalues.size() ? xkey.size() : tempvalues.size() );
	
	for (unsigned i=0; i < x_keys_elevs_paired.size(); i++)
	{
		x_keys_elevs_paired[i] = std::make_pair(xkey[i],tempvalues[i]);
	}
	
	// Now do the sort
	std::sort(x_keys_elevs_paired.begin(), x_keys_elevs_paired.end(), sort_pair_second<double,double>() );



	// now does the same for y values
	inc = 1;
	for (y = 1; y <= ymax; y++)
	{
		for (x = 1; x <= xmax; x++)
		{
			tempvalues2[inc] = elev[x][y];
			ykey[inc] = y;
			inc++;
		}
	}
	// Pair the vectors up
	std::vector<std::pair<double,double> > y_keys_elevs_paired(ykey.size() < tempvalues2.size() ? ykey.size() : tempvalues2.size() );
	
	for (unsigned i=0; i < y_keys_elevs_paired.size(); i++)
	{
		y_keys_elevs_paired[i] = std::make_pair(ykey[i],tempvalues2[i]);
	}
	
	// Now do the sort
	std::sort(y_keys_elevs_paired.begin(), y_keys_elevs_paired.end(), sort_pair_second<double,double>() );
	
	//Array.Sort(tempvalues2, ykey);  // The old C# version (Why write one-line in C# when you can write 8 in C++?) 


	// then works through the list of x and y co-ordinates from highest to lowest...
	for (n = (xmax * ymax); n >= 1; n--)
	{
		x = (int)(xkey[n]);
		//this.InfoStatusPanel.Text = Convert.ToString(x);
		y = (int)(ykey[n]);
		//this.InfoStatusPanel.Text = Convert.ToString(y);

		if (area_depth[x][y] > 0)
		{
			// update area if area_depth is higher
			if (area_depth[x][y] > area[x][y]) area[x][y] = area_depth[x][y];

			double difftot = 0;

			// work out sum of +ve slopes in all 8 directions
			for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
			{

				x2 = x + deltaX[dir];
				y2 = y + deltaY[dir];
				if (x2 < 1) x2 = 1; if (y2 < 1) y2 = 1; if (x2 > xmax) x2 = xmax; if(y2>ymax) y2=ymax;

				// swap comment lines below for drainage area from D8 or Dinfinity
				if (std::remainder(dir, 2) != 0)
				{
					if (elev[x2][y2] < elev[x][y]) difftot += elev[x][y] - elev[x2][y2];
				}
				else
				{
					if (elev[x2][y2] < elev[x][y]) difftot += (elev[x][y] - elev[x2][y2]) / 1.414;
				}
				//if(elev[x][y]-elev[x2][y2]>difftot) difftot=elev[x][y]-elev[x2][y2];
			}
			if (difftot > 0)
			{
				// then distribute to all 8...
				for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
				{

					x2 = x + deltaX[dir];
					y2 = y + deltaY[dir];
					if (x2 < 1) x2 = 1; if (y2 < 1) y2 = 1; if (x2 > xmax) x2 = xmax; if (y2 > ymax) y2 = ymax;

					// swap comment lines below for drainage area from D8 or Dinfinity
					
					if (std::remainder(dir, 2) != 0)
					{
						if (elev[x2][y2] < elev[x][y]) area_depth[x2][y2] += area_depth[x][y] * ((elev[x][y] - elev[x2][y2]) / difftot);
					}
					else
					{
						if (elev[x2][y2] < elev[x][y]) area_depth[x2][y2] += area_depth[x][y] * (((elev[x][y] - elev[x2][y2])/1.414) / difftot);
					}
					
					//if (elev[x][y] - elev[x2][y2] == difftot) area_depth[x2][y2] += area_depth[x][y];
				}

			}
			// finally zero the area depth...
			area_depth[x][y] = 0;
		}
	}

}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// WRAPPER FUNCTIONS TO CARRY OUT EROSION ETC
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// need a bool option here to check if it is a flow only run
// EDIT: No you don't - If flow only, then just don't call the erosion method! (in driver file) Separate qroute/depth etc
	
// CALC EROSION MULTIPLIER
void LSDCatchmentModel::call_erosion()
{
	if (counter >= erode_call)   // erode_call is by default zero, so don't call erosion before water has been routed.
	{
		erode_mult = static_cast<int>(ERODEFACTOR / erode_mult);
		if (erode_mult < 1) 
		{
			erode_mult = 1;
		}
		if (erode_mult > 5)
		{
			erode_mult = 5;
		}
		erode_call = counter + erode_mult;
	}
}	
	
// Carry out the lateral erosion 
// not sure about the necessity of this call...
void LSDCatchmentModel::call_lateral()
{
	if (counter >= lateralcounter)
	{
		lateral3();   // call the actual erosion function
		lateralcounter = counter + (50 * erode_mult);
	}
}
	
	
// CARRY OUT SOIL CREEP, GRASS-GROWING ETC
//
	
// BOOL option for flow only...
	
// CARRY OUT LOCAL LANDSLIDES EVERY X ITERATIONS...
void LSDCatchmentModel::call_landsliding()
{
	if (std::remainder(counter, 10) == 0)
	{
		slide_3();   // slide_3 in CL is the implemented land sliding function
	}
}
	
// DO THE SOIL CREEP SHUFFLE!
void LSDCatchmentModel::call_soilcreep()
{
	if (cycle > creep_time)
	{
		// update the counter creep_time
		creep_time += 14400;	// 10 days if minutes (or four hours if seconds!)
		// call the creep function
		creep(0.028);
	}
}
	
// There is a sand dunes function but I am not implementing that here - DAV
	
// NOW CALL THE SOIL EROSION FUNCTION
void LSDCatchmentModel::call_soil_erosion()
{
	if (SOIL_RATE > 0 && cycle > soil_erosion_time)
	{
		get_area();    // gets the drainage area before doing the maths below. Useful to have fresh D.A.
		soil_erosion_time += 1440;  // do soil erosion daily
		soil_erosion(0.0028);
	}
}
	
// DEVELOP THE SOIL
void LSDCatchmentModel::call_soil_devel()
{
	if (cycle > soil_development_time)
	{
		soil_development_time += 1440 * 365/12;
		if (soildevoption == true) 
			soil_development();
	}
}
	
// MORE SLOPE PROCESSES
void LSDCatchmentModel::call_evapotrans()
{
	if (cycle > creep_time2)
		evaporate(1440);
		// update the creep_time2 
		creep_time2 += 1440; // daily (1440 minutes in a day)
}		
void LSDCatchmentModel::call_slide5()   // not exactly sure what slide_5 does differently to slide_3: not commented well in the original code!
{
		// update the creep_time2 
		creep_time3 += 1440; // daily (1440 minutes in a day)  // NEED A SEPARATE COUTNER FOR THIS CREEP!
		// BOOL for flow only option...
		slide_5();
}
		
// GROW SOME BAD-ASS GRASS
//void LSDCatchmentModel::call_grass_growing()
//{
		//if (grow_grass_time > 0) 
			//grow_grass(1 / (grow_grass_time * 365));
//}	

//=-=-=-=-=-=-=-=-=-=
// DATA OUTPUTS ETC.
//=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::output_data()	  // this was part of erodep() in CL but I felt it should have its own method call - DAV
{
}	

void LSDCatchmentModel::save_data_and_draw_graphics()
{
}



// A wrapper method that calls the chief erosional and water routing methods.
void LSDCatchmentModel::run_components()   // originally erodepo() in CL
{
	
	do 
	{
	double input_output_difference = std::abs(waterinput - waterOut);
	// calculate time step-related variables, make sure they don't fall below the threshold values
	if (maxdepth <= 0.1) 
		maxdepth = 0.1;
	if (time_factor < (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
		time_factor = (courant_number * (DX / std::sqrt(gravity * (maxdepth))));
	if (input_output_difference > in_out_difference && time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
		time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
		
	double local_time_factor = time_factor;
	if (local_time_factor > (courant_number * (DX  / std::sqrt(gravity * (maxdepth)))))
	{
		local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
	}
	
	
	// increment the counters
	counter++;
	cycle += time_factor / 60;
	// cycle is minutes, time_factor is seconds
	
	
	// WATER INPUTS
	//double waterinput = 0;
	
	// route the water and update the flow depths
	qroute();
	depth_update();
	call_erosion();
	call_lateral();
	water_flux_out(local_time_factor);
	
	
	output_data();  // not sure if this is the best place to put this, but it needs to be done every timestep? - DAV
	
	if (cycle >= save_time)
	{
		save_data_and_draw_graphics(); //similar to above worry?
		save_time += saveinterval;
	}
	
	// if we have reached the end of the run, kill the cycle
	} while (cycle < maxcycle * 60);
	// here endeth the erodepo loop!
	
}

//void LSDModelDriver::initialise_params_CM()
//{
	//save_interval2 = int.Parse(googAnimationSaveInterval.Text);
	//save_time2 = save_interval2;

	//in_out_difference=float_parameters["input_output_diff"];
	
	//ERODEFACTOR=double.Parse(erodefactorbox.Text);
	//root=(Math.Sqrt(Math.Pow(DX,2)+Math.Pow(DX,2)));
	//LIMIT=int.Parse(limitbox.Text);
	
	//MIN_Q=float_parameters["min_Q_for_depth_calc"];
	//CREEP_RATE=float_parameters["creep_rate"];
	//SOIL_RATE =float_parameters["soil_erosion_rate"];
	
	//bed_proportion=double.Parse(lateralratebox.Text);

	//cycle=double.Parse(textBox1.Text)*60;

	//maxcycle=int.Parse(cyclemaxbox.Text);
	
	//failureangle=float_parameters["slope_failure_thresh"];

	//saveinterval=float_parameters["save_interval"];
	//M=float_parameters["TOPMODEL_m_value"];
	//grow_grass_time=double.Parse(grasstextbox.Text);
	//output_file_save_interval=int.Parse(outputfilesaveintervalbox.Text);
	//tx=output_file_save_interval;
	//min_time_step=int_parameters["min_time_step"];
	//active=double.Parse(activebox.Text);
	//k_evap=double.Parse(k_evapBox.Text);  // added MJ 15/03/05
	//vegTauCrit=float_parameters["veg_crit_shear"];
	//max_time_step = int.Parse(max_time_step_Box.Text);	
	//water_depth_erosion_threshold = double.Parse(Q2box.Text);
	//max_vel = double.Parse(max_vel_box.Text);
	//courant_number = double.Parse(courantbox.Text);
	//hflow_threshold = double.Parse(textBox4.Text);
	//lateral_cross_channel_smoothing = double.Parse(textBox7.Text);
	//froude_limit = double.Parse(textBox8.Text);
	//mannings = double.Parse(textBox9.Text);

	//rfnum = int.Parse(rfnumBox.Text);

	//if (googleAnimationCheckbox.Checked)
	//{
		//startDate = googleBeginDate.Text;
		//googleTime = System.DateTime.Parse(googleBeginDate.Text);
	//}
	//d1=double.Parse(g1box.Text);
	//d2=double.Parse(g2box.Text);
	//d3=double.Parse(g3box.Text);
	//d4=double.Parse(g4box.Text);
	//d5=double.Parse(g5box.Text);
	//d6=double.Parse(g6box.Text);
	//d7=double.Parse(g7box.Text);
	//d8=double.Parse(g8box.Text);
	//d9=double.Parse(g9box.Text);

	//d1prop=double.Parse(gp1box.Text);
	//d2prop=double.Parse(gp2box.Text);
	//d3prop=double.Parse(gp3box.Text);
	//d4prop=double.Parse(gp4box.Text);
	//d5prop=double.Parse(gp5box.Text);
	//d6prop=double.Parse(gp6box.Text);
	//d7prop=double.Parse(gp7box.Text);
	//d8prop=double.Parse(gp8box.Text);
	//d9prop=double.Parse(gp9box.Text);

	//// new array to deal with suspended sediment   MJ 09/05/05
	//isSuspended = new bool[G_MAX+1];
	//isSuspended[1] = suspGS1box.Checked;
	//isSuspended[2] = false;
	//isSuspended[3] = false;
	//isSuspended[4] = false;
	//isSuspended[5] = false;
	//isSuspended[6] = false;
	//isSuspended[7] = false;
	//isSuspended[8] = false;
	//isSuspended[9] = false;

	//// new array to deal with suspended sediment   MJ 09/05/05
	//fallVelocity = new double[G_MAX+1];
	//fallVelocity[1] = double.Parse(fallGS1box.Text);
	//fallVelocity[2] = double.Parse(fallGS2box.Text);
	//fallVelocity[3] = double.Parse(fallGS3box.Text);
	//fallVelocity[4] = double.Parse(fallGS4box.Text);
	//fallVelocity[5] = double.Parse(fallGS5box.Text);
	//fallVelocity[6] = double.Parse(fallGS6box.Text);
	//fallVelocity[7] = double.Parse(fallGS7box.Text);
	//fallVelocity[8] = double.Parse(fallGS8box.Text);
	//fallVelocity[9] = double.Parse(fallGS9box.Text);

	//input_time_step=int.Parse(input_time_step_box.Text);
	//stage_input_time_step = double.Parse(TidalInputStep.Text);
	//edgeslope =double.Parse(textBox2.Text);
	//lateral_constant=double.Parse(textBox3.Text);
	//veg_lat_restriction = double.Parse(veg_lat_box.Text);

	//bedrock_erosion_rate = double.Parse(bedrock_erosion_rate_box.Text);
	//bedrock_erosion_threshold = double.Parse(bedrock_erosion_threshold_box.Text);
//}
	

void LSDCatchmentModel::zero_values()
{
	// To do
	int x,y,z,n;

	for(y=0; y<=ymax; y++)
	{
		for(x=0; x<=xmax; x++)
		{
			Vel[x][y] = 0;
			area[x][y] = 0;
			elev[x][y] = 0;
			bedrock[x][y] = -9999;
			init_elevs[x][y] = elev[x][y];
			water_depth[x][y] = 0;
			index[x][y] = -9999;
			inputpointsarray[x][y] = false;
			veg[x][y][0] = 0;// elevation
			veg[x][y][1] = 0; // density
			veg[x][y][2] = 0; // jw density
			veg[x][y][3] = 0; // height
			
			edge[x][y] = 0;
			edge2[x][y] = 0;
			
			sand[x][y] = 0;

			qx[x][y] = 0;
			qy[x][y] = 0;

			qxs[x][y] = 0;
			qys[x][y] = 0;

			
			for(n=0; n<=8; n++) 
			{
				vel_dir[x][y][n]=0;
			}
			
			Vsusptot[x][y] = 0;
			rfarea[x][y] = 1;
			
		}
	}

	for(x=1; x<((xmax*ymax)/LIMIT); x++)
	{
		for(y=0; y<=G_MAX; y++)
		{
				grain[x][y] = 0;
		}
		for(z=0; z<=9; z++)
		{
			for(y=0; y<=G_MAX-2; y++)
			{

					strata[x][z][y] =0;
			}
		}
		catchment_input_x_coord[x] = 0;
		catchment_input_y_coord[x] = 0;
	}	

	for (x = 1; x <= rfnum; x++)
	{
		j[x] = 0.000000001;
		jo[x] = 0.000000001;
		j_mean[x] = 0;
		old_j_mean[x] = 0;
		new_j_mean[x] = 0;
	}

}

// __________________________________________
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// HYDROLOGICAL METHODS
// A series of methods for doing the 
// hydro routing, water-depths, etc
// 
// CONTENTS:
// 1. water_flux_out
// 2. qroute
// 3. depth_update
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	
	
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// WATER FLUXES OUT OF CATCHMENT
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculate the water coming out and zero any water depths at the edges
// This will actually set it to the minimum water depth
// This must be done so that water can still move sediment to the edge of the catchment
// and hence remove it from the catchment. (otherwise you would get sediment build
// up around the edges.
void LSDCatchmentModel::water_flux_out(double local_time_factor)    // Extracted as a seprate method from erodepo()
{
	double temptot = 0;    // temporary water total
	for (int y = 1; y <= ymax; y++)
	{
		// Note about C# vs C++ TNT Arrays, 
		// C# uses: A[x,y]
		// C++ TNT array uses: A[x][y]
		
		if (water_depth[xmax][y] > water_depth_erosion_threshold)
		{
			temptot += (water_depth[xmax][y] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
			water_depth[xmax][y] = water_depth_erosion_threshold;
		}
		
		if (water_depth[1][y] > water_depth_erosion_threshold)
		{
			temptot += (water_depth[1][y] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
			water_depth[1][y] = water_depth_erosion_threshold;
		}
	}
	
	for (int x = 1; x <= xmax; x++)
	{ 
		if (water_depth[x][1] > water_depth_erosion_threshold)
		{
			temptot += (water_depth[x][1] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
			water_depth[x][1] = water_depth_erosion_threshold;
		}
		
		if (water_depth[x][ymax] > water_depth_erosion_threshold)
		{
			temptot += (water_depth[x][ymax] - water_depth_erosion_threshold) * DX * DX / local_time_factor;
			water_depth[x][ymax] = water_depth_erosion_threshold;
		}
	}
	waterOut = temptot;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// THE WATER ROUTING ALGORITHM: LISFLOOD-FP 
// 
// Originally derived in Bates et al (2010) Journal of Hydrology
// The Lisflood-FP algorithm is a non steady state water flow
// algorithm that uses a simplified version of the Saint-Venant
// shallow water equations (see Hydrology textbooks) to calculate
// water transport across the model.
//
// Originally implemented in CL in C# by TC
// Translated here to C++ by DAV for LSD
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDCatchmentModel::qroute()
{
	double local_time_factor = time_factor;
	if (local_time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
		local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
		
	// PARALLELISATION	COULD BE INSERTED HERE - DAV
	// #OMP PARALLEL...etc
	
	int inc = 1;
	for (int y =1; y < ymax+1; y++)
	{
	while (down_scan[y][inc] > 0)
	{
		int x = down_scan[y][inc];
		inc++;
		y++;
		
		if (elev[x][y] > -9999)   // stops the water moving into NODATA value cells
		{
			// =-=-=-=-=-=-=-=-=-=-=-=-=-=
			// ROUTING IN THE X-DIRECTION
			// =-=-=-=-=-=-=-=-=-=-=-=-=-=
			// check that we are in a cell containing water and not a -9999 cell
			if ((water_depth[x][y] > 0 || water_depth[x-1][y] > 0) && elev[x - 1][y] > -9999)
			{
				// find the amount of horizontal flow by comparing the water heights of two neighbouring cells: x and x-1. 
				double hflow = std::max(elev[x][y] + water_depth[x][y], elev[x-1][y] + water_depth[x-1][y]) -
								std::max(elev[x-1][y], elev[x][y]);
								
				if (hflow > hflow_threshold)	// don't bother computing flow if it is so minuscule...
				{
					double tempslope = (((elev[x-1][y] + water_depth[x-1][y])) - (elev[x][y] + water_depth[x][y])) / DX;
				
					if (x == xmax) tempslope = edgeslope;
					if (x <= 2) tempslope = 0 - edgeslope; // this deals with the problem of the next-to-edge cells
					
					// appears to be some variation of the Darcy-Weisbach formula? Should check this - DAV.
					// calculate discharge out of cells in x direction.
					qx[x][y] = ((qx[x][y] - (gravity * hflow * local_time_factor * tempslope)) /
								(1 + gravity * hflow * local_time_factor * (mannings * mannings) * std::abs(qx[x][y]) /
									std::pow(hflow, (10/3))));
					
					// Below, the lines attempt to stop a situation where there would be too much water 
					// moving from one cell to another in a single time step, resulting in negative 
					// discharges. Large instabilities can develop in steep catchments.
					if (qx[x][y] > 0 && (qx[x][y] / hflow)/std::sqrt(gravity*hflow) > froude_limit) 
						qx[x][y] = hflow * (std::sqrt(gravity * hflow) * froude_limit);
						
					if (qx[x][y] < 0 && std::abs(qx[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit) 
						qx[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit));
						
					if ((qx[x][y] > 0 && qx[x][y] * local_time_factor / DX) > (water_depth[x][y] / 4))
						qx[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;
						
					if (qx[x][y] < 0 && std::abs(qx[x][y] * local_time_factor / DX) > (water_depth[x-1][y] / 4))
						qx[x][y] = 0 - ((water_depth[x-1][y] * DX) / 5) / local_time_factor;
						
					// BOOL IF statement about the suspended sediment option
					//if (isSuspended == true)
					if (suspended_opt == true)
					{
						if (qx[x][y] > 0) 
							qxs[x][y] = qx[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
						if (qx[x][y] < 0) 
							qxs[x][y] = qx[x][y] * (Vsusptot[x-1][y] / water_depth[x-1][y]);
							
						if (qxs[x][y] > 0 && std::abs(qxs[x][y] * local_time_factor > (Vsusptot[x-1][y] * DX) / 4))
						{
							qxs[x][y] = 0 - ((Vsusptot[x-1][y] * DX) / 5 / local_time_factor);
						}
					}
					
					// CALCULATE THE VELOCITY OF THE FLOWS
					if (qx[x][y] > 0) 
						vel_dir[x][y][7] = qx[x][y] / hflow;
					if (qx[x][y] < 0)
						vel_dir[x-1][y][3] = (0 - qx[x][y]) / hflow;
				}
				else // What to do if the water depth is really, really small:
				{
					qx[x][y] = 0;
					qxs[x][y] = 0;
				}
			}  // end of the X-direction routing code block
			
			// =-=-=-=-=-=-=-=-=-=-=-=-=-=
			// ROUTING IN THE Y-DIRECTION
			// =-=-=-=-=-=-=-=-=-=-=-=-=-=
			// check that we are in a cell containing water and not a -9999 cell
			if ((water_depth[x][y] > 0 || water_depth[x][y-1] > 0) && elev[x][y-1] > -9999)
			{
				// find the amount of horizontal flow by comparing the water heights of two neighbouring cells: x and x-1. 
				double hflow = std::max(elev[x][y] + water_depth[x][y], elev[x][y-1] + water_depth[x][y-1]) -
								std::max(elev[x][y], elev[x][y-1]);
								
				if (hflow > hflow_threshold)	// don't bother computing flow if it is so minuscule...
				{
					double tempslope = (((elev[x][y-1] + water_depth[x][y-1])) - (elev[x][y] + water_depth[x][y])) / DX;
				
					if (y == xmax) tempslope = edgeslope;
					if (y <= 2) tempslope = 0 - edgeslope; // this deals with the problem of the next-to-edge cells
					
					// The Lisflood-FP algorithm developed in Bates et al 2010 - DAV.
					// calculate discharge out of cells in y direction.
					qy[x][y] = ((qy[x][y] - (gravity * hflow * local_time_factor * tempslope)) /
								(1 + gravity * hflow * local_time_factor * (mannings * mannings) * std::abs(qy[x][y]) /
									std::pow(hflow, (10/3))));
					
					// Below, the lines attempt to stop a situation where there would be too much water 
					// moving from one cell to another in a single time step, resulting in negative 
					// discharges. Large instabilities can develop in steep catchments.
					if (qy[x][y] > 0 && (qy[x][y] / hflow)/std::sqrt(gravity*hflow) > froude_limit) 
						qy[x][y] = hflow * (std::sqrt(gravity * hflow) * froude_limit);
						
					if (qy[x][y] < 0 && std::abs(qy[x][y] / hflow) / std::sqrt(gravity * hflow) > froude_limit) 
						qy[x][y] = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit));
						
					if ((qy[x][y] > 0 && qy[x][y] * local_time_factor / DX) > (water_depth[x][y] / 4))
						qy[x][y] = ((water_depth[x][y] * DX) / 5) / local_time_factor;
						
					if (qy[x][y] < 0 && std::abs(qy[x][y] * local_time_factor / DX) > (water_depth[x][y-1] / 4))
						qy[x][y] = 0 - ((water_depth[x][y-1] * DX) / 5) / local_time_factor;
						
					// BOOL IF statement about the suspended sediment option
					if (suspended_opt == true)
					{
						if (qy[x][y] > 0) 
							qys[x][y] = qy[x][y] * (Vsusptot[x][y] / water_depth[x][y]);
						if (qy[x][y] < 0) 
							qys[x][y] = qy[x][y] * (Vsusptot[x][y-1] / water_depth[x][y-1]);
							
						if (qys[x][y] > 0 && std::abs(qys[x][y] * local_time_factor > (Vsusptot[x][y-1] * DX) / 4))
							qys[x][y] = 0 - ((Vsusptot[x][y-1] * DX) / 5 / local_time_factor);
					}
					
					// CALCULATE THE VELOCITY OF THE FLOWS
					if (qy[x][y] > 0) 
						vel_dir[x][y][1] = qy[x][y] / hflow;
					if (qy[x][y] < 0)
						vel_dir[x][y-1][5] = (0 - qy[x][y]) / hflow;
				}
				
				else // What to do if the water depth is really, really small:
				{
					qy[x][y] = 0;
					qys[x][y] = 0;
				}
			}   // end of the y-direction water routing block
		}
	} // endwhile
    } // endfor
}

void LSDCatchmentModel::depth_update()
{
	double local_time_factor = time_factor;
	
	if (local_time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
		local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
		
	//double tempmaxdepth2[]; 
	
	// In the CL code, the array is initialised using 'new' but I am not sure why this needs to be done like this?
	// Quote: "Using C++ arrays with new (that is, using dynamical arrays) should be avoided. There is the problem
	// you have to keep track of the size, and you need to delete them manually, and do all sort of housekeeping."
	// e.g.:
	// double tempmaxdepth2[]; 
	// tempmaxdepth2 = new double[ymax + 2]();
	
	// Why not use:
	std::vector<double> tempmaxdepth2(ymax + 2);
	maxdepth = 0;
	int inc = 1;
	double tempmaxdepth = 0;
	for (int y=1; y<=ymax; y++)
	{
	
	while (down_scan[y][inc] > 0)
	{
		int x = down_scan[y][inc];
		inc++;
		
		// UPDATE THE WATER DEPTHS
		water_depth[x][y] += local_time_factor * (qx[x+1][y] - qx[x][y] + qy[x][y+1] - qy[x][y]) / DX;
		
		// UPDATE THE SUSPENDED SEDIMENT CONCENTRATIONS
		if (suspended_opt == true)
		{
			Vsusptot[x][y] += local_time_factor * (qxs[x + 1][y] - qxs[x][y] + qys[x][y + 1] - qys[x][y]) / DX;
		}
		
		if (water_depth[x][y] > 0) 
		{
			// remove any water depths on NODATA cells (This shouldnae happen!)
			if (elev[x][y] == -9999)
				water_depth[x][y] = 0;
				
			// calculate the maximum flow depth for this time step.
			if (water_depth[x][y] > tempmaxdepth) 
				tempmaxdepth = water_depth[x][y];
		}
	}
	tempmaxdepth2[y] = tempmaxdepth;
	}
	// reduction (if paralellism implemented at later date)
	for (int y = 1; y <= ymax; y++)
		if (tempmaxdepth2[y] > maxdepth) 
			maxdepth = tempmaxdepth2[y];
}
		
void LSDCatchmentModel::catchment_water_input_and_hydrology( double local_time_factor)     // DAV - This can be split into subfunctions
{
	for (int z=1; z <= totalinputpoints; z++)
	{
		int x = catchment_input_x_coord[z];
		int y = catchment_input_y_coord[z];
		double water_add_amt = (j_mean[rfarea[x][y]] * nActualGridCells[rfarea[x][y]]) / 
									(catchment_input_counter[rfarea[x][y]]) * local_time_factor;    //
		if (water_add_amt > ERODEFACTOR) 
		{
			water_add_amt = ERODEFACTOR;
		}
		
		waterinput += (water_add_amt / local_time_factor) * DX * DX;
		water_depth[x][y] += water_add_amt;
	}
	
	// if the input type flag is 1 then the discharge is input from the hydrograph
	if (cycle >= time_1)
	{
		do 
		{
			time_1++;
			calc_J(time_1++);  // calc_J is based on the rainfall rate supplied to the cell
			if (time_factor > max_time_step && new_j_mean[1] > (0.2 / (xmax * ymax * DX * DX)))
				// check after the variable rainfall area has been added
				// stops code going too fast when there is actual flow in the channels greater than 0.2cu
			{
				cycle = time_1 + (max_time_step / 60);
				time_factor = max_time_step;
			}
		} while (time_1 < cycle);
	}
	
	calchydrograph(time_1 - cycle);
	
	double jmeanmax =0;
	for (int n=1; n <= rfnum; n++)
	{
		if (j_mean[n] > jmeanmax)
		{
			jmeanmax = j_mean[n];
		}
	}
	
	if (jmeaninputfile_opt == true) 
	// Yikes, this was originally hard-coded into the GUI
	// interface in CL... (DAV - Needs a new bool param declaring and setting in the driver file)
	{
		j_mean[1] = ((hourly_rain_data[(static_cast<int>(cycle / rain_data_time_step))][1] //check in original
							/ std::pow(DX, 2)) / nActualGridCells[1]);
	}
	
	if (jmeanmax >= baseflow)
	{
		baseflow = baseflow * 3;    // Magic number 3!? - DAV
		get_area(); 		// Could this come from one of the LSDobject files? - DAV
		get_catchment_input_points();
	}	
			
	if (baseflow > (jmeanmax * 3) && baseflow > 0.0000001)
	{
		baseflow = jmeanmax * 1.25;   // Where do these magic numbers come from? DAV
		get_area();
		get_catchment_input_points();		
	}
}

// Calculates the rainfall input to each cell per time step
void LSDCatchmentModel::calc_J(double cycle)    // J is the local rainfall inputed into the cell at each timestep 
										// (Actually this is j_mean)
{
	for (int n=1; n <= rfnum; n++)    // rfnum is the rainfall number int = 2 to begin with
	{
		double local_rain_fall_rate = 0;   // in metres per second
		double local_time_step = 60; // in seconds
		
		old_j_mean[n] = new_j_mean[n];
		jo[n] = j[n];
		
		// Get the M value from the files if one is specified
        if (variable_m_value_flag == 1)
		{
			M = hourly_m_value[1 + static_cast<int>(cycle / rain_data_time_step)];
		}

		local_rain_fall_rate = 0;
		if (hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n] > 0)
		{
			local_rain_fall_rate = rain_factor * ((hourly_rain_data[static_cast<int>(cycle / rain_data_time_step)][n] / 1000) / 3600); 
			/** divide by 1000 to make m/hr, then by 3600 for m/sec */
		}

		if (local_rain_fall_rate == 0)
		{
			j[n] = jo[n] / (1 + ((jo[n] * local_time_step) / M));
			new_j_mean[n] = M / local_time_step * std::log(1 + ((jo[n] * local_time_step) / M));
		}

		if (local_rain_fall_rate > 0)
		{
			j[n] = local_rain_fall_rate / (((local_rain_fall_rate - jo[n]) / jo[n]) * std::exp((0 - local_rain_fall_rate) * local_time_step / M) + 1);
			new_j_mean[n] = (M / local_time_step) * std::log(((local_rain_fall_rate - jo[n]) + jo[n] * std::exp((local_rain_fall_rate * local_time_step) / M)) / local_rain_fall_rate);
		}
		if (new_j_mean[n] < 0) 
		{
			new_j_mean[n] = 0;	
		}	
		// (M is the TOPMODEL m value)
		// DAV - Make a seperate function?

	}
}	
		
// Calculates the storm hydrograph		
void LSDCatchmentModel::calchydrograph(double time)
{
	for (int n=1; n <= rfnum; n++)
	{
		j_mean[n] = old_j_mean[n] + (((new_j_mean[n] - old_j_mean[n]) / 2) * (2 - time));
	}
}

		
void LSDCatchmentModel::get_catchment_input_points()
{
	
	totalinputpoints = 1;
	for (int n=1; n <= rfnum; n++)
	{
		catchment_input_counter[n] = 0;
	}
	for (int x=1; x <= xmax; x++)
	{
		for (int y=1; y <= ymax; y++)
		{
			if ((area[x][y] * j_mean[rfarea[x][y]] * 3 * DX * DX) > MIN_Q \
					&& (area[x][y] * j_mean[rfarea[x][y]] * 3 * DX * DX) < MIN_Q_MAXVAL)
					{
						catchment_input_x_coord[totalinputpoints] = x;
                        catchment_input_y_coord[totalinputpoints] = y;
                        catchment_input_counter[rfarea[x][y]]++;
                        totalinputpoints++;
                    }
                }
            }
        }

void LSDCatchmentModel::evaporate(double time)
{
	double evap_amount = k_evap * (time / 1440);
		// now reduce if greater than erodedepo - to prevent instability
		if (evap_amount > ERODEFACTOR)
		{
			evap_amount = ERODEFACTOR;
		}
		//var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
		//Parallel.For(1, ymax, options, delegate(int y)
		for (int y=1; y < ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0)
			{
				int x = down_scan[y][inc];
				inc++;
				// removes water in rate of mm per day..
				if (water_depth[x][y] > 0)
				{
					water_depth[x][y] -= evap_amount;
					if (water_depth[x][y] < 0) water_depth[x][y] = 0;
				}

			}

		}//);
}

void LSDCatchmentModel::scan_area()
{
	//var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
	//Parallel.For(1, ymax+1, options, delegate(int y)
	for (int y=1; y < ymax; y++)
	{
		int inc = 1;

		for (int x = 1; x <= xmax; x++)
		{
			// zero scan bit..
			down_scan[y][x] = 0;
			// and work out scanned area.
			if (water_depth[x][y] > 0
				|| water_depth[x - 1][y] > 0
				|| water_depth[x - 1][y - 1] > 0
				|| water_depth[x - 1][y + 1] > 0
				|| water_depth[x + 1][y - 1] > 0
				|| water_depth[x + 1][y + 1] > 0
				|| water_depth[x][y - 1] > 0
				|| water_depth[x + 1][y] > 0
				|| water_depth[x][y + 1] > 0)
			{
				down_scan[y][inc] = x;
				inc++;
			}
		}
	}//);	
}


// __________________________________________
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// EROSIONAL METHODS
// A series of methods for doing the 
// eroding, landsliding etc
// 
// CONTENTS:
//	1) sort_active
// 	2) addGS
//	3) sand_fraction
//	4) d50
//	5) slide_GS
//	6) mean_ws_elev
//	7) erode
//	8) lateral3
//	9) slide_3
//	10) slide_5
//	11) creep
//	12) soil_erosion
//	13) soil_development
//
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCatchmentModel::sort_active(int x,int y)
{
	int xyindex;
	double total;
	double amount;
	double coeff;
	int n, z;
	

	if (index[x][y] == -9999) addGS(x,y);  // should not be necessary
	xyindex = index[x][y];

	total = 0.0;
	for (n=0;n<=G_MAX;n++)
	{

			total += grain[xyindex][n];

	}

	if (total > (active*1.5)) // depositing - create new strata layer and remove bottom one..
	{
		// start from bottom
		// remove bottom active layer
		// then move all from layer above into one below, up to the top layer
		for(z=9;z>=1;z--)
		{
			for(n=0;n<=G_MAX-2;n++)
			{

					strata[xyindex][z][n]=strata[xyindex][z-1][n];

			}
		}

		// then remove strata thickness from grain - and add to top strata layer
		coeff = active / total;
		for (n=1;n<=(G_MAX-1);n++)
		{
			if ((grain[xyindex][n] > 0.0))
			{

					amount = coeff * (grain[xyindex][n]);
					strata[xyindex][0][n-1] = amount;
					grain[xyindex][n] -= amount;

			}
		}
	}

	if (total < (active/4)) // eroding - eat into existing strata layer & create new one at bottom
	{
		// Start at top
		// Add top strata to grain
		for (n=1;n<=(G_MAX-1);n++)
		{
				grain[xyindex][n] += strata[xyindex][0][n-1];
		}

		// then from top down add lower strata into upper
		for(z=0;z<=8;z++)
		{
			for(n=0;n<=G_MAX-2;n++)
			{
					strata[xyindex][z][n] = strata[xyindex][z+1][n];
			}
		}

		// add new layer at the bottom
		amount = active;
		z = 9;
		strata[xyindex][z][0] = amount*d1prop; // 0.0;
		strata[xyindex][z][1] = amount*d2prop; // + amount*d1prop;
		strata[xyindex][z][2] = amount*d3prop;
		strata[xyindex][z][3] = amount*d4prop;
		strata[xyindex][z][4] = amount*d5prop;
		strata[xyindex][z][5] = amount*d6prop;
		strata[xyindex][z][6] = amount*d7prop;
		strata[xyindex][z][7] = amount*d8prop;
		strata[xyindex][z][8] = amount*d9prop;

	}

}

void LSDCatchmentModel::addGS(int &x, int &y)
{
		// needs lock statement to stop two being added at the same time...
		//lock (this) // DAV - This is only for paralllism...
		//{
			int n, q;
			grain_array_tot++;
			index[x][y] = grain_array_tot;

			grain[grain_array_tot][0] = 0;
			grain[grain_array_tot][1] = active * d1prop;
			grain[grain_array_tot][2] = active * d2prop;//+(active*d1prop);
			grain[grain_array_tot][3] = active * d3prop;
			grain[grain_array_tot][4] = active * d4prop;
			grain[grain_array_tot][5] = active * d5prop;
			grain[grain_array_tot][6] = active * d6prop;
			grain[grain_array_tot][7] = active * d7prop;
			grain[grain_array_tot][8] = active * d8prop;
			grain[grain_array_tot][9] = active * d9prop;
			grain[grain_array_tot][10] = 0;


			for (n = 0; n <= 9; n++)
			{
				strata[grain_array_tot][n][0] = (active) * d1prop;
				strata[grain_array_tot][n][1] = (active * d2prop);
				strata[grain_array_tot][n][2] = (active) * d3prop;
				strata[grain_array_tot][n][3] = (active) * d4prop;
				strata[grain_array_tot][n][4] = (active) * d5prop;
				strata[grain_array_tot][n][5] = (active) * d6prop;
				strata[grain_array_tot][n][6] = (active) * d7prop;
				strata[grain_array_tot][n][7] = (active) * d8prop;
				strata[grain_array_tot][n][8] = (active) * d9prop;

				if (elev[x][y] - (active * (n + 1)) < (bedrock[x][y] - active))
				{
					for (q = 0; q <= (G_MAX - 2); q++)
					{
						strata[grain_array_tot][n][q] = 0;
					}
				}
			}


			sort_active(x, y);

		//} // this is for the locak statement above

}

double LSDCatchmentModel::sand_fraction(int index1)
{

	int n;
	double active_thickness=0;
	double sand_total=0;
	for(n=1;n<=G_MAX;n++)
	{
		active_thickness+=(grain[index1][n]);
	}

	for(n=1;n<=2;n++) // number of sand fractions...
	{
		sand_total+=(grain[index1][n]);
	}

	if(active_thickness<0.0001)
	{
		return(0.0);
	}
	else
	{
		return(sand_total/active_thickness);
	}

}

double LSDCatchmentModel::mean_ws_elev(int x, int y)
{
	double elevtot = 0;
	int counter = 0;

	for (int dir = 1; dir <= 8; dir++)
	{
		int x2, y2;
		x2 = x + deltaX[dir];
		y2 = y + deltaY[dir];

		if (water_depth[x2][y2] > water_depth_erosion_threshold)
		{
			elevtot += water_depth[x2][y2] + elev[x2][y2];
			counter++;
		}

	}
	if (counter > 0) {
		elevtot /= counter;
		return elevtot;
	}

	else return 0;
}

double LSDCatchmentModel::d50(int index1)
{
	int z,n,i;
	double active_thickness=0;
	double Dfifty=0,max=0,min=0;
	std::array<double, 20> cum_tot;  // std::array only C++11
	
	// The C# way of doing it:
	//double[] cum_tot;
	//cum_tot = new double[20];
	
	for(n=1;n<=G_MAX;n++)
	{
		for(z=0;z<=(0);z++)
		{
			active_thickness+=(grain[index1][n]);
			cum_tot[n]+=active_thickness;
		}
	}
	

	i=1;
	while(cum_tot[i]<(active_thickness*0.5) && i<=9)
	{
		i++;
	}
	
	if(i==1){min=std::log(d1);max=std::log(d1);}
	if(i==2){min=std::log(d1);max=std::log(d2);}
	if(i==3){min=std::log(d2);max=std::log(d3);}
	if(i==4){min=std::log(d3);max=std::log(d4);}
	if(i==5){min=std::log(d4);max=std::log(d5);}
	if(i==6){min=std::log(d5);max=std::log(d6);}
	if(i==7){min=std::log(d6);max=std::log(d7);}
	if(i==8){min=std::log(d7);max=std::log(d8);}
	if(i==9){min=std::log(d8);max=std::log(d9);}
	//if(i==9){min=std::log(d8);max=std::log(d9);}

	Dfifty = std::exp(max - ((max - min) * ((cum_tot[i] - (active_thickness * 0.5)) / (cum_tot[i] - cum_tot[i - 1]))));
	if(active_thickness<0.0000001)Dfifty=0;
	return Dfifty;

}

void LSDCatchmentModel::slide_GS(int x,int y, double amount,int x2, int y2)
{

	/** Ok, heres how it works, x and y are ones material moved from,
	  x2 and y2 are ones material moved to...
	  amd amount is the amount shifted. */

	int n;
	double total = 0;

	// do only for cells where both have grainsize..

	if (index[x][y] != -9999 && index[x2][y2] != -9999)
	{
		for (n = 1; n <= (G_MAX - 1); n++)
		{
			if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
		}

		if (amount > total)
		{
			grain[index[x2][y2]][1] += (amount - total) * d1prop;
			grain[index[x2][y2]][2] += (amount - total) * d2prop;
			grain[index[x2][y2]][3] += (amount - total) * d3prop;
			grain[index[x2][y2]][4] += (amount - total) * d4prop;
			grain[index[x2][y2]][5] += (amount - total) * d5prop;
			grain[index[x2][y2]][6] += (amount - total) * d6prop;
			grain[index[x2][y2]][7] += (amount - total) * d7prop;
			grain[index[x2][y2]][8] += (amount - total) * d8prop;
			grain[index[x2][y2]][9] += (amount - total) * d9prop;

			amount = total;
		}

		if (total > 0)
		{
			for (n = 1; n <= (G_MAX - 1); n++)
			{
				double transferamt = amount * (grain[index[x][y]][n] / total);
				grain[index[x2][y2]][n] += transferamt;
				grain[index[x][y]][n] -= transferamt;
				if (grain[index[x][y]][n] < 0) grain[index[x][y]][n] = 0;
			}

		}

		/* then to set active layer to correct depth before erosion, */
		sort_active(x, y);
		sort_active(x2, y2);
		return;
	}

	//now do for cells where only recieving cells have grainsize
	// just adds amount to reviving cells of normal..
	if (index[x][y] == -9999 && index[x2][y2] != -9999)
	{
		grain[index[x2][y2]][1] += (amount) * d1prop;
		grain[index[x2][y2]][2] += (amount) * d2prop;
		grain[index[x2][y2]][3] += (amount) * d3prop;
		grain[index[x2][y2]][4] += (amount) * d4prop;
		grain[index[x2][y2]][5] += (amount) * d5prop;
		grain[index[x2][y2]][6] += (amount) * d6prop;
		grain[index[x2][y2]][7] += (amount) * d7prop;
		grain[index[x2][y2]][8] += (amount) * d8prop;
		grain[index[x2][y2]][9] += (amount) * d9prop;

		/* then to set active layer to correct depth before erosion, */
		sort_active(x2, y2);
		return;
	}

	// now for cells whre dontaing cell has grainsize
	if (index[x][y] != -9999 && index[x2][y2] == -9999)
	{

		addGS(x2, y2); // add grainsize array for recieving cell..

		if (amount > active)
		{

			grain[index[x2][y2]][1] += (amount - active) * d1prop;
			grain[index[x2][y2]][2] += (amount - active) * d2prop;
			grain[index[x2][y2]][3] += (amount - active) * d3prop;
			grain[index[x2][y2]][4] += (amount - active) * d4prop;
			grain[index[x2][y2]][5] += (amount - active) * d5prop;
			grain[index[x2][y2]][6] += (amount - active) * d6prop;
			grain[index[x2][y2]][7] += (amount - active) * d7prop;
			grain[index[x2][y2]][8] += (amount - active) * d8prop;
			grain[index[x2][y2]][9] += (amount - active) * d9prop;

			amount = active;
		}


		for (n = 1; n <= (G_MAX - 1); n++)
		{
			if (grain[index[x][y]][n] > 0) total += grain[index[x][y]][n];
		}

		for (n = 1; n <= (G_MAX - 1); n++)
		{
			if (total > 0)
			{
				grain[index[x2][y2]][n] += amount * (grain[index[x][y]][n] / total);
				if (grain[index[x][y]][n] > 0.0001) grain[index[x][y]][n] -= amount * (grain[index[x][y]][n] / total);
				if (grain[index[x][y]][n] < 0) grain[index[x][y]][n] = 0;
			}

		}

		/* then to set active layer to correct depth before erosion, */
		sort_active(x, y);
		sort_active(x2, y2);
		return;
	}
}

double LSDCatchmentModel::erode(double mult_factor)
{
	double rho = 1000.0;
	// double gravity = 9.8;
	double tempbmax = 0;
	
	std::vector<double> gtot2(20);     // changed this from an array in the CL code
	// There is no need to use C-style arrays. The std::vector is preferred
	// Also, no need to initalise with new() and delete().
	
	for (int n=0; n <= G_MAX; n++)
	{
		gtot2[n] = 0;
	}
	
	time_factor = time_factor * 1.5;
	if (time_factor > max_time_step) 
	{
		time_factor = max_time_step;
	}
		
	int counter2 = 0;
	
	do
	{
		counter2++;
		tempbmax = 0;
		std::vector<double> tempbmax2(ymax + 2);
		
		// PARALELLISATION GOES HERE
		for (int y =1; y<=ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0);
			{
				int x = down_scan[y][inc];
				inc++;
				
				// zero the velocities
				Vel[x][y] = 0;
				Tau[x][y] = 0;
				
				for(int n=0; n < G_MAX; n++)
				{
					sr[x][y][n] = 0;
					sl[x][y][n] = 0;
					su[x][y][n] = 0;
					sd[x][y][n] = 0;
				}
				ss[x][y] = 0;
				
				if (water_depth[x][y] > water_depth_erosion_threshold)
				{ 
					double temptot1 = 0;
	                double temptot2 = 0;
	                double veltot = 0;
	                double vel = 0;
	                double qtot = 0;
	                double tau = 0;
	                double velnum = 0;
	                double slopetot = 0;		
					
					std::vector<double> temp_dist(11);    // this vector holds the amountto be removed from the cell in each grain size
					std::vector<double> tempdir(11);	// this vector hold the velocity directions temporariliy
					
					// check to see if the there is a nodata value in the cell
					if (index[x][y] == -9999)
					{
						addGS(x, y);    // add the grain size for that cell
					}
						
					// now add up the velocity directions, velocities, and edge directions
					for (int p=1; p <= 8; p += 2)   // D8 routing
					{
						int x2 = x + deltaX[p];
						int y2 = y + deltaY[p];
						
						if (water_depth[x2][y2] > water_depth_erosion_threshold)
						{
							if (edge[x][y] > edge[x2][y2]);
							{
								temptot2 += (edge[x][y] - edge[x2][y2]);
							}
						
							
							if (vel_dir[x][y][p] > 0)
							{
								// First, work out the velocities in each direction
								// (For the sediment distribution)
								vel = vel_dir[x][y][p];
								
								if (vel > max_vel)
								{
									vel = max_vel;   
									// if the velocity is too high, for model stability reasons
									// then it should be cut back to the max_vel.
									// (though this violates cons. of momentum!)
									// assume transferred to other stores...				
								}
								
								tempdir[p] = vel * vel;
								veltot += tempdir[p];
								velnum++;
								qtot += (vel * vel);
								slopetot += ((elev[x][y] - elev[x2][y2]) / DX) * vel;
							}						
						}
					}
					
					// calculate velocities for each cell
					// calculate tau (stress) for each cell
					if (qtot > 0) 
					{ 
						vel = (std::sqrt(qtot));
						Vel[x][y] = vel;
						
						if (vel < 0)
						{
							// Not sure what is done computationally here,
							// Original code just had a text box/meessage update?
						}
						
						// make sure no cells have vels greater than the max vel
						if (vel > max_vel) 
						{
							vel = max_vel;
						}
						
						double ci = gravity * (mannings * mannings) * std::pow(water_depth[x][y], -0.33);
						
						if (slopetot > 0)
						{
							slopetot = 0;
						}
						
						tau = 1000 * ci * vel * vel * (1 + (1 * (slopetot / vel)));
						Tau[x][y] = tau;
					}
					
					// NOW DO SOME EROSION
					// DAV- These ought to be seperate methods really, no need
					// for so many if blocks in a single function. 
					// i.e. one method for the Wilcock, one for Einstein.
					if (tau > 0)
					{
						double d_50 = 0;
						double Fs = 0;
						double Di = 0;
						double graintot = 0;
						
						if (wilcock == true)
						{
							d_50 = d50(index[x][y]);   // d50 function needs implemented
							if (d_50 < 1) 
							{
								d_50 = d1;
							}
							Fs = sand_fraction(index[x][y]);   // sand_fraction needs implemented
							
							for (int n=1; n <= G_MAX; n++)
							{
								graintot += (grain[index[x][y]][n]);
							}
						}
							
							
							for (int n=1; n <= 9; n++)
							{
								switch (n)
								{
									case 1: Di = d1; break;
									case 2: Di = d2; break;
									case 3: Di = d3; break;
									case 4: Di = d4; break;
									case 5: Di = d5; break;
									case 6: Di = d6; break;
									case 7: Di = d7; break;
									case 8: Di = d8; break;
									case 9: Di = d9; break;
								}
								
								// For the WILCOCK and CROW/CURRAN transport/erosion laws
								if (wilcock == true)
								{ 
									double tau_ri = 0;
									tau_ri = (0.021 + 64504398503);// WHATTT??!?!
									double U_star =  std::pow(tau / rho, 0.5);
									double Wi_star;
									double Fi = grain[index[x][y]][n] / graintot;
									
									if ((tau / tau_ri) < 1.35)
									{
										Wi_star = 0.002 * std::pow(tau / tau_ri, 7.5);
									}
									
									else
									{
										Wi_star = 14 * std::pow(1 - (0.894 / std::pow(tau / tau_ri, 0.5)), 4.5);
									}
									
									temp_dist[n] = mult_factor * time_factor * ((Fi * (U_star * U_star * U_star)) / ((2.65 - 1) * gravity)) * Wi_star / DX; 
								}
								
								// EINSTEIN Sediment option
								if (einstein == true)
								{	
									temp_dist[n] = mult_factor * time_factor * (40 * std::pow(((1 / (((2650 - 1000) * Di) / tau / gravity))), 3)) / std::sqrt(1000 / ((2250 - 1000) * gravity * (Di * Di * Di))) / DX;  // recheck the maths here!
								}
								
								// check that the amount of susp. sed. is not really really tiny. (if so, zero)
								if (temp_dist[n] < 0.0000000000001) temp_dist[n] = 0; 							
								// check to see if theres not too little sediment in the cell to be entrained
								if (temp_dist[n] > grain[(index[x][y])][n]) temp_dist[n] = grain[(index[x][y])][n];
								// then check to see if this would make the suspended sediment levels too high
								// reduce if too high
								if (isSuspended[n] && n ==1)
								{
									if ((temp_dist[n] + Vsusptot[x][y]) / water_depth[x][y] > Csuspmax)
									{							
										// work out the max amount of sediment that there can be (water_depth * csuspmax)
										// then subtract what's already there (Vsusptot) to leave what can
										// possibly be entrained. Check if < 0 afterwards.
										temp_dist[n] = (water_depth[x][y] * Csuspmax) - Vsusptot[x][y];
									}
								}
								// Check if < 0.
								if (temp_dist[n] < 0) temp_dist[n] = 0;
								
								// placed this here to speed up reduction of erode repeats
								temptot1  += temp_dist[n];   // DAV - Not sure exactly what this does yet...
							}
							
							// Interlude. Here's a relaxing picture of a lighthouse:
							/*
								  ----------.................._____________  _  .-.
							                                      _____.. . .   | |
							                   _____....------""""             uuuuu
							 ____....------""""                                |===|
							                                                   |===|
							                                                   |===|
							                                                   |===|
							 _a:f____________________________________________ .[__N]. _______
	
							*/
							// DAV - really need to flatten some of these for/if/else nested blocks...
							// Use separate small functions
							
							// CALCULATE THE SED CONC (NON SUSPENDED)
							// check if the substrate erosion makes it below the bedrock
							if (elev[x][y] - temptot1 <= bedrock[x][y])
							{
								// now remove this from the proportion that can be eroded
								// we can do this as we know the proportion (temptot) that is there to be eroded.
								double elevdiff = elev[x][y] - bedrock[x][y];
								double temptot3 = temptot1;
								temptot1 = 0;
								
								for (int n=1; n <= 9; n++)   // n <= 9 is to do each sediment fraction.
								{
									if (elev[x][y] <= bedrock[x][y])
									{
										temp_dist[n] = 0;
									}
									else
									{
										temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
										if (temp_dist[n] < 0) temp_dist[n] = 0;
									}
									temptot1  += temp_dist[n];   // DAV - Not sure exactly what this does yet...
								}
							}
							// BEDROCK EROSION ROUTINE GOES HERE
							//
							//  "If it weren't for the rocks in its bed,
							//   the stream would have no song."
							// 		- Carl Perkins
							//
							
							
							// DAV - This should definitely be a separate method.
							if (tau > bedrock_erosion_threshold)
							{
								double amount = 0;   // 'amount' is the amount of erosion into the bedrock
								amount = std::pow(bedrock_erosion_rate * tau, 1.5) * time_factor * mult_factor * 0.000000317;
								// last value is to turn this into erosion per year (number of years in a second)
								bedrock[x][y] -= amount;
								// now add the amount of bedrock eroded into the sediment proportions.
								grain[(index[x][y])][1] += amount * d1prop;
								grain[(index[x][y])][2] += amount * d2prop;
								grain[(index[x][y])][3] += amount * d3prop;
								grain[(index[x][y])][4] += amount * d4prop;
								grain[(index[x][y])][5] += amount * d5prop;
								grain[(index[x][y])][6] += amount * d6prop;
								grain[(index[x][y])][7] += amount * d7prop;
								grain[(index[x][y])][8] += amount * d8prop;
								grain[(index[x][y])][9] += amount * d9prop;	
							}
						}
						
						// VEGETATION COMPONENTS
						//
						// "Vegetation is the basic instrument the creator 
						//  uses to set all of nature in motion"
						// 		- Antoine Lavoisier
						if (veg[x][y][1] > 0 && tau > vegTauCrit)
						{
							// Remove from the vegetaton layer
							veg[x][y][1] -= mult_factor * time_factor * std::pow(tau - vegTauCrit, 0.5) * 0.00001;
							if (veg[x][y][1] < 0) veg[x][y][1] = 0;
						} 
						
						// Determine if movement should be restricted due to veg, or because 
						// of bedrock
						if (veg[x][y][1] > 0.25)   // Arbitrary threshold? - DAV
						{
							// now check if this amount removed from the cell would put it
							// below the veg layer
							if (elev[x][y] - temptot1 <= veg[x][y][0])
							{
								// now remove from the proportion that can be eroded
								double elevdiff = 0;
								elevdiff = elev[x][y] - veg[x][y][0];
								if (elevdiff < 0)  elevdiff = 0;double temptot3 = temptot1;
								temptot1 = 0;
								
								for (int n = 1; n <= 9; n++)
								{
									temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
									if(elev[x][y] <= veg[x][y][0]) temp_dist[n] = 0;
									temptot1 += temp_dist[n];
								}
								
								if (temptot1 < 0) temptot1 = 0;
							}
						}  // Here endeth the veg-block
						
						if (temptot1 > tempbmax2[y]) tempbmax2[y] = temptot1;   // I have no idea what this is for...
						// Too many temp... temp this temp that....need to rename - DAV
						
						// Work out what portion of bedload has to go where
						if (temptot1 > 0)
						{ 
							for (int p=1; p <= 8; p+=2)
							{
								int x2 = x + deltaX[p];
								int y2 = y + deltaY[p];
								
								if (water_depth[x2][y2] > water_depth_erosion_threshold)
								{
									if (index[x2][y2] == -9999) addGS(x2, y2);
									double factor =0;
									
									// vel slope
									if (vel_dir[x][y][p] > 0)
									{
										factor += 0.75 * tempdir[p] / veltot;
									}
									// now for the lateral gradient
									if (edge[x][y] > edge[x2][y2])
									{
										factor += 0.75 * ((edge[x][y] - edge[x2][y2]) / temptot2);
									}
									
									// Loop through the grainsizes
									for (int n=1; n<=9; n++)
									{
										if (temp_dist[n] > 0)
										{
											if (n == 1 && isSuspended[n])
											{
												// Put the amount entrained by ss into ss[,]
												ss[x][y] = temp_dist[n];
											}
											else
											{
												switch (p)
												{
													case 1: su[x][y][n] = temp_dist[n] * factor; break;
													case 3: sr[x][y][n] = temp_dist[n] * factor; break;
													case 5: sd[x][y][n] = temp_dist[n] * factor; break;
													case 7: sl[x][y][n] = temp_dist[n] * factor; break;	
												}
											}		// It's curly braces all the way down...
										}
									}							
								
							}
						}		// Refuge point for curly brace fatigue...
					}
				}
			}
			// "We need to do a reduction on tempbmax" - DAV I don't know what this means or does --- It is for the paralellism loops, you deleted them so you don't need it anymore.
			//for (int y=1; y<=ymax; y++) if (tempbmax2[y] > tempbmax) tempbmax = tempbmax2[y];   // This is the actual reduction bit
			
			if (tempbmax > ERODEFACTOR)
			{
				time_factor *= (ERODEFACTOR / tempbmax) * 0.5;
			}	
		} // ENDFOR
	} while (tempbmax > ERODEFACTOR);
	
	
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//	UPDATING GRAINSIZE ETC
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	
	// new temp erode array.
	TNT::Array2D<double> erodetot(xmax + 2, ymax + 2);
	//double [,] erodetot;
	//erodetot = new Double[xmax + 2, ymax + 2];

	TNT::Array2D<double> erodetot3(xmax + 2, ymax +2);
	//double[,] erodetot3;
	//erodetot3 = new Double[xmax + 2, ymax + 2];

	//var options1 = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
	//Parallel.For(2, ymax, options1, delegate(int y)
	for (int y=2; y<ymax; y++)
	{
		int inc = 1;
		while (down_scan[y][inc] > 0)
		{
			int x = down_scan[y][inc];
			inc++;

			if (water_depth[x][y] > water_depth_erosion_threshold && x < xmax && x > 1)
			{

				if (index[x][y] == -9999) addGS(x, y);
				for (int n = 1; n <= 9; n++)
				{
					if (n == 1 && isSuspended[n])
					{
						// updating entrainment of SS
						Vsusptot[x][y] += ss[x][y];
						grain[index[x][y]][n] -= ss[x][y];
						erodetot[x][y] -= ss[x][y];

						// this next part is unusual. You have to stop susp sed deposition on the input cells, otherwies
						// it drops sediment out, but cannot entrain as ss levels in input are too high leading to
						// little mountains of sediment. This means a new array in order to check whether a cell is an 
						// input point or not..
						if (!inputpointsarray[x][y])
						{
							// now calc ss to be dropped
							double coeff = (fallVelocity[n] * time_factor) / water_depth[x][y];
							if (coeff > 1) coeff = 1;
							double Vpdrop = coeff * Vsusptot[x][y];
							if (Vpdrop > 0.001) Vpdrop = 0.001; //only allow 1mm to be deposited per iteration
							grain[index[x][y]][n] += Vpdrop;
							erodetot[x][y] += Vpdrop;
							Vsusptot[x][y] -= Vpdrop;
							//if (Vsusptot[x][y] < 0) Vsusptot[x][y] = 0; NOT this line.
						}
					}
					else
					{
						//else update grain and elevations for bedload.
						double val1 = (su[x][y][n] + sr[x][y][n] + sd[x][y][n] + sl[x][y][n]);
						double val2 = (su[x][y + 1][n] + sd[x][y - 1][n] + sl[x + 1][y][n] + sr[x - 1][y][n]);
						grain[index[x][y]][n] += val2 - val1;
						erodetot[x][y] += val2 - val1;
						erodetot3[x][y] += val1;
					}
				}

				elev[x][y] += erodetot[x][y];
				if (erodetot[x][y] != 0) sort_active(x, y);

				//
				// test lateral code...
				//

				if (erodetot3[x][y] > 0)
				{
					double elev_update = 0;

					if (elev[x - 1][y] > elev[x][y] && x > 2)
					{
						double amt = 0;

						if (water_depth[x - 1][y] < water_depth_erosion_threshold)
							amt = mult_factor * lateral_constant * Tau[x][y] * edge[x - 1][y] * time_factor /DX;
						else amt = bed_proportion * erodetot3[x][y] * (elev[x - 1][y] - elev[x][y]) / DX * 0.1;

						if (amt > 0)
						{
							amt *= 1 - (veg[x - 1][y][1] * (1 - veg_lat_restriction));
							if ((elev[x - 1][y] - amt) < bedrock[x - 1][y] || x - 1 == 1) amt = 0;
							if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
							//if (amt > erodetot2 / 2) amt = erodetot2 / 2;
							elev_update += amt;
							elev[x - 1][y] -= amt;
							slide_GS(x - 1, y, amt, x, y);
						}
					}
					if (elev[x + 1][y] > elev[x][y] && x < xmax-1)
					{
						double amt = 0;
						if (water_depth[x + 1][y] < water_depth_erosion_threshold)
							amt = mult_factor * lateral_constant * Tau[x][y] * edge[x + 1][y] * time_factor / DX;
						else amt = bed_proportion * erodetot3[x][y] * (elev[x + 1][y] - elev[x][y]) / DX * 0.1;

						if (amt > 0)
						{
							amt *= 1 - (veg[x + 1][y][1] * (1 - veg_lat_restriction));
							if ((elev[x + 1][y] - amt) < bedrock[x + 1][y] || x + 1 == xmax) amt = 0;
							if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
							//if (amt > erodetot2 /2) amt = erodetot2 /2;
							elev_update += amt;
							elev[x + 1][y] -= amt;
							slide_GS(x + 1, y, amt, x, y);
						}
					}

					elev[x][y] += elev_update;
				}
			}
		}
	}

	//Parallel.For2(2, ymax, delegate(int y)
	for (int y = 2; y < ymax; y++)
	{
		int inc = 1;
		while (down_scan[y][inc] > 0)
		{
			int x = down_scan[y][inc];
			inc++;
			{

				if (erodetot3[x][y] > 0)
				{
					double elev_update = 0;

					if (elev[x][y - 1] > elev[x][y])
					{
						double amt = 0;
						if (water_depth[x][y - 1] < water_depth_erosion_threshold)
							amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y - 1] * time_factor / DX;
						else amt = bed_proportion * erodetot3[x][y] * (elev[x][y - 1] - elev[x][y]) / DX *0.1;

						if (amt > 0)
						{
							amt *= 1 - (veg[x][y - 1][1] * (1 - veg_lat_restriction));
							if ((elev[x][y - 1] - amt) < bedrock[x][y - 1] || y - 1 == 1) amt = 0;
							if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
							//if (amt > erodetot2 / 2) amt = erodetot2 / 2;
							elev_update += amt;
							elev[x][y - 1] -= amt;
							slide_GS(x, y - 1, amt, x, y);
						}
					}
					if (elev[x][y + 1] > elev[x][y])
					{
						double amt = 0;
						if (water_depth[x][y + 1] < water_depth_erosion_threshold)
							amt = mult_factor * lateral_constant * Tau[x][y] * edge[x][y + 1] * time_factor / DX;
						else amt = bed_proportion * erodetot3[x][y] * (elev[x][y + 1] - elev[x][y]) / DX * 0.1;

						if (amt > 0)
						{
							amt *= 1 - (veg[x][y + 1][1] * (1 - veg_lat_restriction));
							if ((elev[x][y + 1] - amt) < bedrock[x][y + 1] || y + 1 == ymax) amt = 0;
							if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
							//if (amt > erodetot2 / 2) amt = erodetot2 / 2;
							elev_update += amt;
							elev[x][y + 1] -= amt;
							slide_GS(x, y + 1, amt, x, y);
						}
					}

					elev[x][y] += elev_update;
				}
			}
		}
	}


	// now calculate sediment outputs from all four edges...
	for (int y = 2; y < ymax; y++)
	{
		if (water_depth[xmax][y] > water_depth_erosion_threshold || Vsusptot[xmax][y] > 0)
		{
			for (int n = 1; n <= 9; n++)
			{
				if (isSuspended[n])
				{
					gtot2[n] += Vsusptot[xmax][y];
					Vsusptot[xmax][y] = 0;
				}
				else
				{
					gtot2[n] += sr[xmax - 1][y][n];
				}
			}
		}
		if (water_depth[1][y] > water_depth_erosion_threshold || Vsusptot[1][y] > 0)
		{
			for (int n = 1; n <= 9; n++)
			{
				if (isSuspended[n])
				{
					gtot2[n] += Vsusptot[1][y];
					Vsusptot[1][y] = 0;
				}
				else
				{
					gtot2[n] += sl[2][y][n];
				}
			}
		}
	}

	for (int x = 2; x < xmax; x++)
	{
		if (water_depth[x][ymax] > water_depth_erosion_threshold || Vsusptot[x][ymax] > 0)
		{
			for (int n = 1; n <= 9; n++)
			{
				if (isSuspended[n])
				{
					gtot2[n] += Vsusptot[x][ymax];
					Vsusptot[x][ymax] = 0;
				}
				else
				{
					gtot2[n] += sd[x][ymax - 1][n];
				}
			}
		}
		if (water_depth[x][1] > water_depth_erosion_threshold || Vsusptot[x][1] > 0)
		{
			for (int n = 1; n <= 9; n++)
			{
				if (isSuspended[n])
				{
					gtot2[n] += Vsusptot[x][1];
					Vsusptot[x][1] = 0;
				}
				else
				{
					gtot2[n] += su[x][2][n];
				}
			}
		}
	}

	/// now update files for outputing sediment and re-circulating...
	/// 

	sediQ = 0;
	for (int n = 1; n <= G_MAX; n++)
	{
		if (temp_grain[n] < 0) temp_grain[n] = 0;
		if (recirculate_opt == true && reach_mode_opt == true)
			temp_grain[n] += gtot2[n] * recirculate_proportion; // important to divide input by time factor, so it can be reduced if re-circulating too much...
		sediQ += gtot2[n] * DX * DX;
		globalsediq += gtot2[n] * DX * DX;
		sum_grain[n] += gtot2[n] * DX * DX; // Gez
	}

	return tempbmax;
}

// Does the lateral erosion
void LSDCatchmentModel::lateral3()
{
	//double[,] edge_temp, edge_temp2, water_depth2;
	//int[,] upscale, upscale_edge;
	
	TNT::Array2D<double> edge_temp, edge_temp2, water_depth2;
	TNT::Array2D<int> upscale, upscale_edge;

	//edge_temp = new Double[xmax + 1][ymax + 1];
	//edge_temp2 = new Double[xmax + 1][ymax + 1];
	//water_depth2 = new Double[xmax + 1][ymax + 1];
	//upscale = new int[(xmax + 1) * 2][(ymax + 1) * 2];
	//upscale_edge = new int[(xmax + 1) * 2][(ymax + 1) * 2];


	// first make water depth2 equal to water depth then remove single wet cells frmo water depth2 that have an undue influence..
	double mft = 0.1;// water_depth_erosion_threshold;//MIN_Q;// vel_dir threshold
	for (int y = 2; y < ymax; y++)
	{

		int inc = 1;
		while (down_scan[y][inc] > 0)
		{
			int x = down_scan[y][inc];

			edge_temp[x][y] = 0;
			if (x == 1) x++;
			if (x == xmax) x--;
			inc++;

			if (Tau[x][y] > mft)
			{
				water_depth2[x][y] = Tau[x][y];
				int tempcounter = 0;
				for (int dir = 1; dir <= 8; dir++)
				{
					int x2, y2;
					x2 = x + deltaX[dir];
					y2 = y + deltaY[dir];
					if (Tau[x2][y2] < mft) tempcounter++;
				}
				if (tempcounter > 6) water_depth2[x][y] = 0;
			}
		}
	}

	// first determine which cells are at the edge of the channel

	for (int y = 2; y < ymax; y++)
	{

		for (int x = 2; x < xmax; x++)
		{

			edge[x][y] = -9999;

			if (water_depth2[x][y] < mft)
			{
				// if water depth < threshold then if its next to a wet cell then its an edge cell
				if (water_depth2[x][y - 1] > mft ||
					water_depth2[x - 1][y] > mft ||
					water_depth2[x + 1][y] > mft ||
					water_depth2[x][y + 1] > mft)
				{
					edge[x][y] = 0;
				}

				// unless its a dry cell surrounded by wet...
				if (water_depth2[x][y - 1] > mft &&
					water_depth2[x - 1][y] > mft &&
					water_depth2[x + 1][y] > mft &&
					water_depth2[x][y + 1] > mft)
				{
					edge[x][y] = -9999;
					edge2[x][y] = -9999;
				}

				// then update upscaled grid..
				upscale[(x * 2)][(y * 2)] = 0; // if dry
				upscale[(x * 2)][(y * 2) - 1] = 0;
				upscale[(x * 2) - 1][(y * 2)] = 0;
				upscale[(x * 2) - 1][(y * 2) - 1] = 0;
			}

			// update upscaled grid with wet cells (if wet)
			if (water_depth2[x][y] >= mft)
			{
				upscale[(x * 2)][(y * 2)] = 1; // if wet
				upscale[(x * 2)][(y * 2) - 1] = 1;
				upscale[(x * 2) - 1][(y * 2)] = 1;
				upscale[(x * 2) - 1][(y * 2) - 1] = 1;
			}
		}
	}


	// now determine edge cells on the new grid..

	for (int y = 2; y < ymax * 2; y++)
	{
		for (int x = 2; x < xmax * 2; x++)
		{
			upscale_edge[x][y] = 0;
			if (upscale[x][y] == 0)
			{
				if (upscale[x][y - 1] == 1 ||
					upscale[x - 1][y] == 1 ||
					upscale[x + 1][y] == 1 ||
					upscale[x][y + 1] == 1)
				{
					upscale[x][y] = 2;
				}
			}
		}
	}

	// now tall up inside and outside on upscaled grid


	for (int y = 2; y < ymax * 2; y++)
	{
		for (int x = 2; x < xmax * 2; x++)
		{
			if (upscale[x][y] == 2)
			{
				int wetcells = 0;
				int drycells = 0;
				int water = 0;
				int edge_cell_counter = 1;

				// sum up dry cells and edge cells - 
				// now manhattan neighbors
				for (int dir = 1; dir <= 7; dir += 2)
				{
					int x2, y2;
					x2 = x + deltaX[dir];
					y2 = y + deltaY[dir];

					if (upscale[x2][y2] == 1) wetcells += 1;
					if (upscale[x2][y2] == 0) drycells += 1;
					if (upscale[x2][y2] == 2) edge_cell_counter += 1;
				}

				if (edge_cell_counter > 3) drycells += edge_cell_counter - 2;
				//						
				water = wetcells - drycells;
				upscale_edge[x][y] = water;
			}
		}
	}

	// now update normal edge array..

	for (int x = 1; x <= xmax; x++)
	{
		for (int y = 1; y <= ymax; y++)
		{
			if (edge[x][y] == 0)
			{
				edge[x][y] = (double)(upscale_edge[(x * 2)][(y * 2)] +
					upscale_edge[(x * 2)][(y * 2) - 1] +
					upscale_edge[(x * 2) - 1][(y * 2)] +
					upscale_edge[(x * 2) - 1][(y * 2) - 1]);
				if (edge[x][y] > 2) edge[x][y] = 2; // important line to stop too great inside bends...
				if (edge[x][y] < -2) edge[x][y] = -2;

			}
		}
	}

	//then apply a smoothing filter over the top of this. here its done X number of times - 

	//double smoothing_times = double.Parse(avge_smoothbox.Text);
	//double downstream_shift = double.Parse(downstreamshiftbox.Text);

	for (int n = 1; n <= smoothing_times+downstream_shift; n++)
	{
		//var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
		//Parallel.For(2, ymax, options, delegate(int y)
		for (int y=2; y<ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0)
			{
				int x = down_scan[y][inc];

				edge_temp[x][y] = 0;
				if (x == 1) x++;
				if (x == xmax) x--;
				if (y == 1) y++;
				if (y == ymax) y--;
				inc++;

				if (edge[x][y] > -9999)
				{
					double mean = 0;
					double num = 0;
					double water_flag = 0;


					// add in cell itself..
					mean += edge[x][y];
					num++;


					for (int dir = 1; dir <= 8; dir++)
					{
						int x2, y2;
						x2 = x + deltaX[dir];
						y2 = y + deltaY[dir];
						if (water_depth2[x2][y2] > mft) water_flag++;

						if ( n > smoothing_times && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft && mean_ws_elev(x2,y2)>mean_ws_elev(x,y))
						{
							//now to mean manhattan neighbours - only if they share a wet diagonal neighbour
							if ((std::abs(deltaX[dir]) + std::abs(deltaY[dir])) != 2)
							{
								if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
									(water_depth2[x + 1][y - 1] > mft ||
									water_depth2[x + 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
									(water_depth2[x + 1][y + 1] > mft ||
									water_depth2[x - 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
									(water_depth2[x - 1][y - 1] > mft ||
									water_depth2[x - 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
									(water_depth2[x - 1][y - 1] > mft ||
									water_depth2[x + 1][y - 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
							}
							//now non manahttan neighbours, with concected by a dry cell checked..
							else
							{
								if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
									(water_depth2[x][y - 1] < mft ||
									water_depth2[x - 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
									(water_depth2[x][y - 1] < mft ||
									water_depth2[x + 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
									(water_depth2[x + 1][y] < mft ||
									water_depth2[x][y + 1] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
									(water_depth2[x][y + 1] < mft ||
									water_depth2[x - 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
							}
						}

						else if ( n <= smoothing_times && edge[x2][y2] > -9999 && water_depth2[x2][y2] < mft)
						{
							//now to mean manhattan neighbours - only if they share a wet diagonal neighbour
							if ((std::abs(deltaX[dir]) + std::abs(deltaY[dir])) != 2)
							{
								if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
									(water_depth2[x + 1][y - 1] > mft ||
									water_depth2[x + 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
									(water_depth2[x + 1][y + 1] > mft ||
									water_depth2[x - 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
									(water_depth2[x - 1][y - 1] > mft ||
									water_depth2[x - 1][y + 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
									(water_depth2[x - 1][y - 1] > mft ||
									water_depth2[x + 1][y - 1] > mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
							}
							//now non manahttan neighbours, with concected by a dry cell checked..
							else
							{
								if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
									(water_depth2[x][y - 1] < mft ||
									water_depth2[x - 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
									(water_depth2[x][y - 1] < mft ||
									water_depth2[x + 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
									(water_depth2[x + 1][y] < mft ||
									water_depth2[x][y + 1] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
								if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
									(water_depth2[x][y + 1] < mft ||
									water_depth2[x - 1][y] < mft))
								{
									mean += (edge[x + deltaX[dir]][y + deltaY[dir]]);
									num++;
								}
							}
						}
					}
					if (mean != 0) edge_temp[x][y] = mean / num;

					// removes too many cells - islands etc..

					//if(num>5&&edge[x,y]>0)edge_temp[x,y]=0;
					//if(num+water_flag>7&&edge[x,y]>0)edge_temp[x,y]=0;

					//remove edge effects
					if (x < 3 || x > (xmax - 3)) edge_temp[x][y] = 0;
					if (y < 3 || y > (ymax - 3)) edge_temp[x][y] = 0;

				}
			}
		}//);

		for (int y = 2; y < ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0)
			{
				int x = down_scan[y][inc];
				//if (x == 1) x++;
				//if (x == xmax) x--;
				inc++;
				if (edge[x][y] > -9999)
				{
					edge[x][y] = edge_temp[x][y];
				}
			}
		}
	}


	// trial line to remove too high inside bends,,
	for (int x = 1; x <= xmax; x++)
	{
		for (int y = 1; y <= ymax; y++)
		{
			if (edge[x][y] > -9999)
			{
				if (edge[x][y] > 0) edge[x][y] = 0;
				//if (edge[x][y] < -0.25) edge[x][y] = -0.25;
				edge[x][y] = 0 - edge[x][y];
				edge[x][y] = 1 / ((2.131 * std::pow(edge[x][y], -1.0794)) * DX);
				//if (edge[x][y] > (1 / (DX * 3))) edge[x][y] = 1 / (DX * 3);
				//edge[x][y] = 1 / edge[x][y]; 
				
			}
			if (water_depth[x][y] > water_depth_erosion_threshold && edge[x][y] == -9999) edge[x][y] = 0;
		}
	}

	//// now smooth across the channel..
	double tempdiff = 0;
	double counter = 0;
	do
	{
		counter++;
		//var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
		//Parallel.For(2, ymax, options, delegate(int y)
		for (int y = 2; y < ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0)
			{
				int x = down_scan[y][inc];

				edge_temp[x][y] = 0;
				if (x == 1) x++;
				if (x == xmax) x--;
				inc++;
				if (water_depth2[x][y] > mft && edge[x][y] == -9999) edge[x][y] = 0;

				if (edge[x][y] > -9999 && water_depth2[x][y] > mft)
				{
					double mean = 0;
					int num = 0;
					for (int dir = 1; dir <= 8; dir+=2)
					{
						int x2, y2;
						x2 = x + deltaX[dir];
						y2 = y + deltaY[dir];

						if (water_depth2[x2][y2] > mft && edge[x2][y2] == -9999) edge[x2][y2] = 0;
						if (edge[x2][y2] > -9999)
						{
							mean += (edge[x2][y2]);
							num++;
						}
					}
					edge_temp[x][y] = mean / num;
				}
			}
		}//);

		tempdiff = 0;
		for (int y = 2; y < ymax; y++)
		{
			int inc = 1;
			while (down_scan[y][inc] > 0)
			{
				int x = down_scan[y][inc];
				if (x == 1) x++;
				if (x == xmax) x--;
				inc++;
				if (edge[x][y] > -9999 && water_depth2[x][y] > mft)
				{
					if (std::abs(edge[x][y] - edge_temp[x][y]) > tempdiff) tempdiff = std::abs(edge[x][y] - edge_temp[x][y]);
					edge[x][y] = edge_temp[x][y];
				}
			}
		}
	} while (tempdiff > lateral_cross_channel_smoothing); //this makes it loop until the averaging across the stream stabilises
	// so that the difference between the old and new values are < 0.0001
	//tempStatusPanel.Text = Convert.ToString(counter);

}	

void LSDCatchmentModel::creep(double time)
{
	/** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
	/** very important differnece here is that slide_GS() is called only if
		BOTH cells are not -9999 therfore if both have grainsize then do additions.
		this is to stop the progressive spread of selected cells upslope */

	int x,y;
	double temp;

	for(x=1;x<=xmax;x++)
	{
		for(y=1;y<=ymax;y++)
		{
			tempcreep[x][y]=0;
		}
	}


	for(x=2;x<xmax;x++)
	{
		for(y=2;y<ymax;y++)
		{
			if(elev[x][y]>bedrock[x][y])
			{
				if(elev[x][y-1]<elev[x][y]&&elev[x][y-1]> -9999)
				{
					temp = ((elev[x][y] - elev[x][y-1]) / DX) * CREEP_RATE * time / DX;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;				
					tempcreep[x][y]-=temp;
					tempcreep[x][y-1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x, y - 1);
					}
				}
				if(elev[x+1][y-1]<elev[x][y]&&elev[x+1][y-1]> -9999)
				{
					temp=((elev[x][y]-elev[x+1][y-1])/ root)*CREEP_RATE*time/ root;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x+1][y-1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x+1, y - 1);
					}
	
				}
				if(elev[x+1][y]<elev[x][y]&&elev[x+1][y]> -9999)
				{
					temp=((elev[x][y]-elev[x+1][y])/ DX)*CREEP_RATE*time/ DX;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x+1][y]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x+1, y);
					}
				}
				if(elev[x+1][y+1]<elev[x][y]&&elev[x+1][y+1]> -9999)
				{
					temp=((elev[x][y]-elev[x+1][y+1])/ root)*CREEP_RATE*time/ root;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x+1][y+1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x+1, y + 1);
					}
				}
				if(elev[x][y+1]<elev[x][y]&&elev[x][y+1]> -9999)
				{
					temp=((elev[x][y]-elev[x][y+1])/ DX)*CREEP_RATE*time/ DX;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x][y+1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x, y + 1);
					}
				}
				if(elev[x-1][y+1]<elev[x][y]&&elev[x-1][y+1]> -9999)
				{
					temp=((elev[x][y]-elev[x-1][y+1])/ root)*CREEP_RATE*time/ root;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x-1][y+1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x-1, y + 1);
					}
				}
				if(elev[x-1][y]<elev[x][y]&&elev[x-1][y]> -9999)
				{
					temp=((elev[x][y]-elev[x-1][y])/ DX)*CREEP_RATE*time/ DX;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x-1][y]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x-1, y );
					}
				}
				if(elev[x-1][y-1]<elev[x][y]&&elev[x-1][y-1]> -9999)
				{
					temp=((elev[x][y]-elev[x-1][y-1])/ root)*CREEP_RATE*time/ root;
					if((elev[x][y]-temp)<bedrock[x][y])temp=elev[x][y]-bedrock[x][y];
					if(temp<0)temp=0;
					tempcreep[x][y]-=temp;
					tempcreep[x-1][y-1]+=temp;
					if(index[x][y]!=-9999)
					{
						slide_GS(x, y, temp, x-1, y - 1);
					}
				}
			}
		}
	}

	for(x=1;x<=xmax;x++)
	{
		for(y=1;y<=ymax;y++)
		{
			elev[x][y]+=tempcreep[x][y];
		}
	}

}	


void LSDCatchmentModel::slide_3()
{
	int x,y,inc;
	double wet_factor;
	double factor=std::tan((failureangle*(3.141592654/180)))*DX;
	double diff=0;

	for(y=2;y<ymax;y++)
	{
		inc=1;
		while(down_scan[y][inc]>0)
		{
			x=down_scan[y][inc];
			if(x==xmax)x=xmax-1;
			if(x==1)x=2;

			inc++;	
			/** check to see if under water **/
			wet_factor=factor;
			//if(water_depth[x][y]>0.01)wet_factor=factor/2;
			if(elev[x][y]<=(bedrock[x][y]+active))wet_factor=10000;

			/** chexk landslides in channel slowly */

			if(((elev[x][y]-elev[x+1][y+1])/1.41)>wet_factor&&elev[x+1][y+1]> -9999)
			{
				diff=((elev[x][y]-elev[x+1][y+1])/1.41)-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x+1][y+1]+=diff;
				slide_GS(x,y,diff,x+1,y+1);
			}
			if((elev[x][y]-elev[x][y+1])>wet_factor&&elev[x][y+1]> -9999)
			{
				diff=(elev[x][y]-elev[x][y+1])-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x][y+1]+=diff;
				slide_GS(x,y,diff,x,y+1);
			}
			if(((elev[x][y]-elev[x-1][y+1])/1.41)>wet_factor&&elev[x-1][y+1]> -9999)
			{
				diff=((elev[x][y]-elev[x-1][y+1])/1.41)-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x-1][y+1]+=diff;
				slide_GS(x,y,diff,x-1,y+1);
			}
			if((elev[x][y]-elev[x-1][y])>wet_factor&&elev[x-1][y]> -9999)
			{
				diff=(elev[x][y]-elev[x-1][y])-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x-1][y]+=diff;
				slide_GS(x,y,diff,x-1,y);
			}	

			if(((elev[x][y]-elev[x-1][y-1])/1.41)>wet_factor&&elev[x-1][y-1]> -9999)
			{
				diff=((elev[x][y]-elev[x-1][y-1])/1.41)-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x-1][y-1]+=diff;
				slide_GS(x,y,diff,x-1,y-1);
			}
			if((elev[x][y]-elev[x][y-1])>wet_factor&&elev[x][y-1]> -9999)
			{
				diff=(elev[x][y]-elev[x][y-1])-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x][y-1]+=diff;
				slide_GS(x,y,diff,x,y-1);
			}
			if(((elev[x][y]-elev[x+1][y-1])/1.41)>wet_factor&&elev[x+1][y-1]> -9999)
			{
				diff=((elev[x][y]-elev[x+1][y-1])/1.41)-wet_factor;
				if((elev[x][y]-diff)<(bedrock[x][y]+active))diff=(elev[x][y]-(bedrock[x][y]+active));
				elev[x][y]-=diff;
				elev[x+1][y-1]+=diff;
				slide_GS(x,y,diff,x+1,y-1);
			}

			if ((elev[x][y] - elev[x + 1][y]) > wet_factor && elev[x + 1][y] > -9999)
			{
				diff = (elev[x][y] - elev[x + 1][y]) - wet_factor;
				if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
				elev[x][y] -= diff;
				elev[x + 1][y] += diff;
				slide_GS(x, y, diff, x + 1, y);
			}
	
		}
	}
	
}

void LSDCatchmentModel::slide_5()
{
	int x, y, inc=0;
	double wet_factor;
	double factor = std::tan((failureangle * (3.141592654 / 180))) * DX;
	//if(landslidesBox.Checked == true) factor = DX * ((-265000 * j_mean) + 1.38);
	double diff = 0;
	double total = 0;

	if (dunes_opt == true)
	{
		for (x = 1; x <= xmax; x++)
		{
			for (y = 1; y <= ymax; y++)
			{
				elev[x][y] -= sand[x][y];
			}
		}
	}


	do
	{
		total = 0;
		inc++;
		for (y = 2; y < ymax; y++)
		{
			for (x = 2; x < xmax; x++)
			{

				wet_factor = factor;
				//if(water_depth[x,y]>0.01)wet_factor=factor/2;
				if (elev[x][y] <= (bedrock[x][y] + active)) wet_factor = 10 * DX;

				/** chexk landslides in channel slowly */

				if (((elev[x][y] - elev[x + 1][y + 1]) / 1.41) > wet_factor && elev[x + 1][y + 1]> -9999)
				{
					diff = ((elev[x][y] - elev[x + 1][y + 1]) / 1.41) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x + 1][y + 1] += diff;
					total += diff;
				}
				if ((elev[x][y] - elev[x][y + 1]) > wet_factor && elev[x][y + 1]> -9999)
				{
					diff = (elev[x][y] - elev[x][y + 1]) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x][y + 1] += diff;
					total += diff;
				}
				if (((elev[x][y] - elev[x - 1][y + 1]) / 1.41) > wet_factor && elev[x - 1][y + 1]> -9999)
				{
					diff = ((elev[x][y] - elev[x - 1][y + 1]) / 1.41) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x - 1][y + 1] += diff;
					total += diff;
				}
				if ((elev[x][y] - elev[x - 1][y]) > wet_factor && elev[x - 1][y]> -9999)
				{
					diff = (elev[x][y] - elev[x - 1][y]) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x - 1][y] += diff;
					total += diff;
				}

				if (((elev[x][y] - elev[x - 1][y - 1]) / 1.41) > wet_factor && elev[x - 1][y - 1]> -9999)
				{
					diff = ((elev[x][y] - elev[x - 1][y - 1]) / 1.41) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x - 1][y - 1] += diff;
					total += diff;
				}
				if ((elev[x][y] - elev[x][y - 1]) > wet_factor && elev[x][y - 1]> -9999)
				{
					diff = (elev[x][y] - elev[x][y - 1]) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x][y - 1] += diff;
					total += diff;
				}
				if (((elev[x][y] - elev[x + 1][y - 1]) / 1.41) > wet_factor && elev[x + 1][y - 1]> -9999)
				{
					diff = ((elev[x][y] - elev[x + 1][y - 1]) / 1.41) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x + 1][y - 1] += diff;
					total += diff;
				}



				if ((elev[x][y] - elev[x + 1][y]) > wet_factor && elev[x + 1][y]> -9999)
				{
					diff = (elev[x][y] - elev[x + 1][y]) - wet_factor;
					if ((elev[x][y] - diff) < (bedrock[x][y] + active)) diff = (elev[x][y] - (bedrock[x][y] + active));
					if (diff > ERODEFACTOR) diff = ERODEFACTOR;
					elev[x][y] -= diff;
					elev[x + 1][y] += diff;
					total += diff;
				}

			}
		}
	} while (total > 0 && inc<200);

	if (dunes_opt == true)
	{
		for (x = 1; x <= xmax; x++)
		{
			for (y = 1; y <= ymax; y++)
			{
				elev[x][y] += sand[x][y];
			}
		}
	}

}

void LSDCatchmentModel::soil_erosion(double time)
{
	/** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
	/** very important differnece here is that slide_GS() is called only if
		BOTH cells are not -9999 therfore if both have grainsize then do additions.
		this is to stop the progressive spread of selected cells upslope */
		
	int x, y;
	double temp;


	for (x = 1; x <= xmax; x++)
	{
		for (y = 1; y <= ymax; y++)
		{
			tempcreep[x][y] = 0;
		}
	}


	for (x = 2; x < xmax; x++)
	{
		for (y = 2; y < ymax; y++)
		{
			if (elev[x][y] > bedrock[x][y])
			{
				if (elev[x][y - 1] < elev[x][y] && elev[x][y - 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x][y - 1]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y]*DX*DX,0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x][y - 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x, y - 1);
					}
				}
				if (elev[x + 1][y - 1] < elev[x][y] && elev[x + 1][y - 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x + 1][y - 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x + 1][y - 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x + 1, y - 1);
					}

				}
				if (elev[x + 1][y] < elev[x][y] && elev[x + 1][y] > -9999)
				{
					temp = ((elev[x][y] - elev[x + 1][y]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x + 1][y] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x + 1, y);
					}
				}
				if (elev[x + 1][y + 1] < elev[x][y] && elev[x + 1][y + 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x + 1][y + 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x + 1][y + 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x + 1, y + 1);
					}
				}
				if (elev[x][y + 1] < elev[x][y] && elev[x][y + 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x][y + 1]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x][y + 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x, y + 1);
					}
				}
				if (elev[x - 1][y + 1] < elev[x][y] && elev[x - 1][y + 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x - 1][y + 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x - 1][y + 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x - 1, y + 1);
					}
				}
				if (elev[x - 1][y] < elev[x][y] && elev[x - 1][y] > -9999)
				{
					temp = ((elev[x][y] - elev[x - 1][y]) / DX) * SOIL_RATE * time / DX * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x - 1][y] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x - 1, y);
					}
				}
				if (elev[x - 1][y - 1] < elev[x][y] && elev[x - 1][y - 1] > -9999)
				{
					temp = ((elev[x][y] - elev[x - 1][y - 1]) / root) * SOIL_RATE * time / root * std::pow(area[x][y] * DX * DX, 0.5);
					if ((elev[x][y] - temp) < bedrock[x][y]) temp = elev[x][y] - bedrock[x][y];
					if (temp < 0) temp = 0;
					tempcreep[x][y] -= temp;
					tempcreep[x - 1][y - 1] += temp;
					if (index[x][y] != -9999)
					{
						slide_GS(x, y, temp, x - 1, y - 1);
					}
				}
			}
		}
	}

	for (x = 1; x <= xmax; x++)
	{
		for (y = 1; y <= ymax; y++)
		{
			elev[x][y] += tempcreep[x][y];
		}
	}
}

// all based on Van Walleghem et al., 2013 (JGR:ES)
void LSDCatchmentModel::soil_development()
{
	for (int x = 1; x <= xmax; x++)
	{
		for (int y = 1; y <= ymax; y++)
		{
			if (elev[x][y] > -9999) // ensure it is not a no-data point
			{
				if (index[x][y] == -9999) addGS(x, y); // first ensure that there is grainsize defined for that cell

				if (bedrock_lower_opt == true) // Bedrock lowering
				{
					if (bedrock[x][y] > -9999 && elev[x][y]>=bedrock[x][y])
					{
						double h = elev[x][y] - bedrock[x][y];
						if (h == 0) h = 0.001;
						bedrock[x][y] += - P1 * std::exp(-b1 * (h)) / 12; //  / 12 to make it months
					}
				}

				if (physical_weather_opt == true) // Physical Weathering
				{
					//amt = k1 * std::exp(-c1*depth_below_surface) * (c2/std::log10(particle_size) * time;
					// start from top then work down - means no need to hold values in temp arrays...
					double Di = 0;
					int xyindex = index[x][y];


					for (int n = 2; n <= (G_MAX - 1); n++)
					{
						if ((grain[xyindex][n] > 0.0))
						{
							switch (n)
							{
								case 1: Di = d1; break;
								case 2: Di = d2; break;
								case 3: Di = d3; break;
								case 4: Di = d4; break;
								case 5: Di = d5; break;
								case 6: Di = d6; break;
								case 7: Di = d7; break;
								case 8: Di = d8; break;
								case 9: Di = d9; break;
							}

							double amount = grain[xyindex][n] * ((-(k1 * std::exp(-c1 * active * 0.5) * (c2 / std::log(Di * 0.001)) * 1)) / 12); //  / 12 to make it months
							grain[xyindex][n] -= amount;
							if (n == 2)
							{
								grain[xyindex][n - 1] += amount;
							}
							else
							{
								grain[xyindex][n - 1] += amount * 0.05;
								grain[xyindex][n - 2] += amount * 0.95;
							}

							for (int z = 1; z <= 9; z++)
							{
								double amount2 = strata[xyindex][z - 1][n] * ((-(k1 * std::exp(-c1 * active * z) * (c2 / std::log(Di * 0.001)) * 1)) / 12); //  / 12 to make it months

								strata[xyindex][z-1][n] -= amount;
								if (n == 2)
								{
									strata[xyindex][z-1][n - 1] += amount;
								}
								else
								{
									strata[xyindex][z-1][n - 1] += amount * 0.05;
									strata[xyindex][z-1][n - 2] += amount * 0.95;
								}

   
							}
						}
					}

					//for (int z = 9; z >= 1; z--)
					//{
					//    for (int n = 0; n <= G_MAX - 2; n++)
					//    {

					//        strata[xyindex][z][n] = strata[xyindex][z - 1][n];

					//    }
					//}

				}

				if (chem_weath_opt == true) // Chemical Weathering
				{
					//amt = k2 * std::exp(-c3 * depth_below_surface) * c4 * Specific_surface_area * time;
				}
			}
		}
	}
}
