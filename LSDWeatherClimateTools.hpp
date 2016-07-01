#ifndef LSDWEATHERCLIMATETOOLS_H
#define LSDWEATHERCLIMATETOOLS_H


#include "TNT/tnt.h"
#include "LSDStatsTools.hpp" // This contains some spline interpolation functions already


class rainGrid
{
friend class LSDCatchmentModel; // might not need this, see how it pans out...

public:

  /// Default constructor -- throws an error.
  rainGrid()
  {
    create();
  }

  /// Create a raingrid from the rainfall data vector,
  /// and the raster or model domain dimensions, for the
  /// current timestep only. Specify an interpolation method
  rainGrid(std::vector< std::vector<float> >& rain_data,
           TNT::Array2D<int>& hydroindex,
           int imax, int jmax, int current_rainfall_timestep, int rf_num)
  {
    std::cout << "Creating a LSD rainGrid object from a rainfall timeseries and hydroindex..." \
                 << std::endl;
    create(rain_data, hydroindex, imax, jmax, current_rainfall_timestep, rf_num);
  }

  /// Create rainGrid from interpolating between sparse points and x, y coord
  /// TODO

  /// Takes a 2D array of regular gridded rainfall and interpolates
  /// it according to a Bivariate Spline. Similar to sciPy's
  /// scipy.interpolate.RectBivariateSpline
  void interpolateRainfall_RectBivariateSpline(rainGrid& raingrid);

  /// Will take a 2D array of regular gridded rainfall and interpolates
  /// it based on a TRI-variate spline. I.e. interpolates based on an
  /// extra third variable which would be terrain in most cases (see
  /// Tait et al 2006, for example)
  void interpolateRainfall_RectTrivariateSpline(rainGrid& raingrid,
                                                TNT::Array2D<double>& elevation);
  
  /// Takes the rainfall data for a current timestep and
  /// reshapes it into a 2D array. 
  void ReShapeRainfallData_2DArray();
  
  /// Upscales the 2d rainfall array to a resampled, higher resolution
  /// array, with the same grid spacing, and dimensions as the model or 
  /// raste rdomain. 
  void upscaleRainfallData();
  
  /// Writes the 2D upscaled and/or interpolated rainfall grid to 
  /// a raster output file for checking.
  void write_rainGrid_to_raster_file(double xmin, double ymin, 
                                               double cellsize,
                                               std::string RAINGRID_FNAME,
                                               std::string RAINGRID_EXTENSION);
 
protected:

  /// For a single instance of a 2D rainfall grid, matching the dimensions of the
  /// model domain.
  TNT::Array2D<double> rainfallgrid2D;
  /// Experimental - stores grids of rainfall data for every rainfall timestep:
  /// Warning - this could be a massive object!
  TNT::Array3D<double> rainfallgrid3D;

private:
  /// Returns an error, empty object pointless
  void create();
  /// Initialises by converting rainfall data file into grid at same
  /// grid spacing as model (or raster) domain grid spacing.
  void create(std::vector<std::vector<float> >& rain_data,
              TNT::Array2D<int>& hydroindex,
              int imax, int jmax, int current_rainfall_timestep, int rf_num);

};


/// Object for storing and calculating the saturation and hence 
/// surface runoff when using spatially
/// variable rainfall input (j_mean, but 2D, i.e. j_mean_array[i][j], same as 
/// model 
class rainfallrunoffGrid
{
public:

  /// Default constructor -- throws an error.
  rainfallrunoffGrid()
  {
    //create();
  }

  /// Create a rainfallrunoffGrid from passing params and refs to params
  rainfallrunoffGrid(int local_time_factor)
  {
    std::cout << "Creating a LSD rainfallrunoffGrid object from a parameters..." \
                 << std::endl;
    //create();
  }  
};

#endif // LSDWEATHERCLIMATETOOLS_H
