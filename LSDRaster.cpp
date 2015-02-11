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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSDRaster.cpp
// cpp file for the LSDRaster object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
// David T. Milodowski, University of Edinburgh
// Martin D. Hurst, British Geological Survey
// Fiona Clubb, University of Edinburgh
// Stuart Grieve, University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Version 1.0.0		16/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// change log
// MASSIVE MERGE: Starting version 1.0.0 on 15/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <math.h>
#include <string.h>
#include "TNT/tnt.h"
#include "TNT/jama_lu.h"
#include "TNT/jama_eig.h"
#include "LSDRaster.hpp"
#include "LSDStatsTools.hpp"
#include "LSDIndexRaster.hpp"
#include "LSDShapeTools.hpp"
using namespace std;
using namespace TNT;
using namespace JAMA;

#ifndef LSDRaster_CPP
#define LSDRaster_CPP

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// operators
// SMM, 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster& LSDRaster::operator=(const LSDRaster& rhs)
 {
  if (&rhs != this)
   {
    create(rhs.get_NRows(),rhs.get_NCols(),rhs.get_XMinimum(),rhs.get_YMinimum(),
           rhs.get_DataResolution(),rhs.get_NoDataValue(),rhs.get_RasterData(),
           rhs.get_GeoReferencingStrings());
   }
  return *this;
 }

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// the create function. This is default and throws an error
// SMM 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::create()
{
  //cout << "LSDRaster line 64 Warning you have an empty LSDRaster!" << endl;
  //exit(EXIT_FAILURE);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this creates a raster using an infile
// SMM 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::create(string filename, string extension)
{
  read_raster(filename,extension);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this creates a raster filled with no data values
// SMM 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::create(int nrows, int ncols, float xmin, float ymin,
            float cellsize, float ndv, Array2D<float> data)
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
    cout << "LSDRaster line 89 dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
  if (RasterData.dim2() != NCols)
  {
    cout << "LSDRaster line 94 dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Like the above function, but copies the GeoReferencing
// SMM 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::create(int nrows, int ncols, float xmin, float ymin,
           float cellsize, float ndv, Array2D<float> data, map<string,string> temp_GRS)
{
  NRows = nrows;
  NCols = ncols;
  XMinimum = xmin;
  YMinimum = ymin;
  DataResolution = cellsize;
  NoDataValue = ndv;
  GeoReferencingStrings = temp_GRS;

  RasterData = data.copy();

  if (RasterData.dim1() != NRows)
  {
    cout << "LSDRaster line 89 dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
  if (RasterData.dim2() != NCols)
  {
    cout << "LSDRaster line 94 dimension of data is not the same as stated in NRows!" << endl;
    exit(EXIT_FAILURE);
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function reads a DEM
// One has to provide both the filename and the extension
// the '.' between the filename and extension is not included
// for example, if the full filename is test.asc
// then
// filename = "test"
// and
// ext = "asc"
// The full filename could also be "test.01.asc"
// so filename would be "test.01"
// and ext would again be "asc"
//
// SMM 2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::read_raster(string filename, string extension)
{
  string string_filename;
  string dot = ".";
  string_filename = filename+dot+extension;
  cout << "\n\nLoading an LSDRaster, the filename is " << string_filename << endl;

  if (extension == "asc")
  {
    // open the data file
    ifstream data_in(string_filename.c_str());

    //Read in raster data
    string str;			// a temporary string for discarding text

    // read the georeferencing data and metadata
    data_in >> str >> NCols;
                cout << "NCols: " << NCols << " str: " << endl;
    data_in >> str >> NRows;
    cout << "NRows: " << NRows << " str: " << endl;
    data_in >> str >> XMinimum >> str >> YMinimum
          >> str >> DataResolution
          >> str >> NoDataValue;

    cout << "Loading asc file; NCols: " << NCols << " NRows: " << NRows << endl
         << "X minimum: " << XMinimum << " YMinimum: " << YMinimum << endl
         << "Data Resolution: " << DataResolution << " and No Data Value: "
         << NoDataValue << endl;

    // this is the array into which data is fed
    Array2D<float> data(NRows,NCols,NoDataValue);

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
      ifs >> str >> NCols;
      //cout << "NCols: " << NCols << " str: " << endl;
      ifs >> str >> NRows;
      //cout << "NRows: " << NRows << " str: " << endl;
      ifs >> str >> XMinimum >> str >> YMinimum
          >> str >> DataResolution
          >> str >> NoDataValue;
    }
    ifs.close();

    cout << "Loading flt file; NCols: " << NCols << " NRows: " << NRows << endl
         << "X minimum: " << XMinimum << " YMinimum: " << YMinimum << endl
         << "Data Resolution: " << DataResolution << " and No Data Value: "
         << NoDataValue << endl;

    // this is the array into which data is fed
    Array2D<float> data(NRows,NCols,NoDataValue);

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
          data[i][j] = float(temp);
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
    int DataType = 4;     // default is float data

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
            
            // get Y minium
            YMinimum = YMax - NRows*DataResolution;	          
            
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
    Array2D<float> data(NRows,NCols,NoDataValue);

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
            data[i][j] = float(temp);
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
            
            data[i][j] = float(temp);
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
             << "If you get a crazy DEM go to LINE 625 of LSDRaster.cpp to debug" << endl;
        float temp;   // might need to change this
        //cout << "Float size: " << sizeof(temp) << endl;
        for (int i=0; i<NRows; ++i)
        {
          for (int j=0; j<NCols; ++j)
          {
            // Use data type to control the bytes being read for each entry
            ifs_data.read(reinterpret_cast<char*>(&temp), DataType);
            
            data[i][j] = float(temp);
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
          << "You entered: " << extension << " options are .flt, .asc and .bil" << endl;
    exit(EXIT_FAILURE);
  }


}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// write_raster
// this function writes a raster. One has to give the filename and extension
// currently the options are for .asc and .flt files
//
// SMM 2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::write_raster(string filename, string extension)
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

    data_out <<  "ncols\t" << NCols
       << "\nnrows\t" << NRows
       << "\nxllcorner\t" << setprecision(14) << XMinimum
       << "\nyllcorner\t" << setprecision(14) << YMinimum
       << "\ncellsize\t" << DataResolution
       << "\nNODATA_value\t" << NoDataValue << endl;


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
    header_ofs <<  "ENVI" << endl;
    header_ofs << "description = {" << endl << this_fname << "}" << endl;
    header_ofs <<  "samples = " << NCols << endl;
    header_ofs <<  "lines = " << NRows << endl;
    header_ofs <<  "bands = 1" << endl;
    header_ofs <<  "header offset = 0" << endl;
    header_ofs <<  "file type = ENVI Standard" << endl;
    header_ofs <<  "data type = 4" << endl;
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
  else
  {
    cout << "You did not enter and approprate extension!" << endl
    << "You entered: " << extension << " options are flt, bil and asc" << endl;
    exit(EXIT_FAILURE);
   }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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
map<string, string> LSDRaster::Update_GeoReferencingStrings(float NewXmin, float NewYmax)
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
void LSDRaster::Update_GeoReferencingStrings()
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
void LSDRaster::impose_georeferencing_UTM(int zone, string NorS)
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
// This function returns the x and y location of a row and column
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc)
{
  
  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
    
  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Function to convert a node position with a row and column to a lat
// and long coordinate
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::get_lat_and_long_locations(int row, int col, double& lat, 
                   double& longitude, LSDCoordinateConverterLLandUTM Converter)
{
  // get the x and y locations of the node
  float x_loc,y_loc;
  get_x_and_y_locations(row, col, x_loc, y_loc);
  
  // get the UTM zone of the node
  int UTM_zone;
  bool is_North;
  get_UTM_information(UTM_zone, is_North);
  //cout << endl << endl << "Line 1034, UTM zone is: " << UTM_zone << endl;
  
  
  if(UTM_zone == NoDataValue)
  {
    lat = NoDataValue;
    longitude = NoDataValue;
  }
  else
  {
    // set the default ellipsoid to WGS84
    int eId = 22;
  
    double xld = double(x_loc);
    double yld = double(y_loc);
  
    // use the converter to convert to lat and long
    double Lat,Long;
    Converter.UTMtoLL(eId, yld, xld, UTM_zone, is_North, Lat, Long);
          
  
    lat = Lat;
    longitude = Long;
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function gets the UTM zone
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::get_UTM_information(int& UTM_zone, bool& is_North)
{

  // set up strings and iterators
  map<string,string>::iterator iter;

  //check to see if there is already a map info string
  string mi_key = "ENVI_map_info";
  iter = GeoReferencingStrings.find(mi_key);
  if (iter != GeoReferencingStrings.end() )
  {
    string info_str = GeoReferencingStrings[mi_key] ;

    // now parse the string
    vector<string> mapinfo_strings;
    istringstream iss(info_str);
    while( iss.good() )
    {
      string substr;
      getline( iss, substr, ',' );
      mapinfo_strings.push_back( substr );
    }
    UTM_zone = atoi(mapinfo_strings[7].c_str());
    //cout << "Line 1041, UTM zone: " << UTM_zone << endl;
    //cout << "LINE 1042 LSDRaster, N or S: " << mapinfo_strings[7] << endl;
    
    // find if the zone is in the north
    string n_str = "n";
    string N_str = "N";
    is_North = false;
    size_t found = mapinfo_strings[8].find(N_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    found = mapinfo_strings[8].find(n_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    //cout << "is_North is: " << is_North << endl;
        
  }
  else
  {
    UTM_zone = NoDataValue;
    is_North = false;
  }
  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This is a utility function to find the central meridian of a UTM zone
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int LSDRaster::Find_UTM_central_meridian(int UTM_zone)
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
bool LSDRaster::check_if_point_is_in_raster(float X_coordinate,float Y_coordinate)
{
  bool is_in_raster = true;
  
  // Shift origin to that of dataset
  float X_coordinate_shifted_origin = X_coordinate - XMinimum;
  float Y_coordinate_shifted_origin = Y_coordinate - YMinimum;

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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// rewrite_with_random_values
// This overwrites existing data with random values
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::rewrite_with_random_values(float range)
{
  // set a seed for the random values
  long seed = time(NULL);

  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      RasterData[row][col] = ran3(&seed)*range;
    }
  }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Simple topographic metrics
// Several simple topographic metrics measuered over a kernal
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Calcualte relief
// This calculates relief over a window. 
// Right now it can have a circular window with kernalType == 1 or square otherwise
//
// Written by JAJ 6-6-2014
// Inserted into trunk by SMM 9-6-2014
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::calculate_relief(float kernelWidth, int kernelType)
{
	int kr = ((kernelWidth/DataResolution))/2-1;
	float max, min;
	Array2D <float> reliefMap(NRows, NCols, 0.0);
	if (kr < 1)
	{
		kernelWidth = 2.5;
		kr = 1;
	}
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			max = RasterData[i][j];
			min = RasterData[i][j];
			for (int sub_i = i-kr; sub_i<=i+kr; ++sub_i)
			{
				if (sub_i<0 || sub_i>=NRows) continue;
				for (int sub_j = j-kr; sub_j<=j+kr; ++sub_j)
				{
					if (sub_j<0 || sub_j>=NCols) continue;
					if (RasterData[sub_i][sub_j] == NoDataValue) continue;

					if (kernelType == 1)	//circular
						if ((pow(sub_i-i,2) + pow(sub_j-j,2))*DataResolution > kernelWidth/2)
							continue;
					if (RasterData[sub_i][sub_j] > max)
						max = RasterData[sub_i][sub_j];
					if (RasterData[sub_i][sub_j] < min)
						min = RasterData[sub_i][sub_j];
				}
			}
			reliefMap[i][j] = max-min;
		}
	}
	return LSDRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                   NoDataValue, reliefMap, GeoReferencingStrings);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Return the mean elevation of the raster
// JAJ, sometime in February 2014
// modified SMM to make sure division by float 9/6/2014
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
float LSDRaster::mean_elevation(void)
{
	float sum_elevation = 0;
	int n = 0;
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			if (RasterData[i][j] != NoDataValue)
			{
				sum_elevation += RasterData[i][j];
				++n;
			}
		}
	}
	return float(sum_elevation/float(n));
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Return the maximum elevation of the raster
// JAJ, sometime in February 2014
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
float LSDRaster::max_elevation( void )
{
  float max_elevation = 0.0;
  bool found=false;
  for (int i=0; i<NRows; ++i)
  {
    for (int j=0; j<NCols; ++j)
    {
      if (not found)
      {
        if (RasterData[i][j] != NoDataValue)
        {
          max_elevation = RasterData[i][j];
          found = true;
        }
      }
      else if (RasterData[i][j] > max_elevation)
      {
        max_elevation = RasterData[i][j];
      }
    }
  }
  return max_elevation;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Return the mean relief of the raster
// JAJ, sometime in February 2014
// modified SMM to make sure division by float 9/6/2014
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
float LSDRaster::mean_relief(float kernelWidth)
{
	LSDRaster relief = calculate_relief(kernelWidth, 1);
	float relief_val, sum_relief = 0;
	int n = 0;

	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			relief_val = relief.get_data_element(i, j);
			if (relief_val != NoDataValue)
			{
				sum_relief += relief_val;
				++n;
			}
		}
	}
	return float(sum_relief/float(n));
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// /\ |  |  DIAMOND SQUARE
// \/ |__|
// Routines for the Diamond Square algorithm
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function allows one to sample across values in the raster while wrapping
// around the sides of the raster
// SMM 9/1/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float LSDRaster::WrapSample(int row, int col)
{

	int wraprow;
	int wrapcol;


	if(row >=0)
	{
		wraprow = row % NRows;
	}
	else
	{
		wraprow = NRows - (-row % NRows);
	}
	if(col >= 0)
	{
		wrapcol = col % NCols;
	}
	else
	{
		wrapcol = NCols - (-col % NCols);
	}

	//cout << "Nrows: " << NRows << " row: " << row << " wraprow: " << wraprow << endl;
	//cout << "Ncols: " << NCols << " col: " << col << " wrapcol: " << wrapcol << endl;

	return RasterData[wraprow][wrapcol];
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function allows one to set values in the raster while wrapping
// around the sides of the raster
// SMM 9/1/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::SetWrapSample(int row, int col, float value)
{
	int wraprow;
	int wrapcol;

	if(row >=0)
	{
		wraprow = row % NRows;
	}
	else
	{
		wraprow = NRows - (-row % NRows);
	}
	if(col >= 0)
	{
		wrapcol = col % NCols;
	}
	else
	{
		wrapcol = NCols - (-col % NCols);
	}

	RasterData[wraprow][wrapcol] = value;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function start with the corners of the algorithm to make individual 'features'
// note: the scale gives the range, so it should be a random value between -scale*0.5 and scale*0.5
// SMM 9/1/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::DSSetFeatureCorners(int featuresize, float scale)
{
	long seed = time(NULL);
	// the function starts from -featuresize since in the diamond square step
	// of the algorithm it wraps from row and column 0
	for (int row = 0; row < NRows; row+= featuresize)
	{
		for (int col = 0; col<NCols; col+= featuresize)
		{
			float randn = (ran3(&seed)-0.5)*scale;
			//cout << "setting feature corners, row: " << row << " and col: " << col << endl;
			SetWrapSample(row,col,randn);
		}
	}
	//cout << "Set the feature corners" << endl;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function samples on the square: 
// a     b
//
//    x
//
// c     d
// The corners are known and the centre is calculated
// SMM 9/1/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::DSSampleSquare(int row,int col, int size, float value)
{
    int hs = size / 2;

    float a = WrapSample(col - hs, row - hs);
    float b = WrapSample(col + hs, row - hs);
    float c = WrapSample(col - hs, row + hs);
    float d = WrapSample(col + hs, row + hs);

    if (a == -9999 || b == -9999 || c == -9999 || d == -9999)
    {
		  cout << "got a nodata; row: " << row << " and col: " << col << endl;

		  if (a == -9999)
		  {
			  cout << "col - hs: " << col - hs << " and row - hs: " << row - hs << endl;
		  }
		  if (b == -9999)
		  {
			  cout << "col + hs: " << col + hs << " and row - hs: " << row - hs << endl;
		  }
		  if (c == -9999)
		  {
			  cout << "col - hs: " << col - hs << " and row + hs: " << row + hs << endl;
		  }
		  if (d == -9999)
		  {
			  cout << "col + hs: " << col + hs << " and row + hs: " << row + hs << endl;
		  }
	  }

    SetWrapSample(row, col, ((a + b + c + d) / 4.0) + value);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This calculates the diamond step
//   c
//
//a  x  b
//
//   d
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::DSSampleDiamond(int row, int col, int size, float value)
{
    int hs = size / 2;

    double a = WrapSample(col - hs, row);
    double b = WrapSample(col + hs, row);
    double c = WrapSample(col, row - hs);
    double d = WrapSample(col, row + hs);

    if (a == -9999 || b == -9999 || c == -9999 || d == -9999)
    {
		  cout << "got a nodata DSSampleDiamond; row: " << row << " and col: " << col << endl;

		  if (a == -9999)
		  {
			  cout << "col - hs: " << col - hs << " and row: " << row << endl;
		  }
		  if (b == -9999)
		  {
			  cout << "col + hs: " << col + hs << " and row: " << row << endl;
		  }
		  if (c == -9999)
		  {
			  cout << "col: " << col << " and row - hs: " << row - hs << endl;
		  }
		  if (d == -9999)
		  {
			  cout << "col: " << col << " and row + hs: " << row + hs << endl;
		  }

	  }

    SetWrapSample(row, col, ((a + b + c + d) / 4.0) + value);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// this function performs the core of the diamond square algorithm
// the 'features' have got to be of size 2^n+1, so the
// feature order is the 'n'. THis means that, say a feature order of 4
// will make a feature with size of 17
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::DiamondSquare_SampleStep(int stepsize, float scale)
{
    int halfstep = stepsize / 2;
    long seed = time(NULL);

	// first do the square step. This gets the sqare for the node
	// at the half distance between the starting points
    for (int row = -halfstep; row < NRows+halfstep; row += stepsize)
    {
      for (int col = -halfstep; col < NCols + halfstep; col += stepsize)
      {
			  //cout << "SS row and col: " << row << " " << col << endl;
	      DSSampleSquare(row, col, stepsize, ((ran3(&seed)-0.5) * scale));
      }
    }

	// now the DiamondStep.
	// The first diamond gets the point halfway below the first column
	// Second diamond gets the column halfway past the first row.
	// That means for col 0 and row 0 it will wrap to the other side
	// This means these values will need to be wrapped in the inital
	// set corners stage!
    for (int row = -stepsize; row < NRows; row += stepsize)
    {
      for (int col = -stepsize; col < NCols; col += stepsize)
      {
			  //cout << "DS row and col: " << row << " " << col << endl;
	      DSSampleDiamond(row + halfstep, col, stepsize, ((ran3(&seed)-0.5) * scale));
	      DSSampleDiamond(row, col + halfstep, stepsize, ((ran3(&seed)-0.5) * scale));
      }
    }

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// this is the diamond square algorithm
// it creates a resized diamond square pseudo-fractal raster
// it has the same xllcorner and yllcorner as the original raster, 
// but is resized so the NRows and NCols are to the closed power of 2
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::DiamondSquare(int feature_order, float scale)
{

    // determine the size of the padded array:
    int PaddedRows = int(pow(2,ceil(log(NRows)/log(2))));
    int PaddedCols = int(pow(2,ceil(log(NCols)/log(2))));

    // now get the maximum feature size. This will be the  minimum
    // power of 2 in the x or y direction
    int max_feature_size;
    if (PaddedRows >= PaddedCols)
    {
      max_feature_size = PaddedCols;
    }
    else
    {
      max_feature_size = PaddedRows;
    }

    cout << "NRows: " << NRows << " and PaddedRows: " << PaddedRows << endl;
    cout << "NCols: " << NCols << " and PaddedCols: " << PaddedCols << endl;
    cout << "max_feature_size: " << max_feature_size << endl;

    //create an array
    Array2D<float> DSRaster_array(PaddedRows,PaddedCols,NoDataValue);

    //create LSDRaster diamond square object
    LSDRaster DSRaster(PaddedRows, PaddedCols, XMinimum, YMinimum, DataResolution,
                               NoDataValue, DSRaster_array, GeoReferencingStrings);

    // get the feature size: it must be a power of 2.
    int featuresize = pow(2,feature_order);
    if (featuresize > max_feature_size)
    {
      cout << "Your featuresize is too big for the DEM. Changing to max feature size" << endl;
    }

    cout << "feature size is: " << featuresize << endl;

    // now initialize the raster over some feature scale
    DSRaster.DSSetFeatureCorners(featuresize, scale);

    // now loop through the features, running the diamond square algorithm.
    int samplesize = featuresize;
    scale = scale/2;

	  cout << "Starting diamond square algorithm, samplesize is: " << samplesize << endl;

    while (samplesize > 1)
    {
		  cout << "Running Diamond square, samplesize: " << samplesize << endl;

      DSRaster.DiamondSquare_SampleStep(samplesize,scale);

      // halve the sample size and scale
      samplesize = samplesize/2;
      scale = scale/2;
    }

    return DSRaster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Make LSDRaster object using a 'template' raster and an Array2D of data.
// SWDG 29/8/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::LSDRasterTemplate(Array2D<float> InputData){

  //do a dimensions check and exit on failure
  if (InputData.dim1() == NRows && InputData.dim2() == NCols){
    LSDRaster OutputRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                           NoDataValue, InputData,GeoReferencingStrings);
    return OutputRaster;
  }
  else{
   	cout << "Array dimensions do not match template LSDRaster object" << endl;
		exit(EXIT_FAILURE);
  }

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a hillshade raster using the algorithm outlined in
// Burrough and McDonnell Principles of GIS 1990 and in the ArcMap web help
// http://edndoc.esri.com/arcobjects/9.2/net/shared/geoprocessing/
// spatial_analyst_tools/how_hillshade_works.htm
//
// Takes 3 floats, representing the altitude of the illumination source in
// degrees, the azimuth of the illumination source in degrees and the z factor.
//
// Default values are altitude = 45, azimuth = 315, z_factor = 1
//
// Outputs an LSDRaster object.
//
// SWDG, February 2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::hillshade(float altitude, float azimuth, float z_factor)
{
    float PI = 3.14159265;

    //print parameters to screen
    cout << "Hillshading with altitude: " << altitude
    << ", azimuth: " << azimuth << " and z-factor: " << z_factor << endl;

    //create output array
    Array2D<float> hillshade(NRows,NCols,NoDataValue);

    //convert zenith and azimuth into radians for calculation
    float zenith_rad = (90 - altitude) * PI / 180.0;
    float azimuth_math = 360-azimuth + 90;
    if (azimuth_math >= 360.0) azimuth_math = azimuth_math - 360;
    float azimuth_rad = azimuth_math * PI /180.0;

    //calculate hillshade value for every non nodata value in the input raster
    for (int i = 1; i < NRows-1; ++i){
        for (int j = 1; j < NCols-1; ++j){
            float slope_rad = 0;
            float aspect_rad = 0;
            float dzdx = 0;
            float dzdy = 0;

            if (RasterData[i][j] != NoDataValue){
                dzdx = ((RasterData[i][j+1] + 2*RasterData[i+1][j] + RasterData[i+1][j+1]) -
                       (RasterData[i-1][j-1] + 2*RasterData[i-1][j] + RasterData[i-1][j+1]))
                        / (8 * DataResolution);
                dzdy = ((RasterData[i-1][j+1] + 2*RasterData[i][j+1] + RasterData[i+1][j+1]) -
                       (RasterData[i-1][j-1] + 2*RasterData[i][j-1] + RasterData[i+1][j-1]))
                       / (8 * DataResolution);

                slope_rad = atan(z_factor * sqrt((dzdx*dzdx) + (dzdy*dzdy)));

                if (dzdx != 0){
                    aspect_rad = atan2(dzdy, (dzdx*-1));
                    if (aspect_rad < 0) aspect_rad = 2*PI + aspect_rad;
                }
                else{
                    if (dzdy > 0) aspect_rad = PI/2;
                    else if (dzdy < 0) aspect_rad = 2 * PI - PI/2;
                    else aspect_rad = aspect_rad;
                }
                hillshade[i][j] = 255.0 * ((cos(zenith_rad) * cos(slope_rad)) +
                                  (sin(zenith_rad) * sin(slope_rad) *
                                  cos(azimuth_rad - aspect_rad)));

                if (hillshade[i][j] < 0) hillshade[i][j] = 0;
            }
        }
    }
    //create LSDRaster hillshade object
    LSDRaster hillshade_raster(NRows, NCols, XMinimum, YMinimum, DataResolution,
                               NoDataValue, hillshade,GeoReferencingStrings);

    return hillshade_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a hillshade derivative raster using the algorithm outlined in
// Codilean (2006), identifying areas in shadow as 1 and all other values as 0.
//
// Is interfaced through LSDRaster::TopoShield and should not be called directly,
// to generate a hillshade use LSDRaster::hillshade instead.
//
// Takes 2 ints, representing theta, the zenith angle of the illumination source in
// degrees and the azimuth angle, phi, of the illumination source in degrees.
//
// Outputs an Array2D of floats.
//
// SWDG, 11/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<float> LSDRaster::Shadow(int theta, int phi)
{
    float PI = 3.14159265;

    //create array of input data and an output array of same dimensions
    Array2D<float> data = RasterData;
    Array2D<float> hillshade(NRows,NCols,NoDataValue);

    //convert zenith and azimuth into radians for calculation
    float zenith_rad = (90 - theta) * PI / 180.0;
    float azimuth_math = 360-phi + 90;
    if (azimuth_math >= 360.0) azimuth_math = azimuth_math - 360;
    float azimuth_rad = azimuth_math * PI /180.0;

    //calculate hillshade value for every non nodata value in the input raster
    for (int i = 1; i < NRows-1; ++i){
        for (int j = 1; j < NCols-1; ++j){
            float slope_rad = 0;
            float aspect_rad = 0;
            float dzdx = 0;
            float dzdy = 0;

            if (data[i][j] != NoDataValue){
                dzdx = ((data[i][j+1] + 2*data[i+1][j] + data[i+1][j+1]) -
                       (data[i-1][j-1] + 2*data[i-1][j] + data[i-1][j+1]))
                        / (8 * DataResolution);
                dzdy = ((data[i-1][j+1] + 2*data[i][j+1] + data[i+1][j+1]) -
                       (data[i-1][j-1] + 2*data[i][j-1] + data[i+1][j-1]))
                       / (8 * DataResolution);

                slope_rad = atan(sqrt((dzdx*dzdx) + (dzdy*dzdy)));

                if (dzdx != 0){
                    aspect_rad = atan2(dzdy, (dzdx*-1));
                    if (aspect_rad < 0) aspect_rad = 2*PI + aspect_rad;
                }
                else{
                    if (dzdy > 0) aspect_rad = PI/2;
                    else if (dzdy < 0) aspect_rad = 2 * PI - PI/2;
                    else aspect_rad = aspect_rad;
                }

                hillshade[i][j] = acos(((sin(zenith_rad) * cos(slope_rad)) +
                                  (sin(zenith_rad) * sin(slope_rad) *
                                  cos(azimuth_rad - aspect_rad))))*180/PI;

                if (hillshade[i][j] > 90) hillshade[i][j] = 1;
                else hillshade[i][j] = 0;
            }

        }
    }

    //create LSDRaster hillshade object
    //LSDRaster hillshade_raster(NRows, NCols, XMinimum, YMinimum, DataResolution,
    //                           NoDataValue, hillshade);

    return hillshade;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function generates a topographic sheilding raster using the algorithm outlined in
// Codilean (2006), creating a raster of values between 0 and 1 whihc can be used as a
// scaling factor in Cosmo analysis.
//
// Goes further than the original algorithm allowing a theoretical theta, phi pair of
// 1,1 to be supplied and although this will increase the computatin time significantly,
// it is much faster than the original Avenue and VBScript implementations.
//
// Takes 2 ints, representing the theta, phi paring required. Codilean (2006) used 5,5
// as the standard values, but in reality values of 10,15 are often preferred to save
// processing time. **phi_step must be a factor of 360**
//
// Outputs an LSDRaster
//
// SWDG, 11/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::TopoShield(int theta_step, int phi_step)
{
  //calculate parameter of scaling calculation
  int phi_factor = 360/phi_step;

  float MaxFactor = 0;

  for(int theta = 10; theta < 90; theta += theta_step){
    MaxFactor += pow(sin(rad(float(theta))),3.3) * phi_factor;
  }

  //calculate maximum scaling factor and write it to an array
  float f90 = 90.0;
  MaxFactor += pow(sin(rad(f90)),3.3);
  Array2D<float> MaxFactorArray(NRows,NCols,MaxFactor);

  //Calculate first shadow with theta value of 90 and scale it
  Array2D<float> Scaler90(NRows,NCols, pow(sin(rad(f90)),3.3));
  Array2D<float> FinalArray = Shadow(0,90) * Scaler90;

  //loop through all the theta, phi pairs and increment the FinalArray with the scaled values
  for(int theta = 10; theta < 90; theta += theta_step)
  {
    for(int phi = 0; phi < 360; phi += phi_step)
    {
      cout << flush <<  "\tTheta = " << theta << ", Phi = " 
           << phi << "           \r";

      Array2D<float> TempArray = Shadow(theta,phi);
      Array2D<float> Scaler(NRows, NCols, pow(sin(rad(float(theta))),3.3));
      FinalArray += TempArray * Scaler;
    }
  }

 //create array of ones needed in sheilding calculation
  Array2D<float> Ones(NRows,NCols,1);

  //Shielding factor calculation
  Array2D<float> ShFactor = Ones - (FinalArray/MaxFactorArray);

  //deal with nodata values on border of dem - more efficient than doing it in the hillshade function 10s of times.
  for (int i = 0; i < NRows; ++i)
  {
    for (int j = 0; j < NCols; ++j)
    {
      if (ShFactor[i][j] > 10)
      {             //arbitrary number. But don't want to miss any values >1
        ShFactor[i][j] = NoDataValue;       //as they may indicate an error in the above calculations.
      }
    }
  }

   //flush output onto a new line
   cout << endl;

  //write LSDRaster
  LSDRaster Shielding(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue,
                      ShFactor,GeoReferencingStrings);
  return Shielding;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=






//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// calculate_polyfit_surface_metrics
//
// This routine houses the code to fit a 6 term polynomial (z =ax^2 + by^2 + cxy
// + dx + ey + f) to a surface, and then use the derivatives of this to
// calculate various useful geometric properties such as slope and curvature.
// 
// The surface is fitted to all the points that lie within circular
// neighbourhood that is defined by the designated window radius.  The user also
// inputs a binary raster, which tells the program which rasters it wants to
// create (label as "true" to produce them, "false" to ignore them. This has 8
// elements, as listed below:
//        0 -> Elevation (smoothed by surface fitting)
//        1 -> Slope
//        2 -> Aspect
//        3 -> Curvature
//        4 -> Planform Curvature
//        5 -> Profile Curvature
//        6 -> Tangential Curvature
//        7 -> Stationary point classification (1=peak, 2=depression, 3=saddle)
// The program returns a vector of LSDRasters.  For options marked "false" in
// boolean input raster, the returned LSDRaster houses a blank raster, as this
// metric has not been calculated.  The desired LSDRaster can be retrieved from
// the output vector by using the cell reference shown in the list above i.e. it
// is the same as the reference in the input binary vector.
//
// DTM 28/03/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
vector<LSDRaster> LSDRaster::calculate_polyfit_surface_metrics(float window_radius, vector<int> raster_selection)
{
  Array2D<float> void_array(1,1,NoDataValue);
  LSDRaster VOID(1,1,NoDataValue,NoDataValue,NoDataValue,NoDataValue,void_array,GeoReferencingStrings);  
  
  // catch if the supplied window radius is less than the data resolution and
  // set it to equal the data resolution - SWDG
  if (window_radius < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }
  // this fits a polynomial surface over a kernel window. First, perpare the
  // kernel
  int kr = int(ceil(window_radius/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                                     // width of kernel

  Array2D<float> data_kernel(kw,kw,NoDataValue);
  Array2D<float> x_kernel(kw,kw,NoDataValue);
  Array2D<float> y_kernel(kw,kw,NoDataValue);
  Array2D<int> mask(kw,kw,0);

  // reset the a,b,c,d,e and f matrices (the coefficient matrices)
  Array2D<float> temp_coef(NRows,NCols,NoDataValue);
  Array2D<float> elevation_raster, slope_raster, aspect_raster, curvature_raster, planform_curvature_raster, 
                  profile_curvature_raster, tangential_curvature_raster, classification_raster, 
                  s1_raster, s2_raster, s3_raster;
  // Copy across raster template into the desired array containers
  if(raster_selection[0]==1)  elevation_raster = temp_coef.copy();
  if(raster_selection[1]==1)  slope_raster = temp_coef.copy();
  if(raster_selection[2]==1)  aspect_raster = temp_coef.copy();
  if(raster_selection[3]==1)  curvature_raster = temp_coef.copy();
  if(raster_selection[4]==1)  planform_curvature_raster = temp_coef.copy();
  if(raster_selection[5]==1)  profile_curvature_raster = temp_coef.copy();
  if(raster_selection[6]==1)  tangential_curvature_raster = temp_coef.copy();
  if(raster_selection[7]==1)  classification_raster = temp_coef.copy();
  
  //float a,b,c,d,e,f;
  
  // scale kernel window to resolution of DEM, and translate coordinates to be
  // centred on cell of interest (the centre cell)
  float x,y,zeta,radial_dist;
  for(int i=0;i<kw;++i)
  {
    for(int j=0;j<kw;++j)
    {
      x_kernel[i][j]=(i-kr)*DataResolution;
      y_kernel[i][j]=(j-kr)*DataResolution;
      // Build circular mask
      // distance from centre to this point.
      radial_dist = sqrt(y_kernel[i][j]*y_kernel[i][j] + x_kernel[i][j]*x_kernel[i][j]);

      if (floor(radial_dist) <= window_radius)
      {
        mask[i][j] = 1;
      }
    }
  }
  // FIT POLYNOMIAL SURFACE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO
  // DETERMINE TOPOGRAPHIC METRICS
  // Have N simultaneous linear equations, and N unknowns.
  // => b = Ax, where x is a 1xN array containing the coefficients we need for
  // surface fitting.
  // A is constructed using different combinations of x and y, thus we only need
  // to compute this once, since the window size does not change.
  // For 2nd order surface fitting, there are 6 coefficients, therefore A is a
  // 6x6 matrix
  Array2D<float> A(6,6,0.0);
  for (int i=0; i<kw; ++i)
  {
    for (int j=0; j<kw; ++j)
    {
      if (mask[i][j] == 1)
      {
        x = x_kernel[i][j];
        y = y_kernel[i][j];

        // Generate matrix A
        A[0][0] += pow(x,4);
        A[0][1] += pow(x,2)*pow(y,2);
        A[0][2] += pow(x,3)*y;
        A[0][3] += pow(x,3);
        A[0][4] += pow(x,2)*y;
        A[0][5] += pow(x,2);
        A[1][0] += pow(x,2)*pow(y,2);
        A[1][1] += pow(y,4);
        A[1][2] += x*pow(y,3);
        A[1][3] += x*pow(y,2);
        A[1][4] += pow(y,3);
        A[1][5] += pow(y,2);
        A[2][0] += pow(x,3)*y;
        A[2][1] += x*pow(y,3);
        A[2][2] += pow(x,2)*pow(y,2);
        A[2][3] += pow(x,2)*y;
        A[2][4] += x*pow(y,2);
        A[2][5] += x*y;
        A[3][0] += pow(x,3);
        A[3][1] += x*pow(y,2);
        A[3][2] += pow(x,2)*y;
        A[3][3] += pow(x,2);
        A[3][4] += x*y;
        A[3][5] += x;
        A[4][0] += pow(x,2)*y;
        A[4][1] += pow(y,3);
        A[4][2] += x*pow(y,2);
        A[4][3] += x*y;
        A[4][4] += pow(y,2);
        A[4][5] += y;
        A[5][0] += pow(x,2);
        A[5][1] += pow(y,2);
        A[5][2] += x*y;
        A[5][3] += x;
        A[5][4] += y;
        A[5][5] += 1;
      }
    }
  }

	// Move window over DEM, fitting 2nd order polynomial surface to the
	// elevations within the window.
	cout << "\n\tRunning 2nd order polynomial fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;

  for(int i=0;i<NRows;++i)
  {
    //cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
      {
        if(raster_selection[0]==1)  elevation_raster[i][j] = NoDataValue;
        if(raster_selection[1]==1)  slope_raster[i][j] = NoDataValue;
        if(raster_selection[2]==1)  aspect_raster[i][j] = NoDataValue;
        if(raster_selection[3]==1)  curvature_raster[i][j] = NoDataValue;
        if(raster_selection[4]==1)  planform_curvature_raster[i][j] = NoDataValue;
        if(raster_selection[5]==1)  profile_curvature_raster[i][j] = NoDataValue;
        if(raster_selection[6]==1)  tangential_curvature_raster[i][j] = NoDataValue;
        if(raster_selection[7]==1)  classification_raster[i][j] = NoDataValue;
      }
      else
      {
      // clip DEM
        //zeta_sampler=zeta.copy();
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            data_kernel[i_kernel][j_kernel] = RasterData[i-kr+i_kernel][j-kr+j_kernel];
            // check for nodata values nearby
            if(data_kernel[i_kernel][j_kernel]==NoDataValue)
            {
              ndv_present=1;
            }
          }
        }
        // Fit polynomial surface, avoiding nodata values          ==================> Could change this, as can fit polynomial surface as long as there are 6 data points.
        if(ndv_present == 0)  // test for nodata values within the selection
        {
          Array1D<float> bb(6,0.0);
          Array1D<float> coeffs(6);
          for (int krow=0; krow<kw; ++krow)
          {
            for (int kcol=0; kcol<kw; ++kcol)
            {
              if (mask[krow][kcol] == 1)
              {
                x = x_kernel[krow][kcol];
                y = y_kernel[krow][kcol];
                zeta = data_kernel[krow][kcol];
                // Generate vector bb
                bb[0] += zeta*x*x;
                bb[1] += zeta*y*y;
                bb[2] += zeta*x*y;
                bb[3] += zeta*x;
                bb[4] += zeta*y;
                bb[5] += zeta;
              }		// end mask
            }			// end kernal column
          }				// end kernal row
          // Solve matrix equations using LU decomposition using the TNT JAMA
          // package:
          // A.coefs = b, where coefs is the coefficients vector.
          LU<float> sol_A(A);  // Create LU object
          coeffs = sol_A.solve(bb);

          float a=coeffs[0];
          float b=coeffs[1];
          float c=coeffs[2];
          float d=coeffs[3];
          float e=coeffs[4];
          float f=coeffs[5];

          // Now calculate the required topographic metrics
          if(raster_selection[0]==1)  elevation_raster[i][j] = f;
          
          if(raster_selection[1]==1)  slope_raster[i][j] = sqrt(d*d+e*e);

          if(raster_selection[2]==1)
          {
            if(d==0 && e==0) aspect_raster[i][j] = NoDataValue;
            else if(d==0 && e>0) aspect_raster[i][j] = 90;
            else if(d==0 && e<0) aspect_raster[i][j] = 270;
            else
            {
              aspect_raster[i][j] = 270. - (180./M_PI)*atan(e/d) + 90.*(d/abs(d));
              if(aspect_raster[i][j] > 360.0) aspect_raster[i][j] -= 360;
//    					aspect_raster[i][j] = 180 - 57.29578*atan(e/d) + 90*(d/abs(d));
//    					if(aspect_raster[i][j] < 180.0)
//    					{
//    						aspect_raster[i][j] = 180.0 - aspect_raster[i][j];
//    					}
//    					else
//    					{
//    						aspect_raster[i][j] = 360.0 + (180 - aspect_raster[i][j]);
//    					}
            }
          }
          
          if(raster_selection[3]==1)  curvature_raster[i][j] = 2*a+2*b;
          
          if(raster_selection[4]==1 || raster_selection[5]==1 || raster_selection[6]==1 || raster_selection[7]==1)
          {
            float fx, fy, fxx, fyy, fxy, p, q;
            fx = d;
            fy = e;
            fxx = 2*a;
            fyy = 2*b;
            fxy = c;
            p = fx*fx + fy*fy;
            q = p + 1;
          
            if (raster_selection[4]==1)
            {
              if (q > 0)  planform_curvature_raster[i][j] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/(sqrt(q*q*q));
              else        planform_curvature_raster[i][j] = NoDataValue;
            }
            if(raster_selection[5]==1)
            {
              if((q*q*q > 0) && ((p*sqrt(q*q*q)) != 0))    profile_curvature_raster[i][j] = (fxx*fx*fx + 2*fxy*fx*fy + fyy*fy*fy)/(p*sqrt(q*q*q));
            else                                         profile_curvature_raster[i][j] = NoDataValue;
            }
            if(raster_selection[6]==1)
            {
              if( q>0 && (p*sqrt(q))!=0) tangential_curvature_raster[i][j] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/(p*sqrt(q));
              else                       tangential_curvature_raster[i][j] = NoDataValue;
            }
            if(raster_selection[7]==1)
            {
              float slope = sqrt(d*d + e*e);
              if (slope < 0.1)
              {
                if (fxx < 0 && fyy < 0 && fxy*fxy < fxx*fxx)      classification_raster[i][j] = 1;// Conditions for peak
                else if (fxx > 0 && fyy > 0 && fxy*fxy < fxx*fyy) classification_raster[i][j] = 2;// Conditions for a depression
                else if (fxx*fyy < 0 || fxy*fxy > fxx*fyy)        classification_raster[i][j] = 3;// Conditions for a saddle
                else classification_raster[i][j] = 0;
              }
            }
          }
        }         // end if statement for no data value
        ndv_present = 0;
      }
    }
  }
  // Now create LSDRasters and load into output vector
  vector<LSDRaster> output_rasters_temp(8,VOID);
  vector<LSDRaster> raster_output = output_rasters_temp;
  if(raster_selection[0]==1)
  {
    LSDRaster Elevation(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,elevation_raster,GeoReferencingStrings);
    raster_output[0] = Elevation;
  }
  if(raster_selection[1]==1)  
  {
    LSDRaster Slope(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,slope_raster,GeoReferencingStrings);
    raster_output[1] = Slope;
  }	
  if(raster_selection[2]==1)  
  {
    LSDRaster Aspect(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,aspect_raster,GeoReferencingStrings);
    raster_output[2] = Aspect;
  }  
  if(raster_selection[3]==1)  
  {
    LSDRaster Curvature(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,curvature_raster,GeoReferencingStrings);
    raster_output[3] = Curvature;
  }  
  if(raster_selection[4]==1)  
  {
    LSDRaster PlCurvature(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,planform_curvature_raster,GeoReferencingStrings);
    raster_output[4] = PlCurvature;
  }
  if(raster_selection[5]==1)
  {
    LSDRaster PrCurvature(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,profile_curvature_raster,GeoReferencingStrings);
    raster_output[5] = PrCurvature;
  }
  if(raster_selection[6]==1)  
  {
    LSDRaster TanCurvature(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,tangential_curvature_raster,GeoReferencingStrings);
    raster_output[6] = TanCurvature;
  }  
  if(raster_selection[7]==1)
  {
    LSDRaster Class(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,classification_raster,GeoReferencingStrings);
    raster_output[7] = Class;
  }
  return raster_output;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// calculate_polyfit_roughness_metrics
//
// This routine houses the code to fit a 6 term polynomial (z =ax^2 + by^2 + cxy
// + dx + ey + f) to a surface, and then use the derivatives of this to
// calculate roughness parameters, that quantify the variability of topographic
// surface normal vectors.
// 
// The surface is fitted to all the points that lie within circular
// neighbourhood that is defined by the designated window radius.  This surface
// is used to determine the orientation of the surface normal vector at each
// cell.  The algorithm then searches through the grid again, using a second
// search window to look for the local variability in normal vector orientation.
// The user also inputs a binary raster, which tells the program which rasters
// it wants to create (label as "true" to produce them, "false" to ignore them.
// This has 3 elements, as listed below:
//        0 -> s1 -> describes clustering of normals around the major axis
//        1 -> s2 -> describes clustering of normals around semi major axis
//        2 -> s3 -> describes clustering around minor axis
// The program returns a vector of LSDRasters.  For options marked "0" in
// binary input raster, the returned LSDRaster houses a blank raster, as this
// metric has not been calculated.  The desired LSDRaster can be retrieved from
// the output vector by using the same cell reference shown in the list above
// i.e. it is the same as the reference in the input binary vector.
//
// DTM 01/04/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
vector<LSDRaster> LSDRaster::calculate_polyfit_roughness_metrics(float window_radius1, float window_radius2, vector<int> raster_selection)
{
	Array2D<float> void_array(1,1,NoDataValue);
  LSDRaster VOID(1,1,NoDataValue,NoDataValue,NoDataValue,NoDataValue,void_array,GeoReferencingStrings);  
  
  // catch if the supplied window radius is less than the data resolution and
	// set it to equal the data resolution - SWDG
  if (window_radius1 < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius1 << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius1 = DataResolution;
  }
  if (window_radius2 < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius2 << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius2 = DataResolution;
  }
  // this fits a polynomial surface over a kernel window. First, perpare the
  // kernel
	int kr = int(ceil(window_radius1/DataResolution));  // Set radius of kernel
	int kw=2*kr+1;                    						     // width of kernel

	Array2D<float> data_kernel(kw,kw,NoDataValue);
	Array2D<float> x_kernel(kw,kw,NoDataValue);
	Array2D<float> y_kernel(kw,kw,NoDataValue);
	Array2D<int> mask(kw,kw,0);

	// reset the a,b,c,d,e and f matrices (the coefficient matrices)
	Array2D<float> temp_coef(NRows,NCols,NoDataValue);
	Array2D<float> pheta, phi, s1_raster, s2_raster, s3_raster;
  // Copy across raster template into the desired array containers
  if(raster_selection[0]==1)  s1_raster = temp_coef.copy();
  if(raster_selection[1]==1)  s2_raster = temp_coef.copy();
	if(raster_selection[2]==1)  s3_raster = temp_coef.copy();
  pheta = temp_coef.copy();
  phi = temp_coef.copy();
  float a,b,c,d,e,f;
  
	// scale kernel window to resolution of DEM, and translate coordinates to be
	// centred on cell of interest (the centre cell)
	float x,y,zeta,radial_dist;
	for(int i=0;i<kw;++i)
	{
	  for(int j=0;j<kw;++j)
	  {
	    x_kernel[i][j]=(i-kr)*DataResolution;
	    y_kernel[i][j]=(j-kr)*DataResolution;
			// Build circular mask
			// distance from centre to this point.
			radial_dist = sqrt(y_kernel[i][j]*y_kernel[i][j] + x_kernel[i][j]*x_kernel[i][j]);

      if (floor(radial_dist) <= window_radius1)
      {
				mask[i][j] = 1;
			}
    }
	}
	// FIT POLYNOMIAL SURFACE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO
	// DETERMINE TOPOGRAPHIC METRICS
	// Have N simultaneous linear equations, and N unknowns.
	// => b = Ax, where x is a 1xN array containing the coefficients we need for
	// surface fitting.
	// A is constructed using different combinations of x and y, thus we only need
	// to compute this once, since the window size does not change.
	// For 2nd order surface fitting, there are 6 coefficients, therefore A is a
	// 6x6 matrix
	Array2D<float> A(6,6,0.0);
	for (int i=0; i<kw; ++i)
	{
		for (int j=0; j<kw; ++j)
		{
			if (mask[i][j] == 1)
      {
       	x = x_kernel[i][j];
				y = y_kernel[i][j];

				// Generate matrix A
				A[0][0] += pow(x,4);
				A[0][1] += pow(x,2)*pow(y,2);
				A[0][2] += pow(x,3)*y;
				A[0][3] += pow(x,3);
				A[0][4] += pow(x,2)*y;
				A[0][5] += pow(x,2);
				A[1][0] += pow(x,2)*pow(y,2);
				A[1][1] += pow(y,4);
				A[1][2] += x*pow(y,3);
				A[1][3] += x*pow(y,2);
				A[1][4] += pow(y,3);
				A[1][5] += pow(y,2);
				A[2][0] += pow(x,3)*y;
				A[2][1] += x*pow(y,3);
				A[2][2] += pow(x,2)*pow(y,2);
				A[2][3] += pow(x,2)*y;
				A[2][4] += x*pow(y,2);
				A[2][5] += x*y;
				A[3][0] += pow(x,3);
				A[3][1] += x*pow(y,2);
				A[3][2] += pow(x,2)*y;
				A[3][3] += pow(x,2);
				A[3][4] += x*y;
				A[3][5] += x;
				A[4][0] += pow(x,2)*y;
				A[4][1] += pow(y,3);
				A[4][2] += x*pow(y,2);
				A[4][3] += x*y;
				A[4][4] += pow(y,2);
				A[4][5] += y;
				A[5][0] += pow(x,2);
				A[5][1] += pow(y,2);
				A[5][2] += x*y;
				A[5][3] += x;
				A[5][4] += y;
				A[5][5] += 1;
		  }
		}
	}

	// Move window over DEM, fitting 2nd order polynomial surface to the
	// elevations within the window.
	cout << "\n\tRunning 2nd order polynomial fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;

	for(int i=0;i<NRows;++i)
	{
		cout << flush << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for(int j=0;j<NCols;++j)
		{
			// Avoid edges
			if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
			{
        pheta[i][j]=NoDataValue;
        phi[i][j]=NoDataValue;
			}
			else
			{
				// clip DEM
				//zeta_sampler=zeta.copy();
				for(int i_kernel=0;i_kernel<kw;++i_kernel)
				{
			  	for(int j_kernel=0;j_kernel<kw;++j_kernel)
			  	{
						data_kernel[i_kernel][j_kernel] =
									RasterData[i-kr+i_kernel][j-kr+j_kernel];
						// check for nodata values nearby
						if(data_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
			  	}
				}
				// Fit polynomial surface, avoiding nodata values          ==================> Could change this, as can fit polynomial surface as long as there are 6 data points.
				if(ndv_present == 0)  // test for nodata values within the selection
				{
					Array1D<float> bb(6,0.0);
					Array1D<float> coeffs(6);
					for (int krow=0; krow<kw; ++krow)
					{
						for (int kcol=0; kcol<kw; ++kcol)
						{
							if (mask[krow][kcol] == 1)
              {
                x = x_kernel[krow][kcol];
					      y = y_kernel[krow][kcol];
					      zeta = data_kernel[krow][kcol];
					      // Generate vector bb
					      bb[0] += zeta*x*x;
					      bb[1] += zeta*y*y;
					      bb[2] += zeta*x*y;
					      bb[3] += zeta*x;
					      bb[4] += zeta*y;
					      bb[5] += zeta;
					    }		// end mask
            }			// end kernal column
					}				// end kernal row
					// Solve matrix equations using LU decomposition using the TNT JAMA
					// package:
          // A.coefs = b, where coefs is the coefficients vector.
					LU<float> sol_A(A);  // Create LU object
					coeffs = sol_A.solve(bb);

			  	a=coeffs[0];
			  	b=coeffs[1];
			  	c=coeffs[2];
			  	d=coeffs[3];
			  	e=coeffs[4];
			  	f=coeffs[5];
			  	
			  	// COMPUTING SURFACE NORMAL in spherical polar coordinate (ignore
          // radial component)
          pheta[i][j] = atan(sqrt(pow(d,2) + pow(e,2)));
          if(d==0 && e==0) phi[i][j] = NoDataValue;
    			else if(d==0 && e>0) phi[i][j] = acos(-1)/2;
          else if(d==0 && e<0) phi[i][j] = 3*acos(-1)/2;
          else phi[i][j]=atan(e/d);
				}					// end if statement for no data value
				ndv_present = 0;
			}
		}
	}
	// 2) DETERMINE ORIENTATION MATRIX AT EACH POINT, AND THEN SOLVE FOR THE
  // EIGENVALUES.
  // Prepare new kernel
  kr=ceil(window_radius2/DataResolution);      // Set radius of kernel as >= specified radius
  kw=2*kr+1;                    // width of kernel
  // Declare kernel arrays
  Array2D<float> pheta_kernel(kw,kw);
  Array2D<float> phi_kernel(kw,kw);
	// Build circular mask
	Array2D<int> mask2(kw,kw,0);
	float x_kernel_ref,y_kernel_ref;
  for(int i=0;i<kw;++i)
  {
    for(int j=0;j<kw;++j)
    {
  		x_kernel_ref=(i-kr)*DataResolution;
     	y_kernel_ref=(j-kr)*DataResolution;
     	radial_dist = sqrt(y_kernel_ref*y_kernel_ref + x_kernel_ref*x_kernel_ref); // distance from centre to this point.
     	if (floor(radial_dist) <= window_radius2)
     	{
        mask2[i][j] = 1;
  		}
    }
	}
  
  // Loop over DEM again, this time looking at variability of surface normals
  cout << "Finding eigenvalues for local surface. Search radius = " << kr << "m" << endl;
  for(int i=0; i<NRows; ++i)
  {
    cout << flush << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0; j<NCols; ++j)
    {
      // Avoid edges and nodata values
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
      {
        if(raster_selection[0]==1)  s1_raster[i][j] = NoDataValue;
        if(raster_selection[1]==1)  s2_raster[i][j] = NoDataValue;
        if(raster_selection[2]==1)  s3_raster[i][j] = NoDataValue;
      }
      else
      {
        // build orientation matrix for this point
        Array2D<double> T(3,3,0.0);
        Array2D<double> D(3,3);
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            pheta_kernel[i_kernel][j_kernel]=pheta[i-kr+i_kernel][j-kr+j_kernel];
            phi_kernel[i_kernel][j_kernel]=phi[i-kr+i_kernel][j-kr+j_kernel];
            // check for nodata values nearby
						if(phi_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
          }
        }        
        int N=0;
        //         if(ndv_present == 0)  // test for nodata values within the selection
// 				{
          for(int i_kernel=0;i_kernel<kw;++i_kernel)
          {
            for(int j_kernel=0;j_kernel<kw;++j_kernel)
            {
              if (mask2[i_kernel][j_kernel]==1)
              {
                double li,mi,ni;
                li=0;
                mi=0;
                ni=0;
                if(phi_kernel[i_kernel][j_kernel]!=0 && phi_kernel[i_kernel][j_kernel]!=NoDataValue  && pheta_kernel[i_kernel][j_kernel]!=NoDataValue)
                { 
                  li=sin(pheta_kernel[i_kernel][j_kernel])*cos(phi_kernel[i_kernel][j_kernel]);
                  mi=sin(pheta_kernel[i_kernel][j_kernel])*sin(phi_kernel[i_kernel][j_kernel]);
                  ni=cos(pheta_kernel[i_kernel][j_kernel]);
                  T[0][0] += pow(li,2);
                  T[0][1] += li*mi;
                  T[0][2] += li*ni;
                  T[1][0] += mi*li;
                  T[1][1] += pow(mi,2);
                  T[1][2] += mi*ni;
                  T[2][0] += ni*li;
                  T[2][1] += ni*mi;
                  T[2][2] += pow(ni,2);
                  ++N;
                }
              }
            }
          }
          // Find eigenvalues of the orientation matrix
          Eigenvalue<double> eigenvalue_matrix(T);
          eigenvalue_matrix.getD(D);
          //surface_roughness(kw,kw,T,lnS1_S2, S3);
          if(raster_selection[0]==1)  s1_raster[i][j] = D[2][2]/N;
          if(raster_selection[1]==1)  s2_raster[i][j] = D[1][1]/N;
          if(raster_selection[2]==1)  s3_raster[i][j] = D[0][0]/N; 
//         }
//       ndv_present = 0; 
      } 
    }
  }
	
	
	// Now create LSDRasters and load into output vector
	vector<LSDRaster> output_rasters_temp(3,VOID);
  vector<LSDRaster> raster_output = output_rasters_temp;
  if(raster_selection[0]==1)
  {
    LSDRaster s1(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s1_raster,GeoReferencingStrings);
    raster_output[0] = s1;
  }
  if(raster_selection[1]==1)  
  {
    LSDRaster s2(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s2_raster,GeoReferencingStrings);
    raster_output[1] = s2;
  }	
  if(raster_selection[2]==1)  
  {
    LSDRaster s3(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s3_raster,GeoReferencingStrings);
    raster_output[2] = s3;
  }  
  return raster_output;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// calculate_polyfit_coefficient_matrices
//
// this function calcualtes 6 coefficient matrices that allow the user to
// then calcualte slope, curvature, aspect, a classification for finding saddles and peaks
// and other metrics
//
// The coefficient matrices are overwritten during the running of this member function
//
// DTM
//
// Updated 15/07/2013 to use a circular mask for surface fitting. DTM
// Updated 24/07/2013 to check window_radius size and correct values below data resolution. SWDG
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_polyfit_coefficient_matrices(float window_radius,
										Array2D<float>& a, Array2D<float>& b,
										Array2D<float>& c, Array2D<float>& d,
										Array2D<float>& e, Array2D<float>& f)
{


	// catch if the supplied window radius is less than the data resolution and set
	// it to equal the data resolution - SWDG
  if (window_radius < DataResolution){
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }

  // this fits a polynomial surface over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));           // Set radius of kernel
	int kw=2*kr+1;                    						// width of kernel

	Array2D<float> data_kernel(kw,kw,NoDataValue);
	Array2D<float> x_kernel(kw,kw,NoDataValue);
	Array2D<float> y_kernel(kw,kw,NoDataValue);
	Array2D<int> mask(kw,kw,0);

	// reset the a,b,c,d,e and f matrices (the coefficient matrices)
	Array2D<float> temp_coef(NRows,NCols,0.0);
	a = temp_coef.copy();
	b = temp_coef.copy();
	c = temp_coef.copy();
	d = temp_coef.copy();
	e = temp_coef.copy();
	f = temp_coef.copy();

	// scale kernel window to resolution of DEM, and translate coordinates to be
	// centred on cell of interest (the centre cell)
	float x,y,zeta,radial_dist;
	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
	      	x_kernel[i][j]=(i-kr)*DataResolution;
	      	y_kernel[i][j]=(j-kr)*DataResolution;

			// Build circular mask
			// distance from centre to this point.
			radial_dist = sqrt(y_kernel[i][j]*y_kernel[i][j] + x_kernel[i][j]*x_kernel[i][j]);

        	if (floor(radial_dist) <= window_radius)
        	{
				mask[i][j] = 1;
			}
      	}
	}

	// FIT POLYNOMIAL SURFACE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO
	// DETERMINE TOPOGRAPHIC METRICS

	// Have N simultaneous linear equations, and N unknowns.
	// => b = Ax, where x is a 1xN array containing the coefficients we need for
	// surface fitting.
	// A is constructed using different combinations of x and y, thus we only need
	// to compute this once, since the window size does not change.
	// For 2nd order surface fitting, there are 6 coefficients, therefore A is a
	// 6x6 matrix
	Array2D<float> A(6,6);
	for (int i=0; i<kw; ++i)
	{
		for (int j=0; j<kw; ++j)
		{
			if (mask[i][j] == 1)
      		{
        		x = x_kernel[i][j];
  				y = y_kernel[i][j];

  				// Generate matrix A
  				A[0][0] += pow(x,4);
  				A[0][1] += pow(x,2)*pow(y,2);
  				A[0][2] += pow(x,3)*y;
  				A[0][3] += pow(x,3);
  				A[0][4] += pow(x,2)*y;
  				A[0][5] += pow(x,2);
  				A[1][0] += pow(x,2)*pow(y,2);
  				A[1][1] += pow(y,4);
  				A[1][2] += x*pow(y,3);
  				A[1][3] += x*pow(y,2);
  				A[1][4] += pow(y,3);
  				A[1][5] += pow(y,2);
  				A[2][0] += pow(x,3)*y;
  				A[2][1] += x*pow(y,3);
  				A[2][2] += pow(x,2)*pow(y,2);
  				A[2][3] += pow(x,2)*y;
  				A[2][4] += x*pow(y,2);
  				A[2][5] += x*y;
  				A[3][0] += pow(x,3);
  				A[3][1] += x*pow(y,2);
  				A[3][2] += pow(x,2)*y;
  				A[3][3] += pow(x,2);
  				A[3][4] += x*y;
  				A[3][5] += x;
  				A[4][0] += pow(x,2)*y;
  				A[4][1] += pow(y,3);
  				A[4][2] += x*pow(y,2);
  				A[4][3] += x*y;
  				A[4][4] += pow(y,2);
  				A[4][5] += y;
  				A[5][0] += pow(x,2);
  				A[5][1] += pow(y,2);
  				A[5][2] += x*y;
  				A[5][3] += x;
  				A[5][4] += y;
  				A[5][5] += 1;
			}
		}
	}

	// Move window over DEM, fitting 2nd order polynomial surface to the
	// elevations within the window.
	cout << "\n\tRunning 2nd order polynomial fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;

	for(int i=0;i<NRows;++i)
	{
		cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for(int j=0;j<NCols;++j)
		{
			// Avoid edges
			if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols))
			{
				a[i][j] = NoDataValue;
				b[i][j] = NoDataValue;
				c[i][j] = NoDataValue;
				d[i][j] = NoDataValue;
				e[i][j] = NoDataValue;
				f[i][j] = NoDataValue;
			}
			// Avoid nodata values
			else if(RasterData[i][j]==NoDataValue)
			{
				a[i][j] = NoDataValue;
				b[i][j] = NoDataValue;
				c[i][j] = NoDataValue;
				d[i][j] = NoDataValue;
				e[i][j] = NoDataValue;
				f[i][j] = NoDataValue;
			}
			else
			{
				// clip DEM
				//zeta_sampler=zeta.copy();
				for(int i_kernel=0;i_kernel<kw;++i_kernel)
				{
			  		for(int j_kernel=0;j_kernel<kw;++j_kernel)
			  		{
						data_kernel[i_kernel][j_kernel] =
									RasterData[i-kr+i_kernel][j-kr+j_kernel];
						// check for nodata values nearby
						if(data_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
			  		}
				}

				// Fit polynomial surface, avoiding nodata values
				if(ndv_present == 0)  // test for nodata values within the selection
				{
					Array1D<float> bb(6,0.0);
					Array1D<float> coeffs(6);
					for (int krow=0; krow<kw; ++krow)
					{
						for (int kcol=0; kcol<kw; ++kcol)
						{
							if (mask[krow][kcol] == 1)
              				{
                				x = x_kernel[krow][kcol];
					      		y = y_kernel[krow][kcol];
					      		zeta = data_kernel[krow][kcol];
					      		// Generate vector bb
					      		bb[0] += zeta*x*x;
					      		bb[1] += zeta*y*y;
					      		bb[2] += zeta*x*y;
					      		bb[3] += zeta*x;
					      		bb[4] += zeta*y;
					      		bb[5] += zeta;
					      	}		// end mask
            			}			// end kernal column
					}				// end kernal row
					// Solve matrix equations using LU decomposition using the TNT JAMA package:
					// A.coefs = b, where coefs is the coefficients vector.
					LU<float> sol_A(A);  // Create LU object
					coeffs = sol_A.solve(bb);

			  		a[i][j]=coeffs[0];
			  		b[i][j]=coeffs[1];
			  		c[i][j]=coeffs[2];
			  		d[i][j]=coeffs[3];
			  		e[i][j]=coeffs[4];
			  		f[i][j]=coeffs[5];
				}					// end if statement for no data value
				ndv_present = 0;
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the elevation based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
// added by FC 24/03/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_elevation(Array2D<float>& f)
{
  // create the new elevation raster
  Array2D<float> elevation_data(NRows,NCols,NoDataValue);

  for (int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if (f[row][col] != NoDataValue)
      {
        elevation_data[row][col] = f[row][col];
      }
    }
  }

  LSDRaster elevation_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                           elevation_data,GeoReferencingStrings);
  return elevation_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the slope based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_slope(Array2D<float>& d, Array2D<float>& e)
{
  // create the new slope raster
  Array2D<float> slope_data(NRows,NCols,NoDataValue);

  for (int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if (d[row][col] != NoDataValue)
      {
        slope_data[row][col] = sqrt(d[row][col]*d[row][col]+e[row][col]*e[row][col]);
      }
    }
  }

  LSDRaster slope_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                         NoDataValue,slope_data,GeoReferencingStrings);
  return slope_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the aspect based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// SMM modified from DTM standalone code 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_aspect(Array2D<float>& d, Array2D<float>& e)
{
  // create the new slope raster
  Array2D<float> aspect_data(NRows,NCols,NoDataValue);

  for (int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if (d[row][col] != NoDataValue)
      {
        if(d[row][col]==0 || e[row][col]==0)
        {
          aspect_data[row][col] = NoDataValue;
        }
        else
        {
          aspect_data[row][col] = 180 - 57.29578*atan(e[row][col]/d[row][col])
                                      + 90*(d[row][col]/abs(d[row][col]));
          if(aspect_data[row][col] < 180.0)
          {
            aspect_data[row][col] = 180.0 - aspect_data[row][col];
          }
          else
          {
            aspect_data[row][col] = 360.0 + (180 - aspect_data[row][col]);
          }
        }
      }
    }
  }

  LSDRaster aspect_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                             NoDataValue,aspect_data,GeoReferencingStrings);
  return aspect_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
//
// SMM modified from DTM standalone code 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_curvature(Array2D<float>& a, Array2D<float>& b)
{
  // create the new slope raster
  Array2D<float> curvature_data(NRows,NCols,NoDataValue);

  for (int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if (a[row][col] != NoDataValue)
      {
        curvature_data[row][col] = 2*a[row][col]+2*b[row][col];
      }
    }
  }


  LSDRaster curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                             NoDataValue,curvature_data,GeoReferencingStrings);
  return curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the planform curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by DM and FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_planform_curvature(Array2D<float>& a, Array2D<float>& b,
                                                          Array2D<float>& c, Array2D<float>& d,
                                                          Array2D<float>& e)
{
	// create the new planform curvature raster
	Array2D<float> pl_curvature_data(NRows,NCols,NoDataValue);
  	float fx, fy, fxx, fyy, fxy, p, q;

	for (int row = 0; row<NRows; row++)
	{

		for(int col = 0; col<NCols; col++)
		{

			if (a[row][col] != NoDataValue)
			{
				fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;

			  	if (q > 0)
			  	{
					pl_curvature_data[row][col] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/(sqrt(q*q*q));
				}
				else
				{
					pl_curvature_data[row][col] = NoDataValue;
				}
			}
		}
	}


	LSDRaster planform_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,pl_curvature_data,GeoReferencingStrings);
	return planform_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the profile curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_profile_curvature(Array2D<float>& a, Array2D<float>& b,
                                                          Array2D<float>& c, Array2D<float>& d,
                                                          Array2D<float>& e)
{
	// create the new profile curvature raster
	Array2D<float> profile_curvature_data(NRows,NCols,NoDataValue);
  	float fx, fy, fxx, fyy, fxy, p, q, qqq, denom;

	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (a[row][col] != NoDataValue)
			{
			  	fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;

			  	qqq = q*q*q;
			  	if( qqq>0)
			  	{
					denom = (p*sqrt(qqq));
					if( denom != 0)
					{
						profile_curvature_data[row][col] = (fxx*fx*fx + 2*fxy*fx*fy + fyy*fy*fy)/denom;
					}
					else
					{
						profile_curvature_data[row][col] = NoDataValue;
					}
				}
				else
				{
					profile_curvature_data[row][col] = NoDataValue;
				}



			}
		}
	}


	LSDRaster profile_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,profile_curvature_data,GeoReferencingStrings);
	return profile_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function calculates the tangential curvature based on a polynomial fit
// the window is determined by the calculate_polyfit_coefficient_matrices
// this function also calculates the a,b,c,d,e and f coefficient matrices
// Code written by DM and FC 09/10/12
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_polyfit_tangential_curvature(Array2D<float>& a, Array2D<float>& b,
                                                          Array2D<float>& c, Array2D<float>& d,
                                                          Array2D<float>& e)
{
	// create the new planform curvature raster
	Array2D<float> ta_curvature_data(NRows,NCols,NoDataValue);
  	float fx, fy, fxx, fyy, fxy, p, q, denom;

	for (int row = 0; row<NRows; row++)
	{

		for(int col = 0; col<NCols; col++)
		{

			if (a[row][col] != NoDataValue)
			{
				fx = d[row][col];
			  	fy = e[row][col];
			  	fxx = 2*a[row][col];
			  	fyy = 2*b[row][col];
			  	fxy = c[row][col];
			  	p = fx*fx + fy*fy;
			  	q = p + 1;


			  	if( q>0)
			  	{
					denom = (p*sqrt(q));
					if( denom != 0)
					{
						ta_curvature_data[row][col] = (fxx*fy*fy - 2*fxy*fx*fy + fyy*fx*fx)/denom;
					}
					else
					{
						ta_curvature_data[row][col] = NoDataValue;
					}
				}
				else
				{
					ta_curvature_data[row][col] = NoDataValue;
				}

			}
		}
	}


	LSDRaster tangential_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,ta_curvature_data,GeoReferencingStrings);
	return tangential_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function identifies approximate position of stationary points within
// discrete surface using a threshold slope. The nature of the stationary point
// is then determined to discriminate peaks, depressions and saddles.
// 0 = Non-stationary
// 1 = Peak
// 2 = Depression
// 3 = Saddle
//
// Added by DTM 17/09/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDRaster::calculate_polyfit_classification(Array2D<float>& a, Array2D<float>& b, Array2D<float>& c,
                                                           Array2D<float>& d, Array2D<float>& e)
{
	// create the new classification raster
	int intNoDataValue = int(NoDataValue);
	Array2D<int> classification(NRows,NCols,intNoDataValue);
	float d2z_dx2,d2z_dy2,d2z_dxdy,slope;
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				slope = sqrt(pow(d[row][col],2) + pow(e[row][col],2));
        		if (slope < 0.1) // Threshold for assessing whether point is close to a stationary point
        		{
					d2z_dx2 = 2*a[row][col];
          			d2z_dy2 = 2*b[row][col];
          			d2z_dxdy = c[row][col];
          			if (d2z_dx2 < 0 && d2z_dy2 < 0 && d2z_dxdy*d2z_dxdy < d2z_dx2*d2z_dy2)  // Conditions for peak
          			{
            			classification[row][col] = 1;
          			}
          			else if (d2z_dx2 > 0 && d2z_dy2 > 0 && d2z_dxdy*d2z_dxdy < d2z_dx2*d2z_dy2) // Conditions for a depression
          			{
            			classification[row][col] = 2;
          			}
          			else if (d2z_dx2*d2z_dy2 < 0 || d2z_dxdy*d2z_dxdy > d2z_dx2*d2z_dy2)  // Conditions for a saddle
          			{
           				classification[row][col] = 3;
          			}
          			else
          			{
            			classification = 0;
          			}
				}
			}
		}
	}

	LSDIndexRaster sp_class_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                                  intNoDataValue,classification,GeoReferencingStrings);
	return sp_class_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
// 5 tangential curvature
// 6 classification
// SMM 18-Dec-2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_and_print_polyfit_rasters(float window_radius, string file_prefix, vector<int> file_code)
{
	// set up polyfit arrays
	Array2D<float> a;
	Array2D<float> b;
	Array2D<float> c;
	Array2D<float> d;
	Array2D<float> e;
	Array2D<float> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=7)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		float decimal = window_radius-float(window_int);
		float decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";


		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster PolySlope = calculate_polyfit_slope(d, e);
			string S_name = "_pslope_";
			S_name = file_prefix+S_name+window_size_str;
			PolySlope.write_raster(S_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster PolyAspect = calculate_polyfit_aspect(d,e);
			string A_name = "_paspect_";
			A_name = file_prefix+A_name+window_size_str;
			PolyAspect.write_raster(A_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster PolyCurv = calculate_polyfit_curvature(a,b);
			string C_name = "_pcurv_";
			C_name = file_prefix+C_name+window_size_str;
			PolyCurv.write_raster(C_name,DEM_flt_extension);
		}
		if (file_code[3] == 1)
		{
			LSDRaster PolyPlCurv = calculate_polyfit_planform_curvature(a,b,c,d,e);
			string CP_name = "_pplcurv_";
			CP_name = file_prefix+CP_name+window_size_str;
			PolyPlCurv.write_raster(CP_name,DEM_flt_extension);
		}
		if (file_code[4] == 1)
		{
			LSDRaster PolyPrCurv = calculate_polyfit_profile_curvature(a,b,c,d,e);
			string CPr_name = "_pprcurv_";
			CPr_name = file_prefix+CPr_name+window_size_str;
			PolyPrCurv.write_raster(CPr_name,DEM_flt_extension);
		}
		if (file_code[5] == 1)
		{
			LSDRaster PolyTaCurv = calculate_polyfit_tangential_curvature(a,b,c,d,e);
			string CTa_name = "_ptacurv_";
			CTa_name = file_prefix+CTa_name+window_size_str;
			PolyTaCurv.write_raster(CTa_name,DEM_flt_extension);
		}
		if (file_code[6] == 1)
		{
			LSDIndexRaster PolyClass = calculate_polyfit_classification(a,b,c,d,e);
			string CCl_name = "_pclass_";
			CCl_name = file_prefix+CCl_name+window_size_str;
			PolyClass.write_raster(CCl_name,DEM_flt_extension);
		}
	}

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// this function combines the polyfit functions and the roughness function in one package that
// is data efficient becasue it only requires one calcualtion of the polyfit matrices.
// it takes the window radius of the polyfit and the window of the roughness calcualtion
// the file codes in the vector are:
// 0 slope
// 1 aspect
// 2 curvature
// 3 planform curvature
// 4 profile curvature
// 5 tangential curvature
// 6 classification
// 7 roughness s1
// 8 roughness s2
// 9 roughness s3
// SMM 19-12-2012 -> adapted by DTM 15/07/2013 since it really makes sense just to kick out
// s1,s2,s3 rather than derivatives for options 7-9
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_and_print_polyfit_and_roughness_rasters(float window_radius, float roughness_radius,
										string file_prefix, vector<int> file_code)
{
	// set up polyfit arrays
	Array2D<float> a;
	Array2D<float> b;
	Array2D<float> c;
	Array2D<float> d;
	Array2D<float> e;
	Array2D<float> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=10)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		float decimal = window_radius-float(window_int);
		float decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";

		int roughness_int = int(roughness_radius);
		float decimalroughness = roughness_radius-float(roughness_int);
		float decimalroughness_ten = decimalroughness*10;
		int decimalroughness_ten_str = int(decimalroughness_ten);
		string roughness_number_str = itoa(roughness_int);
		string remainderroughness_str = itoa(decimalroughness_ten_str);
		string polystring = "ply";
		string roughstring = "rgh";
		string roughness_size_str = roughness_number_str+p_str+remainderroughness_str;
		roughness_size_str = polystring+window_size_str+underscore+roughstring+roughness_size_str;

		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster PolySlope = calculate_polyfit_slope(d, e);
			string S_name = "_pslope_";
			S_name = file_prefix+S_name+window_size_str;
			PolySlope.write_raster(S_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster PolyAspect = calculate_polyfit_aspect(d,e);
			string A_name = "_paspect_";
			A_name = file_prefix+A_name+window_size_str;
			PolyAspect.write_raster(A_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster PolyCurv = calculate_polyfit_curvature(a,b);
			string C_name = "_pcurv_";
			C_name = file_prefix+C_name+window_size_str;
			PolyCurv.write_raster(C_name,DEM_flt_extension);
		}
		if (file_code[3] == 1)
		{
			LSDRaster PolyPlCurv = calculate_polyfit_planform_curvature(a,b,c,d,e);
			string CP_name = "_pplcurv_";
			CP_name = file_prefix+CP_name+window_size_str;
			PolyPlCurv.write_raster(CP_name,DEM_flt_extension);
		}
		if (file_code[4] == 1)
		{
			LSDRaster PolyPrCurv = calculate_polyfit_profile_curvature(a,b,c,d,e);
			string CPr_name = "_pprcurv_";
			CPr_name = file_prefix+CPr_name+window_size_str;
			PolyPrCurv.write_raster(CPr_name,DEM_flt_extension);
		}
		if (file_code[5] == 1)
		{
			LSDRaster PolyTaCurv = calculate_polyfit_tangential_curvature(a,b,c,d,e);
			string CTa_name = "_ptacurv_";
			CTa_name = file_prefix+CTa_name+window_size_str;
			PolyTaCurv.write_raster(CTa_name,DEM_flt_extension);
		}
		if (file_code[6] == 1)
		{
			LSDIndexRaster PolyClass = calculate_polyfit_classification(a,b,c,d,e);
			string CCl_name = "_pclass_";
			CCl_name = file_prefix+CCl_name+window_size_str;
			PolyClass.write_raster(CCl_name,DEM_flt_extension);
		}
		if (file_code[7] == 1|| file_code[8] == 1|| file_code[9] == 1)
		{
			Array2D<float> l;
			Array2D<float> m;
			Array2D<float> n;
			Array2D<float> s1;
			Array2D<float> s2;
			Array2D<float> s3;
			calculate_polyfit_directional_cosines(d, e, l, m, n);
			calculate_orientation_matrix_eigenvalues(roughness_radius,l,m,n,s1,s2,s3);

      		if (file_code[7] == 1)
		  	{
				LSDRaster s1_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s1,GeoReferencingStrings);
        		string s1_name = "_s1_";
        		s1_name = file_prefix+s1_name+roughness_size_str;
        		s1_raster.write_raster(s1_name,DEM_flt_extension);
        	}
  			if (file_code[8] == 1)
  			{
				LSDRaster s2_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s2,GeoReferencingStrings);
        		string s2_name = "_s2_";
  				s2_name = file_prefix+s2_name+roughness_size_str;
  				s2_raster.write_raster(s2_name,DEM_flt_extension);
  			}
  			if (file_code[9] == 1)
  			{
  				LSDRaster s3_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s3,GeoReferencingStrings);
        		string s3_name = "_s3_";
  				s3_name = file_prefix+s3_name+roughness_size_str;
  				s3_raster.write_raster(s3_name,DEM_flt_extension);
  			}

		}		// end file codes 7,8,9

	}			// end test for file codes logic

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// GET HILLTOP CURVATURE DTM 30/04/13
// Input rasters: curvature, hilltop network.
// Output raster: hilltop curvature
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::get_hilltop_curvature(LSDRaster& curvature, LSDRaster& Hilltops)
{
  // create the new planform curvature raster
  Array2D<float> hilltop_curvature(NRows,NCols,NoDataValue);

  for (int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if (Hilltops.get_data_element(row,col) != NoDataValue)
      {
        hilltop_curvature[row][col] = curvature.get_data_element(row,col);
        }
    }
  }

  LSDRaster hilltop_curvature_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                             NoDataValue,hilltop_curvature,GeoReferencingStrings);
  return hilltop_curvature_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// REMOVE POSITIVE HILLTOP CURVATURE
// This function removes positive hilltop curvature values from the hilltop curvature raster
// (to remove noise)
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::remove_positive_hilltop_curvature(LSDRaster& hilltop_curvature)
{
  
  Array2D<float> CHT_array(NRows,NCols,NoDataValue);
  for (int row = 0; row < NRows; row++)
  {
    for (int col = 0; col < NCols; col++)
    {
      float curvature = hilltop_curvature.get_data_element(row,col);
      if (curvature < 0)
      {
         CHT_array[row][col] = curvature; 
      }
    }
  }
  LSDRaster CHT(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, CHT_array,GeoReferencingStrings);
  
  return CHT;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Trigonometry
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Get slope angles
//
// This only works on a slope raster!!
// SMM
// 13/11/2014
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::calculate_slope_angles()
{
  cout << "I am calculating the slope angles; make sure you are using a slope raster!" << endl;
  
  Array2D<float> slope_angle(NRows,NCols, NoDataValue);
  
  for(int row = 0; row < NRows; row++)
  {
    for(int col = 0; col < NCols; col++)
    {
      if(RasterData[row][col] != NoDataValue)
      {
        slope_angle = atan(RasterData[row][col]);
      }
    }
  }

  LSDRaster Slope_angle(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                        slope_angle,GeoReferencingStrings);
  return Slope_angle;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function calculates the water supply rate that causes overland flow
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::calculate_water_supply_rate_for_saturation(float soil_thick,
                         float K, LSDRaster& ContributingArea, LSDRaster& SlopeAngle)
{

  Array2D<float> W_for_sat(NRows,NCols, NoDataValue);
  float SA;
  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if(RasterData[row][col] != NoDataValue)
      {
        if( ContributingArea.get_data_element(row,col) == 0)
        {
          W_for_sat[row][col] = NoDataValue;
        }
        else
        {
          SA = SlopeAngle.get_data_element(row,col);
          W_for_sat[row][col] = (soil_thick*K*sin(SA)*cos(SA)*DataResolution) /
                                ContributingArea.get_data_element(row,col);
        }
      
      }
    }
  }

  LSDRaster w_for_sat(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                        W_for_sat,GeoReferencingStrings);
  return w_for_sat;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function calculates the water supply rate that causes overland flow
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::calculate_factor_of_safety_at_saturation(float C_r, float rho_s,
                         float soil_thick, float tan_phi, LSDRaster& SlopeAngle)
{

  Array2D<float> FS_for_sat(NRows,NCols, NoDataValue);
  float SA;
  float cos_SA;
  float sin_SA;
  float g = 9.80;       // gravity
  float rho_w = 1000;   // density of water
  
  if(rho_s < 700)
  {
    cout << "Calculating factor of Safety. You soil density is really low" << endl
         << "Perhaps you entered it in g/cm^3? It should be in kg/m^3" << endl;
    if(rho_s < 5)
    {
      cout << "Assuming density was given in g/cm^3 so muliplying by 1000" << endl;
      rho_s = rho_s*1000;
    }
  }
  
  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if(RasterData[row][col] != NoDataValue)
      {
        SA = SlopeAngle.get_data_element(row,col);
        cos_SA = cos(SA);
        sin_SA = sin(SA);
        
        if (sin_SA == 0)
        {
          FS_for_sat[row][col] = 0;
        }
        else if (cos_SA == 0)
        {
          FS_for_sat[row][col] = NoDataValue;
        }
        else
        {
          FS_for_sat[row][col] = C_r/(g*rho_s*soil_thick*cos_SA*sin_SA) +
                                (rho_s - rho_w)*cos_SA*tan_phi/(rho_s*sin_SA);
        }
      
      }
    }
  }

  LSDRaster fs_for_sat(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                        FS_for_sat,GeoReferencingStrings);
  return fs_for_sat;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// RRRRR  EEEEEE IIIIII
// RR  RR EE       II
// RRRR   EEEE     II
// RR RR  EE       II
// RR  RR EEEEEE IIIIII
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=
// ROCK EXPOSURE INDEX
// DiBiase et al. (2012) developed the rock eposure index as a proxy for the
// degree of rock exposure within a basin as defined by the proportion of pixels
// with a local slope exceeding a critical value.  They calculate local slope by
// fitting a planar surface to a 9 cell moving window (window radius = 1).
// Algorithm written by DTM, 08/10/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_plane_coefficient_matrices(float window_radius,
										Array2D<float>& a_plane, Array2D<float>& b_plane,
										Array2D<float>& c_plane)
{
	// this fits a plane over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));           // Set radius of kernel
	int kw=2*kr+1;                    						// width of kernel
	Array2D<float> data_kernel(kw,kw,NoDataValue);
	Array2D<float> x_kernel(kw,kw,NoDataValue);
	Array2D<float> y_kernel(kw,kw,NoDataValue);
	// reset the a,b,c matrices (the coefficient matrices)
	Array2D<float> temp_coef(NRows,NCols,0.0);
	a_plane = temp_coef.copy();
	b_plane = temp_coef.copy();
	c_plane = temp_coef.copy();
	// scale kernel window to resolution of DEM, and translate coordinates to be
	// centred on cell of interest (the centre cell)
	float x,y,zeta;
	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
	      	x_kernel[i][j]=(i-kr)*DataResolution;
	      	y_kernel[i][j]=(j-kr)*DataResolution;
	    }
	}
	// FIT PLANE BY LEAST SQUARES REGRESSION AND USE COEFFICIENTS TO DETERMINE
	// LOCAL SLOPE
	// Have N simultaneous linear equations, and N unknowns.
	// => b = Ax, where x is a 1xN array containing the coefficients we need for
	// surface fitting.
	// A is constructed using different combinations of x and y, thus we only need
	// to compute this once, since the window size does not change.
	// For 1st order surface fitting, there are 3 coefficients, therefore A is a
	// 3x3 matrix
	Array2D<float> A(3,3);
	for (int i=0; i<kw; ++i)
	{
		for (int j=0; j<kw; ++j)
		{
			x = x_kernel[i][j];
			y = y_kernel[i][j];
			// Generate matrix A
			A[0][0] += pow(x,2);
			A[0][1] += x*y;
			A[0][2] += x;
			A[1][0] += y*x;
			A[1][1] += pow(y,2);
			A[1][2] += y;
			A[2][0] += x;
			A[2][1] += y;
			A[2][2] += 1;
		}
	}
	// Move window over DEM, fitting planar surface to the elevations within the
  // window.
	cout << "\n\tRunning planar surface fitting" << endl;
	cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
	int ndv_present = 0;
	for(int i=0;i<NRows;++i)
	{
		cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
		for(int j=0;j<NCols;++j)
		{
			// Avoid edges
			if(i-kr < 0 || i+kr+1 > NRows || j-kr < 0 || j+kr+1 > NCols)
			{
				a_plane[i][j] = NoDataValue;
				b_plane[i][j] = NoDataValue;
				c_plane[i][j] = NoDataValue;
			}
			// Avoid nodata values
			else if(RasterData[i][j]==NoDataValue)
			{
				a_plane[i][j] = NoDataValue;
				b_plane[i][j] = NoDataValue;
				c_plane[i][j] = NoDataValue;
			}
			else
			{
				// clip DEM
				//zeta_sampler=zeta.copy();
				for(int i_kernel=0;i_kernel<kw;++i_kernel)
				{
			  		for(int j_kernel=0;j_kernel<kw;++j_kernel)
			  		{
						data_kernel[i_kernel][j_kernel] =
						RasterData[i-kr+i_kernel][j-kr+j_kernel];
						// check for nodata values nearby
						if(data_kernel[i_kernel][j_kernel]==NoDataValue)
						{
							ndv_present=1;
						}
			  		}
				}
				// Fit best fitting plane, avoiding nodata values
				if(ndv_present == 0)  // test for nodata values within the selection
				{
					Array1D<float> bb(3,0.0);
					Array1D<float> coeffs(3);
					for (int krow=0; krow<kw; ++krow)
					{

					  	for (int kcol=0; kcol<kw; ++kcol)
					  	{
							x = x_kernel[krow][kcol];
					    	y = y_kernel[krow][kcol];
					    	zeta = data_kernel[krow][kcol];
					    	// Generate vector bb
					    	bb[0] += zeta*x;
					    	bb[1] += zeta*y;
					    	bb[2] += zeta;
					  	}
					}
					// Solve matrix equations using LU decomposition using the TNT JAMA package:
					// A.coefs = b, where coefs is the coefficients vector.
					LU<float> sol_A(A);  // Create LU object
					coeffs = sol_A.solve(bb);

			  		a_plane[i][j]=coeffs[0];
			  		b_plane[i][j]=coeffs[1];
			  		c_plane[i][j]=coeffs[2];
				}
				ndv_present = 0;
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_REI(Array2D<float>& a_plane, Array2D<float>& b_plane, float CriticalSlope)
{
	// create the REI raster
	Array2D<float> REI_data(NRows,NCols,NoDataValue);
  	float SlopeOfPlane;
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (a_plane[row][col] != NoDataValue)
			{
				SlopeOfPlane = sqrt(a_plane[row][col]*a_plane[row][col]+b_plane[row][col]*b_plane[row][col]);
				// Create binary matrix 1 = rock, 0 = no rock
        		if (SlopeOfPlane > CriticalSlope)
				{
          			REI_data[row][col] = 1;
        		}
        		else
        		{
          			REI_data[row][col] = 0;
        		}
			}
		}
	}

	LSDRaster REI_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,REI_data,GeoReferencingStrings);
	return REI_raster;
}
// Overloaded function that incorporates the above in a nicer wrapper function
LSDRaster LSDRaster::calculate_REI(float window_radius, float CriticalSlope)
{
  Array2D<float> a,b,c;
  calculate_plane_coefficient_matrices(window_radius,a,b,c);
  LSDRaster REI_raster =  calculate_REI(a, b, CriticalSlope);
  return REI_raster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// RRRRR    OOOO   UU   UU  GGGGG HH   HH NN    NN EEEEEE   SSSS    SSSS
// RR  RR  OO  OO  UU   UU GG     HH   HH NNN   NN EE     SS      SS
// RRRR   OO    OO UU   UU GG GGG HHHHHHH NN NN NN EEEE    SSSS    SSSS
// RR RR   OO  OO  UU   UU GG  GG HH   HH NN   NNN EE         SS      SS
// RR  RR   OOOO    UUUUU   GGGG  HH   HH NN    NN EEEEEE SSSSS   SSSSS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Algorithm that assesses surface roughness based on a polynomial fit.  Runs a
// moving window across the DEM and assesses the variability of surface normals
// within that window.  Specifically the components of the normals are combined
// into an orientation matrix, which is then solved to find the eigenvalues s1,
// s2, s3 (Woodcock, 1977).
//
// Added by DTM 13/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Get directional cosines
void LSDRaster::calculate_polyfit_directional_cosines(Array2D<float>& d, Array2D<float>& e,
                                    Array2D<float>& l, Array2D<float>& m, Array2D<float>& n)
{
	float pheta, phi;
	// reset the l, m and n matrices (the directional cosines matrices)
	Array2D<float> temp_coef(NRows,NCols,NoDataValue);

	l = temp_coef.copy();
	m = temp_coef.copy();
	n = temp_coef.copy();
	// get surface normals (in form of direction cosines) for each point in DEM
	for (int row = 0; row<NRows; row++)
	{
		for(int col = 0; col<NCols; col++)
		{
			if (d[row][col] != NoDataValue)
			{
				pheta = atan(sqrt(d[row][col]*d[row][col]+e[row][col]*e[row][col]));
				if (e[row][col] == 0 || d[row][col] == 0)
				{
					phi = NoDataValue;
				}
        		else
        		{
					phi = atan(e[row][col]/d[row][col]);
				}

        		// Directional cosines of normal vectors
        		if (pheta != 0)
        		{ // l and m are undefined if pheta = 0 because phi is undefined for a vertical surface normal
          			l[row][col]=sin(pheta)*cos(phi);
          			m[row][col]=sin(pheta)*sin(phi);
        		}

        		// Can define n for vertical surface normal, since pheta is always defined
        		n[row][col]=cos(pheta);
			}
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Find eigenvalues for orientation matrix
//
// Added by DTM 13/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::calculate_orientation_matrix_eigenvalues(float window_radius,
										Array2D<float>& l, Array2D<float>& m,
										Array2D<float>& n, Array2D<float>& s1,
                    					Array2D<float>& s2, Array2D<float>& s3)
{
	// Reset the eigenvalue matrices
	Array2D<float> temp_coef(NRows,NCols,0.0);

	//Array2D<float> temp_coef(NRows,NCols,NoDataValue);
	s1 = temp_coef.copy();
	s2 = temp_coef.copy();
	s3 = temp_coef.copy();

	// this fits a polynomial surface over a kernel window. First, perpare the kernel
	int kr = int(ceil(window_radius/DataResolution));       // Set radius of kernel => suggest = 1 cell
	int kw=2*kr+1;                    						// width of kernel
	float li,mi,ni;

	// Declare kernel arrays
	Array2D<float> l_kernel(kw,kw,NoDataValue);
	Array2D<float> m_kernel(kw,kw,NoDataValue);
	Array2D<float> n_kernel(kw,kw,NoDataValue);

	// Build circular mask
  	Array2D<int> mask(kw,kw,0);
  	float x_kernel,y_kernel, radial_dist;
  	for(int i=0;i<kw;++i)
	{
	    for(int j=0;j<kw;++j)
	    {
			x_kernel=(i-kr)*DataResolution;
	      	y_kernel=(j-kr)*DataResolution;
        	radial_dist = sqrt(y_kernel*y_kernel + x_kernel*x_kernel); // distance from centre to this point.
        	if (floor(radial_dist) <= window_radius)
        	{
				mask[i][j] = 1;
			}
      	}
	}

	// Loop over DEM, extacting the directional cosines for the data kernel
	for(int row=0;row<NRows;++row)
	{
		for(int col=0;col<NCols;++col)
	  	{
      		//Avoid edges
      		if(row-kr < 0 || row+kr+1 > NRows || col-kr < 0 || col+kr+1 > NCols)
      		{
				s1[row][col]=NoDataValue;
          		s2[row][col]=NoDataValue;
          		s3[row][col]=NoDataValue;
          	}
          	else
          	{
        		//Avoid NoDataValues
        		if(l[row][col] != NoDataValue)
        		{
          			// Construct orientation matrix and solve to retrieve eigenvalues for data window
          			// Build orientation matrix
          			Array2D<float> T(3,3,0.0);
          			Array2D<float> D(3,3);
          			int N=1;
          			for(int i=0;i<kw;++i)
          			{
            			for(int j=0;j<kw;++j)
            			{
							if (mask[i][j]==1)
            			  	{
								li=l[row-kr+i][col-kr+j];
              			  		mi=m[row-kr+i][col-kr+j];
              			  		ni=n[row-kr+i][col-kr+j];
						  	    T[0][0] += li*li;
						  	    T[0][1] += li*mi;
						  	    T[0][2] += li*ni;
						  	    T[1][0] += mi*li;
						  	    T[1][1] += mi*mi;
						  	    T[1][2] += mi*ni;
						  	    T[2][0] += ni*li;
						  	    T[2][1] += ni*mi;
						  	    T[2][2] += ni*ni;
						  	    ++N;
							}
            			}
          			}
          			// Find eigenvalues of the orientation matrix
          			Eigenvalue<float> eigenvalue_matrix(T);
          			eigenvalue_matrix.getD(D);
          			// Normalised eigenvalues (with respect to number of normals):
          			//cout << T << endl;
          			s1[row][col]=D[0][0]/N;
          			s2[row][col]=D[1][1]/N;
          			s3[row][col]=D[2][2]/N;
        		}
      		}
	  	}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is a wrapper to get the three roughness eigenvalues s1, s2 and
// s3.
//
//DTM 15/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::calculate_roughness_rasters(float window_radius, float roughness_radius,
										string file_prefix, vector<int> file_code)

{
	// set up polyfit arrays
	Array2D<float> a;
	Array2D<float> b;
	Array2D<float> c;
	Array2D<float> d;
	Array2D<float> e;
	Array2D<float> f;

	int n_vec_entries = file_code.size();
	if ( n_vec_entries !=3)
	{
		cout << endl << "LSDRaster.calcualte_and_print_polyfit_rasters error" << endl;
		cout << "You have the wrong number of entries in the file code vector; taking no action!!!" << endl << endl;
	}
	else
	{
		int window_int = int(window_radius);
		float decimal = window_radius-float(window_int);
		float decimal_ten = decimal*10;
		int decimal_ten_str = int(decimal_ten);
		string window_number_str = itoa(window_int);
		string remainder_str = itoa(decimal_ten_str);
		string p_str = "p";
		string window_size_str = window_number_str+p_str+remainder_str;
		string DEM_flt_extension = "flt";
		string underscore = "_";

		int roughness_int = int(roughness_radius);
		float decimalroughness = roughness_radius-float(roughness_int);
		float decimalroughness_ten = decimalroughness*10;
		int decimalroughness_ten_str = int(decimalroughness_ten);
		string roughness_number_str = itoa(roughness_int);
		string remainderroughness_str = itoa(decimalroughness_ten_str);
		string polystring = "ply";
		string roughstring = "rgh";
		string roughness_size_str = roughness_number_str+p_str+remainderroughness_str;
		roughness_size_str = polystring+window_size_str+underscore+roughstring+roughness_size_str;

		// calcualte polyfit arrays
		calculate_polyfit_coefficient_matrices(window_radius,a, b,c, d, e, f);
    	// analyse variability of normals
    	Array2D<float> l;
		Array2D<float> m;
		Array2D<float> n;
		Array2D<float> s1;
		Array2D<float> s2;
		Array2D<float> s3;
		calculate_polyfit_directional_cosines(d, e, l, m, n);
		calculate_orientation_matrix_eigenvalues(roughness_radius,l,m,n,s1,s2,s3);

		// now go through vector to see which files you want
		if (file_code[0] == 1)
		{
			LSDRaster s1_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s1,GeoReferencingStrings);
      		string s1_name = "_s1_";
			s1_name = file_prefix+s1_name+roughness_size_str;
			s1_raster.write_raster(s1_name,DEM_flt_extension);
		}
		if (file_code[1] == 1)
		{
			LSDRaster s2_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s2,GeoReferencingStrings);
      		string s2_name = "_s2_";
			s2_name = file_prefix+s2_name+roughness_size_str;
			s2_raster.write_raster(s2_name,DEM_flt_extension);
		}
		if (file_code[2] == 1)
		{
			LSDRaster s3_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,s3,GeoReferencingStrings);
      		string s3_name = "_s3_";
			s3_name = file_prefix+s3_name+roughness_size_str;
			s3_raster.write_raster(s3_name,DEM_flt_extension);
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// HH  HH YY   YY DDDD   RRRRR    OOOO   LL       OOOO    GGGGG  YY   YY
// HH  HH  YYYY   DD DD  RR  RR  OO  OO  LL      OO  OO  GG	      YYYY
// HHHHHH   YY    DD  DD RRRR   OO    OO LL     OO    OO GG GGG    YY
// HH  HH   YY    DD DD  RR RR   OO  OO  LL      OO  OO  GG  GG    YY
// HH  HH   YY    DDDD   RR  RR   OOOO   LLLLLL   OOOO    GGGGG    YY
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function looks for data with 0 elevation and changes it to nodata
// Seems to be required when GDAL has been used to manipulate coastal tiles
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::remove_seas()
{
  cout << "Removing seas, NoDataValue is: " << NoDataValue << endl;
  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if(RasterData[row][col] <= 0)
      {
        RasterData[row][col] = NoDataValue;  
      }
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This masks to nodata below a threshold value
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::mask_to_nodata_below_threshold(float threshold)
{
  cout << "masking seas, NoDataValue is: " << NoDataValue << endl;
  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if(RasterData[row][col] <= threshold)
      {
        RasterData[row][col] = NoDataValue;  
      }
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This masks to nodata based on a masking raster
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::mask_to_nodata_with_mask_raster(LSDIndexRaster& Mask_raster, int mask_value)
{
  
  Array2D<float> new_data_raster;
  new_data_raster = RasterData.copy();
  
  // first check to see if the rasters are the same size
  int IR_NRows = Mask_raster.get_NRows();
  int IR_NCols = Mask_raster.get_NCols();
  int IR_NDV   = Mask_raster.get_NoDataValue();
  
  if(IR_NRows == NRows && IR_NCols == NCols)
  {
    for(int row = 0; row<NRows; row++)
    {
      for(int col = 0; col<NCols; col++)
      {
        
        if(Mask_raster.get_data_element(row,col) == IR_NDV || 
           Mask_raster.get_data_element(row,col) == mask_value)
        {
          //cout << "r: " << row << " c: " << col << " mask: " <<Mask_raster.get_data_element(row,col) << endl;
          new_data_raster[row][col] = NoDataValue;        
        }
      }
    }    
  }
  else
  {
    cout << "Trying to mask raster but the dimensions of the mask do not match"
         << " the dimensions of the raster" << endl;
  }

  LSDRaster NDR(NRows,NCols,XMinimum,YMinimum,DataResolution,
	                           NoDataValue,new_data_raster,GeoReferencingStrings);
	return NDR;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Fill
//------------------------------------------------------------------------------
//
//This function fills pits/sinks in a DEM by incrementing elevations for cells with
//no downslope neighbour. The process is repeated adnausium until no cells require
//incrementing.
//
//Inputs required are a DEM file in ascii raster format as created by ARCMap
//and a file name to create a filled DEM grid.
//
//This code was built ontop of code made available by Jon D. Pelletier as part
//of his book:
//
//Pelletier,J.D.,'Quantitative Modelling of Landscapes' Cambridge University Press
//
//---------------------------------------------------------------------------------
//
// v1.3 reduced fill increment to 1mm  to avoid 'overfilling'
//
// Martin Hurst, October 2011
//
//---------------------------------------------------------------------------------
//
// v1.2 modified to read *.flt files
//
// Martin Hurst, November 2010
//
//---------------------------------------------------------------------------------
//
// v1.1 function incorporated to allow the tool to fill adjacent pixels immediately
// after filling a given pixel, should speed things up.
//
// Martin Hurst, October 2010
//
//---------------------------------------------------------------------------------
//
// v1.0 is slow as it requires many iterations through the dem
//
// Martin Hurst, June 2010
//
//---------------------------------------------------------------------------------
LSDRaster LSDRaster::fill()
{

	Array2D<float> FilledRasterData;
	FilledRasterData = RasterData.copy();
	cout << "N_rows is: " << NRows << " " << NCols << endl;
	cout << "Data rows: " << RasterData.dim1() << " cols: " << RasterData.dim2() << endl;
	for (int i=1; i<NRows-1; i++)
	{
		cout << "\rRow = " << i+1 << " / " << NRows << "    ";
		for (int j=1; j<NCols-1; j++)
		{
			//cout << "R: " << i << " C: " << j;
			//cout << " FDR: " << FilledRasterData[i][j];
			if (FilledRasterData[i][j] == NoDataValue || FilledRasterData[i-1][j-1] == NoDataValue
			         || FilledRasterData[i-1][j] == NoDataValue || FilledRasterData[i-1][j+1] == NoDataValue
			         || FilledRasterData[i][j+1] == NoDataValue || FilledRasterData[i+1][j+1] == NoDataValue
			         || FilledRasterData[i+1][j] == NoDataValue || FilledRasterData[i+1][j-1] == NoDataValue
			         || FilledRasterData[i][j-1] == NoDataValue)
			{ }
			else fill_iterator(FilledRasterData,i,j);

			//if (RasterData[i][j] == NoDataValue || RasterData[i-1][j-1] == NoDataValue
			//         || RasterData[i-1][j] == NoDataValue || RasterData[i-1][j+1] == NoDataValue
			//         || RasterData[i][j+1] == NoDataValue || RasterData[i+1][j+1] == NoDataValue
			//         || RasterData[i+1][j] == NoDataValue || RasterData[i+1][j-1] == NoDataValue
			//         || RasterData[i][j-1] == NoDataValue)
			//{ }
			//else fill_iterator(RasterData,i,j);
			//cout << " itercomplete" << endl;
		}
	}
	cout << endl;

	LSDRaster FilledDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,FilledRasterData,GeoReferencingStrings);
	return FilledDEM;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this is a recursive algorithm that is called by the fill function
//
// MDH, 2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::fill_iterator(Array2D<float>& fill_data, int i, int j)
{
	int a=i;
	int b=j;
	float fill_increment = 0.001;
	float min_zeta;
	float centre_zeta = fill_data[a][b];

	if (a==0 || b==0 || a == NRows-1 || b==NCols-1)
	{ }
	else if (fill_data[a][b] == NoDataValue || fill_data[a-1][b-1] == NoDataValue
			 || fill_data[a-1][b] == NoDataValue
	         || fill_data[a-1][b+1] == NoDataValue || fill_data[a][b+1] == NoDataValue
			 || fill_data[a+1][b+1] == NoDataValue || fill_data[a+1][b] == NoDataValue
			 || fill_data[a+1][b-1] == NoDataValue || fill_data[a][b-1] == NoDataValue)
	{}
	else
	{
		min_zeta = centre_zeta + 10;
		if (fill_data[a-1][b-1] < min_zeta) min_zeta = fill_data[a-1][b-1];
		if (fill_data[a-1][b] < min_zeta) min_zeta = fill_data[a-1][b];
		if (fill_data[a-1][b+1] < min_zeta) min_zeta = fill_data[a-1][b+1];
		if (fill_data[a][b+1] < min_zeta) min_zeta = fill_data[a][b+1];
		if (fill_data[a+1][b+1] < min_zeta) min_zeta = fill_data[a+1][b+1];
		if (fill_data[a+1][b] < min_zeta) min_zeta = fill_data[a+1][b];
		if (fill_data[a+1][b-1] < min_zeta) min_zeta = fill_data[a+1][b-1];
		if (fill_data[a][b-1] < min_zeta) min_zeta = fill_data[a][b-1];

		//increase elevation of centre cell if it is lower than or
		//equal in elevation compared to all adjacent cells
		if (centre_zeta <= min_zeta)
		{

      		// efficiency improvement by Dave Milodowski
      		float zeta_diff = min_zeta - centre_zeta;
      		fill_data[a][b] = fill_data[a][b] + zeta_diff + fill_increment;
			//fill adjacent pixels too
			fill_iterator(fill_data,a-1,b-1);
			fill_iterator(fill_data,a-1,b);
			fill_iterator(fill_data,a-1,b+1);
			fill_iterator(fill_data,a,b+1);
			fill_iterator(fill_data,a+1,b+1);
			fill_iterator(fill_data,a+1,b);
			fill_iterator(fill_data,a+1,b-1);
			fill_iterator(fill_data,a,b-1);
		}
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//---------------------------------------------------------------------------------------
//
//	New fill function
//
//	This function fills pits/sinks in a DEM by checking for pits from lowest to highest
//	elevation, starting at the DEM boundary (raster edge or adjacent to NDVs). Utilises
//	a priority queue to progressively populate the stack and pop out the the lowest value
//	before checking that the neighbouring cells that are yet to be visited must be higher
//	in a hydrologically correct DEM. This method is substantially faster on datasets with
//	pits consisting of multiple cells since each cell only needs to be visited once.
//
//	Input argument required -> MinSlope the minimum slope between two Nodes once filled
//	If set to zero will create flats.
//
//	Method taken from Wang and Liu (2006), Int. J. of GIS. 20(2), 193-213
//
//	Martin Hurst, 12/3/13 */
//
//	Declare the node structure
///@brief Used in pit filling to store elevation data and row and colum indexes.
//	Method taken from Wang and Liu (2006), Int. J. of GIS. 20(2), 193-213
//	Method taken from Wang and Liu (2006), Int. J. of GIS. 20(2), 193-213
struct FillNode
{
  /// @brief Elevation data.
	float Zeta;
	/// @brief Row index value.
  int RowIndex;
	/// @brief Column index value.
  int ColIndex;
};

//Overload the less than and greater than operators to consider Zeta data only
//N.B. Fill only needs greater than but less than useful for mdflow routing
//(I've coded this but not yet added to LSDRaster, it's only faster than presorting
//when applied to pretty large datasets).
bool operator>( const FillNode& lhs, const FillNode& rhs )
{
	return lhs.Zeta > rhs.Zeta;
}
bool operator<( const FillNode& lhs, const FillNode& rhs )
{
	return lhs.Zeta < rhs.Zeta;
}

LSDRaster LSDRaster::fill(float& MinSlope)
{
	//cout << "Inside NewFill" << endl;
  //cout << "DataResolution is: " << DataResolution << endl;
  //cout << "Data[200][200]: "  << RasterData[200][200] << endl;
  
	//declare 1/root(2)
	float one_over_root2 = 0.707106781;

	//Declare the priority Queue with greater than comparison
	priority_queue< FillNode, vector<FillNode>, greater<FillNode> > PriorityQueue;
	//Declare a temporary FillNode structure which we populate before adding to the PQ
	//Declare a central node or node of interest
	FillNode TempFillNode, CentreFillNode;

	//declare vectors for slopes and row and col indices
	vector<float> slopes(8,NoDataValue);
	vector<int> row_kernal(8);
	vector<int> col_kernal(8);

	//Get Dimensions
	//int NRows = Zeta.dim1();
	//int NCols = Zeta.dim2();

	//Index array to track whether nodes are in queue or have been processed
	//-9999 = no_data, 0 = data but not processed or in queue,
	//1 = in queue but not processed, 2 = fully processed and removed from queue
	Array2D<int> FillIndex(NRows,NCols,NoDataValue);
	Array2D<float> FilledZeta;
	FilledZeta = RasterData.copy();

	//Collect boundary cells
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			if (FilledZeta[i][j] != NoDataValue)
			{
				//If there is data the cell needs to be filled so
				//set fill index to zero (i.e. yet to be filled)
				FillIndex[i][j] = 0;

				//If we're at the edge or next to an NoDataValue then
				//put the cell into the priority queue
				if (i==0 || j==0 || i==NRows-1 || j==NCols-1 ||
					FilledZeta[i-1][j-1]==NoDataValue || FilledZeta[i-1][j]==NoDataValue ||
					FilledZeta[i-1][j+1]==NoDataValue || FilledZeta[i][j-1]==NoDataValue ||
					FilledZeta[i][j+1]==NoDataValue || FilledZeta[i+1][j-1]==NoDataValue ||
					FilledZeta[i+1][j]==NoDataValue || FilledZeta[i+1][j+1]==NoDataValue)
				{
					TempFillNode.Zeta = FilledZeta[i][j];
					TempFillNode.RowIndex = i;
					TempFillNode.ColIndex = j;
					PriorityQueue.push(TempFillNode);
					FillIndex[i][j] = 1;
				}
			}
		}
	}

	//Loop through the priority queue from lowest to highest elevations
	//filling as we go and adding unassessed neighbours to the priority queue
	while (!PriorityQueue.empty())
	{
		//first get the highest priority node and assign it before
		//removing it from the queue and declaring it processed
		CentreFillNode = PriorityQueue.top();
		int row=CentreFillNode.RowIndex, col=CentreFillNode.ColIndex;
		//cout << "Pop from Queue: Zeta = " << CentreFillNode.Zeta << endl;

		PriorityQueue.pop();
		FillIndex[row][col] = 2;

		//get neighbour indices
		//rows
		row_kernal[0] = row-1;
		row_kernal[1] = row-1;
		row_kernal[2] = row;
		row_kernal[3] = row+1;
		row_kernal[4] = row+1;
		row_kernal[5] = row+1;
		row_kernal[6] = row;
		row_kernal[7] = row-1;
		//cols
		col_kernal[0] = col;
		col_kernal[1] = col+1;
		col_kernal[2] = col+1;
		col_kernal[3] = col+1;
		col_kernal[4] = col;
		col_kernal[5] = col-1;
		col_kernal[6] = col-1;
		col_kernal[7] = col-1;

		//check if on array boundary and set kernal to NoDataValues to avoid
		//segmentation fault
		if (row == 0)
		{
			row_kernal[0] = NoDataValue;
			row_kernal[1] = NoDataValue;
			row_kernal[7] = NoDataValue;
		}
		else if (row==NRows-1)
		{
			row_kernal[3] = NoDataValue;
			row_kernal[4] = NoDataValue;
			row_kernal[5] = NoDataValue;
		}
		if (col == 0)
		{
			col_kernal[5] = NoDataValue;
			col_kernal[6] = NoDataValue;
			col_kernal[7] = NoDataValue;
		}
		else if (col == NCols-1)
		{
			col_kernal[1] = NoDataValue;
			col_kernal[2] = NoDataValue;
			col_kernal[3] = NoDataValue;
		}

		//loop through neighbours
		for (int Neighbour = 0; Neighbour<8; ++Neighbour)
		{
			//If the neighbour has data and is not already in the priority queue and has not been processed
			if (	row_kernal[Neighbour] == NoDataValue || col_kernal[Neighbour] == NoDataValue ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == 1 ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == 2 ||
					FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] == NoDataValue ) {}
			else
			{
				//check if neighbour is equal/lower and therefore needs filling
				if (FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] <= CentreFillNode.Zeta)
				{
					//Modify neighbour's elevation
					if(Neighbour%2 == 0)
					{
						if (MinSlope > 0)
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = 
                               CentreFillNode.Zeta + MinSlope*DataResolution;
						}
						else
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta;
						}
					}
					else
					{
						if (MinSlope > 0)
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta
							                                     + MinSlope*DataResolution*one_over_root2;
						}
						else
						{
							FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]] = CentreFillNode.Zeta;
						}
					}
				}
				//New neighbour needs to be added to the priority queue
				TempFillNode.Zeta = FilledZeta[row_kernal[Neighbour]][col_kernal[Neighbour]];
				TempFillNode.RowIndex = row_kernal[Neighbour];
				TempFillNode.ColIndex = col_kernal[Neighbour];
				PriorityQueue.push(TempFillNode);
				FillIndex[row_kernal[Neighbour]][col_kernal[Neighbour]] = 1;
				FillIndex[row][col] = 2;
			}
		}
	}
	LSDRaster FilledDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,FilledZeta,GeoReferencingStrings);
	return FilledDEM;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=







//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//D-inf modules
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Main function for generating a D-infinity flow area raster after Tarboton (1997).
// Calls the recurisve D_infAccum function to get flow area for each pixel.
// Returns flow area in pixels.
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::D_inf_FlowArea(Array2D<float> FlowDir_array){

  // Arrays of indexes of neighbour cells wrt target cell and their
  //corresponding ranges of angles
  int dX[] = {1, 1, 1, 0, -1, -1, -1, 0};
  int dY[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  float startFD[] = {180, 225, 270, 315, 0, 45, 90, 135};
  float endFD[] = {270, 315, 360, 45, 90, 135, 180, 225};

  Array2D<float> Flowarea_Raster(NRows,NCols,1);
  Array2D<float> CountGrid(NRows,NCols,NoDataValue); //array to hold no of inflowing neighbours

  int inflow_neighbours; //counter for number of inflowing neighbours
  float flowDir; //temp variable to store the flowdir of a neighbour

  // Calculate the number of inflowing neighbours to each cell.
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flowDir = FlowDir_array[i][j];
      if (flowDir != NoDataValue){
        inflow_neighbours = 0;

        for (int c = 0; c < 8; ++c){ //loop through the 8 neighbours of the target cell
          flowDir = FlowDir_array[i + dY[c]][j + dX[c]];
          if (flowDir >= 0 && flowDir <= 360){
            if (c != 3){  //handles the issue of 0,360 both pointing to North
              if (flowDir > startFD[c] && flowDir < endFD[c]){
                ++inflow_neighbours;
              }
            }
            else{
              if (flowDir > startFD[c] || flowDir < endFD[c]){
                ++inflow_neighbours;
              }
            }
          }
        }
        CountGrid[i][j] = inflow_neighbours;
      }
      else{
        Flowarea_Raster[i][j] = NoDataValue;
      }
    }
  }

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (CountGrid[i][j] == 0){ //there are no inflowing neighbours
        //call the flowarea function and travel downstream from it
        D_infAccum(i, j, CountGrid, Flowarea_Raster, FlowDir_array);
      }
    }
  }

  LSDRaster FlowArea(NRows, NCols, XMinimum, YMinimum, DataResolution,
                          NoDataValue, Flowarea_Raster,GeoReferencingStrings);

  return FlowArea;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Recursive function to calculate accumulating area for a given pixel. Called
