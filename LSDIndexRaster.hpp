//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDIndexRaster
// Land Surface Dynamics IndexRaster
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for manipulating
//  and analysing raster data, with a particular focus on topography
//
// The IndexRaster object stores only integer values and is used mostly
//  for storing indices into raster data.
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

/** @file LSDIndexRaster.hpp
@author Simon M. Mudd, University of Edinburgh
@author David Milodowski, University of Edinburgh
@author Martin D. Hurst, British Geological Survey
@author Stuart W. D. Grieve, University of Edinburgh
@author Fiona Clubb, University of Edinburgh

@version Version 0.0.1
@brief Object to handle integer rasters.

@date 20/08/2012
*/

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------



#ifndef LSDIndexRaster_H
#define LSDIndexRaster_H

#include <string>
#include <vector>
#include <map>
#include "TNT/tnt.h"
//#include "LSDRaster.hpp"

using namespace std;
using namespace TNT;

class LSDRaster;        // do not include since it wouldn't compile since there
                        // would be a looped dependency

/// @brief Object to handle integer rasters.
class LSDIndexRaster
{
  public:
  /// @brief  The create function. This is default and throws an error.
  /// @author SMM
  /// @date 01/01/12
  LSDIndexRaster()              { create(); }

  /// @brief Create an LSDIndexRaster from a file.
  /// Uses a filename and file extension
  /// @return LSDIndexRaster
  /// @param filename A String, the file to be loaded.
  /// @param extension A String, the file extension to be loaded.
  /// @author SMM
  /// @date 01/01/12
  LSDIndexRaster(string filename, string extension)  { create(filename, extension); }

  /// @brief Create an LSDIndexRaster from memory.
  /// @return LSDIndexRaster
  /// @param nrows An integer of the number of rows.
  /// @param ncols An integer of the number of columns.
  /// @param xmin A float of the minimum X coordinate.
  /// @param ymin A float of the minimum Y coordinate.
  /// @param cellsize A float of the cellsize.
  /// @param ndv An integer of the no data value.
  /// @param data An Array2D of integers in the shape nrows*ncols,
  ///containing the data to be written.
  /// @author SMM
  /// @date 01/01/12
  LSDIndexRaster(int nrows, int ncols, float xmin, float ymin,
            float cellsize, int ndv, Array2D<int> data)
                  { create(nrows, ncols, xmin, ymin, cellsize, ndv, data); }

  /// @brief Create an LSDIndexRaster from memory.
  /// @return LSDIndexRaster
  /// @param nrows An integer of the number of rows.
  /// @param ncols An integer of the number of columns.
  /// @param xmin A float of the minimum X coordinate.
  /// @param ymin A float of the minimum Y coordinate.
  /// @param cellsize A float of the cellsize.
  /// @param ndv An integer of the no data value.
  /// @param data An Array2D of integers in the shape nrows*ncols,
  /// @param GRS_map a map containing information about the georeferencing
  /// containing the data to be written.
  /// @author SMM
  /// @date 09/09/14
  LSDIndexRaster(int nrows, int ncols, float xmin, float ymin,
            float cellsize, int ndv, Array2D<int> data, map<string,string> GRS_map)
             { create(nrows, ncols, xmin, ymin, cellsize, ndv, data, GRS_map); }


  /// @brief Create an LSDIndexRaster from an LSDRaster object, rounding to nearest int
  /// @return LSDIndexRaster
  /// @param NonIntLSDRaster an LSDRaster object containing flaoting point data
  /// @author MDH
  /// @date 17/02/15
  LSDIndexRaster(LSDRaster& NonIntLSDRaster)   {create(NonIntLSDRaster); }
  
  // Get functions

