//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDRaster.cpp
// cpp file for the LSDRaster object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
// David T. Milodowski, University of Edinburgh
// Martin D. Hurst, British Geological Survey
// Fiona Clubb, University of Edinburgh
// Stuart Grieve, University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Version 1.0.0		16/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// change log
// MASSIVE MERGE: Starting version 1.0.0 on 15/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <queue>
#include <math.h>
#include <string.h>
#include "TNT/tnt.h"
#include "TNT/jama_lu.h"
#include "TNT/jama_eig.h"
#include "LSDRaster.hpp"
#include "LSDStatsTools.hpp"
#include "LSDIndexRaster.hpp"
using namespace std;
using namespace TNT;
using namespace JAMA;

#ifndef LSDRaster_CPP
#define LSDRaster_CPP


// operators
LSDRaster& LSDRaster::operator=(const LSDRaster& rhs)
 {
  if (&rhs != this)
   {
    create(rhs.get_NRows(),rhs.get_NCols(),rhs.get_XMinimum(),rhs.get_YMinimum(),
           rhs.get_DataResolution(),rhs.get_NoDataValue(),rhs.get_RasterData());
   }
  return *this;
 }

// the create function. This is default and throws an error
void LSDRaster::create()
{
	cout << "LSDRaster line 64 Warning you have an empty LSDRaster!" << endl;
	//exit(EXIT_FAILURE);
}

// this creates a raster using an infile
void LSDRaster::create(string filename, string extension)
{
	read_raster(filename,extension);
}

// this creates a raster filled with no data values
void LSDRaster::create(int nrows, int ncols, double xmin, double ymin,
            double cellsize, double ndv, Array2D<double> data)
{
	NRows = nrows;
	NCols = ncols;
	XMinimum = xmin;
	YMinimum = ymin;
	DataResolution = cellsize;
	NoDataValue = ndv;

	RasterData = data.copy();

	if (RasterData.dim1() != NRows)
	{
		cout << "LSDRaster line 89 dimension of data is not the same as stated in NRows!" << endl;
		exit(EXIT_FAILURE);
	}
	if (RasterData.dim2() != NCols)
	{
		cout << "LSDRaster line 94 dimension of data is not the same as stated in NRows!" << endl;
		exit(EXIT_FAILURE);
	}

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function reads a DEM
// One has to provide both the filename and the extension
// the '.' between the filename and extension is not included
// for example, if the full filename is test.asc
// then
// filename = "test"
// and
// ext = "asc"
// The full filename coult also be "test.01.asc"
// so filename would be "test.01"
// and ext would again be "asc"
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::read_raster(string filename, string extension)
{
	string string_filename;
	string dot = ".";
	string_filename = filename+dot+extension;
	cout << "The filename is " << string_filename << endl;


	if (extension == "asc")
	{
		// open the data file
		ifstream data_in(string_filename.c_str());

		//Read in raster data
		string str;			// a temporary string for discarding text

		// read the georeferencing data and metadata
		data_in >> str >> NCols >> str >> NRows
			    >> str >> XMinimum >> str >> YMinimum
		   		>> str >> DataResolution
			    >> str >> NoDataValue;

		cout << "Loading asc file; NCols: " << NCols << " NRows: " << NRows << endl
		     << "X minimum: " << XMinimum << " YMinimum: " << YMinimum << endl
		     << "Data Resolution: " << DataResolution << " and No Data Value: "
		     << NoDataValue << endl;

		// this is the array into which data is fed
		Array2D<double> data(NRows,NCols,NoDataValue);

		// read the data
		for (int i=0; i<NRows; ++i)
		{
			for (int j=0; j<NCols; ++j)
			{
				data_in >> data[i][j];
			}
		}
		data_in.close();

		// now update the objects raster data
		RasterData = data.copy();
	}
	else if (extension == "flt")
	{
		// float data (a binary format created by ArcMap) has a header file
		// this file must be opened first
		string header_filename;
		string header_extension = "hdr";
		header_filename = filename+dot+header_extension;

		ifstream ifs(header_filename.c_str());
		if( ifs.fail() )
		{
			cout << "\nFATAL ERROR: the header file \"" << header_filename
				 << "\" doesn't exist" << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			string str;
			ifs >> str >> NCols >> str >> NRows
				>> str >> XMinimum >> str >> YMinimum
				>> str >> DataResolution
				>> str >> NoDataValue;
		}
		ifs.close();

		cout << "Loading asc file; NCols: " << NCols << " NRows: " << NRows << endl
			 << "X minimum: " << XMinimum << " YMinimum: " << YMinimum << endl
		     << "Data Resolution: " << DataResolution << " and No Data Value: "
		     << NoDataValue << endl;

		// this is the array into which data is fed
		Array2D<double> data(NRows,NCols,NoDataValue);

		// now read the DEM, using the binary stream option
		ifstream ifs_data(string_filename.c_str(), ios::in | ios::binary);
		if( ifs_data.fail() )
		{
			cout << "\nFATAL ERROR: the data file \"" << string_filename
			     << "\" doesn't exist" << endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			float temp;
			for (int i=0; i<NRows; ++i)
			{
				for (int j=0; j<NCols; ++j)
				{
					ifs_data.read(reinterpret_cast<char*>(&temp), sizeof(temp));
					data[i][j] = double(temp);
				}
			}
		}
		ifs_data.close();

		// now update the objects raster data
		RasterData = data.copy();
	}
	else
	{
		cout << "You did not enter and approprate extension!" << endl
				  << "You entered: " << extension << " options are .flt and .asc" << endl;
		exit(EXIT_FAILURE);
	}


}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// write_raster
// this function writes a raster. One has to give the filename and extension
// currently the options are for .asc and .flt files
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::write_raster(string filename, string extension)
{
	string string_filename;
	string dot = ".";
	string_filename = filename+dot+extension;
	cout << "The filename is " << string_filename << endl;

	// this first bit of logic is for the asc file.
	if (extension == "asc")
	{
		// open the data file
		ofstream data_out(string_filename.c_str());

		if( data_out.fail() )
		{
			cout << "\nFATAL ERROR: unable to write to " << string_filename << endl;
			exit(EXIT_FAILURE);
		}

		data_out <<  "ncols         " << NCols
				<< "\nnrows         " << NRows
				<< "\nxllcorner     " << setprecision(14) << XMinimum
				<< "\nyllcorner     " << setprecision(14) << YMinimum
				<< "\ncellsize      " << DataResolution
				<< "\nNODATA_value  " << NoDataValue << endl;

		for (int i=0; i<NRows; ++i)
		{
			for (int j=0; j<NCols; ++j)
			{
				data_out << setprecision(6) << RasterData[i][j] << " ";
			}
			if (i != NRows-1) data_out << endl;
		}
		data_out.close();

	}
	else if (extension == "flt")
	{
		// float data (a binary format created by ArcMap) has a header file
		// this file must be opened first
		string header_filename;
		string header_extension = "hdr";
		header_filename = filename+dot+header_extension;

		ofstream header_ofs(header_filename.c_str());
		string str;
		header_ofs <<  "ncols         " << NCols
			<< "\nnrows         " << NRows
			<< "\nxllcorner     " << setprecision(14) << XMinimum
			<< "\nyllcorner     " << setprecision(14) << YMinimum
			<< "\ncellsize      " << DataResolution
			<< "\nNODATA_value  " << NoDataValue << endl;
		header_ofs.close();

		// now do the main data
		ofstream data_ofs(string_filename.c_str(), ios::out | ios::binary);
		float temp;
		for (int i=0; i<NRows; ++i)
		{
			for (int j=0; j<NCols; ++j)
			{
				temp = float(RasterData[i][j]);
				data_ofs.write(reinterpret_cast<char *>(&temp),sizeof(temp));
			}
		}
		data_ofs.close();
	}
	else
	{
		cout << "You did not enter and approprate extension!" << endl
				  << "You entered: " << extension << " options are .flt and .asc" << endl;
		exit(EXIT_FAILURE);
	}


}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a hillshade raster using the algorithm outlined in
// Burrough and McDonnell Principles of GIS 1990 and in the ArcMap web help
// http://edndoc.esri.com/arcobjects/9.2/net/shared/geoprocessing/
// spatial_analyst_tools/how_hillshade_works.htm
//
// Takes 3 doubles, representing the altitude of the illumination source in
// degrees, the azimuth of the illumination source in degrees and the z factor.
//
// Default values are altitude = 45, azimuth = 315, z_factor = 1
//
// Outputs an LSDRaster object.
//
// SWDG, February 2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::hillshade(double altitude, double azimuth, double z_factor)
{
    double PI = 3.14159265;

    //print parameters to screen
    cout << "Hillshading with altitude: " << altitude
    << ", azimuth: " << azimuth << " and z-factor: " << z_factor << endl;

    //create output array
    Array2D<double> hillshade(NRows,NCols,NoDataValue);

    //convert zenith and azimuth into radians for calculation
    double zenith_rad = (90 - altitude) * PI / 180.0;
    double azimuth_math = 360-azimuth + 90;
    if (azimuth_math >= 360.0) azimuth_math = azimuth_math - 360;
    double azimuth_rad = azimuth_math * PI /180.0;

    //calculate hillshade value for every non nodata value in the input raster
    for (int i = 1; i < NRows-1; ++i){
        for (int j = 1; j < NCols-1; ++j){
            double slope_rad = 0;
            double aspect_rad = 0;
            double dzdx = 0;
            double dzdy = 0;

            if (RasterData[i][j] != NoDataValue){
                dzdx = ((RasterData[i][j+1] + 2*RasterData[i+1][j] + RasterData[i+1][j+1]) -
                       (RasterData[i-1][j-1] + 2*RasterData[i-1][j] + RasterData[i-1][j+1]))
                        / (8 * DataResolution);
                dzdy = ((RasterData[i-1][j+1] + 2*RasterData[i][j+1] + RasterData[i+1][j+1]) -
                       (RasterData[i-1][j-1] + 2*RasterData[i][j-1] + RasterData[i+1][j-1]))
                       / (8 * DataResolution);

                slope_rad = atan(z_factor * sqrt((dzdx*dzdx) + (dzdy*dzdy)));

                if (dzdx != 0){
                    aspect_rad = atan2(dzdy, (dzdx*-1));
                    if (aspect_rad < 0) aspect_rad = 2*PI + aspect_rad;
                }
                else{
                    if (dzdy > 0) aspect_rad = PI/2;
                    else if (dzdy < 0) aspect_rad = 2 * PI - PI/2;
                    else aspect_rad = aspect_rad;
                }
                hillshade[i][j] = 255.0 * ((cos(zenith_rad) * cos(slope_rad)) +
                                  (sin(zenith_rad) * sin(slope_rad) *
                                  cos(azimuth_rad - aspect_rad)));

                if (hillshade[i][j] < 0) hillshade[i][j] = 0;
            }
        }
    }
    //create LSDRaster hillshade object
    LSDRaster hillshade_raster(NRows, NCols, XMinimum, YMinimum, DataResolution,
                               NoDataValue, hillshade);

    return hillshade_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a hillshade derivative raster using the algorithm outlined in
// Codilean (2006), identifying areas in shadow as 1 and all other values as 0.
//
// Is interfaced through LSDRaster::TopoShield and should not be called directly,
// to generate a hillshade use LSDRaster::hillshade instead.
//
// Takes 2 ints, representing theta, the zenith angle of the illumination source in
// degrees and the azimuth angle, phi, of the illumination source in degrees.
//
// Outputs an Array2D of doubles.
//
// SWDG, 11/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<double> LSDRaster::Shadow(int theta, int phi)
{
    double PI = 3.14159265;

    //create array of input data and an output array of same dimensions
    Array2D<double> data = RasterData;
    Array2D<double> hillshade(NRows,NCols,NoDataValue);

    //convert zenith and azimuth into radians for calculation
    double zenith_rad = (90 - theta) * PI / 180.0;
    double azimuth_math = 360-phi + 90;
    if (azimuth_math >= 360.0) azimuth_math = azimuth_math - 360;
    double azimuth_rad = azimuth_math * PI /180.0;

    //calculate hillshade value for every non nodata value in the input raster
    for (int i = 1; i < NRows-1; ++i){
        for (int j = 1; j < NCols-1; ++j){
            double slope_rad = 0;
            double aspect_rad = 0;
            double dzdx = 0;
            double dzdy = 0;

            if (data[i][j] != NoDataValue){
                dzdx = ((data[i][j+1] + 2*data[i+1][j] + data[i+1][j+1]) -
                       (data[i-1][j-1] + 2*data[i-1][j] + data[i-1][j+1]))
                        / (8 * DataResolution);
                dzdy = ((data[i-1][j+1] + 2*data[i][j+1] + data[i+1][j+1]) -
                       (data[i-1][j-1] + 2*data[i][j-1] + data[i+1][j-1]))
                       / (8 * DataResolution);

                slope_rad = atan(sqrt((dzdx*dzdx) + (dzdy*dzdy)));

                if (dzdx != 0){
                    aspect_rad = atan2(dzdy, (dzdx*-1));
                    if (aspect_rad < 0) aspect_rad = 2*PI + aspect_rad;
                }
                else{
                    if (dzdy > 0) aspect_rad = PI/2;
                    else if (dzdy < 0) aspect_rad = 2 * PI - PI/2;
                    else aspect_rad = aspect_rad;
                }

                hillshade[i][j] = acos(((sin(zenith_rad) * cos(slope_rad)) +
                                  (sin(zenith_rad) * sin(slope_rad) *
                                  cos(azimuth_rad - aspect_rad))))*180/PI;

                if (hillshade[i][j] > 90) hillshade[i][j] = 1;
                else hillshade[i][j] = 0;
            }

        }
    }

    //create LSDRaster hillshade object
    //LSDRaster hillshade_raster(NRows, NCols, XMinimum, YMinimum, DataResolution,
    //                           NoDataValue, hillshade);

    return hillshade;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a topographic sheilding raster using the algorithm outlined in
// Codilean (2006), creating a raster of values between 0 and 1 whihc can be used as a
// scaling factor in Cosmo analysis.
//
// Goes further than the original algorithm allowing a theoretical theta, phi pair of
// 1,1 to be supplied and although this will increase the computatin time significantly,
// it is much faster than the original Avenue and VBScript implementations.
//
// Takes 2 ints, representing the theta, phi paring required. Codilean (2006) used 5,5
// as the standard values, but in reality values of 10,15 are often preferred to save
// processing time. **phi_step must be a factor of 360**
//
// Outputs an LSDRaster
//
// SWDG, 11/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::TopoShield(int theta_step, int phi_step){

  //calculate parameter of scaling calculation
  int phi_factor = 360/phi_step;

  double MaxFactor = 0;

  for(int theta = 10; theta < 90; theta += theta_step){
    MaxFactor += pow(sin(rad(theta)),3.3) * phi_factor;
  }

  //calculate maximum scaling factor and write it to an array
  MaxFactor += pow(sin(rad(90)),3.3);
  Array2D<double> MaxFactorArray(NRows,NCols,MaxFactor);

  //Calculate first shadow with theta value of 90 and scale it
  Array2D<double> Scaler90(NRows,NCols, pow(sin(rad(90)),3.3));
  Array2D<double> FinalArray = Shadow(0,90) * Scaler90;

  //loop through all the theta, phi pairs and increment the FinalArray with the scaled values
  for(int theta = 10; theta < 90; theta += theta_step){
    for(int phi = 0; phi < 360; phi += phi_step){

      cout << flush <<  "\tTheta = " << theta << ", Phi = " << phi << "           \r";

      Array2D<double> TempArray = Shadow(theta,phi);
      Array2D<double> Scaler(NRows, NCols, pow(sin(rad(theta)),3.3));
      FinalArray += TempArray * Scaler;
    }
  }

 //create array of ones needed in sheilding calculation
  Array2D<double> Ones(NRows,NCols,1);

  //Shielding factor calculation
  Array2D<double> ShFactor = Ones - (FinalArray/MaxFactorArray);

  //deal with nodata values on border of dem - more efficient than doing it in the hillshade function 10s of times.
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (ShFactor[i][j] > 10){             //arbitrary number. But don't want to miss any values >1
        ShFactor[i][j] = NoDataValue;       //as they may indicate an error in the above calculations.
      }
    }
  }

   //flush output onto a new line
   cout << endl;

  //write LSDRaster
  LSDRaster Shielding(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, ShFactor);
  return Shielding;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this looks for isolated instances of nodata and fills them
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::check_isolated_nodata()
{
	for (int row=0; row<NRows; ++row)
	{
		for(int col=0; col<NCols; ++col)
		{
			if(RasterData[row][col] < 0)
			{
				cout << "LSDRaster::check_isolated_nodata stargine data point: row: "
				     << row << " col: " << col << " data: " << RasterData[row][col];
				RasterData[row][col] = NoDataValue;
			}

			if(RasterData[row][col] == NoDataValue)
			{
				cout << "LSDRaster::check_isolated_nodata found nodata: row: "
				     << row << " col: " << col << " data: " << RasterData[row][col];
			}

		}
	}
	cout << "Done!" << endl;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// calculate_polyfit_coefficient_matrices
//
// this function calcualtes 6 coefficient matrices that allow the user to
// then calcualte slope, curvature, aspect, a classification for finding saddles and peaks
// and other metrics
//
// The coefficient matrices are overwritten during the running of this member function
//
// Updated 15/07/2013 to use a circular mask for surface fitting. DTM
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_polyfit_coefficient_matrices(double window_radius,
										Array2D<double>& a, Array2D<double>& b,
										Array2D<double>& c, Array2D<double>& d,
										Array2D<double>& e, Array2D<double>& f)
{
	// this fits a polynomial surface over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));           // Set radius of kernel
	int kw=2*kr+1;                    						// width of kernel

	Array2D<double> data_kernel(kw,kw,NoDataValue);
	Array2D<double> x_kernel(kw,kw,NoDataValue);
	Array2D<double> y_kernel(kw,kw,NoDataValue);
	Array2D<int> mask(kw,kw,0);

	// reset the a,b,c,d,e and f matrices (the coefficient matrices)
	Array2D<double> temp_coef(NRows,NCols,0.0);
	a = temp_coef.copy();
	b = temp_coef.copy();
	c = temp_coef.copy();
	d = temp_coef.copy();
	e = temp_coef.copy();
	f = temp_coef.copy();

	// scale kernel window to resolution of DEM, and translate coordinates to be
	// centred on cell of interest (the centre cell)
	double x,y,zeta,radial_dist;
	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
	      	x_kernel[i][j]=(i-kr)*DataResolution;
	      	y_kernel[i][j]=(j-kr)*DataResolution;

			// Build circular mask
			// distance from centre to this point.
			radial_dist = sqrt(y_kernel[i][j]*y_kernel[i][j] + x_kernel[i][j]*x_kernel[i][j]);

        	if (floor(radial_dist) <= window_radius)
        	{
				mask[i][j] = 1;
			}
      	}
	}

	// FIT POLYNOMIAL SURFACE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO
	// DETERMINE TOPOGRAPHIC METRICS

	// Have N simultaneous linear equations, and N unknowns.
	// => b = Ax, where x is a 1xN array containing the coefficients we need for
	// surface fitting.
	// A is constructed using different combinations of x and y, thus we only need
	// to compute this once, since the window size does not change.
	// For 2nd order surface fitting, there are 6 coefficients, therefore A is a
	// 6x6 matrix
	Array2D<double> A(6,6);
	for (int i=0; i<kw; ++i)
	{
		for (int j=0; j<kw; ++j)
		{
			if (mask[i][j] == 1)
      		{
        		x = x_kernel[i][j];
  				y = y_kernel[i][j];

  				// Generate matrix A
  				A[0][0] += pow(x,4);
  				A[0][1] += pow(x,2)*pow(y,2);
  				A[0][2] += pow(x,3)*y;
  				A[0][3] += pow(x,3);
  				A[0][4] += pow(x,2)*y;
  				A[0][5] += pow(x,2);
  				A[1][0] += pow(x,2)*pow(y,2);
  				A[1][1] += pow(y,4);
  				A[1][2] += x*pow(y,3);
  				A[1][3] += x*pow(y,2);
  				A[1][4] += pow(y,3);
  				A[1][5] += pow(y,2);
  				A[2][0] += pow(x,3)*y;
  				A[2][1] += x*pow(y,3);
  				A[2][2] += pow(x,2)*pow(y,2);
  				A[2][3] += pow(x,2)*y;
  				A[2][4] += x*pow(y,2);
  				A[2][5] += x*y;
  				A[3][0] += pow(x,3);
  				A[3][1] += x*pow(y,2);
  				A[3][2] += pow(x,2)*y;
  				A[3][3] += pow(x,2);
  				A[3][4] += x*y;
  				A[3][5] += x;
  				A[4][0] += pow(x,2)*y;
  				A[4][1] += pow(y,3);
  				A[4][2] += x*pow(y,2);
  				A[4][3] += x*y;
  				A[4][4] += pow(y,2);
  				A[4][5] += y;
  				A[5][0] += pow(x,2);
  				A[5][1] += pow(y,2);
  				A[5][2] += x*y;
  				A[5][3] += x;
  				A[5][4] += y;
  				A[5][5] += 1;
			}
		}
	}

	// Move window over DEM, fitting 2nd order polynomial surface to the
	// elevations within the window.
	cout << "\n\tRunning 2nd order polynomial fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;

	for(int i=0;i<NRows;++i)
	{
		cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for(int j=0;j<NCols;++j)
		{
			// Avoid edges
			if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols))
			{
				a[i][j] = NoDataValue;
				b[i][j] = NoDataValue;
				c[i][j] = NoDataValue;
				d[i][j] = NoDataValue;
				e[i][j] = NoDataValue;
				f[i][j] = NoDataValue;
			}
			// Avoid nodata values
			else if(RasterData[i][j]==NoDataValue)
			{
				a[i][j] = NoDataValue;
				b[i][j] = NoDataValue;
				c[i][j] = NoDataValue;
				d[i][j] = NoDataValue;
				e[i][j] = NoDataValue;
				f[i][j] = NoDataValue;
			}
			else
			{
				// clip DEM
				//zeta_sampler=zeta.copy();
				for(int i_kernel=0;i_kernel<kw;++i_kernel)
				{
			  		for(int j_kernel=0;j_kernel<kw;++j_kernel)
			  		{
						data_kernel[i_kernel][j_kernel] =
									RasterData[i-kr+i_kernel][j-kr+j_kernel];
						// check for nodata values nearby
						if(data_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
			  		}
				}

				// Fit polynomial surface, avoiding nodata values
				if(ndv_present == 0)  // test for nodata values within the selection
				{
					Array1D<double> bb(6,0.0);
					Array1D<double> coeffs(6);
					for (int krow=0; krow<kw; ++krow)
					{
						for (int kcol=0; kcol<kw; ++kcol)
						{
							if (mask[krow][kcol] == 1)
              				{
                				x = x_kernel[krow][kcol];
					      		y = y_kernel[krow][kcol];
					      		zeta = data_kernel[krow][kcol];
					      		// Generate vector bb
					      		bb[0] += zeta*x*x;
					      		bb[1] += zeta*y*y;
					      		bb[2] += zeta*x*y;
					      		bb[3] += zeta*x;
					      		bb[4] += zeta*y;
					      		bb[5] += zeta;
					      	}		// end mask
            			}			// end kernal column
					}				// end kernal row
					// Solve matrix equations using LU decomposition using the TNT JAMA package:
					// A.coefs = b, where coefs is the coefficients vector.
					LU<double> sol_A(A);  // Create LU object
					coeffs = sol_A.solve(bb);

			  		a[i][j]=coeffs[0];
			  		b[i][j]=coeffs[1];
			  		c[i][j]=coeffs[2];
			  		d[i][j]=coeffs[3];
			  		e[i][j]=coeffs[4];
			  		f[i][j]=coeffs[5];
				}					// end if statement for no data value
				ndv_present = 0;
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the elevation based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
// added by FC 24/03/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_elevation(Array2D<double>& f)
{
	// create the new elevation raster
	Array2D<double> elevation_data(NRows,NCols,NoDataValue);

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (f[row][col] != NoDataValue)
			{
				elevation_data[row][col] = f[row][col];
			}
		}
	}


	LSDRaster elevation_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,elevation_data);
	return elevation_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the slope based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_slope(Array2D<double>& d, Array2D<double>& e)
{
	// create the new slope raster
	Array2D<double> slope_data(NRows,NCols,NoDataValue);

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				slope_data[row][col] = sqrt(d[row][col]*d[row][col]+e[row][col]*e[row][col]);
			}
		}
	}


	LSDRaster slope_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,slope_data);
	return slope_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the aspect based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_aspect(Array2D<double>& d, Array2D<double>& e)
{
	// create the new slope raster
	Array2D<double> aspect_data(NRows,NCols,NoDataValue);

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				if(d[row][col]==0 || e[row][col]==0)
				{
					aspect_data[row][col] = NoDataValue;
				}
				else
				{
					aspect_data[row][col] = 180 - 57.29578*atan(e[row][col]/d[row][col])
					                            + 90*(d[row][col]/abs(d[row][col]));
					if(aspect_data[row][col] < 180.0)
					{
						aspect_data[row][col] = 180.0 - aspect_data[row][col];
					}
					else
					{
						aspect_data[row][col] = 360.0 + (180 - aspect_data[row][col]);
					}
				}
			}
		}
	}

	LSDRaster aspect_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,aspect_data);
	return aspect_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_curvature(Array2D<double>& a, Array2D<double>& b)
{
	// create the new slope raster
	Array2D<double> curvature_data(NRows,NCols,NoDataValue);

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (a[row][col] != NoDataValue)
			{
				curvature_data[row][col] = 2*a[row][col]+2*b[row][col];
			}
		}
	}


	LSDRaster curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,curvature_data);
	return curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the planform curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by DM and FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_planform_curvature(Array2D<double>& a, Array2D<double>& b,
                                                          Array2D<double>& c, Array2D<double>& d,
                                                          Array2D<double>& e)
{
	// create the new planform curvature raster
	Array2D<double> pl_curvature_data(NRows,NCols,NoDataValue);
  	double fx, fy, fxx, fyy, fxy, p, q;

	for (int row = 0; row<NRows; row++)
	{

		for(int col = 0; col<NCols; col++)
		{

			if (a[row][col] != NoDataValue)
			{
				fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;

			  	if (q > 0)
			  	{
					pl_curvature_data[row][col] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/(sqrt(q*q*q));
				}
				else
				{
					pl_curvature_data[row][col] = NoDataValue;
				}
			}
		}
	}


	LSDRaster planform_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,pl_curvature_data);
	return planform_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the profile curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_profile_curvature(Array2D<double>& a, Array2D<double>& b,
                                                          Array2D<double>& c, Array2D<double>& d,
                                                          Array2D<double>& e)
{
	// create the new profile curvature raster
	Array2D<double> profile_curvature_data(NRows,NCols,NoDataValue);
  	double fx, fy, fxx, fyy, fxy, p, q, qqq, denom;

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (a[row][col] != NoDataValue)
			{
			  	fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;

			  	qqq = q*q*q;
			  	if( qqq>0)
			  	{
					denom = (p*sqrt(qqq));
					if( denom != 0)
					{
						profile_curvature_data[row][col] = (fxx*fx*fx + 2*fxy*fx*fy + fyy*fy*fy)/denom;
					}
					else
					{
						profile_curvature_data[row][col] = NoDataValue;
					}
				}
				else
				{
					profile_curvature_data[row][col] = NoDataValue;
				}



			}
		}
	}


	LSDRaster profile_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,profile_curvature_data);
	return profile_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the tangential curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by DM and FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_tangential_curvature(Array2D<double>& a, Array2D<double>& b,
                                                          Array2D<double>& c, Array2D<double>& d,
                                                          Array2D<double>& e)
{
	// create the new planform curvature raster
	Array2D<double> ta_curvature_data(NRows,NCols,NoDataValue);
  	double fx, fy, fxx, fyy, fxy, p, q, denom;

	for (int row = 0; row<NRows; row++)
	{

		for(int col = 0; col<NCols; col++)
		{

			if (a[row][col] != NoDataValue)
			{
				fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;


			  	if( q>0)
			  	{
					denom = (p*sqrt(q));
					if( denom != 0)
					{
						ta_curvature_data[row][col] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/denom;
					}
					else
					{
						ta_curvature_data[row][col] = NoDataValue;
					}
				}
				else
				{
					ta_curvature_data[row][col] = NoDataValue;
				}

			}
		}
	}


	LSDRaster tangential_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,ta_curvature_data);
	return tangential_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function identifies approximate position of stationary points within               // Added by DTM 17/09/2012
