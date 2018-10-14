#include "TNT/tnt.h"   // Template Numerical Toolkit library
#include "topotools/LSDRaster.hpp"


#ifndef LSDCatchmentModel_geodecomp_H
#define LSDCatchmentModel_geodecomp_H


/// set by ncols and nrows
extern unsigned int jmax, imax;
extern double xll, yll;
extern double no_data_value;
extern double DX, DY;

void runSimulation();


#endif
