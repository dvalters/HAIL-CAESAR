//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDRaster
// Land Surface Dynamics Raster
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for manipulating
//  and analysing raster data, with a particular focus on topography
//
// Developed by:
//  Simon M. Mudd
//  Martin D. Hurst
//  David T. Milodowski
//  Stuart W.D. Grieve
//  Declan A. Valters
//  Fiona Clubb
//
// Copyright (C) 2013 Simon M. Mudd 2013
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
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


/** @file LSDRaster.hpp
@author Simon M. Mudd, University of Edinburgh
@author David Milodowski, University of Edinburgh
@author Martin D. Hurst, British Geological Survey
@author Stuart W. D. Grieve, University of Edinburgh
@author Fiona Clubb, University of Edinburgh

@version Version 1.0.0
@brief Main analysis object to interface with other LSD objects.
@details This object contains a diverse range of geomophological
analysis routines which can be used in conjunction with the other objects in
the package.

<b>change log</b>an
MASSIVE MERGE: Starting version 1.0.0 on 15/07/2013

@date 16/07/2013
*/

/**
@mainpage
This  is the documentation for Edinburgh Topographic Analysis Package (ETAP),
incorporating LSDRaster.

These pages will help you get started using this software.

\image html ./logo.png


Tools are included to:
- Generate topographic metrics
- Perform Chi analysis
- And other important science stuff
.

@author Simon M. Mudd, University of Edinburgh
@author David Milodowski, University of Edinburgh
@author Martin D. Hurst, British Geological Survey
@author Stuart W. D. Grieve, University of Edinburgh
@author Fiona Clubb, University of Edinburgh

*/

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------



#ifndef LSDRaster_H
#define LSDRaster_H

#include <string>
#include <vector>
#include <map>
#include "TNT/tnt.h"
#include "LSDIndexRaster.hpp"
#include "LSDShapeTools.hpp"
using namespace std;
using namespace TNT;

///@brief Main analysis object to interface with other LSD objects.
class LSDRaster
{
  public:
  // declare the LSDFlowInfo object to be a friend class
  // this gives the LSDFlowInfo object access to the data elements
  // in the LSDRaster
  /// @brief Object to perform flow routing.
  friend class LSDFlowInfo;

  /// @brief The create function. This is default and throws an error.
  LSDRaster()		       	{ create(); }
  
  /// @brief Create an LSDRaster from a file.
  /// Uses a filename and file extension
  /// @return LSDRaster
  /// @param filename A String, the file to be loaded.
  /// @param extension A String, the file extension to be loaded.
  LSDRaster(string filename, string extension)	{ create(filename, extension); }

  /// @brief Create an LSDRaster from memory.
  /// @return LSDRaster
  /// @param nrows An integer of the number of rows.
  /// @param ncols An integer of the number of columns.
  /// @param xmin A float of the minimum X coordinate.
  /// @param ymin A float of the minimum Y coordinate.
  /// @param cellsize A float of the cellsize.
  /// @param ndv An integer of the no data value.
  /// @param data An Array2D of floats in the shape nrows*ncols,
  ///containing the data to be written.
  LSDRaster(int nrows, int ncols, float xmin, float ymin,
            float cellsize, float ndv, Array2D<float> data)
      { create(nrows, ncols, xmin, ymin, cellsize, ndv, data); }

  /// @brief Create an LSDRaster from memory, includes georeferencing
  /// @return LSDRaster
  /// @param nrows An integer of the number of rows.
  /// @param ncols An integer of the number of columns.
  /// @param xmin A float of the minimum X coordinate.
  /// @param ymin A float of the minimum Y coordinate.
  /// @param cellsize A float of the cellsize.
  /// @param ndv An integer of the no data value.
  /// @param data An Array2D of floats in the shape nrows*ncols,
  /// @param temp_GRS a map of strings containing georeferencing information. Used
  /// mainly with ENVI format files
  ///containing the data to be written.
  LSDRaster(int nrows, int ncols, float xmin, float ymin,
      float cellsize, float ndv, Array2D<float> data, map<string,string> temp_GRS)
  { create(nrows, ncols, xmin, ymin, cellsize, ndv, data, temp_GRS); } 


  // Get functions

  /// @return Number of rows as an integer.
  int get_NRows() const				{ return NRows; }
  /// @return Number of columns as an integer.
  int get_NCols() const				{ return NCols; }
  /// @return Minimum X coordinate as an integer.
  float get_XMinimum() const			{ return XMinimum; }
  /// @return Minimum Y coordinate as an integer.
  float get_YMinimum() const			{ return YMinimum; }
  /// @return Data resolution as an integer.
  float get_DataResolution() const	{ return DataResolution; }
  /// @return No Data Value as an integer.
  int get_NoDataValue() const			{ return NoDataValue; }
  /// @return Raster values as a 2D Array.
  Array2D<float> get_RasterData() const { return RasterData.copy(); }
  
  /// @brief Get the raw raster data, double format
  /// @author DAV  
  Array2D<double> get_RasterData_dbl() const { return RasterData_dbl.copy(); }
  
  /// @brief Get the raw raster data, integer format
  /// @author DAV  
  Array2D<int> get_RasterData_int() const { return RasterData_int.copy(); }
  
  /// @return map containing the georeferencing strings
  map<string,string> get_GeoReferencingStrings() const { return GeoReferencingStrings; }

  /// @brief Get the raster data at a specified location.
  /// @param row An integer, the X coordinate of the target cell.
  /// @param column An integer, the Y coordinate of the target cell.
  /// @return The raster value at the position (row, column).
  /// @author SMM
  /// @date 01/01/12
  float get_data_element(int row, int column)  { return RasterData[row][column]; }

  /// Assignment operator.
  LSDRaster& operator=(const LSDRaster& LSDR);

  /// @brief Read a raster into memory from a file.
  ///
  /// The supported formats are .asc and .flt which are
  /// both exported and imported by arcmap.
  ///
  /// The filename is the string of characters before the '.' in the extension
  /// and the extension is the characters after the '.'.
  ///
  /// If the full filename is my_dem.01.asc then:
  /// filename = "my_dem.01" and extension = "asc".
  ///
  ///
  /// For float files both a data file and a header are read
  /// the header file must have the same filename, before extention, of
  /// the raster data, and the extension must be .hdr.
  ///
  /// @author SMM
  /// @date 01/01/12
  void read_raster(string filename, string extension);

  /// @brief Reads a raster from an ascii file for use in LSDCatchmentModel
  /// @bug You can't return a TNT::Array properly in a function. See google for details.
  /// @author DAV
  /// @return Returns a 2D Array of the raster file. 
  TNT::Array2D<double> get_ascii_raster(string FILENAME);
  
  /// @brief reads a raster from an ascii file, populates an LSDRaster object array
  /// @author DAV
  /// @details This method takes a filename (the .asc file).
  /// It reads in the ascii file and fills in the array (and header) part of the LSDRaster object
  /// with double precision values in the array. You must have declared an instance of an LSDRaster object.
  /// @example myLSDRasterobject.read_ascii_raster("elevations.asc")
  /// @result The myLSDRasterobject.RasterData_dbl data member is now populated with an array of
  /// the DEM data. The data members for NCols, NRows, etc. are also updated.
  void read_ascii_raster(string FILENAME);

  /// @brief Read a raster from memory to a file.
  ///
  /// The supported formats are .asc and .flt which are
  /// both exported and imported by arcmap.
  ///
  /// The filename is the string of characters before the '.' in the extension
  /// and the extension is the characters after the '.'.
  ///
  /// If the full filename is my_dem.01.asc then:
  /// filename = "my_dem.01" and extension = "asc".
  ///
  /// For float files both a data file and a header are written
  /// the header file must have the same filename, before extention, of
  /// the raster data, and the extension must be .hdr.
  ///
  /// @param filename a string of the filename _without_ the extension.
  /// @param extension a string of the extension _without_ the leading dot
  /// @author SMM
  /// @date 01/01/12
  void write_raster(string filename, string extension);

  /// @brief Checks to see if two rasters have the same dimensions
  /// @detail Does NOT check georeferencing
  /// @param Compare_raster: the raster to compare 
  /// @author SMM
  /// @date 04/05/2015
  bool does_raster_have_same_dimensions(LSDRaster& Compare_raster);

  /// @brief Checks to see if two rasters have the same dimensions
  /// @detail Does NOT check georeferencing
  /// @param Compare_raster: the raster to compare 
  /// @author SMM
  /// @date 04/05/2015
  bool does_raster_have_same_dimensions(LSDIndexRaster& Compare_raster);


  /// @brief Checks to see if two rasters have the same georeferencing
  /// @param Compare_raster: the raster to compare 
  /// @author SMM
  /// @date 02/03/2015
  bool does_raster_have_same_dimensions_and_georeferencing(LSDRaster& Compare_raster);

  /// @brief Checks to see if two rasters have the same georeferencing
  /// @param Compare_raster: the raster to compare 
  /// @author SMM
  /// @date 02/03/2015
  bool does_raster_have_same_dimensions_and_georeferencing(LSDIndexRaster& Compare_raster);


  /// @brief Method which takes a new xmin and ymax value and modifys the GeoReferencingStrings
  /// map_info line to contain these new values.
  ///
  /// @details Intended for use in the rastertrimmer methods and is called from within these methods. 
  /// Modifying georeferencing information by hand is messy and should be avoided if
  /// at all possible.
  /// @param NewXmin floating point value of the new minimum x value in the raster.
  /// @param NewYmax floating point value of the new maximum y value in the raster.
  /// @return An updated GeoReferencingStrings object.
  ///
  /// @author SWDG
  /// @date 6/11/14
  map<string, string> Update_GeoReferencingStrings(float NewXmin, float NewYmax);

  /// @brief Method which updates the map info element of the georeferencing strings based on
  /// information within the datamembers of the raster
  ///
  /// @details Intended for use when changing raster dimesions
  ///
  /// @author SMM
  /// @date 6/11/14
  void Update_GeoReferencingStrings();

  /// @brief This method imposes georefereing strings assuming the coordinate
  /// system is UTM
  /// @param zone the UTM zone
  /// @param NorS a string containing characters that start either N (for north)
  /// or S for south. The letter is not case sensitive
  /// @author SMM
  /// @date 6/11/14
  void impose_georeferencing_UTM(int zone, string NorS);

  /// @brief This method looks up the central meridian given a UTM zone
  /// @param UTM_zone the UTM zone
  /// @return central_meridian an integer of the central meridian of this UTM zone
  /// @author SMM
  /// @date 6/11/14
  int Find_UTM_central_meridian(int UTM_zone);
  