// discrete surface using a threshold slope. The nature of the stationary point
// is then determined to discriminate peaks, depressions and saddles.
// 0 = Non-stationary
// 1 = Peak
// 2 = Depression
// 3 = Saddle
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDRaster::calculate_polyfit_classification(Array2D<double>& a, Array2D<double>& b, Array2D<double>& c,
                                                           Array2D<double>& d, Array2D<double>& e)
{
	// create the new classification raster
	int intNoDataValue = int(NoDataValue);
	Array2D<int> classification(NRows,NCols,intNoDataValue);
	double d2z_dx2,d2z_dy2,d2z_dxdy,slope;
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				slope = sqrt(pow(d[row][col],2) + pow(e[row][col],2));
        		if (slope < 0.1) // Threshold for assessing whether point is close to a stationary point
        		{
					d2z_dx2 = 2*a[row][col];
          			d2z_dy2 = 2*b[row][col];
          			d2z_dxdy = c[row][col];
          			if (d2z_dx2 < 0 && d2z_dy2 < 0 && d2z_dxdy*d2z_dxdy < d2z_dx2*d2z_dy2)  // Conditions for peak
          			{
            			classification[row][col] = 1;
          			}
          			else if (d2z_dx2 > 0 && d2z_dy2 > 0 && d2z_dxdy*d2z_dxdy < d2z_dx2*d2z_dy2) // Conditions for a depression
          			{
            			classification[row][col] = 2;
          			}
          			else if (d2z_dx2*d2z_dy2 < 0 || d2z_dxdy*d2z_dxdy > d2z_dx2*d2z_dy2)  // Conditions for a saddle
          			{
           				classification[row][col] = 3;
          			}
          			else
          			{
            			classification = 0;
          			}
				}
			}
		}
	}

	LSDIndexRaster sp_class_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,intNoDataValue,classification);
	return sp_class_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function takes the polyfit functions and requires a window radius and a vector telling the
// function which rasters to print to file. The function is data efficient since one does not
// need to recalucalte the polyfit coefficeint matrices
// it also takes a string which is the prename of the data files
// the file codes in the vector are:
// 0 slope
// 1 aspect
// 2 curvature
// 3 planform curvature
// 4 profile curvature
// 5 tangential curvature
// 6 classification
// SMM 18-Dec-2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_and_print_polyfit_rasters(double window_radius, string file_prefix, vector<int> file_code)
{
	// set up polyfit arrays
	Array2D<double> a;
	Array2D<double> b;
	Array2D<double> c;
	Array2D<double> d;
	Array2D<double> e;
	Array2D<double> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=7)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		double decimal = window_radius-double(window_int);
		double decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";


		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster PolySlope = calculate_polyfit_slope(d, e);
			string S_name = "_pslope_";
			S_name = file_prefix+S_name+window_size_str;
			PolySlope.write_raster(S_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster PolyAspect = calculate_polyfit_aspect(d,e);
			string A_name = "_paspect_";
			A_name = file_prefix+A_name+window_size_str;
			PolyAspect.write_raster(A_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster PolyCurv = calculate_polyfit_curvature(a,b);
			string C_name = "_pcurv_";
			C_name = file_prefix+C_name+window_size_str;
			PolyCurv.write_raster(C_name,DEM_flt_extension);
		}
		if (file_code[3] == 1)
		{
			LSDRaster PolyPlCurv = calculate_polyfit_planform_curvature(a,b,c,d,e);
			string CP_name = "_pplcurv_";
			CP_name = file_prefix+CP_name+window_size_str;
			PolyPlCurv.write_raster(CP_name,DEM_flt_extension);
		}
		if (file_code[4] == 1)
		{
			LSDRaster PolyPrCurv = calculate_polyfit_profile_curvature(a,b,c,d,e);
			string CPr_name = "_pprcurv_";
			CPr_name = file_prefix+CPr_name+window_size_str;
			PolyPrCurv.write_raster(CPr_name,DEM_flt_extension);
		}
		if (file_code[5] == 1)
		{
			LSDRaster PolyTaCurv = calculate_polyfit_tangential_curvature(a,b,c,d,e);
			string CTa_name = "_ptacurv_";
			CTa_name = file_prefix+CTa_name+window_size_str;
			PolyTaCurv.write_raster(CTa_name,DEM_flt_extension);
		}
		if (file_code[6] == 1)
		{
			LSDIndexRaster PolyClass = calculate_polyfit_classification(a,b,c,d,e);
			string CCl_name = "_pclass_";
			CCl_name = file_prefix+CCl_name+window_size_str;
			PolyClass.write_raster(CCl_name,DEM_flt_extension);
		}
	}

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// this function combines the polyfit functions and the roughness function in one package that
// is data efficient becasue it only requires one calcualtion of the polyfit matrices.
// it takes the window radius of the polyfit and the window of the roughness calcualtion
// the file codes in the vector are:
// 0 slope
// 1 aspect
// 2 curvature
// 3 planform curvature
// 4 profile curvature
// 5 tangential curvature
// 6 classification
// 7 roughness s1
// 8 roughness s2
// 9 roughness s3
// SMM 19-12-2012 -> adapted by DTM 15/07/2013 since it really makes sense just to kick out
// s1,s2,s3 rather than derivatives for options 7-9
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_and_print_polyfit_and_roughness_rasters(double window_radius, double roughness_radius,
										string file_prefix, vector<int> file_code)
{
	// set up polyfit arrays
	Array2D<double> a;
	Array2D<double> b;
	Array2D<double> c;
	Array2D<double> d;
	Array2D<double> e;
	Array2D<double> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=10)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		double decimal = window_radius-double(window_int);
		double decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";

		int roughness_int = int(roughness_radius);
		double decimalroughness = roughness_radius-double(roughness_int);
		double decimalroughness_ten = decimalroughness*10;
		int decimalroughness_ten_str = int(decimalroughness_ten);
		string roughness_number_str = itoa(roughness_int);
		string remainderroughness_str = itoa(decimalroughness_ten_str);
		string polystring = "ply";
		string roughstring = "rgh";
		string roughness_size_str = roughness_number_str+p_str+remainderroughness_str;
		roughness_size_str = polystring+window_size_str+underscore+roughstring+roughness_size_str;

		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster PolySlope = calculate_polyfit_slope(d, e);
			string S_name = "_pslope_";
			S_name = file_prefix+S_name+window_size_str;
			PolySlope.write_raster(S_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster PolyAspect = calculate_polyfit_aspect(d,e);
			string A_name = "_paspect_";
			A_name = file_prefix+A_name+window_size_str;
			PolyAspect.write_raster(A_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster PolyCurv = calculate_polyfit_curvature(a,b);
			string C_name = "_pcurv_";
			C_name = file_prefix+C_name+window_size_str;
			PolyCurv.write_raster(C_name,DEM_flt_extension);
		}
		if (file_code[3] == 1)
		{
			LSDRaster PolyPlCurv = calculate_polyfit_planform_curvature(a,b,c,d,e);
			string CP_name = "_pplcurv_";
			CP_name = file_prefix+CP_name+window_size_str;
			PolyPlCurv.write_raster(CP_name,DEM_flt_extension);
		}
		if (file_code[4] == 1)
		{
			LSDRaster PolyPrCurv = calculate_polyfit_profile_curvature(a,b,c,d,e);
			string CPr_name = "_pprcurv_";
			CPr_name = file_prefix+CPr_name+window_size_str;
			PolyPrCurv.write_raster(CPr_name,DEM_flt_extension);
		}
		if (file_code[5] == 1)
		{
			LSDRaster PolyTaCurv = calculate_polyfit_tangential_curvature(a,b,c,d,e);
			string CTa_name = "_ptacurv_";
			CTa_name = file_prefix+CTa_name+window_size_str;
			PolyTaCurv.write_raster(CTa_name,DEM_flt_extension);
		}
		if (file_code[6] == 1)
		{
			LSDIndexRaster PolyClass = calculate_polyfit_classification(a,b,c,d,e);
			string CCl_name = "_pclass_";
			CCl_name = file_prefix+CCl_name+window_size_str;
			PolyClass.write_raster(CCl_name,DEM_flt_extension);
		}
		if (file_code[7] == 1|| file_code[8] == 1|| file_code[9] == 1)
		{
			Array2D<double> l;
			Array2D<double> m;
			Array2D<double> n;
			Array2D<double> s1;
			Array2D<double> s2;
			Array2D<double> s3;
			calculate_polyfit_directional_cosines(d, e, l, m, n);
			calculate_orientation_matrix_eigenvalues(roughness_radius,l,m,n,s1,s2,s3);

      		if (file_code[7] == 1)
		  	{
				LSDRaster s1_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s1);
        		string s1_name = "_s1_";
        		s1_name = file_prefix+s1_name+roughness_size_str;
        		s1_raster.write_raster(s1_name,DEM_flt_extension);
        	}
  			if (file_code[8] == 1)
  			{
				LSDRaster s2_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s2);
        		string s2_name = "_s2_";
  				s2_name = file_prefix+s2_name+roughness_size_str;
  				s2_raster.write_raster(s2_name,DEM_flt_extension);
  			}
  			if (file_code[9] == 1)
  			{
  				LSDRaster s3_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s3);
        		string s3_name = "_s3_";
  				s3_name = file_prefix+s3_name+roughness_size_str;
  				s3_raster.write_raster(s3_name,DEM_flt_extension);
  			}

		}		// end file codes 7,8,9

	}			// end test for file codes logic

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// GET HILLTOP CURVATURE DTM 30/04/13
// Input rasters: curvature, hilltop network.
// Output raster: hilltop curvature
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::get_hilltop_curvature(LSDRaster& curvature, LSDIndexRaster& Hilltops)
{
	// create the new planform curvature raster
	Array2D<double> hilltop_curvature(NRows,NCols,NoDataValue);

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (Hilltops.get_data_element(row,col) != NoDataValue)
      		{
        		hilltop_curvature[row][col] = curvature.get_data_element(row,col);
        	}
    	}
  	}

  	LSDRaster hilltop_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,hilltop_curvature);
	return hilltop_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// RRRRR  EEEEEE IIIIII
