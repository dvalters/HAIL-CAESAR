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


#include <cstdlib> 
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <cmath>
#include "LSDShapeTools.hpp"
#include "LSDStatsTools.hpp"
using namespace std;

#ifndef ShapeTools_CPP
#define ShapeTools_CPP


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to test the Byte order of the system.
// Returns a boolean value where true is little endian.
//
// SWDG 11/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool SystemEndiannessTest(){
  
  int TestInt = 1;  //this is stored as 4 bytes in memory
  int ReconstructedTestInt;
  int ReconstructedTestIntSwapped;
  
  char * TestBytes = (char *) &TestInt;  //convert each byte of the int into a char
  
  memcpy(&ReconstructedTestInt, TestBytes, 4);

  BYTE temp = TestBytes[0];
  TestBytes[0] = TestBytes[3]; 
  TestBytes[3] = temp;
    
  temp = TestBytes[1];
  TestBytes[1] = TestBytes[2]; 
  TestBytes[2] = temp;

  memcpy(&ReconstructedTestIntSwapped, TestBytes, 4);  
  
  if (ReconstructedTestInt == 1){
    //cout << "Little Endian" << endl;
    return true;
  }
  else if (ReconstructedTestIntSwapped == 1){
    //cout << "Big Endian" << endl;
    return false;
  }
  else{
    cout << "Unable to determine endianness of system." << endl;
    exit(EXIT_FAILURE);
  }
 
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to swap the byte order of a word in memory. Used if the system's byte order
// does not match the data in the shapefile.
//
// SWDG 11/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ByteSwap(int length, void * ByteData){
    
    BYTE temp;

    for (int i = 0; i< length/2; ++i){

      temp = ((BYTE *) ByteData)[i];
      ((BYTE *) ByteData)[i] = ((BYTE *) ByteData)[length-i-1]; 
      ((BYTE *) ByteData)[length-i-1] = temp;
        
    }
    
}    
 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to get the size of the binary file being loaded.
//
// Taken from http://www.dreamincode.net/forums/topic/170054-understanding-and-reading-binary-files-in-c/ 
//
// SWDG 10/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long getFileSize(FILE *file){
  long lCurPos, lEndPos;
  lCurPos = ftell(file);
  fseek(file, 0, 2);
  lEndPos = ftell(file);
  fseek(file, lCurPos, 0);
  return lEndPos;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to load an ESRI ShapeFile.
