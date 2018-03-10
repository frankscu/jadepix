#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

#include "Decode.hh"
#include "wrapdecode2root.hh"

using namespace std;

Wrapdecode2root::Wrapdecode2root(){
    std::cout << "Start to decode ...  "<< std::endl;
};

Wrapdecode2root::~Wrapdecode2root(){
    std::cout << "End decoding ...  "<< std::endl;
};

void Wrapdecode2root::decode2root(string datafile, string rootfile, int data_structure, int max_decode_frame)
{
    //std::vector<std::string> arg(argv,argv+argc);

    //std::string& datafile = arg[1];   // input File name
    //std::string& rootfile = arg[2];    // output ROOT File
    //int data_structure = std::atoi(argv[3]);  //-- Data Structure (with TimeStamp = 1, without TimeStamp = 2)
    //int max_decode_frame = std::atoi(argv[4]);  // Max Number of Frame to be decoded.
    // Use Decode Class
    Decode decode;

    // Set Header & Footer word
    decode.set_header_footer_word();

    // Open Data File
    decode.open_file(datafile);

    // Making ROOT file
    decode.set_output_filename(rootfile);

    // Define Tree & Histogram
    TTree *tree = new TTree("tree", "Dump to ROOT file");
    decode.set_tree_pointer(tree);
    decode.Define_TTree();
    decode.init_hist();

    // Init Loop Counters
    decode.init_loop_counter();

    // Cut TimeStamp saved at the beginning.
    if (data_structure == 1)
    {
        decode.skip_timestamp();
    }

    // Init Variables
    decode.init_variables();

    for (int nFrame = 0; nFrame < max_decode_frame; nFrame++)
    {
        if(nFrame%1000==0)std::cout << "Frame number = " << nFrame << std::endl;
        decode.set_loop_counter(nFrame);

        // Frame Header
        int end_of_file = decode.find_frame_header();
        if (end_of_file == 2)
            break;

        // Data :  (row_header + data + row_footer)*16
        end_of_file = decode.decode_pixel_data();
        if (end_of_file == 2)
            break;
        decode.compare_two_frame();

        // Frame Footer
        end_of_file = decode.find_frame_footer();
        if (end_of_file == 2)
            break;

        // Fill to the ROOT Tree
        decode.fill_to_tree();
    }

    decode.write_file(tree);
}