// by the driver for every cell which has no contributing cells - eg the highest
// points on the landscape. Avoids the need to flatten and sort the DEM as
// required in the original Tarboton (1997) implementation. For more detail on the
// recursive algorithm following channels see Mark (1998) "Network Models in
// Geomorphology".
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::D_infAccum(int i, int j, Array2D<float> CountGrid, 
              Array2D<float> Flowarea_Raster, Array2D<float> FlowDir)
{

  float flowAccumVal = Flowarea_Raster[i][j];
  float flowDir = FlowDir[i][j];

  //tables of angles and indexes used to rotate around each neighbour
  float FD_Low[] = {0, 45, 90, 135, 180, 225, 270, 315};
  float FD_High_361[] = {45, 90, 135, 180, 225, 270, 315, 361};  //this array ends with 361 to catch angles up to 360
  float FD_High[] = {45, 90, 135, 180, 225, 270, 315, 360};
  int Di1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int Dj1[] = {0, 1, 1, 1, 0, -1, -1, -1};
  int Di2[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  int Dj2[] = {1, 1, 1, 0, -1, -1, -1, 0};

  float proportion1 = 0; //proportion of flow to the lowest neighbour
  float proportion2 = 0; //proportion of flow to the second lowest neighbour

  // indexes to store the coordinates of the neighbours where flow is to be routed
  int a1 = 0;
  int b1 = 0;
  int a2 = 0;
  int b2 = 0;

  CountGrid[i][j] = -1; // flags a visted cell

  if (flowDir >= 0){  //avoids flagged pits

    // find which two cells receive flow and the proportion to each
    for (int q = 0; q < 8; ++q){
      if (flowDir >= FD_Low[q] && flowDir < FD_High_361[q]){
        proportion1 = (FD_High[q] - flowDir) / 45;
        a1 = i + Di1[q];
        b1 = j + Dj1[q];
        proportion2 = (flowDir - FD_Low[q]) / 45;
        a2 = i + Di2[q];
        b2 = j + Dj2[q];
        }
    }

      if (proportion1 > 0 && Flowarea_Raster[a1][b1] != NoDataValue){
        Flowarea_Raster[a1][b1] = Flowarea_Raster[a1][b1] + flowAccumVal * proportion1;
        CountGrid[a1][b1] = CountGrid[a1][b1] - 1;
        if (CountGrid[a1][b1] == 0){
          D_infAccum(a1, b1, CountGrid, Flowarea_Raster, FlowDir); //recursive call
        }
      }
      if (proportion2 > 0 && Flowarea_Raster[a2][b2] != NoDataValue){
        Flowarea_Raster[a2][b2] = Flowarea_Raster[a2][b2] + flowAccumVal * proportion2;
        CountGrid[a2][b2] = CountGrid[a2][b2] - 1;
        if (CountGrid[a2][b2] == 0){
          D_infAccum(a2, b2, CountGrid, Flowarea_Raster, FlowDir); //recursive call
      }
    }
  }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// D-infinity flow direction algorithm after Tarboton (1997).
//
// Algorithm takes a filled DEM and for each cell calculates the steepest descent
// based on 8 triangular facets. Flow direction is assigned as an angle from 0-360
// degrees with -1 used to flag unresolved areas such as pits.
//
// Code is ported and optimised from a Java implementation of the algorithm
// supplied under the GNU GPL licence through WhiteBox GAT:
// http://www.uoguelph.ca/~hydrogeo/Whitebox/ and provides identical results
// to the whitebox tool.
//
// SWDG - 26/07/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<float> LSDRaster::D_inf_FlowDir(){

  Array2D<float> FlowDir_Array(NRows,NCols,NoDataValue);

  float maxSlope; //maxiumum slope
  float flowDir = 0; //temp variable to hold flowdirections while looping thru the 8 facets

  float pi = 3.14159265;

  //components of the triangular facets as outlined in Tarboton (1997) fig 3
  //& equations 1-5
  float e0;
  float e1;
  float e2;
  float s1;
  float s2;
  float r;
  float s;

  //Facet elevation factors from Tarboton (1997) Table 1
  int acVals[] = {0, 1, 1, 2, 2, 3, 3, 4};
  int afVals[] = {1, -1, 1, -1, 1, -1, 1, -1};
  int e1Col[] = {1, 0, 0, -1, -1, 0, 0, 1};
  int e1Row[] = {0, -1, -1, 0, 0, 1, 1, 0};
  int e2Col[] = {1, 1, -1, -1, -1, -1, 1, 1};
  int e2Row[] = {-1, -1, -1, -1, 1, 1, 1, 1};

  for (int i = 1; i < NRows - 1; ++i){
    for (int j = 1; j < NCols - 1; ++j){
      e0 = RasterData[i][j];
      if (e0 == NoDataValue){
        FlowDir_Array[i][j] = NoDataValue; //if there is no elevation data we cant have a flowdir
      }
      else{
        maxSlope = -9999999;  //set to a low value that != NDV so any slope will be bigger than it

        for (int a = 0; a < 8; ++a){  //loop through the 8 facets
          e1 = RasterData[i + e1Row[a]][j + e1Col[a]];
          e2 = RasterData[i + e2Row[a]][j + e2Col[a]];
          if (e1 != NoDataValue && e2 != NoDataValue){ //avoid facets lyng in no data
            if (e0 > e1 && e0 > e2){
              //calculate slopes (s1,s2,s) and bearings (r) along edges
              //of the facet when e0 is higher than e1 and e2
              s1 = (e0 - e1) / DataResolution;
              if (s1 == 0){
                s1 = 0.00001;
              }
              s2 = (e1 - e2) / DataResolution;
              r = atan(s2 / s1);
              s = sqrt(s1 * s1 + s2 * s2);

              if (s1 < 0 && s2 < 0){
                s = -1 * s;
              }
              if (s1 < 0 && s2 == 0){
                s = -1 * s;
              }
              if (s1 == 0 && s2 < 0){
                s = -1 * s;
              }
              if (s1 == 0.001 && s2 < 0){
                s = -1 * s;
              }
              if (r < 0 || r > atan(1)){
                if (r < 0){
                  r = 0;
                  s = s1;
                }
                else{
                  r = atan(1);
                  s = (e0 - e2) / (DataResolution * sqrt(2)); //diagonal cell length
                }
              }
              if (s >= maxSlope && s != 0.00001){
                maxSlope = s;
                flowDir = afVals[a] * r + acVals[a] * (pi / 2);
              }
            }
            //calculate slope (s) and bearing (r) along edges
            //of the facet when e0 is higher than e1 or e2
            else if (e0 > e1 || e0 > e2){
              if (e0 > e1){
                r = 0;
                s = (e0 - e1) / DataResolution;
              }
              else{
                r = atan(1);
                s = (e0 - e2) / (DataResolution * sqrt(2));
              }
              if (s >= maxSlope && s != 0.00001){
                maxSlope = s;
                flowDir = afVals[a] * r + acVals[a] * (pi / 2);
              }
            }
          }
        }

        if (maxSlope <= 0){
          FlowDir_Array[i][j] = -1;  //unresolved - Tarboton uses D8 to fill these pits - we have a better fill algorithm
        }
        else{
          flowDir = round((flowDir * (180 / pi)) * 10) / 10;
          flowDir = 360 - flowDir + 90;
          if (flowDir > 360){
            flowDir = flowDir - 360;
          }
          FlowDir_Array[i][j] = flowDir;
        }
      }
    }
  }

  return FlowDir_Array;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Function to write the D-infinity flow directions to an LSDRaster.
//
//SWDG - 26/7/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::write_dinf_flowdir_to_LSDRaster(Array2D<float> dinflow){

  LSDRaster FlowDirection(NRows, NCols, XMinimum, YMinimum, DataResolution,
                          NoDataValue, dinflow,GeoReferencingStrings);

  return FlowDirection;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Wrapper Function to create a D-infinity flow area raster with one function call.
//
//SWDG - 26/7/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::D_inf(){

  Array2D<float> Dinf_flow = D_inf_FlowDir();
  LSDRaster Dinf_area = D_inf_FlowArea(Dinf_flow);

  return Dinf_area;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Wrapper Function to create a D-infinity flow area raster, in spatial units, with one function call.
//
//SWDG - 16/10/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::D_inf_units(){

  Array2D<float> Dinf_flow = D_inf_FlowDir();
  LSDRaster Dinf_area = D_inf_FlowArea(Dinf_flow);

  float cell_area = DataResolution*DataResolution;
  Array2D<float> pixel_area(NRows, NCols, cell_area);

  Array2D<float> Dinf_area_units = Dinf_area.get_RasterData() * pixel_area; 
  LSDRaster Dinf_area_units_raster = Dinf_area.LSDRasterTemplate(Dinf_area_units);

  return Dinf_area_units_raster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Function to multiply D_inf flow accumulation data to drainage area by multiplying through
// by the data resolution. (This could be a duplicate function but I don't have time to check!).
//
// MDH - 27/8/14
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=--=-

LSDRaster LSDRaster::D_inf_ConvertFlowToArea()
{
	float cell_area = DataResolution*DataResolution;
	Array2D<float> Dinf_area_units = RasterData; 
	for (int i=0;i<Dinf_area_units.dim1(); ++i)
	{
		for (int j=0; j<Dinf_area_units.dim2(); ++j)
		{
			if (Dinf_area_units[i][j] != NoDataValue) Dinf_area_units[i][j] = Dinf_area_units[i][j]*cell_area;
		}
	}
			
	return LSDRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Dinf_area_units);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Function to trace upstream from a pour point and extract watershed associated with
// D_inf flow routing
//
// Uses a priority queue ordered by elevation to collect every pixel that drains to a
// particular pour point and returns the resultant basin as an LSDIndexRaster mask
//
// MDH - 26/8/14
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=--=-
LSDIndexRaster LSDRaster::D_inf_watershed(LSDRaster D_inf_FlowDir, int PourRow, int PourCol)
{
	//Declare the priority Queue with greater than comparison
	priority_queue< FillNode, vector<FillNode>, greater<FillNode> > PriorityQueue;
	//Declare a temporary FillNode structure which we populate before adding to the PQ
	//Declare a central node or node of interest
	FillNode TempNode, CentreNode;
	//Index array to track whether nodes are in queue or have been processed
	//-9999 = no_data, 0 = data but not processed or in queue,
	//1 = in queue but not processed, 2 = fully processed and removed from queue
	
	// Arrays of indexes of neighbour cells wrt target cell and their
	//corresponding ranges of angles
	int dX[] = {1, 1, 1, 0, -1, -1, -1, 0};
	int dY[] = {-1, 0, 1, 1, 1, 0, -1, -1};
	float startFD[] = {180, 225, 270, 315, 0, 45, 90, 135};
	float endFD[] = {270, 315, 360, 45, 90, 135, 180, 225};
  	float FlowDir;
  	
	//Index array for collecting catchment area	
	Array2D<int> WatershedArray(NRows,NCols,NoDataValue);
	//Array2D<int> QueueCode(NRows,NCols,NoDataValue);

	//add outlet to priority queue as starting point
	TempNode.Zeta = RasterData[PourRow][PourCol];
	TempNode.RowIndex = PourRow;
	TempNode.ColIndex = PourCol;
	PriorityQueue.push(TempNode);
	
	int row, col;
		  
	//Loop through the priority queue from lowest to highest elevations
	//filling as we go and adding unassessed neighbours to the priority queue
	while (!PriorityQueue.empty())
	{
		//first get the highest priority node and assign it before
		//removing it from the queue and declaring it processed
		CentreNode = PriorityQueue.top();
		row=CentreNode.RowIndex;
		col=CentreNode.ColIndex;
		PriorityQueue.pop();
		
		//loop through the 8 neighbours of the target cell
		for (int c = 0; c < 8; ++c)
		{ 
      	//handle edges here
      	if ((row +dY[c] > NRows-1) || (col + dX[c] > NCols-1) || (row+dY[c]<0) || (col+dY[c]<0)) continue;
      	
      	FlowDir = D_inf_FlowDir.RasterData[row + dY[c]][col + dX[c]];
         
         if (FlowDir >= 0)
         {
         	//handles the issue of 0,360 both pointing to North
            if (c != 3)
            {
            	if ((FlowDir > startFD[c]) && (FlowDir < endFD[c]))
	            {
   	         	if (WatershedArray[row+dY[c]][col+dX[c]] != 1)
   	         	{
   	         		//add contributing node to priority queue
   	         		TempNode.Zeta = RasterData[row+dY[c]][col+dX[c]];
							TempNode.RowIndex = row+dY[c];
							TempNode.ColIndex = col+dX[c];
							PriorityQueue.push(TempNode);
							WatershedArray[row+dY[c]][col+dX[c]] = 1;
   	           	}
   	         }
				}
            else
            {
            	if (FlowDir > startFD[c] || FlowDir < endFD[c])
            	{
            		if (WatershedArray[row+dY[c]][col+dX[c]] != 1)
   	         	{
   	         		//add contributing node to priority queue
   	         		TempNode.Zeta = RasterData[row+dY[c]][col+dX[c]];
							TempNode.RowIndex = row+dY[c];
							TempNode.ColIndex = col+dX[c];
							PriorityQueue.push(TempNode);
							WatershedArray[row+dY[c]][col+dX[c]] = 1;
   	           	}
   	         }
            }
			}
		}
	}
	LSDIndexRaster Watershed(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,WatershedArray);
	return Watershed;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Topographic_Index
// Calculates the topographic index, defined as ln(A/S) (Kirkby, 1975), where A
// is the accumulation area and S is the slope.  This is an indicator of the
// distribution of moisture within the topography.
// DTM 01/04/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::calculate_topographic_index(LSDRaster& AccumulationArea, LSDRaster& Slope)
{
  Array2D<float> TopographicIndexArray(NRows,NCols,NoDataValue);
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      if(RasterData[i][j]!=NoDataValue) TopographicIndexArray[i][j] = log(AccumulationArea.get_data_element(i,j)/Slope.get_data_element(i,j));
    }
  }
  LSDRaster TopographicIndex(NRows,NCols,XMinimum,YMinimum,DataResolution,
                             NoDataValue,TopographicIndexArray,GeoReferencingStrings);
  return TopographicIndex;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//end of d-inf modules
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Generate data in three text files to create a boomerang plot as in Roering et al [2007].
// Added spline curves and return of a pair of LH values - 1/11/13 SWDG. 
// SWDG 27/8/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
pair<float,float> LSDRaster::Boomerang(LSDRaster& Slope, LSDRaster& Dinf, string RasterFilename, float log_bin_width, int SplineResolution, float bin_threshold){
  
  Array2D<float> slope = Slope.get_RasterData();
  Array2D<float> area = Dinf.get_RasterData();

  //do some log binning
  vector<float> Mean_x_out;
  vector<float> Mean_y_out;
  vector<float> Midpoints_out;
  vector<float> STDDev_x_out;
  vector<float> STDDev_y_out;
  vector<float> STDErr_x_out;
  vector<float> STDErr_y_out;
  vector<int> number_observations;
  
  log_bin_data(area, slope, log_bin_width, Mean_x_out, Mean_y_out, Midpoints_out, 
          STDDev_x_out, STDDev_y_out, STDErr_x_out, STDErr_y_out, 
          number_observations, NoDataValue);  
  
  //remove empty bins 
  RemoveSmallBins(Mean_x_out, Mean_y_out, Midpoints_out, STDDev_x_out, 
              STDDev_y_out, STDErr_x_out, STDErr_y_out, number_observations, bin_threshold);
  
  //index value of max slope
  int slope_max_index = distance(Mean_y_out.begin(), max_element(Mean_y_out.begin(), Mean_y_out.end()));

  //hillslope length from the maximum binned values
  float LH = Mean_x_out[slope_max_index]/DataResolution;
      
  // Fit splines through the binned data to get the LH
  vector<float> Spline_X;
  vector<float> Spline_Y;
  PlotCubicSplines(Mean_x_out, Mean_y_out, SplineResolution, Spline_X, Spline_Y);

  //index value of max spline slope
  int slope_max_index_spline = distance(Spline_Y.begin(), max_element(Spline_Y.begin(), Spline_Y.end()));

  //hillslope length from spline curve - maybe get a range of values about the maximum?

  float LH_spline = Spline_X[slope_max_index_spline]/DataResolution;
  
  //set up a filestream object to write the binned data
  ofstream file;

  stringstream ss_bin;
  ss_bin << RasterFilename << "_boom_binned.txt";
  file.open(ss_bin.str().c_str());   //needs a null terminated character array, not a string. See pg 181 of accelerated c++
    
  for(int q = 0; q < int(Mean_x_out.size()); q++){
    file << Mean_x_out[q] << " " << Mean_y_out[q] << " " << STDDev_x_out[q] << " " << STDDev_y_out[q] << " " << STDErr_x_out[q] << " " << STDErr_y_out[q] << endl;
  }
  file.close();
      
  //set up a filestream object to write the spline data
  ofstream SplineFile;

  stringstream ss_spline;
  ss_spline << RasterFilename << "_boom_spline.txt";
  SplineFile.open(ss_spline.str().c_str());   //needs a null terminated character array, not a string. See pg 181 of accelerated c++
    
  for(int q = 0; q < int(Spline_X.size()); q++){ //fixed bug here where I looped over the wrong vector - SWDG 7/11/13
    SplineFile << Spline_X[q] << " " << Spline_Y[q] << endl;

  }
  SplineFile.close();
  
  //set up a filestream object to write the data cloud
  ofstream cloud;

  stringstream ss_cloud;
  ss_cloud << RasterFilename << "_boom_cloud.txt";
  cloud.open(ss_cloud.str().c_str());     //needs a null terminated character array, not a string. See pg 181 of accelerated c++

  for (int i = 1; i < NRows-1; ++i){
    for (int j = 1; j < NCols-1; ++j){
      if(area[i][j] != NoDataValue && slope[i][j] != NoDataValue){
        cloud << area[i][j] << " " << slope[i][j] << endl;
      }
    }
  }
  cloud.close();

  // create a pair containing the two LH values, first is the binned value and second is the spline value
  pair <float,float> LH_Pair;
  LH_Pair = make_pair (LH, LH_spline);
  
  return LH_Pair;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Punch basins out of an LSDRaster to create DEMs of a single catchment.
//
// Writes files in the user supplied format (flt or asc) and returns a vector
// LSDRasters so they can be loaded into other functions.
// Updated 24/9/13 to return a vector of LSDRasters SWDG
// SWDG 27/8/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
vector<LSDRaster> LSDRaster::BasinPuncher(vector<int> basin_ids, LSDIndexRaster BasinArray){

  Array2D<int> BasinRaster = BasinArray.get_RasterData();

  vector<LSDRaster> BasinVector; //vector to contain individual basin LSDRasters

  for(int a = 0; a < int(basin_ids.size()); ++a){

    Array2D<float> BasinDEM(NRows, NCols, NoDataValue);
    bool Flag = false;

    for (int i=0; i<NRows; ++i){
		  for (int j=0; j<NCols; ++j){
		    if(BasinRaster[i][j] == basin_ids[a]){
		      Flag = true;
          BasinDEM[i][j] = RasterData[i][j];
        }
		  }
		}

    if (Flag == true){ //only write the raster if there is data to write
      LSDRaster Basin(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                      BasinDEM,GeoReferencingStrings);
      BasinVector.push_back(Basin);
    }
  }
  return BasinVector;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Collect all basin average metrics into a single file.
//
// File is written with the format:
// "basin_id slope elevation aspect area drainage_density hilltop_curvature hillslope_length mean_slope hilltop_relief hilltop_aspect E* R* LH_bins LH_splines LH_density"
// SWDG 27/8/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::CollectBasinMetrics(LSDIndexRaster& Basins, LSDRaster& Slope, LSDRaster& Elevation, LSDRaster& Aspect,
                              LSDRaster& Area, LSDRaster& DrainageDensity, LSDRaster& Cht, LSDRaster& HillslopeLength,
                              LSDRaster& MeanSlope, LSDRaster& Relief, LSDRaster& MeanAspect, LSDRaster& LH_drainage_density, Array2D<float> LH_Data, float CriticalSlope, string RasterFilename)
{

  Array2D<int> basin_ids = Basins.get_RasterData();

  //vectors to contain output data
  vector<int> BasinIDVector;
  vector<float> SlopeVector;
  vector<float> ElevationVector;
  vector<float> AspectVector;
  vector<float> AreaVector;
  vector<float> DrainageDensityVector;
  vector<float> ChtVector;
  vector<float> HillslopeLengthVector;
  vector<float> MeanSlopeVector;
  vector<float> ReliefVector;
  vector<float> MeanAspectVector;
  vector<float> LHFromDDVector;
  vector<float> EStarVector;
  vector<float> RStarVector;

  //Get vector of unique basin indexes
  vector<int> basin_index = Unique(basin_ids, NoDataValue);

  //loop through each basin
  for (vector<int>::iterator it = basin_index.begin(); it !=  basin_index.end(); ++it){

    int SlopeCounter = 0;
    float SlopeSum = 0;
    float ElevationSum = 0;
    int ElevationCounter = 0;
    float AspectSum = 0;
    int AspectCounter = 0;
    float AreaSum = 0;
    int AreaCounter = 0;
    float DrainageDensitySum = 0;
    int DrainageDensityCounter = 0;
    float ChtSum = 0;
    int ChtCounter = 0;
    float HillslopeLengthSum = 0;
    int HillslopeLengthCounter = 0;
    float MeanSlopeSum = 0;
    int MeanSlopeCounter = 0;
    float ReliefSum = 0;
    int ReliefCounter = 0;
    float MeanAspectSum = 0;
    int MeanAspectCounter = 0;
    float LHFromDD = 0;

    for (int i = 0; i < NRows; ++i){
      for (int j = 0; j < NCols; ++j){

       if (Slope.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         SlopeSum += Slope.get_data_element(i,j);
         ++SlopeCounter;
        }
       if (Elevation.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         ElevationSum += Elevation.get_data_element(i,j);
         ++ElevationCounter;
        }
       if (Aspect.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         AspectSum += Aspect.get_data_element(i,j);
         ++AspectCounter;
        }
       if (Area.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         AreaSum += Area.get_data_element(i,j);
         ++AreaCounter;
        }
       if (DrainageDensity.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         DrainageDensitySum += DrainageDensity.get_data_element(i,j);
         ++DrainageDensityCounter;
        }
       if (Cht.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         ChtSum += Cht.get_data_element(i,j);
         ++ChtCounter;
        }
       if (HillslopeLength.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         HillslopeLengthSum += HillslopeLength.get_data_element(i,j);
         ++HillslopeLengthCounter;
        }
       if (MeanSlope.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         MeanSlopeSum += MeanSlope.get_data_element(i,j);
         ++MeanSlopeCounter;
        }
       if (Relief.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         ReliefSum += Relief.get_data_element(i,j);
         ++ReliefCounter;
        }
       if (MeanAspect.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         MeanAspectSum += MeanAspect.get_data_element(i,j);
         ++MeanAspectCounter;
        }
       if (LH_drainage_density.get_data_element(i,j) != NoDataValue && basin_ids[i][j] == *it ){
         LHFromDD = LH_drainage_density.get_data_element(i,j); //this is already a basin average value
        }
      }
    }

    //calculate means
    float AVGSlope = SlopeSum/SlopeCounter;
    float AVGElevation = ElevationSum/ElevationCounter;
    float AVGAspect = AspectSum/AspectCounter;
    float AVGArea = AreaSum/AreaCounter;
    float AVGDrainageDensity = DrainageDensitySum/DrainageDensityCounter;
    float AVGCht = ChtSum/ChtCounter;
    float AVGHillslopeLength = HillslopeLengthSum/HillslopeLengthCounter;
    float AVGMeanSlope = MeanSlopeSum/MeanSlopeCounter;
    float AVGRelief = ReliefSum/ReliefCounter;
    float AVGMeanAspect = MeanAspectSum/MeanAspectCounter;
    float EStar = (2 * (abs(AVGCht)) * AVGHillslopeLength) / CriticalSlope;
    float RStar = AVGRelief / (AVGHillslopeLength * CriticalSlope);

    //write means to vectors
    BasinIDVector.push_back(*it);
    SlopeVector.push_back(AVGSlope);
    ElevationVector.push_back(AVGElevation);
    AspectVector.push_back(AVGAspect);
    AreaVector.push_back(AVGArea);
    DrainageDensityVector.push_back(AVGDrainageDensity);
    ChtVector.push_back(AVGCht);
    HillslopeLengthVector.push_back(AVGHillslopeLength);
    MeanSlopeVector.push_back(AVGMeanSlope);
    ReliefVector.push_back(AVGRelief);
    MeanAspectVector.push_back(AVGMeanAspect);
    LHFromDDVector.push_back(LHFromDD);
    EStarVector.push_back(EStar);
    RStarVector.push_back(RStar);

  }

  stringstream filename;
  filename << RasterFilename << "_BasinMetrics.txt";
  ofstream file;
  cout << filename.str() << endl;
  file.open(filename.str().c_str());
  file << "basin_id slope elevation aspect area drainage_density hilltop_curvature hillslope_length mean_slope hilltop_relief hilltop_aspect E* R* LH_bins LH_splines LH_density" << endl;


  for(int q = 0; q < int(BasinIDVector.size()); q++){
    
    float LH_bins = 0.0;
    float LH_splines = 0.0;
    
    for (int k = 0; k < int(LH_Data.dim1()); ++k){
      if (LH_Data[k][0] == BasinIDVector[q]){ //if we have LH data for this basin id then write it to the variables
        LH_bins = LH_Data[k][1];
        LH_splines = LH_Data[k][2];
      }
    }   
  
    file << BasinIDVector[q] << " " << SlopeVector[q] << " " << ElevationVector[q] << " " << AspectVector[q] << " " << AreaVector[q] << " " << DrainageDensityVector[q] << " " << ChtVector[q] << " " << HillslopeLengthVector[q] << " " << MeanSlopeVector[q] <<  " " << ReliefVector[q] << " " << MeanAspectVector[q] << " " << EStarVector[q] << " " << RStarVector[q] << " " << LH_bins << " " << LH_splines << " " << LHFromDDVector[q] << endl;
  }

  file.close();

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Raster_statistics_by_index
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
// A generic function for calculating commonly used statistics on a raster that
// has been separated into discrete categories (marked by index raster) that are
// to be considered separately.  An example is to calculate basin metrics
// for a bunch of drainage basins labelled with a unique ID (although there is a 
// specific function to carry out this particular analysis, this type of problem
// is sufficiently common to warrant a standalone function).
// 
// Takes a raster and a corresponding index raster, and calculates the mean, sd
// and standard error by index.  Returns four vectors: mean, st.dev., st.err and 
// Number of points for each category.
// DTM 28/11/2013 (adapting SDWG's DrainageDensity function)
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::raster_statistics_by_index(LSDIndexRaster& IndexRaster, 
                   vector<float>& mean_vector, vector<float>& SD_vector,
                   vector<float>& SErr_vector, vector<int>& NPts_vector)
{
  int intNoDataValue = IndexRaster.get_NoDataValue();
  //Declare all the variables needed in this method
  vector<int> IDs;
  vector<int> IDs_sorted;
  vector<size_t> index_map;
  int q = 0;
  //convert Basin Raster to an Array
  Array2D<int> ids = IndexRaster.get_RasterData(); 
  //Loop over every pixel and record the value and basin ID into two vectors
  for (int i = 0; i < NRows; ++i)
  {
    for (int j = 0; j < NCols; ++j)
    {
      if (ids[i][j] != intNoDataValue)
      {         
          IDs.push_back(ids[i][j]);        
      }    
    }
  }
  int N_IDs = IDs.size();
  vector<float> Values(N_IDs, NoDataValue);
  vector<float> SortedValues;
  int vector_index = 0;
  for (int i = 0; i < NRows; ++i)
  {
    for (int j = 0; j < NCols; ++j)
    {
      if (ids[i][j] != intNoDataValue)
      {         
        if(RasterData[i][j] != NoDataValue)
        {
          Values[vector_index]=RasterData[i][j];        
        }
        ++vector_index;
      }    
    }
  }
  //sort our two vectors based on the Basin IDs: has the effect of grouping each basin together in 1D space
  matlab_int_sort(IDs, IDs_sorted, index_map);               
  matlab_float_reorder(Values, index_map, SortedValues);
  
  // get the ID
  cout << "Now getting stats for values by index" << endl;
  int start_id = IDs_sorted[0];
  vector<float> values_for_this_ID;
  float mean, SD, SErr;
  while (q < int(IDs_sorted.size()))
  {
    //cout << q << "/" << IDs_sorted.size() << endl;
    if (start_id == IDs_sorted[q])
    {
      if (SortedValues[q]!=NoDataValue) values_for_this_ID.push_back(SortedValues[q]);
      ++q;
    }
    else
    {
      // get mean, sd and serr.
      if (values_for_this_ID.empty() == true)
      {
        mean = NoDataValue;
        SD = NoDataValue;
        SErr = NoDataValue;
      }
      else
      {
        mean = get_mean(values_for_this_ID);
        SD = get_standard_deviation(values_for_this_ID, mean);
        SErr = get_standard_error(values_for_this_ID, SD);
      }  
      //record the mean, sd and serr into the maps
      mean_vector.push_back(mean);
      SD_vector.push_back(SD);
      SErr_vector.push_back(SErr);
      NPts_vector.push_back(values_for_this_ID.size());
      
      values_for_this_ID.clear();
      start_id = IDs_sorted[q];      
    }
  }
  // Process the final basin once the loop is completed
  if (values_for_this_ID.empty() == true)
  {
    mean = NoDataValue;
    SD = NoDataValue;
    SErr = NoDataValue;
  }
  else
  { 
    mean = get_mean(values_for_this_ID);
    SD = get_standard_deviation(values_for_this_ID, mean);
    SErr = get_standard_error(values_for_this_ID, SD);
  }  
  cout << "kicking out output vectors" << endl;
  mean_vector.push_back(mean);
  SD_vector.push_back(SD);
  SErr_vector.push_back(SErr);
  NPts_vector.push_back(values_for_this_ID.size());
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module to sample LSDRaster values running along a ridgetop network. Pass in
// a TNT array of floats that denotes the ridge network. Ridge network is
// generated from LSDJunctionNetwork::ExtractRidges
//
// Returns sampled LSDRaster object
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeSample(Array2D<float>& Ridges){

  Array2D<float> Sample_data(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue && Ridges[i][j] != NoDataValue ){
        Sample_data[i][j] = RasterData[i][j];
      }
    }
  }

  LSDRaster Sample(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, 
                   Sample_data,GeoReferencingStrings);
  return Sample;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Pass a smoothing window over a ridge LSDRaster object to calculate an average
// value running along the ridgetop.
//
// Pass in an optional integer smoothing window radius between 1 and 6.
// Default value is 2
//
// Returns LSDRaster object containing the averaged data.
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeSmoother(int WindowRadius){

  //arbitrary upper bound to limit too large a radius
  if (WindowRadius < 1 || WindowRadius > 6){
    WindowRadius = 2; //
  }

  Array2D<float> Smoothed(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      float sum = 0;
      int counter = 0;
      if (RasterData[i][j] != NoDataValue){
          for (int a = (-1*WindowRadius); a <= WindowRadius; ++a){
              for (int b = (-1*WindowRadius); b <= WindowRadius; ++b){
                if(a >= 0 && a <= NRows){
                  if(b >= 0 && b <= NCols){
                    if (RasterData[i+a][j+b] != NoDataValue){
                        sum += RasterData[i+a][j+b];
                        ++counter;
                    }
                  }
                }
             }
          }
      Smoothed[i][j] = sum/counter;
      }
    }
  }

  LSDRaster Smooth(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, 
                   Smoothed,GeoReferencingStrings);
  return Smooth;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Pass a buffer over a ridge LSDRaster object to increase sampling area.
//
// Pass in an optional integer buffer radius between 1 and 6.
// Default value is 2
//
// Returns LSDRaster object denoting the buffered ridge data.
//
// Buffers equally in all directions, so use with care to avoid sampling areas
// away from the axis of the original ridge line.
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::RidgeBuffer(int BufferRadius){

  Array2D<float> HilltopBuffer(NRows, NCols, NoDataValue);

  //arbitrary upper bound to limit too large a buffer
  if (BufferRadius < 1 || BufferRadius > 6){
    BufferRadius = 2;
  }

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue){
        for (int a = (-1 * BufferRadius); a <= BufferRadius; ++a){
          for (int b = (-1 * BufferRadius); b <= BufferRadius; ++b){
            if(i + a >= 0 && i + a <= NRows){
              if(j + b >= 0 && j + b <= NCols){
                HilltopBuffer[i + a][j + b] = RasterData[i][j];
              }
            }
          }
        }
      }
    }
  }

  LSDRaster Buffer(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                   NoDataValue, HilltopBuffer,GeoReferencingStrings);
  return Buffer;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module assigns an average CHT (or other input LSDRaster value) to each basin,
// works by searching for every hilltop value that falls within a basin, summing
// these values and writing the final average to every cell identified as the
// basin in question.
//
// Pass in an LSDIndexRaster of Drainage basins, generated using
// ChannelNetwork::ExtractBasinsOrder
//
// Returns LSDRaster of average basin CHT for each identified basin.
//
// Very inefficent at present. Module loops through every cell in LSDRaster
// (2 * number of basins) + 1 times. Beware!
// Bug fixed in assignment of basin IDs - SWDG 2/9/13
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::BasinAverager(LSDIndexRaster& Basins){

  Array2D<int> basin_ids = Basins.get_RasterData();
  Array2D<float> Averaged(NRows,NCols,NoDataValue);
  //Get vector of unique basin indexes
  vector<int> basin_index = Unique(basin_ids, NoDataValue);

  //loop through each basin
  for (vector<int>::iterator it = basin_index.begin(); it !=  basin_index.end(); ++it){
    int counter = 0;
    float sum = 0;

    for (int i = 0; i < NRows; ++i){
      for (int j = 0; j < NCols; ++j){

       if (RasterData[i][j] != NoDataValue && basin_ids[i][j] == *it ){
         sum += RasterData[i][j];
         ++counter;
        }
      }
    }

    for (int i = 0; i < NRows; ++i){
      for (int j = 0; j < NCols; ++j){
        if(basin_ids[i][j] == *it){
          Averaged[i][j] = sum/counter;
        }
      }
    }
  }

  LSDRaster Averaged_out(NRows,NCols, XMinimum, YMinimum, DataResolution, 
                            NoDataValue, Averaged,GeoReferencingStrings);
  return Averaged_out;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// Write the area(in spatial units of area) of each basin to the basin's pixels.
// Refactored to follow the drainage density calculations design pattern.
// SWDG 20/11/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::BasinArea(LSDIndexRaster Basins){
  //Declare all the variables needed in this method
  vector<int> IDs;
  vector<int> IDs_sorted;
  vector<size_t> index_map;
  int q = 0;
  float area_sum = 0;
  map <int,float> Basin_Areas; //structure to hold pairs of area values with a basin ID as a key  
  Array2D<float> Areas(NRows,NCols,NoDataValue);  //Output raster
    
  //convert Basin Raster to an Array
  Array2D<int> basin_ids = Basins.get_RasterData();  
  
  //Loop over every pixel and record it's basin ID in a vectors  
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (basin_ids[i][j] != NoDataValue){                 
        IDs.push_back(basin_ids[i][j]);
      }    
    }
  }
  
  //sort our vectors based on the Basin IDs: has the effect of grouping each basin together in 1D space
  // so if we count each pixel and multiply it by the pixel area we get basin area
  matlab_int_sort(IDs, IDs_sorted, index_map);               
  
  // get the first basin ID
  int start_id = IDs_sorted[0];

  while (q < int(IDs_sorted.size())){
    if (start_id == IDs_sorted[q]){
      ++area_sum;  
      ++q;
    }
    else{
      //record the area in the map using the Basin ID as a key
      Basin_Areas[start_id] = area_sum * (DataResolution * DataResolution);   
                    
      area_sum = 0;
      start_id = IDs_sorted[q];      
    }
  }
  
  // Process the final basin once the loop is completed
  Basin_Areas[start_id] = area_sum * (DataResolution * DataResolution);
  
  //write data to the output raster
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (basin_ids[i][j] != NoDataValue){
        Areas[i][j] = Basin_Areas[basin_ids[i][j]];        
      }
    }
  }

  //write the array to an LSDRaster and return it
  LSDRaster Areas_out(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                      NoDataValue, Areas,GeoReferencingStrings);
  return Areas_out;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// Convert a basin, given by a basin ID, into a chain of xy coordinates for