  /// @return Number of rows as an integer.
  int get_NRows() const           { return NRows; }
  /// @return Number of columns as an integer.
  int get_NCols() const           { return NCols; }
  /// @return Minimum X coordinate as an integer.
  float get_XMinimum() const           { return XMinimum; }
  /// @return Minimum Y coordinate as an integer.
  float get_YMinimum() const           { return YMinimum; }
  /// @return Data resolution as an integer.
  float get_DataResolution() const           { return DataResolution; }
  /// @return No Data Value as an integer.
  int get_NoDataValue() const           { return NoDataValue; }
  /// @return Raster values as a 2D Array.
  Array2D<int> get_RasterData() const { return RasterData; }
  /// @return Map of strings containing georeferencing information
  map<string,string> get_GeoReferencingStrings() const { return GeoReferencingStrings; }

  /// Assignment operator.
  LSDIndexRaster& operator=(const LSDIndexRaster& LSDIR);

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
  /// @author SMM
  /// @date 01/01/12
  void read_raster(string filename, string extension);

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
  /// @author SMM
  /// @date 01/01/12
  void write_raster(string filename, string extension);

  /// @brief Get the raster data at a specified location.
  /// @param row An integer, the X coordinate of the target cell.
  /// @param column An integer, the Y coordinate of the target cell.
  /// @return The raster value at the position (row, column).
  /// @author SMM
  /// @date 01/01/12
  int get_data_element(int row, int column)  { return RasterData[row][column]; }

  /// @brief Sets the raster data at a specified location.
  /// @param row An integer, the X coordinate of the target cell.
  /// @param column An integer, the Y coordinate of the target cell.
  /// @param value the vaule of the data element at the give row and column
  /// @return The raster value at the position (row, column).
  /// @author SMM
  /// @date 18/03/15
  void set_data_element(int row, int column, int data) 
                                   { RasterData[row][column] = data; }

