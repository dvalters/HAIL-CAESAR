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

  inline void parse_line(std::ifstream &infile, std::string &parameter, std::string &value)
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

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
  // This function removes control characters from the end of a string
  // These get introduced if you use the DOS format in your parameter file
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
  inline std::string RemoveControlCharactersFromEndOfString(std::string toRemove)
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
