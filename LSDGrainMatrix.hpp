#ifndef LSDGRAINMATRIX_HPP
#define LSDGRAINMATRIX_HPP

#include <string>
#include <vector>
#include <map>
#include "TNT/tnt.h"
//#include "LSDIndexRaster.hpp"
//#include "LSDShapeTools.hpp"


class LSDGrainMatrix
{
public:
  
  friend class LSDRaster;
  
  LSDGrainMatrix()
  {
    create();  //throws an error -- we don't want an empty object
  }
  
  /// Create a GrainMatrix object by reading in a file
  LSDGrainMatrix(std::string fname, std::string fname_extension)
  {
    create(fname, fname_extension);
  }
  
  /// Create a GrainMatrix object from references to arrays (in LSDCatchmentModel, though needn't be this object)
  LSDGrainMatrix( TNT::Array2D<int>& indexes, 
                  TNT::Array2D<double>& graindatas,
                  TNT::Array3D<double>& stratadatas)
  {
    create(indexes, graindatas, stradatas);
  }
  
  /// Writes the GrainMatrix object to an output text file (Warning: large file!)
  void write_grainMatrix_to_ascii_file(std::string filename, std::string fname_extension);
  
protected:
  TNT::Array2D<int> rasterIndex;
  TNT::Array2D<double> grainData;
  TNT::Array3D<double> strataData;
  
private:
  void create();
  void create(std::string fname, std::string fname_extension);
  void create(TNT::Array2D<int>& indexes, 
              TNT::Array2D<double>& graindatas,
              TNT::Array3D<double>& stratadatas);
  
}
    


#endif // LSDGRAINMATRIX_HPP
