%module(directors="1") jadepix
%{
#include "JadepixDecoder.h"
#include "JadepixFrame.h"
#include "JadepixDigi.h"
%}

%import "std_string.i"
%import "std_map.i"
%import "std_pair.i"
%import "std_vector.i"

%include "JadepixDecoder.h"
%include "JadepixFrame.h"
%include "JadepixDigi.h"
