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
// David Milodowski, University of Edinburgh
// Martin D. Hurst, British Geological Survey
// <your name here>
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Version 0.0.2		29/08/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "TNT/tnt.h"
#include "TNT/jama_lu.h"
#include "LSDRaster.hpp"
//#include "LSDFlowInfo.hpp"
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
	cout << "You need to initialize with a filename!" << endl;
	exit(EXIT_FAILURE);
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
		cout << "dimesntion of data is not the same as stated in NRows!" << endl;
		exit(EXIT_FAILURE);
	}
	if (RasterData.dim2() != NCols)
	{
		cout << "dimesntion of data is not the same as stated in NRows!" << endl;
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
//
// calculate_polyfit_coefficient_matrices
//
// this function calcualtes 6 coefficient matrices that allow the user to
// then calcualte slope, curvature, aspect, a classification for finding saddles and peaks
// and other metrics
//
// The coefficient matrices are overwritten during the running of this member function
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
	double x,y,zeta;
	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
	      x_kernel[i][j]=(i-kr)*DataResolution;
	      y_kernel[i][j]=(j-kr)*DataResolution;
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
			if(i-kr < 0 || i+kr+1 > NRows || j-kr < 0 || j+kr+1 > NCols)
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
					    }
					}
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
				}
				ndv_present = 0;
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calucaltes the slope based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_slope(Array2D<double> d, Array2D<double> e)
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
//
// This function calucaltes the curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_curvature(Array2D<double> a, Array2D<double> b)
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
// This function calucaltes the aspect based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_aspect(Array2D<double> d, Array2D<double> e)
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
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Classification goes here
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
	for (int i=1; i<NRows-1; ++i)
	{
		cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for (int j=1; j<NCols-1; ++j)
		{
			if (FilledRasterData[i][j] == NoDataValue || FilledRasterData[i-1][j-1] == NoDataValue
			         || FilledRasterData[i-1][j] == NoDataValue || FilledRasterData[i-1][j+1] == NoDataValue
			         || FilledRasterData[i][j+1] == NoDataValue || FilledRasterData[i+1][j+1] == NoDataValue
			         || FilledRasterData[i+1][j] == NoDataValue || FilledRasterData[i+1][j-1] == NoDataValue
			         || FilledRasterData[i][j-1] == NoDataValue)
			{}
			else fill_iterator(FilledRasterData,i,j);
		}
	}

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
	{}
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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function calcualtes the receiver nodes
// it returns the receiver vector r_i
// it also returns a flow direction array in this ordering:
//
// 7  0 1
// 6 -1 2
// 5  4 3
//
// note this is different from ArcMap flowdirection
//	int Arc_flowdir;			// flow direction in arcmap format
//								// 32  64  128
//								// 16  --  1
//								// 8    4  2
// one can convert nthese indices using the LSDIndexRaster object
// note in arc the row index increases down (to the south)

#endif
