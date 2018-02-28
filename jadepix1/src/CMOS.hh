#ifndef CMOS_H
#define CMOS_H

class CMOS
{
public:
   CMOS();
   
   // Set Parameters
   void set_header_footer_word();
   void set_ana_param(int ch_start, int ch_end);
   void set_max_ana_event(int nevent);

   // File Open 
   int open_file(std::string datafile);

   // Init 
   void init_variables();

   // Decode Member Function 
   int find_frame_header();
   int find_frame_footer();
   int decode_pixel_data();
   int compare_two_frame();

   void set_frame_number(int frame_number){CurrentFrame = frame_number;}

   // Hit Calculation Member Function
   void init_loop_variables();
   int calc_pixel_hit();
   int calc_cluster_hit();
   void fill_to_histo();

private:
   // Data Header & Footer
   unsigned long Frame_Header;
   unsigned long Frame_Footer;

   // File Stream
   ifstream fin;
    
   // Error Flag
   int header_error_flag;
   int data_error_flag;
   int footer_error_flag;

   // Container for one frame
   signed short int  adc[48][16];
   
   unsigned short int header_id[48];
   unsigned short int header_event_type[48];
   unsigned short int header_row_counter[48];
   unsigned short int header_counter_4bit[48];
   unsigned short int header_counter_16bit[48];

   unsigned short int header_id_pre[48];
   unsigned short int header_event_type_pre[48];
   unsigned short int header_row_counter_pre[48];
   unsigned short int header_counter_4bit_pre[48];
   unsigned short int header_counter_16bit_pre[48]; 

   unsigned short int footer_id[48];
   unsigned short int footer_event_type[48];
   unsigned short int footer_row_counter[48];
   unsigned short int footer_counter_4bit[48];
   unsigned short int footer_memaddr_counter[48];

   unsigned short int footer_id_pre[48];
   unsigned short int footer_event_type_pre[48];
   unsigned short int footer_row_counter_pre[48];
   unsigned short int footer_counter_4bit_pre[48];
   unsigned short int footer_memaddr_counter_pre[48]; 

   // Initial Setting parameters
   int Max_ana_Event;
   int CH_START;
   int CH_END;
   
   // Frame Number
   int CurrentFrame;

   // Store previous ADC value
   int pre_adc[48][16];

   // Store Mean & RMS for each 48row*16column
   double Calib_MEAN_ADC[48][16];
   double Calib_RMS_ADC[48][16];


   // Hit Info. (for 16 column & all )
   double Column_Hit_Pos[16][25];
   double Column_Hit_ADC[16][25];
   int Column_Nhit[16];
   int CMOS_Nhit;

   // Cluster Info.
   int CMOS_Ncluster;
   double CMOS_Hit_Pos_col[100];
   double CMOS_Hit_Pos_row[100];
   double CMOS_Hit_ADC[100];
   
   int cluster_id[100];

   double SUM_ADC[100];
   double SUM_Pos_col[100];
   double SUM_Pos_row[100];
   
   double CMOS_Cluster_Hit_Pos_col[100];
   double CMOS_Cluster_Hit_Pos_row[100];
   double CMOS_Cluster_Hit_ADC[100];
   int  CMOS_Cluster_Nhit[100];
   int  CMOS_Cluster_pre_Nhit[100];
};

#endif
