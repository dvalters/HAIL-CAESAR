// LSDUtils.cpp

// File parsing utils for the HAIL-CAESAR model
//
// Credit goes to JAJ (James Jenkinson) who is the original
// author of these routines.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <fstream>
#include <ostream>
#include <string>

namespace LSDUtils
{
  /// @brief Template for sorting a std::pair.
  /// @details This template will perform a sort on the std::pair types. It
  /// will sort ascending based on the second item in the pair. It is intended
  /// to mimic the C# method: sort(Array,Array). Needs further testing, but
  /// should work in principle.
  /// @author DAV
  template <class T1, class T2, class Pred = std::less<T2> >
  struct sort_pair_second
  {
    bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right)
    {
      Pred p;
      return p(left.second, right.second);
    }
  };

  void parse_line(std::ifstream &infile, std::string &parameter, std::string &value);

  // This function removes control characters from the end of a string
  // These get introduced if you use the DOS format in your parameter file
  std::string RemoveControlCharactersFromEndOfString(std::string toRemove);

  // Duplicated below -pick one to remove!
  inline bool file_check(std::string name)
  {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
  }

  // Wee function to check if file exists
  // Not sure if this works in Windows...must test sometime
  inline bool does_file_exist(const std::string &filename)
  {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) ==0);
  }

  // A simple function to test OpenMP in the LSDTopoTools environment
  void quickOpenMPtest();
}
