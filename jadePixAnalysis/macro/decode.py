#!/bin/env python

import pyJadePixAna as ja

decode = ja.Wrapdecode2root()

anaylysis = ja.Wrappixelana()


#basefilename = "test_180208114003"
#basefilename = "test_180211112859"
basefilename = "output_180211142337"

raw_data = "E:\\data\\"+basefilename+".df"
root_file = "E:\\data\\"+basefilename+".root"

data_format = 2
max_decode_frame_number = 5000


decode.decode2root(raw_data, root_file, data_format, max_decode_frame_number)

number_of_input_file = 1
outfilename = "E:\\data\\Ana_"+basefilename+".root"

anaylysis.pixelana(max_decode_frame_number, number_of_input_file, outfilename, root_file)