// RR  RR EE       II
// RRRR   EEEE     II
// RR RR  EE       II
// RR  RR EEEEEE IIIIII
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=
// ROCK EXPOSURE INDEX
// DiBiase et al. (2012) developed the rock eposure index as a proxy for the
// degree of rock exposure within a basin as defined by the proportion of pixels
// with a local slope exceeding a critical value.  They calculate local slope by
// fitting a planar surface to a 9 cell moving window (window radius = 1).
// Algorithm written by DTM, 08/10/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_plane_coefficient_matrices(double window_radius,
										Array2D<double>& a_plane, Array2D<double>& b_plane,
										Array2D<double>& c_plane)
{
	// this fits a plane over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));           // Set radius of kernel
	int kw=2*kr+1;                    						// width of kernel
	Array2D<double> data_kernel(kw,kw,NoDataValue);
	Array2D<double> x_kernel(kw,kw,NoDataValue);
	Array2D<double> y_kernel(kw,kw,NoDataValue);
	// reset the a,b,c matrices (the coefficient matrices)
	Array2D<double> temp_coef(NRows,NCols,0.0);
	a_plane = temp_coef.copy();
	b_plane = temp_coef.copy();
	c_plane = temp_coef.copy();
	// scale kernel window to resolution of DEM, and translate coordinates to be
	// centred on cell of interest (the centre cell)
	double x,y,zeta;
	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
	      	x_kernel[i][j]=(i-kr)*DataResolution;
	      	y_kernel[i][j]=(j-kr)*DataResolution;
	    }
	}
	// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
	// LOCAL SLOPE
	// Have N simultaneous linear equations, and N unknowns.
	// => b = Ax, where x is a 1xN array containing the coefficients we need for
	// surface fitting.
	// A is constructed using different combinations of x and y, thus we only need
	// to compute this once, since the window size does not change.
	// For 1st order surface fitting, there are 3 coefficients, therefore A is a
	// 3x3 matrix
	Array2D<double> A(3,3);
	for (int i=0; i<kw; ++i)
	{
		for (int j=0; j<kw; ++j)
		{
			x = x_kernel[i][j];
			y = y_kernel[i][j];
			// Generate matrix A
			A[0][0] += pow(x,2);
			A[0][1] += x*y;
			A[0][2] += x;
			A[1][0] += y*x;
			A[1][1] += pow(y,2);
			A[1][2] += y;
			A[2][0] += x;
			A[2][1] += y;
			A[2][2] += 1;
		}
	}
	// Move window over DEM, fitting planar surface to the elevations within the
  // window.
	cout << "\n\tRunning planar surface fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;
	for(int i=0;i<NRows;++i)
	{
		cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for(int j=0;j<NCols;++j)
		{
			// Avoid edges
			if(i-kr < 0 || i+kr+1 > NRows || j-kr < 0 || j+kr+1 > NCols)
			{
				a_plane[i][j] = NoDataValue;
				b_plane[i][j] = NoDataValue;
				c_plane[i][j] = NoDataValue;
			}
			// Avoid nodata values
			else if(RasterData[i][j]==NoDataValue)
			{
				a_plane[i][j] = NoDataValue;
				b_plane[i][j] = NoDataValue;
				c_plane[i][j] = NoDataValue;
			}
			else
			{
				// clip DEM
				//zeta_sampler=zeta.copy();
				for(int i_kernel=0;i_kernel<kw;++i_kernel)
				{
			  		for(int j_kernel=0;j_kernel<kw;++j_kernel)
			  		{
						data_kernel[i_kernel][j_kernel] =
						RasterData[i-kr+i_kernel][j-kr+j_kernel];
						// check for nodata values nearby
						if(data_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
			  		}
				}
				// Fit best fitting plane, avoiding nodata values
				if(ndv_present == 0)  // test for nodata values within the selection
				{
					Array1D<double> bb(3,0.0);
					Array1D<double> coeffs(3);
					for (int krow=0; krow<kw; ++krow)
					{

					  	for (int kcol=0; kcol<kw; ++kcol)
					  	{
							x = x_kernel[krow][kcol];
					    	y = y_kernel[krow][kcol];
					    	zeta = data_kernel[krow][kcol];
					    	// Generate vector bb
					    	bb[0] += zeta*x;
					    	bb[1] += zeta*y;
					    	bb[2] += zeta;
					  	}
					}
					// Solve matrix equations using LU decomposition using the TNT JAMA package:
					// A.coefs = b, where coefs is the coefficients vector.
					LU<double> sol_A(A);  // Create LU object
					coeffs = sol_A.solve(bb);

			  		a_plane[i][j]=coeffs[0];
			  		b_plane[i][j]=coeffs[1];
			  		c_plane[i][j]=coeffs[2];
				}
				ndv_present = 0;
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDRaster::calculate_REI(Array2D<double>& a_plane, Array2D<double>& b_plane, double CriticalSlope)
{
	// create the REI raster
	int intndv = int(NoDataValue);
	Array2D<int> REI_data(NRows,NCols,intndv);
  	double SlopeOfPlane;
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (a_plane[row][col] != NoDataValue)
			{
				SlopeOfPlane = sqrt(a_plane[row][col]*a_plane[row][col]+b_plane[row][col]*b_plane[row][col]);
				// Create binary matrix 1 = rock, 0 = no rock
        		if (SlopeOfPlane > CriticalSlope)
				{
          			REI_data[row][col] = 1;
        		}
        		else
        		{
          			REI_data[row][col] = 0;
        		}
			}
		}
	}

	LSDIndexRaster REI_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,intndv,REI_data);
	return REI_raster;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// RRRRR    OOOO   UU   UU  GGGGG HH   HH NN    NN EEEEEE   SSSS    SSSS                  // Added by DTM 13/09/2012
// RR  RR  OO  OO  UU   UU GG     HH   HH NNN   NN EE     SS      SS
// RRRR   OO    OO UU   UU GG GGG HHHHHHH NN NN NN EEEE    SSSS    SSSS
// RR RR   OO  OO  UU   UU GG  GG HH   HH NN   NNN EE         SS      SS
// RR  RR   OOOO    UUUUU   GGGG  HH   HH NN    NN EEEEEE SSSSS   SSSSS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Algorithm that assesses surface roughness based on a polynomial fit.  Runs a
// moving window across the DEM and assesses the variability of surface normals
// within that window.  Specifically the components of the normals are combined
// into an orientation matrix, which is then solved to find the eigenvalues s1,
// s2, s3 (Woodcock, 1977).
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Get directional cosines
void LSDRaster::calculate_polyfit_directional_cosines(Array2D<double>& d, Array2D<double>& e,
                                    Array2D<double>& l, Array2D<double>& m, Array2D<double>& n)
{
	double pheta, phi;
	// reset the l, m and n matrices (the directional cosines matrices)
	Array2D<double> temp_coef(NRows,NCols,NoDataValue);

	l = temp_coef.copy();
	m = temp_coef.copy();
	n = temp_coef.copy();
	// get surface normals (in form of direction cosines) for each point in DEM
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				pheta = atan(sqrt(d[row][col]*d[row][col]+e[row][col]*e[row][col]));
				if (e[row][col] == 0 || d[row][col] == 0)
				{
					phi = NoDataValue;
				}
        		else
        		{
					phi = atan(e[row][col]/d[row][col]);
				}

        		// Directional cosines of normal vectors
        		if (pheta != 0)
        		{ // l and m are undefined if pheta = 0 because phi is undefined for a vertical surface normal
          			l[row][col]=sin(pheta)*cos(phi);
          			m[row][col]=sin(pheta)*sin(phi);
        		}

        		// Can define n for vertical surface normal, since pheta is always defined
        		n[row][col]=cos(pheta);
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Find eigenvalues for orientation matrix
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_orientation_matrix_eigenvalues(double window_radius,
										Array2D<double>& l, Array2D<double>& m,
										Array2D<double>& n, Array2D<double>& s1,
                    					Array2D<double>& s2, Array2D<double>& s3)
{
	// Reset the eigenvalue matrices
	Array2D<double> temp_coef(NRows,NCols,0.0);

	//Array2D<double> temp_coef(NRows,NCols,NoDataValue);
	s1 = temp_coef.copy();
	s2 = temp_coef.copy();
	s3 = temp_coef.copy();

	// this fits a polynomial surface over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));       // Set radius of kernel => suggest = 1 cell
	int kw=2*kr+1;                    						// width of kernel
	double li,mi,ni;

	// Declare kernel arrays
	Array2D<double> l_kernel(kw,kw,NoDataValue);
	Array2D<double> m_kernel(kw,kw,NoDataValue);
	Array2D<double> n_kernel(kw,kw,NoDataValue);

	// Build circular mask
  	Array2D<int> mask(kw,kw,0);
  	double x_kernel,y_kernel, radial_dist;
  	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
			x_kernel=(i-kr)*DataResolution;
	      	y_kernel=(j-kr)*DataResolution;
        	radial_dist = sqrt(y_kernel*y_kernel + x_kernel*x_kernel); // distance from centre to this point.
        	if (floor(radial_dist) <= window_radius)
        	{
				mask[i][j] = 1;
			}
      	}
	}

	// Loop over DEM, extacting the directional cosines for the data kernel
	for(int row=0;row<NRows;++row)
	{
		for(int col=0;col<NCols;++col)
	  	{
      		//Avoid edges
      		if(row-kr < 0 || row+kr+1 > NRows || col-kr < 0 || col+kr+1 > NCols)
      		{
				s1[row][col]=NoDataValue;
          		s2[row][col]=NoDataValue;
          		s3[row][col]=NoDataValue;
          	}
          	else
          	{
        		//Avoid NoDataValues
        		if(l[row][col] != NoDataValue)
        		{
          			// Construct orientation matrix and solve to retrieve eigenvalues for data window
          			// Build orientation matrix
          			Array2D<double> T(3,3,0.0);
          			Array2D<double> D(3,3);
          			int N=1;
          			for(int i=0;i<kw;++i)
          			{
            			for(int j=0;j<kw;++j)
            			{
							if (mask[i][j]==1)
            			  	{
								li=l[row-kr+i][col-kr+j];
              			  		mi=m[row-kr+i][col-kr+j];
              			  		ni=n[row-kr+i][col-kr+j];
						  	    T[0][0] += li*li;
						  	    T[0][1] += li*mi;
						  	    T[0][2] += li*ni;
						  	    T[1][0] += mi*li;
						  	    T[1][1] += mi*mi;
						  	    T[1][2] += mi*ni;
						  	    T[2][0] += ni*li;
						  	    T[2][1] += ni*mi;
						  	    T[2][2] += ni*ni;
						  	    ++N;
							}
            			}
          			}
          			// Find eigenvalues of the orientation matrix
          			Eigenvalue<double> eigenvalue_matrix(T);
          			eigenvalue_matrix.getD(D);
          			// Normalised eigenvalues (with respect to number of normals):
          			//cout << T << endl;
          			s1[row][col]=D[0][0]/N;
          			s2[row][col]=D[1][1]/N;
          			s3[row][col]=D[2][2]/N;
        		}
      		}
	  	}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is a wrapper to get the three roughness eigenvalues s1, s2 and
// s3.  DTM 15/07/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::calculate_roughness_rasters(double window_radius, double roughness_radius,
										string file_prefix, vector<int> file_code)

{
	// set up polyfit arrays
	Array2D<double> a;
	Array2D<double> b;
	Array2D<double> c;
	Array2D<double> d;
	Array2D<double> e;
	Array2D<double> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=3)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		double decimal = window_radius-double(window_int);
		double decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";

		int roughness_int = int(roughness_radius);
		double decimalroughness = roughness_radius-double(roughness_int);
		double decimalroughness_ten = decimalroughness*10;
		int decimalroughness_ten_str = int(decimalroughness_ten);
		string roughness_number_str = itoa(roughness_int);
		string remainderroughness_str = itoa(decimalroughness_ten_str);
		string polystring = "ply";
		string roughstring = "rgh";
		string roughness_size_str = roughness_number_str+p_str+remainderroughness_str;
		roughness_size_str = polystring+window_size_str+underscore+roughstring+roughness_size_str;

		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);
    	// analyse variability of normals
    	Array2D<double> l;
		Array2D<double> m;
		Array2D<double> n;
		Array2D<double> s1;
		Array2D<double> s2;
		Array2D<double> s3;
		calculate_polyfit_directional_cosines(d, e, l, m, n);
		calculate_orientation_matrix_eigenvalues(roughness_radius,l,m,n,s1,s2,s3);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster s1_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s1);
      		string s1_name = "_s1_";
			s1_name = file_prefix+s1_name+roughness_size_str;
			s1_raster.write_raster(s1_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster s2_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s2);
      		string s2_name = "_s2_";
			s2_name = file_prefix+s2_name+roughness_size_str;
			s2_raster.write_raster(s2_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster s3_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s3);
      		string s3_name = "_s3_";
			s3_name = file_prefix+s3_name+roughness_size_str;
			s3_raster.write_raster(s3_name,DEM_flt_extension);
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// HH  HH YY   YY DDDD   RRRRR    OOOO   LL       OOOO    GGGGG  YY   YY
// HH  HH  YYYY   DD DD  RR  RR  OO  OO  LL      OO  OO  GG	      YYYY
// HHHHHH   YY    DD  DD RRRR   OO    OO LL     OO    OO GG GGG    YY
// HH  HH   YY    DD DD  RR RR   OO  OO  LL      OO  OO  GG  GG    YY
// HH  HH   YY    DDDD   RR  RR   OOOO   LLLLLL   OOOO    GGGGG    YY
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Fill
//---------------------------------------------------------------------------------
//
//This function fills pits/sinks in a DEM by incrementing elevations for cells with
//no downslope neighbour. The process is repeated adnausium until no cells require
//incrementing.
//
//Inputs required are a DEM file in ascii raster format as created by ARCMap
//and a file name to create a filled DEM grid.
//
//This code was built ontop of code made available by Jon D. Pelletier as part
//of his book:
//
//Pelletier,J.D.,'Quantitative Modelling of Landscapes' Cambridge University Press
//
//---------------------------------------------------------------------------------
//
// v1.3 reduced fill increment to 1mm  to avoid 'overfilling'
//
// Martin Hurst, October 2011
//
//---------------------------------------------------------------------------------
//
// v1.2 modified to read *.flt files
//
// Martin Hurst, November 2010
//
//---------------------------------------------------------------------------------
//
// v1.1 function incorporated to allow the tool to fill adjacent pixels immediately
// after filling a given pixel, should speed things up.
//
// Martin Hurst, October 2010
//
//---------------------------------------------------------------------------------
//
// v1.0 is slow as it requires many iterations through the dem
//
// Martin Hurst, June 2010
//
//---------------------------------------------------------------------------------
LSDRaster LSDRaster::fill()
{

	Array2D<double> FilledRasterData;
	FilledRasterData = RasterData.copy();
	cout << "N_rows is: " << NRows << " " << NCols << endl;
	cout << "Data rows: " << RasterData.dim1() << " cols: " << RasterData.dim2() << endl;
	for (int i=1; i<NRows-1; i++)
	{
		cout << "\rRow = " << i+1 << " / " << NRows << "    ";
		for (int j=1; j<NCols-1; j++)
		{
			//cout << "R: " << i << " C: " << j;
			//cout << " FDR: " << FilledRasterData[i][j];
			if (FilledRasterData[i][j] == NoDataValue || FilledRasterData[i-1][j-1] == NoDataValue
			         || FilledRasterData[i-1][j] == NoDataValue || FilledRasterData[i-1][j+1] == NoDataValue
			         || FilledRasterData[i][j+1] == NoDataValue || FilledRasterData[i+1][j+1] == NoDataValue
			         || FilledRasterData[i+1][j] == NoDataValue || FilledRasterData[i+1][j-1] == NoDataValue
			         || FilledRasterData[i][j-1] == NoDataValue)
			{ }
			else fill_iterator(FilledRasterData,i,j);

			//if (RasterData[i][j] == NoDataValue || RasterData[i-1][j-1] == NoDataValue
			//         || RasterData[i-1][j] == NoDataValue || RasterData[i-1][j+1] == NoDataValue
			//         || RasterData[i][j+1] == NoDataValue || RasterData[i+1][j+1] == NoDataValue
			//         || RasterData[i+1][j] == NoDataValue || RasterData[i+1][j-1] == NoDataValue
			//         || RasterData[i][j-1] == NoDataValue)
			//{ }
			//else fill_iterator(RasterData,i,j);
			//cout << " itercomplete" << endl;
		}
	}
	cout << endl;

	LSDRaster FilledDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilledRasterData);
	return FilledDEM;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this is a recursive algorithm that is called by the fill function
void LSDRaster::fill_iterator(Array2D<double>& fill_data, int i, int j)
{
	int a=i;
	int b=j;
	double fill_increment = 0.001;
	double min_zeta;
	double centre_zeta = fill_data[a][b];

	if (a==0 || b==0 || a == NRows-1 || b==NCols-1)
	{ }
	else if (fill_data[a][b] == NoDataValue || fill_data[a-1][b-1] == NoDataValue
			 || fill_data[a-1][b] == NoDataValue
	         || fill_data[a-1][b+1] == NoDataValue || fill_data[a][b+1] == NoDataValue
			 || fill_data[a+1][b+1] == NoDataValue || fill_data[a+1][b] == NoDataValue
			 || fill_data[a+1][b-1] == NoDataValue || fill_data[a][b-1] == NoDataValue)
	{}
	else
	{
		min_zeta = centre_zeta + 10;
		if (fill_data[a-1][b-1] < min_zeta) min_zeta = fill_data[a-1][b-1];
		if (fill_data[a-1][b] < min_zeta) min_zeta = fill_data[a-1][b];
		if (fill_data[a-1][b+1] < min_zeta) min_zeta = fill_data[a-1][b+1];
		if (fill_data[a][b+1] < min_zeta) min_zeta = fill_data[a][b+1];
		if (fill_data[a+1][b+1] < min_zeta) min_zeta = fill_data[a+1][b+1];
		if (fill_data[a+1][b] < min_zeta) min_zeta = fill_data[a+1][b];
		if (fill_data[a+1][b-1] < min_zeta) min_zeta = fill_data[a+1][b-1];
		if (fill_data[a][b-1] < min_zeta) min_zeta = fill_data[a][b-1];

		//increase elevation of centre cell if it is lower than or
		//equal in elevation compared to all adjacent cells
		if (centre_zeta <= min_zeta)
		{

      		// efficiency improvement by Dave Milodowski
      		double zeta_diff = min_zeta - centre_zeta;
      		fill_data[a][b] = fill_data[a][b] + zeta_diff + fill_increment;
			//fill adjacent pixels too
			fill_iterator(fill_data,a-1,b-1);
			fill_iterator(fill_data,a-1,b);
			fill_iterator(fill_data,a-1,b+1);
			fill_iterator(fill_data,a,b+1);
			fill_iterator(fill_data,a+1,b+1);
			fill_iterator(fill_data,a+1,b);
			fill_iterator(fill_data,a+1,b-1);
			fill_iterator(fill_data,a,b-1);
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//---------------------------------------------------------------------------------------
//
//	New fill function
//
//	This function fills pits/sinks in a DEM by checking for pits from lowest to highest
//	elevation, starting at the DEM boundary (raster edge or adjacent to NDVs). Utilises
//	a priority queue to progressively populate the stack and pop out the the lowest value
//	before checking that the neighbouring cells that are yet to be visited must be higher
//	in a hydrologically correct DEM. This method is substantially faster on datasets with
//	pits consisting of multiple cells since each cell only needs to be visited once.
//
//	Input argument required -> MinSlope the minimum slope between two Nodes once filled
//	If set to zero will create flats.
//
//	Method taken from Wang and Liu (2006), Int. J. of GIS. 20(2), 193-213
//
//	Martin Hurst, 12/3/13 */
//
//	Declare the node structure
///@brief Used in pit filling to store elevation data and row and colum indexes.
struct FillNode
{
  /// @brief Elevation data.
	double Zeta;
	/// @brief Row index value.
  int RowIndex;
	/// @brief Column index value.
  int ColIndex;
};

//Overload the less than and greater than operators to consider Zeta data only
//N.B. Fill only needs greater than but less than useful for mdflow routing
//(I've coded this but not yet added to LSDRaster, it's only faster than presorting
//when applied to pretty large datasets).
bool operator>( const FillNode& lhs, const FillNode& rhs )
{
	return lhs.Zeta > rhs.Zeta;
}
bool operator<( const FillNode& lhs, const FillNode& rhs )
{
	return lhs.Zeta < rhs.Zeta;
}

LSDRaster LSDRaster::fill(double& MinSlope)
{
	//cout << "Inside NewFill" << endl;

	//declare 1/root(2)
	double one_over_root2 = 0.707106781;

	//Declare the priority Queue with greater than comparison
	priority_queue< FillNode, vector<FillNode>, greater<FillNode> > PriorityQueue;
	//Declare a temporary FillNode structure which we populate before adding to the PQ
	//Declare a central node or node of interest
	FillNode TempFillNode, CentreFillNode;

	//declare vectors for slopes and row and col indices
	vector<double> slopes(8,NoDataValue);
	vector<int> row_kernal(8);
	vector<int> col_kernal(8);

	//Get Dimensions
	//int NRows = Zeta.dim1();
	//int NCols = Zeta.dim2();

	//Index array to track whether nodes are in queue or have been processed
	//-9999 = no_data, 0 = data but not processed or in queue,
	//1 = in queue but not processed, 2 = fully processed and removed from queue
	Array2D<int> FillIndex(NRows,NCols,NoDataValue);
	Array2D<double> FilledZeta;
	FilledZeta = RasterData.copy();

	//Collect boundary cells
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			if (FilledZeta[i][j] != NoDataValue)
			{
				//If there is data the cell needs to be filled so
				//set fill index to zero (i.e. yet to be filled)
				FillIndex[i][j] = 0;

				//If we're at the edge or next to an NoDataValue then
				//put the cell into the priority queue
				if (i==0 || j==0 || i==NRows-1 || j==NCols-1 ||
					FilledZeta[i-1][j-1]==NoDataValue || FilledZeta[i-1][j]==NoDataValue ||
					FilledZeta[i-1][j+1]==NoDataValue || FilledZeta[i][j-1]==NoDataValue ||
					FilledZeta[i][j+1]==NoDataValue || FilledZeta[i+1][j-1]==NoDataValue ||
					FilledZeta[i+1][j]==NoDataValue || FilledZeta[i+1][j+1]==NoDataValue)
				{
					TempFillNode.Zeta = FilledZeta[i][j];
					TempFillNode.RowIndex = i;
					TempFillNode.ColIndex = j;
					PriorityQueue.push(TempFillNode);
					FillIndex[i][j] = 1;
				}
			}
		}
	}

	//Loop through the priority queue from lowest to highest elevations
	//filling as we go and adding unassessed neighbours to the priority queue
	while (!PriorityQueue.empty())
	{
		//first get the highest priority node and assign it before
		//removing it from the queue and declaring it processed
		CentreFillNode = PriorityQueue.top();
		int row=CentreFillNode.RowIndex, col=CentreFillNode.ColIndex;
		//cout << "Pop from Queue: Zeta = " << CentreFillNode.Zeta << endl;

		PriorityQueue.pop();
		FillIndex[row][col] = 2;

		//get neighbour indices
		//rows
		row_kernal[0] = row-1;
		row_kernal[1] = row-1;
		row_kernal[2] = row;
		row_kernal[3] = row+1;
		row_kernal[4] = row+1;
		row_kernal[5] = row+1;
		row_kernal[6] = row;
		row_kernal[7] = row-1;
		//cols
		col_kernal[0] = col;
		col_kernal[1] = col+1;
		col_kernal[2] = col+1;
		col_kernal[3] = col+1;
		col_kernal[4] = col;
		col_kernal[5] = col-1;
		col_kernal[6] = col-1;
		col_kernal[7] = col-1;

		//check if on array boundary and set kernal to NoDataValues to avoid
		//segmentation fault
		if (row == 0)
		{
			row_kernal[0] = NoDataValue;
			row_kernal[1] = NoDataValue;
			row_kernal[7] = NoDataValue;
		}
		else if (row==NRows-1)
		{
			row_kernal[3] = NoDataValue;
			row_kernal[4] = NoDataValue;
			row_kernal[5] = NoDataValue;
		}
		if (col == 0)
		{
			col_kernal[5] = NoDataValue;
			col_kernal[6] = NoDataValue;
			col_kernal[7] = NoDataValue;
		}
		else if (col == NCols-1)
		{
			col_kernal[1] = NoDataValue;
			col_kernal[2] = NoDataValue;
			col_kernal[3] = NoDataValue;
		}

		//loop through neighbours
		for (int Neighbour = 0; Neighbour<8; ++Neighbour)
		{
			//If the neighbour has data and is not already in the priority queue and has not been processed
			if (	row_kernal[Neighbour] == NoDataValue || col_kernal[Neighbour] == NoDataValue ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == 1 ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == 2 ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == NoDataValue ) {}
			else
			{
				//check if neighbour is equal/lower and therefore needs filling
				if (FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] <= CentreFillNode.Zeta)
				{
					//Modify neighbour's elevation
					if(Neighbour%2 == 0)
					{
						if (MinSlope > 0)
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta + MinSlope*DataResolution;
						}
						else
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta;
						}
					}
					else
					{
						if (MinSlope > 0)
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta
							                                     + MinSlope*DataResolution*one_over_root2;
						}
						else
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta;
						}
					}
				}
				//New neighbour needs to be added to the priority queue
				TempFillNode.Zeta = FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]];
				TempFillNode.RowIndex = row_kernal[Neighbour];
				TempFillNode.ColIndex = col_kernal[Neighbour];
				PriorityQueue.push(TempFillNode);
				FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] = 1;
				FillIndex[row][col] = 2;
			}
		}
	}
	LSDRaster FilledDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilledZeta);
	return FilledDEM;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=







