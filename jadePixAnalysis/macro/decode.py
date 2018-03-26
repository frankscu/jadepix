#!/bin/env python

import pyJadePixAna as ja

decode = ja.Wrapdecode2root()

anaylysis = ja.Wrappixelana()


#basefilename = "CHIPA2_180322145454"
#basefilename = "CHIPA1_180322145948"
#basefilename = "CHIPA3_180322150048"
#basefilename = "CHIPA4_180322150146"
#basefilename = "CHIPA5_180322150238"
#basefilename = "CHIPA6_180322150330"
#basefilename = "CHIPA7_180322150423"
#basefilename = "CHIPA8_180322150515"
#basefilename = "CHIPA9_180322150605"
#basefilename = "CHIPA10_180322150703"
#basefilename = "Source_CHIPA1_180322153107"
#basefilename = "Source_CHIPA2_180322153848"
#basefilename = "Sr_CHIPA1_180323091433"
#basefilename = "Sr_CHIPA2_180323091741"
#basefilename = "Sr_CHIPA3_180323091821"
#basefilename = "Sr_CHIPA4_180323091845"
#basefilename = "Sr_CHIPA5_180323091908"
#basefilename = "Sr_CHIPA6_180323091932"
#basefilename = "Sr_CHIPA7_180323091954"
#basefilename = "Sr_CHIPA8_180323092017"
#basefilename = "Sr_CHIPA9_180323092039"
#basefilename = "Sr_CHIPA10_180323092105"
basefilename = "CHIPA1_180322145948"



raw_data = "F:\\data\\"+basefilename+".df"
root_file = "F:\\data\\"+basefilename+".root"

data_format = 2
max_decode_frame_number = 10000


decode.decode2root(raw_data, root_file, data_format, max_decode_frame_number)

number_of_input_file = 1
outfilename = "F:\\data\\Ana_"+basefilename+".root"

anaylysis.pixelana(max_decode_frame_number, number_of_input_file, outfilename, root_file)