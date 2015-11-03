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



#ifndef LSDIndexRaster_CPP
#define LSDIndexRaster_CPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "TNT/tnt.h"
#include "LSDIndexRaster.hpp"
#include "LSDRaster.hpp"
#include "LSDStatsTools.hpp"

using namespace std;
using namespace TNT;


// operators
// SMM 2012
LSDIndexRaster& LSDIndexRaster::operator=(const LSDIndexRaster& rhs)
 {
  
  if (&rhs != this)
   {
    create(rhs.get_NRows(),rhs.get_NCols(),rhs.get_XMinimum(),rhs.get_YMinimum(),
           rhs.get_DataResolution(), rhs.get_NoDataValue(), rhs.get_RasterData(),
           rhs.get_GeoReferencingStrings() );
   }
  return *this;
 }

// the create function. This is default and throws an error
// SMM 2012
void LSDIndexRaster::create()
{
  //cout << "You need to initialize with a filename!" << endl;
  //exit(EXIT_FAILURE);
}

// this creates a raster using an infile
// SMM 2012
void LSDIndexRaster::create(string filename, string extension)
{
  read_raster(filename,extension);
}

// this creates a raster filled with the data in data
// SMM 2012
void LSDIndexRaster::create(int nrows, int ncols, float xmin, float ymin,
            float cellsize, int ndv, Array2D<int> data)
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
    cout << "LSDIndexRaster create::dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
  if (RasterData.dim2() != NCols)
  {
    cout << "LSDIndexRaster create::dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// similar to above but contains the georeferencing
// SMM 2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::create(int nrows, int ncols, float xmin, float ymin,
            float cellsize, int ndv, Array2D<int> data, map<string,string> GRS_map)
{
  //cout << "YOYOYOYOY" << endl;
  //cout << "NR: " << NRows << " NC: " << NCols << endl;

  NRows = nrows;
  NCols = ncols;
  XMinimum = xmin;
  YMinimum = ymin;
  DataResolution = cellsize;
  NoDataValue = ndv;
  GeoReferencingStrings = GRS_map;

  RasterData = data.copy();

  if (RasterData.dim1() != NRows)
  {
    cout << "LSDIndexRaster create::dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
  if (RasterData.dim2() != NCols)
  {
    cout << "LSDIndexRaster create::dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDIndexRaster from an LSDRaster by rounding floats to ints
// Doesn't work yet
// MDH, Feb 2015
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::create(LSDRaster& NonIntLSDRaster)
{
  NRows = NonIntLSDRaster.get_NRows();
  NCols = NonIntLSDRaster.get_NCols();
  XMinimum = NonIntLSDRaster.get_XMinimum();
  YMinimum = NonIntLSDRaster.get_YMinimum();
  DataResolution = NonIntLSDRaster.get_DataResolution();
  NoDataValue = NonIntLSDRaster.get_NoDataValue();
  GeoReferencingStrings = NonIntLSDRaster.get_GeoReferencingStrings();
  Array2D<float> RasterDataFloat = NonIntLSDRaster.get_RasterData();

  //Declarations
  Array2D<int> RasterDataInt(NRows,NCols,NoDataValue);

  for (int i=0; i<NRows; ++i)
  {
    for (int j=0; j<NCols; ++j)
    {
      RasterDataInt[i][j] = int(RasterDataFloat[i][j]+0.5);
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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
// SMM 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDIndexRaster::read_raster(string filename, string extension)
{
  string string_filename;
  string dot = ".";
  string_filename = filename+dot+extension;
  cout << "The filename is " << string_filename << endl;
  int DataType;

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
  else if (extension == "bil")
  {
    // float data (a binary format created by ArcMap) has a header file
    // this file must be opened first
    string header_filename;
    string header_extension = "hdr";
    header_filename = filename+dot+header_extension;
    int NoDataExists = 0;

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
      ifs >> str;
      if (str != "ENVI")
      {
        cout << "\nFATAL ERROR: this is not an ENVI header file!, first line is: " 
             << str << endl;
        exit(EXIT_FAILURE);       
      }
      else
      {
        // the the rest of the lines
        int NChars = 5000; // need a big buffer beacause of the projection string
        char thisline[NChars];  
        vector<string> lines;
        while( ifs.getline(thisline, NChars) )
        {
          lines.push_back(thisline);
        }
        //cout << "Number of lines is: " << lines.size() << endl;
        //for(int i = 0; i< int(lines.size()); i++)
        //{
        //  cout << "Line["<<i<<"]: " << lines[i] << endl;
        //}
        
        // now loop through and get the number of rows
        int counter = 0;
        int NLines = int(lines.size());
        int this_NRows = 0;
        size_t found; 
        string str_find = "lines";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            // get the data using a stringstream
            istringstream iss(lines[counter]);
            iss >> str >> str >> str;
            this_NRows = atoi(str.c_str());
            //cout << "NRows = " << this_NRows << endl;
            NRows = this_NRows;
            
            // advance to the end so you move on to the new loop            
            counter = lines.size();            
          }
          else
          {
            counter++;
          }
        }

        // get data type
        counter = 0;
        str_find = "data type";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            // get the data using a stringstream
            istringstream iss(lines[counter]);
            iss >> str >> str >> str >> str >> str;
            DataType = atoi(str.c_str());
            cout << "Data Type = " << DataType << endl;
                     
            // advance to the end so you move on to the new loop            
            counter = lines.size();    
          }
          else
          {
            counter++;
          }
        }

        // get the number of columns
        counter = 0;
        int this_NCols = 0;
        str_find = "samples";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            // get the data using a stringstream
            istringstream iss(lines[counter]);
            iss >> str >> str >> str;
            this_NCols = atoi(str.c_str());
            //cout << "NCols = " << this_NCols << endl;
            NCols = this_NCols;
            
            // advance to the end so you move on to the new loop            
            counter = lines.size();    
          }
          else
          {
            counter++;
          }
        }        

        // get data ignore value
        counter = 0;
        float this_NoDataValue = 0;
        str_find = "data ignore value";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            // get the data using a stringstream
            istringstream iss(lines[counter]);
            iss >> str >> str >> str >> str >> str;
            this_NoDataValue = atoi(str.c_str());
            //cout << "NCols = " << this_NCols << endl;
            NoDataValue = this_NoDataValue;
            
            NoDataExists = 1;   // set this to true
            
            // advance to the end so you move on to the new loop            
            counter = lines.size();    
          }
          else
          {
            counter++;
          }
        }   

        // get the map info
        counter = 0;
        string this_map_info = "empty";
        str_find = "map info";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            //cout << "Found map info on line " << counter << '\n';  
      
            // now split the line 
            size_t start_pos;
            size_t end_pos;
            string open_curly_bracket = "{";
            string closed_curly_bracket = "}";
            start_pos = lines[counter].find(open_curly_bracket);
            end_pos = lines[counter].find(closed_curly_bracket);
            //cout << "startpos: " << start_pos << " and end pos: " << end_pos << endl;
            string info_str = lines[counter].substr(start_pos+1, end_pos-start_pos-1);
            //cout << "\nThe map info string is:\n" << info_str << endl;
            string mi_key = "ENVI_map_info";
            GeoReferencingStrings[mi_key] = info_str;

            // now parse the string
            vector<string> mapinfo_strings;
            istringstream iss(info_str);
            while( iss.good() )
            {
              string substr;
              getline( iss, substr, ',' );
              mapinfo_strings.push_back( substr );
            }
            XMinimum = atof(mapinfo_strings[3].c_str());
            float YMax = atof(mapinfo_strings[4].c_str());
             
            DataResolution = atof(mapinfo_strings[5].c_str());
            
            // get Y minimum
            // IMPORTANT THIS USES CONVENTION THAT THE MINIMUM AND MAXIMUM VALUES
            // ARE AT THE PIXEL EDGES AS IN QGIS!!! 
            YMinimum = YMax - (NRows)*DataResolution;

            //using a string comparison as float(X) != float(X) in many cases due to floating point math
            // http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm  - SWDG	          
            if (mapinfo_strings[5] != mapinfo_strings[6]) 
            {
              cout << "Warning! Loading ENVI DEM, but X and Y data spacing are different!" << endl;
            }

            //cout << "Xmin: " << XMinimum << " YMin: " << YMinimum << " spacing: " 
            //     << DataResolution << endl;

            counter = lines.size();
          }
          else
          {
            counter++;
          }
        }     

        // get the projection string
        counter = 0;
        string this_coordinate_system_string = "empty";
        str_find = "coordinate system string";
        while (counter < NLines)
        {
          found = lines[counter].find(str_find); 
          if (found!=string::npos)
          {
            //cout << "Found coordinate system string on line " << counter << '\n';  

            // now split the line 
            size_t start_pos;
            size_t end_pos;
            string open_curly_bracket = "{";
            string closed_curly_bracket = "}";
            start_pos = lines[counter].find(open_curly_bracket);
            end_pos = lines[counter].find(closed_curly_bracket);
            //cout << "startpos: " << start_pos << " and end pos: " << end_pos << endl;
            string csys_str = lines[counter].substr(start_pos+1, end_pos-start_pos-1);
            //cout << "\nThe coordinate system string is:\n" << csys_str << endl;
            string cs_key = "ENVI_coordinate_system";
            GeoReferencingStrings[cs_key] = csys_str;
            counter = lines.size();
          }
          else
          {
            counter++;
          }
        }          
      }         
    }
    ifs.close(); 

    // this is the array into which data is fed
    if (NoDataExists == 0)
    {
      NoDataValue = -9999;
    }
    //bool set_NDV = false;
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
      if (DataType == 2)
      {
        //cout << "Loading raster, recasting data from int to float!" << endl;
        int temp;
        //cout << "Integer size: " << sizeof(temp) << endl;
        for (int i=0; i<NRows; ++i)
        {
          for (int j=0; j<NCols; ++j)
          {
            ifs_data.read(reinterpret_cast<char*>(&temp), 2);
            //cout << temp << " ";
            data[i][j] = int(temp);
            if (data[i][j]<-1e10)
            {
              data[i][j] = NoDataValue;
            }
          }
          //cout << endl;
        }
      }
      else if (DataType == 4)
      {
        float temp;
        //cout << "Float size: " << sizeof(temp) << endl;
        for (int i=0; i<NRows; ++i)
        {
          for (int j=0; j<NCols; ++j)
          {
            ifs_data.read(reinterpret_cast<char*>(&temp), sizeof(temp));
            
            data[i][j] = int(temp);
            if (data[i][j]<-1e10)
            {
              data[i][j] = NoDataValue;
            }
          }
        }
      } 
      else if (DataType == 13)
      {
        unsigned long int temp;
        //cout << "Float size: " << sizeof(temp) << endl;
        for (int i=0; i<NRows; ++i)
        {
          for (int j=0; j<NCols; ++j)
          {
            ifs_data.read(reinterpret_cast<char*>(&temp), sizeof(temp));
            
            data[i][j] = int(temp);
            if (data[i][j]<-1e10)
            {
              data[i][j] = NoDataValue;
            }
          }
        }
      } 
      else
      {
        cout << "WARNING loading ENVI raster with unusual data type. " << endl
             << "If you get a crazy DEM go to LINE 604 of LSDIndexRaster.cpp to debug" << endl;
        int temp;   // might need to change this
        //cout << "Float size: " << sizeof(temp) << endl;
        for (int i=0; i<NRows; ++i)
        {
          for (int j=0; j<NCols; ++j)
          {
            // Use data type to control the bytes being read for each entry
            ifs_data.read(reinterpret_cast<char*>(&temp), DataType);
            
            data[i][j] = int(temp);
            if (data[i][j]<-1e10)
            {
              data[i][j] = NoDataValue;
            }
          }
        }
      }
    }
    ifs_data.close();

    cout << "Loading ENVI bil file; NCols: " << NCols << " NRows: " << NRows << endl
         << "X minimum: " << XMinimum << " YMinimum: " << YMinimum << endl
         << "Data Resolution: " << DataResolution << " and No Data Value: "
         << NoDataValue << endl;

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
// SMM 2012
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
      << "\nNODATA_value  " << NoDataValue
      << "\nbyteorder     LSBFIRST" << endl;
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
  else if (extension == "bil")
  {
    // float data (a binary format created by ArcMap) has a header file
    // this file must be opened first
    string header_filename;
    string header_extension = "hdr";
    header_filename = filename+dot+header_extension;

    // you need to strip the filename
    string frontslash = "/";
    size_t found = string_filename.find_last_of(frontslash); 
    //cout << "Found is: " << found << endl;
    
    int length = int(string_filename.length());
    string this_fname = string_filename.substr(found+1,length-found-1);
    //cout << "fname is: " << this_fname << endl;

    ofstream header_ofs(header_filename.c_str());
    string str;
    float temp;
    int data_type = sizeof(temp);
    
    header_ofs <<  "ENVI" << endl;
    header_ofs << "description = {" << endl << this_fname << "}" << endl;
    header_ofs <<  "samples = " << NCols << endl;
    header_ofs <<  "lines = " << NRows << endl;
    header_ofs <<  "bands = 1" << endl;
    header_ofs <<  "header offset = 0" << endl;
    header_ofs <<  "file type = ENVI Standard" << endl;
    header_ofs <<  "data type = " << data_type << endl;
    header_ofs <<  "interleave = bsq" << endl;
    header_ofs <<  "byte order = 0" << endl;
    
    // now check to see if there are the map info and coordinate system 
    map<string,string>::iterator iter;
    string cs_str_key = "ENVI_coordinate_system";
    string mi_str_key = "ENVI_map_info";

    string cs_str;
    string mi_str;
    iter = GeoReferencingStrings.find(mi_str_key);
    if (iter != GeoReferencingStrings.end() )
    {
      mi_str = (*iter).second;
      //cout << "Map info system string exists, it is: " << mi_str << endl;
      header_ofs <<  "map info = {"<<mi_str<<"}" << endl;
    }
    else
    {
      cout << "Warning, writing ENVI file but no map info string" << endl;
    } 
    iter = GeoReferencingStrings.find(cs_str_key);
    if (iter != GeoReferencingStrings.end() )
    {
      cs_str = (*iter).second;
      //cout << "Coord, system string exists, it is: " << cs_str << endl;
      header_ofs <<  "coordinate system string = {"<<cs_str<<"}" << endl;
    }
    else
    {
      cout << "Warning, writing ENVI file but no coordinate system string" << endl;
    }
    header_ofs <<  "data ignore value = " << NoDataValue << endl;
  
    header_ofs.close();

    // now do the main data
    ofstream data_ofs(string_filename.c_str(), ios::out | ios::binary);

    for (int i=0; i<NRows; ++i)
    {
      for (int j=0; j<NCols; ++j)
      {
        temp = float(RasterData[i][j]);
        data_ofs.write(reinterpret_cast<char *>(&temp),data_type);
      }
    }
    data_ofs.close();
  }
  else
  {
    cout << "You did not enter and approprate extension!" << endl
         << "You entered: " << extension << " options are .flt, .bil and .asc" << endl;
    exit(EXIT_FAILURE);
  }

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function returns the x and y location of a row and column
// Same as above but with floats
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc)
{
  
  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
    
  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Method to flatten an LSDRaster and place the non NDV values in a csv file.
// Each value is placed on its own line, so that it can be read more quickly in python etc.
// It includes the x and y locations so it can be read by GIS software
// SMM 29/6/15
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::FlattenToCSV(string FileName_prefix)
{

  // append csv to the filename
  string FileName = FileName_prefix+".csv";
  
  //open a file to write
  ofstream WriteData;                                
  WriteData.open(FileName.c_str());
  
  WriteData.precision(8);
  WriteData << "x,y,value" << endl;
  
  // the x and y locations
  float x_loc, y_loc;

  //loop over each cell and if there is a value, write it to the file
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      if (RasterData[i][j] != NoDataValue)
      {
        get_x_and_y_locations(i,j,x_loc,y_loc);
        WriteData << x_loc << "," << y_loc << "," << RasterData[i][j] << endl;
      }
    }
  }

  WriteData.close();

} 


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function checks to see if two rasters have the same dimesions 
// It DOES NOT check georeferencingand
// Georeferencing
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool LSDIndexRaster::does_raster_have_same_dimensions(LSDRaster& Compare_raster)
{

  // me check with bad grammar
  bool is_dimensions_same;

  if (NRows == Compare_raster.get_NRows() &&
      NCols == Compare_raster.get_NCols() &&
      XMinimum == Compare_raster.get_XMinimum() &&
      YMinimum == Compare_raster.get_YMinimum() &&
      DataResolution == Compare_raster.get_DataResolution() &&
      NoDataValue == Compare_raster.get_NoDataValue())
  {
    is_dimensions_same = true;
  }
  else
  {
    is_dimensions_same = false;
  }

  return is_dimensions_same;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function checks to see if two rasters have the same dimesions 
// It DOES NOT check georeferencingand
// Georeferencing
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool LSDIndexRaster::does_raster_have_same_dimensions(LSDIndexRaster& Compare_raster)
{

  // me check with bad grammar
  bool is_dimensions_same;

  if (NRows == Compare_raster.get_NRows() &&
      NCols == Compare_raster.get_NCols() &&
      XMinimum == Compare_raster.get_XMinimum() &&
      YMinimum == Compare_raster.get_YMinimum() &&
      DataResolution == Compare_raster.get_DataResolution() &&
      NoDataValue == Compare_raster.get_NoDataValue())
  {
    is_dimensions_same = true;
  }
  else
  {
    is_dimensions_same = false;
  }

  return is_dimensions_same;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function checks to see if two rasters have the same dimesions and
// Georeferencing
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool LSDIndexRaster::does_raster_have_same_dimensions_and_georeferencing(LSDRaster& Compare_raster)
{
  map<string,string> DRS = Compare_raster.get_GeoReferencingStrings();
  string mi_str_key = "ENVI_map_info";
  string cs_str_key = "ENVI_coordinate_system";
  
  bool is_georef_and_dimensions_same;

  if (NRows == Compare_raster.get_NRows() &&
      NCols == Compare_raster.get_NCols() &&
      XMinimum == Compare_raster.get_XMinimum() &&
      YMinimum == Compare_raster.get_YMinimum() &&
      DataResolution == Compare_raster.get_DataResolution() &&
      NoDataValue == Compare_raster.get_NoDataValue() &&
      GeoReferencingStrings[mi_str_key] == DRS[mi_str_key] &&
      GeoReferencingStrings[cs_str_key] == DRS[cs_str_key])
  {
    is_georef_and_dimensions_same = true;
  }
  else
  {
    is_georef_and_dimensions_same = false;
  }

  return is_georef_and_dimensions_same;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function checks to see if two rasters have the same dimesions and
// Georeferencing. Uses an index raster to check
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool LSDIndexRaster::does_raster_have_same_dimensions_and_georeferencing(LSDIndexRaster& Compare_raster)
{
  map<string,string> DRS = Compare_raster.get_GeoReferencingStrings();
  string mi_str_key = "ENVI_map_info";
  string cs_str_key = "ENVI_coordinate_system";
  
  bool is_georef_and_dimensions_same;

  if (NRows == Compare_raster.get_NRows() &&
      NCols == Compare_raster.get_NCols() &&
      XMinimum == Compare_raster.get_XMinimum() &&
      YMinimum == Compare_raster.get_YMinimum() &&
      DataResolution == Compare_raster.get_DataResolution() &&
      NoDataValue == Compare_raster.get_NoDataValue() &&
      GeoReferencingStrings[mi_str_key] == DRS[mi_str_key] &&
      GeoReferencingStrings[cs_str_key] == DRS[cs_str_key])
  {
    is_georef_and_dimensions_same = true;
  }
  else
  {
    is_georef_and_dimensions_same = false;
  }

  return is_georef_and_dimensions_same;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// THis clips to a smaller raster. The smaller raster does not need
// to have the same data resolution as the old raster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDIndexRaster::clip_to_smaller_raster(LSDIndexRaster& smaller_raster)
{
  // Get the MinX, MaxX, MinY, MaxY from the rasters
  //float XMaximum = XMinimum + (NCols * DataResolution -1);
  //float YMaximum = YMinimum + (NRows * DataResolution -1);
  
  float SR_XMinimum = smaller_raster.get_XMinimum();
  float SR_YMinimum = smaller_raster.get_YMinimum();
  
  float SR_NRows = smaller_raster.get_NRows();
  float SR_NCols = smaller_raster.get_NCols();
  float SR_DataR = smaller_raster.get_DataResolution();
  
  float SR_XMaximum = SR_XMinimum+(SR_NCols)*SR_DataR;
  float SR_YMaximum = SR_YMinimum+(SR_NRows)*SR_DataR;
  
  cout << "Small Xmin: " << SR_XMinimum << " YMin: " << SR_YMinimum << " Xmax: "
       << SR_XMaximum << " YMax: " << SR_YMaximum << endl;
  
  
  // find the col of old raster that has the same Xlocations as the XLL of smaller raster
  // the 0.5*DataResolution is in case of rounding errors
  int XLL_col = int((SR_XMinimum-XMinimum+0.5*DataResolution)/DataResolution);
  int XUL_col = int((SR_XMaximum-XMinimum+0.5*DataResolution)/DataResolution);
  
  // check these columns
  if (XLL_col < 0)
  {
    XLL_col = 0;
  }
  if (XUL_col >= NCols)
  {
    XUL_col = NCols-1;
  }
  
  // find the row of old raster that has the same Xlocations as the XLL of smaller raster
  // the 0.5*DataResolution is in case of rounding errors  
  // Slightly different logic for y because the DEM starts from the top corner
  int YLL_row = NRows - int((SR_YMinimum-YMinimum+0.5*DataResolution)/DataResolution);
  int YUL_row = NRows - int((SR_YMaximum-YMinimum+0.5*DataResolution)/DataResolution);
  
  // check these rows
  if (YLL_row < 0)
  {
    YLL_row = 0;
  }
  if (YUL_row >= NRows)
  {
    YUL_row = NRows-1;
  }
  
  cout << "Small XLLCol: " << XLL_col << " XLR_col: " << XUL_col << " XLLrow: "
       << YLL_row << " YUL_row: " << YUL_row << endl;


  // get the new number of rows and columns:
  int New_NRows = YLL_row-YUL_row;
  int New_NCols = XUL_col-XLL_col;
  
  cout << "New NRows: " << New_NRows  << " New_NCols: " << New_NCols << endl;
  
  // now extract the data for the new raster
  float NewR_XMinimum = XMinimum+float(XLL_col)*DataResolution;
  float NewR_YMinimum = YMinimum + ((NRows - YLL_row ) * DataResolution);
  
  Array2D<int> NewData(New_NRows,New_NCols, NoDataValue);
  
  for(int row = 0; row< New_NRows; row++)
  {
    for(int col = 0; col<=New_NCols; col++)
    {
       NewData[row][col] = RasterData[row+YUL_row][col+XLL_col];
    }
  }
  
  LSDIndexRaster TrimmedRaster(New_NRows, New_NCols, NewR_XMinimum,
                          NewR_YMinimum, DataResolution, NoDataValue, NewData, 
                          GeoReferencingStrings);  

  TrimmedRaster.Update_GeoReferencingStrings();
  
  return TrimmedRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// THis clips to a smaller raster. The smaller raster does not need
// to have the same data resolution as the old raster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDIndexRaster::clip_to_smaller_raster(LSDRaster& smaller_raster)
{
  // Get the MinX, MaxX, MinY, MaxY from the rasters
  //float XMaximum = XMinimum + (NCols * DataResolution -1);
  //float YMaximum = YMinimum + (NRows * DataResolution -1);
  
  float SR_XMinimum = smaller_raster.get_XMinimum();
  float SR_YMinimum = smaller_raster.get_YMinimum();
  
  float SR_NRows = smaller_raster.get_NRows();
  float SR_NCols = smaller_raster.get_NCols();
  float SR_DataR = smaller_raster.get_DataResolution();
  
  float SR_XMaximum = SR_XMinimum+(SR_NCols)*SR_DataR;
  float SR_YMaximum = SR_YMinimum+(SR_NRows)*SR_DataR;
  
  cout << "Small Xmin: " << SR_XMinimum << " YMin: " << SR_YMinimum << " Xmax: "
       << SR_XMaximum << " YMax: " << SR_YMaximum << endl;
  
  
  // find the col of old raster that has the same Xlocations as the XLL of smaller raster
  // the 0.5*DataResolution is in case of rounding errors
  int XLL_col = int((SR_XMinimum-XMinimum+0.5*DataResolution)/DataResolution);
  int XUL_col = int((SR_XMaximum-XMinimum+0.5*DataResolution)/DataResolution);
  
  // check these columns
  if (XLL_col < 0)
  {
    XLL_col = 0;
  }
  if (XUL_col >= NCols)
  {
    XUL_col = NCols-1;
  }
  
  // find the row of old raster that has the same Xlocations as the XLL of smaller raster
  // the 0.5*DataResolution is in case of rounding errors  
  // Slightly different logic for y because the DEM starts from the top corner
  int YLL_row = NRows - int((SR_YMinimum-YMinimum+0.5*DataResolution)/DataResolution);
  int YUL_row = NRows - int((SR_YMaximum-YMinimum+0.5*DataResolution)/DataResolution);
  
  // check these rows
  if (YLL_row < 0)
  {
    YLL_row = 0;
  }
  if (YUL_row >= NRows)
  {
    YUL_row = NRows-1;
  }
  
  cout << "Small XLLCol: " << XLL_col << " XLR_col: " << XUL_col << " XLLrow: "
       << YLL_row << " YUL_row: " << YUL_row << endl;


  // get the new number of rows and columns:
  int New_NRows = YLL_row-YUL_row;
  int New_NCols = XUL_col-XLL_col;
  
  cout << "New NRows: " << New_NRows  << " New_NCols: " << New_NCols << endl;
  
  // now extract the data for the new raster
  float NewR_XMinimum = XMinimum+float(XLL_col)*DataResolution;
  float NewR_YMinimum = YMinimum + ((NRows - YLL_row ) * DataResolution);
  
  Array2D<int> NewData(New_NRows,New_NCols, NoDataValue);
  
  for(int row = 0; row< New_NRows; row++)
  {
    for(int col = 0; col<=New_NCols; col++)
    {
       NewData[row][col] = RasterData[row+YUL_row][col+XLL_col];
    }
  }
  
  LSDIndexRaster TrimmedRaster(New_NRows, New_NCols, NewR_XMinimum,
                          NewR_YMinimum, DataResolution, NoDataValue, NewData, 
                          GeoReferencingStrings);  

  TrimmedRaster.Update_GeoReferencingStrings();
  
  return TrimmedRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
// Method which takes a new xmin and ymax value and modifys the GeoReferencingStrings
// map_info line to contain these new values. Intended for use in the rastertrimmer
// methods and is called from within these methods.
// 
// Modifying georeferencing information by hand is messy and should be avoided if
// at all possible. 
//
// Returns an updated GeoReferencingStrings object
//
// SWDG 6/11/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
map<string, string> LSDIndexRaster::Update_GeoReferencingStrings(float NewXmin, float NewYmax)
{

  // set up strings and iterators
  map<string,string>::iterator iter;

  //String to get the map_info out of the map
  string cs_key = "ENVI_map_info";  

  //check to see if there is already a map info string
  iter = GeoReferencingStrings.find(cs_key);
  if (iter != GeoReferencingStrings.end() )
  {
    // there is a mapinfo string  
    // now parse the string
    vector<string> mapinfo_strings;
    istringstream iss(GeoReferencingStrings[cs_key]);
    while( iss.good() )
    {
      string substr;
      getline( iss, substr, ',' );
      mapinfo_strings.push_back( substr );
    }
  
    //Construct the new string with the updated xmin ymin values
    stringstream CombineMapinfo;

    CombineMapinfo << mapinfo_strings[0] << "," << mapinfo_strings[1] << "," 
       << mapinfo_strings[2] << ", " << NewXmin << ", " << NewYmax << "," 
       << mapinfo_strings[5] << "," << mapinfo_strings[6] << "," << mapinfo_strings[7] 
      << "," << mapinfo_strings[8] << "," << mapinfo_strings[9];

    //Store the new string in the map
    GeoReferencingStrings[cs_key] = CombineMapinfo.str();

  }

  
  return GeoReferencingStrings;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Similar to above, but in this case the function uses data stored within
// the data members of the raster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::Update_GeoReferencingStrings()
{

  float YMax =  YMinimum + NRows*DataResolution;

  // set up strings and iterators
  map<string,string>::iterator iter;
  string mi_str_key = "ENVI_map_info";
  
  string info_str;
  
  //check to see if there is already a map info string
  iter = GeoReferencingStrings.find(mi_str_key);
  if (iter != GeoReferencingStrings.end() )
  {
    // there is a mapinfo string  
    info_str = GeoReferencingStrings[mi_str_key];

    // now parse the string
    vector<string> mapinfo_strings;
    istringstream iss(info_str);
    while( iss.good() )
    {
      string substr;
      getline( iss, substr, ',' );
      mapinfo_strings.push_back( substr );
    }

    //Construct the new string with the updated xmin ymin values
    stringstream CombineMapinfo;

    CombineMapinfo << mapinfo_strings[0] << "," << mapinfo_strings[1] << "," 
       << mapinfo_strings[2] << ", " << XMinimum << ", " << YMax << "," 
       << DataResolution << "," << DataResolution << "," << mapinfo_strings[7] 
       << "," << mapinfo_strings[8] << "," << mapinfo_strings[9];
    
    //Store the new string in the map
    GeoReferencingStrings[mi_str_key] = CombineMapinfo.str();
    
    
    cout << "New string is: " << endl << GeoReferencingStrings[mi_str_key] << endl;
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// 
// This function imposes the mapinfo strings. It assumes UTM
// THIS HAS NOT BEEN TESTED!!!!!!!!!!!!
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDIndexRaster::impose_georeferencing_UTM(int zone, string NorS)
{
  string str_NorS;
  string str_NorSlong;
  string cs_string_fnbit;
  if (NorS.find("N") == 0 || NorS.find("n") == 0)
  {
    str_NorS = "N";
    str_NorSlong = "North";
    cs_string_fnbit = "0";
  }
  else if (NorS.find("S") == 0 || NorS.find("s") == 0)
  {
    str_NorS = "S";
    str_NorSlong = "South"; 
    cs_string_fnbit = "10000000";   
  }
  else
  {
    cout << "imposing georeferencing, but I didn't understand N or S, defaulting to North." << endl;
    str_NorS = "N";
    str_NorSlong = "North";
    cs_string_fnbit = "0";
  }

  string delim = ", ";
  string str_UTM = "UTM";
  string str_x  = "1";
  string str_y = "1";
  string xmin = dtoa(XMinimum);
  float YMax =  YMinimum + NRows*DataResolution;
  string ymax = dtoa(YMax);
  
  string DR = dtoa(DataResolution);
  string str_UTMZ = itoa(zone);
  string str_hemis = str_NorSlong;
  string str_spheroid = "WGS-84";  

  string new_string = str_UTM+delim+str_x+delim+str_y+delim+xmin+delim
                       +ymax+delim+DR+delim+DR+delim+str_UTMZ+delim+str_hemis
                       +delim+str_spheroid;
  GeoReferencingStrings["ENVI_map_info"]= new_string;
  
  string cs_string_firstbit = "PROJCS[\"WGS_1984_UTM_Zone_";
  string cs_string_secondbit = str_UTMZ+str_NorS;
  string cs_string_thirdbit =  "\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",";
  string cs_string_fifthbit = "],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",";
  string cs_string_seventhbit = "],UNIT[\"Meter\",1]]";
  
  int central_meridian = Find_UTM_central_meridian(zone);
  string cs_string_central_merid = itoa(central_meridian);
  
  
  string cs_str = cs_string_firstbit+cs_string_secondbit+cs_string_thirdbit
                 +cs_string_central_merid+cs_string_fifthbit+cs_string_fnbit
                 +cs_string_seventhbit;
                 
  GeoReferencingStrings["ENVI_coordinate_system"]= cs_str;               
}                 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This is a utility function to find the central meridian of a UTM zone
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int LSDIndexRaster::Find_UTM_central_meridian(int UTM_zone)
{
  // initiate the vector
  vector<int> zone(61,NoDataValue);
  
  // here is the lookuptable
  zone[1]=-177;
  zone[2]=-171;
  zone[3]=-165;
  zone[4]=-159;
  zone[5]=-153;
  zone[6]=-147;
  zone[7]=-141;
  zone[8]=-135;
  zone[9]=-129;
  zone[10]=-123;
  zone[11]=-117;
  zone[12]=-111;
  zone[13]=-105;
  zone[14]=-99;
  zone[15]=-93;
  zone[16]=-87;
  zone[17]=-81;
  zone[18]=-75;
  zone[19]=-69;
  zone[20]=-63;
  zone[21]=-57;
  zone[22]=-51;
  zone[23]=-45;
  zone[24]=-39;
  zone[25]=-33;
  zone[26]=-27;
  zone[27]=-21;
  zone[28]=-15;
  zone[29]=-9;
  zone[30]=-3;
  zone[31]=3;
  zone[32]=9;
  zone[33]=15;
  zone[34]=21;
  zone[35]=27;
  zone[36]=33;
  zone[37]=39;
  zone[38]=45;
  zone[39]=51;
  zone[40]=57;
  zone[41]=63;
  zone[42]=69;
  zone[43]=75;
  zone[44]=81;
  zone[45]=87;
  zone[46]=93;
  zone[47]=99;
  zone[48]=105;
  zone[49]=111;
  zone[50]=117;
  zone[51]=123;
  zone[52]=129;
  zone[53]=135;
  zone[54]=141;
  zone[55]=147;
  zone[56]=153;
  zone[57]=159;
  zone[58]=165;
  zone[59]=171;
  zone[60]=177;

  int central_meridian;

  // now look up the table
  if(UTM_zone <1 || UTM_zone > 60)
  {
    cout << "Trying to assign central meridian but you have chosen an invalid UTM zone" << endl;
    cout << "defaulting to central meridian of 0";
    central_meridian = 0;
  }
  else
  {
    central_meridian = zone[UTM_zone];
  }
  
  return central_meridian;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Checks to see is a point is in the raster
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool LSDIndexRaster::check_if_point_is_in_raster(float X_coordinate, float Y_coordinate)
{
  bool is_in_raster = true;
  
  // Shift origin to that of dataset
  float X_coordinate_shifted_origin = X_coordinate - XMinimum - DataResolution*0.5; 
  float Y_coordinate_shifted_origin = Y_coordinate - YMinimum - DataResolution*0.5; 

  // Get row and column of point
  int col_point = int(X_coordinate_shifted_origin/DataResolution);
  int row_point = (NRows - 1) - int(round(Y_coordinate_shifted_origin/DataResolution));

  if(col_point < 0 || col_point > NCols-1 || row_point < 0 || row_point > NRows -1)
  {
    is_in_raster = false;
  }

  return is_in_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Calculate the minimum bounding rectangle for an LSDIndexRaster Object and crop out
// all the surrounding NoDataValues to reduce the size and load times of output
// rasters.
//
// Ideal for use with chi analysis tools which output basin and chi m value rasters
// which can be predominantly no data. As an example, a 253 Mb file can be reduced to
// ~5 Mb with no loss or resampling of data.
//
// Returns A trimmed LSDIndexRaster object.
//
// SWDG 22/08/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::RasterTrimmer(){

  //minimum index value in a column
  int a = 0;
  int min_col = 100000; //a big number

  for (int row = 0; row < NRows; ++row){
    a = 0;
    while (RasterData[row][a] == NoDataValue && a < NCols-1){
      ++a;
    }
    if (min_col > a){
      min_col = a;
    }
  }

  //maximum index value in a column
  a = NCols - 1;
  int max_col = 0; //a small number

  for (int row = 0; row < NRows; ++row){
    a = NCols - 1;
    while (RasterData[row][a] == NoDataValue && a > 0){
      --a;
    }
    if (max_col < a){
      max_col = a;
    }
  }

  //minimum index value in a row
  a = 0;
  int min_row = 100000; //a big number

  for (int col = 0; col < NCols; ++col){
    a = 0;
    while (RasterData[a][col] == NoDataValue && a < NRows - 1){
      ++a;
    }
    if (min_row > a){
      min_row = a;
    }
  }

  //maximum index value in a row
  a = NRows - 1;
  int max_row = 0; //a small number

  for (int col = 0; col < NCols; ++col){
    a = NRows - 1;
    while (RasterData[a][col] == NoDataValue && a > 0){
      --a;
    }
    if (max_row < a){
      max_row = a;
    }
  }

  // create new row and col sizes taking account of zero indexing
  int new_row_dimension = (max_row-min_row) + 1;
  int new_col_dimension = (max_col-min_col) + 1;

  Array2D<int>TrimmedData(new_row_dimension, new_col_dimension, NoDataValue);

  //loop over min bounding rectangle and store it in new array of shape new_row_dimension x new_col_dimension
  int TrimmedRow = 0;
  int TrimmedCol = 0;
  for (int row = min_row - 1; row < max_row; ++row){
    for(int col = min_col - 1; col < max_col; ++col){
      TrimmedData[TrimmedRow][TrimmedCol] = RasterData[row][col];
      ++TrimmedCol;
    }
    ++TrimmedRow;
    TrimmedCol = 0;
  }

  //calculate lower left corner coordinates of new array
  float new_XLL = ((min_col - 1) * DataResolution) + XMinimum;
  float new_YLL = YMinimum + ((NRows - (max_row + 0)) * DataResolution);

  LSDIndexRaster TrimmedRaster(new_row_dimension, new_col_dimension, new_XLL,
                          new_YLL, DataResolution, NoDataValue, TrimmedData);

  return TrimmedRaster;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Make LSDIndexRaster object using a 'template' raster and an Array2D of data.
// SWDG 2/9/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::LSDRasterTemplate(Array2D<int> InputData){

  //do a dimensions check and exit on failure
  if (InputData.dim1() == NRows && InputData.dim2() == NCols){
    LSDIndexRaster OutputRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, InputData,GeoReferencingStrings);
    return OutputRaster;
  }
  else{
   	cout << "Array dimensions do not match template LSDIndexRaster object" << endl;
		exit(EXIT_FAILURE);
  }

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to resample an LSDIndexRaster to a lower resolution. 
// OutputResolution is the resolution in spatial units to be resampled to.
// Returns an LSDRaster resampled to the OutputResolution.
// SWDG 17/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=   
LSDIndexRaster LSDIndexRaster::Resample(float OutputResolution){

  if (OutputResolution < DataResolution){
    cout << "Your resample resolution of " << OutputResolution << " is lower that the current data resolution " << DataResolution << endl;
    exit(EXIT_FAILURE);
  } 

  int NewNRows = (NRows*DataResolution/OutputResolution); 
  int NewNCols = (NCols*DataResolution/OutputResolution);

  Array2D<int> Resampled(NewNRows, NewNCols, NoDataValue);
  
  int centre_i;
  int centre_j;   
  
  float ResolutionRatio = OutputResolution/DataResolution;
  
  for (int i = 0; i < NewNRows; ++i){
    for (int j = 0; j < NewNCols; ++j){
    
      //find the centre of the new grid in the old grid units
      centre_i = (i*ResolutionRatio) + (ResolutionRatio/2);
      centre_j = (j*ResolutionRatio) + (ResolutionRatio/2);
     
      Resampled[i][j] = RasterData[centre_i][centre_j];           
      
    }
  }                              

  LSDIndexRaster OutputRaster(NewNRows,NewNCols,XMinimum,YMinimum,OutputResolution,NoDataValue,Resampled);
  return OutputRaster;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=  
// Method to combine two rasters, ignoring nodata.  
// SWDG 17/6/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=  
LSDIndexRaster LSDIndexRaster::CombineBinaryNetwork(LSDIndexRaster& Network1, LSDIndexRaster& Network2){

  Array2D<int> CombinedNetworkArray(NRows, NCols, NoDataValue);

   for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
   
      if (Network1.get_data_element(i,j) == NoDataValue && Network2.get_data_element(i,j) == NoDataValue){
        CombinedNetworkArray[i][j] = NoDataValue;
      }
      else if ((Network1.get_data_element(i,j) == NoDataValue && Network2.get_data_element(i,j) != NoDataValue)){
        CombinedNetworkArray[i][j] = Network2.get_data_element(i,j);  
      }
      else if ((Network1.get_data_element(i,j) != NoDataValue && Network2.get_data_element(i,j) == NoDataValue)){
        CombinedNetworkArray[i][j] = Network1.get_data_element(i,j);  
      }
      else if ((Network1.get_data_element(i,j) != NoDataValue && Network2.get_data_element(i,j) != NoDataValue)){
        CombinedNetworkArray[i][j] = Network2.get_data_element(i,j) + Network1.get_data_element(i,j);  
      }
      
    }
  }

  LSDIndexRaster CombinedNetwork(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, CombinedNetworkArray,GeoReferencingStrings);
  return CombinedNetwork;

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=  
// Method to merge a floodplain raster with a channel raster. Creates an output 
// LSDIndexRaster which is coded channel == input channel index, floodplain == 500, NDV == hillslopes.   
// SWDG 05/03/15
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=  
LSDIndexRaster LSDIndexRaster::MergeChannelWithFloodplain(LSDIndexRaster FloodPlain){

  //get the Channel network data as an array
  Array2D<int> ChannelArray = RasterData.copy();
  
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
    
      if (ChannelArray[i][j] != NoDataValue && FloodPlain.get_data_element(i,j) != NoDataValue){
          ChannelArray[i][j] = 1;
      }
      else if (ChannelArray[i][j] == NoDataValue && FloodPlain.get_data_element(i,j) != NoDataValue){
          ChannelArray[i][j] = 500;
      }        
    
    }
  }

  LSDIndexRaster CombinedOutput(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, ChannelArray, GeoReferencingStrings);
  return CombinedOutput;


}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Identify connected components from binary array using a two-pass approach.
// Closely follows the algorithm described by He et al. (2008), "A Run-Based Two-Scan Labeling
// Algorithm," Image Processing, IEEE Transactions on , vol.17, no.5, pp.749,756,
// doi: 10.1109/TIP.2008.919369
// Components must be identified by the number 1.
// DTM 13/07/2015
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::ConnectedComponents()
{
  cout << "\t\t Connected Components; first pass" << endl;
  Array2D<int> LabelledComponents(NRows,NCols,NoDataValue);
  int next_label = 0;
  bool flag = false;

  DisjointSet DS;

  for(int i = 0; i<NRows; ++i){
    for(int j = 0; j<NCols; ++j){
      // Iterate through the array
      if(RasterData[i][j] == 1){
        if(flag == false){
          LabelledComponents[i][j] = next_label;
	  DS.DSMakeSet(next_label);
	  ++next_label;
          flag = true; 
        }
        // Check neighbours
        else{
          // first row is easy
          if(i == 0){
            if(LabelledComponents[i][j-1] != NoDataValue) LabelledComponents[i][j] = LabelledComponents[i][j-1];
            else{
              LabelledComponents[i][j] = next_label;
	      DS.DSMakeSet(next_label);
	      ++next_label;
            }
          }
          // for first column, need to check above and above right
          else if(j==0){
            int above = LabelledComponents[i-1][j];
            int above_right = LabelledComponents[i-1][j+1];
            vector<int> neighbourhood_labels;

            if(above != NoDataValue) neighbourhood_labels.push_back(above);
            if(above_right != NoDataValue) neighbourhood_labels.push_back(above_right);

            int N = neighbourhood_labels.size();
            if (N == 0){
              LabelledComponents[i][j] = next_label;
	      DS.DSMakeSet(next_label);
              ++next_label;
	    }
	    else if(N==1) LabelledComponents[i][j] = neighbourhood_labels[0];
            else{
	      DSnode* node1 = DS.get_DSnode(neighbourhood_labels[0]);
	      DSnode* node2 = DS.get_DSnode(neighbourhood_labels[1]);
	      LabelledComponents[i][j] = DS.Union_return_label(node1,node2);
	    }
          }
          // for final column, check above left, above left and above.
          else if(j==NCols-1){ 
            int above_left = LabelledComponents[i-1][j-1];
            int above = LabelledComponents[i-1][j];
            int left = LabelledComponents[i][j-1];
            vector<int> neighbourhood_labels;

            if(above_left != NoDataValue) neighbourhood_labels.push_back(above_left);
            if(above != NoDataValue) neighbourhood_labels.push_back(above);
            if(left != NoDataValue) neighbourhood_labels.push_back(left);

            int N = neighbourhood_labels.size();
            if (N == 0){
              LabelledComponents[i][j] = next_label;
	      DS.DSMakeSet(next_label);
              ++next_label;
	    }
	    else if(N==1) LabelledComponents[i][j] = neighbourhood_labels[0];
            else{
	      DSnode* node1 = DS.get_DSnode(neighbourhood_labels[0]);
	      for(int k = 1; k<N; ++k){
		DSnode* node2 = DS.get_DSnode(neighbourhood_labels[k]);
		LabelledComponents[i][j] = DS.Union_return_label(node1,node2);
	      }
	    }
          }
          // for other cells, check above left, above, above right and left.
          else{
            int above_left = LabelledComponents[i-1][j-1];
            int above = LabelledComponents[i-1][j];
            int above_right = LabelledComponents[i-1][j+1];
            int left = LabelledComponents[i][j-1];
            vector<int> neighbourhood_labels;
            if(above_left != NoDataValue) neighbourhood_labels.push_back(above_left);
            if(above != NoDataValue) neighbourhood_labels.push_back(above);
            if(above_right != NoDataValue) neighbourhood_labels.push_back(above_right);
            if(left != NoDataValue) neighbourhood_labels.push_back(left);
            int N = neighbourhood_labels.size();
	    if (N == 0){
              LabelledComponents[i][j] = next_label;
	      DS.DSMakeSet(next_label);
              ++next_label;
	    }
	    else if(N==1) LabelledComponents[i][j] = neighbourhood_labels[0];
            else{
	      DSnode* node1 = DS.get_DSnode(neighbourhood_labels[0]);
	      for(int k = 1; k<N; ++k){
		DSnode* node2 = DS.get_DSnode(neighbourhood_labels[k]);
		LabelledComponents[i][j] = DS.Union_return_label(node1,node2);
	      }
	    }
          }
	}
      }
    }
  }
  DS.Reduce();
  // Second pass, assign equivalences 
  cout << "Second pass" << endl;
  for(int i = 0; i<NRows; ++i){
    for(int j = 0; j<NCols; ++j){
      if(LabelledComponents[i][j]!=NoDataValue){
	LabelledComponents[i][j] = DS.get_parent(LabelledComponents[i][j]);
      }
    }
  }

  // Now 
  LSDIndexRaster ConnectedComponentsRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,LabelledComponents,GeoReferencingStrings);
  return ConnectedComponentsRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// The following two functions are used to thin a multi-pixel binary feature into a single pixel skeleton.  It uses the algorithm described by Zhang and Suen (1984), A fast algorithm for thinning digital patterns, Communications of the ACM.
// Thinning algorithm 
void LSDIndexRaster::thinningIteration(Array2D<int>& binary, int iter){

  Array2D<int> marker(NRows,NCols,0);
  int p2,p3,p4,p5,p6,p7,p8,p9;
  for(int i = 1; i<NRows-1; ++i){
    for(int j = 1; j<NCols-1; ++j){
      p2 = binary[i-1][j];
      p3 = binary[i-1][j+1];
      p4 = binary[i][j+1];
      p5 = binary[i+1][j+1];
      p6 = binary[i+1][j];
      p7 = binary[i+1][j-1];
      p8 = binary[i][j-1];
      p9 = binary[i-1][j-1];
      int A = ((p2==0) && (p3==1)) + ((p3==0) && (p4==1)) + ((p4==0) && (p5==1)) + ((p5==0) && (p6==1)) + ((p6==0) && (p7==1)) + ((p7==0) && (p8==1)) + ((p8==0) && (p9==1)) + ((p9==0) && (p2==1));
      int B = p2+p3+p4+p5+p6+p7+p8+p9;
      int m1,m2;
      if(iter==0){
	m1 = p2*p4*p6;
	m2 = p4*p6*p8;
      }
      else{
	m1 = p2*p4*p8;
	m2 = p2*p6*p8;
      }
      if(A==1 && B>=2 && B<=6 && m1==0 && m2==0){
	marker[i][j]=1;
      }
    }
  }
  for(int i=1; i<NRows-1; ++i){
    for(int j=1;j<NCols-1; ++j){
      if(marker[i][j]==1) binary[i][j]=0;
    }
  }
}
LSDIndexRaster LSDIndexRaster::thin_to_skeleton(){
  int finish_flag = 0;
  Array2D<int> binary_old(NRows,NCols,0);
  // Remove nodata pixels
  for(int i=0; i<NRows; ++i){
    for(int j=0; j<NCols; ++j){
      if(RasterData[i][j]==1) binary_old[i][j] = 1;
    }    
  }
  Array2D<int> binary_new = binary_old.copy();
  int count = 1;
  int total_removed = 0;
  int even = 1;
  int odd = 0;
  while(finish_flag == 0){
    cout << flush << "Thinning - iteration number " << count << "; ";
    ++count;
    int removed = 0;
    finish_flag = 1;
    thinningIteration(binary_new,odd);
    thinningIteration(binary_new,even);
    for(int i=0; i<NRows; ++i){
      for(int j=0; j<NCols; ++j){
        // Check to see if there are any changes this time
	// Complete iteration as soon as a difference is detected
	if(binary_new[i][j]!=binary_old[i][j]){
	  ++removed;
	  finish_flag = 0;
	}
      }
    }
    total_removed += removed;
    cout << "removed " << removed << " pixels; " << total_removed << "; removed in total     \r";
    binary_old = binary_new.copy();
  }
  cout << "\nDone" << endl;
  LSDIndexRaster skeleton(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,binary_new.copy(),GeoReferencingStrings);
  return skeleton;
}

LSDIndexRaster LSDIndexRaster::find_end_points()
{
  Array2D<int> EndPoints(NRows,NCols,NoDataValue);
  int test;
  for(int i=1; i<NRows-1; ++i){
    cout << flush << i << "/" << NRows << "\r";
    for(int j=1; j<NCols-1; ++j){
      if(RasterData[i][j]==1){	
	test = RasterData[i-1][j]+RasterData[i-1][j+1]+RasterData[i][j+1]+RasterData[i+1][j+1]+RasterData[i+1][j]+RasterData[i+1][j-1]+RasterData[i][j-1]+RasterData[i-1][j-1];
	if(test<=1){
	  EndPoints[i][j] = 1;
	}
      }
    }
  }
  LSDIndexRaster Ends(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,EndPoints,GeoReferencingStrings);
  return Ends;
}

void LSDIndexRaster::remove_downstream_endpoints(LSDIndexRaster CC, LSDRaster Topo){
  //first loop through the array to find the number of different components to check
  int max_segment_ID = 0;
  for(int i = 0; i<NRows; ++i){
    for(int j = 0; j<NCols; ++j){
      if(RasterData[i][j]!=NoDataValue && CC.get_data_element(i,j) > max_segment_ID) max_segment_ID = CC.get_data_element(i,j);
    }
  }
  vector<vector<int> > end_points_row,end_points_col;
  vector<vector<float> > end_point_elevations;
  vector<float> empty_float;
  vector<int> empty_int;
  for(int i=0; i < max_segment_ID+1; ++i){
    end_points_row.push_back(empty_int);
    end_points_col.push_back(empty_int);
    end_point_elevations.push_back(empty_float);
  }
  int index;
  for(int i = 0; i<NRows; ++i){
    for(int j = 0; j<NCols; ++j){
      if(RasterData[i][j]!=NoDataValue){
	index = CC.get_data_element(i,j);
	end_points_row[index].push_back(i);
	end_points_col[index].push_back(j);
	end_point_elevations[index].push_back(Topo.get_data_element(i,j));
      }
    }
  }
  //Now sort end points by elevation, and remove the lowest elevation point in each group
  Array2D<int> FilteredEnds(NRows,NCols,NoDataValue);
  for(int i=0; i < max_segment_ID+1;++i){
    int N = end_point_elevations[i].size();
    if(N>0){
      vector<size_t> index_map;
      matlab_float_sort(end_point_elevations[i], end_point_elevations[i], index_map);
      matlab_int_reorder(end_points_row[i],index_map,end_points_row[i]);
      matlab_int_reorder(end_points_col[i],index_map,end_points_col[i]);
      // erase lowest end point to leave just the segment heads.
      for(int k = 1; k < N; ++k)
      {
       FilteredEnds[ end_points_row[i][k] ][ end_points_col[i][k] ] = 1;
      }
    }
  }
  RasterData = FilteredEnds.copy();
}


LSDIndexRaster LSDIndexRaster::filter_by_connected_components(int connected_components_threshold)
{
  LSDIndexRaster ConnectedComponentsRaster = ConnectedComponents();
  Array2D<int> BinaryArray = RasterData.copy();
  vector<int> IDs;
  for(int i = 0; i < NRows; ++i){
    for(int j = 0; j < NCols; ++j){
      if(ConnectedComponentsRaster.get_data_element(i,j) != NoDataValue) IDs.push_back(ConnectedComponentsRaster.get_data_element(i,j));
    }
  }
  vector<size_t> index_map;
  matlab_int_sort(IDs, IDs, index_map);
  int N = IDs.back()+1;
  vector<int> ID(N,0);
  vector<int> count(N,0);
  for(int i = 0; i<N; ++i) ID[i]=i;
  for(int i = 0; i<int(IDs.size()); ++i) ++count[IDs[i]];
  for(int i = 0; i < NRows; ++i){
    for(int j = 0; j < NCols; ++j){
      if(ConnectedComponentsRaster.get_data_element(i,j) != NoDataValue){
  if(count[ConnectedComponentsRaster.get_data_element(i,j)] >= connected_components_threshold){
    BinaryArray[i][j] = 1;
  }
  else BinaryArray[i][j]=0;
      }
    }
  }
  LSDIndexRaster filtered_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,BinaryArray,GeoReferencingStrings);
  return filtered_raster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to convert all values in an LSDIndexRaster to a single value
// SWDG 24/7/15 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::ConvertToBinary(int Value, int ndv){

  Array2D<int> binary(NRows,NCols,NoDataValue);
    
  for(int i = 0; i < NRows; ++i){
    for(int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue && RasterData[i][j] != ndv){
        binary[i][j] = Value;                          
      }                                                                                 
    }
  }
    
  LSDIndexRaster binmask(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,binary,GeoReferencingStrings);
  return binmask;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to remove patches generated by the connected components analysis that are
// smaller than a user defined threshold, minimum_segment_size.
// SWDG 17/9/15 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::RemoveSmallPatches(int minimum_patch_size){

  Array2D<int> PatchIDs = RasterData;

  //Strip out any values that only occur below the minimum_segment_size param value  
  cout << "\tRemoving patches that have an area smaller than " << minimum_patch_size << " pixels." << endl;
  
  //flatten the array to make the counting easier
  vector<int> Flat_Patches = Flatten_Without_Nodata(PatchIDs, NoDataValue);
  
  //get unique patch IDs
  vector<int> Unique_Patches = Unique(PatchIDs, NoDataValue);
  
  //get number of instances of each value in the vector as a map
  map<int,int> Counts; 
  Count_Instances(Flat_Patches,Unique_Patches,Counts);
  
  //loop over map, get vector of keys where value < user defined limit and store patchIDs to be removed as vector  
  vector<int> PatchesToRemove;
  vector<int> PatchesToShorten;
  
  for (int w = 0; w< int(Unique_Patches.size());++w){
  
    if (Counts[Unique_Patches[w]] < minimum_patch_size){
      PatchesToRemove.push_back(Unique_Patches[w]);
    }
  }
  
  //need to handle a vector of zero length (eg all patches are long enough)
  if (!PatchesToRemove.empty()){
  
    //loop over PatchIDs, checking each value for membership in the vector of values to be removed, and if true, set cell value to NDV
    for(int i = 1; i < NRows-1; ++i){
      for(int j = 1; j < NCols-1; ++j){
      
        if (PatchIDs[i][j] != NoDataValue){
          
          if (find(PatchesToRemove.begin(), PatchesToRemove.end(), PatchIDs[i][j]) != PatchesToRemove.end()){
            //the PatchID has been marked for removal, so change it to NDV
            PatchIDs[i][j] = NoDataValue;  
          }
        }        
      }        
    }
    
    
  }
  else{
    cout << "\t\tNo patches below the threshold." << endl;
  }
  
  //create the final LSDIndexRaster and return it
  LSDIndexRaster Patches(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,PatchIDs);
  return Patches;  

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to remove small holes in patches from an integer raster (at the moment set to run on
// a raster made up of 0s, 1s, and 2s).
// FJC 22/10/15
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::remove_holes_in_patches(int window_radius)
{
  int pixel_radius = int(window_radius/DataResolution);
  if (window_radius < DataResolution) window_radius = DataResolution;
  Array2D<int> RasterArray = RasterData;

  Array2D<int> FilledRaster(NRows, NCols, 0);
  //search the neighbours of each pixel for 0 values within the window radius
  for (int row = 0; row < NRows; row ++)
  {
    for (int col = 0; col < NCols; col++)
    {
      if (RasterArray[row][col] == 1) FilledRaster[row][col] = 1;
      if (RasterArray[row][col] == 0)
      {
        vector<int> counts(8,0);
        for (int i = 1; i <= pixel_radius; i++)
        {
          //set exceptions for first or last row
          int min_row = row-i;
          int max_row = row+i;
          if (min_row < 0) min_row = 0;
          if (max_row >= NRows) max_row = NRows-1;
          
          //set exceptions for first or last col
          int min_col = col-i;
          int max_col = col+i;
          if (min_col < 0) min_col = 0;
          if (max_col >= NCols) max_col = NCols-1;
          
          //check whether surrounding pixels in all directions are equal to 0
          if (RasterArray[min_row][min_col]  == 1) counts.at(0) = 1; 
          if (RasterArray[row][min_col]  == 1) counts.at(1) = 1; 
          if (RasterArray[max_row][min_col]  == 1) counts.at(2) = 1; 
          if (RasterArray[min_row][col]  == 1) counts.at(3) = 1; 
          if (RasterArray[min_row][max_col]  == 1) counts.at(4) = 1; 
          if (RasterArray[row][max_col]  == 1) counts.at(5) = 1; 
          if (RasterArray[max_row][max_col]  == 1) counts.at(6) = 1; 
          if (RasterArray[max_row][col] == 1) counts.at(7) = 1; 
          
          // if 1s surround the pixel, then fill in the pixel
          if (counts.at(0) > 0 && counts.at(1) > 0 && counts.at(2) > 0 && counts.at(3) > 0 && counts.at(4) > 0 && counts.at(5) > 0 && counts.at(6) > 0 && counts.at(7) > 0) 
          {
            FilledRaster[row][col] = 1;
            i = pixel_radius+1;
          } 
        }
      }
      if (RasterArray[row][col] == 2) FilledRaster[row][col] = 2;
    }
  }
  
  //create new LSDIndexRaster with the filled patches
  LSDIndexRaster FilledPatches(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilledRaster,GeoReferencingStrings);
  return FilledPatches;  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to remove checkerboard pattern from an integer raster (at the moment set to run on
// a raster made up of 0s and 1s).
// FJC 22/10/15
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDIndexRaster::remove_checkerboard_pattern()
{
  Array2D<int> RasterArray = RasterData;
  Array2D<int> FilledArray(NRows, NCols, 0);
  int count = 0;
  //search the neighbours of each pixel for 1 values within the window radius
  for (int row = 0; row < NRows; row ++)
  {
    for (int col = 0; col < NCols; col++)
    {
      if (RasterArray[row][col] == 1) FilledArray[row][col] = 1;
      if (RasterArray[row][col] == 0)
      {    
        // set exceptions for end rows and cols
        int min_row = row-1;
        int max_row = row+1;
        if (min_row < 0) min_row = 0;
        if (max_row >= NRows) max_row = NRows-1;
        
        int min_col = col-1;
        int max_col = col+1;
        if (min_col < 0) min_col = 0;
        if (max_col >= NCols) max_col = NCols-1;
        
        // check whether N, S, E, and W pixels are equal to 1
        if (RasterArray[min_row][col]  == 1) count++;               //north
        if (RasterArray[max_row][col] == 1) count++;               //south
        if (RasterArray[row][min_col]  == 1) count++;               //east
        if (RasterArray[row][max_col] == 1) count++;                //west
        if (count == 4)
        {
          //fill in pixel
          FilledArray[row][col] = 1;
          cout << "Filled in pixel, woohoo" << endl;
        }    
        count = 0;    
      }
    }
  }
  
  //create new LSDIndexRaster with the filled patches
  LSDIndexRaster FilledRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilledArray,GeoReferencingStrings);
  return FilledRaster; 
  
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#endif