//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//D-inf modules
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Main function for generating a D-infinity flow area raster after Tarboton (1997).
// Calls the recurisve D_infAccum function to get flow area for each pixel.
// Returns flow area in pixels.
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::D_inf_FlowArea(Array2D<double> FlowDir_array){

  // Arrays of indexes of neighbour cells wrt target cell and their
  //corresponding ranges of angles
  int dX[] = {1, 1, 1, 0, -1, -1, -1, 0};
  int dY[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  double startFD[] = {180, 225, 270, 315, 0, 45, 90, 135};
  double endFD[] = {270, 315, 360, 45, 90, 135, 180, 225};

  Array2D<double> Flowarea_Raster(NRows,NCols,1);
  Array2D<double> CountGrid(NRows,NCols,NoDataValue); //array to hold no of inflowing neighbours

  int inflow_neighbours; //counter for number of inflowing neighbours
  double flowDir; //temp variable to store the flowdir of a neighbour

  // Calculate the number of inflowing neighbours to each cell.
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flowDir = FlowDir_array[i][j];
      if (flowDir != NoDataValue){
        inflow_neighbours = 0;

        for (int c = 0; c < 8; ++c){ //loop through the 8 neighbours of the target cell
          flowDir = FlowDir_array[i + dY[c]][j + dX[c]];
          if (flowDir >= 0 && flowDir <= 360){
            if (c != 3){  //handles the issue of 0,360 both pointing to North
              if (flowDir > startFD[c] && flowDir < endFD[c]){
                ++inflow_neighbours;
              }
            }
            else{
              if (flowDir > startFD[c] || flowDir < endFD[c]){
                ++inflow_neighbours;
              }
            }
          }
        }
        CountGrid[i][j] = inflow_neighbours;
      }
      else{
        Flowarea_Raster[i][j] = NoDataValue;
      }
    }
  }

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (CountGrid[i][j] == 0){ //there are no inflowing neighbours
        //call the flowarea function and travel downstream from it
        D_infAccum(i, j, CountGrid, Flowarea_Raster, FlowDir_array);
      }
    }
  }

  LSDRaster FlowArea(NRows, NCols, XMinimum, YMinimum, DataResolution,
                          NoDataValue, Flowarea_Raster);

  return FlowArea;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Recursive function to calculate accumulating area for a given pixel. Called
// by the driver for every cell which has no contributing cells - eg the highest
// points on the landscape. Avoids the need to flatten and sort the DEM as
// required in the original Tarboton (1997) implementation. For more detail on the
// recursive algorithm following channels see Mark (1998) "Network Models in
// Geomorphology".
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::D_infAccum(int i, int j, Array2D<double> CountGrid, Array2D<double> Flowarea_Raster, Array2D<double> FlowDir){

  double flowAccumVal = Flowarea_Raster[i][j];
  double flowDir = FlowDir[i][j];

  //tables of angles and indexes used to rotate around each neighbour
  double FD_Low[] = {0, 45, 90, 135, 180, 225, 270, 315};
  double FD_High_361[] = {45, 90, 135, 180, 225, 270, 315, 361};  //this array ends with 361 to catch angles up to 360
  double FD_High[] = {45, 90, 135, 180, 225, 270, 315, 360};
  int Di1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int Dj1[] = {0, 1, 1, 1, 0, -1, -1, -1};
  int Di2[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int Dj2[] = {1, 1, 1, 0, -1, -1, -1, 0};

  double proportion1 = 0; //proportion of flow to the lowest neighbour
  double proportion2 = 0; //proportion of flow to the second lowest neighbour

  // indexes to store the coordinates of the neighbours where flow is to be routed
  int a1 = 0;
  int b1 = 0;
  int a2 = 0;
  int b2 = 0;

  CountGrid[i][j] = -1; // flags a visted cell

  if (flowDir >= 0){  //avoids flagged pits

    // find which two cells receive flow and the proportion to each
    for (int q = 0; q < 8; ++q){
      if (flowDir >= FD_Low[q] && flowDir < FD_High_361[q]){
        proportion1 = (FD_High[q] - flowDir) / 45;
        a1 = i + Di1[q];
        b1 = j + Dj1[q];
        proportion2 = (flowDir - FD_Low[q]) / 45;
        a2 = i + Di2[q];
        b2 = j + Dj2[q];
        }
    }

      if (proportion1 > 0 && Flowarea_Raster[a1][b1] != NoDataValue){
        Flowarea_Raster[a1][b1] = Flowarea_Raster[a1][b1] + flowAccumVal * proportion1;
        CountGrid[a1][b1] = CountGrid[a1][b1] - 1;
        if (CountGrid[a1][b1] == 0){
          D_infAccum(a1, b1, CountGrid, Flowarea_Raster, FlowDir); //recursive call
        }
      }
      if (proportion2 > 0 && Flowarea_Raster[a2][b2] != NoDataValue){
        Flowarea_Raster[a2][b2] = Flowarea_Raster[a2][b2] + flowAccumVal * proportion2;
        CountGrid[a2][b2] = CountGrid[a2][b2] - 1;
        if (CountGrid[a2][b2] == 0){
          D_infAccum(a2, b2, CountGrid, Flowarea_Raster, FlowDir); //recursive call
      }
    }
  }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// D-infinity flow direction algorithm after Tarboton (1997).
//
// Algorithm takes a filled DEM and for each cell calculates the steepest descent
// based on 8 triangular facets. Flow direction is assigned as an angle from 0-360
// degrees with -1 used to flag unresolved areas such as pits.
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<double> LSDRaster::D_inf_FlowDir(){

  Array2D<double> FlowDir_Array(NRows,NCols,NoDataValue);

  double maxSlope; //maxiumum slope
  double flowDir = 0; //temp variable to hold flowdirections while looping thru the 8 facets

  double pi = 3.14159265;

  //components of the triangular facets as outlined in Tarboton (1997) fig 3
  //& equations 1-5
  double e0;
  double e1;
  double e2;
  double s1;
  double s2;
  double r;
  double s;

  //Facet elevation factors from Tarboton (1997) Table 1
  int acVals[] = {0, 1, 1, 2, 2, 3, 3, 4};
  int afVals[] = {1, -1, 1, -1, 1, -1, 1, -1};
  int e1Col[] = {1, 0, 0, -1, -1, 0, 0, 1};
  int e1Row[] = {0, -1, -1, 0, 0, 1, 1, 0};
  int e2Col[] = {1, 1, -1, -1, -1, -1, 1, 1};
  int e2Row[] = {-1, -1, -1, -1, 1, 1, 1, 1};

  for (int i = 1; i < NRows - 1; ++i){
    for (int j = 1; j < NCols - 1; ++j){
      e0 = RasterData[i][j];
      if (e0 == NoDataValue){
        FlowDir_Array[i][j] = NoDataValue; //if there is no elevation data we cant have a flowdir
      }
      else{
        maxSlope = -9999999;  //set to a low value that != NDV so any slope will be bigger than it

        for (int a = 0; a < 8; ++a){  //loop through the 8 facets
          e1 = RasterData[i + e1Row[a]][j + e1Col[a]];
          e2 = RasterData[i + e2Row[a]][j + e2Col[a]];
          if (e1 != NoDataValue && e2 != NoDataValue){ //avoid facets lyng in no data
            if (e0 > e1 && e0 > e2){
              //calculate slopes (s1,s2,s) and bearings (r) along edges
              //of the facet when e0 is higher than e1 and e2
              s1 = (e0 - e1) / DataResolution;
              if (s1 == 0){
                s1 = 0.00001;
              }
              s2 = (e1 - e2) / DataResolution;
              r = atan(s2 / s1);
              s = sqrt(s1 * s1 + s2 * s2);

              if (s1 < 0 && s2 < 0){
                s = -1 * s;
              }
              if (s1 < 0 && s2 == 0){
                s = -1 * s;
              }
              if (s1 == 0 && s2 < 0){
                s = -1 * s;
              }
              if (s1 == 0.001 && s2 < 0){
                s = -1 * s;
              }
              if (r < 0 || r > atan(1)){
                if (r < 0){
                  r = 0;
                  s = s1;
                }
                else{
                  r = atan(1);
                  s = (e0 - e2) / (DataResolution * sqrt(2)); //diagonal cell length
                }
              }
              if (s >= maxSlope && s != 0.00001){
                maxSlope = s;
                flowDir = afVals[a] * r + acVals[a] * (pi / 2);
              }
            }
            //calculate slope (s) and bearing (r) along edges
            //of the facet when e0 is higher than e1 or e2
            else if (e0 > e1 || e0 > e2){
              if (e0 > e1){
                r = 0;
                s = (e0 - e1) / DataResolution;
              }
              else{
                r = atan(1);
                s = (e0 - e2) / (DataResolution * sqrt(2));
              }
              if (s >= maxSlope && s != 0.00001){
                maxSlope = s;
                flowDir = afVals[a] * r + acVals[a] * (pi / 2);
              }
            }
          }
        }

        if (maxSlope <= 0){
          FlowDir_Array[i][j] = -1;  //unresolved - Tarboton uses D8 to fill these pits - we have a better fill algorithm
        }
        else{
          flowDir = round((flowDir * (180 / pi)) * 10) / 10;
          flowDir = 360 - flowDir + 90;
          if (flowDir > 360){
            flowDir = flowDir - 360;
          }
          FlowDir_Array[i][j] = flowDir;
        }
      }
    }
  }

  return FlowDir_Array;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Function to write the D-infinity flow directions to an LSDRaster.
//
//SWDG - 26/7/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::write_dinf_flowdir_to_LSDRaster(Array2D<double> dinflow){

  LSDRaster FlowDirection(NRows, NCols, XMinimum, YMinimum, DataResolution,
                          NoDataValue, dinflow);

  return FlowDirection;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Wrapper Function to create a D-infinity flow area raster with one function call.
//
//SWDG - 26/7/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::D_inf(){

  Array2D<double> Dinf_flow = D_inf_FlowDir();
  LSDRaster Dinf_area = D_inf_FlowArea(Dinf_flow);

  return Dinf_area;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//end of d-inf modules
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=




//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ASPECT DRIVEN FLOW ROUTING
// --------------------------------------------------------------------------------------
// Divergent flow routing algorithm to simulate hilltop flow routing, routing flow from
// hilltops to valley bottoms.  The original algorithm was written in C++ by Martin Hurst
// based on Lea [1991] 'An aspect driven kinematic routing algorithm'.
// Records mean slope, hillslope length relief, and aspect for each hilltop-channel flow
// path.
// Pre-requisites are a hilltop or ridge network raster, with the network broken down
// into segments; a slope raster; a curvature raster; a stream network raster.
// Output is a text file that contains the the x and y coordinates and HilltopSegment id
// and HilltopCurvature, in addition to the mean slope, hillslope length and relief for
// each flow path.
// New efficiency saving -> use mean slope = relief/total flow length, rather than using
// the slope raster.  This means that you only have to update flow length along each flow
// path and not mean_slope.
//
// Error in Aspect calculation fixed so output file now prints the aspect in radians and
// degrees - SWDG 02/04/13
//
// Now returns an updated hilltop LSDRaster with each hilltop pixel assigned the unique
// value for the stream network that the hilltop flows into  - SWDG 04/04/13
//
// Now takes an Array2D of doubles as the Hilltop network, as all ridges are defined as
// LSDRaster objects - SWDG 8/4/13
//
//---------------------------------------------------------------------------------------
// David Milodowski, 17/12/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::hilltop_flow_routing(Array2D<int>& StreamNetwork, Array2D<double>& Hilltops, Array2D<double>& Aspect,
                                          Array2D<double>& Curvature)
{

	Array2D<double> Hilltops_linked(NRows,NCols,NoDataValue);

  cout << "\n\n" << endl;

  double PI = 3.14159265;
  //Setup output file
	char *outfile = new char[21];
	strcpy(outfile,"hillslope_metrics.txt");

	ofstream ofs;
	ofs.open(outfile);
	if( ofs.fail() )
  {
		cout << "\nFATAL ERROR: unable to write to " << outfile << endl;
		exit(EXIT_FAILURE);
	}
	ofs << "X Y Seg Cht S R Lh AspRad AspDeg \n"; //file headder

  //-------------------------------------------------------------------------------------
  // First calculate vectors with northing and easting
  double YMaximum = YMinimum + NRows*DataResolution;
  Array1D<double> Easting(NCols, NoDataValue);
	Array1D<double> Northing(NRows, NoDataValue);
	int vec_size = 1000000;
  Array1D<double> east_vec(vec_size);
	Array1D<double> north_vec(vec_size);

  for (int i=0; i<NRows; ++i)
  {
		Northing[i] = YMaximum - i*DataResolution - 0.5;
	}
	for (int j=0; j<NCols; ++j)
  {
		Easting[j] = XMinimum + j*DataResolution + 0.5;
	}

  // Next convert aspect from bearing to radians with east as theta = 0/2*pi
  Array2D<double> Theta(NRows,NCols,NoDataValue);

  for (int i=0; i<NRows; ++i)
  {
    for (int j=0; j<NCols; ++j)
    {
      if (Aspect[i][j]!=NoDataValue)
      {
        Theta[i][j]=(PI/180)*((-1*Aspect[i][j])+90);
      }
    }
  }
	// cycle through study area, find hilltops and trace downstream
	int hilltop_count = 0;
  double d, xo, yo, xi, yi; //s_local, s_edge;
  double asp, length, relief, mean_slope;
  double aspect_old, aspect_new, aspect_rads;
  double temp_yo1, temp_yo2, temp_xo1, temp_xo2;
  double X,Y;
  int dir;

  for (int i=1; i<NRows-1; ++i)
  {
		for (int j=1; j<NCols-1; ++j)
    {
			cout << flush << "\t\ti = " << i << " / " << NRows << "\r";
			// ignore edge cells and non-hilltop cells
			// route initial node by aspect and get outlet coordinates
			if (Hilltops[i][j] != NoDataValue)
      {
				//double slope_total = 0;
				length = 0;
				int flag = true;
				int count = 0;
				//path = blank.copy();
			  Array2D<double> path(NRows,NCols,NoDataValue);

				++hilltop_count;
				count = 1;
				double aspect = Aspect[i][j];
				double theta = Theta[i][j];
				int a = i;
				int b = j;
				path[a][b] = 1;

				east_vec[0] = Easting[b];
				north_vec[0] = Northing[a];

				//s_local = slope[a][b];

				// test direction, calculate outlet coordinates and update indices.
				// easterly
        if (aspect >= 45 && aspect < 135)
        {
					xo = 1, yo = (1+tan(theta))/2;
					d = abs(1/(2*cos(theta)));
					xi = 0, yi = yo;
					dir = 1;
					east_vec[count] = Easting[b] + 0.5*DataResolution;
					north_vec[count] = Northing[a] + yo - 0.5*DataResolution;
					++b;
				}
				//southerly
				else if (aspect >= 135 && aspect < 225)
        {
					xo = (1-(1/tan(theta)))/2, yo = 0;
					d = abs(1/(2*cos((PI/2)-theta)));
					xi = xo, yi = 1;
					dir = 2;
					east_vec[count] = Easting[b] + xo - 0.5*DataResolution;
					north_vec[count] = Northing[a] - 0.5*DataResolution;
					++a;
				}
				// westerly
				else if (aspect >= 225 && aspect < 315)
        {
					xo = 0, yo = (1-tan(theta))/2;
					d = abs(1/(2*cos(theta)));
					xi = 1,	yi = yo;
					dir = 3;
					east_vec[count] = Easting[b] -0.5*DataResolution;
					north_vec[count] = Northing[a] + yo - 0.5*DataResolution;
					--b;
				}
				//northerly
				else if (aspect >= 315 || aspect < 45)
        {
					xo = (1+(1/tan(theta)))/2, yo = 1;
					d = abs(1/(2*cos((PI/2) - theta)));
					xi = xo, yi = 0;
					dir = 4;
					east_vec[count] = Easting[b] + xo - 0.5*DataResolution;
					north_vec[count] = Northing[a] + 0.5*DataResolution;
					--a;
				}
				else
        {
					cout << "FATAL ERROR, Kinematic routing algorithm encountered null aspect value" << endl;
					exit(EXIT_FAILURE);
				}
        //collect slopes and totals weighted by path length
				//slope_total += s_local*d;
				length += d;

        // Update local slope
			  //s_local = Slope[a][b];
        // Check that we haven't reached the edge of the DEM, and that we
        // haven't visited this pixel before.
        if (a == 0 || b == 0 ||	a == NRows-1 || b == NCols-1 || StreamNetwork[a][b] != NoDataValue || path[a][b] == 1)
        {
          flag = false;
        }

        // If these conditions are satisfied, then continue the trace until a
        // stream is encountered.
        while (flag == true)
        {
        	path[a][b] = 1;
          aspect_old = aspect;
					aspect_new = Aspect[a][b];
					theta = Theta[a][b];
          ++ count;

				  // Test for perimeter flow paths
					if ((dir == 1 && aspect_new > 0 && aspect_new < 180)
						||	(dir == 2 && aspect_new > 90 && aspect_new < 270)
						||	(dir == 3 && aspect_new > 180 && aspect_new < 360)
						||	(dir == 4 && aspect_new > 270)
            ||  (dir == 4 && aspect_new < 90))
          {
						// DO NORMAL FLOW PATH
						// set xo, yo to 0 and 1 in turn and test for true outlet (xi || yi == 0 || 1)
						temp_yo1 = yi + (1-xi)*tan(theta); 		// xo = 1
						temp_xo1 = xi + (1-yi)*(1/tan(theta)); 	// yo = 1
						temp_yo2 = yi - xi*tan(theta);			// xo = 0
						temp_xo2 = xi - yi*(1/tan(theta));		// yo = 0

						// can't outlet at same point as inlet
						if (dir == 1) temp_yo2 = -1;
						else if (dir == 2) temp_xo1 = -1;
						else if (dir == 3) temp_yo1 = -1;
						else if (dir == 4) temp_xo2 = -1;

						//s_local = Slope[a][b];

						if (temp_yo1 <= 1 && temp_yo1 > 0)
            {
							xo = 1, yo = temp_yo1;
							d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
							xi = 0, yi = yo,
							dir = 1;
							++b;
							if (xi== 0 && yi == 0) yi = 0.00001;
							else if (xi== 0 && yi == 1) yi = 1 - 0.00001;
						}
						else if (temp_xo2 <= 1 && temp_xo2 > 0)
            {
							xo = temp_xo2, yo = 0;
							d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
							xi = xo, yi = 1,
							dir = 2;
							++a;
							if (xi== 0 && yi == 1) xi = 0.00001;
							else if (xi== 1 && yi == 1) xi = 1 - 0.00001;
						}
						else if (temp_yo2 <= 1 && temp_yo2 > 0)
            {
							xo = 0, yo = temp_yo2;
							d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
							xi = 1, yi = yo,
							dir = 3;
							--b;
							if (xi== 1 && yi == 0) yi = 0.00001;
							else if (xi== 1 && yi == 1) yi = 1 - 0.00001;
						}

						else if (temp_xo1 <= 1 && temp_xo1 > 0)
            {
							xo = temp_xo1, yo = 1;
							d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
							xi = xo, yi = 0,
							dir = 4;
							--a;
							if (xi == 0 && yi == 0) xi = 0.00001;
							else if (xi== 1 && yi == 0) xi = 1 - 0.00001;
						}
						//slope_total += s_local*d;
					}

          else
          {
						// ROUTE ALONG EDGES
						if (dir	== 1)
            {
							if 	(aspect_old <= 90 || aspect_new >= 270)
              {
								xo = 0.00001, yo = 1;
								//s_edge = abs(s_local*sin(theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = xo, yi = 1-yo;
								dir = 4;
								--a;
							}
							else if (aspect_old > 90 || aspect_new < 270)
              {
								xo = 0.00001, yo = 0;
								//s_edge = abs(s_local*sin((PI/2)-theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = xo, yi = 1-yo;
								dir = 2;
								++a;
							}
							else
              {
								cout << "Flow unable to route N or S" << endl;
								exit(EXIT_FAILURE);
							}
						}

            else if (dir == 2)
            {
							if 	(aspect_old <= 180 || aspect_new >= 0)
              {
								xo = 1, yo = 1-0.00001;
								//s_edge = abs(s_local*sin((2/PI)-theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 1;
								++b;
							}
							else if (aspect_old > 180 || aspect_new < 360)
              {
								xo = 0, yo = 1-0.00001;
								//s_edge = abs(s_local*sin(theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 3;
								--b;
							}
							else
              {
								cout << "Flow unable to route E or W" << endl;
								exit(EXIT_FAILURE);
							}
						}
            else if (dir == 2)
            {
							if 	(aspect_old <= 180 || aspect_new >= 0)
              {
								xo = 1, yo = 1-0.00001;
								//s_edge = abs(s_local*sin((2/PI)-theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 1;
								++b;
							}
							else if (aspect_old > 180 || aspect_new < 360)
              {
								xo = 0, yo = 1-0.00001;
								//s_edge = abs(s_local*sin(theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 3;
								--b;

							}
							else {
								cout << "Flow unable to route E or W" << endl;
								exit(EXIT_FAILURE);
							}
						}
						else if (dir == 3)
            {
							if 	(aspect_old <= 270 || aspect_new >= 90)
              {
								xo = 1-0.00001, yo = 0;
								//s_edge = abs(s_local*sin(theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = xo, yi = 1-yo;
								dir = 2;
								++a;
							}
							else if (aspect_old > 270 || aspect_new < 90)
              {
								xo = 1-0.00001, yo = 1;
								//s_edge = abs(s_local*sin((2/PI) - theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = xo, yi = 1- yo;
								dir = 4;
								--a;
							}
							else
              {
								cout << "Flow unable to route N or S" << endl;
								exit(EXIT_FAILURE);
							}
						}

            else if (dir == 4)
            {
							if 	(aspect_old <= 360 || aspect_new >= 180)
              {
								xo = 0, yo = 0.00001;
								//s_edge = abs(s_local*sin((PI/2) - theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 3;
								--b;
							}
							else if (aspect_old > 0 || aspect_new < 180)
              {
								xo = 1, yo = 0.00001;
								//s_edge = abs(s_local*sin(theta));
								d = sqrt((pow((xo-xi),2) + pow((yo-yi),2)));
								xi = 1-xo, yi = yo;
								dir = 1;
								++b;
							}
							else
              {
								cout << "Flow unable to route E or W" << endl;
								exit(EXIT_FAILURE);
							}
						}
						//slope_total += s_edge*d;
					}
					length += d;
					aspect = aspect_new;
					// Check that we have not reached the edge of the map and that we have not
					// crossed over the path or reached a stream.
          if (a == 0 || b == 0 ||	a == NRows-1 || b == NCols-1 || StreamNetwork[a][b] != NoDataValue || path[a][b] == 1)
          {
            flag = false;
				  }
        }
        // Determine aspect of flow routing from hilltop->channel
        if (a>i && b>j) aspect_rads = 180 - (180/PI)*atan((a-i)/(b-j));               // SE
        else if (a>i && b<j) aspect_rads = -(180/PI)*atan((a-i)/(b-j));               // NE
        else if (a<i && b<j) aspect_rads = (180/PI)*360-atan((a-i)/(b-j));            // NW
        else if (a<i && b>j) aspect_rads = (180/PI)*180-atan((a-i)/(b-j));            // SW
        else if (a==i && b>j) aspect_rads = 90;                                       // E
        else if (a==i && b<j) aspect_rads = 270;                                      // W
        else if (a>i && b==j) aspect_rads = 180;                                      // S
        else if (a<i && b==j) aspect_rads = 0;                                        // N
        else aspect_rads = NoDataValue;
        if (aspect_rads != NoDataValue) asp = (aspect_rads * PI)/180; //convert aspect to radians
        else asp = NoDataValue;

				//if trace finished at a stream, print hillslope info.
				if (StreamNetwork[a][b] != NoDataValue && aspect_rads != NoDataValue)
        {
          // PRINT TO FILE Cht Sbar Relief Lh
					X = XMinimum + j*DataResolution;
					Y = YMinimum - (NRows-i)*DataResolution;
					relief = RasterData[i][j] - RasterData[a][b];
					length = length*DataResolution;
					mean_slope = relief/length;

          //mean_slope = slope_total/(length/DataResolution);

          Hilltops_linked[i][j] = StreamNetwork[a][b];

					ofs << X << " " << Y << " " << StreamNetwork[a][b] /*Hilltops[i][j]*/ << " " << Curvature[i][j]
            << " " << mean_slope << " " << relief << " " << length << " " << asp << " " << aspect_rads << " \n";

					//PRINT FILE OF PATH NODES FOR EACH HILLSLOPE VECTOR
				//	stringstream s;
				//	s << ht_count;
				//	file_part_2 = s.str();
				//	filename = file_part_1;
				//	filename.append(file_part_2), filename.append(file_part_3);
				//	strcpy(filename_c,filename.c_str());



				//	ofstream prof_out;
				//	prof_out.open(filename_c);
				//	prof_out << "Easting " << "Northing" << endl;
				//	prof_out.precision(10);

				//	for (int c=0;c<count;++c) {
						//print
				//		prof_out 	<< east_vec[c] << " "
				//					<< north_vec[c] << endl;
				//	}
				//	prof_out.close();
				}
      }
		}
	}
	ofs.close();

  LSDRaster IndexedHilltops(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Hilltops_linked);
  return IndexedHilltops;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module to sample LSDRaster values running along a ridgetop network. Pass in
// a TNT array of doubles that denotes the ridge network. Ridge network is
// generated from LSDChannelNetwork::ExtractRidges
//
// Returns sampled LSDRaster object
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeSample(Array2D<double>& Ridges){

  Array2D<double> Sample_data(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue && Ridges[i][j] != NoDataValue ){
        Sample_data[i][j] = RasterData[i][j];
      }
    }
  }

  LSDRaster Sample(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Sample_data);
  return Sample;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Pass a smoothing window over a ridge LSDRaster object to calculate an average
// value running along the ridgetop.
//
// Pass in an optional integer smoothing window radius between 1 and 6.
// Default value is 2
//
// Returns LSDRaster object containing the averaged data.
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeSmoother(int WindowRadius){

  //arbitrary upper bound to limit too large a radius
  if (WindowRadius < 1 || WindowRadius > 6){
    WindowRadius = 2; //
  }

  Array2D<double> Smoothed(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      double sum = 0;
      int counter = 0;
      if (RasterData[i][j] != NoDataValue){
          for (int a = (-1*WindowRadius); a <= WindowRadius; ++a){
              for (int b = (-1*WindowRadius); b <= WindowRadius; ++b){
                if(a >= 0 && a <= NRows){
                  if(b >= 0 && b <= NCols){
                    if (RasterData[i+a][j+b] != NoDataValue){
                        sum += RasterData[i+a][j+b];
                        ++counter;
                    }
                  }
                }
             }
          }
      Smoothed[i][j] = sum/counter;
      }
    }
  }

  LSDRaster Smooth(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Smoothed);
  return Smooth;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Pass a buffer over a ridge LSDRaster object to increase sampling area.
//
// Pass in an optional integer buffer radius between 1 and 6.
// Default value is 2
//
// Returns LSDRaster object denoting the buffered ridge data.
//
// Buffers equally in all directions, so use with care to avoid sampling areas
// away from the axis of the original ridge line.
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeBuffer(int BufferRadius){

  Array2D<double> HilltopBuffer(NRows, NCols, NoDataValue);

  //arbitrary upper bound to limit too large a buffer
  if (BufferRadius < 1 || BufferRadius > 6){
    BufferRadius = 2;
  }

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue){
        for (int a = (-1 * BufferRadius); a <= BufferRadius; ++a){
          for (int b = (-1 * BufferRadius); b <= BufferRadius; ++b){
            if(i + a >= 0 && i + a <= NRows){
              if(j + b >= 0 && j + b <= NCols){
                HilltopBuffer[i + a][j + b] = RasterData[i][j];
              }
            }
          }
        }
      }
    }
  }

  LSDRaster Buffer(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, HilltopBuffer);
  return Buffer;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module assigns an average CHT (or other input LSDRaster value) to each basin,
