%module(directors="1") jadipix
%{
#include "JadipixDecoder.h"
%}

%import "std_string.i"
%import "std_map.i"
%import "std_pair.i"

namespace std{
  %template(IPair) pair<int,int>;
  %template(IPairMap) map<pair<int,int>,int>;
}

%include "JadipixDecoder.h"
