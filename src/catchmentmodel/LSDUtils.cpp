// LSDUtils.cpp
#include <omp.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sys/stat.h>
#include <catchmentmodel/LSDUtils.hpp>

void quickOpenMPtest()
{
  // test
  #ifdef OMP_COMPILE_FOR_PARALLEL
  int thread_id;
  int num_threads = omp_get_max_threads();
  int num_procs = omp_get_num_procs();

  std::cout << "Hello! My name is OpenMP. I like to do LSD (Land Surface Dynamics) in parallel!" << std::endl;
  std::cout << "Your system has: " << num_procs << " PROCESSORS available and " << num_threads << " THREADS to use!" << std::endl;
  std::cout << "(Note: On some systems, threads are reported the same as processors.)" << std::endl;

  #pragma omp parallel private ( thread_id ) num_threads (num_threads)
  {
    thread_id = omp_get_thread_num();

    // So, you need a lock here (critical) because the << operator is
    // not thread safe (i.e. you get race conditions)
    #pragma omp critical
    {
      std::cout << "Yo, whaddup! From thread number..." << thread_id << std::endl;
    }
  }
  std::cout << "Goodbye, parallel region!" << std::endl;
  #endif
}