// works by searching for every hilltop value that falls within a basin, summing
// these values and writing the final average to every cell identified as the
// basin in question.
//
// Pass in an LSDIndexRaster of Drainage basins, generated using
// ChannelNetwork::ExtractBasinsOrder
//
// Returns LSDRaster of average basin CHT for each identified basin.
//
// Very inefficent at present. Module loops through every cell in LSDRaster
// (2 * number of basins) + 1 times. Beware!
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::BasinAverager(LSDIndexRaster& Basins){

  vector<int> basin_index;
  Array2D<int> basin_ids = Basins.get_RasterData();

  Array2D<double> Averaged(NRows,NCols,NoDataValue);

  //make list of unique basins in each raster
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      int id = basin_ids[i][j];
      if (id != NoDataValue){
        //check if next basin_id is unique
        if(find(basin_index.begin(), basin_index.end(), id) == basin_index.end()){
          basin_index.push_back(id);
        }
      }
    }
  }

  //loop through each basin
  for (vector<int>::iterator it = basin_index.begin(); it !=  basin_index.end(); ++it){
    int counter = 0;
    double sum = 0;

    for (int i = 0; i < NRows; ++i){
      for (int j = 0; j < NCols; ++j){

       if (RasterData[i][j] != NoDataValue && basin_ids[i][j] != *it ){
         sum += RasterData[i][j];
         ++counter;
        }
      }
    }

    for (int i = 0; i < NRows; ++i){
      for (int j = 0; j < NCols; ++j){
        if(basin_ids[i][j] == *it){
          Averaged[i][j] = sum/counter;
        }
      }
    }
  }

  LSDRaster Averaged_out(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Averaged);
  return Averaged_out;
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM and routes the flow accordingly. Consequently the dem is sorted and indexed
// using LSDStatsTools.
//
// Can handle DEMs containing flats, but pits must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//
// Updated 23/4/13 to allow periodic boundary condtitions - SWDG
// Needs to be able to handle the boundary cond vector and only reflect bondaries when needed.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::MDFlow(vector<string> BoundaryConditions){

  int Row2 = NRows + 2;
  int Col2 = NCols + 2;

  //create output array, populated with nodata
  Array2D<double> area_final(NRows, NCols, NoDataValue);

  Array2D<double> elev_pad(Row2, Col2, NoDataValue);
  Array2D<double> area(Row2, Col2, NoDataValue);

  //declare variables
  vector<double> flat;
  vector<double> sorted;
  vector<size_t> index_map;
  double one_ov_root_2 = 0.707106781187;


  //translate the data down and right by one cell to create a border
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
        elev_pad[i+1][j+1] = RasterData[i][j];
    }
  }

  //reflect boundaries
  for (int i = 0; i < Row2; ++i){
    for (int j = 0; j < Col2; ++j){
      if(i==0 && j != 0 && j != Col2-1){
        //top row
        elev_pad[i][j] = elev_pad[Row2-2][j];
      }
      if(i==Row2-1 && j != 0 && j != Col2-1){
        //bottom row
        elev_pad[i][j] = elev_pad[1][j];
      }

      if(j==0 && i != 0 && i != Row2-1){
        //left column
        elev_pad[i][j] = elev_pad[i][Col2-2];
      }

      if(j==Col2-1 && i!=0 && i != Row2-1){
        //right column
        elev_pad[i][j] = elev_pad[i][1];
      }
    }
  }

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every non ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(elev_pad[i][j]);
      if (elev_pad[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_double_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  double total = 0;
			  double slope1 = 0;
        double slope2 = 0;
        double slope3 = 0;
        double slope4 = 0;
        double slope5 = 0;
        double slope6 = 0;
        double slope7 = 0;
        double slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (elev_pad[i][j] > elev_pad[i-1][j-1] && elev_pad[i-1][j-1] != NoDataValue){
          slope1 = (elev_pad[i][j] - elev_pad[i-1][j-1]) * one_ov_root_2;
          total += slope1;
        }
			  if (elev_pad[i][j] > elev_pad[i-1][j] && elev_pad[i-1][j] != NoDataValue){
          slope2 = elev_pad[i][j] - elev_pad[i-1][j];
          total += slope2;
			  }
		  	if (elev_pad[i][j] > elev_pad[i-1][j+1] && elev_pad[i-1][j+1] != NoDataValue){
          slope3 = (elev_pad[i][j] - elev_pad[i-1][j+1]) * one_ov_root_2;
          total += slope3;
		  	}
			  if (elev_pad[i][j] > elev_pad[i][j+1] && elev_pad[i][j+1] != NoDataValue){
          slope4 = elev_pad[i][j] - elev_pad[i][j+1];
          total += slope4;
        }
			  if (elev_pad[i][j] > elev_pad[i+1][j+1] && elev_pad[i+1][j+1] != NoDataValue){
          slope5 = (elev_pad[i][j] - elev_pad[i+1][j+1]) * one_ov_root_2;
          total += slope5;
		  	}
			  if (elev_pad[i][j] > elev_pad[i+1][j] && elev_pad[i+1][j] != NoDataValue){
          slope6 = elev_pad[i][j] - elev_pad[i+1][j];
          total += slope6;
        }
			  if (elev_pad[i][j] > elev_pad[i+1][j-1] && elev_pad[i+1][j-1] != NoDataValue){
          slope7 = (elev_pad[i][j] - elev_pad[i+1][j-1]) * one_ov_root_2;
          total += slope7;
			  }
			  if (elev_pad[i][j] > elev_pad[i][j-1] && elev_pad[i][j-1] != NoDataValue){
          slope8 = elev_pad[i][j] - elev_pad[i][j-1];
          total += slope8;
        }

			//if no slope is found (total == 0), pass flow to all equal elevation cells]
      if (total == 0) {
			  if(elev_pad[i-1][j-1] == elev_pad[i][j]){
          slope1 = 1;
          total += 1;
        }
				if(elev_pad[i-1][j] == elev_pad[i][j]){
				  slope2 = 1;
          total += 1;
        }
				if(elev_pad[i-1][j+1] == elev_pad[i][j]){
				  slope3 = 1;
          total += 1;
        }
				if(elev_pad[i][j+1] == elev_pad[i][j]){
				  slope4 = 1;
          total += 1;
        }
			  if(elev_pad[i+1][j+1] == elev_pad[i][j]){
				  slope5 = 1;
          total += 1;
        }
				if(elev_pad[i+1][j] == elev_pad[i][j]){
				  slope6 = 1;
          total += 1;
        }
				if(elev_pad[i+1][j-1] == elev_pad[i][j]){
          slope7 = 1;
          total += 1;
        }
				if(elev_pad[i][j-1] == elev_pad[i][j]){
          slope8 = 1;
          total += 1;
        }
			}

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }

  //translate the data back
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
        area_final[i][j] = area[i+1][j+1];
    }
  }

  //write output LSDRaster object
  LSDRaster MultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, area_final);
  return MultiFlow;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM, and weights them using the equation from Freeman et al 1991 and routes the
