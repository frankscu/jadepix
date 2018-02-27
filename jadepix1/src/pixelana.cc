#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"
#include "TChain.h"

#include "CMOS.hh"
#include "conf_param.hh"
#include "define_hist.hh"   // All of histograms are defined in this file.

using namespace std;

int main(int argc, char *argv[])
{
   std::vector<std::string> arg(argv,argv+argc);
  
   std::string& datafile = arg[1];      // input File name
   std::string& outfile = arg[2];       // output ROOT File

   int max_ana_frame = std::atoi(argv[3]);  // Max Number of Frame to be decoded.

   // Use CMOS Class
   CMOS ana;

   // Set Header & Footer word
   ana.set_header_footer_word();
   
   // Set Column Parameter (e.g. column 5-8 )
   ana.set_ana_param(decode_ch_start, decode_ch_end);

   // Open Data File
   ana.open_file(datafile);
   
   // Set Maximum Event
   ana.set_max_ana_event(max_ana_frame);

   // Histrogram Delearation
   TObjArray hlist;        
   make_histogram(&hlist);
   
   // Init Variables
   ana.init_variables();

   for(int nFrame=0; nFrame<max_ana_frame; nFrame++)  
   { 
      if( nFrame%100 == 0 )std::cout << "Frame number = " << nFrame << std::endl;
      ana.set_frame_number(nFrame);
      
      // Frame Header 
      int header_flag = ana.find_frame_header();
      if(header_flag==2)break;          // End of File
      
      // Data :  (row_header + data + row_footer)*16
      int data_flag = ana.decode_pixel_data();
      if(data_flag==2)break;             // End of File 
      
      // Data Checker
      int data_check_flag = ana.compare_two_frame();
      
      // Frame Footer
      int footer_flag =  ana.find_frame_footer();
      if(footer_flag==2)break;          // End of File
      
      // Calc Hit Information
      if( data_check_flag == 0 && footer_flag == 0 )  // This frame is OK.
      {
         ana.init_loop_variables();
         ana.calc_pixel_hit();
         ana.calc_cluster_hit();
         ana.fill_to_histo();
      }
   }
   
   // Output ROOT file
   TFile *fout = new TFile(outfile.c_str(),"RECREATE");
   hlist.Write();
   fout->Close();
   
   hlist.Delete();

   return 0;
}


