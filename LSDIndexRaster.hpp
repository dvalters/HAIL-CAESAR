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

#include <string>
#include <vector>
#include "TNT/tnt.h"
using namespace std;
using namespace TNT;

#ifndef LSDIndexRaster_H
#define LSDIndexRaster_H

/// @brief Object to handle integer rasters.
class LSDIndexRaster
{
	public:
	/// @brief  The create function. This is default and throws an error.
	/// @author SMM
  /// @date 01/01/12
	LSDIndexRaster()										{ create(); }

	/// @brief Create an LSDIndexRaster from a file.
  /// Uses a filename and file extension
  /// @return LSDIndexRaster
  /// @param filename A String, the file to be loaded.
  /// @param extension A String, the file extension to be loaded.
	/// @author SMM
  /// @date 01/01/12
	LSDIndexRaster(string filename, string extension)	{ create(filename, extension); }

	/// @brief Create an LSDIndexRaster from memory.
  /// @return LSDIndexRaster
  /// @param nrows An integer of the number of rows.
  /// @param ncols An integer of the number of columns.
  /// @param xmin A double of the minimum X coordinate.
  /// @param ymin A double of the minimum Y coordinate.
  /// @param cellsize A double of the cellsize.
  /// @param ndv An integer of the no data value.
  /// @param data An Array2D of integers in the shape nrows*ncols,
  ///containing the data to be written.
 	/// @author SMM
  /// @date 01/01/12
	LSDIndexRaster(int nrows, int ncols, double xmin, double ymin,
	          double cellsize, int ndv, Array2D<int> data)
								{ create(nrows, ncols, xmin, ymin, cellsize, ndv, data); }

	// Get functions

	/// @return Number of rows as an integer.
	int get_NRows() const				{ return NRows; }
	/// @return Number of columns as an integer.
  int get_NCols() const				{ return NCols; }
  /// @return Minimum X coordinate as an integer.
	double get_XMinimum() const			{ return XMinimum; }
	/// @return Minimum Y coordinate as an integer.
	double get_YMinimum() const			{ return YMinimum; }
	/// @return Data resolution as an integer.
	double get_DataResolution() const	{ return DataResolution; }
	/// @return No Data Value as an integer.
	int get_NoDataValue() const			{ return NoDataValue; }
	/// @return Raster values as a 2D Array.
	Array2D<int> get_RasterData() const { return RasterData; }

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
  int get_data_element(int row, int column)	{ return RasterData[row][column]; }

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

	protected:
	///Number of rows.
  int NRows;
  ///Number of columns.
	int NCols;
	///Minimum X coordinate.
  double XMinimum;
	///Minimum Y coordinate.
	double YMinimum;

	///Data resolution.
	double DataResolution;
	///No data value.
	int NoDataValue;

	/// Raster data.
	Array2D<int> RasterData;

	private:
	void create();
	void create(string filename, string extension);
	void create(int ncols, int nrows, double xmin, double ymin,
	            double cellsize, int ndv, Array2D<int> data);

};

#endif