  /// @brief this gets the x and y location of a node at row and column
  /// @param row the row of the node
  /// @param col the column of the node
  /// @param x_loc the x location (Northing) of the node
  /// @param y_loc the y location (Easting) of the node
  /// @author SMM
  /// @date 22/12/2014
  void get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc);

  /// @brief Method to flatten an LSDRaster to a text file, with a sigle value on each line. 
  /// @brief Method to flatten an LSDRaster and place the non NDV values in a csv file.
  /// @detail Each value is placed on its own line, so that it can be read more quickly in python etc.
  ///   It includes the x and y locations so it can be read by GIS software
  /// @param FileName_prefix The prefix of the file to write, if no path is included it will write to the current directory. 
  ///  The csv extension is added automatically. 
  /// @author SMM
  /// @date 29/6/15
  void FlattenToCSV(string FileName);


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

  /// @brief This returns a clipped raster that has the same dimensions as the
  ///  smaller raster
  /// @param smaller_raster the raster to which the bigger raster should be
  ///  clipped
  /// @author SMM
  /// @date 20/03/2015
  LSDIndexRaster clip_to_smaller_raster(LSDRaster& smaller_raster);

  /// @brief This returns a clipped raster that has the same dimensions as the
  ///  smaller raster
  /// @param smaller_raster the raster to which the bigger raster should be
  ///  clipped
  /// @author SMM
  /// @date 20/03/2015
  LSDIndexRaster clip_to_smaller_raster(LSDIndexRaster& smaller_raster);


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

  /// @brief this check to see if a point is within the raster
  /// @param X_coordinate the x location of the point
  /// @param Y_coordinate the y location of the point
  /// @return is_in_raster a boolean telling if the point is in the raster
  /// @author SMM
  /// @date 13/11/2014
  bool check_if_point_is_in_raster(float X_coordinate,float Y_coordinate);


  /// @brief Calculate the minimum bounding rectangle for an LSDIndexRaster Object and crop out
  /// all the surrounding NoDataValues to reduce the size and load times of output rasters.
  ///
  /// @details Ideal for use with chi analysis tools which output basin and chi m value rasters
  /// which can be predominantly no data. As an example, a 253 Mb file can be reduced to
  /// ~5 Mb with no loss or resampling of data.
  ///
  /// @return A trimmed LSDIndexRaster object.
  /// @author SWDG
  /// @date 22/08/13
  LSDIndexRaster RasterTrimmer();

  /// @brief Make LSDIndexRaster object using a 'template' raster and an Array2D of data.
  /// @param InputData 2DArray of ints to be written to LSDIndexRaster.
  /// @return LSDRaster containing the data passed in.
  /// @author SWDG
  /// @date 02/9/13
  LSDIndexRaster LSDRasterTemplate(Array2D<int> InputData);
  
  /// @brief Method to resample an LSDIndexRaster to a lower resolution. 
  /// @param OutputResolution the resolution in spatial units to be resampled to.
  /// @return An LSDIndexRaster resampled to the OutputResolution.
  /// @author SWDG
  /// @date 17/3/14   
  LSDIndexRaster Resample(float OutputResolution);

  /// @brief Method to combine two rasters, ignoring nodata.  
  /// @param Network1 The first raster to be combined.
  /// @param Network2 The second raster to be combined.
  /// @return An LSDIndexRaster of the combined inputs.
  /// @author SWDG
  /// @date 17/6/14
  LSDIndexRaster CombineBinaryNetwork(LSDIndexRaster& Network1, LSDIndexRaster& Network2);
    
  /// @brief Method to merge a floodplain raster with a channel raster. 
  ///
  /// @details Creates an output LSDIndexRaster which is coded coded 
  /// channel == input channel index, floodplain == 500, NDV == hillslopes.
  /// This allows the preservation of the stream order in addition to the floodplain geometry.
  /// @param FloodPlain an LSDIndexRaster of the floodplains coded with any integer value.
  /// @return An LSDIndexRaster of the merged channels and floodplains.     
  /// @author SWDG
  /// @date 05/03/15
  LSDIndexRaster MergeChannelWithFloodplain(LSDIndexRaster FloodPlain);


  /// @brief Method to identify connected components using a two pass method
  ///
  /// @details Takes a binary array, where components parts are identifed by 1
  /// and separates into connected components according to a two-pass algorithm
  /// based on that described in He et al. (2008), "A Run-Based Two-Scan
  /// Labeling Algorithm," Image Processing, IEEE Transactions on , vol.17, no.5,
  /// pp.749,756, doi: 10.1109/TIP.2008.919369
  /// @return an LSDRaster with labelled connected components
  /// @author DTM
  /// @date 13/07/2015
  LSDIndexRaster ConnectedComponents();

  /// @brief Method to filter a binary array according to a connected components threshold
  ///
  /// @author DTM
  /// @date 22/07/2015
  LSDIndexRaster filter_by_connected_components(int connected_components_threshold);


  /// @brief A method to thin a multipixel feature (binary) to a single thread skeleton
  ///
  /// @details Takes a binary array in which features are identified as 1, and
  /// background 0, and thins the features down to a skeleton, using the thinning
  /// algorithm described by Zhang and Suen, 1984, "A fast algorithm for thinning
  /// digital patterns", Communications of the ACM.
  /// @return an LSDIndexRaster with the skeleton
  /// @author DTM
  /// @date 15/07/2015
  LSDIndexRaster thin_to_skeleton();
  void thinningIteration(Array2D<int>& binary, int iter);

  LSDIndexRaster find_end_points();
  void remove_downstream_endpoints(LSDIndexRaster CC, LSDRaster Topo);

  /// @brief Method to convert all values in an LSDIndexRaster to a single value.
  /// @param Value, an integer value that will be assigned to every non NDV cell in the raster.
  /// @param ndv an integer no data value.
  /// @author SWDG
  /// @date 24/07/2015
  LSDIndexRaster ConvertToBinary(int Value, int ndv);
  
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
  Array2D<int> RasterData;

  private:
  void create();
  void create(string filename, string extension);
  void create(int ncols, int nrows, float xmin, float ymin,
              float cellsize, int ndv, Array2D<int> data);
  void create(int ncols, int nrows, float xmin, float ymin,
              float cellsize, int ndv, Array2D<int> data, 
              map<string,string> GRS_map);
  void create(LSDRaster& NonIntLSDRaster);            

};

#endif