// 
// Only works for X,Y point shapefiles at present and it's behaviour is totally undefined 
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
PointData LoadShapefile(string Filename){
    
  BYTE *ByteData;      // Pointer to our buffered data
  FILE *file = NULL;    // File pointer

  // Open the file in binary mode using the "rb" format string
  // This also checks if the file exists and/or can be opened for reading correctly
  if ((file = fopen(Filename.c_str(), "rb")) == NULL){
    cout << "Could not open specified file" << endl;
    exit(EXIT_FAILURE);
  }    
  else{
    cout << "File opened successfully" << endl;  
  }
  
  // Get the size of the file in bytes
  long fileSize = getFileSize(file);

  // Allocate space in the buffer for the whole file
  ByteData = new BYTE[fileSize];

  // Read the file in to the buffer
  fread(ByteData, fileSize, 1, file);

  //Declare variables used in the method
  int FileLength;
  int ShapeType;
  double Xmin;
  double Ymin;
  double Xmax;
  double Ymax;
  double Zmin;
  double Zmax;
  double Mmin;
  double Mmax;
  int RecordLength;
  int NoOfRecords;
  PointData Points;
  double TempX;
  double TempY;

  bool LittleEndian = SystemEndiannessTest(); // Get byteorder of the system
  
  // If system byte order is Little Endian
  if (LittleEndian == true){
  
    // Get the length of the file
    ByteSwap(4, ByteData+24);
    memcpy(&FileLength, ByteData+24, 4);

    // Get type of shape in file (not currently used) see         
    memcpy(&ShapeType, ByteData+32, 4);
    
    // Get Georeferencing data (not currently used)
    memcpy(&Xmin, ByteData+36, 8);
    memcpy(&Ymin, ByteData+44, 8);
    memcpy(&Xmax, ByteData+52, 8);
    memcpy(&Ymax, ByteData+60, 8);
    memcpy(&Zmin, ByteData+68, 8);
    memcpy(&Zmax, ByteData+76, 8);
    memcpy(&Mmin, ByteData+84, 8);
    memcpy(&Mmax, ByteData+92, 8);
        
    // Get the length of the first record (All records are the same length for points)    
    ByteSwap(4, ByteData+104);
    memcpy(&RecordLength, ByteData+104, 4);
    
    //Calculate the number of records in the file
    NoOfRecords = (FileLength-50)/(RecordLength+4);  // FileLength - 50(the length in words of the header)                                            
                                                     // divided by RecordLength+4 (4 is the length in words of the record header)
    
    if (NoOfRecords == 0){
      cout << "Empty Shapefile. No Data to read!\n" << endl;
      exit(EXIT_FAILURE);
    }
    
    //Read all of the records into the Points structure
    for (int q = 0; q < NoOfRecords; ++q){
    
      memcpy(&TempX, ByteData+112+(q * ((RecordLength+4)*2)), 8); // RecordLength+4*2 == 28 (The total length of a record)
      memcpy(&TempY, ByteData+120+(q * ((RecordLength+4)*2)), 8); 
    
      Points.X.push_back(TempX);
      Points.Y.push_back(TempY);
    
    } 
    
  }
  // If system byte order is Big Endian
  else{
  
    // Get the length of the file
    memcpy(&FileLength, ByteData+24, 4);

    // Get type of shape in file (not currently used) see         
    ByteSwap(4, ByteData+32);
    memcpy(&ShapeType, ByteData+32, 4);
  
    // Get Georeferencing data (not currently used)
    ByteSwap(8, ByteData+36);
    memcpy(&Xmin, ByteData+36, 8);
    ByteSwap(8, ByteData+44);
    memcpy(&Ymin, ByteData+44, 8);
    ByteSwap(8, ByteData+52);
    memcpy(&Xmax, ByteData+52, 8);
    ByteSwap(8, ByteData+60);
    memcpy(&Ymax, ByteData+60, 8);
    ByteSwap(8, ByteData+68);
    memcpy(&Zmin, ByteData+68, 8);
    ByteSwap(8, ByteData+76);
    memcpy(&Zmax, ByteData+76, 8);
    ByteSwap(8, ByteData+84);
    memcpy(&Mmin, ByteData+84, 8);
    ByteSwap(8, ByteData+92);
    memcpy(&Mmax, ByteData+92, 8);
        
    // Get the length of the first record (All records are the same length for points)    
    memcpy(&RecordLength, ByteData+104, 4);
    
    //Calculate the number of records in the file
    NoOfRecords = (FileLength-50)/(RecordLength+4);  // FileLength - 50(the length in words of the header)                                            
                                                     // divided by RecordLength+4 (4 is the length in words of the record header)
    
    if (NoOfRecords == 0){
      cout << "Empty Shapefile. No Data to read!\n" << endl;
      exit(EXIT_FAILURE);
    }
    
    //Read all of the records into the Points structure
    for (int q = 0; q < NoOfRecords; ++q){
    
      ByteSwap(8, ByteData+112+(q * ((RecordLength+4)*2)));
      memcpy(&TempX, ByteData+112+(q * ((RecordLength+4)*2)), 8); // RecordLength+4*2 == 28 (The total length of a record)
      ByteSwap(8, ByteData+120+(q * ((RecordLength+4)*2)));
      memcpy(&TempY, ByteData+120+(q * ((RecordLength+4)*2)), 8); 
    
      Points.X.push_back(TempX);
      Points.Y.push_back(TempY);
    
    }   
    
  }

  // Close the file and return the point data
  fclose(file);  
  return Points;
  
}

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
vector<PointData> LoadPolyline(string Filename){
    
  BYTE *ByteData;      // Pointer to our buffered data
  FILE *file = NULL;    // File pointer

  // Open the file in binary mode using the "rb" format string
  // This also checks if the file exists and/or can be opened for reading correctly
  if ((file = fopen(Filename.c_str(), "rb")) == NULL){
    cout << "Could not open specified file" << endl;
    exit(EXIT_FAILURE);
  }    
  else{
    cout << "File opened successfully" << endl;  
  }
  
  // Get the size of the file in bytes
  long fileSize = getFileSize(file);

  // Allocate space in the buffer for the whole file
  ByteData = new BYTE[fileSize];

  // Read the file in to the buffer
  fread(ByteData, fileSize, 1, file);

  //Declare variables used in the method
  int FileLength;
  int ShapeType;
  double Xmin;
  double Ymin;
  double Xmax;
  double Ymax;
  double Zmin;
  double Zmax;
  double Mmin;
  double Mmax;
  int RecordLength;
  PointData Points;
  double TempX;
  double TempY;
  vector<PointData> Polylines;
  int Skip;
  int shapetype;                         
  int numparts; 
  int numpoints;

  bool LittleEndian = SystemEndiannessTest(); // Get byteorder of the system
  
  // If system byte order is Little Endian
  if (LittleEndian == true){
  
    // Get the length of the file
    ByteSwap(4, ByteData+24);
    memcpy(&FileLength, ByteData+24, 4);

    if (FileLength == 50){
      cout << "Empty Shapefile. No Data to read!\n" << endl;
      exit(EXIT_FAILURE);
    }

    // Get type of shape in file (not currently used) see         
    memcpy(&ShapeType, ByteData+32, 4);
  
    // Get Georeferencing data (not currently used)
    memcpy(&Xmin, ByteData+36, 8);
    memcpy(&Ymin, ByteData+44, 8);
    memcpy(&Xmax, ByteData+52, 8);
    memcpy(&Ymax, ByteData+60, 8);
    memcpy(&Zmin, ByteData+68, 8);
    memcpy(&Zmax, ByteData+76, 8);
    memcpy(&Mmin, ByteData+84, 8);
    memcpy(&Mmax, ByteData+92, 8);
        
    // Get the length of the first record 
    ByteSwap(4, ByteData+104);
    memcpy(&RecordLength, ByteData+104, 4);                                                       
    Skip = RecordLength*2 + 8 + 100;       
    
    memcpy(&shapetype, ByteData+108, 4);
    memcpy(&numparts, ByteData+144, 4);
    memcpy(&numpoints, ByteData+148, 4);  // number of XY pairs in the first polyline
        
    for (int q = 0; q < numpoints; ++q){
    
      memcpy(&TempX, ByteData+156+(q*16), 8);
      memcpy(&TempY, ByteData+164+(q*16), 8);
      
      Points.X.push_back(TempX);
      Points.Y.push_back(TempY);
        
    }
    
    Polylines.push_back(Points);
                   
    while (Skip < (FileLength*2)){
    
      ByteSwap(4, ByteData+Skip+4);
      memcpy(&RecordLength, ByteData+Skip+4, 4);         
      memcpy(&shapetype, ByteData+Skip+8, 4);                         
      memcpy(&numparts, ByteData+Skip+44, 4);
      memcpy(&numpoints, ByteData+Skip+48, 4);  // number of XY pairs in the polyline
    
      PointData Points;
      for (int w = 0; w < numpoints; ++w){
    
        memcpy(&TempX, ByteData+Skip+56+(w*16), 8);
        memcpy(&TempY, ByteData+Skip+64+(w*16), 8);
      
        Points.X.push_back(TempX);
        Points.Y.push_back(TempY);
        
      }
    
      Polylines.push_back(Points);
      Skip += (RecordLength*2) +8;
      
    }
      
  }
  // If system byte order is Big Endian
  else{
  
    // Get the length of the file
    memcpy(&FileLength, ByteData+24, 4);

    if (FileLength == 50){
      cout << "Empty Shapefile. No Data to read!\n" << endl;
      exit(EXIT_FAILURE);
    }

    // Get type of shape in file (not currently used) see
    ByteSwap(4, ByteData+32);         
    memcpy(&ShapeType, ByteData+32, 4);
  
    // Get Georeferencing data (not currently used)
    
    ByteSwap(8, ByteData+36);
    memcpy(&Xmin, ByteData+36, 8);
    ByteSwap(8, ByteData+44);
    memcpy(&Ymin, ByteData+44, 8);
    ByteSwap(8, ByteData+52);
    memcpy(&Xmax, ByteData+52, 8);
    ByteSwap(8, ByteData+60);
    memcpy(&Ymax, ByteData+60, 8);
    ByteSwap(8, ByteData+68);
    memcpy(&Zmin, ByteData+68, 8);
    ByteSwap(8, ByteData+76);
    memcpy(&Zmax, ByteData+76, 8);
    ByteSwap(8, ByteData+84);
    memcpy(&Mmin, ByteData+84, 8);
    ByteSwap(8, ByteData+92);
    memcpy(&Mmax, ByteData+92, 8);
        
    // Get the length of the first record     
    
    memcpy(&RecordLength, ByteData+104, 4);                                                           
    Skip = RecordLength*2 + 8 + 100;   
    
    ByteSwap(8, ByteData+108);
    memcpy(&shapetype, ByteData+108, 4);
    ByteSwap(8, ByteData+144); 
    memcpy(&numparts, ByteData+144, 4);
    ByteSwap(8, ByteData+148);
    memcpy(&numpoints, ByteData+148, 4);  // number of XY pairs in the first polyline
        
    for (int q = 0; q < numpoints; ++q){
    
      ByteSwap(8, ByteData+156+(q*16));
      memcpy(&TempX, ByteData+156+(q*16), 8);
      ByteSwap(8, ByteData+164+(q*16));
      memcpy(&TempY, ByteData+164+(q*16), 8);
      
      cout << TempX << " " << TempY << endl; 
      Points.X.push_back(TempX);
      Points.Y.push_back(TempY);
        
    }
    
    Polylines.push_back(Points);
         
    cout << "---------" << endl;
          
    while (Skip < (FileLength*2)){
    
      ByteSwap(8, ByteData+Skip+4);
      memcpy(&RecordLength, ByteData+Skip+4, 4);   
      ByteSwap(8, ByteData+Skip+8);      
      memcpy(&shapetype, ByteData+Skip+8, 4);
      ByteSwap(8, ByteData+Skip+44);                    
      memcpy(&numparts, ByteData+Skip+44, 4);
      ByteSwap(8, ByteData+Skip+48);
      memcpy(&numpoints, ByteData+Skip+48, 4);  // number of XY pairs in the polyline
    
      PointData Points;
      for (int w = 0; w < numpoints; ++w){
    
        ByteSwap(8, ByteData+Skip+56+(w*16));
        memcpy(&TempX, ByteData+Skip+56+(w*16), 8);
        ByteSwap(8, ByteData+Skip+64+(w*16));
        memcpy(&TempY, ByteData+Skip+64+(w*16), 8);
      
        cout << TempX << " " << TempY << endl; 
        Points.X.push_back(TempX);
        Points.Y.push_back(TempY);
        
      }
    
      Polylines.push_back(Points);
      Skip += (RecordLength*2) +8;  
     
      cout << "------" << endl;      
      
    }  
  
  }

  // Close the file and return the point data
  fclose(file);      
  return Polylines;
  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to convert an IndexChannelTree to a PointData object.
// 
// Returns a vector of points.
// DTM 11/07/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
PointData LoadChannelTree(string Filename, int multistem_option, int trib_number)
{
  if((multistem_option != 0) && (multistem_option != 1) && (multistem_option !=2))
  {
    cout << "multistem_option must be 0 (mainstem only), 1 (all tributaries) or 2 (specify channel number).  Setting mainstem only default" << endl;
    multistem_option = 0;
  }
  
  ifstream channel_data_in;
  channel_data_in.open(Filename.c_str());

  if( channel_data_in.fail() )
  {
    cout << "\nFATAL ERROR: the channel network file \"" << Filename << "\" doesn't exist" << endl;
    exit(EXIT_FAILURE);
  }

  PointData Points;

  int channel_number;
  int receiver_cnumber;
  int recevier_cnode;

  int node;
  int row;
  int col;

  float flow_dist;
  float elev;
  float drain_area;

  int last_cn = 0;    // this is 1 if this is the first node in a channel
  int last_receiver_node = -1;
  int last_receiver_channel = -1;

  float XMinimum,YMinimum,DataResolution,NoDataValue;
  int NRows,NCols;

  channel_data_in >> NRows >> NCols >> XMinimum >> YMinimum >> DataResolution >> NoDataValue;
  float x,y;
  while( channel_data_in >> channel_number >> receiver_cnumber >> recevier_cnode
                         >> node >> row >> col >> flow_dist >> elev >> drain_area)
  {
    // get the receiver_channel and receiver node for the first channel (these will be recursive)
    if (last_receiver_node == -1)
    {
      last_receiver_node = recevier_cnode;
      last_receiver_channel = receiver_cnumber;
    }
    // now load everything into the PointData object :-)

    if(multistem_option == 0)
    {
      if(channel_number == 0)
      {
        x = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
        y = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution; 
        Points.X.push_back(x);
        Points.Y.push_back(y);           
      }
    }
    else if(multistem_option == 1)
    {
      x = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
      y = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution; 
      Points.X.push_back(x);
      Points.Y.push_back(y);   
    }
    else if(multistem_option == 2)
    {
      if(channel_number == trib_number)
      {
        x = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
        y = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution; 
        Points.X.push_back(x);
        Points.Y.push_back(y);   
      }
    }
    else
    {
      if(channel_number == 0)
      {
        x = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
        y = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution; 
        Points.X.push_back(x);
        Points.Y.push_back(y);     
      }
    } 
  }
  return Points;
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// The create function, sets up some vectors for holding 
// ellipses and datums
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCoordinateConverterLLandUTM::create()
{
  // this sets up the ellipsoid and datum vectors
  vector<LSDEllipsoid> Ellipse_data_temp;
  
  // declare the names of the ellipsoids
  char t00[] = "Airy1830";
  char t01[] = "AiryModified";
  char t02[] = "AustralianNational";
  char t03[] = "Bessel1841Namibia";
  char t04[] = "Bessel1841";
  char t05[] = "Clarke1866";
  char t06[] = "Clarke1880";
  char t07[] = "EverestIndia1830";
  char t08[] = "EverestSabahSarawak";
  char t09[] = "EverestIndia1956";
  char t10[] = "EverestMalaysia1969";
  char t11[] = "EverestMalay_Sing";
  char t12[] = "EverestPakistan";
  char t13[] = "Fischer1960Modified";
  char t14[] = "Helmert1906";
  char t15[] = "Hough1960";
  char t16[] = "Indonesian1974";
  char t17[] = "International1924";
  char t18[] = "Krassovsky1940";
  char t19[] = "GRS80";
  char t20[] = "SouthAmerican1969";
  char t21[] = "WGS72";
  char t22[] = "WGS84";
  
  // add the ellipsoids to the vector
  LSDEllipsoid E00( 0, t00,    6377563.396,  299.3249646);
  LSDEllipsoid E01( 1, t01,    6377340.189,  299.3249646);
  LSDEllipsoid E02( 2, t02,  6378160,  298.25);
  LSDEllipsoid E03( 3, t03,  6377483.865,  299.1528128);
  LSDEllipsoid E04( 4, t04,    6377397.155,  299.1528128);
  LSDEllipsoid E05( 5, t05,    6378206.4,  294.9786982);
  LSDEllipsoid E06( 6, t06,    6378249.145,  293.465);
  LSDEllipsoid E07( 7, t07,  6377276.345,  300.8017);
  LSDEllipsoid E08( 8, t08,  6377298.556,  300.8017);
  LSDEllipsoid E09( 9, t09,  6377301.243,  300.8017);
  LSDEllipsoid E10(10, t10,  6377295.664,  300.8017);  //Dana has no datum that uses this LSDEllipsoid E00!
  LSDEllipsoid E11(11, t11,  6377304.063,  300.8017);
  LSDEllipsoid E12(12, t12,  6377309.613,  300.8017);
  LSDEllipsoid E13(13, t13,  6378155,  298.3);
  LSDEllipsoid E14(14, t14,    6378200,  298.3);
  LSDEllipsoid E15(15, t15,    6378270,  297);
  LSDEllipsoid E16(16, t16,    6378160,  298.247);
  LSDEllipsoid E17(17, t17,  6378388,  297);
  LSDEllipsoid E18(18, t18,    6378245,  298.3);
  LSDEllipsoid E19(19, t19,      6378137,  298.257222101);
  LSDEllipsoid E20(20, t20,  6378160,  298.25);
  LSDEllipsoid E21(21, t21,      6378135,  298.26);
  LSDEllipsoid E22(22, t22,      6378137,  298.257223563);
  
  Ellipse_data_temp.push_back(E00);
  Ellipse_data_temp.push_back(E01);
  Ellipse_data_temp.push_back(E02);
  Ellipse_data_temp.push_back(E03);
  Ellipse_data_temp.push_back(E04);
  Ellipse_data_temp.push_back(E05);
  Ellipse_data_temp.push_back(E06);
  Ellipse_data_temp.push_back(E07);
  Ellipse_data_temp.push_back(E08);
  Ellipse_data_temp.push_back(E09);
  Ellipse_data_temp.push_back(E10);
  Ellipse_data_temp.push_back(E11);
  Ellipse_data_temp.push_back(E12);
  Ellipse_data_temp.push_back(E13);
  Ellipse_data_temp.push_back(E14);
  Ellipse_data_temp.push_back(E15);
  Ellipse_data_temp.push_back(E16);
  Ellipse_data_temp.push_back(E17);
  Ellipse_data_temp.push_back(E18);
  Ellipse_data_temp.push_back(E19);
  Ellipse_data_temp.push_back(E20);
  Ellipse_data_temp.push_back(E21);
  Ellipse_data_temp.push_back(E22);

  //names for ellipsoidId's
  int eClarke1866 = 5;
  int eGRS80 = 19;
  int eWGS72 = 21;
  int eWGS84 = 22;


  // now for the datum
  vector<LSDDatum> Datum_data_temp;
  
  // initiate the datum names
  char T00[] = "NAD27_AK";
  char T01[] = "NAD27_AK_AleutiansE";
  char T02[] = "NAD27_AK_AleutiansW";
  char T03[] = "NAD27_Bahamas";
  char T04[] = "NAD27_Bahamas_SanSalv";
  char T05[] = "NAD27_AB_BC";
  char T06[] = "NAD27_MB_ON";
  char T07[] = "NAD27_NB_NL_NS_QC";
  char T08[] = "NAD27_NT_SK";
  char T09[] = "NAD27_YT";
  char T10[] = "NAD27_CanalZone";
  char T11[] = "NAD27_Cuba";
  char T12[] = "NAD27_Greenland";
  char T13[] = "NAD27_Carribean";
  char T14[] = "NAD27_CtrlAmerica";
  char T15[] = "NAD27_Canada";
  char T16[] = "NAD27_ConUS";
  char T17[] = "NAD27_ConUS_East";
  char T18[] = "NAD27_ConUS_West";
  char T19[] = "NAD27_Mexico";
  char T20[] = "NAD83_AK";
  char T21[] = "NAD83_AK_Aleutians";
  char T22[] = "NAD83_Canada";  
  char T23[] = "NAD83_ConUS";
  char T24[] = "NAD83_Hawaii";
  char T25[] = "NAD83_Mexico_CtrlAmerica";
  char T26[] = "WGS72";
  char T27[] = "WGS84";

  LSDDatum D00(0, T00,      eClarke1866,  -5,  135,  172); //NAD27 for Alaska Excluding Aleutians
  LSDDatum D01( 1, T01,  eClarke1866,  -2,  152,  149); //NAD27 for Aleutians East of 180W
  LSDDatum D02( 2, T02,  eClarke1866,  2,  204,  105); //NAD27 for Aleutians West of 180W
  LSDDatum D03( 3, T03,    eClarke1866,  -4,  154,  178); //NAD27 for Bahamas Except SanSalvadorIsland
  LSDDatum D04( 4, T04,  eClarke1866,  1,  140,  165); //NAD27 for Bahamas SanSalvadorIsland
  LSDDatum D05( 5, T05,    eClarke1866,  -7,  162,  188); //NAD27 for Canada Alberta BritishColumbia
  LSDDatum D06( 6, T06,    eClarke1866,  -9,  157,  184); //NAD27 for Canada Manitoba Ontario
  LSDDatum D07( 7, T07,    eClarke1866,  -22,  160,  190); //NAD27 for Canada NewBrunswick Newfoundland NovaScotia Quebec
  LSDDatum D08( 8, T08,    eClarke1866,  4,  159,  188); //NAD27 for Canada NorthwestTerritories Saskatchewan
  LSDDatum D09( 9, T09,      eClarke1866,  -7,  139,  181); //NAD27 for Canada Yukon
  LSDDatum D10(10, T10,    eClarke1866,  0,  125,  201); //NAD27 for CanalZone (ER: is that Panama??)
  LSDDatum D11(11, T11,      eClarke1866,  -9,  152,  178); //NAD27 for Cuba
  LSDDatum D12(12, T12,    eClarke1866,  11,  114,  195); //NAD27 for Greenland (HayesPeninsula)
  LSDDatum D13(13, T13,    eClarke1866,  -3,  142,  183); //NAD27 for Antigua Barbados Barbuda Caicos Cuba DominicanRep GrandCayman Jamaica Turks
  LSDDatum D14(14, T14,    eClarke1866,  0,  125,  194); //NAD27 for Belize CostaRica ElSalvador Guatemala Honduras Nicaragua
  LSDDatum D15(15, T15,    eClarke1866,  -10,  158,  187); //NAD27 for Canada
  LSDDatum D16(16, T16,    eClarke1866,  -8,  160,  176); //NAD27 for CONUS
  LSDDatum D17(17, T17,    eClarke1866,  -9,  161,  179); //NAD27 for CONUS East of Mississippi Including Louisiana Missouri Minnesota
  LSDDatum D18(18, T18,    eClarke1866,  -8,  159,  175); //NAD27 for CONUS West of Mississippi Excluding Louisiana Missouri Minnesota
  LSDDatum D19(19, T19,    eClarke1866,  -12,  130,  190); //NAD27 for Mexico
  LSDDatum D20(20, T20,      eGRS80,    0,  0,  0); //NAD83 for Alaska Excluding Aleutians
  LSDDatum D21(21, T21,    eGRS80,    -2,  0,  4); //NAD83 for Aleutians
  LSDDatum D22(22, T22,    eGRS80,    0,  0,  0); //NAD83 for Canada
  LSDDatum D23(23, T23,    eGRS80,    0,  0,  0); //NAD83 for CONUS
  LSDDatum D24(24, T24,    eGRS80,    1,  1,  -1); //NAD83 for Hawaii
  LSDDatum D25(25, T25,  eGRS80,    0,  0,  0); //NAD83 for Mexico CentralAmerica
  LSDDatum D26(26, T26,      eWGS72,    0,  0,  0); //WGS72 for world
  LSDDatum D27(27, T27,      eWGS84,    0,  0,  0); //WGS84 for world
  
  Datum_data_temp.push_back(D00);
  Datum_data_temp.push_back(D01);
  Datum_data_temp.push_back(D02);
  Datum_data_temp.push_back(D03);
  Datum_data_temp.push_back(D04);
  Datum_data_temp.push_back(D05);
  Datum_data_temp.push_back(D06);
  Datum_data_temp.push_back(D07);
  Datum_data_temp.push_back(D08);
  Datum_data_temp.push_back(D09);
  Datum_data_temp.push_back(D10);
  Datum_data_temp.push_back(D11);
  Datum_data_temp.push_back(D12);
  Datum_data_temp.push_back(D13);
  Datum_data_temp.push_back(D14);
  Datum_data_temp.push_back(D15);
  Datum_data_temp.push_back(D16);
  Datum_data_temp.push_back(D17);
  Datum_data_temp.push_back(D18);
  Datum_data_temp.push_back(D19);
  Datum_data_temp.push_back(D20);
  Datum_data_temp.push_back(D21);
  Datum_data_temp.push_back(D22);
  Datum_data_temp.push_back(D23);
  Datum_data_temp.push_back(D24);
  Datum_data_temp.push_back(D25);
  Datum_data_temp.push_back(D26);
  Datum_data_temp.push_back(D27);

  Ellipsoids = Ellipse_data_temp;
  Datums = Datum_data_temp;
  
  RADIANS_PER_DEGREE = M_PI/180.0;
  DEGREES_PER_RADIAN = 180.0/M_PI;

  /** Useful constants **/
  TWOPI = 2.0 * M_PI;
  HALFPI = M_PI / 2.0;

  // Grid granularity for rounding UTM coordinates to generate MapXY.
  grid_size = 100000.0;    // 100 km grid

  // WGS84 Parameters
  WGS84_A=6378137.0;    // major axis
  WGS84_B=6356752.31424518;  // minor axis
  WGS84_F=0.0033528107;    // ellipsoid flattening
  WGS84_E=0.0818191908;    // first eccentricity
  WGS84_EP=0.0820944379;    // second eccentricity

  // UTM Parameters
  UTM_K0=0.9996;      // scale factor
  UTM_FE=500000.0;    // false easting
  UTM_FN_N=0.0;           // false northing, northern hemisphere
  UTM_FN_S=10000000.0;    // false northing, southern hemisphere
  UTM_E2=(WGS84_E*WGS84_E);  // e^2
  UTM_E4=(UTM_E2*UTM_E2);    // e^4
  UTM_E6=(UTM_E4*UTM_E2);    // e^6
  UTM_EP2=(UTM_E2/(1-UTM_E2));  // e'^2
  
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// converts LatLong to UTM coords
// 3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
// Lat and Long are in degrees;  
// North latitudes and East Longitudes are positive.
//
// Minor modifications for our objects by SMM, 07/12/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCoordinateConverterLLandUTM::LLtoUTM(int eId, double Lat, double Long,  
             double& Northing, double& Easting, int& Zone)
{

  double UTMEasting, UTMNorthing;
  
  double a = WGS84_A;
  double eccSquared = UTM_E2;
  double k0 = UTM_K0;

  double LongOrigin;
  double eccPrimeSquared;
  double N, T, C, A, M;
  
        //Make sure the longitude is between -180.00 .. 179.9
  double LongTemp = (Long+180)-int((Long+180)/360)*360-180;

  double LatRad = Lat*RADIANS_PER_DEGREE;
  double LongRad = LongTemp*RADIANS_PER_DEGREE;
  double LongOriginRad;
  int    ZoneNumber;

  ZoneNumber = int((LongTemp + 180)/6) + 1;
  
  if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
  {
    ZoneNumber = 32;
  }
        // Special zones for Svalbard
  if( Lat >= 72.0 && Lat < 84.0 ) 
  {
    if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
    else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
    else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
    else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
   }
        // +3 puts origin in middle of zone
  LongOrigin = (ZoneNumber - 1)*6 - 180 + 3; 
  LongOriginRad = LongOrigin * RADIANS_PER_DEGREE;

  //compute the UTM Zone from the latitude and longitude
  cout << "Zone is  " << ZoneNumber << endl;
  Zone = ZoneNumber;
  
  eccPrimeSquared = (eccSquared)/(1-eccSquared);

  N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
  T = tan(LatRad)*tan(LatRad);
  C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
  A = cos(LatRad)*(LongRad-LongOriginRad);

  M = a*((1 - eccSquared/4 - 3*eccSquared*eccSquared/64
                - 5*eccSquared*eccSquared*eccSquared/256) * LatRad 
               - (3*eccSquared/8 + 3*eccSquared*eccSquared/32
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
               + (15*eccSquared*eccSquared/256
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad) 
               - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

  UTMEasting = (double)
          (k0*N*(A+(1-T+C)*A*A*A/6
                 + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
           + 500000.0);

  UTMNorthing = (double)
          (k0*(M+N*tan(LatRad)
               *(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

  if(Lat < 0)
  {
    //10000000 meter offset for southern hemisphere
    UTMNorthing += 10000000.0;
  }

  Northing = UTMNorthing;
  Easting = UTMEasting;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// converts LatLong to UTM coords
// 3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
// Lat and Long are in degrees;  
// North latitudes and East Longitudes are positive.
//
// Minor modifications for our objects by SMM, 07/12/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCoordinateConverterLLandUTM::LLtoUTM_ForceZone(int eId, double Lat, double Long,  
             double& Northing, double& Easting, int Zone)
{

  double UTMEasting, UTMNorthing;
  
  double a = WGS84_A;
  double eccSquared = UTM_E2;
  double k0 = UTM_K0;

  double LongOrigin;
  double eccPrimeSquared;
  double N, T, C, A, M;
  
        //Make sure the longitude is between -180.00 .. 179.9
  double LongTemp = (Long+180)-int((Long+180)/360)*360-180;

  double LatRad = Lat*RADIANS_PER_DEGREE;
  double LongRad = LongTemp*RADIANS_PER_DEGREE;
  double LongOriginRad;
  int ZoneNumber;

  ZoneNumber = int((LongTemp + 180)/6) + 1;
  
  if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
  {
    ZoneNumber = 32;
  }
        // Special zones for Svalbard
  if( Lat >= 72.0 && Lat < 84.0 ) 
  {
    if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
    else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
    else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
    else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
  }
   
  if (ZoneNumber != Zone)
  {
    cout << "WARNING: the point is located in zone " << ZoneNumber << endl
         << "but you are forcing the points into Zone " << Zone << endl;
   } 
  
  // +3 puts origin in middle of zone
  LongOrigin = (Zone - 1)*6 - 180 + 3; 
  LongOriginRad = LongOrigin * RADIANS_PER_DEGREE;
  
  eccPrimeSquared = (eccSquared)/(1-eccSquared);

  N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
  T = tan(LatRad)*tan(LatRad);
  C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
  A = cos(LatRad)*(LongRad-LongOriginRad);

  M = a*((1 - eccSquared/4 - 3*eccSquared*eccSquared/64
                - 5*eccSquared*eccSquared*eccSquared/256) * LatRad 
               - (3*eccSquared/8 + 3*eccSquared*eccSquared/32
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
               + (15*eccSquared*eccSquared/256
                  + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad) 
               - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

  UTMEasting = (double)
          (k0*N*(A+(1-T+C)*A*A*A/6
                 + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
           + 500000.0);

  UTMNorthing = (double)
          (k0*(M+N*tan(LatRad)
               *(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));

  if(Lat < 0)
  {
    //10000000 meter offset for southern hemisphere
    UTMNorthing += 10000000.0;
  }

  Northing = UTMNorthing;
  Easting = UTMEasting;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// converts UTM coords to LatLong;  3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
// Lat and Long are in degrees;  North latitudes and East Longitudes are positive.
//
// Minor modifications for our objects by SMM, 07/12/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCoordinateConverterLLandUTM::UTMtoLL(int eId, double UTMNorthing, double UTMEasting, 
                                             int UTMZone, bool isNorth, 
                                             double& Lat, double& Long)
{

  double k0 = UTM_K0;
  double a = WGS84_A;
  double eccSquared = UTM_E2;
  double eccPrimeSquared;
  double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
  double N1, T1, C1, R1, D, M;
  double LongOrigin;
  double mu, phi1Rad;
  double x, y;
  int ZoneNumber;

  x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
  y = UTMNorthing;

  ZoneNumber = UTMZone;
  if(not isNorth)
  {
    //cout << "Line 1010, you are in the Southern hemisphere!"<< endl;
    //remove 10,000,000 meter offset used for southern hemisphere
    y -= 10000000.0;
  }

        //+3 puts origin in middle of zone
  LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;
  eccPrimeSquared = (eccSquared)/(1-eccSquared);

  M = y / k0;
  mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64
                   -5*eccSquared*eccSquared*eccSquared/256));

  phi1Rad = mu + ((3*e1/2-27*e1*e1*e1/32)*sin(2*mu) 
                        + (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
                        + (151*e1*e1*e1/96)*sin(6*mu));

  N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
  T1 = tan(phi1Rad)*tan(phi1Rad);
  C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
  R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
  D = x/(N1*k0);

  Lat = phi1Rad - ((N1*tan(phi1Rad)/R1)
                         *(D*D/2
                           -(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                           +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared
                             -3*C1*C1)*D*D*D*D*D*D/720));

  Lat = Lat * DEGREES_PER_RADIAN;

  Long = ((D-(1+2*T1+C1)*D*D*D/6
                 +(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
                 *D*D*D*D*D/120)
                / cos(phi1Rad));
  Long = LongOrigin + Long * DEGREES_PER_RADIAN;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// converts LatLongHt in datum dIn, to LatLongHt in datum dTo;  
// 2002dec: by Eugene Reimer, from PeterDana equations.
// Lat and Long params are in degrees;  
// North latitudes and East longitudes are positive;  Height is in meters;
// ==This approach to Datum-conversion is a waste of time;  
// to get acceptable accuracy a large table is needed -- see NADCON, NTv2...
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDCoordinateConverterLLandUTM::DatumConvert(int dIn, double LatIn, 
                  double LongIn, double HtIn, 
                  int dTo,  double& LatTo, double& LongTo, double& HtTo)
{
  double a,ee,N,X,Y,Z,EE,p,b,t;

  //--transform to XYZ, using the "In" ellipsoid
  //LongIn += 180;
  a = Ellipsoids[Datums[dIn].eId].EquatorialRadius;
  ee= Ellipsoids[Datums[dIn].eId].eccSquared;
  N = a / sqrt(1 - ee*sin(rad(LatIn))*sin(rad(LatIn)));
  X = (N + HtIn) * cos(rad(LatIn)) * cos(rad(LongIn));
  Y = (N + HtIn) * cos(rad(LatIn)) * sin(rad(LongIn));
  Z = (N*(1-ee) + HtIn) * sin(rad(LatIn));

  //--apply delta-terms dX dY dZ
  //cout<<"\tX:" <<X <<" Y:" <<Y <<" Z:" <<Z;    //==DEBUG
  X+= Datums[dIn].dX - Datums[dTo].dX;
  Y+= Datums[dIn].dY - Datums[dTo].dY;
  Z+= Datums[dIn].dZ - Datums[dTo].dZ;
  //cout<<"\tX:" <<X <<" Y:" <<Y <<" Z:" <<Z;    //==DEBUG

  //--transform back to LatLongHeight, using the "To" ellipsoid
  a = Ellipsoids[Datums[dTo].eId].EquatorialRadius;
  ee= Ellipsoids[Datums[dTo].eId].eccSquared;
  EE= ee/(1-ee);
  p = sqrt(X*X + Y*Y);
  b = a*sqrt(1-ee);
  t = atan(Z*a/(p*b));
  LatTo = atan((Z+EE*b*sin(t)*sin(t)*sin(t)) / (p-ee*a*cos(t)*cos(t)*cos(t)));
  LongTo= atan(Y/X);
  HtTo  = p/cos(LatTo) - a/sqrt(1-ee*sin(LatTo)*sin(LatTo));
  LatTo = deg(LatTo);
  LongTo = deg(LongTo);
  LongTo -= 180;
}


#endif