// fast plotting of vector basin outlines.
//
// Produces a generalised polygon and will not cope well with complex geometries.
//
// Needs to be updated to write data into an esri ascii format so the files can 
// be loaded into arc.  
// SWDG 21/11/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::GetBasinVector(LSDIndexRaster Basins, int BasinOfInterest){

  //convert Basin Raster to an Array
  Array2D<int> basin_ids = Basins.get_RasterData(); 
  
  vector<float> I;
  vector<float> J;

  int NDVCount = 0;

  for (int i = 1; i < NRows-1; ++i){
    for (int j = 1; j < NCols-1; ++j){
    
      NDVCount = 0;
      
      if (basin_ids[i][j] == BasinOfInterest){
      
        //count border cells that are NDV
        if (basin_ids[i-1][j-1] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i][j-1] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i+1][j-1] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i-1][j] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i+1][j] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i-1][j+1] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i][j+1] != BasinOfInterest){ ++NDVCount; }
        if (basin_ids[i+1][j+1] != BasinOfInterest){ ++NDVCount; }
        
        if (NDVCount >= 4 && NDVCount < 8){  //increase the first value to get a simpler polygon
          //edge pixel
          I.push_back(i);
          J.push_back(j);
        }
      }
    }
  }

  //now have 2 vectors of i and j indexes of every point
  
  int q = 0;
  
  vector<float> X; //could prealloc here based on I.size()
  vector<float> Y;
  
  while (q < int(I.size())){
    
    //convert I,J into x,y          
    X.push_back((J[q] * DataResolution) + XMinimum);
    Y.push_back(((I[q] - NRows) * DataResolution) + YMinimum);
    ++q;
  } 
   
   //get centroid
  float mean_x = get_mean(X);
  float mean_y = get_mean(Y);
 
  //vector to contain the angles between each point and the centroid
  vector<float> A; //could prealloc here too

  int k = 0;
  
  float an = 0;

  while (k < int(Y.size())){
    //calculate angle between each point and the centroid
    an = atan2(Y[k] - mean_y, X[k] - mean_x);
    A.push_back(an);
    ++k;
  }
  
  //sort the data by angle and reorder the coordinates based on the sort
  vector<float> A_sorted;
  vector<size_t> index_map;
  vector<float> Reordered_X;
  vector<float> Reordered_Y;

  matlab_float_sort(A, A_sorted, index_map);
  matlab_float_reorder(X, index_map, Reordered_X);
  matlab_float_reorder(Y, index_map, Reordered_Y);

  //write the data to a file
  ofstream write_chain;

  write_chain.open("chain.txt");

  int w = 0;
  while (w < int(X.size())){
    write_chain << Reordered_X[w] << " " << Reordered_Y[w] << endl;
    ++w;
  }
  
  //close the chain
  write_chain << Reordered_X[0] << " " << Reordered_Y[0] << endl;
  write_chain.close(); 

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// Calulate drainage density of a set of input basins.
//
// Calculated as flow length/basin area.
//
// Refactored to optimise performance. SWDG - 19/11/13 
//
// SWDG 04/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::DrainageDensity(LSDIndexRaster& StreamNetwork, LSDIndexRaster& Basins, Array2D<int> FlowDir){

  //Declare all the variables needed in this method
  float two_times_root2 = 2.828427;
  vector<float> Lengths;
  vector<float> SortedLengths; 
  vector<int> IDs;
  vector<int> IDs_sorted;
  vector<size_t> index_map;
  int q = 0;
  float length_sum = 0;
  float area_sum = 0;
  map <int,float> Basin_DD; //structure to hold pairs of DD values with a basin ID as a key  
  Array2D<float> Density(NRows,NCols,NoDataValue);  //Output raster
    
  //convert Basin Raster to an Array
  Array2D<int> basin_ids = Basins.get_RasterData();  
  
  //Loop over every pixel and record it's stream length and basin ID in two vectors  
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (basin_ids[i][j] != NoDataValue){         
        if (StreamNetwork.get_data_element(i,j) != NoDataValue){
          if ((FlowDir[i][j] % 2) != 0 && (FlowDir[i][j] != -1 )){ //is odd but not -1
            Lengths.push_back(DataResolution * two_times_root2); //diagonal
            IDs.push_back(basin_ids[i][j]);
          }
          else if (FlowDir[i][j] % 2 == 0){  //is even
            Lengths.push_back(DataResolution); //cardinal
            IDs.push_back(basin_ids[i][j]);       
          }
        }
        else{
          Lengths.push_back(0.0);
          IDs.push_back(basin_ids[i][j]);  
        }
      }    
    }
  }
  
  //sort our two vectors based on the Basin IDs: has the effect of grouping each basin together in 1D space
  matlab_int_sort(IDs, IDs_sorted, index_map);               
  matlab_float_reorder(Lengths, index_map, SortedLengths);
  
  // get the first basin ID
  int start_id = IDs_sorted[0];

  while (q < int(IDs_sorted.size())){
    if (start_id == IDs_sorted[q]){
      length_sum += SortedLengths[q];
      area_sum += (DataResolution * DataResolution);  
      ++q;
    }
    else{
      Basin_DD[start_id]=(length_sum/area_sum);   //record the DD density in the map using the Basin ID as a key
      length_sum = 0;
      area_sum = 0;
      start_id = IDs_sorted[q];      
    }
  }
  
  // Process the final basin once the loop is completed
  Basin_DD[start_id]=(length_sum/area_sum);
  
  //write data to the output raster
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (basin_ids[i][j] != NoDataValue){
        Density[i][j] = Basin_DD[basin_ids[i][j]];        
      }
    }
  }

  //write the array to an LSDRaster and return it
  LSDRaster DrainageDensity(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                            NoDataValue, Density,GeoReferencingStrings);
  return DrainageDensity;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Simple method to convert a drainage density raster into a hillslope length raster. 
