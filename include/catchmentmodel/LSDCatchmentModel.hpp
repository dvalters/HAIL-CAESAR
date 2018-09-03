#include "TNT/tnt.h"   // Template Numerical Toolkit library
#include "topotools/LSDRaster.hpp"


#ifndef LSDCatchmentModel_geodecomp_H
#define LSDCatchmentModel_geodecomp_H

/*class LSDCatchmentModel
{
public:*/


/// set by ncols and nrows
unsigned int jmax, imax;
double xll, yll;
double no_data_value;

double DX;
double DY;

void runSimulation();

//};


#endif
