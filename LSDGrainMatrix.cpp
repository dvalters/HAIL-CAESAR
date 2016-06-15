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


#include "LSDCatchmentModel.hpp"

#ifndef LSDCatchmentModel_CPP
#define LSDCatchmentModel_CPP

void LSDGrainMatrix::create()
{
  //
}

#endif