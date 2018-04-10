#ifndef EF_STRING_H
#define EF_STRING_H

#include <cctype>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace EF {
    inline std::string trim(const std::string& str, const std::string& delims = " \t\n\r\v") {
        size_t b = str.find_first_not_of(delims);
        size_t e = str.find_last_not_of(delims);
        if(b == std::string::npos || e == std::string::npos) {
            return "";
        }
        return std::string(str, b, e - b + 1);
    }
    
    inline void split_string(const std::string& s, std::vector<std::string>& v, const std::string& c){
      std::string::size_type pos1, pos2;
      pos2 = s.find(c);
      pos1 = 0;
      while(std::string::npos != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));
        
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
      }
      if(pos1 != s.length())
        v.push_back(s.substr(pos1));
    }
}

#endif 