// flow accordingly.
//
// Paper link: http://www.sciencedirect.com/science/article/pii/009830049190048I
//
// Cardinal Weighting = (elevation_drop/total_elevation_drop)^1.1
// Diagonal Weighting = ((elevation_drop/total_elevation_drop)*(1/root(2)))^1.1
//
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::FreemanMDFlow(){

  //create output array, populated with nodata
  Array2D<double> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<double> flat;
  vector<double> sorted;
  vector<size_t> index_map;
  double one_ov_root_2 = 0.707106781187;
  double p = 1.1; //value avoids preferential flow to diagonals

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_double_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  double total = 0;
			  double slope1 = 0;
        double slope2 = 0;
        double slope3 = 0;
        double slope4 = 0;
        double slope5 = 0;
        double slope6 = 0;
        double slope7 = 0;
        double slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = pow(((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2),p);
          total += slope1;
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = pow((RasterData[i][j] - RasterData[i-1][j]),p);
          total += slope2;
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = pow(((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2),p);
          total += slope3;
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = pow((RasterData[i][j] - RasterData[i][j+1]),p);
          total += slope4;
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = pow(((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2),p);
          total += slope5;
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = pow((RasterData[i][j] - RasterData[i+1][j]),p);
          total += slope6;
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = pow(((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2),p);
          total += slope7;
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = pow((RasterData[i][j] - RasterData[i][j-1]),p);
          total += slope8;
        }

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }
  //write output LSDRaster object
  LSDRaster FreemanMultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, area);
  return FreemanMultiFlow;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM, and weights them using the equation from Quinn et al 1991 and routes the
// flow accordingly.
//
// Paper link: http://onlinelibrary.wiley.com/doi/10.1002/hyp.3360050106/abstract
//
// Cardinal Weighting = (elevation_drop/total_elevation_drop)*DataResolution/2
// Diagonal Weighting =
//      ((elevation_drop/total_elevation_drop)*(1/root(2)))* DataResolution*0.354
//
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::QuinnMDFlow(){

  //create output array, populated with nodata
  Array2D<double> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<double> flat;
  vector<double> sorted;
  vector<size_t> index_map;
  double one_ov_root_2 = 0.707106781187;
  double Lc = DataResolution/2; //cardinal scaling factor
  double Ld = DataResolution * 0.354; //diagonal scaling factor


  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_double_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  double total = 0;
			  double slope1 = 0;
        double slope2 = 0;
        double slope3 = 0;
        double slope4 = 0;
        double slope5 = 0;
        double slope6 = 0;
        double slope7 = 0;
        double slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = ((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2) * Ld;
          total += slope1;
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = (RasterData[i][j] - RasterData[i-1][j]) * Lc;
          total += slope2;
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = ((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2) * Ld;
          total += slope3;
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = (RasterData[i][j] - RasterData[i][j+1]) * Lc;
          total += slope4;
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = ((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2) * Ld;
          total += slope5;
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = (RasterData[i][j] - RasterData[i+1][j]) * Lc;
          total += slope6;
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = ((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2) * Ld;
          total += slope7;
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = (RasterData[i][j] - RasterData[i][j-1]) * Lc;
          total += slope8;
        }

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }
  //write output LSDRaster object
  LSDRaster QuinnMultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, area);
  return QuinnMultiFlow;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM. Finds the cell of the steepest descent and then checks the two
// neighbouring cells slopes. If either is also downslope proportion flow 
// between the steepest cell and the steepest neighbour. If neither neighbour
// is downslope 100% of flow follows the steepest path.  
// 
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG - 02/08/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::M2DFlow(){

  //create output array, populated with nodata
  Array2D<double> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<double> flat;
  vector<double> sorted;
  vector<size_t> index_map;
  double one_ov_root_2 = 0.707106781187;

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_double_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){
      
        //reset variables on each loop
			  double slope0 = 0;
        double slope1 = 0;
        double slope2 = 0;
        double slope3 = 0;
        double slope4 = 0;
        double slope5 = 0;
        double slope6 = 0;
        double slope7 = 0;
        vector<double> slopes;
        
        double p1 = 0;
        double p2 = 0;
        int second_slope = -1; //initialized using value outside of range.
        
        //Get magnitude of downslope flow slope0->7 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope0 = ((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2);
          slopes.push_back(slope0);
        }
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope1 = (RasterData[i][j] - RasterData[i-1][j]);
          slopes.push_back(slope1);
			  }
        else {
          slopes.push_back(0);
        }
        
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope2 = ((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2);
          slopes.push_back(slope2);
		  	}
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope3 = (RasterData[i][j] - RasterData[i][j+1]);
          slopes.push_back(slope3);
        }
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope4 = ((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2);
          slopes.push_back(slope4);
		  	}
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope5 = (RasterData[i][j] - RasterData[i+1][j]);
          slopes.push_back(slope5);
        }
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope6 = ((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2);
          slopes.push_back(slope6);
			  }
        else {
          slopes.push_back(0);
        }
        
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope7 = (RasterData[i][j] - RasterData[i][j-1]);
          slopes.push_back(slope7);
        }
        else {
          slopes.push_back(0);
        }
                
        if (int(slopes.size()) > 0 ){   //catch outlets with no neighbours to drain to
                
          //find maximum slope & its index location in the slopes vector      
          double S_max = *max_element(slopes.begin(), slopes.end());
          int S_max_index = find(slopes.begin(), slopes.end(), S_max) - slopes.begin();
          
          //find steepest neighbour
          if (S_max_index == 0){
            if (slope7 > 0 && slope1 == 0){
              second_slope = 7;            
            }
            if (slope7 == 0 && slope1 > 0){
              second_slope = 1;          
            }          
            if (slope7 > 0 && slope1 > 0){
              if (slope7 > slope1){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope7 == slope1){
              second_slope = 0;          
            }            
          }
  
          if (S_max_index == 1){
            if (slope0 > 0 && slope2 == 0){
              second_slope = 7;            
            }
            if (slope0 == 0 && slope2 > 0){
              second_slope = 1;          
            }          
            if (slope0 > 0 && slope2 > 0){
              if (slope0 > slope2){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope0 == slope2){
              second_slope = 0;          
            }            
          }
                       
          if (S_max_index == 2){
            if (slope1 > 0 && slope3 == 0){
              second_slope = 7;            
            }
            if (slope1 == 0 && slope3 > 0){
              second_slope = 1;          
            }          
            if (slope1 > 0 && slope3 > 0){
              if (slope1 > slope3){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope1 == slope3){
              second_slope = 0;          
            }            
          }
          
          if (S_max_index == 3){
            if (slope2 > 0 && slope4 == 0){
              second_slope = 7;            
            }
            if (slope2 == 0 && slope4 > 0){
              second_slope = 1;          
            }          
            if (slope2 > 0 && slope4 > 0){
              if (slope2 > slope4){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope2 == slope4){
              second_slope = 0;          
            }            
          }                
  
          if (S_max_index == 4){
            if (slope3 > 0 && slope5 == 0){
              second_slope = 7;            
            }
            if (slope3 == 0 && slope5 > 0){
              second_slope = 1;          
            }          
            if (slope3 > 0 && slope5 > 0){
              if (slope3 > slope5){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope3 == slope5){
              second_slope = 0;          
            }            
          }
  
          if (S_max_index == 5){
            if (slope4 > 0 && slope6 == 0){
              second_slope = 7;            
            }
            if (slope4 == 0 && slope6 > 0){
              second_slope = 1;          
            }          
            if (slope4 > 0 && slope6 > 0){
              if (slope4 > slope6){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope4 == slope6){
              second_slope = 0;          
            }            
          }
          
          
          if (S_max_index == 6){
            if (slope5 > 0 && slope7 == 0){
              second_slope = 7;            
            }
            if (slope5 == 0 && slope7 > 0){
              second_slope = 1;          
            }          
            if (slope5 > 0 && slope7 > 0){
              if (slope5 > slope7){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope5 == slope7){
              second_slope = 0;          
            }            
          }
          
          if (S_max_index == 7){
            if (slope6 > 0 && slope0 == 0){
              second_slope = 7;            
            }
            if (slope6 == 0 && slope0 > 0){
              second_slope = 1;          
            }          
            if (slope6 > 0 && slope0 > 0){
              if (slope6 > slope0){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }           
            }
            if (slope6 == slope0){
              second_slope = 0;          
            }            
          }   
  
          //get proportions p1 and p2
          if (second_slope != S_max_index){
            p1 = S_max/(S_max + slopes[second_slope]);
            p2 = slopes[second_slope]/(S_max + slopes[second_slope]);
          }
          else{ //flow only in 1 direction
            p1 = 1;
            p2 = 0;
          }
          
          //partition flow following the steepest slope and it's steepest neighbour
          if (S_max_index == 0 && area[i-1][j-1] != NoDataValue){ 
            area[i-1][j-1] += area[i][j] * p1;
            if (second_slope == 1){
              area[i-1][j] += area[i][j] * p2;
            }
            if (second_slope == 7){
              area[i][j-1] += area[i][j] * p2;
            }
          }
          
          if (S_max_index == 1 && area[i-1][j] != NoDataValue){ 
            area[i-1][j] += area[i][j] * p1;
            if (second_slope == 2){
              area[i-1][j+1] += area[i][j] * p2;
            }
            if (second_slope == 0){
              area[i-1][j-1] += area[i][j] * p2;
            }
          }        
          
          if (S_max_index == 2 && area[i-1][j+1] != NoDataValue){ 
            area[i-1][j+1] += area[i][j] * p1;
            if (second_slope == 3){
              area[i][j+1] += area[i][j] * p2;
            }
            if (second_slope == 1){
              area[i-1][j] += area[i][j] * p2;
            }
          }        
  
          if (S_max_index == 3 && area[i][j+1] != NoDataValue){ 
            area[i][j+1] += area[i][j] * p1;
            if (second_slope == 4){
              area[i+1][j+1] += area[i][j] * p2;
            }
            if (second_slope == 2){
              area[i-1][j+1] += area[i][j] * p2;
            }
          }
  
          if (S_max_index == 4 && area[i+1][j+1] != NoDataValue){ 
            area[i+1][j+1] += area[i][j] * p1;
            if (second_slope == 5){
              area[i+1][j] += area[i][j] * p2;
            }
            if (second_slope == 3){
              area[i][j+1] += area[i][j] * p2;
            }
          }
          
          if (S_max_index == 5 && area[i+1][j] != NoDataValue){ 
            area[i+1][j] += area[i][j] * p1;
            if (second_slope == 6){
              area[i+1][j-1] += area[i][j] * p2;
            }
            if (second_slope == 4){
              area[i+1][j+1] += area[i][j] * p2;
            }
          }        
          
          if (S_max_index == 6 && area[i+1][j-1] != NoDataValue){ 
            area[i+1][j-1] += area[i][j] * p1;
            if (second_slope == 7){
              area[i][j-1] += area[i][j] * p2;
            }
            if (second_slope == 5){
              area[i+1][j] += area[i][j] * p2;
            }
          }        
  
          if (S_max_index == 7 && area[i][j-1] != NoDataValue){ 
            area[i][j-1] += area[i][j] * p1;
            if (second_slope == 0){
              area[i-1][j] += area[i][j] * p2;
            }
            if (second_slope == 6){
              area[i-1][j-1] += area[i][j] * p2;
            }
          }
        }
      }
    }
  }
  
  //write output LSDRaster object
  LSDRaster Multi2Flow(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, area);
  return Multi2Flow;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// PREDICTING CHANNEL HEADS USING TANGENTIAL CURVATURE
//
// This function is used to predict channel head locations based on the method proposed by
// Pelletier (2013).  It creates a contour curvature map and identifies channel heads as pixels greater
// than a user defined contour curvature threshold value, set by default at 0.1.  The threshold curvature
// can also be defined as a multiple of the standard deviation of the curvature.  Before this function is called
// the DEM must be filtered using the wiener filter in the LSDRasterSpectral object in order to remove high frequency
// noise.
//
// Reference: Pelletier (2013) A robust, two-parameter method for the extraction of drainage
// networks from high-resolution digital elevation models (DEMs): Evaluation using synthetic and real-world
// DEMs, Water Resources Research 49: 1-15
//
// added by FC 16/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

LSDRaster LSDRaster::calculate_pelletier_channel_heads(int NRows,int NCols,int XMinimum,int YMinimum,double DataResolution,
                                                          int NoDataValue,double window_radius,
                                                          double tan_curv_threshold,Array2D<double>& tan_curv_array)
{
	Array2D<double> chan_head_locations(NRows,NCols,NoDataValue);
	double total_curv = 0;
	int n_observations = 0;

  //get the mean of the tangential curvature
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] != NoDataValue)
      {
        total_curv = total_curv + tan_curv_array[row][col];
        ++n_observations;
      }
    }
  }

  double mean_curv = total_curv/n_observations;
  double total_st_dev = 0;

  // get the standard deviation of the curvature and use 3*st dev as the threshold value
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] != NoDataValue)
      {
        total_st_dev = ((tan_curv_array[row][col] - mean_curv)*(tan_curv_array[row][col] - mean_curv)) + total_st_dev;
      }
    }
  }

  //double st_dev = sqrt(total_st_dev/n_observations);
 // double tan_curv_threshold = 3*st_dev;

  // Get all the locations where the tan curvature is greater than the user defined threshold
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] > tan_curv_threshold)
      {
        chan_head_locations[row][col] = tan_curv_array[row][col];
      }
      else
      {
        chan_head_locations[row][col] = 0;
      }
    }
  }

  // Write raster of predicted channel head locations
  LSDRaster channel_heads(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,chan_head_locations);
  return channel_heads;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=





//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//		 sSSSs MM   MM  oOOo   oOOo  TTTTTT HH  HH IIII NN   NN  gGGGG
//		SS     M M M M oO  Oo oO  Oo   TT   HH  HH  II  NNN  NN GG
//		 sSSs  M  M  M OO  OO OO  OO   TT   HHHHHH  II  NN N NN GG GGG
//		    SS M     M oO  Oo oO  Oo   TT   HH  HH  II  NN  NNN GG  GG
//		sSSSs  M     M  oOOo   oOOo    TT   HH  HH IIII NN   NN  GGGGG
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::NonLocalMeansFilter(int WindowRadius, int SimilarityRadius, int DegreeFiltering, double Sigma)
{
	/*
	Perform Non-local means filtering on a DEM following Baude et al. [2005]
	Smoothes non-gaussian noise.

	Inputs required:
		the search window radius,
		the similarity window radius and
		the degree of filtering

	Martin Hurst, February, 2012
	Modified by David Milodowski, May 2012- generates grid of recording filtered noise

	WindowRadius has to be <= SimilarityRadius ?

	Adapted from a matlab script by:
	Author: Jose Vicente Manjon Herrera & Antoni Buades
	Date: 09-03-2006

	Implementation of the Non local filter proposed for A. Buades, B. Coll and J.M. Morel in
	"A non-local algorithm for image denoising"

	**Added soft threshold optimal correction - David Milodowski, 05/2012
	*/


	//Declare Arrays to hold Filtered Data and Noise
	Array2D<double> FilteredRasterData(NRows,NCols);
	//Array2D<double> FilteredNoise(NRows,NCols);

	//Declare Array to hold a padded copy of the raster with padding values taken
	//as reflected values from the edge of RasterData
	Array2D<double> PaddedRasterData(NRows+2*SimilarityRadius, NCols+2*SimilarityRadius,0.0);
	PadRasterSymmetric(PaddedRasterData, SimilarityRadius);

	//initiate the local gaussian kernel and populate
	int KernelDimension = 2*SimilarityRadius+1;
	Array2D<double> Kernel(KernelDimension,KernelDimension,0.0);
  	MakeGaussianKernel(Kernel, Sigma, SimilarityRadius);

	//initiate temporary arrays
	Array2D<double> W1(KernelDimension,KernelDimension);
	Array2D<double> W2(KernelDimension,KernelDimension);

	//initiate temp variables
	float w, wmax, average, sweight, d;

	//loop through DEM
	int i1, j1, rowmin, rowmax, colmin, colmax;

  	for (int i=0; i<NRows; ++i)
	{
		i1 = i+SimilarityRadius;
		for (int j=0; j<NCols; ++j)
		{
			j1 = j+SimilarityRadius;
      		//Get DEM sample  with size SimilarityRadius, centred on cell of interest
      		for (int a=0; a<(KernelDimension); ++a)
			{
  		  		for (int b=0; b<(KernelDimension); ++b)
				{
          			W1[a][b] = PaddedRasterData[i1-SimilarityRadius+a][j1-SimilarityRadius+b];
				}
      		}

      		wmax=0;
      		average=0;
  			sweight=0;

			//get bounding conditions
			rowmin = max(i1-WindowRadius,SimilarityRadius);
			rowmax = min(i1+WindowRadius,NRows+SimilarityRadius-1);
			colmin = max(j1-WindowRadius,SimilarityRadius);
			colmax = min(j1+WindowRadius,NCols+SimilarityRadius-1);

			//loop to calculate weigths for each cell
      		for (int row=rowmin; row<rowmax+1; ++row)
			{
			     for (int col=colmin; col<colmax+1; ++col)
				{
			          d=0;

					//If centre cell do nothing
			          if (row!=i1 || col!=j1)
					//Otherwise do the calculations
			          {
						//Extract DEM centred around each point in kernel
						for (int a=0; a<(KernelDimension); ++a)
						{
							for (int b=0; b<(KernelDimension); ++b)
							{
								W2[a][b] = PaddedRasterData[row+a-SimilarityRadius][col+b-SimilarityRadius];
								d += Kernel[a][b]*(W1[a][b]-W2[a][b])*(W1[a][b]-W2[a][b]);
							}
						}

						w = exp(-d/(DegreeFiltering*DegreeFiltering));
            				if (w>wmax) wmax=w;
           				sweight += w;
            				average += w*PaddedRasterData[row][col];
          			}
				}
      		}

			average += wmax*PaddedRasterData[i1][j1];
			sweight += wmax;

      		if (sweight > 0) FilteredRasterData[i][j] = average/sweight;
			else FilteredRasterData[i][j] = RasterData[i][j];

      		// Also extract a record of the noise
      		//FilteredNoise[i][j]=RasterData[i][j]-FilteredRasterData[i][j];
		}
	}

	LSDRaster NLFilteredDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilteredRasterData);
	//LSDRaster NLFilteredNoise(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilteredNoise);
	return NLFilteredDEM; //, NLFilteredNoise;
}

void LSDRaster::PadRasterSymmetric(Array2D<double>& PaddedRasterData, int& SimilarityRadius)
{
	/*

	Creates a buffer around an array (of size SimilarityRadius) and gives the new border
	mirror symmetric values of the original array reflected across the boundary.
	SimilarityRadius should be the size of the window if filtering

	New array has size nrows + 2*SimilarityRadius by ncols + 2*SimilarityRadius

	Martin Hurst, Feb 2012

	*/

	int PaddedNRows = NRows + 2*SimilarityRadius;
	int PaddedNCols = NCols + 2*SimilarityRadius;

	int minus_i;
	int minus_j;

	for (int i=0; i<PaddedNRows; ++i)
	{
		for (int j=0; j<PaddedNCols; ++j)
		{
			//reverse of i and j
			minus_i = PaddedNRows-1-i;
			minus_j = PaddedNCols-1-j;

			//north boundary
			if (i<SimilarityRadius) {
				if (j<SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][SimilarityRadius-j];
				}
				else if (j>(NCols-1+SimilarityRadius)) {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];

				}
				else {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][j-SimilarityRadius];
				}
			}
			//south boundary
			else if (i>NRows-1+SimilarityRadius) {
				if (j<SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][SimilarityRadius-j];
				}
				else if (j>NCols+SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];
				}
				else {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][j-SimilarityRadius];
				}
			}
			//west boundary
			else if (j<SimilarityRadius) {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][SimilarityRadius-j];
			}
			//east boundary
			else if (j>NCols-1+SimilarityRadius) {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];
			}
			//copy rest of RasterData
			else {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][j-SimilarityRadius];
			}
		}
	}

}

