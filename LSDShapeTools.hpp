//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDShapeTools
// Land Surface Dynamics Shapefile tools
//
// A collection of routines for maipulating the binary ESRI shapefile format
// for use within the Edinburgh Land Surface Dynamics group topographic toolbox
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

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

using namespace std;
#include <vector>
#include <cstdlib> 
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <cmath>

#ifndef ShapeTools_H
#define ShapeTools_H

// An unsigned char can store 1 Byte (8bits) of data
typedef unsigned char BYTE;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to test the Byte order of the system.
// Returns a boolean value where true is little endian.
//
// SWDG 11/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool SystemEndiannessTest();


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Structure to store X and Y point data.
//
// SWDG 12/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
struct PointData
{
  
  vector<double> X;
  vector<double> Y;

};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to swap the byte order of a word in memory. Used if the system's byte order
// does not match the data in the shapefile.
//
// SWDG 11/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ByteSwap(int length, void * ByteData);


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to load an ESRI ShapeFile.
// 
// Only works for X,Y point shapefiles at present and it's behavious is totally undefined 
// if you pass in any other type of file.
//  
// In future this will be rebuilt into a full class that can support shapefiles of 
// different types.
//
// Built in part from:
// http://www.dreamincode.net/forums/topic/170054-understanding-and-reading-binary-files-in-c/ 
//
// SWDG 13/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
PointData LoadShapefile(string Filename);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to load an ESRI polyline Shapefile.
// 
// Only works for polyline shapefiles at present and it's behaviour is totally undefined 
// if you pass in any other type of file.
//  
// In future this will be rebuilt into a full class that can support shapefiles of 
// different types.
//
// Returns a vector of points. So that each item in the vector represents a single polyline.
//
// Built in part from:
// http://www.dreamincode.net/forums/topic/170054-understanding-and-reading-binary-files-in-c/ 
//
// SWDG 17/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<PointData> LoadPolyline(string Filename);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to convert an IndexChannelTree to a PointData object.
// 
// Returns a vector of points.
//
// multistem_option -> 0 = mainstem only, 1 = all tributaries, 2 specify tributary number (DAV 09/04/2015)
// DTM 11/07/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
PointData LoadChannelTree(string Filename, int multistem_option = 0, int trib_number = 0);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to get the size of the binary file being loaded.
//
// Taken from http://www.dreamincode.net/forums/topic/170054-understanding-and-reading-binary-files-in-c/ 
//
// SWDG 10/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
long getFileSize(FILE *file);

/// @brief Ellipsoid class for converting coordinates between UTM and lat long
class LSDEllipsoid
{
  public:
  
  /// @detail the default constructor
  LSDEllipsoid()    {};
  
  /// @detail assigment constructor for the ellipsiod class
  /// @param id a reference into the ellipsoid
  /// @param name the name of the ellipsoid
  /// @param radius the radius of the equator in km
  /// @param fr not sure what this is
  LSDEllipsoid(int id, char* name, double radius, double fr)
      { Name=name;  EquatorialRadius=radius;  eccSquared=2/fr-1/(fr*fr);}
  
  /// name of the ellipsoid
  char* Name;
  
  /// equatorial radius in km
  double EquatorialRadius;
  
  /// square of the equatorial radius 
  double eccSquared;
};

/// @brief Datum class for converting coordinates between UTM and lat long
class LSDDatum
{
  public:
    LSDDatum(){};
    LSDDatum(int id, char* name, int eid, double dx, double dy, double dz)
      { Name=name;  eId=eid;  dX=dx;  dY=dy;  dZ=dz;}
  
  /// name of the datum
  char* Name;
  
  /// the ellipsoid id
  int   eId;
  
