//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDIndexRaster.h
// header file for the LSDIndexRaster object
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
// Version 0.0.1		28/08/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <string>
#include <vector>
#include "TNT/tnt.h"
using namespace std;
using namespace TNT;

#ifndef LSDIndexRaster_H
#define LSDIndexRaster_H


class LSDIndexRaster
{
	public:
	LSDIndexRaster()										{ create(); }
	LSDIndexRaster(string filename, string extension)	{ create(filename, extension); }
	LSDIndexRaster(int nrows, int ncols, double xmin, double ymin,
	          double cellsize, int ndv, Array2D<int> data)
								{ create(nrows, ncols, xmin, ymin, cellsize, ndv, data); }

	// get functions
	// these get data elements
	int get_NRows() const				{ return NRows; }
	int get_NCols() const				{ return NCols; }
	double get_XMinimum() const			{ return XMinimum; }
	double get_YMinimum() const			{ return YMinimum; }
	double get_DataResolution() const	{ return DataResolution; }
	int get_NoDataValue() const			{ return NoDataValue; }
	Array2D<int> get_RasterData() const
										{ return RasterData; }

	// operator
	LSDIndexRaster& operator=(const LSDIndexRaster& LSDIR);

	// tools for reading and writing DEMs
	// currently the supported formats are .asc and .flt,
	// both exported and imported by arcmap
	// the filename is the string of characters before the '.' in the extension
	// the extension is the characters after the '.'
	// for example, if the full filename is my_dem.01.asc
	// then filename = "my_dem.01"
	// and extension = "asc"
	// for float files both a data file and a header is read/written
	// the header file must have the same filename, before extention, of
	// the raster data, and the extension must be .hdr
	// EXPLANATION OF HOW TO EXPORT AND IMPORT FROM ARCMAP HERE
    void read_raster(string filename, string extension);
    void write_raster(string filename, string extension);

    // functions for extracting data elements
    int get_data_element(int row, int column)	{ return RasterData[row][column]; }

	protected:

	// data for georeferencing
	int NRows;			// number of rows
	int NCols;			// number of columns
	double XMinimum;
	double YMinimum;

	// metadata
	double DataResolution;
	int NoDataValue;

	// the actual raster
	Array2D<int> RasterData;

	private:
	void create();
	void create(string filename, string extension);
	void create(int ncols, int nrows, double xmin, double ymin,
	            double cellsize, int ndv, Array2D<int> data);

};

#endif
