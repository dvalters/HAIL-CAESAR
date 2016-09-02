/// LSDGrainMatrix.hpp
/// HEADER FILE

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
#include <fstream>
#include <sstream>
#include "TNT/tnt.h"

#ifndef LSDGrainMatrix_H
#define LSDGrainMatrix_H

/// @brief This class is used primarily for the LSDCatchmentModel, to package up data
/// about the stratigraphy and grain fraction data into neat objects. 
class LSDGrainMatrix
{
public:
  friend class LSDRaster;
  /*
  LSDGrainMatrix()
  {
    create();  //throws an error -- we don't want an empty object
  }
  */
  /*
  /// Create a GrainMatrix object by reading in a file
  LSDGrainMatrix(std::string fname, std::string fname_extension)
  {
    create(fname, fname_extension);
  }
  */
  /// Create a GrainMatrix object from references to arrays (in LSDCatchmentModel, though needn't be this object)
  LSDGrainMatrix( int imax, int jmax, int NoDataVal, int G_MAX,
                  TNT::Array2D<int>& indexes, 
                  TNT::Array2D<double>& graindatas,
                  TNT::Array3D<double>& stratadatas)
    : rasterIndex(indexes), grainData(graindatas), strataData(stratadatas)
  {
    create(imax, jmax, NoDataVal, G_MAX);
  }
  
  /// Writes the GrainMatrix object to an output text file (Warning: large file!)
  void write_grainMatrix_to_ascii_file(std::string filename, std::string fname_extension);
  
protected:
  TNT::Array2D<int>& rasterIndex;
  TNT::Array2D<double>& grainData;
  TNT::Array3D<double>& strataData;
  
  int NCols;
  int NRows;
  int NoData;
  int GrainFracMax;
  
private:
  //void create();
  //void create(std::string fname, std::string fname_extension);
  void create(int imax, int jmax, int NoDataVal, int G_MAX);
  
};
    


#endif // LSDGRAINMATRIX_HPP