// Returns a Raster of basin average hillslope lengths. The LH value is calculated using 
// LH = 1/2*DD [Tucker et al 2001]. 
// SWDG - 7/11/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::HillslopeLengthFromDrainageDensity(LSDIndexRaster& StreamNetwork, 
                       LSDIndexRaster& Basins, Array2D<int> FlowDir)
{

  LSDRaster DrainageDensity = LSDRaster::DrainageDensity(StreamNetwork, Basins, FlowDir);
  Array2D<float> HillslopeLength(NRows,NCols,NoDataValue);
  
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (DrainageDensity.get_data_element(i,j) != NoDataValue){
        HillslopeLength[i][j] = 1 / (2 * DrainageDensity.get_data_element(i,j));
      }
    }
  }
  
  LSDRaster HillslopeLengthRaster = DrainageDensity.LSDRasterTemplate(HillslopeLength);
  return HillslopeLengthRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM and routes the flow accordingly. Consequently the dem is sorted and indexed
// using LSDStatsTools.
//
// Can handle DEMs containing flats, but pits must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//
// Updated 23/4/13 to allow periodic boundary condtitions - SWDG
// Needs to be able to handle the boundary cond vector and only reflect bondaries when needed.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::MDFlow(vector<string> BoundaryConditions)
{

  int Row2 = NRows + 2;
  int Col2 = NCols + 2;

  //create output array, populated with nodata
  Array2D<float> area_final(NRows, NCols, NoDataValue);

  Array2D<float> elev_pad(Row2, Col2, NoDataValue);
  Array2D<float> area(Row2, Col2, NoDataValue);

  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map;
  float one_ov_root_2 = 0.707106781187;


  //translate the data down and right by one cell to create a border
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
        elev_pad[i+1][j+1] = RasterData[i][j];
    }
  }

  //reflect boundaries
  for (int i = 0; i < Row2; ++i){
    for (int j = 0; j < Col2; ++j){
      if(i==0 && j != 0 && j != Col2-1){
        //top row
        elev_pad[i][j] = elev_pad[Row2-2][j];
      }
      if(i==Row2-1 && j != 0 && j != Col2-1){
        //bottom row
        elev_pad[i][j] = elev_pad[1][j];
      }

      if(j==0 && i != 0 && i != Row2-1){
        //left column
        elev_pad[i][j] = elev_pad[i][Col2-2];
      }

      if(j==Col2-1 && i!=0 && i != Row2-1){
        //right column
        elev_pad[i][j] = elev_pad[i][1];
      }
    }
  }

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every non ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(elev_pad[i][j]);
      if (elev_pad[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  float total = 0;
			  float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        float slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (elev_pad[i][j] > elev_pad[i-1][j-1] && elev_pad[i-1][j-1] != NoDataValue){
          slope1 = (elev_pad[i][j] - elev_pad[i-1][j-1]) * one_ov_root_2;
          total += slope1;
        }
			  if (elev_pad[i][j] > elev_pad[i-1][j] && elev_pad[i-1][j] != NoDataValue){
          slope2 = elev_pad[i][j] - elev_pad[i-1][j];
          total += slope2;
			  }
		  	if (elev_pad[i][j] > elev_pad[i-1][j+1] && elev_pad[i-1][j+1] != NoDataValue){
          slope3 = (elev_pad[i][j] - elev_pad[i-1][j+1]) * one_ov_root_2;
          total += slope3;
		  	}
			  if (elev_pad[i][j] > elev_pad[i][j+1] && elev_pad[i][j+1] != NoDataValue){
          slope4 = elev_pad[i][j] - elev_pad[i][j+1];
          total += slope4;
        }
			  if (elev_pad[i][j] > elev_pad[i+1][j+1] && elev_pad[i+1][j+1] != NoDataValue){
          slope5 = (elev_pad[i][j] - elev_pad[i+1][j+1]) * one_ov_root_2;
          total += slope5;
		  	}
			  if (elev_pad[i][j] > elev_pad[i+1][j] && elev_pad[i+1][j] != NoDataValue){
          slope6 = elev_pad[i][j] - elev_pad[i+1][j];
          total += slope6;
        }
			  if (elev_pad[i][j] > elev_pad[i+1][j-1] && elev_pad[i+1][j-1] != NoDataValue){
          slope7 = (elev_pad[i][j] - elev_pad[i+1][j-1]) * one_ov_root_2;
          total += slope7;
			  }
			  if (elev_pad[i][j] > elev_pad[i][j-1] && elev_pad[i][j-1] != NoDataValue){
          slope8 = elev_pad[i][j] - elev_pad[i][j-1];
          total += slope8;
        }

			//if no slope is found (total == 0), pass flow to all equal elevation cells]
      if (total == 0) {
			  if(elev_pad[i-1][j-1] == elev_pad[i][j]){
          slope1 = 1;
          total += 1;
        }
				if(elev_pad[i-1][j] == elev_pad[i][j]){
				  slope2 = 1;
          total += 1;
        }
				if(elev_pad[i-1][j+1] == elev_pad[i][j]){
				  slope3 = 1;
          total += 1;
        }
				if(elev_pad[i][j+1] == elev_pad[i][j]){
				  slope4 = 1;
          total += 1;
        }
			  if(elev_pad[i+1][j+1] == elev_pad[i][j]){
				  slope5 = 1;
          total += 1;
        }
				if(elev_pad[i+1][j] == elev_pad[i][j]){
				  slope6 = 1;
          total += 1;
        }
				if(elev_pad[i+1][j-1] == elev_pad[i][j]){
          slope7 = 1;
          total += 1;
        }
				if(elev_pad[i][j-1] == elev_pad[i][j]){
          slope8 = 1;
          total += 1;
        }
			}

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }

  //translate the data back
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
        area_final[i][j] = area[i+1][j+1];
    }
  }

  //write output LSDRaster object
  LSDRaster MultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                      NoDataValue, area_final,GeoReferencingStrings);
  return MultiFlow;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM, and weights them using the equation from Freeman et al 1991 and routes the