  /// @brief this function gets the UTM_zone and a boolean that is true if
  /// the map is in the northern hemisphere
  /// @param UTM_zone the UTM zone. Replaced in function. 
  /// @param is_North a boolean that is true if the DEM is in the northern hemisphere.
  ///  replaced in function
  /// @author SMM
  /// @date 22/12/2014
  void get_UTM_information(int& UTM_zone, bool& is_North);

  /// @brief this gets the x and y location of a node at row and column
  /// @param row the row of the node
  /// @param col the column of the node
  /// @param x_loc the x location (Northing) of the node
  /// @param y_loc the y location (Easting) of the node
  /// @author SMM
  /// @date 22/12/2014
  void get_x_and_y_locations(int row, int col, double& x_loc, double& y_loc);

  /// @brief this gets the x and y location of a node at row and column
  /// @param row the row of the node
  /// @param col the column of the node
  /// @param x_loc the x location (Northing) of the node
  /// @param y_loc the y location (Easting) of the node
  /// @author SMM
  /// @date 22/12/2014
  void get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc);

  /// @brief a function to get the lat and long of a node in the raster
  /// @detail Assumes WGS84 ellipsiod
  /// @param row the row of the node
  /// @param col the col of the node
  /// @param lat the latitude of the node (in decimal degrees, replaced by function)
  ///  Note: this is a double, because a float does not have sufficient precision
  ///  relative to a UTM location (which is in metres)
  /// @param long the longitude of the node (in decimal degrees, replaced by function)
  ///  Note: this is a double, because a float does not have sufficient precision
  ///  relative to a UTM location (which is in metres)
  /// @param Converter a converter object (from LSDShapeTools)
  /// @author SMM
  /// @date 22/12/2014
  void get_lat_and_long_locations(int row, int col, double& lat, 
                  double& longitude, LSDCoordinateConverterLLandUTM Converter);

  /// @brief this check to see if a point is within the raster
  /// @param X_coordinate the x location of the point
  /// @param Y_coordinate the y location of the point
  /// @return is_in_raster a boolean telling if the point is in the raster
  /// @author SMM
  /// @date 13/11/2014
  bool check_if_point_is_in_raster(float X_coordinate,float Y_coordinate);
  
  /// @brief This function returns a vector with the X adn Y minimum and max
  ///   values
  /// @return XYMinMax a vector with four elements
  ///   XYMinMax[0] = XMinimum
  ///   XYMinMax[1] = YMinimum
  ///   XYMinMax[2] = XMaximum
  ///   XYMinMax[3] = XMaximum
  /// @author SMM
  /// @date 3/7/2015
  vector<float> get_XY_MinMax();
  
  ///@brief This function returns the raster data as a vector 
  ///@return vector<float> with raster data 
  ///@author FJC 
  ///@date 06/11/15 
  vector<float> get_RasterData_vector(); 
  
  /// @brief rewrite all the data array values with random numbers (with a 
  /// uniform distribution). 
  /// @param range is the range of values.
  /// @author SMM
  /// @date 18/02/14
  void rewrite_with_random_values(float range);

  /// @brief Calculate the minimum bounding rectangle for an LSDRaster Object and crop out
  /// all the surrounding NoDataValues to reduce the size and load times of output rasters.
  ///
  /// @details Ideal for use with chi analysis tools which output basin and chi m value rasters
  /// which can be predominantly no data. As an example, a 253 Mb file can be reduced to
  /// ~5 Mb with no loss or resampling of data.\n
  ///
  /// Modded 6/11/14 to cope with bil files and to catch cases where some or all of the
  /// edges cannot be trimmed - SWDG
  ///
  /// @return A trimmed LSDRaster object.
  /// @author SWDG
  /// @date 22/08/13
  LSDRaster RasterTrimmer();

  /// @brief Calculate the minimum bounding rectangle for an LSDRaster Object and crop out
  /// all the surrounding NoDataValues to reduce the size and load times of output rasters.
  ///  Similar to RasterTrimmer but has a pixel buffer. Useful for CRN data since
  ///   sometimes the channel in the DEM does not correspond exactly with the 
  ///   data point. 
  /// @details Ideal for use with chi analysis tools which output basin and chi m value rasters
  /// which can be predominantly no data. As an example, a 253 Mb file can be reduced to
  /// ~5 Mb with no loss or resampling of data.\n
  /// @param padded_pixels the number of pixels to pad the DEM with
  /// @return A trimmed LSDRaster object.
  /// @author SMM
  /// @date 18/03/15
  LSDRaster RasterTrimmerPadded(int padded_pixels);

  /// @brief Takes a raster and trims nodata from around the edges to 
  /// result in a rectangular LSDRaster
  /// @return A trimmed LSDRaster object.
  /// @author SMM
  /// @date 5/11/14
  LSDRaster RasterTrimmerSpiral();
  
  /// @brief This returns a clipped raster that has the same dimensions as the
  ///  smaller raster
  /// @param smaller_raster the raster to which the bigger raster should be
  ///  clipped
  /// @author SMM
  /// @date 20/03/2015
  LSDRaster clip_to_smaller_raster(LSDRaster& smaller_raster);

  /// @brief This returns a clipped raster that has the same dimensions as the
  ///  smaller raster
  /// @param smaller_raster the raster to which the bigger raster should be
  ///  clipped
  /// @author SMM
  /// @date 20/03/2015
  LSDRaster clip_to_smaller_raster(LSDIndexRaster& smaller_raster);

  /// @brief Make LSDRaster object using a 'template' raster and an Array2D of data.
  /// @param InputData 2DArray of floats to be written to LSDRaster.
  /// @return LSDRaster containing the data passed in.
  /// @author SWDG
  /// @date 29/8/13
  LSDRaster LSDRasterTemplate(Array2D<float> InputData);

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //
  // Simple topographic metrics
  // Several simple topographic metrics measuered over a kernal
  //
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  /// @brief Calculates a relief raster
  /// @param The width in metres of the kernal you wnat to use for resolution
  /// @param if this ==1 then you use a circular kernal. Otherwise kernal is square
  /// @return The spatially distributed relief
  /// @author JAJ (entered into trunk SMM)
  /// @date 6/06/2014 Happy 3rd Birthday Skye!!!
  LSDRaster calculate_relief(float kernelWidth, int kernelType);

  /// @brief Calculates mean elevation of a raster
  /// @return The spatially distributed relief
  /// @author JAJ (entered into trunk SMM)
  /// @date 01/02/2014 modified 09/06/2014 by SMM
  float mean_elevation(void);
  
  /// @brief Calculates max elevation of a raster
  /// @return The spatially distributed relief
  /// @author JAJ (entered into trunk SMM)
  /// @date 01/02/2014 
  float max_elevation(void);

  /// @brief Calculates mean relief of a raster, it defaults to a circular kernal
  /// @return The spatially distributed relief
  /// @author JAJ (entered into trunk SMM)
  /// @date 01/02/2014 	modified 09/06/2014 by SMM
  float mean_relief(float kernelsize);

  /// @brief Calculates the mean difference between two rasters
  /// @details checks raster dimensions  but not georeferencing since it
  ///  is used to compare asc format model results
  /// @author SMM
  /// @date 04/05/2015
  float difference_rasters(LSDRaster& compare_raster);

  /// @brief This multiplies the raster data by a multiplier
  /// @detail Note that values are replaced
  /// @author SMM
  /// @date 06/05/2015
  void raster_multiplier(float multiplier);

  // Functions for the Diamond Square algorithm

  /// @brief This returns a value from the array data element but wraps around
  /// the array dimensions so that row > NRows (for example) returns a value.
  /// @param The row of the data point you want.
  /// @parame Column of desired data point.
  /// @return The value of the data array at the desired row and column.
  /// @author SMM
  /// @date 16/02/2014
  float WrapSample(int row, int col);

  /// @brief This sets a value in the data array withthe added feature that it 
  /// wraps beyond NRows and NCols.
  /// @param The row of data to be reset.
  /// @param The column of the data to be reset.
  /// @param The value of the data to be reset.
  /// @author SMM
  /// @date 16/02/2014
  void SetWrapSample(int row, int col, float value);

  /// @brief This sets the corners of features as the first step in the diamond
  /// square algorithm. 
  /// @param The first parameter is the feature size. This needs to be a power of 2, but
  /// this is set by the parent DiamondSquare function (that is, this function should not
  /// be called independantly. 
  /// @param The scale is effectivly the maximum relief of the surface to be produced by the 
  /// algorithm.
  /// @author SMM
  /// @date 16/02/2014
  void DSSetFeatureCorners(int featuresize, float scale);

  /// @brief This is the square sampling step of the diamond square algorithm: it takes 
  /// the average of the four corners and adds a random number to set the centrepoint
  /// of a square. 
  /// @param The row of the centrepoint.
  /// @param The column of the centrepoint.
  /// @param The size of this square (in pixels, must be divisible by 2).
  /// @param The random value added to the average of the four corners.
  /// @author SMM
  /// @date 16/02/2014 
  void DSSampleSquare(int row,int col, int size, float value);
  
  /// @brief This is the diamond sampling step of the diamond square algorithm: it takes 
  /// the average of the four corners and adds a random number to set the centrepoint
  /// of a diamond. 
  /// @param The row of the centrepoint.
  /// @param The column of the centrepoint.
  /// @param The size of this diamond (in pixels, must be divisible by 2).
  /// @param The random value added to the average of teh four corners.
  /// @author SMM
  /// @date 16/02/2014 
  void DSSampleDiamond(int row, int col, int size, float value);

  /// @brief This is the sampling function for the diamond square algorithm: it
  /// runs both a diamond and a square sampling for each step.
  ///
  /// @param The stepsize, which is the size of the diamonds and the squares. 
  /// @param The scale which sets the maxmum relief within a particular square or
  /// diamond and is scaled by the stepsize (that is smaller squares have smaller scales).
  ///
  /// @author SMM
  /// @date 16/02/2014 
  void DiamondSquare_SampleStep(int stepsize, float scale);
 
  /// @brief This is the driving function for the diamond square algorithm.
  /// @details The driving function takes the current raster and then pads it
  /// in each direction to have rows and columns that are the nearest powers
  /// of 2. The xllocation and yllocation data values are preserved. The function
  /// returns a pseudo fractal landscape generated with the diamond square algorithm
  /// 
  /// @param feature order is an interger n where the feature size consists of 2^n nodes.
  /// If the feature order is set bigger than the dimensions of the parent raster then
  /// this will default to the order of the parent raster.
  /// @param Scale is a floating point number that sets the maximum relief of the resultant raster.
  /// @return Returns a diamond square pseudo-fractal surface in and LSDRaster object.
  /// @author SMM
  /// @date 16/02/2014 
  LSDRaster DiamondSquare(int feature_order, float scale);

  // Functions relating to shading, shadowing and shielding

  /// @brief This function generates a hillshade raster.
  ///
  /// It uses the the algorithm outlined in Burrough and McDonnell Principles
  /// of GIS (1990) and in the ArcMap web help
  /// http://edndoc.esri.com/arcobjects/9.2/net/shared/geoprocessing/
  /// spatial_analyst_tools/how_hillshade_works.htm
  ///
  /// Default values are altitude = 45, azimuth = 315, z_factor = 1
  /// @param altitude (float) of the illumination source in degrees.
  /// @param azimuth (float) of the illumination source in degrees
  /// @param z_factor (float) Scaling factor between vertical and horizontal.
  /// @return Hillshaded LSDRaster object
  /// @author SWDG
  /// @date February 2013
  LSDRaster hillshade();
  LSDRaster hillshade(float altitude, float azimuth, float z_factor);

  /// @brief This function generates a hillshade derivative raster using the
  /// algorithm outlined in Codilean (2006).
  ///
  /// @details It identifies areas in shadow as 1 and all other values as 0. Is
  /// interfaced through LSDRaster::TopoShield and should not be called directly,
  /// to generate a hillshade use LSDRaster::hillshade instead.
  /// @param theta The zenith angle of the illumination source in degrees.
  /// @param phi The azimuth angle of the illumination source in degrees.
  /// @return 2D Array of floats.
  /// @author SWDG
  /// @date 11/4/13
  Array2D<float> Shadow(int theta, int phi);

	/// @brief Function to determine areas of a DEM that are in shadow from a 
	/// given radiation source defined by an Azimuth and Zenith following Codilean (2006). 
	///
	/// @details Performs a coordinate transformation, rotating the x,y,z coordinates about the
	/// Azimuth and Zenith such that the coordinates are aligned with the Azimuth and Zenith. 
	/// Shaded cells are then found by tracking in the direction of the radiation source and 
	/// looking for transformed z values greater than that at the cell of interest which would
	/// therefore cast a shadow.
  ///
  /// @param Azimuth of the illumination source in degrees.
  /// @param ZenithAngle of the illumination source in degrees
  /// @return Hillshaded LSDIndexRaster 
  /// @author MDH
  /// @date Feb 2015
	LSDRaster CastShadows(int Azimuth, int ZenithAngle);
	
	/// @brief Function to determine areas of a DEM that are in shadow from a 
	/// given radiation source defined by an Azimuth and Zenith following Codilean (2006). 
	///
	/// @details Performs a coordinate transformation, rotating the x,y,z coordinates about the
	/// Azimuth and Zenith such that the coordinates are aligned with the Azimuth and Zenith. 
	/// Shaded cells are then found by tracking in the direction of the radiation source and 
	/// looking for transformed z values greater than that at the cell of interest which would
	/// therefore cast a shadow.
  ///
  /// @param Azimuth of the illumination source in degrees.
  /// @param ZenithAngle of the illumination source in degrees
  /// @return Hillshaded 2D Array of ints
  /// @author MDH
  /// @date Feb 2015
	Array2D<float> Shadows(int Azimuth, int ZenithAngle);
	
  /// @brief This function generates a topographic shielding raster using the algorithm 
  /// outlined in Codilean (2006).
  ///
  /// @details Creating a raster of values between 0 and 1 of shadowed cells which can 
  /// be used as a scaling factor in Cosmo analysis.
  ///
  /// Goes further than the original algorithm allowing a theoretical theta,
  /// phi pair of 1,1 to be supplied and although this will increase the
  /// computation time significantly, it is much faster than the original
  /// Avenue and VBScript implementations (This is probably no longer true
  /// now that we incorporate drop shadows (MDH, Feb 2015)).
  ///
  /// Takes 2 ints, representing the theta, phi paring required.
  /// Codilean (2006) used 5,5 as the standard values, but in reality values of
  /// 10,15 are often preferred to save processing time.
  /// @param theta_step Spacing of sampled theta values.
  /// @param phi_step Spacing of sampled phi values.
  /// @pre phi_step must be a factor of 360.
  /// @author SWDG
  /// @date 11/4/13
  LSDRaster TopographicShielding(int theta_step, int phi_step);
  LSDRaster TopographicShielding();
  
  /// @brief Surface polynomial fitting and extraction of topographic metrics
  /// 
  /// @detail A six term polynomial surface is fitted to all the points that lie
  /// within circular neighbourhood that is defined by the designated window
  /// radius.  The user also inputs a binary raster, which tells the program
  /// which rasters it wants to create (label as "1" to produce them, "0" to
  /// ignore them. This has 8 elements, as listed below:
  ///        0 -> Elevation (smoothed by surface fitting)
  ///        1 -> Slope
  ///        2 -> Aspect
  ///        3 -> Curvature
  ///        4 -> Planform Curvature
  ///        5 -> Profile Curvature
  ///        6 -> Tangential Curvature
  ///        7 -> Stationary point classification (1=peak, 2=depression, 3=saddle)
  /// The program returns a vector of LSDRasters.  For options marked "false" in
  /// boolean input raster, the returned LSDRaster houses a blank raster, as this
  /// metric has not been calculated.  The desired LSDRaster can be retrieved from
  /// the output vector by using the cell reference shown in the list above i.e. it
  /// is the same as the reference in the input boolean vector.
  /// @param window_radius -> the radius of the circular window over which to
  /// fit the surface
  /// @param raster_selection -> a binary raster, with 8 elements, which
  /// identifies which metrics you want to calculate.
  /// @return A vector of LSDRaster objects.  Those that you have not asked to
  /// be calculated are returned as a 1x1 Raster housing a NoDataValue
  ///
  /// @author DTM
  /// @date 28/03/2014
  vector<LSDRaster> calculate_polyfit_surface_metrics(float window_radius, vector<int> raster_selection);
    
  /// @brief Surface polynomial fitting and extraction of roughness metrics
  /// 
  /// @detail 
  /// A six term polynomial surface is fitted to all the points that lie within
  /// circular neighbourhood that is defined by the designated window radius. 
  /// This surface is used to determine the orientation of the surface normal
  /// vector at each cell.  The algorithm then searches through the grid again,
  /// using a second search window to look for the local variability in normal
  /// vector orientation. The user also inputs a binary raster, which tells the
  /// program which rasters it wants to create (label as "1" to produce them,
  /// "0" to ignore them. This has 3 elements, as listed below:
  ///        0 -> s1 -> describes clustering of normals around the major axis
  ///        1 -> s2 -> describes clustering of normals around semi major axis
  ///        2 -> s3 -> describes clustering around minor axis
  /// The program returns a vector of LSDRasters.  For options marked "0" in
  /// binary input raster, the returned LSDRaster houses a blank raster, as this
  /// metric has not been calculated.  The desired LSDRaster can be retrieved from
  /// the output vector by using the same cell reference shown in the list above
  /// i.e. it is the same as the reference in the input binary vector.
  /// @param window_radius1 -> the radius of the circular window over which to
  /// fit the surface
  /// @param window_radius2 -> the radius of the circular window over which to
  /// look for local variability of surface normal orientation
  /// @param raster_selection -> a binary raster, with 3 elements, which
  /// identifies which metrics you want to calculate.
  /// @return A vector of LSDRaster objects.  Those that you have not asked to
  /// be calculated are returned as a 1x1 Raster housing a NoDataValue
  ///
  /// @author DTM
  /// @date 01/04/2014
  vector<LSDRaster> calculate_polyfit_roughness_metrics(float window_radius1,
                          float window_radius2, vector<int> raster_selection);

  // this calculates coefficeint matrices for calculating a variety of
  // surface metrics such as slope, aspect, curvature, etc.

  /// @brief This function calculates 6 coefficient matrices that allow the user to
  /// then calcualte slope, curvature, aspect, a classification for finding saddles and peaks
  /// and other metrics.
  ///
  /// @details The coefficient matrices are overwritten during the running of this member function.
  ///
  /// Have N simultaneous linear equations, and N unknowns.	
  /// => b = Ax, where x is a 1xN array containing the coefficients we need for
  /// surface fitting.
  /// A is constructed using different combinations of x and y, thus we only need
  /// to compute this once, since the window size does not change.
  /// For 2nd order surface fitting, there are 6 coefficients, therefore A is a
  /// 6x6 matrix.
  /// Updated 15/07/2013 to use a circular mask for surface fitting - DTM.
  /// Updated 24/07/2013 to check window_radius size and correct values below data resolution - SWDG.
  /// @param window_radius Radius of the mask in <b>spatial units</b>.
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @param c coefficeint c.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @param f coefficeint f.
  /// @author DTM, SMM
  /// @date 01/01/12
  void calculate_polyfit_coefficient_matrices(float window_radius,
                Array2D<float>& a, Array2D<float>& b,
                Array2D<float>& c, Array2D<float>& d,
                Array2D<float>& e, Array2D<float>& f);

  // a series of functions for retrieving derived data from the polyfit calculations

  /// @brief  This function calculates the elevation based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param f coefficeint f.
  /// @return LSDRaster of elevations.
  /// @author FC
  /// @date 24/03/13
  LSDRaster calculate_polyfit_elevation(Array2D<float>& f);
  
  /// @brief  This function calculates the slope based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of slope.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_slope(Array2D<float>& d, Array2D<float>& e);
  
  /// @brief  This function calculates the aspect based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of aspect.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_aspect(Array2D<float>& d,Array2D<float>& e);

  /// @brief  This function calculates the curvature based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @return LSDRaster of curvature.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_curvature(Array2D<float>& a,Array2D<float>& b);

  /// @brief  This function calculates the planform curvature based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @param c coefficeint c.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of planform curvature.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_planform_curvature(Array2D<float>& a, Array2D<float>& b, Array2D<float>& c,
                      	Array2D<float>& d, Array2D<float>& e);
  
  /// @brief  This function calculates the profile curvature based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @param c coefficeint c.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of profile curvature.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_profile_curvature(Array2D<float>& a, Array2D<float>& b, Array2D<float>& c,
                        Array2D<float>& d, Array2D<float>& e);

  /// @brief  This function calculates the tangential curvature based on a polynomial fit.
  ///
  /// @details the window is determined by the calculate_polyfit_coefficient_matrices
  /// this function also calculates the a,b,c,d,e and f coefficient matrices.
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @param c coefficeint c.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of tangential curvature.
  /// @author DTM, SMM
  /// @date 01/01/12
  LSDRaster calculate_polyfit_tangential_curvature(Array2D<float>& a, Array2D<float>& b, Array2D<float>& c,
                             Array2D<float>& d, Array2D<float>& e);
  
  /// @brief This function identifies approximate position of stationary points within
  /// discrete surface using a threshold slope.
  ///
  /// @details The nature of the stationary point is then determined to discriminate
  /// peaks, depressions and saddles. \n
  /// 0 = Non-stationary  \n
  /// 1 = Peak             \n
  /// 2 = Depression        \n
  /// 3 = Saddle             \n
  /// @param a coefficeint a.
  /// @param b coefficeint b.
  /// @param c coefficeint c.
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @return LSDRaster of classified elevation data.
  /// @author DTM
  /// @date	17/09/2012
  LSDIndexRaster calculate_polyfit_classification(Array2D<float>& a, Array2D<float>& b, Array2D<float>& c,
                                                Array2D<float>& d, Array2D<float>& e);

  /// @brief Gets the hilltop curvature raster.
  /// @details Modified to take an LSDRaster of hilltops - SWDG 29/8/13
  /// @param curvature LSDRaster of curvatures.
  /// @param Hilltops LSDRaster of hilltops.
  /// @return LSDRaster of hilltop curvatures.
  /// @author DTM
  /// @date 30/04/13
  LSDRaster get_hilltop_curvature(LSDRaster& curvature, LSDRaster& Hilltops);
  
  /// @brief Removes positive hilltop curvature values
  /// @details Modifies the hilltop curvature raster to remove pixels with
  /// positive curvature caused by noise
  /// @param hilltop_curvature hilltop curvature input raster
  /// @return LSDRaster of hilltop curvature with positive values removed
  /// @author FJC
  /// @date 24/03/14 
  LSDRaster remove_positive_hilltop_curvature(LSDRaster& hilltop_curvature);

  /// @brief Gets the percentage of bedrock ridges 
  /// @details Uses the hilltop curvature raster and the roughness raster. If the 
  /// roughness value is greater than the threshold (set to 0.015) then the pixel 
  /// is designated bedrock. 
  /// @param roughness roughness input raster 
  /// @param hilltop_curvature hilltop curvature input raster 
  /// @param threshold threshold value to be designated bedrock 
  /// @return float with percentage bedrock pixels 
  /// @author FJC 
  /// @date 01/04/15   
  float get_percentage_bedrock_ridgetops(LSDRaster&roughness, LSDRaster& hilltop_curvature, float threshold); 

  /// @brief Calculates slope angle in radians. Needs the slope raster
  /// @return LSDRaster containing the slope angle in radians. For use in trigonometric
  /// calculations
  /// @author SMM
  /// @date 13/11/14 
  LSDRaster calculate_slope_angles();

  /// @brief Calculates the water supply rate required for saturation of the hillslope
  /// soil
  /// @details Water supply rate is the precipitation rate minus recharge and ET
  /// @param soil_thick soil thickness in m
  /// @param K hydraulic conductivity in m/ (some time unit)
  /// @param ConstributingArea the contributing area raster, should be in m^2
  /// @param SlopeAngle the angle of the slope (you need to use the function calculate_slope_angles)
  ///  to get this
  /// @return Water_supply_rate_for_saturation a raster holding the water supply 
  /// rate required for saturation. The units will be the same as hydraulic 
  /// conductivity but the lenght unit must be metres (to match contributing area)
  /// @author SMM
  /// @date 13/11/14 
  LSDRaster calculate_water_supply_rate_for_saturation(float soil_thick,
                  float K, LSDRaster& ContributingArea, LSDRaster& SlopeAngle);

  /// @brief This calculates the factor of safety if the soil is completely 
  /// saturated
  /// @details Factor of safety <1 == unstable
  /// @param C_r root cohesion in N/m^2
  /// @param rho_s soil density in kg/m^3
  /// @param soil_thick soil thickness in m
  /// @param tan_phi the friction angle of the soil
  /// @param SlopeAngle the angle of the slope (you need to use the function calculate_slope_angles)
  ///  to get this
  /// @return Factor_of_safety The factor of safety when the soil is saturated
  /// rate required for saturation. The units will be the same as hydraulic 
  /// conductivity but the lenght unit must be metres (to match contributing area)
  /// @author SMM
  /// @date 13/11/14 
  LSDRaster calculate_factor_of_safety_at_saturation(float C_r, float rho_s,
                         float soil_thick, float tan_phi, LSDRaster& SlopeAngle);

  // surface roughness
  /// @brief Algorithm that assesses surface roughness based on a polynomial fit.
  ///
  /// @details Runs a moving window across the DEM and assesses the variability of
  /// surface normals within that window.  Specifically the components of the
  /// normals are combined into an orientation matrix, which is then solved to
  /// find the eigenvalues s1, s2, s3 (Woodcock, 1977).
  /// @param d coefficeint d.
  /// @param e coefficeint e.
  /// @param l coefficeint l.
  /// @param m coefficeint m.
  /// @param n coefficeint n.
  /// @author DTM
  /// @date 13/09/2012
  void calculate_polyfit_directional_cosines(Array2D<float>& d, Array2D<float>& e, Array2D<float>& l,
                                             Array2D<float>& m, Array2D<float>& n);
  
  /// @brief Find eigenvalues for orientation matrix
  /// @param window_radius
  /// @param l coefficeint l.
  /// @param m coefficeint m.
  /// @param n coefficeint n.
  /// @param s1 coefficeint s1.
  /// @param s2 coefficeint s2.
  /// @param s3 coefficeint s3.
  /// @author DTM
  /// @date 13/09/2012
  void calculate_orientation_matrix_eigenvalues(float window_radius,
                       Array2D<float>& l, Array2D<float>& m,
                       Array2D<float>& n, Array2D<float>& s1,
                       Array2D<float>& s2, Array2D<float>& s3);

  // Rock exposure index  / roughness
  /// @brief This function is a wrapper to get the three roughness eigenvalues
  /// s1, s2 and s3.
  /// @param window_radius
  /// @param a_plane
  /// @param b_plane
  /// @param c_plane
  /// @author DTM
  /// @date 15/7/2013
  void calculate_plane_coefficient_matrices(float window_radius, Array2D<float>& a_plane,
             Array2D<float>& b_plane, Array2D<float>& c_plane);
  /// @brief Create the REI raster
  ///
  /// @details
  /// @param a_plane
  /// @param b_plane
  /// @param CriticalSlope
  /// @return LSDIndexRaster of rock exposure.
  /// @author DTM
  LSDRaster calculate_REI(Array2D<float>& a_plane, Array2D<float>& b_plane, float CriticalSlope);
  
  /// @brief Create the REI raster (imporoved wrapper)
  /// Rock exposure index defined as areas with local slope exceeding some
  /// critical slope as defined by DiBiase et al. (2012)
  /// @details
  /// @param window radius
  /// @param CriticalSlope
  /// @return LSDIndexRaster of rock exposure.
  /// @author DTM
  LSDRaster calculate_REI(float window_radius, float CriticalSlope);

  /// @brief this function takes the polyfit functions and requires a window radius and a vector telling the
  /// function which rasters to print to file.
  ///
  /// @details The function is data efficient since one does not need to
  /// recalculate the polyfit coefficeint matrices. It also takes a string
  /// which is the prename of the data files the file codes in the vector are:\n
  /// 0 slope         \n
  /// 1 aspect        \n
  /// 2 curvature       \n
  /// 3 planform curvature\n
  /// 4 profile curvature   \n
  /// 6 tangential curvature  \n
  /// 7 classification          \n
  /// @param window_radius Radius of the mask.
  /// @param file_prefix Output filename string.
  /// @param file_list Vector of files to be created.
  /// @author SMM
  /// @date 19-12-2012
  void calculate_and_print_polyfit_rasters(float window_radius,
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
  /// @brief This function takes the combines the polyfit functions and the roughness function in one package.
  ///
  /// @details The function is data efficient since one does not need to
  /// recalculate the polyfit coefficeint matrices. Itakes the window radius of
  /// the polyfit and the window of the roughness calculation the file codes in
  /// the vector are:\n
  /// 0 slope         \n
  /// 1 aspect        \n
  /// 2 curvature       \n
  /// 3 planform curvature\n
  /// 4 profile curvature   \n
  /// 6 tangential curvature  \n
  /// 7 classification          \n
  /// 8 roughness s1   \n
  /// 9 roughness s2  \n
  /// 10 roughness s3 \n
  /// @param window_radius Radius of the mask.
  /// @param roughness_radius Radius of the roughness window.
  /// @param file_prefix Output filename string.
  /// @param file_list Vector of files to be created.
  /// @author SMM
  /// @date 19-12-2012
  void calculate_and_print_polyfit_and_roughness_rasters(float window_radius, float roughness_radius,
                    string file_prefix, vector<int> file_list);

  // this function combines the polyfit functions and the roughness function in one package that
  // is data efficient becasue it only requires one calculation of the polyfit matrices.
  // it takes the window radius of the polyfit and the window of the roughness calcualtion
  // the file codes in the vector are:
  // 0 roughness s1
  // 1 roughness s2
  // 2 roughness s3
  // DTM 15-07-2013
  /// @brief This function takes the combines the roughness functions in one package.
  ///
  /// @details The function is data efficient since one does not need to
  /// recalculate the polyfit coefficeint matrices. I takes the window radius of
  /// the polyfit and the window of the roughness calculation the file codes in
  /// the vector are:\n
  /// 0 roughness s1   \n
  /// 1 roughness s2  \n
  /// 2 roughness s3 \n
  /// @param window_radius Radius of the mask.
  /// @param roughness_radius Radius of the roughness window.
  /// @param file_prefix Output filename string.
  /// @param file_code Vector of files to be created.
  /// @author DTM
  /// @date 15-07-2013
  void calculate_roughness_rasters(float window_radius, float roughness_radius, string file_prefix, vector<int> file_code);


  // hydrology tools

  /// @brief This function changes any elevation <= 0 to NoDataValue
  /// @author SMM
  /// @date 03/10/2014
  void remove_seas();

  /// @brief This function changes any elevation <= threshold to NoDataValue
  /// @author SMM
  /// @date 29/10/2014
  void mask_to_nodata_below_threshold(float threshold);

  /// @brief This function masks a raster to nodata based on a mask value and
  /// a mask raster
  /// @param Mask_raster the LSDIndexRaster that contains the mask
  /// @param mask_value the value on the mask that triggers a nodata
  /// @return a new LSDRaster that has the masked portions as nodata
  /// @author SMM
  /// @date 4/11/2014
  LSDRaster mask_to_nodata_with_mask_raster(LSDIndexRaster& Mask_raster, int mask_value);

  ///@brief This function fills pits/sinks in a DEM by incrementing elevations for cells with
  ///no downslope neighbour. The process is repeated adnausium until no cells require
  ///incrementing.
  ///
  ///Inputs required are a DEM file in ascii raster format as created by ARCMap
  ///and a file name to create a filled DEM grid.
  ///
  ///This code was built ontop of code made available by Jon D. Pelletier as part
  ///of his book:
  ///
  ///Pelletier,J.D.,'Quantitative Modelling of Landscapes' Cambridge University Press
  ///
  ///---------------------------------------------------------------------------------
  ///
  /// v1.3 reduced fill increment to 1mm  to avoid 'overfilling'
  ///
  /// Martin Hurst, October 2011
  ///
  ///---------------------------------------------------------------------------------
  ///
  /// v1.2 modified to read *.flt files
  ///
  /// Martin Hurst, November 2010
  ///
  ///---------------------------------------------------------------------------------
  ///
  /// v1.1 function incorporated to allow the tool to fill adjacent pixels immediately
  /// after filling a given pixel, should speed things up.
  ///
  /// Martin Hurst, October 2010
  ///
  ///---------------------------------------------------------------------------------
  ///
  /// v1.0 is slow as it requires many iterations through the dem
  ///
  /// Martin Hurst, June 2010
  /// @return Filled LSDRaster.
  /// @author MDH
  /// @date 01/06/10
  LSDRaster fill();

  /// @brief This is a recursive algorithm that is called by the fill function.
  /// @param fill_data
  /// @param i
  /// @param j
  /// @author MDH
  /// @date 01/06/10
  void fill_iterator(Array2D<float>& fill_data, int i, int j);


  /// @brief This function fills pits/sinks in a DEM by checking for pits from
  ///lowest to highest elevation, starting at the DEM boundary (raster edge or
  /// adjacent to NDVs).
  ///
  /// @details Utilises a priority queue to progressively populate the stack and
  /// pop out the the lowest value before checking that the neighbouring cells
  /// that are yet to be visited must be higher in a hydrologically correct DEM.
  /// This method is substantially faster on datasets with pits consisting of
  /// multiple cells since each cell only needs to be visited once.
  ///
  /// Method taken from Wang and Liu (2006), Int. J. of GIS. 20(2), 193-213
  /// @param MinSlope The minimum slope between two Nodes once filled. If set
  /// to zero will create flats.
  /// @return Filled LSDRaster object.
  /// @author Martin Hurst
  /// @date 12/3/13
  LSDRaster fill(float& MinSlope);
    
  // multidirection flow routing
  /// @brief Generate a flow area raster using a multi direction algorithm.
  ///
  /// @details Computes the proportion of all downslope flows for each cell in
  /// the input DEM and routes the flow accordingly. Consequently the dem is
  /// sorted and indexed using LSDStatsTools. Can handle DEMs containing flats,
  /// but pits must be filled using the new LSDRaster fill.
  ///
  /// Currently only works with periodic boundaries. Function built around
  /// original c++ code by Martin Hurst.
  /// @param BoundaryConditions Vector as in LSDFlowInfo object.
  /// @return LSDRaster of flow area.
  /// @author SWDG
  /// @date 18/4/13 - 24/4/13
  LSDRaster MDFlow(vector<string> BoundaryConditions);
  
  /// @brief Generate a flow area raster using a multi direction algorithm.
  ///
  /// @details Computes the proportion of all downslope flows for each cell in the input
  /// DEM, and weights them using the equation from Freeman et al 1991 and routes the
  /// flow accordingly.
  ///
  /// Paper link: http://www.sciencedirect.com/science/article/pii/009830049190048I
  ///
  /// Cardinal Weighting = (elevation_drop/total_elevation_drop)^1.1  \n
  /// Diagonal Weighting = ((elevation_drop/total_elevation_drop)*(1/root(2)))^1.1
  ///
  /// Can <b>NOT</b> handle DEMs containing flats or pits -  must be filled using the new
  /// LSDRaster fill. Function built around original c++ code by Martin Hurst.
  /// @return LSDRaster of flow area.
  /// @author SWDG
  /// @date 18/4/13
  LSDRaster FreemanMDFlow();
  
  /// @brief Route flow from one source pixel using FreemanMDFlow.  Adapted from SWDG's
  /// code above.
  /// @param i_source -> the row index of the source pixel
  /// @param j_source -> the column index of the source pixel
  /// @author DTM
  /// @date 07/11/2013
  LSDRaster FreemanMDFlow_SingleSource(int i_source,int j_source);
  
  /// @brief Extracts a multi-pixel channel network, as opposed to a single thread
  /// channel, using method outlined in Pelletier, 2013, WRR; A robust, two-parameter
  /// method for the extraction of drainage networks from high-resolution digital 
  ///elevation models (DEMs): Evaluation using synthetic and real-world DEMs
  /// 
  /// @detail First runs Freeman MD flow routing algorithm from previously identified
  /// source.  Then removes all pixels from the channel network if the ratio of
  /// discharge to upstream channel heads is below a user-specified threshold
  /// @param vector<int> channel_heads_rows - a vector of source node row indices
  /// @param vector<int> channel_heads_cols - a vector of source node column indices
  /// @param float R_threshold - the threshold ratio of discharge:upstream channel
  /// heads; Pelletier (2013) reccomends a value of 0.1
  /// @author DTM
  /// @date 22/05/2014
  LSDRaster FMDChannelsFromChannelHeads(vector<int>& channel_heads_rows, vector<int>& channel_heads_cols, float R_threshold);
  
  /// @brief Generate a flow area raster using a multi direction algorithm.
  ///
  /// @details Computes the proportion of all downslope flows for each cell in the input
  /// DEM, and weights them using the equation from Quinn et al 1991 and routes the
  /// flow accordingly.
  ///
  /// Paper link: http://onlinelibrary.wiley.com/doi/10.1002/hyp.3360050106/abstract
  ///
  /// Cardinal Weighting = (elevation_drop/total_elevation_drop)*DataResolution/2 \n
  /// Diagonal Weighting = ((elevation_drop/total_elevation_drop)*(1/root(2)))* DataResolution*0.354
  ///
  /// Can <b>NOT</b> handle DEMs containing flats or pits -  must be filled using the new
  /// LSDRaster fill. Function built around original c++ code by Martin Hurst.
  /// @return LSDRaster of flow area.
  /// @author SWDG
  /// @date 18/4/13
  LSDRaster QuinnMDFlow();
  
  /// @brief Generate a flow area raster using a multi 2-direction algorithm.
  ///
  /// @details Computes the proportion of all downslope flows for each cell in the input
  /// DEM. Finds the cell of the steepest descent and then checks the two
  /// neighbouring cells slopes. If either is also downslope proportion flow
  /// between the steepest cell and the steepest neighbour. If neither neighbour
  /// is downslope 100% of flow follows the steepest path.
  ///
  /// Can <b>NOT</b> handle DEMs containing flats or pits -  must be filled using the new
  /// LSDRaster fill. Function built around original c++ code by Martin Hurst.
  /// @return LSDRaster of flow area.
  /// @author SWDG
  /// @date 02/08/13
  LSDRaster M2DFlow();

//  // channel head identification
//  /// @brief This function is used to predict channel head locations based on the method proposed by Pelletier (2013).
//  ///
//  /// @details It creates a contour curvature map and identifies channel heads as pixels greater
//  /// than a user defined contour curvature threshold value, set by default at 0.1.  The threshold curvature
//  /// can also be defined as a multiple of the standard deviation of the curvature.  Before this function is called
//  /// the DEM must be filtered using the wiener filter in the LSDRasterSpectral object in order to remove high frequency
//  /// noise.
//  ///
//  /// Reference: Pelletier (2013) A robust, two-parameter method for the extraction of drainage
//  /// networks from high-resolution digital elevation models (DEMs): Evaluation using synthetic and real-world
//  /// DEMs, Water Resources Research 49: 1-15
//  ///
//  /// @param window_radius Integer window radius - suggest 6m.
//  /// @param tan_curv_threshold Double curvature threshold value.
//  /// @param tan_curv_array 2D array of tangential curvature.
//  /// @return LSDIndexRaster of predicted channel head locations.
//  /// @author FC, DTM
//  /// @date 07/11/13
//		LSDIndexRaster calculate_pelletier_channel_heads(float window_radius, float tan_curv_threshold, Array2D<float>& tan_curv_array);

  // some tools associated with ridgeline analyis

  /// @brief Module to sample LSDRaster values running along a ridgetop network.
  ///
  /// @details Ridge network is generated from LSDJunctionNetwork::ExtractRidges.
  /// @param Ridges 2D Array of ridge lines.
  /// @return Sampled LSDRaster object.
  /// @author SWDG
  /// @date 04/2013

  LSDRaster RidgeSample(Array2D<float>& Ridges);
  /// @brief Pass a smoothing window over a ridge LSDRaster object to calculate
  /// an average value running along the ridgetop.
  /// @param WindowRadius optional integer smoothing radius between 1 and 6.
  /// @pre Default smoothing radius is 2 and will revert to that value if a
  /// radius outside the vaid range (1 to 6) is passed.
  /// @return Averaged LSDRaster object.
  /// @author SWDG
  /// @date 04/2013
  LSDRaster RidgeSmoother(int WindowRadius);
  
  /// @brief Pass a buffer over a ridge LSDRaster object to increase sampling area.
  ///
  /// @details Buffers equally in all directions, so use with care to avoid
  /// sampling areas away from the axis of the original ridge line.
  /// @param BufferRadius optional integer buffer radius between 1 and 6.
  /// @pre Default buffer radius is 2 and will revert to that value if a
  /// radius outside the vaid range (1 to 6) is passed.
  /// @return LSDRaster object containing buffered ridges.
  /// @author SWDG
  /// @date 04/2013
  LSDRaster RidgeBuffer(int BufferRadius);

  /// @brief Module assigns an average LSDRaster value to each basin.
  ///
  /// @details Works by searching for every hilltop value that falls within a
  /// basin, summing these values and writing the final average to every cell
  /// identified as the basin in question.
  ///
  /// Very inefficent at present. Module loops through every cell in LSDRaster
  /// (2 * number of basins) + 1 times. Beware!
  /// @param Basins LSDIndexRaster of Drainage basins, generated using
  /// ChannelNetwork::ExtractBasinsOrder.
  /// \n\n Bug fixed in assignment of basin IDs - SWDG 2/9/13.
  /// @return LSDRaster of average basin value for each identified basin.
  /// @author SWDG
  /// @date 04/2013
  LSDRaster BasinAverager(LSDIndexRaster& Basins);

  /// @brief Module calculates an average value to each for a given raster from within a basin.
  /// @param Basins LSDIndexRaster of a Drainage basin
  /// @return Float of the averaged value
  /// @author SWDG
  /// @date 24/3/2015
  float SingleBasinAverager(LSDIndexRaster& Basin);

  /// @brief Write the area(in spatial units) of each basin to the basin's pixels.
  ///
  /// @details Big optimisation following the Drainage density's design pattern.
  /// Works by flattening the raster into a 1D vector, sorting it and summing 
  /// each run of Basin IDs. Gives a fast count of the number of pixels per basin,
  /// which is multiplied by the cellsize to get an area in spatial units.
  /// @param Basins LSDIndexRaster of drainage basins to measure.
  /// @return LSDRaster of basin areas.
  /// @author SWDG
  /// @date 20/11/2013  
  LSDRaster BasinArea(LSDIndexRaster Basins);

  /// @brief Convert a basin, given by a basin ID, into a chain of xy coordinates for
  /// fast plotting of vector basin outlines.
  ///
  /// @details Produces a generalised polygon and will not cope well with complex geometries.
  /// \n\n
  /// Needs to be updated to write data into an esri ascii format so the files can 
  /// be loaded into arc. Currently writes to a text file called chain.txt.
  /// @param Basins IndexRaster of basins
  /// @param BasinOfInterest integer of the basin ID to be converted.
  /// @author SWDG
  /// @date 21/11/2013
  void GetBasinVector(LSDIndexRaster Basins, int BasinOfInterest);

  /// @brief Punch basins out of an LSDRaster to create DEMs of a single catchment.
  ///
  /// @details Writes files in the user supplied format (flt or asc) and returns a vector 
  /// of LSDRasters so they can be loaded into other functions.
  /// Updated 24/9/13 to return a vector of LSDRasters - SWDG.
  /// @param basin_ids Vector of basins to punch out.
  /// @param BasinArray Basin outlines used to punch out the LSDRasters.
  /// @return Vector of output filenames. 
  /// @author SWDG 
  /// @date 27/8/13
  vector<LSDRaster> BasinPuncher(vector<int> basin_ids, LSDIndexRaster BasinArray);


  /// @brief Cookie cut a raster using a smaller raster.
  ///
  /// @details Requires that both rasters share a spatial extent.
  /// @param Cutter an LSDRaster to be used as the template.
  /// @return LSDRaster of the data cut to the other ratser's shape. 
  /// @author SWDG
  /// @date 06/07/15
  LSDRaster CookieCutRaster(LSDRaster Cutter);

  /// @brief Collect all basin average metrics into a single file.
  ///
  /// @details File is written with the format: \n\n
  /// "basin_id slope elevation aspect area drainage_density hilltop_curvature 
  /// hillslope_length mean_slope hilltop_relief hilltop_aspect E* R* LH_bins LH_splines LH_density"
  /// @param Basins LSDIndexRaster of drainage basins to sample.
  /// @param Slope
  /// @param Elevation
  /// @param Aspect
  /// @param Area
  /// @param DrainageDensity
  /// @param Cht
  /// @param HillslopeLength
  /// @param MeanSlope
  /// @param Relief
  /// @param MeanAspect
  /// @param LH_drainage_density Hillslope length for each basin calculated 
  /// as in Tucker et al 2001 - Added 6/11/13 SWDG. 
  /// @param LH_Data Array of LH data generated from the Boomerang plotting 
  /// function and compiled using the BasinMetrics Driver - Added 1/11/13 SWDG.
  /// @param CriticalSlope Threshold value for E* and R* values.
  /// @param RasterFilename Filename of the input raster data.
  /// @author SWDG
  /// @date 27/8/13
  void CollectBasinMetrics(LSDIndexRaster& Basins, LSDRaster& Slope, LSDRaster& Elevation, LSDRaster& Aspect,
                              LSDRaster& Area, LSDRaster& DrainageDensity, LSDRaster& Cht, LSDRaster& HillslopeLength,
                              LSDRaster& MeanSlope, LSDRaster& Relief, LSDRaster& MeanAspect, LSDRaster& LH_drainage_density,
                              Array2D<float> LH_Data, float CriticalSlope, string RasterFilename);
  
  /// @brief Takes a raster and a corresponding index raster, and calculates the mean, sd
  /// and standard error by index.  Returns four vectors: mean, st.dev., st.err and 
  /// Number of points for each category.
  /// @param input index raster
  /// @param output vector with mean values
  /// @param output vector with standard deviation values
  /// @param output vector with standard error values
  /// @param output vector with number of points
  /// @author DTM
  /// @date 28/11/2013
  void raster_statistics_by_index(LSDIndexRaster& IndexRaster, vector<float>& mean_vector, 
          vector<float>& sd_vector, vector<float>& serr_vector, vector<int>& Npts_vector);
  

  /// @brief Generate data in two text files to create a boomerang plot as in Roering et al [2007].
  ///
  /// @details Should now do all it needs to do: if it gets any more complex I'll look at
  /// refactoring it into a few methods. 
  /// The latest addition is to take a bin_threshold to allow different sizes of bins to be 
  /// removed from the final analysis - SWDG 11/11/13.
  /// @param Slope LSDRaster of slope.
  /// @param D_inf D-infinity Flowarea LSDRaster.
  /// @param RasterFilename Filename used to give unique name to output data.
  /// @param log_bin_width Width (in log space) of the bins, with respect to D_inf.
  /// @param SplineResolution Number of values between each point for the spline curve.
  /// @param bin_threshold Threshold fraction of values needed to keep a bin.
  /// @return A pair of floats containing the two LH values in the order LH(bins), LH(splines).
  /// @author SWDG
  /// @date 27/8/13
  pair<float,float> Boomerang(LSDRaster& Slope, LSDRaster& D_inf, string RasterFilename, 
          float log_bin_width = 0.1, int SplineResolution = 200, float bin_threshold = 0.05);

  /// @brief Calculate drainage density of a set of input basins.
  /// @details Calculated as flow length/basin area and written to every
  /// cell of the identified basin. Completely rewritten to optimise performance.
  /// Now only traverses the DEM 3 times regardless of the number of input basins,
  /// previously traversed the DEM (2*No of Basins + 2) times. \n\n
  /// Works by creating a pair of vectors of basin IDs and stream length for each pixel
  /// which are then sorted by basin ID and summed in order to calculate DD in a single 
  /// pass over the DEM.    
  /// @param StreamNetwork LSDIndexRaster of the stream network.
  /// @param Basins LSDIndexRaster of the basins to be analysed.
  /// @param FlowDir Array2D of flowdirections generated by FlowInfo.get_FlowDirection().
  /// @return LSDRaster of basins coded with drainage density.
  /// @author SWDG
  /// @date 19/11/13
  LSDRaster DrainageDensity(LSDIndexRaster& StreamNetwork, LSDIndexRaster& Basins, Array2D<int> FlowDir);

  /// @brief Calculate drainage density of a DEM. 
  /// @details Calculated as flow length/raster area  
  /// @param StreamNetwork LSDIndexRaster of the stream network. 
  /// @param FlowDir Array2D of flowdirections generated by FlowInfo.get_FlowDirection(). 
  /// @return float with drainage density of raster 
  /// @author FJC 
  /// @date 27/03/15  
  float get_drainage_density_of_raster(LSDIndexRaster& StreamNetwork, Array2D<int> FlowDir); 

  /// @brief Simple method to calculate drainage density for each basin and then 
  /// convert these values into a hillslope length raster.
  ///
  /// @details The LH value is calculated using LH = 1/2*DD [Tucker et al 2001].  
  /// @param StreamNetwork LSDIndexRaster of the stream network.
  /// @param Basins LSDIndexRaster of the basins to be analysed.
  /// @param FlowDir Array2D of flowdirections generated by FlowInfo.get_FlowDirection().
  /// @return Raster of basin average hillslope lengths. 
  /// @author SWDG
  /// @date 7/11/13 
  LSDRaster HillslopeLengthFromDrainageDensity(LSDIndexRaster& StreamNetwork, 
                             LSDIndexRaster& Basins, Array2D<int> FlowDir);

  /// @brief Method to export thinned vector field data to a text file.
  ///
  /// @details The file is written with the format "i j Magnitude Direction".  
  /// @param Magnitude LSDRaster of the vector's magnitude.
  /// @param Direction LSDRaster of the vector's direction.
  /// @param output_file Filename for the output text file.
  /// @param step Integer value used to thin the data, 1 preserves all the data, 2 keeps every second point and so on.
  /// @author SWDG
  /// @date 20/1/14   
  void GetVectors(LSDRaster Magnitude, LSDRaster Direction, string output_file, int step);

  // Smoothing tools
  //Nonlocal Means Filtering - Default values following Baudes et al [2005]
  /// @brief Perform Non-local means filtering on a DEM following Baude et al [2005].
  ///
  /// @details Smoothes non-gaussian noise. Martin Hurst, February, 2012
  /// Modified by David Milodowski, May 2012- generates grid of recording filtered noise
  ///
  /// WindowRadius has to be <= SimilarityRadius ?
  ///
  /// Adapted from a matlab script by:
  /// Author: Jose Vicente Manjon Herrera & Antoni Buades
  /// Date: 09-03-2006
  ///
  /// Implementation of the Non local filter proposed for A. Buades, B. Coll and J.M. Morel in
  /// "A non-local algorithm for image denoising"
  ///
  /// **Added soft threshold optimal correction - David Milodowski, 05/2012
  /// @param WindowRadius search window radius (defualt=2).
  /// @param SimilarityRadius similarity window radius (defualt=2).
  /// @param DegreeFiltering degree of filtering (defualt=2).
  /// @param Sigma (default=1.25).
  /// @return Filtered LSDRaster object.
  /// @author MDH, DTM
  /// @date February 2012
  LSDRaster NonLocalMeansFilter(int WindowRadius=2, int SimilarityRadius=2, 
                                int DegreeFiltering=2, float Sigma=1.25);
   
  /// @brief Creates a buffer around an array (of size SimilarityRadius) and
  /// gives the new border mirror symmetric values of the original array reflected
  /// across the boundary.
  ///
  /// @details SimilarityRadius should be the size of the window if filtering.
  /// New array has size nrows + 2*SimilarityRadius by ncols + 2*SimilarityRadius.
  /// @param PaddedRasterData Padded LSDRaster.
  /// @param SimilarityRadius similarity window radius.
  /// @author Martin Hurst
  /// @date February 2012
  void PadRasterSymmetric(Array2D<float>& PaddedRasterData, int& SimilarityRadius);

  /// @brief Generate gaussian weighted kernel.
  ///
  /// @details kernel array must be predeclared of size SimilarityRadius and consist of zeros:
  /// Array2D<float> Kernel(SimilarityRadius,SimilarityRadius,0.0);
  ///
  /// Kernel generated using: G(x,y) = (1/2*pi*sigma^2) exp ((-x^2+y^2)/(2*sigma^2))
  /// @param Kernel
  /// @param sigma
  /// @param SimilarityRadius similarity window radius.
  /// @author Martin Hurst
  /// @date February 2012
  void MakeGaussianKernel(Array2D<float>& Kernel, float sigma, int SimilarityRadius);

  ///  @brief Filters the raster using a square 2D Gaussian filter.  The filter is applied
  ///  using a moving kernel of gaussian weights with a radius of 3*sigma
  ///
  ///  @details Equivalent lenghtscale of smoothing (from Lashermes et al., 2007)
  ///  original data -> 4*sigma
  ///  1st derivative -> 2*pi*sigma
  ///  2nd derivative -> sqrt(2)*pi*sigma
  ///  @param sigma
  ///  @param kr (an optional integer halfwidth for the smoothing window.  If not supplied then the window is set to cover at least three*sigma)
  ///  @return a filtered raster
  ///  @author David Milodowski
  ///  @date Feb 2015
  LSDRaster GaussianFilter(float sigma, int kr = 0);
  
  ///  @brief Filters the raster using a Perona-Malik non-linear diffusion filter
  /// 
  ///  @details This follows the algorithm descibed in Passalacqua et al. (2010), A
  ///  geometric framework for channel network extraction from lidar: Nonlinear diffusion
  ///  and geodesic paths, J. Geophys. Res., 115(F1), F01002, doi:10.1029/2009JF001254.
  ///  See also  Catte et al. (1992), Image Selective Smoothing and Edge Detection by
  ///  Nonlinear Diffusion, SIAM J. Numer. Anal., 29(1), 182-193, doi:10.1137/0729012.
  ///  @param timesteps the number of diffusion timesteps.  Suggest ~50 for 1m LiDAR
  ///  @param lambda percentile (selects the gradient percentile that is used to define lambda.  Suggest 90th percentile)
  ///  @param dt The timestep for each round of diffusion.  Suggest that this is 0.1 to avoid stability issues (same as Passalacqua et al. 2010)
  ///  @return a filtered raster
  ///  @author David Milodowski
  ///  @date Feb 2015
  LSDRaster PeronaMalikFilter(int timesteps, float percentile_for_lambda, float dt);


  //D-infinity tools

  /// @brief D-infinity flow direction algorithm after Tarboton (1997).
  ///
  /// @details Algorithm takes a filled DEM and for each cell calculates the steepest descent
  /// based on 8 triangular facets. Flow direction is assigned as an angle from 0-360
  /// degrees with -1 used to flag unresolved areas such as pits.
  ///
  /// Code is ported and optimised from a Java implementation of the algorithm
  /// supplied under the GNU GPL licence through WhiteBox GAT:
  /// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
  /// to the whitebox tool.
  /// @return Array of Flow directions in degrees.
  /// @author SWDG
  /// @date 26/07/13
  Array2D<float> D_inf_FlowDir();

  /// @brief Main function for generating a D-infinity flow area raster after Tarboton (1997).
  ///
  /// @details Calls the recurisve D_infAccum function to get flow area for each pixel.
  /// Returns flow area in pixels.
  ///
  /// Code is ported and optimised from a Java implementation of the algorithm
  /// supplied under the GNU GPL licence through WhiteBox GAT:
  /// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
  /// to the whitebox tool.
  /// @param FlowDir_array Array of Flowdirections generated by D_inf_FlowDir().
  /// @return LSDRaster of D-inf flow areas in pixels.
  /// @author SWDG
  /// @date 26/07/13
  LSDRaster D_inf_FlowArea(Array2D<float> FlowDir_array);

  /// @brief Recursive function to calculate accumulating area for a given pixel.
  ///
  /// @details Called by the driver for every cell which has no contributing
  /// cells - eg the highest points on the landscape. Avoids the need to flatten
  /// and sort the DEM as required in the original Tarboton (1997)
  /// implementation. For more detail on the recursive algorithm following
  /// channels see Mark (1998) "Network Models in Geomorphology".
  ///
  /// Code is ported and optimised from a Java implementation of the algorithm
  /// supplied under the GNU GPL licence through WhiteBox GAT:
  /// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
  /// to the whitebox tool.
  ///
  /// @param i Row index of the target cell.
  /// @param j Column index of the target cell.
  /// @param CountGrid Array showing the number of cells flowing into each cell.
  /// @param Flowarea_Raster Array of the flow areas which is populated using this function.
  /// @param FlowDir_array Array of Flowdirections generated by D_inf_FlowDir().
  /// @author SWDG
  /// @date 26/07/13
  void D_infAccum(int i, int j, Array2D<float> CountGrid, Array2D<float> Flowarea_Raster, 
                  Array2D<float> FlowDir_array);

  /// @brief Wrapper Function to create a D-infinity flow area raster with one function call.
  /// @return LSDRaster of D-inf flow areas in pixels.
  /// @author SWDG
  /// @date 26/07/13
  LSDRaster D_inf();

  /// @brief Wrapper Function to create a D-infinity flow area raster, in spatial units, with one function call.
  /// @return LSDRaster of D-inf flow areas in spatial units.
  /// @author SWDG
  /// @date 16/10/13  
  LSDRaster D_inf_units();

	///@brief Wrapper Function to convert a D-infinity flow raster into spatial units.
  /// @return LSDRaster of D-inf flow areas in spatial units.
  /// @author MDH (after SWDG)
  /// @date 5/9/14  
	LSDRaster D_inf_ConvertFlowToArea();
	
  /// @brief Function to write the D-infinity flow directions to an LSDRaster.
  /// @param dinflow Array of Flowdirections generated by D_inf_FlowDir().
  /// @return LSDRaster of D-inf flow directions in degrees.
  /// @author SWDG
  /// @date 26/07/13
  LSDRaster write_dinf_flowdir_to_LSDRaster(Array2D<float> dinflow);

	/// @brief Function to generate upslope contributing area extent from the D-infinity flow directions to an LSDRaster.
  /// @param dinflow LSDRaster of Flowdirections generated by D_inf_FlowDir().
  /// @param i and j indices of pour point node
  /// @return LSDIndexRaster of catchment
  /// @author MDH (after SWDG)
  /// @date 26/08/14
  LSDIndexRaster D_inf_watershed(LSDRaster D_inf_FlowDir, int PourRow, int PourCol);
  
  /// @brief Function to calculate the topographic index, a moisture
  /// distribution indicator
  ///
  /// @details Calculates the topographic index, defined as ln(A/S) (Kirkby,
  /// 1975), where A is the accumulation area and S is the slope.  This is an 
  /// indicator of the distribution of moisture within the topography.
  /// @param LSDRaster of Accumulations Area, calculated ideally with D-inf
  /// @param LSDRaster of Slope
  /// @return LSDRaster of Topographic Index
  /// @author DTM
  /// @date 01/04/2014
  LSDRaster calculate_topographic_index(LSDRaster& AccumulationArea, LSDRaster& Slope);

  /// @brief Method to turn a point shapefile into an LSDIndexRaster.
  ///
  /// @details Can be used to turn a shapefile of channel heads into a sources raster. Does not do 
  /// any bounds checking or shapefile type checking.
  /// @param FileName The path and filename of the shapefile to be loaded, must include the file extension.
  /// @return An LSDIndexRaster converted from a point shapefile.
  /// @author SWDG
  /// @date 13/3/14                     
  LSDIndexRaster PointShapefileToRaster(string FileName);
  
  /// @brief Method to turn a polyline shapefile into an LSDIndexRaster.
  ///
  /// @details Can be used to turn a shapefile of a river network into a raster. Does not do 
  /// any bounds checking or shapefile type checking.
  ///
  /// Works by calculating points along each line spaced by less than the data resolution. 
  /// This has the effect of flaggin every raster cell along a polyline. 
  /// @param FileName The path and filename of the shapefile to be loaded, must include the file extension.
  /// @return An LSDIndexRaster converted from a polyline shapefile.
  /// @author SWDG
  /// @date 21/3/14     
  LSDIndexRaster PolylineShapefileToRaster(string FileName);
  
  /// @brief Method to resample an LSDRaster to a lower resolution. 
  /// @param OutputResolution the resolution in spatial units to be resampled to.
  /// @return An LSDRaster resampled to the OutputResolution.
  /// @author SWDG
  /// @date 17/3/14   
  LSDRaster Resample(float OutputResolution);
  
  /// @brief creates a circular mask for neighbourhood statistics
  ///  
  /// @details The second argument (neighbourhood_switch) specifies the neighbourhood type:
  ///   0 Square neighbourhood
  ///   1 Circular window
  /// @param window radius of mask 
  /// @param int neighbourhood_switch -> see above
  /// @return a binary array with the mask
  /// @author DTM 
  /// @date 20/06/2014 
  Array2D<int> create_mask(float window_radius,  int neighbourhood_switch);

  /// @brief gets mean value for specified circular neighbourhood
  ///
  /// @details The second argument (neighbourhood_switch) specifies the neighbourhood type:
  ///   0 Square neighbourhood
  ///   1 Circular window
  /// @param float window_radius -> radius of neighbourhood 
  /// @param int neighbourhood_switch -> see above
  /// @return an LSDRaster with the mean value of cells in neightbourhood 
  /// @author DTM 
  /// @date 20/06/2014   
  LSDRaster neighbourhood_statistics_spatial_average(float window_radius, int neighbourhood_switch);

  /// @brief gets mean & standard deviation value specified circular neighbourhood
  ///
  /// @details The second argument (neighbourhood_switch) specifies the neighbourhood type:
  ///   0 Square neighbourhood
  ///   1 Circular window
  /// @param float window_radius -> radius of neighbourhood 
  /// @param int neighbourhood_switch -> see above
  /// @return vector<LSDRaster> with 2 LSDRasters, the first with the mean value 
  /// of cells in neighbourhood, the second with the standard deviation
  /// @author DTM 
  /// @date 24/09/2014    
  vector<LSDRaster> neighbourhood_statistics_spatial_average_and_SD(float window_radius, 
                                                    int neighbourhood_switch);

  /// @brief gets relief within value specified circular neighbourhood
  ///
  /// @details The second argument (neighbourhood_switch) specifies the neighbourhood type:
  ///   0 Square neighbourhood
  ///   1 Circular window
  /// @param float window_radius -> radius of neighbourhood 
  /// @param int neighbourhood_switch -> see above
  /// @return LSDRasters contianing the relief within a neighbourhood
  /// @author SMM 
  /// @date 16/11/2014    
  LSDRaster neighbourhood_statistics_local_relief(float window_radius, 
                                                    int neighbourhood_switch);
  
  /// @brief tests neighbourhood for the fraction of values for which the specified 
  /// condition is met.
  ///   
  /// The second argument (neighbourhood_switch) specifies the neighbourhood type:
  ///   0 Square neighbourhood
  ///   1 Circular window
  /// @details the third argument (condition_switch) specifies the condition using
  /// an integer as follows:
  ///   0 ==
  ///   1 !=
  ///   2 >
  ///   3 >=
  ///   4 <
  ///   5 <=
  /// @param float window_radius -> radius of neighbourhood 
  /// @param int neighbourhood_switch -> see above
  /// @param int condition_switch -> see above
  /// @param float test_value -> the value to test against in the conditional statement  
  /// @return an LSDRaster with the fraction of cells in neightbourhood for which condition statement is true  
  /// @author DTM 
  /// @date 20/06/2014 
  LSDRaster neighbourhood_statistics_fraction_condition(float window_radius, int neighbourhood_switch, 
                    int condition_switch,  float test_value);
  

  /// @brief Function to change border pixels to nodata
  /// @param int border_width
  /// @param int irregular_switch (0 (default) -> only creates border pixels around raster edge;
  /// 1 -> creates border around edges and nodata values e.g. for irregularly shaped datasets)
  /// @return Updated LSDRaster
  /// @author DTM
  /// @date 29/05/2014
  LSDRaster border_with_nodata(int border_width, int irregular_switch = 0);

  /// @brief This function creates a mask that has the value of 1 for cells 
  /// that are either on the edge or bordered by nodata (from their 9 neighbors)
  /// @return and LSDIndexRaster with the mask
  /// @author SMM
  /// @date 29/10/2014
  LSDIndexRaster find_cells_bordered_by_nodata();


  /// @brief This looks for isolated instances of no data.
  ///
  /// Does nothing else but print their location to the screen.
  /// @author MDH, DTM
  /// @date 01/01/12
  void check_isolated_nodata();

  /// @brief a very rudimentay filling routine that just fills nodata nodes
  /// with the average elevation of its neighbors
  /// @author SMM
  /// @date 04/11/2014
  void rudimentary_nodata_fill();

  /// @brief A routine that fills nodata holes. It cannot recognise nodata
  ///  around the edges so the spiral trimmer should be used first
  /// @detail The routine sweeps the raster looking for nodata and filling
  ///  this nodata with an average value from surrounding nodes. The sweeping
  ///  changes directions, four sweep directions in all (+ rows, - rows, + cols, -cols)
  ///  and it alternates between these directions until the raster is filled. 
  /// @param window_size the number of pixles around the centre pixel to take the 
  ///  spatial average
  /// @author SMM
  /// @date 09/12/2014
  LSDRaster alternating_direction_nodata_fill(int window_width);

  /// @brief A routine that fills nodata holes. It first prepares the data
  ///  with the sprial trimmer so nodata around the edges is removed.
  /// @detail The routine sweeps the raster looking for nodata and filling
  ///  this nodata with an average value from surrounding nodes. The sweeping
  ///  changes directions, four sweep directions in all (+ rows, - rows, + cols, -cols)
  ///  and it alternates between these directions until the raster is filled.
  ///  The routine is particularly useful for data with holes that is to be
  ///  prepared for spectral analysis. 
  /// @param window_size the number of pixles around the centre pixel to take the 
  ///  spatial average
  /// @author SMM
  /// @date 09/12/2014
  LSDRaster alternating_direction_nodata_fill_with_trimmer(int window_width);
  
  /// @brief Function to create a masked LSDIndexRaster raster based on a conditional statement
  /// @param string Condition ("<", ">", "==", "!=")
  /// @param float TestValue The value that the condition is tested against
  /// @return LSDIndexRaster of the mask
  /// @author MDH
  /// @date 27/08/2014
	LSDIndexRaster Create_Mask(string Condition, float TestValue);
	
	/// @brief Function to extract an LSDRaster based on a LSDIndexRaster mask
  /// @param LSDIndexRaster TheMask
  /// @return masked LSDRaster 
  /// @author MDH
  /// @date 27/08/2014
	LSDRaster ExtractByMask(LSDIndexRaster Mask);

  /// @brief method to locate channel pixels outlined by Lashermes.
  /// 
  /// @detail picks departure from gaussian behaviour, then uses this as a threshold to create a binary dataset.
  /// Lashermes, B., E. Foufoula-Georgiou, and W. E. Dietrich (2007), Channel network extraction from high resolution topography using wavelets, Geophys. Res. Lett., 34, L23S04, doi:10.1029/2007GL031140. 
  /// @param string q_q_file A text file containing data to produce a q_q_plot.
  /// @return LSDIndexRaster A binary raster where the pixel value is 1 where the input raster exceeded the defined threshold 
  /// @author DTM
  /// @date 11/02/2015 
  LSDIndexRaster IsolateChannelsLashermesCurvature(float sigma, string q_q_filename);
  LSDIndexRaster IsolateChannelsLashermesAspect(float sigma, string q_q_filename);
  LSDIndexRaster IsolateChannelsLashermesFull(float sigma, string q_q_filename_prefix);
  LSDIndexRaster IsolateChannelsLashermesCurvatureArea(float sigma, float area_threshold, string q_q_filename);
  /// @brief method to locate channel pixels outlined by Passalacqua et al. (2010).
  /// 
  /// @detail Isolate channelised portions of the landscape using the method proposed by Passalacqua
  /// et al. (2010) A geometric framework for channel network extraction from lidar: Nonlinear
  /// diffusion and geodesic paths, J. Geophys. Res., 115(F1), F01002, doi:10.1029/2009JF001254.
  ///
  /// @param number of timesteps for non-linear filtering steps - suggest 20 in complex terrain
  /// @param a catchment area threshold for pruning
  /// @param a window radius for surface fitting from which curvature calculation is performed
  /// @return LSDIndexRaster A binary raster where the pixel value is 1 where the input raster exceeded the defined threshold 
  /// @author DTM
  /// @date 21/04/2015
  LSDIndexRaster IsolateChannelsGeonet(float timesteps, float area_threshold, float window_radius, string q_q_filename);
  /// @brief uses quantile-quantile analysis to pick departure from gaussian behaviour, then uses this as a threshold to create a binary dataset. 
  /// @param string q_q_file A text file containing data to produce a q_q_plot.
  /// @return LSDIndexRaster A binary raster where the pixel value is 1 where the input raster exceeded the defined threshold 
  /// @author DTM     /// @return LSDIndexRaster A binary raster where the pixel value is 1 where the input raster exceeded the defined threshold 
  /// @author DTM
  /// @date 10/02/2015 
  LSDIndexRaster IsolateChannelsQuantileQuantile(string q_q_filename);
  LSDIndexRaster IsolateChannelsQuantileQuantileAdaptive(int half_width);
  
  /// @brief Function to calculate the curvature threshold used for DrEICH channel extraction which varies across the landscape
  /// @param half_width radius over which to calculate the curvature threshold
  /// @return Array2D<float> array with curvature threshold for each row and col 
  /// @author FJC   
  /// @date 20/07/15   
  Array2D<float> CalculateAdaptiveCurvatureThresholdSD(int half_width);

  /// @brief Method to convert an LSDRaster hilltop file into a series of contigiuous hilltop patches.
  ///
  /// @details Connects all contiguous patches of hilltop via a unique ID, allowing hilltop patches
  /// to be generated in a manner similar to the methodology employed by Hurst et al. (2012) and (2013)
  /// to spatially average data for E*R* calculations. Currently has no method to limit the maximum size
  /// of segments.
  /// @param minimum_patch_size Integer of the minimum number of pixels required for a patch to be created
  /// @return An LSDIndexRaster of hilltop patches.
  /// @author SWDG
  /// @date 5/6/15
  LSDIndexRaster CreateHilltopPatches(int minimum_patch_size);
  LSDIndexRaster CreateHilltopPatchesNEW(int minimum_patch_size);
  /// @brief Simple method to flatten an LSDRaster to a text file, with a sigle value on each line. 
  /// @param FileName The name of the file to write, if no path is included it will write to the current directory. 
  /// @author SWDG
  /// @date 9/2/15
  void FlattenToFile(string FileName);

  /// @brief Method to flatten an LSDRaster to a text file, with a sigle value on each line. 
  /// @brief Method to flatten an LSDRaster and place the non NDV values in a csv file.
  /// @detail Each value is placed on its own line, so that it can be read more quickly in python etc.
  ///   It includes the x and y locations so it can be read by GIS software
  /// @param FileName_prefix The prefix of the file to write, if no path is included it will write to the current directory. 
  ///  The csv extension is added automatically. 
  /// @author SMM
  /// @date 29/6/15
  void FlattenToCSV(string FileName);

  /// @brief Simple method to remove any values below a user supplied value from an LSDRaster. 
  /// @param Value float of the threshold below which values will be removed. 
  /// @author SWDG
  /// @date 22/6/15
  LSDRaster RemoveBelow(float Value);

  /// @brief apply a mask to a raster dataset, converts pixels to nodata where mas=k=1
  /// @author DTM
  /// @date 25/08/2015
  LSDRaster apply_mask(LSDIndexRaster& mask);

  /// @brief Method to convert all values in an LSDRaster to a single value.
  /// @param Value, an integer value that will be assigned to every non NDV cell in the raster.
  /// @param ndv an integer no data value.
  /// @author SWDG
  /// @date 24/07/2015
  LSDIndexRaster ConvertToBinary(int Value, int ndv);
  
  /// @brief Function to get potential floodplain patches using a slope and relief threshold
  /// @param Relief raster with relief values
  /// @param Slope raster with slope values
  /// @param relief_threshold threshold value for relief (floodplain must be lower than this)
  /// @param slope_threshold threshold value for slope (floodplain must be lower than this)
  /// @author FJC
  /// @date 20/10/15
  LSDIndexRaster get_potential_floodplain_patches(LSDRaster& Relief, LSDRaster& Slope, float relief_threshold,
                                                     float slope_threshold);
                                                     
  /// @brief Function to get threshold from a raster using a histogram of Probability-Density
  /// values. The function identifies the peaks in the PDF, and sets the threshold to
  /// the peak with the lowest value.  This can be used to set the floodplain threshold.
  /// @param bin_width Bin width for histogram
  /// @param peak_threshold Threshold probability-density value above which peaks will be selected
  /// @param peak_distance Threshold distance between identified peaks (will remove smaller peaks close to
  /// the largest ones)
  /// @author FJC
  /// @date 16/11/15                                                  
  float get_threshold_for_floodplain(float bin_width, float peak_threshold, int peak_distance);

  protected:

  ///Number of rows.
  int NRows;
  ///Number of columns.
  int NCols;
  ///Minimum X coordinate.
  float XMinimum;
  ///Minimum Y coordinate.
  float YMinimum;

  ///Data resolution.
  float DataResolution;
  ///No data value.
  int NoDataValue;

  ///A map of strings for holding georeferencing information
  map<string,string> GeoReferencingStrings;

  /// Raster data.
  Array2D<float> RasterData;
  
  /// These are used primarily by LSDCatchmentModel
  Array2D<double> RasterData_dbl;
  Array2D<int> RasterData_int;



  private:
  void create();
  void create(string filename, string extension);
  void create(int ncols, int nrows, float xmin, float ymin,
              float cellsize, float ndv, Array2D<float> data);
  void create(int ncols, int nrows, float xmin, float ymin,
              float cellsize, float ndv, Array2D<float> data, map<string,string> GRS);

};

#endif
