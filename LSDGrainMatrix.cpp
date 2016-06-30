/// LSDGrainMatrix.cpp

/*
 * SUMMARY
 *
 * LSDGrainMatrix is a class that defines an object to store data
 * about the surface and subsurface layers of raster or catchment.
 * It stores the grainsize fractions (of n fractions) for a given number
 * of surface and n subsurface layers. Be warned it is quite a large object
 * if you wish to define multiple fractions and multiple
 * subsurface layers!
 * 
 * @author Declan Valters
 * @date 2016
 * University of Manchester
 * @contact declan.valters@manchester.ac.uk
 * @version 0.01
 *
 * Released under the GNU v2 Public License
 *
 */

#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <iterator> // For the printing vector method

#include <sys/stat.h> // For fatal errors

// Only for the debug macro
#include <cstdio>


#include "LSDGrainMatrix.hpp"

#ifndef LSDGrainMatrix_CPP
#define LSDGrainMatrix_CPP

/*void LSDGrainMatrix::create()
{
  std::cout << "You are trying to create an LSDGrainMatrix object with no supplied files or parameters." << std::endl << "Exiting..." << std::endl;
  exit(EXIT_FAILURE);
}*/

void LSDGrainMatrix::create(int imax, int jmax, int NoDataVal, int G_MAX)
{
  NRows = imax; // +2? -check in LSDCatchmentModel
  NCols = jmax;
  NoData = NoDataVal;
  GrainFracMax = G_MAX;
  std::cout << "Initialised a Grain Matrix..." << std::endl;
}

void LSDGrainMatrix::write_grainMatrix_to_ascii_file(std::string filename, 
                                                     std::string fname_extension)
{
  std::string string_filename;
  std::string dot = ".";
  string_filename = filename + dot + fname_extension;
  std::cout << "The graindata filename is: " << string_filename << std::endl;
  
  if (fname_extension == "asc")
  {
    // Open a grain data file
    std::ofstream data_out(string_filename.c_str());
    
    if( data_out.fail() )
    {
      std::cout << "\n ERREUR FATALE!!!: Unable to write to " << string_filename \
                   << std::endl;
      exit(EXIT_FAILURE);
    }
    
    for(int i=1; i<=NRows; ++i)
    {
      for(int j=1; j<=NCols; ++j)
      {
        //std::cout << rasterIndex[i][j] << ", " << NoData << std::endl;
        if (rasterIndex[i][j] != NoData)
        {  
          // Write the first part of the output file line (x,y location and index)
          data_out << i << " " << j << " " << rasterIndex[i][j] << " ";
          
          // Now write the Surface Grain bit
          for (int inc=0; inc<= GrainFracMax; inc++)
          {
            //std::cout << rasterIndex[i][j] << std::endl;
            data_out << grainData[rasterIndex[i][j]][inc] << " ";
          }
          
          // Now write the subsurface grain fractions
          for(int z=0; z<=9; z++) // Loop through subsurface layers...
          {
            for(int inc=0; inc<=(GrainFracMax-2); inc++)
            {
              data_out << strataData[rasterIndex[i][j]][z][inc] << " ";
            }
          }
          data_out << std::endl;
        }
        
      } 
    }
    data_out.close();
  }
  
  else
  {
    std::cout << "You did not enter and approprate extension!" << std::endl
    << "You entered: " << fname_extension << ", options are: asc" << std::endl;
    exit(EXIT_FAILURE);
  }
}

#endif

























