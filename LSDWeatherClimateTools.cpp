/// LSDWeatherClimateTools.cpp

/*
 * Used for various weather/storm generators, climate data interpolation
 * etc. 
 *
 * Will use some of the interpolate routines in LSDSTatsTools.
 *
 * @author Declan Valters
 * @date  2014, 2015, 2016
 * University of Manchester
 * @contact declan.valters@manchester.ac.uk
 * @version 0.01
 *
 * Released under the GNU v2 Public License
 *
 */

#ifndef LSDWeatherClimateTools_CPP
#define LSDWeatherClimateTools_CPP

#include "LSDWeatherClimateTools.hpp"
#include "LSDRaster.hpp"

void rainGrid::create()
{
  std::cout << "You have tried to create an empty WeatherClimate object. " <<
               std::endl << "Please try again with some parameters" <<
               std::endl;
  exit(EXIT_FAILURE);
}

void rainGrid::create(std::vector< std::vector<float> >& rain_data,
                      TNT::Array2D<int>& hydroindex,
                      int imax, int jmax, int current_rainfall_timestep,
                      int rf_num)
{
  // Creates a 2D object of rainfall data based on the extents of the
  // current model domain, and the rainfall timeseries.
  rainfallgrid2D = TNT::Array2D<double>(imax+2, jmax+2);

  // DEBUG
  
  std::cout << "HYDROINDEX DEBUG: " << std::endl;
  std::cout << hydroindex.dim1() << ", " << hydroindex.dim2() << std::endl;
  for (int i=1; i<imax; i++) 
  {
    for (int j=1; j<jmax; j++)
    {
      for (int rf=1; rf<=rf_num; rf++)
      {
        if (hydroindex[i][j]==rf) 
        {
          rainfallgrid2D[i][j] = rain_data[current_rainfall_timestep][rf];
        }
      }
    }
  }
}

void rainGrid::write_rainGrid_to_raster_file(double xmin,
                                             double ymin,
                                             double cellsize,
                                             std::string RAINGRID_FNAME,
                                             std::string RAINGRID_EXTENSION)
{
  // For checking purposes mainly. Writes grid to an ascii file so
  // I can see if the upscale or interpolate has worked correctly.
  int nrows = rainfallgrid2D.dim1();
  int ncols = rainfallgrid2D.dim2();

  double nodata = -9999.0;
  
  LSDRaster output_raingrid(nrows, ncols, xmin, ymin, cellsize, nodata,
                            rainfallgrid2D);
  output_raingrid.strip_raster_padding();
  output_raingrid.write_double_raster(RAINGRID_FNAME, RAINGRID_EXTENSION);
}

void rainGrid::ReShapeRainfallData_2DArray()
{
  // Reshapes the flattened rainfall timeseries file into a 2D array
  // for one time step slice
}

void rainGrid::upscaleRainfallData()
{
  // Either does equivalent of NumPy's np.kronecker product or np.zoom.
  // Needs further thought.
}

void rainGrid::interpolateRainfall_RectBivariateSpline(rainGrid &raingrid)
{
  // SPLINES!!
}
#endif




