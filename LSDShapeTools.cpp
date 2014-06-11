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
#include "LSDShapeTools.hpp"
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
  	
	BYTE *ByteData;			// Pointer to our buffered data
	FILE *file = NULL;		// File pointer

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
  	
	BYTE *ByteData;			// Pointer to our buffered data
	FILE *file = NULL;		// File pointer

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





#endif