// flow accordingly.
//
// Paper link: http://www.sciencedirect.com/science/article/pii/009830049190048I
//
// Cardinal Weighting = (elevation_drop/total_elevation_drop)^1.1
// Diagonal Weighting = ((elevation_drop/total_elevation_drop)*(1/root(2)))^1.1
//
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::FreemanMDFlow(){

  //create output array, populated with nodata
  Array2D<float> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map;
  float one_ov_root_2 = 0.707106781187;
  float p = 1.1; //value avoids preferential flow to diagonals

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  float total = 0;
			  float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        float slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = pow(((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2),p);
          total += slope1;
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = pow((RasterData[i][j] - RasterData[i-1][j]),p);
          total += slope2;
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = pow(((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2),p);
          total += slope3;
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = pow((RasterData[i][j] - RasterData[i][j+1]),p);
          total += slope4;
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = pow(((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2),p);
          total += slope5;
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = pow((RasterData[i][j] - RasterData[i+1][j]),p);
          total += slope6;
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = pow(((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2),p);
          total += slope7;
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = pow((RasterData[i][j] - RasterData[i][j-1]),p);
          total += slope8;
        }

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }
  //write output LSDRaster object
  LSDRaster FreemanMultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                             NoDataValue, area,GeoReferencingStrings);
  return FreemanMultiFlow;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Route flow from one source pixel using FreemanMDFlow.  Adapted from SWDG's
// code above.
// DTM 07/11/2013
LSDRaster LSDRaster::FreemanMDFlow_SingleSource(int i_source,int j_source)
{

  //create output array, populated with nodata
  Array2D<float> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map;
  float one_ov_root_2 = 0.707106781187;
  float p = 1.1; //value avoids preferential flow to diagonals

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i)
  {
    for (int j = 0; j < NCols; ++j)
    {
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue)
      {
        area[i][j] = 0;
      }
    }
  }
  area[i_source][j_source] = DataResolution*DataResolution;
  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);
  bool reached_source = false; 
		  
  for(int q = 0 ;q < int(flat.size()); ++q)
  {

    if (sorted[q] != NoDataValue)
    {
      //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;
      // test to see whether we have reached the source
      if(i==i_source && j==j_source) reached_source=true;
      //skip edge cells and cells above the source pixel
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1 && reached_source == true){

        //reset variables on each loop
			  float total = 0;
			  float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        float slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = pow(((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2),p);
          total += slope1;
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = pow((RasterData[i][j] - RasterData[i-1][j]),p);
          total += slope2;
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = pow(((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2),p);
          total += slope3;
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = pow((RasterData[i][j] - RasterData[i][j+1]),p);
          total += slope4;
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = pow(((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2),p);
          total += slope5;
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = pow((RasterData[i][j] - RasterData[i+1][j]),p);
          total += slope6;
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = pow(((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2),p);
          total += slope7;
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = pow((RasterData[i][j] - RasterData[i][j-1]),p);
          total += slope8;
        }

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      } 
    }
  }
  //write output LSDRaster object
  LSDRaster FreemanMultiFlowSingleSource(NRows, NCols, XMinimum, YMinimum, 
                 DataResolution, NoDataValue, area,GeoReferencingStrings);
  return FreemanMultiFlowSingleSource;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This extracts the valley network from previously idenified channel heads
// using workflow outlined in Pelletier (2013)
// DTM 27/06/2014
LSDRaster LSDRaster::FMDChannelsFromChannelHeads(vector<int>& channel_heads_rows, 
                              vector<int>& channel_heads_cols, float R_threshold)
{
  int n_sources = channel_heads_rows.size();
  //create output array, populated with nodata
  Array2D<float> area(NRows, NCols, NoDataValue);
  Array2D<int> upslope_channel_heads(NRows, NCols, int(NoDataValue));
  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map; 
  float one_ov_root_2 = 0.707106781187;
  float p = 1.1; //value avoids preferential flow to diagonals

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i)
  {
    for (int j = 0; j < NCols; ++j)
    {
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue)
      {
        area[i][j] = 0;
        upslope_channel_heads[i][j] = 0;
      }
    }
  }

  int row,col;
  for(int i = 0; i<n_sources; ++i)
  {
    row = channel_heads_rows[i];
    col = channel_heads_cols[i];
    area[row][col] = DataResolution*DataResolution;
    upslope_channel_heads[row][col] = 1;
  }
  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);
  for(int q = 0 ;q < int(flat.size()); ++q)
  {

    if (sorted[q] != NoDataValue)
    {
      //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells and cells above the source pixel
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  float total = 0;
			  float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        float slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = pow(((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2),p);
          total += slope1;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i-1][j-1];
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = pow((RasterData[i][j] - RasterData[i-1][j]),p);
          total += slope2;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i-1][j];
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = pow(((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2),p);
          total += slope3;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i-1][j+1];
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = pow((RasterData[i][j] - RasterData[i][j+1]),p);
          total += slope4;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i][j+1];
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = pow(((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2),p);
          total += slope5;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i+1][j+1];
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = pow((RasterData[i][j] - RasterData[i+1][j]),p);
          total += slope6;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i+1][j];
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = pow(((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2),p);
          total += slope7;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i+1][j-1];
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = pow((RasterData[i][j] - RasterData[i][j-1]),p);
          total += slope8;
          if(upslope_channel_heads[i][j] >=1) ++upslope_channel_heads[i][j-1];
        }

        //divide slope by total to get the proportion of flow directed to each cell
        //and increment the downslope cells. If no downslope flow to a node, 0 is
        //added, so no change is seen.
  			if(total>0)
        {
          area[i-1][j-1] += (area[i][j] * (slope1)/total);
    			area[i-1][j] += (area[i][j] * (slope2)/total);
    			area[i-1][j+1] += (area[i][j] * (slope3)/total);
    			area[i][j+1] += (area[i][j] * (slope4)/total);
    			area[i+1][j+1] += (area[i][j] * (slope5)/total);
    			area[i+1][j] += (area[i][j] * (slope6)/total);
    			area[i+1][j-1] += (area[i][j] * (slope7)/total);
    			area[i][j-1] += (area[i][j] * (slope8)/total);
    		}
      } 
    }
  }
  Array2D<float> MDChannelArray(NRows,NCols,NoDataValue);
  // Now reduce the channel network according to upslope pixels
  float test_value;
  for(int i = 0; i< NRows; ++i)
  {
    for(int j = 0; j<NCols; ++j)
    {
      if(upslope_channel_heads[i][j] >= 1 && RasterData[i][j]!=NoDataValue)
      {
        test_value = area[i][j]/float(upslope_channel_heads[i][j]);
        if(test_value >=R_threshold) MDChannelArray[i][j]=area[i][j];
        
      }
    }
  }                              
  //write output LSDRaster object
  LSDRaster ChannelRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                       NoDataValue, MDChannelArray,GeoReferencingStrings);
  return ChannelRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM, and weights them using the equation from Quinn et al 1991 and routes the
