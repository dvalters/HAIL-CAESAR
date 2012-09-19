//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDRaster.h
// header file for the LSDRaster object
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


#include <string>
#include <vector>
#include "TNT/tnt.h"
//#include "LSDIndexRaster.hpp"
//#include "LSDFlowInfo.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDRaster_H
#define LSDRaster_H

class LSDRaster
{
	public:
	// declare the LSDFlowInfo object to be a friend class
	// this gives the LSDFlowInfo object access to the data elements
	// in the LSDRaster
	friend class LSDFlowInfo;

	LSDRaster()										{ create(); }
	LSDRaster(string filename, string extension)	{ create(filename, extension); }
	LSDRaster(int nrows, int ncols, double xmin, double ymin,
	          double cellsize, double ndv, Array2D<double> data)
								{ create(nrows, ncols, xmin, ymin, cellsize, ndv, data); }

	// get functions
	// these get data elements
	int get_NRows() const				{ return NRows; }
	int get_NCols() const				{ return NCols; }
	double get_XMinimum() const			{ return XMinimum; }
	double get_YMinimum() const			{ return YMinimum; }
	double get_DataResolution() const	{ return DataResolution; }
	double get_NoDataValue() const		{ return NoDataValue; }
	Array2D<double> get_RasterData() const
										{ return RasterData; }

	// operator
	LSDRaster& operator=(const LSDRaster& LSDR);


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
    double get_data_element(int row, int column)	{ return RasterData[row][column]; }


    // functions for data analysis

    // this calcualtes coefficeint matrices for calcualting a variety of
    // surface metrics such as slope, aspect, curvature, etc.
    void calculate_polyfit_coefficient_matrices(double window_radius,
									Array2D<double>& a, Array2D<double>& b,
									Array2D<double>& c, Array2D<double>& d,
									Array2D<double>& e, Array2D<double>& f);


	LSDRaster calculate_polyfit_slope(Array2D<double> a, Array2D<double> b);
	LSDRaster calculate_polyfit_aspect(Array2D<double> d,Array2D<double> e);
	LSDRaster calculate_polyfit_curvature(Array2D<double> d,Array2D<double> e);

	//LSDRaster calculate_polyfit_classification(Array2D<double> a, Array2D<double> b,
	//								  Array2D<double> c, Array2D<double> d,
	//								  Array2D<double> e, Array2D<double> f);


	// hydrology tools
	LSDRaster fill();
	void fill_iterator(Array2D<double>& fill_data, int i, int j);


	protected:

	// data for georeferencing
	int NRows;			// number of rows
	int NCols;			// number of columns
	double XMinimum;
	double YMinimum;

	// metadata
	double DataResolution;
	double NoDataValue;

	// the actual raster
	Array2D<double> RasterData;

	private:
	void create();
	void create(string filename, string extension);
	void create(int ncols, int nrows, double xmin, double ymin,
	            double cellsize, double ndv, Array2D<double> data);

};

#endif
