/// HAIL-CAESAR_main.cpp
///
/// HAIL-CAESAR
/// (The High Performance Architecture Independent LISFLOOD CAESAR model)
///
/// This is the main function for running the HAIL-CAESAR model.
///
/// Update: November 2016:
/// Major driver file reorganisation:
///
/// You can now create 'modular' style model simulations by building your
/// own driver files from the various model components. (Inspired by the
/// neat Landlab modelling framework). Most functionallity is still controlled
/// via the parameter file, but now you can have finer grained control over
/// the catchment simulation building blocks.

///
/// Acknowledgements:
///
/// Numerous people have contributed directly and indirectly to the algorithms,
/// code structure, libraries and functionallity behind the Catchment Model.
/// Much code has been adapted from other open source numerical models, including:
///
/// - The CAESAR-Lisflood model (Coulthard et. al, 2013)
/// (Cellular automaton code, erosional model, area scanning algorithm,
/// implementation of the LISFLOOD flow routing algorithm.) Tom Coulthard et al.
/// Paul Bates et al.
///
/// - LSDTopoTools pacakge (Edinburgh Land Surface Dynamics group):
/// Raster data input/output routines, Parameter file parsing, Object-oriented
/// driver files, general C++ coding style. Simon Mudd, et al.
///
///
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>

#include "catchmentmodel/LSDCatchmentModel.hpp"
#include "catchmentmodel/LSDUtils.hpp"

#ifndef GIT_REVISION
#define GIT_REVISION "N/A"
#endif
#define CHM_VERS 1.0

using namespace LSDUtils;

int main(int argc, char *argv[])
{
  #ifdef DEBUG
  std::cout << "===================================" << std::endl;
  std::cout << "||    Running in DEBUG mode      ||" << std::endl;
  std::cout << "===================================" << std::endl;
  #endif

  std::cout << "##################################" << std::endl;
  std::cout << "#  CATCHMENT HYDROGEOMORPHOLOGY  #" << std::endl;
  std::cout << "#        MODEL version 1.0       #" << std::endl;
  std::cout << "#          (HAIL-CAESAR)         #" << std::endl;
  std::cout << "##################################" << std::endl;
  std::cout << " Version: "<< CHM_VERS << std::endl;
  std::cout << " at git commit number: " GIT_REVISION << std::endl;
  std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;


  // For the timing routine
  #ifdef OMP_COMPILE_FOR_PARALLEL
  double start_time = omp_get_wtime();
  #endif

  // Just prints out how many threads/cores you have, when run in parallel mode
  #ifdef OMP_COMPILE_FOR_PARALLEL
  quickOpenMPtest();
  #endif

  if (argc < 3)
  {
    std::cout << "\n###################################################" << std::endl;
    std::cout << "No parameter file supplied" << std::endl;
    std::cout << "You must supply a path and parameter file!" << std::endl;
    std::cout << "see https://dvalters.github.io/HAIL-CAESAR/" << std::endl;
    std::cout << "for assistance." << std::endl;
    std::cout << "###################################################" << std::endl;

    exit(0);  // Game over, try again.
  }

  if (argc > 3)
  {
    std::cout << "Too many input arguments supplied (should be 3...)" << std::endl;
    exit(0);
  }


  std::string pname(argv[1]);
  std::string pfname(argv[2]);
  // The path name and the parameter file name, respectively.
  // Remember: argc[0] is the program name that you just typed in to the terminal.
  std::cout << "The pathname is: " << pname
            << " and the parameter file is: " << pfname << std::endl;

  // Create a catchment model object initialising the model
  // domain extents from the DEM header.
  LSDCatchmentModel simulation(pname, pfname);
  simulation.initialise_model_domain_extents();
  simulation.initialise_arrays();
  simulation.load_data();
  simulation.set_time_counters();

  // Create a runoff object, a grid to manage the input
  // of rainfall to the catchment domain.
  runoffGrid runoff(simulation.get_imax(), simulation.get_jmax());
  simulation.initialise_rainfall_runoff(runoff);
  simulation.initialise_drainage_area();

  // These parameters control the interval in which
  // certain model components are called,
  // such as landsliding, vegetation, and creep
  // I don't put them in the parameter file as they
  // are not usually needed to be altered.
  int in_channel_landslide_iter_interval = 10;
  int creep_time_interval_days = 14400;
  int scan_area_interval_iter = 5;
  int global_landsliding_interval_hours = 1440;
  int vegetation_growth_interval_hours = 1440;
  double creep_coeff = 0.028;

  // Check parameters by prining them to STDOUT.
  simulation.print_parameters();

  // Entering the main loop here
  std::cout << "Entering main model loop..." << std::endl;
  do
  {
    // Simulation iteration functions
    simulation.set_loop_cycle();
    // This is the difference between water entering the catchment_waterinputs
    // (from rainfall) and water exiting the catchment from it's boundaries.
    // If this value meets a user-defined threshold, the time step is
    // automatically increased.
    simulation.set_inputoutput_diff();
    simulation.set_maximum_timestep();
    simulation.increment_counters();

    // Hydrological and flow routing processes

    // In reach mode, add the reach inputs and hydrology
    simulation.reach_water_and_sediment_input();
    // Add water to the catchment from rainfall input file
    simulation.catchment_waterinputs(runoff);
    // Distribute the water with the LISFLOOD Cellular Automaton algorithm
    simulation.flow_route();
    // Calculate the new water depths in the catchment
    simulation.depth_update();

    // Check wetted area
    // This masks a portion of the catchment that actually
    // contains water, which is then used to speed up
    // iterations later by skipping cells that have miniscule water content.
    simulation.check_wetted_area(scan_area_interval_iter);

    // Erosion processes if not a hydrology-only simulation
    if (!simulation.is_hydro_only())
    {
      // This is quite inefficient code in the current version...
      simulation.call_erosion();
      //simulation.call_lateral(); // not tested in this version!
    }

    // Water outputs from edges/catchment outlet
    simulation.water_flux_out();

    // Hillslope & vegetation processes
    simulation.call_channel_landsliding(in_channel_landslide_iter_interval);
    simulation.slope_creep(creep_time_interval_days, creep_coeff);
    simulation.call_global_landsliding(global_landsliding_interval_hours);
    simulation.grow_vegetation(vegetation_growth_interval_hours);

    // Outputs
    simulation.write_output_timeseries(runoff);
    // Prints current timestep/cycle to STDOUT
    simulation.print_cycle();
    // Writes the DEM files with water depth, erosion etc.
    simulation.save_raster_output();

    // if we have reached the end of the simulation, stop the loop
  } while (simulation.get_cycle() / 60 < simulation.get_maxcycle());

  std::cout << "THE SIMULATION IS FINISHED!" << std::endl;

  // Timing routine for parallel
  #ifdef OMP_COMPILE_FOR_PARALLEL
  double end_time = omp_get_wtime();
  double simulation_run_time = (end_time - start_time)/60;
  std::cout << "The simulation ran in " << simulation_run_time << \
               " minutes. This includes everything executed within the main() function." \
            << std::endl;
  #endif

  return 0;
}