// flow accordingly.
//
// Paper link: http://onlinelibrary.wiley.com/doi/10.1002/hyp.3360050106/abstract
//
// Cardinal Weighting = (elevation_drop/total_elevation_drop)*DataResolution/2
// Diagonal Weighting =
//      ((elevation_drop/total_elevation_drop)*(1/root(2)))* DataResolution*0.354
//
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG, 18/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::QuinnMDFlow(){

  //create output array, populated with nodata
  Array2D<float> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map;
  float one_ov_root_2 = 0.707106781187;
  float Lc = DataResolution/2; //cardinal scaling factor
  float Ld = DataResolution * 0.354; //diagonal scaling factor


  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  float total = 0;
			  float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        float slope8 = 0;

        //Get sum of magnitude of downslope flow, total, and store the magnitude of
        //each of the 8 downslope cells as slope1->8 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope1 = ((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2) * Ld;
          total += slope1;
        }
			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope2 = (RasterData[i][j] - RasterData[i-1][j]) * Lc;
          total += slope2;
			  }
		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope3 = ((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2) * Ld;
          total += slope3;
		  	}
			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope4 = (RasterData[i][j] - RasterData[i][j+1]) * Lc;
          total += slope4;
        }
			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope5 = ((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2) * Ld;
          total += slope5;
		  	}
			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope6 = (RasterData[i][j] - RasterData[i+1][j]) * Lc;
          total += slope6;
        }
			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope7 = ((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2) * Ld;
          total += slope7;
			  }
			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope8 = (RasterData[i][j] - RasterData[i][j-1]) * Lc;
          total += slope8;
        }

      //divide slope by total to get the proportion of flow directed to each cell
      //and increment the downslope cells. If no downslope flow to a node, 0 is
      //added, so no change is seen.
			area[i-1][j-1] += area[i][j] * (slope1/total);
			area[i-1][j] += area[i][j] * (slope2/total);
			area[i-1][j+1] += area[i][j] * (slope3/total);
			area[i][j+1] += area[i][j] * (slope4/total);
			area[i+1][j+1] += area[i][j] * (slope5/total);
			area[i+1][j] += area[i][j] * (slope6/total);
			area[i+1][j-1] += area[i][j] * (slope7/total);
			area[i][j-1] += area[i][j] * (slope8/total);
      }
    }
  }
  //write output LSDRaster object
  LSDRaster QuinnMultiFlow(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                           NoDataValue, area,GeoReferencingStrings);
  return QuinnMultiFlow;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function built around original c++ code by Martin Hurst to generate a flowarea
