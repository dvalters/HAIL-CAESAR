#include "libgeodecomp.h"
#include "catchmentmodel/LSDCatchmentModel.hpp"

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  LibGeoDecomp::Typemaps::initializeMaps();
  runSimulation();
  MPI_Finalize();
  return 0;  
}



