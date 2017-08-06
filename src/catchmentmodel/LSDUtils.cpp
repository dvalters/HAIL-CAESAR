// LSDUtils.cpp
#include <omp.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sys/stat.h>
#include <catchmentmodel/LSDUtils.hpp>

namespace LSDUtils
{
    void parse_line(std::ifstream &infile, std::string &parameter, std::string &value)
    {
      char c;
      char buff[256];
      int pos = 0;
      int word = 0;

      while ( infile.get(c) )
      {
        if (pos >= 256)
        {
          std::cout << "Buffer overrun, word too long in parameter line: " << std::endl;
          std::string line;
          getline(infile, line);
          std::cout << "\t" << buff << " ! \n" << line << std::endl;
          exit(1);
        }
        // preceeding whitespace
        if (c == '#')
        {
          if (word == 0)
          {
            parameter = "NULL";
            value = "NULL";
          }
          if (word == 1)
            value = "NULL";
          word = 2;
        }

        if ((c == ' ' || c == '\t') && pos == 0)
          continue;
        else if ( (c == ':' && word == 0) || ( (c == ' ' || c == '\n' || c == '\t') && word == 1))
        {
          while (buff[pos-1] == ' ' || buff[pos-1] == '\t')
            --pos;      // Trailing whitespace
          buff[pos] = '\0';   // Append Null char
          if (word == 0)
            parameter = buff;   // Assign buffer contents
          else if (word == 1)
            value = buff;
          ++word;
          pos = 0;        // Rewind buffer
        }
        else if ( c == '\n' && word == 0 )
        {
          parameter = "NULL";
          buff[pos] = '\0';
          value = buff;
          ++word;
        }
        else if (word < 2)
        {
          buff[pos] = c;
          ++pos;
        }

        if (c == '\n')
          break;
      }
      if (word == 0)
      {
        parameter = "NULL";
        value = "NULL";
      }
    }

    std::string RemoveControlCharactersFromEndOfString(std::string toRemove)
    {
      int len =  toRemove.length();
      if(len != 0)
      {
        if (iscntrl(toRemove[len-1]))
        {
          //cout << "Bloody hell, here is another control character! Why Microsoft? Why?" << endl;
          toRemove.erase(len-1);
        }
      }
      return toRemove;
    }

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
}