// raster.
//
// Computes the proportion of all downslope flows for each cell in the input
// DEM. Finds the cell of the steepest descent and then checks the two
// neighbouring cells slopes. If either is also downslope proportion flow
// between the steepest cell and the steepest neighbour. If neither neighbour
// is downslope 100% of flow follows the steepest path.
//
// Can *NOT* handle DEMs containing flats or pits -  must be filled using the new
// LSDRaster fill.
//
// Outputs an LSDRaster
//
// SWDG - 02/08/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::M2DFlow(){

  //create output array, populated with nodata
  Array2D<float> area(NRows, NCols, NoDataValue);

  //declare variables
  vector<float> flat;
  vector<float> sorted;
  vector<size_t> index_map;
  float one_ov_root_2 = 0.707106781187;

  //loop through the dem cells creating a row major 1D vector, flat, and
  //setting the cell area to every npn ndv cell
  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      flat.push_back(RasterData[i][j]);
      if (RasterData[i][j] != NoDataValue){
        area[i][j] = DataResolution*DataResolution;
      }
    }
  }

  //sort the 1D elevation vector and produce an index
  matlab_float_sort_descending(flat, sorted, index_map);

  for(int q = 0 ;q < int(flat.size()); ++q){

    if (sorted[q] != NoDataValue){

		  //use row major ordering to reconstruct each cell's i,j coordinates
  	  int i = index_map[q] / NCols;
   	  int j = index_map[q] % NCols;

      //skip edge cells
      if (i != 0 && j != 0 && i != NRows-1 && j != NCols-1){

        //reset variables on each loop
			  float slope0 = 0;
        float slope1 = 0;
        float slope2 = 0;
        float slope3 = 0;
        float slope4 = 0;
        float slope5 = 0;
        float slope6 = 0;
        float slope7 = 0;
        vector<float> slopes;

        float p1 = 0;
        float p2 = 0;
        int second_slope = -1; //initialized using value outside of range.

        //Get magnitude of downslope flow slope0->7 *Avoids NDVs*
			  if (RasterData[i][j] > RasterData[i-1][j-1] && RasterData[i-1][j-1] != NoDataValue){
          slope0 = ((RasterData[i][j] - RasterData[i-1][j-1]) * one_ov_root_2);
          slopes.push_back(slope0);
        }
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i-1][j] && RasterData[i-1][j] != NoDataValue){
          slope1 = (RasterData[i][j] - RasterData[i-1][j]);
          slopes.push_back(slope1);
			  }
        else {
          slopes.push_back(0);
        }

		  	if (RasterData[i][j] > RasterData[i-1][j+1] && RasterData[i-1][j+1] != NoDataValue){
          slope2 = ((RasterData[i][j] - RasterData[i-1][j+1]) * one_ov_root_2);
          slopes.push_back(slope2);
		  	}
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i][j+1] && RasterData[i][j+1] != NoDataValue){
          slope3 = (RasterData[i][j] - RasterData[i][j+1]);
          slopes.push_back(slope3);
        }
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i+1][j+1] && RasterData[i+1][j+1] != NoDataValue){
          slope4 = ((RasterData[i][j] - RasterData[i+1][j+1]) * one_ov_root_2);
          slopes.push_back(slope4);
		  	}
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i+1][j] && RasterData[i+1][j] != NoDataValue){
          slope5 = (RasterData[i][j] - RasterData[i+1][j]);
          slopes.push_back(slope5);
        }
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i+1][j-1] && RasterData[i+1][j-1] != NoDataValue){
          slope6 = ((RasterData[i][j] - RasterData[i+1][j-1]) * one_ov_root_2);
          slopes.push_back(slope6);
			  }
        else {
          slopes.push_back(0);
        }

			  if (RasterData[i][j] > RasterData[i][j-1] && RasterData[i][j-1] != NoDataValue){
          slope7 = (RasterData[i][j] - RasterData[i][j-1]);
          slopes.push_back(slope7);
        }
        else {
          slopes.push_back(0);
        }

        if (int(slopes.size()) > 0 ){   //catch outlets with no neighbours to drain to

          //find maximum slope & its index location in the slopes vector
          float S_max = *max_element(slopes.begin(), slopes.end());
          int S_max_index = find(slopes.begin(), slopes.end(), S_max) - slopes.begin();

          //find steepest neighbour
          if (S_max_index == 0){
            if (slope7 > 0 && slope1 == 0){
              second_slope = 7;
            }
            if (slope7 == 0 && slope1 > 0){
              second_slope = 1;
            }
            if (slope7 > 0 && slope1 > 0){
              if (slope7 > slope1){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope7 == slope1){
              second_slope = 0;
            }
          }

          if (S_max_index == 1){
            if (slope0 > 0 && slope2 == 0){
              second_slope = 7;
            }
            if (slope0 == 0 && slope2 > 0){
              second_slope = 1;
            }
            if (slope0 > 0 && slope2 > 0){
              if (slope0 > slope2){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope0 == slope2){
              second_slope = 0;
            }
          }

          if (S_max_index == 2){
            if (slope1 > 0 && slope3 == 0){
              second_slope = 7;
            }
            if (slope1 == 0 && slope3 > 0){
              second_slope = 1;
            }
            if (slope1 > 0 && slope3 > 0){
              if (slope1 > slope3){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope1 == slope3){
              second_slope = 0;
            }
          }

          if (S_max_index == 3){
            if (slope2 > 0 && slope4 == 0){
              second_slope = 7;
            }
            if (slope2 == 0 && slope4 > 0){
              second_slope = 1;
            }
            if (slope2 > 0 && slope4 > 0){
              if (slope2 > slope4){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope2 == slope4){
              second_slope = 0;
            }
          }

          if (S_max_index == 4){
            if (slope3 > 0 && slope5 == 0){
              second_slope = 7;
            }
            if (slope3 == 0 && slope5 > 0){
              second_slope = 1;
            }
            if (slope3 > 0 && slope5 > 0){
              if (slope3 > slope5){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope3 == slope5){
              second_slope = 0;
            }
          }

          if (S_max_index == 5){
            if (slope4 > 0 && slope6 == 0){
              second_slope = 7;
            }
            if (slope4 == 0 && slope6 > 0){
              second_slope = 1;
            }
            if (slope4 > 0 && slope6 > 0){
              if (slope4 > slope6){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope4 == slope6){
              second_slope = 0;
            }
          }


          if (S_max_index == 6){
            if (slope5 > 0 && slope7 == 0){
              second_slope = 7;
            }
            if (slope5 == 0 && slope7 > 0){
              second_slope = 1;
            }
            if (slope5 > 0 && slope7 > 0){
              if (slope5 > slope7){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope5 == slope7){
              second_slope = 0;
            }
          }

          if (S_max_index == 7){
            if (slope6 > 0 && slope0 == 0){
              second_slope = 7;
            }
            if (slope6 == 0 && slope0 > 0){
              second_slope = 1;
            }
            if (slope6 > 0 && slope0 > 0){
              if (slope6 > slope0){
                second_slope = 7;
              }
              else{
                second_slope = 1;
              }
            }
            if (slope6 == slope0){
              second_slope = 0;
            }
          }

          //get proportions p1 and p2
          if (second_slope != S_max_index){
            p1 = S_max/(S_max + slopes[second_slope]);
            p2 = slopes[second_slope]/(S_max + slopes[second_slope]);
          }
          else{ //flow only in 1 direction
            p1 = 1;
            p2 = 0;
          }

          //partition flow following the steepest slope and it's steepest neighbour
          if (S_max_index == 0 && area[i-1][j-1] != NoDataValue){
            area[i-1][j-1] += area[i][j] * p1;
            if (second_slope == 1){
              area[i-1][j] += area[i][j] * p2;
            }
            if (second_slope == 7){
              area[i][j-1] += area[i][j] * p2;
            }
          }

          if (S_max_index == 1 && area[i-1][j] != NoDataValue){
            area[i-1][j] += area[i][j] * p1;
            if (second_slope == 2){
              area[i-1][j+1] += area[i][j] * p2;
            }
            if (second_slope == 0){
              area[i-1][j-1] += area[i][j] * p2;
            }
          }

          if (S_max_index == 2 && area[i-1][j+1] != NoDataValue){
            area[i-1][j+1] += area[i][j] * p1;
            if (second_slope == 3){
              area[i][j+1] += area[i][j] * p2;
            }
            if (second_slope == 1){
              area[i-1][j] += area[i][j] * p2;
            }
          }

          if (S_max_index == 3 && area[i][j+1] != NoDataValue){
            area[i][j+1] += area[i][j] * p1;
            if (second_slope == 4){
              area[i+1][j+1] += area[i][j] * p2;
            }
            if (second_slope == 2){
              area[i-1][j+1] += area[i][j] * p2;
            }
          }

          if (S_max_index == 4 && area[i+1][j+1] != NoDataValue){
            area[i+1][j+1] += area[i][j] * p1;
            if (second_slope == 5){
              area[i+1][j] += area[i][j] * p2;
            }
            if (second_slope == 3){
              area[i][j+1] += area[i][j] * p2;
            }
          }

          if (S_max_index == 5 && area[i+1][j] != NoDataValue){
            area[i+1][j] += area[i][j] * p1;
            if (second_slope == 6){
              area[i+1][j-1] += area[i][j] * p2;
            }
            if (second_slope == 4){
              area[i+1][j+1] += area[i][j] * p2;
            }
          }

          if (S_max_index == 6 && area[i+1][j-1] != NoDataValue){
            area[i+1][j-1] += area[i][j] * p1;
            if (second_slope == 7){
              area[i][j-1] += area[i][j] * p2;
            }
            if (second_slope == 5){
              area[i+1][j] += area[i][j] * p2;
            }
          }

          if (S_max_index == 7 && area[i][j-1] != NoDataValue){
            area[i][j-1] += area[i][j] * p1;
            if (second_slope == 0){
              area[i-1][j] += area[i][j] * p2;
            }
            if (second_slope == 6){
              area[i-1][j-1] += area[i][j] * p2;
            }
          }
        }
      }
    }
  }

  //write output LSDRaster object
  LSDRaster Multi2Flow(NRows, NCols, XMinimum, YMinimum, DataResolution, 
                       NoDataValue, area,GeoReferencingStrings);
  return Multi2Flow;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// PREDICTING CHANNEL HEADS USING TANGENTIAL CURVATURE
//
// This function is used to predict channel head locations based on the method
// proposed by Pelletier (2013).  It creates a contour curvature map and identifies
// channel heads as pixels greater than a user defined contour curvature threshold
// value, set by default at 0.1.  The threshold curvature can also be defined as a
// multiple of the standard deviation of the curvature.  Before this function is
// called the DEM must be filtered using the wiener filter in the LSDRasterSpectral
// object in order to remove high frequency noise.
//
// Reference: Pelletier (2013) A robust, two-parameter method for the extraction of
// drainage networks from high-resolution digital elevation models (DEMs): Evaluation
// using synthetic and real-world DEMs, Water Resources Research 49: 1-15
//
// added by FC 16/07/13
//
// edited by DTM 07/11/13
// Initial function gave a map of pixels with sufficient tangential curvature to be
// designated as a channel.  This map needed to be reduced to give the source pixels
// only.  This is done by i) sorting all the possible sources by elevation and ii)
// routing flow from each potential source using Freeman MD flow.  Any potential
// sources that are located on ANY down-slope pathway from previously visited source
// pixels are excluded from the final source map. 
// 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LSDIndexRaster LSDRaster::calculate_pelletier_channel_heads(float tan_curv_threshold, Array2D<float>& tan_curv_array)
// {
//   Array2D<float> curv_array(NRows,NCols,NoDataValue);
//   vector<int> possible_sources_row;
//   vector<int> possible_sources_col;
//   vector<float> possible_sources_elev;
//   // Get all the locations where the tan curvature is greater than the user defined threshold
//   for (int row = 0; row < NRows; row++)
// 	{
//     for(int col = 0; col < NCols; col++)
//     {
//       if (tan_curv_array[row][col] > tan_curv_threshold)
//       {
//         possible_sources_row.push_back(row);
//         possible_sources_col.push_back(col);
//         possible_sources_elev.push_back(RasterData[row][col]);
//         curv_array[row][col] = tan_curv_array[row][col];
//       }
//     }
//   }
//   //LSDRaster AboveThreshold(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,curv_array);
//   //AboveThreshold.write_raster("thresh","flt");
//   // Now sort possible sources by elevation, then route flow using d-inf,
//   // excluding potential sources that are on downslope pathway from other
//   // sources
// //   int n_possible_sources = possible_sources_elev.size();
// //   if (n_possible_sources <= 0)
// //   {
// //     cout << "NO SOURCES FOUND" << endl;
// //   }
// //   else
// //   {
// //     cout << "Found " << n_possible_sources << " possible source pixels" << endl;
// //   }
//   vector<size_t> index_map;
// 
//   // sort
//   matlab_float_sort_descending(possible_sources_elev, possible_sources_elev, index_map);
//   matlab_int_reorder(possible_sources_row, index_map, possible_sources_row);
//   matlab_int_reorder(possible_sources_col, index_map, possible_sources_col);
//   LSDIndexRaster SourcesRaster = IdentifyFurthestUpstreamSourcesWithFreemanMDFlow(possible_sources_row,possible_sources_col,tan_curv_array);
//    
//   return SourcesRaster;
// }

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculate the minimum bounding rectangle for an LSDRaster Object and crop out
// all the surrounding NoDataValues to reduce the size and load times of output
// rasters.
//
// Ideal for use with chi analysis tools which output basin and chi m value rasters
// which can be predominantly no data. As an example, a 253 Mb file can be reduced to
// ~5 Mb with no loss or resampling of data.
//
// Modded 6/11/14 to cope with bil files and to catch cases where some or all of the
// edges cannot be trimmed - SWDG
//
// Returns A trimmed LSDRaster object.
//
// SWDG 22/08/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::RasterTrimmer(){

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

  if (min_row == 0 && min_col == 0 && max_row == (NRows - 1) && max_col == (NCols - 1)){
    cout << "Raster cannot be trimmed! \nReturning original raster.\n" << endl;
        
    LSDRaster Output(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, 
                     RasterData, GeoReferencingStrings);
                     
    return Output;
  }
   
  // create new row and col sizes taking account of zero indexing
  int new_row_dimension = (max_row-min_row) + 1;
  int new_col_dimension = (max_col-min_col) + 1;

  Array2D<float>TrimmedData(new_row_dimension, new_col_dimension, NoDataValue);

  //loop over min bounding rectangle and store it in new array of shape new_row_dimension x new_col_dimension
  int TrimmedRow = 0;
  int TrimmedCol = 0;
  
  //check if the north or west edges cannot be trimmed and stop an out of bounds error
  if (min_row == 0){ min_row = 1; }
  if (min_col == 0){ min_col = 1; }
  
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
  
  //Check if the file is in *.bil format and if it is update the GeoReferencingStrings
  if (!GeoReferencingStrings.empty()){
    float YMax = new_YLL + (new_row_dimension* DataResolution);
    GeoReferencingStrings = Update_GeoReferencingStrings(new_XLL,YMax);
  }
    
  LSDRaster TrimmedRaster(new_row_dimension, new_col_dimension, new_XLL,
                          new_YLL, DataResolution, NoDataValue, TrimmedData, GeoReferencingStrings);

  return TrimmedRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This is a raster trimmer that gets a rectangular DEM that doesn't have NoData
// around the edges
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::RasterTrimmerSpiral()
{
  int North_node = 0;
  int South_node = NRows-1;
  int East_node = NCols-1;
  int West_node = 0;
  int row,col;
  
  bool found_NDV;
  
  // mark the borders as not found
  bool found_north_border = false;
  bool found_south_border = false;
  bool found_east_border = false;
  bool found_west_border = false; 
  
  // the border row and columns are set to nodata
  int North_border = NoDataValue;
  int South_border = NoDataValue;
  int West_border = NoDataValue;
  int East_border = NoDataValue;
  
  bool AllBordersFound = false;
  
  // enter the spiral routine
  while(North_node < South_node && East_node > West_node && AllBordersFound == false)
  {
    //cout << "Nn: " << North_node << " En: " << East_node << " Sn: " << South_node << " Wn: " << West_node << endl;
    //cout << "Nb: " << North_border << " Eb: " << East_border << " Sb: " << South_border << " Wb: " << West_border << endl;
      
    // first do north edge
    if(found_north_border == false)
    {
      col = West_node;
      row = North_node;
      found_NDV = false;
      while(col<=East_node && found_NDV == false)
      {
        if( RasterData[row][col] == NoDataValue)
        {
          //cout << "N Found nodata: ["<<row<<"]["<< col << "]"<< endl;
          found_NDV = true;
          North_node++;
        }  
        col++;
      }
      if (found_NDV == false)
      {
        //cout << "I found the north border!" << endl;
        found_north_border = true;
        North_border = North_node;
      }   
    }
    
    // now do the east edge
    if(found_east_border == false)
    {
      row = North_node;
      col = East_node;
      found_NDV = false;
      while(row<=South_node && found_NDV == false)
      {
        if( RasterData[row][col] == NoDataValue)
        {
          //cout << "E Found nodata: ["<<row<<"]["<< col << "]"<< endl;
          found_NDV = true;
          East_node--;
        } 
        row++; 
      }
      if (found_NDV == false)
      {
        found_east_border = true;
        East_border = East_node;
      }        
    }

    // now do the south edge
    if(found_south_border == false)
    {
      row = South_node;
      col = West_node;
      found_NDV = false;
      while(col<=East_node && found_NDV == false)
      {
        if( RasterData[row][col] == NoDataValue)
        {
          //cout << "S Found nodata: ["<<row<<"]["<< col << "]"<< endl;
          found_NDV = true;
          South_node--;
        } 
        col++; 
      }
      if (found_NDV == false)
      {
        found_south_border = true;
        South_border = South_node;
      }        
    }

    // now do the west edge
    if(found_west_border == false)
    {
      row = North_node;
      col = West_node;
      found_NDV = false;
      while(row<=South_node && found_NDV == false)
      {
        if( RasterData[row][col] == NoDataValue)
        {
          //cout << "W Found nodata: ["<<row<<"]["<< col << "]"<< endl;
          found_NDV = true;
          West_node++;
        } 
        row++; 
      }
      if (found_NDV == false)
      {
        found_west_border = true;
        West_border = West_node;
      } 
    }       
    
    // check to see if all the borders have been found
    if(North_border != NoDataValue && South_border != NoDataValue
         && East_border != NoDataValue && West_border != NoDataValue)
    {
      AllBordersFound = true;
    }
  }

  //cout << endl << endl << "Here are the borders: " << endl;
  //cout << "Nn: " << North_node << " En: " << East_node << " Sn: " << South_node << " Wn: " << West_node << endl;
  //cout << "Nb: " << North_border << " Eb: " << East_border << " Sb: " << South_border << " Wb: " << West_border << endl;
      

  int min_row = North_node;
  int max_row = South_node;
  int min_col = West_node;
  int max_col = East_node;

  // create new row and col sizes taking account of zero indexing
  int new_row_dimension = (max_row-min_row) + 1;
  int new_col_dimension = (max_col-min_col) + 1;
  
  //cout << "New dimensions are: rows: " << new_row_dimension << " cols: "
  //     << new_col_dimension << endl;

  Array2D<float>TrimmedData(new_row_dimension, new_col_dimension, NoDataValue);

  //loop over min bounding rectangle and store it in new array of shape new_row_dimension x new_col_dimension
  int TrimmedRow = 0;
  int TrimmedCol = 0;
  for (int row = min_row; row < max_row; ++row){
    for(int col = min_col; col < max_col; ++col){
      TrimmedData[TrimmedRow][TrimmedCol] = RasterData[row][col];
      ++TrimmedCol;
    }
    ++TrimmedRow;
    TrimmedCol = 0;
  }

  //calculate lower left corner coordinates of new array
  float new_XLL = (min_col * DataResolution) + XMinimum;
  float new_YLL = YMinimum + ((NRows - max_row - 1) * DataResolution);

  LSDRaster TrimmedRaster(new_row_dimension, new_col_dimension, new_XLL,
                          new_YLL, DataResolution, NoDataValue, TrimmedData, GeoReferencingStrings);  

  TrimmedRaster.Update_GeoReferencingStrings();

  return TrimmedRaster;
  
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-




//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Export input LSDRasters as a vector field which can be plotted in python.
// Data is written in the format "i j Magnitude Direction"
// SWDG 20/1/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::GetVectors(LSDRaster Magnitude, LSDRaster Direction, string output_file, int step)
{

  vector<string> OutputData;

  for (int i = 0; i < NRows; i+=step){
    for (int j = 0; j < NCols; j+=step){
    
      if (Magnitude.get_data_element(i,j) != NoDataValue && Direction.get_data_element(i,j) != NoDataValue){
        stringstream output_line;
        output_line << i << " " << j << " " << Magnitude.get_data_element(i,j) << " " << Direction.get_data_element(i,j);
        OutputData.push_back(output_line.str());         
      } 
    
    }
  }

  ofstream WriteData;
  WriteData.open(output_file.c_str());
  
  WriteData << "i j Magnitude Direction" << endl; 

  for (int k = 0; k < int(OutputData.size()); ++k){
  
    WriteData << OutputData[k] << endl;
  
  }

  WriteData.close();
  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//		 sSSSs MM   MM  oOOo   oOOo  TTTTTT HH  HH IIII NN   NN  gGGGG
//		SS     M M M M oO  Oo oO  Oo   TT   HH  HH  II  NNN  NN GG
//		 sSSs  M  M  M OO  OO OO  OO   TT   HHHHHH  II  NN N NN GG GGG
//		    SS M     M oO  Oo oO  Oo   TT   HH  HH  II  NN  NNN GG  GG
//		sSSSs  M     M  oOOo   oOOo    TT   HH  HH IIII NN   NN  GGGGG
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//  Perform Non-local means filtering on a DEM following Baude et al. [2005]
//  Smoothes non-gaussian noise.
//
//  Inputs required:
//    the search window radius,
//    the similarity window radius and
//    the degree of filtering
//
//  Martin Hurst, February, 2012
//  Modified by David Milodowski, May 2012- generates grid of recording filtered noise
//
//  WindowRadius has to be <= SimilarityRadius ?
//
//  Adapted from a matlab script by:
//  Author: Jose Vicente Manjon Herrera & Antoni Buades
//  Date: 09-03-2006
//
//  Implementation of the Non local filter proposed for A. Buades, B. Coll and J.M. Morel in
//  "A non-local algorithm for image denoising"
//
//  Added soft threshold optimal correction - David Milodowski, 05/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//  Martin Hurst, February, 2012
//  Modified by David Milodowski, May 2012- generates grid of recording filtered noise
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::NonLocalMeansFilter(int WindowRadius, int SimilarityRadius, int DegreeFiltering, float Sigma)
{

  //Declare Arrays to hold Filtered Data and Noise
  Array2D<float> FilteredRasterData(NRows,NCols);
  //Array2D<float> FilteredNoise(NRows,NCols);

  //Declare Array to hold a padded copy of the raster with padding values taken
  //as reflected values from the edge of RasterData
  Array2D<float> PaddedRasterData(NRows+2*SimilarityRadius, NCols+2*SimilarityRadius,0.0);
  PadRasterSymmetric(PaddedRasterData, SimilarityRadius);

  //initiate the local gaussian kernel and populate
  int KernelDimension = 2*SimilarityRadius+1;
  Array2D<float> Kernel(KernelDimension,KernelDimension,0.0);
  MakeGaussianKernel(Kernel, Sigma, SimilarityRadius);

  //initiate temporary arrays
  Array2D<float> W1(KernelDimension,KernelDimension);
  Array2D<float> W2(KernelDimension,KernelDimension);

  //initiate temp variables
  float w, wmax, average, sweight, d;

  //loop through DEM
  int i1, j1, rowmin, rowmax, colmin, colmax;

  for (int i=0; i<NRows; ++i)
  {
    i1 = i+SimilarityRadius;
    for (int j=0; j<NCols; ++j)
      {
        j1 = j+SimilarityRadius;
        //Get DEM sample  with size SimilarityRadius, centred on cell of interest
        for (int a=0; a<(KernelDimension); ++a)
        {
          for (int b=0; b<(KernelDimension); ++b)
          {
            W1[a][b] = PaddedRasterData[i1-SimilarityRadius+a][j1-SimilarityRadius+b];
          }
        }

        wmax=0;
        average=0;
        sweight=0;

        //get bounding conditions
        rowmin = max(i1-WindowRadius,SimilarityRadius);
        rowmax = min(i1+WindowRadius,NRows+SimilarityRadius-1);
        colmin = max(j1-WindowRadius,SimilarityRadius);
        colmax = min(j1+WindowRadius,NCols+SimilarityRadius-1);

        //loop to calculate weigths for each cell
        for (int row=rowmin; row<rowmax+1; ++row)
        {
          for (int col=colmin; col<colmax+1; ++col)
          {
            d=0;

            //If centre cell do nothing
            if (row!=i1 || col!=j1)
            //Otherwise do the calculations
            {
              //Extract DEM centred around each point in kernel
              for (int a=0; a<(KernelDimension); ++a)
              {
                for (int b=0; b<(KernelDimension); ++b)
                {
                  W2[a][b] = PaddedRasterData[row+a-SimilarityRadius][col+b-SimilarityRadius];
                  d += Kernel[a][b]*(W1[a][b]-W2[a][b])*(W1[a][b]-W2[a][b]);
                }
              }

              w = exp(-d/(DegreeFiltering*DegreeFiltering));
              if (w>wmax) wmax=w;
              sweight += w;
              average += w*PaddedRasterData[row][col];
            }
         }
      }
      average += wmax*PaddedRasterData[i1][j1];
      sweight += wmax;

      if (sweight > 0) FilteredRasterData[i][j] = average/sweight;
      else FilteredRasterData[i][j] = RasterData[i][j];

      // Also extract a record of the noise
      //FilteredNoise[i][j]=RasterData[i][j]-FilteredRasterData[i][j];
    }
  }

  LSDRaster NLFilteredDEM(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,FilteredRasterData,GeoReferencingStrings);
  //LSDRaster NLFilteredNoise(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,FilteredNoise);
  return NLFilteredDEM; //, NLFilteredNoise;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//	Creates a buffer around an array (of size SimilarityRadius) and gives the new border
//	mirror symmetric values of the original array reflected across the boundary.
//	SimilarityRadius should be the size of the window if filtering
//
//	New array has size nrows + 2*SimilarityRadius by ncols + 2*SimilarityRadius
//
//	Martin Hurst, Feb 2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::PadRasterSymmetric(Array2D<float>& PaddedRasterData, int& SimilarityRadius)
{


	int PaddedNRows = NRows + 2*SimilarityRadius;
	int PaddedNCols = NCols + 2*SimilarityRadius;

	int minus_i;
	int minus_j;

	for (int i=0; i<PaddedNRows; ++i)
	{
		for (int j=0; j<PaddedNCols; ++j)
		{
			//reverse of i and j
			minus_i = PaddedNRows-1-i;
			minus_j = PaddedNCols-1-j;

			//north boundary
			if (i<SimilarityRadius) {
				if (j<SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][SimilarityRadius-j];
				}
				else if (j>(NCols-1+SimilarityRadius)) {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];

				}
				else {
					PaddedRasterData[i][j] = RasterData[SimilarityRadius-i][j-SimilarityRadius];
				}
			}
			//south boundary
			else if (i>NRows-1+SimilarityRadius) {
				if (j<SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][SimilarityRadius-j];
				}
				else if (j>NCols+SimilarityRadius) {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];
				}
				else {
					PaddedRasterData[i][j] = RasterData[i-SimilarityRadius-2*(SimilarityRadius-minus_i)][j-SimilarityRadius];
				}
			}
			//west boundary
			else if (j<SimilarityRadius) {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][SimilarityRadius-j];
			}
			//east boundary
			else if (j>NCols-1+SimilarityRadius) {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][j-SimilarityRadius-2*(SimilarityRadius-minus_j)];
			}
			//copy rest of RasterData
			else {
				PaddedRasterData[i][j] = RasterData[i-SimilarityRadius][j-SimilarityRadius];
			}
		}
	}

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Generate gaussian weighted kernel
//  kernel array must be predeclared of size SimilarityRadius and consist of zeros:
//  Array2D<float> Kernel(SimilarityRadius,SimilarityRadius,0.0);
//
//  Kernel generated using:
//  G(x,y) = (1/2*pi*sigma^2) exp ((-x^2+y^2)/(2*sigma^2))
//
//  Martin Hurst, Feb 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::MakeGaussianKernel(Array2D<float>& Kernel, float sigma, int SimilarityRadius)
{

  float pi = 3.1415926536;
  float left_side = 1/(2*pi*sigma*sigma);
  float twosigma2 = 2.0*sigma*sigma;
  float right_side;
  float wgt = 0;
  float value;

  //calculate kernel values
  for (int i=0;i<2*SimilarityRadius+1;++i)
  {
    for (int j=0;j<2*SimilarityRadius+1;++j)
    {
      right_side = -(((j-SimilarityRadius)*(j-SimilarityRadius) 
                        + (i-SimilarityRadius)*(i-SimilarityRadius))/twosigma2);
      right_side = exp(right_side);
      value = left_side*right_side;
      Kernel[i][j] = value;
      wgt += value;
    }
  }

  //scale to sum to 1
  for (int i=0;i<2*SimilarityRadius+1;++i)
  {
    for (int j=0;j<2*SimilarityRadius+1;++j)
    {
      Kernel[i][j] = Kernel[i][j]/wgt;
    }
  }
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Gaussian Filtering
//  Filters the raster using a square 2D Gaussian filter.  The filter is applied
//  using a moving kernel of gaussian weights with a radius of 3*sigma
//
//  Equivalent lenghtscale of smoothing (from Lashermes et al., 2007)
//  original data -> 4*sigma
//  1st derivative -> 2*pi*sigma
//  2nd derivative -> sqrt(2)*pi*sigma
//  David Milodowski, Feb 2015
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LSDRaster LSDRaster::GaussianFilter(float sigma)
{
  int kr = int(ceil(3*sigma/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                                     // width of kernel
  Array2D<float> filtered = RasterData.copy();
  Array2D<float> gaussian_kernel_weights(kw,kw,0.0);
  
  // gemerate kernel
  float x,y;
  for(int i=0;i<kw;++i)
  {
    for(int j=0;j<kw;++j)
    {
      x = (j-kr)*DataResolution;
      y = (i-kr)*DataResolution;
      gaussian_kernel_weights[i][j]= (1/(2*M_PI*sigma*sigma)) * exp(-(x*x+y*y)/(2*sigma*sigma));  
    }
  }
  //  Now loop through dem filtering using the gaussian kernel
  for(int i=0;i<NRows;++i)
  {
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
      {
        filtered[i][j] = NoDataValue;
      }
      else
      {
        float summed_weights = 0;
        float summed_values = 0; 
        
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            if(RasterData[i-kr+i_kernel][j-kr+j_kernel]!=NoDataValue)
            {
              summed_weights += gaussian_kernel_weights[i_kernel][j_kernel];
              summed_values += RasterData[i-kr+i_kernel][j-kr+j_kernel]*gaussian_kernel_weights[i_kernel][j_kernel];
            } 
          }
        }
        // Get filtered value, ensuring that weights are normalised
        filtered[i][j] = summed_values/summed_weights;
      }
    }
  }
  LSDRaster FilteredRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,filtered);
  FilteredRaster.write_raster("test_gauss","flt");
  return FilteredRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to turn a point shapefile into an LSDIndexRaster.
//
// Can be used to turn a shapefile of channel heads into a sources raster. Does not do 
// any bounds checking or shapefile type checking.
//
// SWDG 13/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDRaster::PointShapefileToRaster(string FileName){

  PointData Points = LoadShapefile(FileName);
  
  Array2D<int> Output(NRows, NCols, NoDataValue);
  
  int i;
  int j;
  float YMax = YMinimum + (DataResolution * (NRows));
              
  for (int q = 0; q < int(Points.X.size()); ++q){
  
    j = (Points.X[q] - XMinimum)/DataResolution;
    i = (YMax - Points.Y[q])/DataResolution;
    
    Output[i][j] = 1;
  
  }
  
  LSDIndexRaster OutputRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                                     NoDataValue,Output,GeoReferencingStrings);
  return OutputRaster;
  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to turn a polyline shapefile into an LSDIndexRaster.
//
// Can be used to turn a shapefile of a river network into a raster. Does not do 
// any bounds checking or shapefile type checking.
//
// Works by calculating points along each line spaced by less than the data resolution. 
// This has the effect of flagging every raster cell along a polyline. 
//
// SWDG 21/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDRaster::PolylineShapefileToRaster(string FileName){

  vector<PointData> Polylines = LoadPolyline(FileName);   //load the coordinates of the shapefile
  Array2D<int> Output(NRows, NCols, NoDataValue);   //output array
  
  float i;
  float j;
  float k;
  float Xm;
  float Ym;
  float length;
  float fraction;
  int i_new;
  int j_new;
  
  float YMax = YMinimum + (DataResolution * (NRows));
  
  for (int w = 0; w < int(Polylines.size()); ++w){
    for (int q = 0; q < int(Polylines[w].X.size())-1; ++q){
    
      length = sqrt( ((Polylines[w].X[q+1] - Polylines[w].X[q]) * (Polylines[w].X[q+1] - Polylines[w].X[q])) + ((Polylines[w].Y[q+1] - Polylines[w].Y[q]) * (Polylines[w].Y[q+1] - Polylines[w].Y[q])) );
      fraction = length/DataResolution;
      
      for (int n = 0; n <= fraction; ++n){
      
        k = n/fraction;  //calculate the next step along the line to get the coordinates of
                
        Xm = Polylines[w].X[q] + (k*(Polylines[w].X[q+1] - Polylines[w].X[q]));      //get the next point along the line
        Ym = Polylines[w].Y[q] + (k*(Polylines[w].Y[q+1] - Polylines[w].Y[q]));
      
        j = (Xm - XMinimum)/DataResolution;     //convert the x,y coordinates into raster coords
        i = (YMax - Ym)/DataResolution;
        
        i_new = trunc(i);           //convert the floating point raster coordinates into integers
        j_new = ceil(j)-1;
        
        Output[i_new][j_new] = 1; 
      
      }        
    }
  }
  
  LSDIndexRaster OutputRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,
     NoDataValue,Output,GeoReferencingStrings);
  return OutputRaster;
  
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to resample an LSDRaster to a lower resolution. 
// OutputResolution is the resolution in spatial units to be resampled to.
// Returns an LSDRaster resampled to the OutputResolution.
// SWDG 17/3/14
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=   
LSDRaster LSDRaster::Resample(float OutputResolution){

  if (OutputResolution < DataResolution){
    cout << "Your resample resolution of " << OutputResolution << " is lower that the current data resolution " << DataResolution << endl;
    exit(EXIT_FAILURE);
  } 

  int NewNRows = (NRows*DataResolution/OutputResolution); 
  int NewNCols = (NCols*DataResolution/OutputResolution);

  Array2D<float> Resampled(NewNRows, NewNCols, NoDataValue);
  
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

  LSDRaster OutputRaster(NewNRows,NewNCols,XMinimum,YMinimum,OutputResolution,
                      NoDataValue,Resampled,GeoReferencingStrings);
  return OutputRaster;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// BASIC TOOLS
//---------------------------------------------------------------------------------------
// Basic functions e.g. spatial averaging, which may be useful for loads of stuff
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Creates a mask for neighbourhood functions.   Uses a neighbourhood switch to select 
// circular (1) vs square window (0).  Default is a square window.
// DTM 19/06/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<int> LSDRaster::create_mask(float window_radius, int neighbourhood_switch)
{
  if((neighbourhood_switch != 1) && (neighbourhood_switch != 0))
  {
    neighbourhood_switch = 0;
    cout << "\t\t incorrect specification of neighbourhood type, so set to square (default).\n"
         << " Note that correct neighbourhood_switch values are: 0 = square, 1 = circular" << endl;
  }
  int kernel_radius = int(ceil(window_radius/DataResolution));
  int kernel_width = 2*kernel_radius + 1;
  Array2D<int> mask(kernel_width,kernel_width,0);
  float x,y,radial_dist;
  for(int i=0;i<kernel_width;++i)
  {
    for(int j=0;j<kernel_width;++j)
    {
      x=(i-kernel_radius)*DataResolution;
      y=(j-kernel_radius)*DataResolution;
      // Build circular mask
      // distance from centre to this point.
      if(neighbourhood_switch == 0) mask[i][j] = 1;
      else if(neighbourhood_switch == 1)
      {
        radial_dist = sqrt(y*y + x*x);
        if (floor(radial_dist) <= window_radius)
        {
          mask[i][j] = 1;
  	}
      }
      else mask[i][j] = 1; 
    }
  }
  return mask;
}
//---------------------------------------------------------------------------------------


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// spatial_average
// Calculates a spatial average using a specified moving window.  Uses a neighbourhood 
// switch to select circular (1) vs square window (0)
// DTM 19/06/2014 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::neighbourhood_statistics_spatial_average(float window_radius, int neighbourhood_switch)
{
  Array2D<float> SpatialAverageArray(NRows,NCols,NoDataValue);
//   Array2D<float> StandardDeviationArray(NRows,NCols,NoDataValue);
  
  // catch if the supplied window radius is less than the data resolution and
  // set it to equal the data resolution - SWDG
  if (window_radius < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }
  
  // Prepare kernel
  int kr = int(ceil(window_radius/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                                     // width of kernel
  Array2D<float> data_kernel(kw,kw,NoDataValue);
  Array2D<int> mask = create_mask(window_radius, neighbourhood_switch);
  
  // Move window over DEM and extract neighbourhood pixels
  cout << "\n\tRunning neighbourhood statistics..." << endl;
  cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
  float mean, value;
  vector<float> data;
  for(int i=0;i<NRows;++i)
  {
    cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
      {
        SpatialAverageArray[i][j] = NoDataValue;
      }
      else
      {
        // Sample DEM
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            value = RasterData[i-kr+i_kernel][j-kr+j_kernel];
            if(value!=NoDataValue && mask[i_kernel][j_kernel]==1) data.push_back(value);
          }
        }
        // Get stats
        mean = get_mean(data);
        SpatialAverageArray[i][j] = mean;      
        // StandardDeviationArray[i][j] = get_standard_deviation(data,mean);
        data.clear();
      }
    }
  }
  
  LSDRaster SpatialAverage(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,SpatialAverageArray,GeoReferencingStrings);
  return SpatialAverage;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// overloaded function to kick out 2 rasters -> local standard deviation & average
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
vector<LSDRaster> LSDRaster::neighbourhood_statistics_spatial_average_and_SD(float window_radius, int neighbourhood_switch)
{
  Array2D<float> SpatialAverageArray(NRows,NCols,NoDataValue);
  Array2D<float> StandardDeviationArray(NRows,NCols,NoDataValue);
  
  // catch if the supplied window radius is less than the data resolution and
  // set it to equal the data resolution - SWDG
  if (window_radius < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }
  // Prepare kernel
  int kr = int(ceil(window_radius/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                    						     // width of kernel
  Array2D<float> data_kernel(kw,kw,NoDataValue);
  Array2D<int> mask = create_mask(window_radius, neighbourhood_switch);
  
  // Move window over DEM and extract neighbourhood pixels
  cout << "\n\tRunning spatial statistics module..." << endl;
  cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
  float mean, value;
  vector<float> data;
  for(int i=0;i<NRows;++i)
  {
    cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) 
                    || RasterData[i][j]==NoDataValue)
      {
        SpatialAverageArray[i][j] = NoDataValue;
      }
      else
      {
        // Sample DEM
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            value = RasterData[i-kr+i_kernel][j-kr+j_kernel];
            if(value!=NoDataValue && mask[i_kernel][j_kernel]==1) data.push_back(value);
          }
        }
        
        // Get stats
        mean = get_mean(data);
        SpatialAverageArray[i][j] = mean;      
        StandardDeviationArray[i][j] = get_standard_deviation(data,mean);
        data.clear();
      }
    }
  }
  
  LSDRaster SpatialAverage(NRows,NCols,XMinimum,YMinimum,DataResolution,
                         NoDataValue,SpatialAverageArray,GeoReferencingStrings);
  LSDRaster SpatialSD(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,
                          StandardDeviationArray,GeoReferencingStrings);
  vector<LSDRaster> output_rasters;
  output_rasters.push_back(SpatialAverage);
  output_rasters.push_back(SpatialSD);
  return output_rasters;
}
//------------------------------------------------------------------------------


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// local relief
// Calculates relief using a specified moving window.  Uses a neighbourhood 
// switch to select circular (1) vs square window (0)
// SMM 15/11/2014 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::neighbourhood_statistics_local_relief(float window_radius, int neighbourhood_switch)
{
  Array2D<float> SpatialReliefArray(NRows,NCols,NoDataValue);
//   Array2D<float> StandardDeviationArray(NRows,NCols,NoDataValue);
  
  // catch if the supplied window radius is less than the data resolution and
  // set it to equal the data resolution - SWDG
  if (window_radius < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }
  
  // Prepare kernel
  int kr = int(ceil(window_radius/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                                     // width of kernel
  Array2D<float> data_kernel(kw,kw,NoDataValue);
  Array2D<int> mask = create_mask(window_radius, neighbourhood_switch);
  
  // Move window over DEM and extract neighbourhood pixels
  cout << "\n\tRunning neighbourhood statistics..." << endl;
  cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
  
  float min_in_window;
  float max_in_window;
  float value;
  float relief;
  
  bool max_set, min_set;     // switches to state whether max and min have been set
  for(int i=0;i<NRows;++i)
  {
    cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) || RasterData[i][j]==NoDataValue)
      {
        SpatialReliefArray[i][j] = NoDataValue;
      }
      else
      {
        // reset max and min values
        min_in_window = 100000;      // set to a large value to make sure it is reset lower
        max_in_window = -100000;     // set to a small value
        
        // Sample DEM
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            value = RasterData[i-kr+i_kernel][j-kr+j_kernel];
            // make sure it is in the mask and not nodata
            if(value!=NoDataValue && mask[i_kernel][j_kernel]==1)
            {
              
              // set min and max values
              if (value > max_in_window)
              {
                max_in_window = value;
                max_set = true;  
              }
              if (value < min_in_window)
              {
                min_in_window = value;
                min_set = true;
              }
            
            } 
          }
        }
        // Get relief
        if(min_set && max_set)
        {
          relief = max_in_window-min_in_window;
        }
        SpatialReliefArray[i][j] = relief;      

      }
    }
  }
  
  LSDRaster SpatialRelief(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,SpatialReliefArray,GeoReferencingStrings);
  return SpatialRelief;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// neighbourhood_statistics_fraction_condition
