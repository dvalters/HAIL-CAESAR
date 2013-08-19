//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDIndexRaster.cpp
// cpp file for the LSDIndexRaster object
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
// Version 0.0.1		20/08/2012
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
#include "TNT/tnt.h"
#include "LSDIndexRaster.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDIndexRaster_CPP
#define LSDIndexRaster_CPP

// operators
LSDIndexRaster& LSDIndexRaster::operator=(const LSDIndexRaster& rhs)
 {
  if (&rhs != this)
   {
    create(rhs.get_NRows(),rhs.get_NCols(),rhs.get_XMinimum(),rhs.get_YMinimum(),
           rhs.get_DataResolution(), rhs.get_NoDataValue(), rhs.get_RasterData() );
   }
  return *this;
 }

// the create function. This is default and throws an error
void LSDIndexRaster::create()
{
	cout << "You need to initialize with a filename!" << endl;
	exit(EXIT_FAILURE);
}

// this creates a raster using an infile
void LSDIndexRaster::create(string filename, string extension)
{
	read_raster(filename,extension);
}

// this creates a raster filled with no data values
void LSDIndexRaster::create(int nrows, int ncols, double xmin, double ymin,
            double cellsize, int ndv, Array2D<int> data)
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
void LSDIndexRaster::read_raster(string filename, string extension)
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
		Array2D<int> data(NRows,NCols,NoDataValue);

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
		Array2D<int> data(NRows,NCols,NoDataValue);

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
					data[i][j] = int(temp);
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
void LSDIndexRaster::write_raster(string filename, string extension)
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



#endif
