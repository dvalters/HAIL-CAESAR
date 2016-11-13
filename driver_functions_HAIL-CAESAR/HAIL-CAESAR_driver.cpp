/// HAIL-CAESAR_main.cpp
///
/// The Catchment Hydrogeomorphology Model v1.0 (CHM1)
/// 
/// This is the main function for running the CHM1 catchment hydrology and
/// sediment erosion model.
/// 
/// Update: November 2016:
/// Major driver file reorganisation:
///
/// You can now create 'modular' style model simulations by building your
/// own driver files from the CHM1 components. (Inspired by the
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
/// implementation of the LISFLOOD flow routing algorithm.)
///
/// - LSDTopoTools pacakge (Edinburgh Land Surface Dynamics group):
/// Raster data input/output routines, Parameter file parsing, Object-oriented
/// driver files, general C++ coding style.
///
///
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>

#include "../LSDCatchmentModel.hpp"

bool file_check(string name)
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

int main(int argc, char *argv[])
{
  // For the timing routine
  #ifdef OMP_COMPILE_FOR_PARALLEL
  double start_time = omp_get_wtime();
  #endif

  // Just prints out how many threads/cores you have, when run in parallel mode
  #ifdef OMP_COMPILE_FOR_PARALLEL
  LSDCatchmentModel::quickOpenMPtest();
  #endif

  if (argc < 3)
  {
    std::cout << "\n###################################################" << std::endl;
    std::cout << "No parameter file supplied" << std::endl;
    std::cout << "You must supply a path and parameter file!" << std::endl;
    std::cout << "see http://www.geos.ed.ac.uk/~smudd/LSDTT_docs/html/" << std::endl;
    std::cout << "LSDCatchmentModel docs for assistance." << std::endl;
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

  // Create a catchment model object
  LSDCatchmentModel simulation(pname, pfname);
  simulation.initialise_model_domain_extents();
  simulation.initialise_arrays();
  simulation.load_data();
  simulation.set_time_counters();

  // Create a runoff object
  runoffGrid runoff(simulation.get_imax(), simulation.get_jmax());
  simulation.initialise_rainfall_runoff(runoff);
  simulation.initialise_drainage_area();

  // These parameters control the interval in which
  // certain model components are called,
  // such as landsliding, vegetation, and creep
  // I don't put them in the parameter file as they
  // are not usually needed to be altered.
  int local_landsliding_interval = 10;
  int creep_time_interval_days = 14400;
  int scan_area_interval_iter = 5;
  int inchannel_landsliding_interval_hours = 1440;
  int vegetation_growth_interval_hours = 1440;
  double creep_coeff = 0.028;

  // Check parameters
  simulation.print_parameters();

  // Entering the main loop here
  std::cout << "Entering main model loop..." << std::endl;
  do
  {
    // Simulation iteration functions
    simulation.set_loop_cycle();
    simulation.set_inputoutput_diff();
    simulation.set_global_timefactor();
    simulation.increment_counters();

    // Hydrological and flow routing processes
    simulation.catchment_waterinputs(runoff);
    simulation.flow_route();
    simulation.depth_update();

    // Check drainage area (For traditional TOPMODEL)
    simulation.check_wetted_area(scan_area_interval_iter);

    // Erosion processes if not a hydro-only simulation
    if (!simulation.is_hydro_only())
    {
      simulation.call_erosion();
      //simulation.call_lateral(); // not tested in this version!
    }

    // Water outputs from edges/catchment outlet
    simulation.water_flux_out();

    // Hillslope & vegetation processes
    simulation.local_landsliding(local_landsliding_interval);
    simulation.slope_creep(creep_time_interval_days, creep_coeff);
    simulation.inchannel_landsliding(inchannel_landsliding_interval_hours);
    simulation.grow_vegetation(vegetation_growth_interval_hours);

    // Outputs
    simulation.write_output_timeseries(runoff);
    simulation.print_cycle();
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