// A function that determines the fraction of cells in a circular neighbourhood that
// satisfy a given condition
// options
// 0 ==
// 1 !=
// 2 >
// 3 >=
// 4 <
// 5 <=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDRaster::neighbourhood_statistics_fraction_condition(float window_radius,
            int neighbourhood_switch, int condition_switch, float test_value)
{
  Array2D<float> FractionTrueArray(NRows,NCols,NoDataValue);

  if (window_radius < DataResolution)
  {
    cout << "Supplied window radius: " << window_radius << " is less than the data resolution: " <<
    DataResolution << ".\nWindow radius has been set to data resolution." << endl;
    window_radius = DataResolution;
  }
  
  // Prepare kernel
  int kr = int(ceil(window_radius/DataResolution));  // Set radius of kernel
  int kw=2*kr+1;                    						     // width of kernel
  Array2D<float> data_kernel(kw,kw,NoDataValue);
  Array2D<int> mask = create_mask(window_radius, neighbourhood_switch);
  
  // Move window over DEM and extract neighbourhood pixels
  cout << "\n\tRunning neighbourhood statistics..." << endl;
  cout << "\t\tDEM size = " << NRows << " x " << NCols << endl;
  float value;
  float count = 0;
  vector<float> data;

  for(int i=0;i<NRows;++i)
  {
    cout << "\tRow = " << i+1 << " / " << NRows << "    \r";
    for(int j=0;j<NCols;++j)
    {
      // Avoid edges
      if((i-kr < 0) || (i+kr+1 > NRows) || (j-kr < 0) || (j+kr+1 > NCols) 
                    || RasterData[i][j]==NoDataValue)
      {
        FractionTrueArray[i][j] = NoDataValue;
      }
      else
      {
        // Sample DEM
        for(int i_kernel=0;i_kernel<kw;++i_kernel)
        {
          for(int j_kernel=0;j_kernel<kw;++j_kernel)
          {
            value = RasterData[i-kr+i_kernel][j-kr+j_kernel];
            if(value!=NoDataValue && mask[i_kernel][j_kernel]==1)
            {
              count = count + 1;
              if(condition_switch == 0 && value == test_value) data.push_back(value);
              if(condition_switch == 1 && value != test_value) data.push_back(value);
              if(condition_switch == 2 && value > test_value) data.push_back(value); 
              if(condition_switch == 3 && value >= test_value) data.push_back(value);
              if(condition_switch == 4 && value < test_value) data.push_back(value);
              if(condition_switch == 5 && value <= test_value) data.push_back(value);
            }
          }
        }
        // Get stats
        FractionTrueArray[i][j] = float(data.size())/count;
        count = 0;
        data.clear();
      }
    }
  }
  LSDRaster FractionTrue(NRows,NCols,XMinimum,YMinimum,DataResolution,
                        NoDataValue,FractionTrueArray,GeoReferencingStrings);
  return FractionTrue;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Border with nodata values
// This function replaces the border pixels of a raster with nodatavalues.  This
// is particularly useful when dealing with output from functions that have edge
// effects (e.g. spectral filtering)
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::border_with_nodata(int border_width, int irregular_switch)
{
  Array2D<float> Data = RasterData.copy();
  // kernel dimensions
  int kr = border_width;
  int kw = 2*kr + 1;
  float value;
  for(int i = 0; i<NRows; ++i)
  {
    for(int j = 0; j< NCols; ++j)
    {
      if( i<border_width || i>NRows-border_width-1 || j<border_width || j>NCols-border_width-1)
      {
        Data[i][j]=NoDataValue;
        //cout << i << "/" << NRows << " " << j << "/" << NCols << endl;
      }
      else
      {
        if(irregular_switch == 1)
        {
          // Sample DEM
          for(int i_kernel=0;i_kernel<kw;++i_kernel)
          {
            for(int j_kernel=0;j_kernel<kw;++j_kernel)
            {
              value = RasterData[i-kr+i_kernel][j-kr+j_kernel];
              if(value == NoDataValue)
              {
                i_kernel = kw;    // as soon as NoDataValue found, skip to next cell
                j_kernel = kw;
                Data[i][j]=NoDataValue;
              }
            }
          }
        }        
      }
    }
  }
  
  LSDRaster bordered_DEM(NRows,NCols,XMinimum,YMinimum,DataResolution,
                      NoDataValue,Data,GeoReferencingStrings);
  return bordered_DEM;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Create mask based on threshold value and condition
// This function generates an index raster comprising a mask where values of an LSDRaster
// have met a specified condition (e.g. where all values exceed a threshold slope)
//
// MDH, 27/8/14
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDRaster::Create_Mask(string Condition, float TestValue)
{
	//declare mask array
	Array2D<int> Mask(NRows,NCols,NoDataValue);
	
	cout << "Creating Mask: Condition is " << Condition << endl;
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			if (Condition == "<") {
				if (RasterData[i][j] < TestValue) Mask[i][j] = 1;
			}
			else if (Condition == ">") {
				if (RasterData[i][j] > TestValue) Mask[i][j] = 1;
			}
			else if (Condition == "==") {
				if (RasterData[i][j] == TestValue) Mask[i][j] = 1;
			}
			else
			{
				cout << "Condition is " << Condition << endl;
				cout << "Condition not recognised" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	LSDIndexRaster MaskRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,Mask);
	return MaskRaster;	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Extract values by mask
// This function generates a masked raster from an LSDIndexRaster masks, retaining 
// values where the Mask == 1
//
// MDH, 27/8/14
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LSDRaster LSDRaster::ExtractByMask(LSDIndexRaster Mask)
{
	// declare new array
	Array2D<float> MaskedArray(NRows,NCols,NoDataValue);
	Array2D<int> MaskArray = Mask.get_RasterData();
	
	for (int i=0; i<NRows; ++i)
	{
		for (int j=0; j<NCols; ++j)
		{
			if (MaskArray[i][j] == 1)
			{
				MaskedArray[i][j] = RasterData[i][j];
			}
		}
	}
	LSDRaster MaskedRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,MaskedArray);
	return MaskedRaster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// this function creates an LSDIndexRaster
// that has values of 0 for nodes that are not on edge or are borderd
// by nodata, and 1 for those that are on the edge and bordered by nodata
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDRaster::find_cells_bordered_by_nodata()
{
  // this is the array that holds the mask
  Array2D<int> Mask(NRows,NCols,0);
  
  // first, get the edges:
  for(int row = 0; row<NRows; row++)
  {
    Mask[row][0] = 1;
    Mask[row][NCols-1] = 1;
  }
  for(int col = 0; col<NCols; col++)
  {
    Mask[0][col] = 1;
    Mask[NRows-1][col] = 1;
  }

  // now loop through the rest of the data. 
  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      if(RasterData[row][col] == NoDataValue)
      {
        // you need to mask all the surrounding nodes
        Mask[row][col] = 1;
        
        // these are a bunch of tedious if statments to make sure you
        // don't try and access data out of the array bounds       
        if(row !=0)
        {          
          Mask[row-1][col] = 1;
          
          if(col != 0)
          {
            Mask[row-1][col-1] = 1;
          }
          if(col != NCols-1)
          {
            Mask[row-1][col+1] = 1;
          }
        }
        if(row !=NRows-1)
        {          
          Mask[row+1][col] = 1;
          
          if(col != 0)
          {
            Mask[row+1][col-1] = 1;
          }
          if(col != NCols-1)
          {
            Mask[row+1][col+1] = 1;
          }
        } 
        if(col != 0)
        {
          Mask[row][col-1] = 1;
        }       
        if(col != NCols-1)
        {
          Mask[row][col+1] = 1;
        }
      }
    }
  }

  LSDIndexRaster Mask_Raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                         int(NoDataValue),Mask,GeoReferencingStrings);
  return Mask_Raster;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// this looks for isolated instances of nodata and fills them
//
// Not sure about author, I think MDH (SMM comment) 2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::check_isolated_nodata()
{
  for (int row=0; row<NRows; ++row)
  {
    for(int col=0; col<NCols; ++col)
    {
      if(RasterData[row][col] < 0)
      {
        cout << "LSDRaster::check_isolated_nodata stargine data point: row: "
             << row << " col: " << col << " data: " << RasterData[row][col];
        RasterData[row][col] = NoDataValue;
      }

      if(RasterData[row][col] == NoDataValue)
      {
        cout << "LSDRaster::check_isolated_nodata found nodata: row: "
             << row << " col: " << col << " data: " << RasterData[row][col];
      }

    }
  }
  cout << "Done!" << endl;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This is a rudimentary NoData filling algorithm
// you should run the raster trimmer before invoking this
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDRaster::rudimentary_nodata_fill()
{
  int local_row, local_col;
  float local_average;

  // loop through all interior nodes. Nodes on the edge should not have nodata
  // after the trimming process
  for (int row=1; row<NRows-1; ++row)
  {
    for(int col=1; col<NCols-1; ++col)
    {
      float total = 0;
      int n_cells = 0;
      if(RasterData[row][col] == NoDataValue)
      {
        for(int i = -1; i<=1; i++)
        {
          for(int j = -1; j<=1; j++)
          {
            local_row = row+i;
            local_col = col+j;
            if(RasterData[local_row][local_col] != NoDataValue)
            {
              total+= RasterData[local_row][local_col];
              n_cells++;
            } 
          }
        }       
      }
      if(n_cells<0)
      {
        local_average = total/float(n_cells);
        RasterData[row][col] = local_average;  
      }
    }
  }
  cout << "Done!" << endl;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This is a slightly more complex nodata filler
// you should run the raster trimmer before invoking this
// SMM
// 09/12/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::alternating_direction_nodata_fill(int window_width)
{
  

  // check argument
  if(window_width <1)
  {
    cout << "You need a positive window width, defaulting to 1" << endl;
    window_width = 1;
  }
  
  cout << "Sweeping nodata, window width is: " << window_width << endl;

  // This function loops in alternating directions until there is no more nodata
  int NNoData = 0;
  float this_window_sum;
  float local_average;
  int this_window_ndata;
  int window_row, window_col;
  
  // set up data to be 
  Array2D<float> this_sweep_data = RasterData.copy();;
  Array2D<float> updated_raster;
  
  // set the sweep number to 0
  int nsweep = 0;
  
  do
  {
    // reset the number of nodata points in this sweep to zero
    NNoData = 0;

    cout << "LINE 8268, Sweep number: " << nsweep << endl;
    cout << "Line 8275, switch is: " << nsweep%4 << endl;
    
    // copy over the updated raster
    updated_raster = this_sweep_data.copy();
 
   int begin_ndv = 0;
    for(int row = 0; row<NRows; row++)
      {
	for(int col = 0; col <NCols; col++)
	  {
	    if( updated_raster[row][col] == NoDataValue)
	      {
		begin_ndv++;
	      }
	  }
        }
    cout << "Line 8285, the updated raster has " << begin_ndv << " no data nodes " << endl;
   
    // now run a sweep
    switch(nsweep%4)
    {     
      case(0):
      {
        
        // sweep 0
        for (int row=0; row<NRows; ++row)
        {
          for(int col=0; col<NCols; ++col)
          {
            // if the node contains nodata, search the surrounding nodes
            if(updated_raster[row][col] == NoDataValue)
            {
              NNoData++;
              this_window_sum = 0;
              this_window_ndata = 0;

              //cout << "Row: " << row << " col: " << col << endl;
              for(int r = -window_width; r<=window_width; r++)
              {
                for(int c = -window_width; c<=window_width; c++)
                {
                  window_row = r+row;
                  window_col = c+col;
                  
                  
                  if(window_row > 0 && window_row < NRows-1 
                     && window_col > 0 && window_col < NCols-1)
                  {
		    //cout << "wr: " << window_row << " wc: " << window_col << " data: " << updated_raster[window_row][window_col] << endl;
                    if(updated_raster[window_row][window_col] != NoDataValue)
                    {
                      this_window_sum += updated_raster[window_row][window_col];
                      this_window_ndata += 1;
                    }
                  }
                }
              }
              
              // now get the average
              if(this_window_ndata>0)
              {
                //cout << "Found a nodata replacement" << endl;
                local_average = this_window_sum/float(this_window_ndata );
                this_sweep_data[row][col] = local_average;  
              }
            }
          }
        }
        break;
      }
      case(1):
      {
        // sweep 1
        for (int col=0; col<NCols; ++col)
        {
          for(int row=0; row<NRows; ++row)
          {
            // if the node contains nodata, search the surrounding nodes
            if(updated_raster[row][col] == NoDataValue)
            {
              NNoData++;
              this_window_sum = 0;
              this_window_ndata = 0;
              for(int r = -window_width; r<=window_width; r++)
              {
                for(int c = -window_width; c<=window_width; c++)
                {
                  window_row = r+row;
                  window_col = c+col;
                  
                  if(window_row > 0 && window_row < NRows-1 && window_col > 0 && window_col < NCols-1)
                  {
                    if(updated_raster[window_row][window_col] != NoDataValue)
                    {
                      this_window_sum += updated_raster[window_row][window_col];
                      this_window_ndata += 1;
                    }
                  }
                }
              }
              
              // now get the average
              if(this_window_ndata>0)
              {
                local_average = this_window_sum/float(this_window_ndata );
                this_sweep_data[row][col] = local_average;  
              }
            }
          }
        }
	break;
      }
      case(2):
      {
        // sweep 2
        for (int row=0; row<NRows; ++row)
        {
          for(int col=NCols-1; col>0; --col)
          {
            // if the node contains nodata, search the surrounding nodes
            if(updated_raster[row][col] == NoDataValue)
            {
              NNoData++;
              this_window_sum = 0;
              this_window_ndata = 0;
              for(int r = -window_width; r<=window_width; r++)
              {
                for(int c = -window_width; c<=window_width; c++)
                {
                  window_row = r+row;
                  window_col = c+col;
                  
                  if(window_row > 0 && window_row < NRows-1 && window_col > 0 && window_col < NCols-1)
                  {
                    if(updated_raster[window_row][window_col] != NoDataValue)
                    {
                      this_window_sum += updated_raster[window_row][window_col];
                      this_window_ndata += 1;
                    }
                  }
                }
              }
              
              // now get the average
              if(this_window_ndata>0)
              {
                local_average = this_window_sum/float(this_window_ndata );
                this_sweep_data[row][col] = local_average;  
              }
            }
          }
        }
        break;        
      }
      case(3):
      {
        // sweep 3
        for (int col=0; col<NCols; ++col)
        {
          for(int row=NRows-1; row>0; --row)
          {
            // if the node contains nodata, search the surrounding nodes
            if(updated_raster[row][col] == NoDataValue)
            {
              NNoData++;
              this_window_sum = 0;
              this_window_ndata = 0;
              for(int r = -window_width; r<=window_width; r++)
              {
                for(int c = -window_width; c<=window_width; c++)
                {
                  window_row = r+row;
                  window_col = c+col;
                  
                  if(window_row > 0 && window_row < NRows-1 && window_col > 0 && window_col < NCols-1)
                  {
                    if(updated_raster[window_row][window_col] != NoDataValue)
                    {
                      this_window_sum += updated_raster[window_row][window_col];
                      this_window_ndata += 1;
                    }
                  }
                }
              }
              
              // now get the average
              if(this_window_ndata>0)
              {
                local_average = this_window_sum/float(this_window_ndata );
                this_sweep_data[row][col] = local_average;  
              }
            }
          }
        }        
      }
      break;    
    }

    int test_ndv = 0;
    for(int row = 0; row<NRows; row++)
      {
	for(int col = 0; col <NCols; col++)
	  {
	    if( this_sweep_data[row][col] == NoDataValue)
	      {
		test_ndv++;
	      }
	  }
        }

	cout << "Line 8452, testing ndv = " << test_ndv<< endl; 
      
    
    cout << "Line 8445, number of nodata nodes: " << NNoData << endl;
    // increment the sweep number
    nsweep++;

  } while(NNoData > 0);
  
  
  LSDRaster Hole_filled_Raster(NRows,NCols,XMinimum,YMinimum,DataResolution,
                         int(NoDataValue),this_sweep_data,GeoReferencingStrings);
  return Hole_filled_Raster;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Same as above but trims the data
// SMM
// 09/12/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDRaster LSDRaster::alternating_direction_nodata_fill_with_trimmer(int window_width)
{
  cout << "Starting nodata filling by trimming the raster" << endl;
  
  // first remove the seas
  remove_seas();

  // now trim the raster << endl;
  LSDRaster Trimmed_raster = RasterTrimmerSpiral();
  cout << "I've trimmed the raster" << endl;
  LSDRaster nodata_filled = Trimmed_raster.alternating_direction_nodata_fill(window_width);
  cout << "Trimmed raster dimensions are rows: " << nodata_filled.get_NRows()
       << " and cols: " << nodata_filled.get_NCols() << endl;
  return nodata_filled;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Isolate channelised portions of the landscape using the method proposed by Lashermes et
// al. (2007) Lashermes, B., E. Foufoula-Georgiou, and W. E. Dietrich (2007), Channel
// network extraction from high resolution topography using wavelets, Geophys. Res. Lett.,
// 34, L23S04, doi:10.1029/2007GL031140.
// This function (i) filters the DEM using a Gaussian filter; (ii) calculates the
// curvature and/or aspect; (iii) uses quantile quantile analysis to define a curvature
// threshold for channel initiation.
// DTM 06/02/2015
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDRaster::IsolateChannelsLashermesCurvature(float sigma, string q_q_filename)
{
  cout << "\t Isolation of channelised pixels using curvature" << endl;   
  // filter
  cout << "\t\t Gaussian filter" << endl;
  LSDRaster FilteredTopo = GaussianFilter(sigma);
  // calculate curvature
  vector<LSDRaster> output_rasters;
  float window_radius = 1;
  vector<int> raster_selection(8,0.0);
  raster_selection[3]=1;
  cout << "\t\t Calculate curvature" << endl;
  output_rasters = calculate_polyfit_surface_metrics(window_radius, raster_selection);
  LSDRaster curvature = output_rasters[3];
  // use q-q plot to isolate the channels
  cout << "\t\t Finding threshold using q-q plot" << endl;
  LSDIndexRaster channels = curvature.IsolateChannelsQuantileQuantile(q_q_filename);
  return channels;
}
LSDIndexRaster LSDRaster::IsolateChannelsLashermesAspect(float sigma, string q_q_filename)
{
  // filter
  LSDRaster FilteredTopo = GaussianFilter(sigma);
  // calculate curvature
  vector<LSDRaster> output_rasters;
  float window_radius = 1;
  vector<int> raster_selection(8,0.0);
  raster_selection[2]=1;
  output_rasters = calculate_polyfit_surface_metrics(window_radius, raster_selection);
  LSDRaster aspect = output_rasters[2];
  // calculate variability of slope direction d(pheta) = sqrt(d(pheta)/dy^2 + d(pheta)/dx^2)
  Array2D<float> d_pheta1(NRows,NCols,NoDataValue);
  float d_pheta_dx,d_pheta_dy;
  for(int i = 1; i<NRows-1; ++i)
  {
    for(int j = 1; j < NCols-1; ++j)
    {                                            
      if (aspect.get_data_element(i,j)!=NoDataValue && aspect.get_data_element(i-1,j)!=NoDataValue
       && aspect.get_data_element(i+1,j)!=NoDataValue && aspect.get_data_element(i,j+1)!=NoDataValue
       && aspect.get_data_element(i,j-1)!= NoDataValue)
      {
         d_pheta_dx = (aspect.get_data_element(i,j+1)-aspect.get_data_element(i,j-1))/(2*DataResolution);
         d_pheta_dy = (aspect.get_data_element(i+1,j)-aspect.get_data_element(i-1,j))/(2*DataResolution);
         d_pheta1=sqrt(d_pheta_dx*d_pheta_dx+d_pheta_dy*d_pheta_dy);
      }
    }
  }
  // aspect is not a continuous function, so need to repeat with aspect origin rotate by 180 degrees.
  Array2D<float> aspect_temp = aspect.get_RasterData();
  for(int i = 0; i<NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {                                            
      if (aspect_temp[i][j]!=NoDataValue)
      {
         if(aspect_temp[i][j]<180) aspect_temp[i][j]+=180;
         else aspect_temp[i][j]-=180;
      }
    }
  }
  for(int i = 1; i<NRows-1; ++i)
  {
    for(int j = 1; j < NCols-1; ++j)
    {                                            
      if (aspect_temp[i][j]!=NoDataValue && aspect_temp[i-1][j]!=NoDataValue
       && aspect_temp[i+1][j]!=NoDataValue && aspect_temp[i][j+1]!=NoDataValue
       && aspect_temp[i][j-1]!= NoDataValue)
      {
         d_pheta_dx = (aspect_temp[i][j+1]-aspect_temp[i][j-1])/(2*DataResolution);
         d_pheta_dy = (aspect_temp[i+1][j]-aspect_temp[i-1][j])/(2*DataResolution);
         d_pheta1=sqrt(d_pheta_dx*d_pheta_dx+d_pheta_dy*d_pheta_dy);
      }
    }
  }
  
  // use q-q plot to isolate the channels
  LSDIndexRaster channels = aspect.IsolateChannelsQuantileQuantile(q_q_filename);
  return channels;
}
LSDIndexRaster LSDRaster::IsolateChannelsLashermesFull(float sigma, string q_q_filename_prefix)
{
  string q_q_filename_aspect = q_q_filename_prefix + "_aspect.txt";
  string q_q_filename_curvature = q_q_filename_prefix + "_curvature.txt";
  LSDIndexRaster Channels1 = IsolateChannelsLashermesAspect(sigma, q_q_filename_aspect);
  LSDIndexRaster Channels2 = IsolateChannelsLashermesCurvature(sigma, q_q_filename_curvature);
  Array2D<int> binary_array(NRows,NCols,int(NoDataValue));
  for(int i = 1; i<NRows-1; ++i)
  {
    for(int j = 1; j < NCols-1; ++j)
    {
      if(Channels1.get_data_element(i,j)==1 && Channels2.get_data_element(i,j)==1) binary_array[i][j] = 1;
      else if(Channels1.get_data_element(i,j)==0 || Channels2.get_data_element(i,j)==0) binary_array[i][j] = 0;
    }
  }
  
  LSDIndexRaster channels(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,binary_array);
  return channels;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function does part (iii) of the above
LSDIndexRaster LSDRaster::IsolateChannelsQuantileQuantile(string q_q_filename)
{
  
  vector<float> values;
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      if(RasterData[i][j] != NoDataValue)
      {
        values.push_back(RasterData[i][j]);
      }
    }
  }
  
  vector<float> quantile_values,normal_variates,mn_values;
  int N_points = 1000;
  quantile_quantile_analysis(values, quantile_values, normal_variates, mn_values, N_points);
  ofstream ofs;
  ofs.open(q_q_filename.c_str());
  
  if(ofs.fail())
  {
    cout << "\nFATAL ERROR: unable to write output_file" << endl;
    exit(EXIT_FAILURE);
  }
  ofs << "normal_variate value\n";
  for(int i = 0; i<quantile_values.size();++i)
  {
    ofs << normal_variates[i] << " " << quantile_values[i] << " " << mn_values[i] << "\n";
  }
  ofs.close();
  
  // Find q-q threshold
  cout << "\t finding deviation from Gaussian distribution to define q-q threshold" << endl;
  vector<int> indices;
  int flag = 0;
  float threshold_condition=0.99;
  float curvature_threshold = 0.1;
  for(int i = 0; i<quantile_values.size(); ++i)
  {
    if(normal_variates[i] >= 0)
    {
      if(mn_values[i]<threshold_condition*quantile_values[i])
      {
        if (flag==0)
        {
          flag = 1;
          curvature_threshold = quantile_values[i];
        }
      }
      else flag = 0;
    }
  }
  
  cout << "\t Creating channel raster based on curvature threshold (threshold = " << curvature_threshold << ")" << endl;
  Array2D<int> binary_raster(NRows,NCols,NoDataValue);
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      if(RasterData[i][j] != NoDataValue)
      {
        if(RasterData[i][j] >= curvature_threshold) binary_raster[i][j]=1;
        else binary_raster[i][j]=0;
      }
    }
  }
  cout << "DONE" << endl;
  LSDIndexRaster ChannelMask(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,binary_raster);
  return ChannelMask;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Simple method to flatten an LSDRaster and place the non NDV values in a file.
// Each value is placed on its own line, so that it can be read more quickly in python etc.
// SWDG 9/2/15
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDRaster::FlattenToFile(string FileName){

  //open a file to write
  ofstream WriteData;                                
  WriteData.open(FileName.c_str());

  //loop over each cell and if there is a value, write it to the file
  for(int i = 0; i < NRows; ++i){
    for(int j = 0; j < NCols; ++j){
      if (RasterData[i][j] != NoDataValue){
        WriteData << RasterData[i][j] << endl;
      }
    }
  }

  WriteData.close();

} 


#endif