void LSDRaster::MakeGaussianKernel(Array2D<double>& Kernel, double sigma, int SimilarityRadius)
{
	/*

	Generate gaussian weighted kernel
	kernel array must be predeclared of size SimilarityRadius and consist of zeros:
	Array2D<double> Kernel(SimilarityRadius,SimilarityRadius,0.0);

	Kernel generated using:
	G(x,y) = (1/2*pi*sigma^2) exp ((-x^2+y^2)/(2*sigma^2))

	Martin Hurst, Feb 2012

	*/

	double pi = 3.1415926536;
	double left_side = 1/(2*pi*sigma*sigma);
	double twosigma2 = 2.0*sigma*sigma;
	double right_side;
	double wgt = 0;
	double value;

	//calculate kernel values
	for (int i=0;i<2*SimilarityRadius+1;++i)
	{
		for (int j=0;j<2*SimilarityRadius+1;++j)
		{
			right_side = -(((j-SimilarityRadius)*(j-SimilarityRadius) + (i-SimilarityRadius)*(i-SimilarityRadius))/twosigma2);
			right_side = exp(right_side);
			value = left_side*right_side;
			Kernel[i][j] = value;
			wgt += value;
		}
	}

	//scale to sum to 1
	for (int i=0;i<2*SimilarityRadius+1;++i)
	{
		for (int j=0;j<2*SimilarityRadius+1;++j)
		{
			Kernel[i][j] = Kernel[i][j]/wgt;
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


/*

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//		FFFFFF  oOOo  UU  UU RRRR   IIIIII EEEEEE RRRR
//		FF     oO  Oo UU  UU RR RR    II   EE     RR RR
//		FFFF   OO  OO UU  UU RRRR     II   EEEE   RRRR
//		FF     oO  Oo UU  UU RR RR    II   EE     RR RR
//		FF      oOOo   uUUu  RR  RR IIIIII EEEEEE RR  RR
//
//     AAAA  NN    NN  AAAA  LL   YY    YY  sSSSs IIIIII  sSSSs
//    AA  AA NNNN  NN AA  AA LL    YY  YY  SS       II   SS
//    AAAAAA NN NN NN AAAAAA LL     YYYY    sSSs    II    sSSs
//    AA  AA NN  NNNN AA  AA LL      YY        SS   II       SS
//    AA  AA NN   NNN AA  AA LLLLLL  YY    SSSSs  IIIIII SSSSs
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// FAST FOURIER TRANSFORM MODULE
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Computes both the forward and inverse fast fourier transforms of a 2D
// discrete dataset.
// FOR FORWARD TRANSFORM:
//    - InputArray = zeta_padded (padded DEM)
//    - transform_direction = -1
//    - OutputArray = 2D spectrum

void LSDRaster::dfftw2D_fwd(Array2D<double>& InputArray, Array2D<double>& OutputArrayReal, Array2D<double>& OutputArrayImaginary, int transform_direction, int Ly, int Lx)
{
  fftw_complex *input,*output;
  fftw_plan plan;
  // Declare one_dimensional contiguous arrays of dimension Ly*Lx
  input = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Ly*Lx);
  output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Ly*Lx);
  // SET UP PLAN
  // -forwards, transform_direction==-1, -inverse, transform_direction==1
  if (transform_direction==-1)
  {
    cout << "  Running 2D discrete FORWARD fast fourier transform..." << endl;
    plan = fftw_plan_dft_2d(Ly,Lx,input,output,transform_direction,FFTW_MEASURE);
  }
  else
  {
    cout << "\nFATAL ERROR: for the tranform direction\n\t -1 = FORWARD \n\t" << endl;
		exit(EXIT_FAILURE);
  }
  // LOAD DATA INTO COMPLEX ARRAY FOR FFT IN ROW MAJOR ORDER
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      input[Lx*i+j][0] = InputArray[i][j];
    }
  }
  // EXECUTE PLAN
  fftw_execute(plan);
  // RETRIEVE OUTPUT - since data is real, we only need to extract real part of
  // the output.
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      OutputArrayReal[i][j] = output[Lx*i+j][0];
      OutputArrayImaginary[i][j] = output[Lx*i+j][1];
    }
  }
  // DEALLOCATE PLAN AND ARRAYS
  fftw_destroy_plan(plan);
  fftw_free(input);
  fftw_free(output);
}

//------------------------------------------------------------------------------
// FOR INVERSE TRANSFORM:
//    - InputArrays = Real and Imaginary components of 2D spectrum
//    - transform_direction = 1
//    - OutputArray = reconstructed DEM
void LSDRaster::dfftw2D_inv(Array2D<double>& InputArrayReal, Array2D<double>& InputArrayImaginary, Array2D<double>& OutputArray, int transform_direction, int Ly, int Lx)
{
  fftw_complex *input,*output;
  fftw_plan plan;
  // Declare one_dimensional contiguous arrays of dimension Ly*Lx
  input = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Ly*Lx);
  output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Ly*Lx);
  // SET UP PLAN
  // -forwards => transform_direction==-1, -inverse => transform_direction==1
  if (transform_direction==1)
  {
  cout << "  Running 2D discrete INVERSE fast fourier transform..." << endl;
  plan = fftw_plan_dft_2d(Ly,Lx,input,output,transform_direction,FFTW_MEASURE);
  }
  else {
    cout << "\nFATAL ERROR: for the tranform direction\n\t 1 = INVERSE \n\t" << endl;
		exit(EXIT_FAILURE);
  }
  // LOAD DATA INTO COMPLEX ARRAY FOR FFT IN ROW MAJOR ORDER
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      input[Lx*i+j][0] = InputArrayReal[i][j];
      input[Lx*i+j][1] = InputArrayImaginary[i][j];
    }
  }
  // EXECUTE PLAN
  fftw_execute(plan);
  // RETRIEVE OUTPUT ARRAY
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      OutputArray[i][j] = output[Lx*i+j][0];
    }
  }
  // DEALLOCATE PLAN AND ARRAYS
  fftw_destroy_plan(plan);
  fftw_free(input);
  fftw_free(output);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// DETREND DATA MODULE
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
// LOCAL SLOPE ax + by + c = z
// Have N simultaneous linear equations, and N unknowns.
// => b = Ax, where x is a 1xN array containing the coefficients we need for
// surface fitting.
// A is constructed using different combinations of x and y, thus we only need
// to compute this once, since the window size does not change.
// For 1st order surface fitting, there are 3 coefficients, therefore A is a
// 3x3 matrix
// Module kicks out detrended array, and an array with the trend plane
void LSDRaster::detrend2D(Array2D<double>& zeta, Array2D<double>& zeta_detrend, Array2D<double>& trend_plane, int nrows, int ncols, double ndv)
{
  cout << "  Detrending the DEM by fitting a planar surface..." << endl;
  Array2D<double> A(3,3,0.0);
	Array1D<double> bb(3,0.0);
	Array1D<double> coeffs(3);

  for (int i=0; i<nrows; ++i)
	{
    for (int j=0; j<ncols; ++j)
		{
			if(zeta[i][j] != ndv)
			{
        double x = j;
			  double y = i;
        // Generate matrix A
			  A[0][0] += pow(x,2);
			  A[0][1] += x*y;
			  A[0][2] += x;
			  A[1][0] += y*x;
			  A[1][1] += pow(y,2);
			  A[1][2] += y;
			  A[2][0] += x;
			  A[2][1] += y;
			  A[2][2] += 1;
			  // Generate vector bb
			  bb[0] += zeta[i][j]*x;
			  bb[1] += zeta[i][j]*y;
			  bb[2] += zeta[i][j];
			}
		}
	}
  // Solve matrix equations using LU decomposition using the TNT JAMA package:
	// A.coefs = b, where coefs is the coefficients vector.
	LU<double> sol_A(A);  // Create LU object
	coeffs = sol_A.solve(bb);
	double a_plane = coeffs[0];
	double b_plane = coeffs[1];
	double c_plane = coeffs[2];
	// Create detrended surface
  for (int i=0; i<nrows; ++i)
	{
    for (int j=0; j<ncols; ++j)
		{
      double x = j;
			double y = i;
      trend_plane[i][j] = a_plane*x + b_plane*y + c_plane;
       if(zeta[i][j] != ndv)
      {
        zeta_detrend[i][j] = zeta[i][j] - trend_plane[i][j];
      }
      else // Set NoDataValues as 0 on detrended surface
      {
        zeta_detrend[i][j] = 0;
      }
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// HANN WINDOW MODULE
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Use 2D elliptical Hann (raised cosine) window on data matrix, to reduce
// spectral leakage and retain good frequency resolution.
// Return windowed data, the Hann window and also the summed square of the
// weighting coefficients, WSS.
// Another option would be to use a 2D Welch window, but functionality is very
// similar.
void LSDRaster::window_data_Hann2D(Array2D<double>& zeta_detrend, Array2D<double>& zeta_Hann2D, Array2D<double>& Hann2D, double& WSS, int nrows, int ncols, int ndv)
{
  double PI = 3.14159265;
  cout << "  Windowing DEM using an elliptical 2D Hann window..." << endl;
  double ny = nrows;
  double nx = ncols;
  // Get matrix coordinates of centroid of matrix
  double a = (nx-1)/2;
  double b = (ny-1)/2;
  // Set up data window

  Array2D<double> r_prime_matrix(nrows,ncols,0.0);
  Array2D<double> id(nrows,ncols,0.0);
  Array2D<double> theta_matrix(nrows,ncols,0.0);
  double r; // radial polar coordinate
  double theta; // angular polar coordinate
  double rprime;
  double HannCoefficient = 0;
  for(int i = 0; i < nrows; ++i)
  {
    for(int j = 0; j < ncols; ++j)
    {
      double x = j;
      double y = i;
      if(x == a)
      {
        theta = (PI/2);
      }
      else
      {
        theta = atan2((y - b),(x - a));
      }
      r = sqrt((y - b)*(y - b) + (x - a)*(x - a)); // distance from centre to this point
      rprime = sqrt((a*a)*(b*b)/(b*b*(cos(theta)*cos(theta)) + a*a*(sin(theta)*sin(theta)))); // distance from centre to edge of ellipse for this particular theta
      if(r < rprime)
      {
        HannCoefficient = 0.5 * (1 + cos(PI * r/rprime));
        Hann2D[i][j] = HannCoefficient;
        WSS += HannCoefficient*HannCoefficient;
        zeta_Hann2D[i][j] = zeta_detrend[i][j] * HannCoefficient;
        id[i][j]=1;
      }
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// SHIFT ORIGIN OF SPECTRUM IN FOURIER DOMAIN
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// "The output of most algorithms that compute the DFT must be rearranged to
// place the zero wavenumber element near the center of the array. Provided Nx
// and Ny are even, dividing the output array into four equal quadrants and
// exchanging the nonadjacent quadrants will place the zero wavenumber element
// at the position (Nx/2, Ny/2) in the new array."  (Perron et al., 2008)

void LSDRaster::shift_spectrum(Array2D<double>& spectrum_real,  Array2D<double>& spectrum_imaginary, Array2D<double>& spectrum_real_shift, Array2D<double>& spectrum_imaginary_shift, int Ly, int Lx)
{
  int QuadrantRows = Ly/2;
  int QuadrantCols = Lx/2;
  for(int i = 0; i < QuadrantRows; ++i)
  {
    for(int j = 0; j< QuadrantCols; ++j)
    {
      spectrum_real_shift[i][j] = spectrum_real[i+QuadrantRows][j+QuadrantCols]; // top left to bottom right
      spectrum_real_shift[i+QuadrantRows][j] = spectrum_real[i][j+QuadrantCols]; // bottom right to top left
      spectrum_real_shift[i][j+QuadrantCols] = spectrum_real[i+QuadrantRows][j]; // top right to bottom left
      spectrum_real_shift[i+QuadrantRows][j+QuadrantCols] = spectrum_real[i][j]; // bottom right to top left

      spectrum_imaginary_shift[i][j] = spectrum_imaginary[i+QuadrantRows][j+QuadrantCols];   // etc...
      spectrum_imaginary_shift[i+QuadrantRows][j] = spectrum_imaginary[i][j+QuadrantCols];
      spectrum_imaginary_shift[i][j+QuadrantCols] = spectrum_imaginary[i+QuadrantRows][j];
      spectrum_imaginary_shift[i+QuadrantRows][j+QuadrantCols] = spectrum_imaginary[i][j];
    }
  }
}
//------------------------------------------------------------------------------
// DE-SHIFT ORIGIN OF SPECTRUM
// Inverse process of above to return filtered spectrum to original format
// required for the inverse fourier transform algorithm.
void LSDRaster::LSDRaster::shift_spectrum_inv(Array2D<double>& FilteredSpectrumReal, Array2D<double>& FilteredSpectrumImaginary, Array2D<double>& FilteredSpectrumReal_deshift, Array2D<double>& FilteredSpectrumImaginary_deshift, int Ly, int Lx)
{
  int QuadrantRows = Ly/2;
  int QuadrantCols = Lx/2;

  for(int i = 0; i < QuadrantRows; ++i)
  {
    for(int j = 0; j< QuadrantCols; ++j)
    {
      FilteredSpectrumReal_deshift[i+QuadrantRows][j+QuadrantCols] = FilteredSpectrumReal[i][j];
      FilteredSpectrumReal_deshift[i][j+QuadrantCols] = FilteredSpectrumReal[i+QuadrantRows][j];
      FilteredSpectrumReal_deshift[i+QuadrantRows][j] = FilteredSpectrumReal[i][j+QuadrantCols];
      FilteredSpectrumReal_deshift[i][j] = FilteredSpectrumReal[i+QuadrantRows][j+QuadrantCols];

      FilteredSpectrumImaginary_deshift[i+QuadrantRows][j+QuadrantCols] = FilteredSpectrumImaginary[i][j];
      FilteredSpectrumImaginary_deshift[i][j+QuadrantCols] = FilteredSpectrumImaginary[i+QuadrantRows][j];
      FilteredSpectrumImaginary_deshift[i+QuadrantRows][j] = FilteredSpectrumImaginary[i][j+QuadrantCols];
      FilteredSpectrumImaginary_deshift[i][j] = FilteredSpectrumImaginary[i+QuadrantRows][j+QuadrantCols];
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// CALCULATE THE DFT PERIODOGRAM
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Multiply fourier analysis output by complex conjugate and normalise.
// Note that for complex number z=x+iy, z*=x-iy, z.z* = x^2 + y^2
// Returns 2D PSD as only output
// Usage => Declare Array2D<double> P_DFT(Ly,Lx,0.0)
//       => then P_DFT = calculate_2D_PSD(...)
void LSDRaster::calculate_2D_PSD(Array2D<double>& spectrum_real_shift, Array2D<double>& spectrum_imaginary_shift, Array2D<float>& P_DFT_output, int Lx, int Ly, double WSS)
{
  Array2D<float> P_DFT(Ly,Lx,0.0);
  for (int i=0; i<Ly; ++i)
  {
    for (int j=0; j<Lx; ++j)
    {
      P_DFT[i][j] = (pow(spectrum_real_shift[i][j],2) + pow(spectrum_imaginary_shift[i][j],2))/(Ly*Lx*WSS);
    }
  }
  // Copy periodogram to output file
  P_DFT_output = P_DFT;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// GET RADIAL POWER SPECTRUM
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Collapse 2D PSD into a radial PSD
void LSDRaster::calculate_radial_PSD(Array2D<double>& P_DFT, vector<double>& RadialPSD_output, vector<double>& RadialFrequency_output, int Lx, int Ly, double WSS, double dem_res)
{
  // CALCULATE FREQUENCY INCREMENTS - for generation of power spectrum
  // Frequency goes from zero to 1/(2*resolution), the Nyquist frequency in
  // nrows_padded/2 increments.
  double dfx = 1/(dem_res*Lx);
  double dfy = 1/(dem_res*Ly);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  vector<double> RadialFrequency(Ly*(Lx/2+1),0.0); // This is the distance from the origin in Frequency space.  Note that half of spectrum is redundant, since the fourier transform of a real dataset is symmetric, with a degeneracy of two.
  vector<double> RadialPSD(Ly*(Lx/2+1),0.0);
  double NyquistFreq = 1/(2*dem_res);
  double RadialFreq;
  int count = 0;
  for (int i=0; i < Ly; ++i)
  {
    for (int j=0; j < (Lx/2+1); ++j)
    {

      double x = j;
      double y = i;
      RadialFreq = sqrt((y - (Ly/2))*(y - (Ly/2))*dfy*dfy + (x - (Lx/2))*(x - (Lx/2))*dfx*dfx); // distance from centre to this point. Converting position in frequency into an absolute frequency
      if (RadialFreq <= NyquistFreq)  // Ignore radial frequencies greater than the Nyquist frequency as these are aliased
        {
          RadialFrequency[count] = RadialFreq;
          RadialPSD[count] = 2*P_DFT[i][j];   // Due to degeneracy
          ++count;
        }
    }
  }
  // Sort radial frequency
  vector<size_t> index_map;
  matlab_double_sort(RadialFrequency,RadialFrequency,index_map);
  // Reorder amplitudes to match sorted frequencies
  matlab_double_reorder(RadialPSD,index_map,RadialPSD);

  // Get number of discrete radial frequencies
  int n_freqs = 0;
  for (int i=0; i<(Ly*(Lx/2+1)); ++i)
  {
    if (RadialFrequency[i] != RadialFrequency[i+1])
    {
      ++n_freqs;
    }
  }

  // Convert to spatially averaged spectrum
  cout << "  Converting to radially averaged PSD..." << endl;
  vector<double> RadialFrequency_grouped(n_freqs,0.0); // This is the distance from the origin in Frequency space
  vector<double> RadialPSD_average(n_freqs,0.0);       // This will ultimately contain the radially averaged PSD
  int n_occurences = 0;           // This will keep track of the number of occurences of each radial frequency
  int pointer = 0;
  for (int i=0; i<(Ly*(Lx/2+1)); ++i)
  {
    RadialFrequency_grouped[pointer] = RadialFrequency[i];
    RadialPSD_average[pointer] += RadialPSD[i];
    ++n_occurences;

    if (RadialFrequency[i] != RadialFrequency[i+1])
    {
      RadialPSD_average[pointer] = RadialPSD_average[pointer]/n_occurences;
      // increment pointer and reset n_occurences
      ++pointer;
      n_occurences = 0;
    }
  }

  // Copy across to output vectors
  RadialFrequency_output = RadialFrequency_grouped;
  RadialPSD_output = RadialPSD_average;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MAIN FUNCTIONS USING SPECTRAL FILTERS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// COMPUTE DISCRETE FAST FOURIER TRANSFORM OF A REAL, 2-DIMENSIONAL DATASET.
// Computes the 2D and radial power spectra of a 2D array.
// Input arguement is the width of the logarithmically spaced bins. For
// topography, suggest this is 0.1
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::fftw2D_spectral_analysis(char* file_id, double LogBinWidth)
{
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DETREND DATA
	// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
	// LOCAL SLOPE ax + by + c = z
	Array2D<double> zeta_detrend(NRows,NCols);
	Array2D<double> trend_plane(NRows,NCols);
  detrend2D(RasterData, zeta_detrend, trend_plane, NRows, NCols, NoDataValue);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // USE ELLIPTICAL 2D HANN (raised cosine) WINDOW ON ZETA MATRIX.
  // RETURN WINDOWED DATA AND ALSO THE SUMMED SQUARE OF THE WEIGHTING
  // COEFFICIENTS.
  Array2D<double> Hann2D(NRows,NCols,0.0);
  Array2D<double> zeta_Hann2D(NRows,NCols,0.0);
  double WSS = 0; // summed square of weighting coefficients
  window_data_Hann2D(zeta_detrend, zeta_Hann2D, Hann2D, WSS, NRows, NCols, int(NoDataValue));

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 2D DISCRETE FAST FOURIER TRANSFORM
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // PAD DATA WITH ZEROS TO A POWER OF TWO (facilitates FFT)
  int Ly = int(pow(2,ceil(log(NRows)/log(2))));
  int Lx = int(pow(2,ceil(log(NCols)/log(2))));

  Array2D<double> zeta_padded(Ly,Lx);
  for (int i=0;i<Ly;++i){
    for (int j=0;j<Lx;++j){
      if (i<NRows && j<NCols)
      {
        zeta_padded[i][j] = zeta_detrend[i][j];
      }
      else
      {
        zeta_padded[i][j]=0;
      }
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DO 2D FORWARD FAST FOURIER TRANSFORM
  int transform_direction = -1;
  Array2D<double> SpectrumReal(Ly,Lx);
  Array2D<double> SpectrumImaginary(Ly,Lx);
  dfftw2D_fwd(zeta_padded, SpectrumReal, SpectrumImaginary, transform_direction, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // REARRANGE SPECTRUM SO THAT ORIGIN IS AT THE CENTRE
  Array2D<double> SpectrumReal_shift(Ly,Lx);
  Array2D<double> SpectrumImaginary_shift(Ly,Lx);
  shift_spectrum(SpectrumReal, SpectrumImaginary, SpectrumReal_shift, SpectrumImaginary_shift, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // GET RADIAL POWER SPECTRUM
  // For forward transform, return the spectral power of the topography both
  // in a 2D array, and also as a one dimensional array of radial frequency
  vector<double> RadialPSD;
  vector<double> RadialFrequency;
  calculate_radial_PSD(SpectrumReal_shift, RadialPSD, RadialFrequency, Lx, Ly, WSS, DataResolution);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // BIN POWER SPECTRUM INTO LOGARITMICALLY SPACED BINS OF RADIAL FREQUENCY TO
  // GET MODEL "SIGNAL" FOR WIENER FILTER
  cout << "  Binning radial PSD into logarithmically spaced bins..." << endl;
  // Initiate output vectors
  vector<double> Bin_MeanRadialFreq;
  vector<double> Bin_RadialPSD;
  vector<double> BinMidpoints;
  vector<double> StandardDeviationRadialFreq;
  vector<double> StandardDeviationRadialPSD;
  // Execute log binning
  log_bin_data(RadialFrequency, RadialPSD, LogBinWidth, Bin_MeanRadialFreq, Bin_RadialPSD, BinMidpoints,
  								StandardDeviationRadialFreq, StandardDeviationRadialPSD, int(NoDataValue));

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // WRITE OUTPUTS TO FILE
  int len = strlen(file_id);
  cout << "  Writing output files..." << endl;

  // 2D PSD
  char *PSD_file = new char[len+6];
  strcpy(PSD_file,file_id);
  strcat(PSD_file,"_P_DFT");

  LSDRaster PowerSpectrum(Lx,Ly,-(Lx/2),(Lx/2-1),DataResolution,NoDataValue,SpectrumReal_shift);
  PowerSpectrum.write_raster(PSD_file,"flt");
  //----------------------------------------------------------------------------

	ofstream ofs;
  //----------------------------------------------------------------------------
  // Radially averaged PSD

  char *RadialPSD_file = new char[len+14];
	strcpy(RadialPSD_file,file_id);
  strcat(RadialPSD_file,"_RadialPSD.txt");

	ofs.open(RadialPSD_file);
	if( ofs.fail() )
  {
		cout << "\nFATAL ERROR: unable to write to " << RadialPSD_file << endl;
		exit(EXIT_FAILURE);
	}
	ofs << "Freq Wavelength PSD Model_PSD Model_noise\n";
  for(int i=0; i < int(RadialFrequency.size()); ++i)
  {
    ofs << RadialFrequency[i] << " " << 1/RadialFrequency[i] << " " << RadialPSD[i] << " \n";
  }
  ofs.close();
  //----------------------------------------------------------------------------
  // Binned averaged PSD
  char *RadialPSD_binned_file = new char[len+21];
	strcpy(RadialPSD_binned_file,file_id);
  strcat(RadialPSD_binned_file,"_RadialPSD_binned.txt");

	ofs.open(RadialPSD_binned_file);
	if( ofs.fail() )
  {
		cout << "\nFATAL ERROR: unable to write to " << RadialPSD_binned_file << endl;
		exit(EXIT_FAILURE);
	}
	ofs << "Freq Wavelength PSD Sigma Model Noise\n";
  for(int i=0; i < int(Bin_MeanRadialFreq.size()); ++i)
  {
    ofs << Bin_MeanRadialFreq[i] << " " << 1/Bin_MeanRadialFreq[i] << " " << Bin_RadialPSD[i] << " " << StandardDeviationRadialPSD[i] << " \n";
  }
  ofs.close();

  //----------------------------------------------------------------------------
  cout << "  DONE!" << endl;
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=









//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//		 sSSSs PPPPP  EEEEEE   CCCC TTTTTT RRRR    AAAA  LL
//		SS     PP  PP EE      CC      TT   RR RR  AA  AA LL
//		 sSSs  PPPPP  EEEE   CC       TT   RRRR   AAAAAA LL
//		    SS PP     EE      CC      TT   RR RR  AA  AA LL
//		SSSSs  PP     EEEEEE   CCCC   TT   RR  RR AA  AA LLLLLL
//
//    FFFFFF IIIIII LL   TTTTTT EEEEEE RRRR    sSSSs
//    FF       II   LL     TT   EE     RR RR  SS
//    FFFF     II   LL     TT   EEEE   RRRR    sSSs
//    FF       II   LL     TT   EE     RR RR      SS
//    FF     IIIIII LLLLLL TT   EEEEEE RR  RR SSSSs
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// FILTER WEIGHTS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// BANDPASS FILTER
// Filter array to band between frequency bands f1 and f2.  The bandpass filter
// is a gaussian filter centred at (f1+f2)/2 and with a SD of |f2-f1|/6.
void LSDRaster::bandpass_filter(Array2D<double>& RawSpectrumReal, Array2D<double>& RawSpectrumImaginary, Array2D<double>& FilteredSpectrumReal, Array2D<double>& FilteredSpectrumImaginary, int Lx, int Ly, double dem_res, double f1, double f2)
{
  // CALCULATE FREQUENCY INCREMENTS - for generation of power spectrum
  // Frequency goes from zero to 1/(2*resolution), the Nyquist frequency in
  // nrows_padded/2 increments.
  double dfx = 1/(dem_res*Lx);
  double dfy = 1/(dem_res*Ly);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  cout << "  Gaussian bandpass filter between f1 = " << f1 << " and f2 = " << f2 << endl;
  double f; // radial frequency
  double weight; // Filter weight
  double sigma = sqrt((f2-f1)*(f2-f1))/6; // Standard Deviation of Gaussian filter
  for (int i=0; i < Ly; ++i)
  {
    for (int j=0; j < Lx; ++j)
    {
      double x = j;
      double y = i;
      f = sqrt((y - (Ly/2))*(y - (Ly/2))*dfy*dfy + (x - (Lx/2))*(x - (Lx/2))*dfx*dfx); // distance from centre to this point. Converting position in frequency into an absolute frequency
      weight = exp(-(f - 0.5*(f1 + f2))*(f - 0.5*(f1 + f2))/(2*sigma*sigma));
      FilteredSpectrumReal[i][j] = weight*RawSpectrumReal[i][j];
      FilteredSpectrumImaginary[i][j] = weight*RawSpectrumImaginary[i][j];
    }
  }
}
//------------------------------------------------------------------------------
// LOWPASS FILTER
// Filter array to retain frequencies below f1.  The filter edge is a radial
// gaussian function with a SD of |f2-f1|/3.
void LSDRaster::lowpass_filter(Array2D<double>& RawSpectrumReal, Array2D<double>& RawSpectrumImaginary, Array2D<double>& FilteredSpectrumReal, Array2D<double>& FilteredSpectrumImaginary, int Lx, int Ly, double dem_res, double f1, double f2)
{
  // CALCULATE FREQUENCY INCREMENTS - for generation of power spectrum
  // Frequency goes from zero to 1/(2*resolution), the Nyquist frequency in
  // nrows_padded/2 increments.
  double dfx = 1/(dem_res*Lx);
  double dfy = 1/(dem_res*Ly);
  //----------------------------------------------------------------------------
  cout << "  Lowpass filter with edges controlled by radial Gaussian function between f1 = " << f1 << " and f2 = " << f2 << endl;
  double f; // radial frequency
  double weight; // Filter weight
  double sigma;   // Standard Deviation of Gaussian edge
  for (int i=0; i < Ly; ++i)
  {
    for (int j=0; j < Lx; ++j)
    {
      double x = j;
      double y = i;
      f = sqrt((y - (Ly/2))*(y - (Ly/2))*dfy*dfy + (x - (Lx/2))*(x - (Lx/2))*dfx*dfx); // distance from centre to this point. Converting position in frequency into an absolute frequency
      if (f < f1)
      {
        weight = 1;
      }
      else
      {
        if (f2 > f1)
        {
          sigma = sqrt((f2-f1)*(f2-f1))/3;
          weight = exp(-(f - f1)*(f-f1)/(2*sigma*sigma));
        }
        else
        {
          weight = 0; // this is for the case that f1 = f2 and essentially the weighting function acts with a hard edge at f=f1=f2.
        }
      }
      FilteredSpectrumReal[i][j] = weight*RawSpectrumReal[i][j];
      FilteredSpectrumImaginary[i][j] = weight*RawSpectrumImaginary[i][j];
    }
  }
}
//------------------------------------------------------------------------------
// HIGHPASS FILTER
// Filter array to retain frequencies above f1.  The filter edge is a radial
// gaussian function with a SD of |f2-f1|/3.
void LSDRaster::highpass_filter(Array2D<double>& RawSpectrumReal, Array2D<double>& RawSpectrumImaginary, Array2D<double>& FilteredSpectrumReal, Array2D<double>& FilteredSpectrumImaginary, int Lx, int Ly, double dem_res, double f1, double f2)
{
  // CALCULATE FREQUENCY INCREMENTS - for generation of power spectrum
  // Frequency goes from zero to 1/(2*resolution), the Nyquist frequency in
  // nrows_padded/2 increments.
  double dfx = 1/(dem_res*Lx);
  double dfy = 1/(dem_res*Ly);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  cout << "    Highpass filter with edges controlled by radial Gaussian function between f1 = " << f1 << " and f2 = " << f2 << endl;
  double f; // radial frequency
  double weight; // Filter weight
  double sigma = sqrt((f2-f1)*(f2-f1))/3; // Standard Deviation of Gaussian filter
  for (int i=0; i < Ly; ++i)
  {
    for (int j=0; j < Lx; ++j)
    {
      double x = j;
      double y = i;
      f = sqrt((y - (Ly/2))*(y - (Ly/2))*dfy*dfy + (x - (Lx/2))*(x - (Lx/2))*dfx*dfx); // distance from centre to this point. Converting position in frequency into an absolute frequency
      if (f > f2)
      {
        weight = 1;
      }
      else
      {
        if (f2 > f1)
        {
          sigma = sqrt((f2-f1)*(f2-f1))/3;
          weight = exp(-(f - f2)*(f-f2)/(2*sigma*sigma));
        }
        else
        {
          weight = 0; // this is for the case that f1 = f2 and essentially the weighting function acts with a hard edge at f=f1=f2.
        }
      }
      FilteredSpectrumReal[i][j] = weight*RawSpectrumReal[i][j];
      FilteredSpectrumImaginary[i][j] = weight*RawSpectrumImaginary[i][j];
    }
  }
}

//------------------------------------------------------------------------------
// WIENER FILTER
// The Wiener filter is a spectral filter that removes noise from an image or
// DEM.  Essentially, it works on the principle that the observed spectrum
// contains the superposition of the real signal and an additional noise signal,
// which we want to remove.  If we know, or can make a reasonable guess at the
// noise, N(f), and signal, S(f), parts of the spectrum then we can remove the
// noise using the filter:
//
//        phi(f) = |S(f)|^2/(|S(f)|^2 + |N(f)|^2)
//
// For topography; at long wavelengths the topographic signal obeys an
// approximate power law relationship between amplitude and frequency,
// decreasing as the frequency increases (and wavelength decreases).  Noise
// typically dominates the high frequency part of the spectrum.  Thus at high
// frequencies the spectrum is dominated by noise, and the filter weight goes to
// zero.  In contrast, at low frequencies, the signal dominates and the filter
// weight goes to 1.
//
void LSDRaster::wiener_filter(Array2D<double>& RawSpectrumReal, Array2D<double>& RawSpectrumImaginary, Array2D<double>& FilteredSpectrumReal, Array2D<double>& FilteredSpectrumImaginary, int Lx, int Ly, double dem_res, double WSS)
{
  vector<double> RadialPSD;
  vector<double> RadialFrequency;
  // CALCULATE FREQUENCY INCREMENTS - for generation of power spectrum
  // Frequency goes from zero to 1/(2*resolution), the Nyquist frequency in
  // nrows_padded/2 increments.
  double dfx = 1/(dem_res*Lx);
  double dfy = 1/(dem_res*Ly);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // GET RADIAL POWER SPECTRUM
  // For forward transform, return the spectral power of the topography both
  // in a 2D array, and also as a one dimensional array of radial frequency
  calculate_radial_PSD(RawSpectrumReal, RadialPSD, RadialFrequency, Lx, Ly, WSS, dem_res);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // FIT POWER LAW TO SPECTRUM BETWEEN RANGE OF WAVELENGTHS 1000m - 100m (THE
  // RANGE EXPECTED TO FALL WITHIN WAVELENGTHS CONTROLLED BY RIDGE-VALLEY
  // TOPOGRAPHY)
  cout << "  Fitting power law to data..." << endl;
  vector<double> LogRadialFrequency;
  vector<double> LogRadialPSD;
  int n_freqs = RadialFrequency.size();
  double f_low = 0.001; // frequency at wavelength of 1000m
  double f_high = 0.01; // frequency at wavelength of 100m
  double m_model,logc_model,c_model;      // Coefficients of power law fit => logPSD = logc + m*log(freq) => PSD = c*freq^m
  for (int i = 0; i < n_freqs; ++i)
  {
    if(RadialFrequency[i] <= f_high && RadialFrequency[i] >= f_low)
    {
      LogRadialFrequency.push_back(log10(RadialFrequency[i]));
      LogRadialPSD.push_back(log10(RadialPSD[i]));
    }
  }
  // Least squares regression
  vector<double> residuals;
  vector<double> regression_results = simple_linear_regression(LogRadialFrequency, LogRadialPSD, residuals);
  m_model = regression_results[0];
  logc_model = regression_results[1];

  //linear_fit(LogRadialFrequency, LogRadialPSD, m_model, logc_model);
  c_model = pow(10,logc_model);

  // Extend relationship across entire frequency range to produce model spectrum
  vector<double> RadialPSD_model(n_freqs,0.0);
  for (int i = 0; i < n_freqs; ++i)
  {
    RadialPSD_model[i] = c_model*pow(RadialFrequency[i],m_model);
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // GET MEAN AMPLITUDE OF SPECTRUM CLOSE TO NYQUIST FREQUENCY AS ESTIMATE OF
  // WHITE NOISE SPECTRUM

  // MAKE INTO IF LOOP WITH CONDITION FOR WHITE NOISE VS MODEL NOISE

  // Note that this assumes that high frequency "noise" is random.  We might
  // expect that the high frequency content might actually be more structured
  // than that, particularly if the high frequency signal is being produced by
  // rock exposures, unfiltered vegetation, or even pesky gophers.
  cout << "  Estimating noise distribution... ";

  // i) Basically going to use a simple high-pass filter, and then average the
  // amplitude to give noise.
  //vector<double> WhiteNoise(n_freqs,0.0);
  double f_highpass = pow(10,-0.7);
  //double WhiteNoiseAmplitude = 0;
  //for (int i = 0; i < RadialPSD_average.size(); ++i)
  //{
  //  if(RadialFrequency_grouped[i] >= f_highpass)
  //  {
  //    WhiteNoiseAmplitude += RadialPSD_average[i];
  //  }
  //}
  //WhiteNoiseAmplitude = WhiteNoiseAmplitude/n_freqs_noise;
  //for (int i = 0; i < n_freqs; ++i)
  //{
  //  WhiteNoise[i] = WhiteNoiseAmplitude;
  //}
  //----------------------------------------------------------------------------
  // ii) Alternatively can model noise using a linear fit through the high
  // frequency part of the spectrum - in particular it would be interesting to
  // see if it approximates pink noise, which is ubiquitous to natural systems
  // with self-organised criticality.
  double m_noise,logc_noise,c_noise;
  vector<double> LogRadialFrequency_highpass;
  vector<double> LogRadialPSD_highpass;
  for (int i = 0; i < n_freqs; ++i)
  {
    if(RadialFrequency[i] >= f_highpass)
    {
      LogRadialFrequency_highpass.push_back(log10(RadialFrequency[i]));
      LogRadialPSD_highpass.push_back(log10(RadialPSD[i]));
    }
  }
  // Least squares regression
  regression_results = simple_linear_regression(LogRadialFrequency_highpass, LogRadialPSD_highpass,  residuals);
  m_noise = regression_results[0];
  logc_noise = regression_results[1];


  //linear_fit(LogRadialFrequency_highpass, LogRadialPSD_highpass, m_noise, logc_noise);


  c_noise = pow(10,logc_noise);
  // Extend relationship across entire frequency range to produce model spectrum
  vector<double> Noise_model(n_freqs,0.0);
  for (int i = 0; i < n_freqs; ++i)
  {
    Noise_model[i] = c_noise*pow(RadialFrequency[i],m_noise);
  }
  //cout << "Modeled noise exponent = " << m_noise << endl;
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // WIENER FILTER
  // Determine Wiener Coefficients and apply to spectrum
  // WienerCoefficient = Signal/(Signal + Noise).  Basically acts as a lowpass
  // filter to remove noise from image.
  double model;
  double noise;
  double f; // radial frequency
  double WienerCoefficient; // Filter weight
  for (int i=0; i < Ly; ++i)
  {
    for (int j=0; j < Lx; ++j)
    {
      double x = j;
      double y = i;
      f = sqrt((y - (Ly/2))*(y - (Ly/2))*dfy*dfy + (x - (Lx/2))*(x - (Lx/2))*dfx*dfx); // Radial Frequency
      model = c_model*pow(f,m_model);
      noise = c_noise*pow(f,m_noise);
      if (f == 0) WienerCoefficient = 1;
      else WienerCoefficient = model/(model+noise);
      FilteredSpectrumReal[i][j] = WienerCoefficient*RawSpectrumReal[i][j];
      FilteredSpectrumImaginary[i][j] = WienerCoefficient*RawSpectrumImaginary[i][j];
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MAIN FUNCTIONS USING SPECTRAL FILTERS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// FAST FOURIER TRANSFORM FILTER FOR A REAL, 2-DIMENSIONAL DATASET.
//
// Note that FLow <= FHigh
//
// There are three types of filters depending on the intentions of the user
//
// BANDPASS FILTER (FilterType = 1)
// Filter array to band between frequency bands f1 and f2.  The bandpass filter
// is a gaussian filter centred at (f1+f2)/2 and with a SD of |f2-f1|/6.
//
// LOWPASS FILTER (FilterType = 2)
// Filter array to retain frequencies below f1.  The filter edge is a radial
// gaussian function with a SD of |f2-f1|/3.  f1 is the frequency below which
// the filter starts to taper; f2 is the frequency at which the filter tapers to
// zero. If f1 = f2, the edge is effectively a step function.
// HIGHPASS FILTER (FilterType = 3)
//
// Filter array to retain frequencies above f2.  The filter edge is a radial
// gaussian function with a SD of |f2-f1|/3.  f2 is the frequency below which
// the filter starts to taper; f1 is the frequency at which the filter tapers to
// zero. If f1 = f2, the edge is effectively a step function.
//
// A second type of bandpass filter is possible by combining the highpass and
// lowpass filters.
//------------------------------------------------------------------------------
// David Milodowski, 10/12/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::fftw2D_filter(int FilterType, double FLow, double FHigh)
{
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // for FORWARD TRANSFORM
  cout << "\n***fftw_2Dfilt_v1.1: spectral filtering of array***" << endl;
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DETREND DATA => DO NOT WINDOW!
	// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
	// LOCAL SLOPE ax + by + c = z
	Array2D<double> zeta_detrend(NRows,NCols);
	Array2D<double> trend_plane(NRows,NCols);
  detrend2D(RasterData, zeta_detrend, trend_plane, NRows, NCols, NoDataValue);
  //double WSS = 1; // dataset is not windowed

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 2D DISCRETE FAST FOURIER TRANSFORM
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // PAD DATA WITH ZEROS TO A POWER OF TWO (facilitates FFT)
  int Ly = int(pow(2,ceil(log(NRows)/log(2))));
  int Lx = int(pow(2,ceil(log(NCols)/log(2))));

  Array2D<double> zeta_padded(Ly,Lx);
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      if (i<NRows && j<NCols)
      {
        zeta_padded[i][j] = zeta_detrend[i][j];
      }
      else
      {
        zeta_padded[i][j]=0;
      }
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DO 2D FORWARD FAST FOURIER TRANSFORM
  int transform_direction = -1;
  Array2D<double> spectrum_real(Ly,Lx);
  Array2D<double> spectrum_imaginary(Ly,Lx);
  dfftw2D_fwd(zeta_padded, spectrum_real, spectrum_imaginary, transform_direction, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // REARRANGE SPECTRUM SO THAT ORIGIN IS AT THE CENTRE
  Array2D<double> spectrum_real_shift(Ly,Lx);
  Array2D<double> spectrum_imaginary_shift(Ly,Lx);
  shift_spectrum(spectrum_real, spectrum_imaginary, spectrum_real_shift, spectrum_imaginary_shift, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // INVERSE TRANSFORM
  // For inverse tranform, take 2D power spectrum.  Filter for desired frequncy
  // band and return spectrally filtered topography
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // APPLY FILTER
  // First, remove the frequency ranges that are not wanted
  Array2D<double> FilteredSpectrumReal(Ly,Lx,0.0);
  Array2D<double> FilteredSpectrumImaginary(Ly,Lx,0.0);

  // SET FILTER PARAMETERS
  if (FilterType == 1)
  {
    bandpass_filter(spectrum_real_shift, spectrum_imaginary_shift, FilteredSpectrumReal, FilteredSpectrumImaginary, Lx, Ly, DataResolution, FLow, FHigh);
  }
  else if (FilterType == 2)
  {
    lowpass_filter(spectrum_real_shift, spectrum_imaginary_shift, FilteredSpectrumReal, FilteredSpectrumImaginary, Lx, Ly, DataResolution, FLow, FHigh);
  }
  else if (FilterType == 3)
  {
    highpass_filter(spectrum_real_shift, spectrum_imaginary_shift, FilteredSpectrumReal, FilteredSpectrumImaginary, Lx, Ly, DataResolution, FLow, FHigh);
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DE-SHIFT ORIGIN OF SPECTRUM
  // Return filtered spectrum to original format (de-shifted)
  Array2D<double> FilteredSpectrumReal_deshift(Ly,Lx);
  Array2D<double> FilteredSpectrumImaginary_deshift(Ly,Lx);
  shift_spectrum_inv(FilteredSpectrumReal, FilteredSpectrumImaginary, FilteredSpectrumReal_deshift, FilteredSpectrumImaginary_deshift, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DO 2D INVERSE FAST FOURIER TRANSFORM
  transform_direction = 1;
  Array2D<double> FilteredTopographyPadded(Ly,Lx);
  dfftw2D_inv(FilteredSpectrumReal_deshift, FilteredSpectrumImaginary_deshift, FilteredTopographyPadded, transform_direction, Ly, Lx);
  // Need to scale output by the number of pixels, and by the Hann window to
  // recover the topography, before adding the planar trend back to the dataset
  cout << "  Scaling output filtered topography..." << endl;
  Array2D<double> FilteredTopography(NRows,NCols,NoDataValue);
  for (int i=0; i < NRows; ++i)
  {
    for (int j=0; j < NCols; ++j)
    {
      FilteredTopography[i][j] = FilteredTopographyPadded[i][j]/(Lx*Ly) + trend_plane[i][j];
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  LSDRaster FilteredTopographyRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilteredTopography);
  return FilteredTopographyRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// WIENER FILTER FOR A REAL, 2-DIMENSIONAL DATASET.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// The Wiener filter is a spectral filter that removes noise from an image or
// DEM.  Essentially, it works on the principle that the observed spectrum
// contains the superposition of the real signal and an additional noise signal,
// which we want to remove.  If we know, or can make a reasonable guess at the
// noise, N(f), and signal, S(f), parts of the spectrum then we can remove the
// noise using the filter:
//
//        phi(f) = |S(f)|^2/(|S(f)|^2 + |N(f)|^2)
//
// For topography; at long wavelengths the topographic signal obeys an
// approximate power law relationship between amplitude and frequency,
// decreasing as the frequency increases (and wavelength decreases).  Noise
// typically dominates the high frequency part of the spectrum.  Thus at high
// frequencies the spectrum is dominated by noise, and the filter weight goes to
// zero.  In contrast, at low frequencies, the signal dominates and the filter
// weight goes to 1.
//
// The optimal wiener filter is described in more detail in Numerical Recipes,
// 13.3, p149.
//
// The exact structure of the noise is worth thinking about.  White noise, which
// is random, has equal power across all wavelengths.  In the instance of
// topography, noise can be created by a whole range of sources, from rock
// exposure, to pit and mound topography, to unfiltered vegetation etc.  It is
// likely that these sources will not produce purely white noise, but rather
// will show an element of structure.  This program makes two assumptions about
// the noise: i) it dominates the signal at high frequencies (close to the
// Nquist frequency) and ii) we can reasonably model this using a linear fit in
// log-log space - i.e. it obeys some form of power law function between
// frequency and amplitude.  Note that if the noise in the signal is really
// white noise, then the power law function for the noise would simply have an
// exponent of zero.  I prefer this formulation because it permits the
// characterisation of the noise model without assuming that the noise has a
// particular structure (white noise, pink noise etc.)
//
//------------------------------------------------------------------------------
// David Milodowski, 10/12/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::fftw2D_wiener()
{
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DETREND DATA => DO NOT WINDOW!
	// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
	// LOCAL SLOPE ax + by + c = z
	Array2D<double> zeta_detrend(NRows,NCols);
	Array2D<double> trend_plane(NRows,NCols);
  detrend2D(RasterData, zeta_detrend, trend_plane, NRows, NCols, NoDataValue);
  double WSS = 1; // dataset is not windowed

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // 2D DISCRETE FAST FOURIER TRANSFORM
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // PAD DATA WITH ZEROS TO A POWER OF TWO (facilitates FFT)
  int Ly = int(pow(2,ceil(log(NRows)/log(2))));
  int Lx = int(pow(2,ceil(log(NCols)/log(2))));

  Array2D<double> zeta_padded(Ly,Lx);
  for (int i=0;i<Ly;++i)
  {
    for (int j=0;j<Lx;++j)
    {
      if (i<NRows && j<NCols)
      {
        zeta_padded[i][j] = zeta_detrend[i][j];
      }
      else
      {
        zeta_padded[i][j]=0;
      }
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DO 2D FORWARD FAST FOURIER TRANSFORM
  int transform_direction = -1;
  Array2D<double> spectrum_real(Ly,Lx);
  Array2D<double> spectrum_imaginary(Ly,Lx);
  dfftw2D_fwd(zeta_padded, spectrum_real, spectrum_imaginary, transform_direction, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // REARRANGE SPECTRUM SO THAT ORIGIN IS AT THE CENTRE
  Array2D<double> spectrum_real_shift(Ly,Lx);
  Array2D<double> spectrum_imaginary_shift(Ly,Lx);
  shift_spectrum(spectrum_real, spectrum_imaginary, spectrum_real_shift, spectrum_imaginary_shift, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // INVERSE TRANSFORM
  // For inverse tranform, take 2D power spectrum.  Filter for desired frequncy
  // band and return spectrally filtered topography
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // APPLY FILTER
  // First, remove the frequency ranges that are not wanted
  Array2D<double> FilteredSpectrumReal(Ly,Lx,0.0);
  Array2D<double> FilteredSpectrumImaginary(Ly,Lx,0.0);
  // SET FILTER PARAMETERS
  wiener_filter(spectrum_real_shift, spectrum_imaginary_shift, FilteredSpectrumReal, FilteredSpectrumImaginary, Lx, Ly, DataResolution, WSS);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DE-SHIFT ORIGIN OF SPECTRUM
  // Return filtered spectrum to original format (de-shifted)
  Array2D<double> FilteredSpectrumReal_deshift(Ly,Lx);
  Array2D<double> FilteredSpectrumImaginary_deshift(Ly,Lx);
  shift_spectrum_inv(FilteredSpectrumReal, FilteredSpectrumImaginary, FilteredSpectrumReal_deshift, FilteredSpectrumImaginary_deshift, Ly, Lx);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // DO 2D INVERSE FAST FOURIER TRANSFORM
  transform_direction = 1;
  Array2D<double> FilteredTopographyPadded(Ly,Lx);
  dfftw2D_inv(FilteredSpectrumReal_deshift, FilteredSpectrumImaginary_deshift, FilteredTopographyPadded, transform_direction, Ly, Lx);
  // Need to scale output by the number of pixels, and by the Hann window to
  // recover the topography, before adding the planar trend back to the dataset
  cout << "  Scaling output filtered topography..." << endl;
  Array2D<double> FilteredTopography(NRows,NCols,NoDataValue);
  for (int i=0; i < NRows; ++i)
  {
    for (int j=0; j < NCols; ++j)
    {
      FilteredTopography[i][j] = FilteredTopographyPadded[i][j]/(Lx*Ly) + trend_plane[i][j];
    }
  }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  LSDRaster FilteredTopographyRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilteredTopography);
  return FilteredTopographyRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

#endif
