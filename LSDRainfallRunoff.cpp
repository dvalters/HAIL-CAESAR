/// LSDRainfallRunoff.cpp

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

#ifndef LSDRainfallRunoff_CPP
#define LSDRainfallRunoff_CPP

#include "LSDRainfallRunoff.hpp"
#include "LSDRaster.hpp"

void rainGrid::create()
{
  std::cout << "You have tried to create an empty RainfallRunoff object. " <<
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
  rainfallgrid2D = TNT::Array2D<double>(imax+2, jmax+2, 0.0);

  // DEBUG
  
  //std::cout << "HYDROINDEX DEBUG: " << std::endl;
  //std::cout << hydroindex.dim1() << ", " << hydroindex.dim2() << std::endl;
  // DAV addeded pragma for testing 08/2016
  #pragma omp parallel for
  for (int i=1; i<imax; i++) 
  {
    for (int j=1; j<jmax; j++)
    {
      for (int rf=1; rf<rf_num; rf++)
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

void rainGrid::downscaleRainfallData()
{
  // Either does equivalent of NumPy's np.kronecker product or np.zoom.
  // Needs further thought.
}

void rainGrid::interpolateRainfall_RectBivariateSpline(rainGrid &raingrid)
{
  // SPLINES!!
}
// =-=-=-=-=-=-=-=-=-=-=-=-=
// RAINFALL RUNOFF OBJECT
// =-=-=-=-=-=-=-=-=-=-=-=-=


void runoffGrid::create(int imax, int jmax)
{
  std::cout << "Creating an EMPTY RUNOFF GRID OBJECT..." << std::endl;
  // set arrays to relevant size for model domain
  // Zero or set to very small value near zero.
  j_array = TNT::Array2D<double>(imax +2, jmax +2, 0.000000001);
  jo_array = TNT::Array2D<double>(imax +2, jmax +2, 0.000000001);
  j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);
  old_j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);
  new_j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);

  // This is all that happens, use calculate_runoff() to fill in with proper values.

}

// Same as abouve but calculates values for runoff matrices given raingrid and a timestep
//  and other relevant params.
void runoffGrid::create(int current_rainfall_timestep, int imax, int jmax,
                                int rain_factor, double M,
                                const rainGrid& current_rainGrid,
                                const TNT::Array2D<double>& elevations)
{
  std::cout << "Creating a RUNOFF GRID OBJECT FROM RAINGRID..." << std::endl;
  // set arrays to relevant size for model domain
  j_array = TNT::Array2D<double>(imax +2, jmax +2, 0.000000001);
  jo_array = TNT::Array2D<double>(imax +2, jmax +2, 0.000000001);
  j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);
  old_j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);
  new_j_mean_array = TNT::Array2D<double>(imax +2, jmax +2, 0.0);

  calculate_runoff(rain_factor, M, jmax, imax, current_rainGrid, elevations);
}

void runoffGrid::write_runoffGrid_to_raster_file(double xmin,
                                                  double ymin,
                                                  double cellsize,
                                                  std::string RUNOFFGRID_FNAME,
                                                  std::string RUNOFFGRID_EXTENSION)
{
  // For checking purposes mainly. Writes grid to an ascii file so
  // I can see if the upscale or interpolate has worked correctly.
  int nrows = new_j_mean_array.dim1();
  int ncols = new_j_mean_array.dim2();

  int nodata = -9999;
  
  LSDRaster output_runoffgrid(nrows, ncols, xmin, ymin, cellsize, nodata,
                            new_j_mean_array);
  output_runoffgrid.strip_raster_padding();
  output_runoffgrid.write_double_raster(RUNOFFGRID_FNAME, RUNOFFGRID_EXTENSION); 
}


void runoffGrid::calculate_runoff(int rain_factor, double M, int jmax, int imax, 
                                  const rainGrid& current_rainGrid, 
                                  const TNT::Array2D<double>& elevations)
{
  //std::cout << "calculate_runoff" << std::endl;
  // DAV addeded pragma for testing 08/2016
  #pragma omp parallel for            
  for (int m=1; m<imax; m++)
  {
    for (int n=1; n<jmax; n++)
    {
      // Do not bother calculating runoff outside the catchment boundaries.
      // I.e. in no data values
      if (elevations[m][n] > -9999)
      {
        double local_rainfall_rate =0;
        double local_time_step=60;
  
        old_j_mean_array[m][n] = new_j_mean_array[m][n];
        jo_array[m][n] = j_array[m][n];
  
        // Variable M value would go here
        // if (variable_m_flag == true) { }
  
  
        if (current_rainGrid.get_rainfall(m, n) > 0)
        {
          //std::cout << "Rainfall is: " << current_rainGrid.get_rainfall(m, n) << std::endl;
          // Divide by 1000 to get m/hr, then 3600 for m/sec
          local_rainfall_rate = rain_factor * ((current_rainGrid.get_rainfall(m, n)
              / 1000) / 3600);
        }
  
        // If case is zero, we still need to calculate the amount of saturation decay
        // for this time step (TOPMODEL based)
        if (local_rainfall_rate == 0)
        {
          j_array[m][n] = jo_array[m][n] / (1 + ((jo_array[m][n] * local_time_step) / M));
  
          new_j_mean_array[m][n] = M / local_time_step *
              std::log(1 + ((jo_array[m][n] * local_time_step) / M));
        }
  
        // If there is some rain in this cell, we need to calculate how much
        // is runoff vs infiltrates (TOPMODEL based)
        if (local_rainfall_rate > 0)
        {
          //std::cout << "Cell Rainfall Rate is: " << local_rainfall_rate << std::endl;
          
          j_array[m][n] = local_rainfall_rate / (((local_rainfall_rate - jo_array[m][n]) / jo_array[m][n])
                     * std::exp((0 - local_rainfall_rate) * local_time_step / M) + 1);
  
          new_j_mean_array[m][n] = (M / local_time_step)
                              * std::log(((local_rainfall_rate - jo_array[m][n]) + jo_array[m][n]
                              * std::exp((local_rainfall_rate *local_time_step)
                                         / M)) / local_rainfall_rate);
        }
  
        // Don't want to have negative J_means!
        if (new_j_mean_array[m][n] < 0)
        {
          new_j_mean_array[m][n] = 0;
        }
      }
    }
  }
}



#endif




