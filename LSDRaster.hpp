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


#include <string>
#include <vector>
#include "TNT/tnt.h"
#include "LSDIndexRaster.hpp"
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
										{ return RasterData.copy(); }

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


	// Functions relating to shading, shadowing and shielding
	// This function calculates Hillshade
	// it uses the Arc algorithm (but is in c++ so much faster
	LSDRaster hillshade(double altitude, double azimuth, double z_factor);
	// shadow function: called by toposhield and should not be called directly
	// hence the Array rather than raster output
	Array2D<double> Shadow(int theta, int phi);
	// this calucaltes the shielding factor for cosmogenic analyis
	LSDRaster TopoShield(int theta_step, int phi_step);

    // this looks for isolated instances of no data
    void check_isolated_nodata();

    // functions for extracting data elements
    double get_data_element(int row, int column)	{ return RasterData[row][column]; }

    // this calculates coefficeint matrices for calculating a variety of
    // surface metrics such as slope, aspect, curvature, etc.
    void calculate_polyfit_coefficient_matrices(double window_radius,
									Array2D<double>& a, Array2D<double>& b,
									Array2D<double>& c, Array2D<double>& d,
									Array2D<double>& e, Array2D<double>& f);

	// a series of functions for retrieving derived data from the polyfit calculations
	LSDRaster calculate_polyfit_elevation(Array2D<double>& f);
	LSDRaster calculate_polyfit_slope(Array2D<double>& d, Array2D<double>& e);
	LSDRaster calculate_polyfit_aspect(Array2D<double>& d,Array2D<double>& e);
	LSDRaster calculate_polyfit_curvature(Array2D<double>& a,Array2D<double>& b);
  	LSDRaster calculate_polyfit_planform_curvature(Array2D<double>& a, Array2D<double>& b, Array2D<double>& c,
  													Array2D<double>& d, Array2D<double>& e);
  	LSDRaster calculate_polyfit_profile_curvature(Array2D<double>& a, Array2D<double>& b, Array2D<double>& c,
  													Array2D<double>& d, Array2D<double>& e);
	LSDRaster calculate_polyfit_tangential_curvature(Array2D<double>& a, Array2D<double>& b, Array2D<double>& c,
  													Array2D<double>& d, Array2D<double>& e);
	LSDIndexRaster calculate_polyfit_classification(Array2D<double>& a, Array2D<double>& b, Array2D<double>& c,
	                                                Array2D<double>& d, Array2D<double>& e);

	// Gets the hilltop curvature raster. Takes an index raster that has hilltops
	LSDRaster 	get_hilltop_curvature(LSDRaster& curvature, LSDIndexRaster& Hilltops);

	// surface roughness
	void calculate_polyfit_directional_cosines(Array2D<double>& d, Array2D<double>& e, Array2D<double>& l,
	                                           Array2D<double>& m, Array2D<double>& n);
	void calculate_orientation_matrix_eigenvalues(double window_radius,
													Array2D<double>& l, Array2D<double>& m,
													Array2D<double>& n, Array2D<double>& s1,
                    								Array2D<double>& s2, Array2D<double>& s3);

  	// Rock exposure index
  	void calculate_plane_coefficient_matrices(double window_radius, Array2D<double>& a_plane,
										Array2D<double>& b_plane, Array2D<double>& c_plane);
  	LSDIndexRaster calculate_REI(Array2D<double>& a_plane, Array2D<double>& b_plane, double CriticalSlope);

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
	// 6 tangential curvature
	// 7 classification
	// SMM 19-12-2012
	void calculate_and_print_polyfit_rasters(double window_radius,
											string file_prefix, vector<int> file_list);

	// this function combines the polyfit functions and the roughness function in one package that
	// is data efficient becasue it only requires one calcualtion of the polyfit matrices.
	// it takes the window radius of the polyfit and the window of the roughness calcualtion
	// the file codes in the vector are:
	// 0 slope
	// 1 aspect
	// 2 curvature
	// 3 planform curvature
	// 4 profile curvature
	// 6 tangential curvature
	// 7 classification
	// 8 roughness s1
	// 9 roughness s2
	// 10 roughness s3
	// SMM 19-12-2012
	void calculate_and_print_polyfit_and_roughness_rasters(double window_radius, double roughness_radius,
										string file_prefix, vector<int> file_list);

  	// this function combines the polyfit functions and the roughness function in one package that
	// is data efficient becasue it only requires one calculation of the polyfit matrices.
	// it takes the window radius of the polyfit and the window of the roughness calcualtion
	// the file codes in the vector are:
	// 0 roughness s1
	// 1 roughness s2
	// 2 roughness s3
	// DTM 15-07-2013
  	void calculate_roughness_rasters(double window_radius, double roughness_radius, string file_prefix, vector<int> file_code);


	// hydrology tools
	LSDRaster fill();
	LSDRaster fill(double& MinSlope);
	void fill_iterator(Array2D<double>& fill_data, int i, int j);

  	LSDRaster hilltop_flow_routing(Array2D<int>& StreamNetwork, Array2D<double>& Hilltops, Array2D<double>& Aspect,
                                          Array2D<double>& Curvature);

	// multidirection flow routing
	LSDRaster MDFlow(vector<string> BoundaryConditions);
	LSDRaster FreemanMDFlow();
	LSDRaster QuinnMDFlow();

	// channel head identification
	LSDRaster calculate_pelletier_channel_heads(int NRows,int NCols,int XMinimum,int YMinimum,double DataResolution,
	                                                          int NoDataValue,double window_radius,
                                                          double tan_curv_threshold,Array2D<double>& tan_curv_array);

	// some tools associated with ridgeline analyis
	LSDRaster RidgeSample(Array2D<double>& Ridges);
	LSDRaster RidgeSmoother(int WindowRadius);
	LSDRaster RidgeBuffer(int BufferRadius);
	LSDRaster BasinAverager(LSDIndexRaster& Basins);



	// Smoothing tools
	//Nonlocal Means Filtering - Default values following Baudes et al [2005]
	LSDRaster NonLocalMeansFilter(int WindowRadius=2, int SimilarityRadius=2, int DegreeFiltering=2, double Sigma=1.25);
	void PadRasterSymmetric(Array2D<double>& PaddedRasterData, int& SimilarityRadius);
	void MakeGaussianKernel(Array2D<double>& Kernel, double sigma, int SimilarityRadius);






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