  double dX;
  double dY;
  double dZ;
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// A class for converting datums and coordinates
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class LSDCoordinateConverterLLandUTM
{
  public:
    // default constructor. This sets up the data elements. 
    LSDCoordinateConverterLLandUTM()     { create(); }

    /// @brief converts LatLong to UTM coords
    /// 3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
    /// @param eID the ellipsoid ID. Options are:
    ///  0 = "Airy1830";
    ///  1 = "AiryModified";
    ///  2 = "AustralianNational";
    ///  3 = "Bessel1841Namibia";
    ///  4 = "Bessel1841";
    ///  5 = "Clarke1866";
    ///  6 = "Clarke1880";
    ///  7 = "EverestIndia1830";
    ///  8 = "EverestSabahSarawak";
    ///  9 = "EverestIndia1956";
    ///  10 = "EverestMalaysia1969";
    ///  11 = "EverestMalay_Sing";
    ///  12 = "EverestPakistan";
    ///  13 = "Fischer1960Modified";
    ///  14 = "Helmert1906";
    ///  15 = "Hough1960";
    ///  16 = "Indonesian1974";
    ///  17 = "International1924";
    ///  18 = "Krassovsky1940";
    ///  19 = "GRS80";
    ///  20 = "SouthAmerican1969";
    ///  21 = "WGS72";
    ///  22 = "WGS84";
    /// @param Lat the latitude in decimal degrees
    /// @param Long the longitude in decimal degrees
    /// @param Northing in metres. This argument is replaced by the function
    /// @param Easting in metres. This argument is replaced by the function
    /// @param Zone the UTM zone. This argument is replaced by the function
    /// @author SMM, modified from Chuck Gantz
    /// @date 07/12/2014
    void LLtoUTM(int eId, double Lat, double Long,  
             double& Northing, double& Easting, int& Zone);

    /// @brief converts LatLong to UTM coords, but forces the data to a specific zone
    /// 3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
    /// Updated by Simon M Mudd
    /// @param eID the ellipsoid ID. Options are:
    ///  0 = "Airy1830";
    ///  1 = "AiryModified";
    ///  2 = "AustralianNational";
    ///  3 = "Bessel1841Namibia";
    ///  4 = "Bessel1841";
    ///  5 = "Clarke1866";
    ///  6 = "Clarke1880";
    ///  7 = "EverestIndia1830";
    ///  8 = "EverestSabahSarawak";
    ///  9 = "EverestIndia1956";
    ///  10 = "EverestMalaysia1969";
    ///  11 = "EverestMalay_Sing";
    ///  12 = "EverestPakistan";
    ///  13 = "Fischer1960Modified";
    ///  14 = "Helmert1906";
    ///  15 = "Hough1960";
    ///  16 = "Indonesian1974";
    ///  17 = "International1924";
    ///  18 = "Krassovsky1940";
    ///  19 = "GRS80";
    ///  20 = "SouthAmerican1969";
    ///  21 = "WGS72";
    ///  22 = "WGS84";
    /// @param Lat the latitude in decimal degrees
    /// @param Long the longitude in decimal degrees
    /// @param Northing in metres. This argument is replaced by the function
    /// @param Easting in metres. This argument is replaced by the function
    /// @param Zone the UTM zone. This argument is replaced by the function
    /// @author SMM, modified from Chuck Gantz
    /// @date 11/04/2016
    void LLtoUTM_ForceZone(int eId, double Lat, double Long,  
             double& Northing, double& Easting, int Zone);

  
    /// @brief converts LatLong to UTM coords
    /// 3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
    /// @param eID the ellipsoid ID. Options are:
    ///  0 = "Airy1830";
    ///  1 = "AiryModified";
    ///  2 = "AustralianNational";
    ///  3 = "Bessel1841Namibia";
    ///  4 = "Bessel1841";
    ///  5 = "Clarke1866";
    ///  6 = "Clarke1880";
    ///  7 = "EverestIndia1830";
    ///  8 = "EverestSabahSarawak";
    ///  9 = "EverestIndia1956";
    ///  10 = "EverestMalaysia1969";
    ///  11 = "EverestMalay_Sing";
    ///  12 = "EverestPakistan";
    ///  13 = "Fischer1960Modified";
    ///  14 = "Helmert1906";
    ///  15 = "Hough1960";
    ///  16 = "Indonesian1974";
    ///  17 = "International1924";
    ///  18 = "Krassovsky1940";
    ///  19 = "GRS80";
    ///  20 = "SouthAmerican1969";
    ///  21 = "WGS72";
    ///  22 = "WGS84";
    /// @param Northing in metres. 
    /// @param Easting in metres. 
    /// @param Zone the UTM zone. 
    /// @param isNorth is a boolean that states if the map is in the northern hemisphere
    /// @param Lat the latitude in decimal degrees. 
    ///  This argument is replaced by the function
    /// @param Long the longitude in decimal degrees
    ///  This argument is replaced by the function
    /// @author SMM, modified from Chuck Gantz
    /// @date 07/12/2014
    void UTMtoLL(int eId, double Northing, double Easting, int Zone, bool isNorth,  
             double& Lat, double& Long);



    /// @brief converts LatLongHt in datum dIn, to LatLongHt in datum dTo;  
    /// @detail 2002dec: by Eugene Reimer, from PeterDana equations.
    ///   Lat and Long params are in degrees;  
    /// North latitudes and East longitudes are positive;  Height is in meters;
    /// ==This approach to Datum-conversion is a waste of time;  
    /// to get acceptable accuracy a large table is needed -- see NADCON, NTv2...
    void DatumConvert(int dIn, double LatIn, double LongIn, double HtIn, 
                  int dTo,  double& LatTo, double& LongTo, double& HtTo);
  
  protected:
  
    /// @brief a vector holding the ellipsoids
    vector<LSDEllipsoid> Ellipsoids;
    
    /// @brief a vectro holding the datums
    vector<LSDDatum> Datums;

    double RADIANS_PER_DEGREE;
    double DEGREES_PER_RADIAN;

    /** Useful constants **/
    double TWOPI;
    double HALFPI;

    // Grid granularity for rounding UTM coordinates to generate MapXY.
    double grid_size;    // 100 km grid

    // WGS84 Parameters
    double WGS84_A;                     // major axis
    double WGS84_B;                     // minor axis
    double WGS84_F;                     // ellipsoid flattening
    double WGS84_E;                     // first eccentricity
    double WGS84_EP;                    // second eccentricity

    // UTM Parameters
    double UTM_K0;              // scale factor
    double UTM_FE;              // false easting
    double UTM_FN_N;            // false northing, northern hemisphere
    double UTM_FN_S;            // false northing, southern hemisphere
    double UTM_E2;              // e^2
    double UTM_E4;              // e^4
    double UTM_E6;              // e^6
    double UTM_EP2;             // e'^2

  private:
  
    /// @brief This create function sets up the data membeers that hold the
    ///  ellipsoid and datum data
    void create();

};

#endif
