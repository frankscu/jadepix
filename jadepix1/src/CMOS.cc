#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TProfile.h"

#include "CMOS.hh"

//----------------------------------    For General Routine     --------------------------------------- 


CMOS::CMOS()
{
   std::cout<<"Analysis start!"<<std::endl;
}

void CMOS::set_header_footer_word()
{
   Frame_Header = 0xAAAAAAAA;
   Frame_Footer = 0xF0F0F0F0;
}

void CMOS::set_max_ana_event(int n)
{
   Max_ana_Event = n;
   std::cout << "Max event = " << Max_ana_Event << std::endl;
}

void CMOS::set_ana_param(int ch_start,  int ch_end)
{
   CH_START = ch_start - 1;   // CH7 = "6" in C++ code.  
   CH_END = ch_end - 1;  

   std::cout << "CH_START = " << CH_START << ",  CH_END = " << CH_END << std::endl;
}

int CMOS::open_file(std::string datafile)
{
   fin.open(datafile.c_str(),std::ios::in|std::ios::binary);
   if(!fin.is_open())
   {
      std::cout << "File open error!" << std::endl;
      return 0;
   }
   
   return 1;
}

void CMOS::init_variables()
{
   for(int row=0;row<48; row++)
   {
      header_id[row] = 0;
      header_event_type[row] = 0;
      header_row_counter[row] = 0;
      header_counter_4bit[row] = 0;
      header_counter_16bit[row] = 0;

      header_id_pre[row] = 0;
      header_event_type_pre[row] = 0;
      header_row_counter_pre[row] = 0;
      header_counter_4bit_pre[row] = 0;
      header_counter_16bit_pre[row] = 0;

      footer_id[row] = 0;
      footer_event_type[row] = 0;
      footer_row_counter[row] = 0;
      footer_counter_4bit[row] = 0;
      footer_memaddr_counter[row] = 0;

      footer_id_pre[row] = 0;
      footer_event_type_pre[row] = 0;
      footer_row_counter_pre[row] = 0;
      footer_counter_4bit_pre[row] = 0;
      footer_memaddr_counter_pre[row] = 0;

      for(int ch=0;ch<16;ch++)     
      {    
         adc[row][ch] = 0;
         pre_adc[row][ch] = 0;
      }
   }
      
      header_error_flag = 0;
      data_error_flag = 0;
      footer_error_flag = 0;
}       


int CMOS::find_frame_header()
{
   int flag=0;  
   header_error_flag = 0;  // Error flag 

   int header_find_flag_frame_header=0;
   int read_count = 0;
   while(header_find_flag_frame_header==0)
   {
      unsigned int tmp_buf_header;
      fin.read((char *)&tmp_buf_header,sizeof(tmp_buf_header));
      read_count++;
      if(fin.eof())
	{
         std::cout << "End of File" << std::endl;
         return 2;
	}
      if(tmp_buf_header==Frame_Header)
	{
	  header_find_flag_frame_header = 1;

          if( read_count > 1 ) 
          {
             flag = 1;
             header_error_flag = 1;
             std::cout << "Header Search :  Read word(=4byte) count = " << read_count << std::endl; 
          }
	}
   }
   
   return  flag;
}

int CMOS::find_frame_footer()
{
   int flag=0;  
   footer_error_flag = 0; // Error flag

   unsigned int tmp_buf_footer;
   fin.read((char *)&tmp_buf_footer,sizeof(tmp_buf_footer));
   if(fin.eof())
   {
      std::cout << "End of File" << std::endl;
      return 2;
   }
   if( tmp_buf_footer != Frame_Footer )
   {
      flag = 1;
      footer_error_flag = 1;
      std::cout << "Event Footer (0xf0f0_f0f0) is not here ! " << std::endl;
   }

   return flag;
}

int CMOS::decode_pixel_data()
{
   int flag=0;  // Error flag

   for(int row=0; row<48; row++)
   {
      for(int i=0;i<10;i++)
      {
         unsigned int tmp_buf_read;
         fin.read((char *)&tmp_buf_read,sizeof(tmp_buf_read));  
         if(fin.eof())
         {
            flag = 1;
            std::cout << "End of Data " << std::endl;
            return 2;
         }

         if( i==0 )  // row header
         {
            header_id[row] =  (tmp_buf_read & 0xf0000000) >> 28;
            header_event_type[row] = (tmp_buf_read & 0x0c000000) >> 26;
            header_row_counter[row] = (tmp_buf_read & 0x03f00000) >> 20;
            header_counter_4bit[row]= (tmp_buf_read & 0x000f0000) >> 16;
            header_counter_16bit[row] = (tmp_buf_read & 0x0000ffff);

            double col_sta = (tmp_buf_read & 0x0000f000) >> 12;
            double col_end = (tmp_buf_read & 0x00000f00) >> 8;
            double ad_id = (tmp_buf_read & 0x0000003f);

            /*
            std::cout << "col_sta = " << col_sta << ", col_end = " << col_end << ",  ad_id = " << ad_id << std::endl;
            std::cout << "header_id = " << header_id[row] << std::endl;
            std::cout << "event_type = " << header_event_type[row] << std::endl;
            std::cout << "row_counter = " << header_row_counter[row] << std::endl;
            std::cout << "event_counter = " << header_counter_4bit[row] << std::endl;
            */
         }
 
         int ch1, ch2;
         if( i == 1 ){ ch1 = 0;  ch2 = 1; }
         if( i == 2 ){ ch1 = 2;  ch2 = 3; }
         if( i == 3 ){ ch1 = 4;  ch2 = 5; }
         if( i == 4 ){ ch1 = 6;  ch2 = 7; }
         if( i == 5 ){ ch1 = 8;  ch2 = 9; }
         if( i == 6 ){ ch1 = 10; ch2 = 11; }
         if( i == 7 ){ ch1 = 12; ch2 = 13; }
         if( i == 8 ){ ch1 = 14; ch2 = 15; }
        
         if( i==9 )  // row footer
         {
            // Valid before a bug fix
            //footer_id[row] =  (tmp_buf_read & 0x00f00000) >> 20;
            //footer_event_type[row] = (tmp_buf_read & 0x000c0000) >> 18;
            //footer_row_counter[row] = (tmp_buf_read & 0x0003f000) >> 12;
            //footer_counter_4bit[row]= (tmp_buf_read & 0x00000f00) >> 8;
            //footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);

            footer_id[row] =  (tmp_buf_read & 0xf0000000) >> 28;
            footer_event_type[row] = (tmp_buf_read & 0x0c000000) >> 26;
            footer_row_counter[row] = (tmp_buf_read & 0x03f00000) >> 20;
            footer_counter_4bit[row]= (tmp_buf_read & 0x000f0000) >> 16;
            footer_memaddr_counter[row] = (tmp_buf_read & 0x0000003f);


            double row_sta = (tmp_buf_read & 0x0fa00000) >> 22;
            double row_end = (tmp_buf_read & 0x003f0000) >> 16;
            double event_number = (tmp_buf_read & 0x0000ffff);

            //std::cout << "row_sta = " << row_sta << ", row_end = " << row_end << ",  event_number  = " << event_number << std::endl;
         }

         if( i>0 && i<9)  // Only Pixel Data
         {            
            adc[row][ch1] = (tmp_buf_read & 0x0000ffff) ;
            adc[row][ch2] = (tmp_buf_read & 0xffff0000) >> 16;
         }
      }
   }  // End of 48*10 readout

   return flag;
}

int CMOS::compare_two_frame()
{
   int flag=0;  
   data_error_flag = 0;  // Error flag

   // Data Check
   for(int row=0;row<48;row++)
   {
      if( header_id[row] != 15 ) // row header
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << CurrentFrame << " : " ;
         std::cout << "Row Header is not find : row = " << row << std::endl;
      }
      if( footer_id[row] != 14 ) // row footer
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << CurrentFrame << " : " ;
         std::cout << "Row Feader is not find : row = " << row << std::endl;
      }
      if( header_event_type[row] != 2 )
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << CurrentFrame << " : " ;      
         std::cout << "Event type is something wrong : row = " << row << std::endl;
      }
      if( header_row_counter[row] != row )
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event num = " << CurrentFrame << " : " ;
         std::cout << "Row counter is something wrong : row = " << row << std::endl;
         std::cout << "Row_counter = " << header_row_counter[row] << std::endl;
         std::cout << "Row_counter_pre = " << header_row_counter_pre[row] << std::endl;
      }
      if( (header_counter_4bit[row] - header_counter_4bit_pre[row]) != 1 && (header_counter_4bit[row] - header_counter_4bit_pre[row]) != -15)
      {
         flag = 1;
         data_error_flag = 1;
         std::cout << "Event counter(4bit) is something wrong : row = " << row << std::endl;
         std::cout << "Event_counter(4bit) = " << header_counter_4bit[row] << std::endl;
         std::cout << "Event_counter_pre(4bit) = " << header_counter_4bit_pre[row] << std::endl;
      }
//      if( (header_counter_16bit[row] - header_counter_16bit_pre[row]) != 1 && (header_counter_16bit[row] - header_counter_16bit_pre[row]) != -65535)
//      {
//         flag = 1;
//         data_error_flag = 1;
//         std::cout << "Event counter(16bit) is something wrong : row = " << row << std::endl;
//         std::cout << "Event_counter(16bit) = " << header_counter_16bit[row] << std::endl;
//         std::cout << "Event_counter_pre(16bit) = " << header_counter_16bit_pre[row] << std::endl;
//      }
   }

   // Copy those information 
   for(int row=0;row<48;row++)
   {
      header_id_pre[row] = header_id[row];
      header_event_type_pre[row] = header_event_type[row];
      header_row_counter_pre[row] = header_row_counter[row];
      header_counter_4bit_pre[row] = header_counter_4bit[row];
      header_counter_16bit_pre[row] = header_counter_16bit[row];
   }

   return 0;
}




void CMOS::init_loop_variables()
{
   for(int ch=CH_START;ch<=CH_END;ch++)
   {
      for(int i=0;i<25;i++)
      {
         Column_Hit_Pos[ch][i] = -1;
         Column_Hit_ADC[ch][i] = 9999;
         Column_Nhit[ch] = 0;
      }
   }

   for( int i=0; i<100; i++)
   {
      CMOS_Hit_Pos_col[i] = -1;
      CMOS_Hit_Pos_row[i] = -1;
      CMOS_Hit_ADC[i] = 9999;

      cluster_id[i] = -999;

      SUM_ADC[i] = 0.0;
      SUM_Pos_col[i] = -99.0;
      SUM_Pos_row[i] = -99.0;
      CMOS_Cluster_pre_Nhit[i] = 0;

      CMOS_Cluster_Hit_Pos_col[i] = -99.0;
      CMOS_Cluster_Hit_Pos_row[i] = -99.0;
      CMOS_Cluster_Hit_ADC[i] = 0;
      CMOS_Cluster_Nhit[i] = 0;
   }
   CMOS_Nhit = 0;
   CMOS_Ncluster = 0;
   
}


int CMOS::calc_pixel_hit()
{
   for(int ch=CH_START;ch<=CH_END;ch++)
   {
      for(int row=0;row<48;row++)     
      {             
         double diff_CDS = (double)adc[row][ch]  - (double)pre_adc[row][ch];
         if(pre_adc[row][ch]==9999)diff_CDS=0.0; // First event.

         pre_adc[row][ch] = adc[row][ch];

         //std::cout << "adc = " << diff_CDS << std::endl;
         if( diff_CDS < -160 )  // Cut threshld !!
         {
            if( Column_Nhit[ch]<25 )  // Less than 25 hits for each column can be registered.
            {
               Column_Hit_Pos[ch][Column_Nhit[ch]] = row;
               Column_Hit_ADC[ch][Column_Nhit[ch]] = diff_CDS;
               Column_Nhit[ch]++;
            }
         }
      }
   }
      
   // Merge all of hits into one 
   int counter=0;
   for(int ch=CH_START;ch<=CH_END;ch++)
   {
      for(int i=0; i<Column_Nhit[ch] ; i++)
      {
         CMOS_Hit_Pos_col[ counter ] = ch;
         CMOS_Hit_Pos_row[ counter ] = Column_Hit_Pos[ch][i];
         CMOS_Hit_ADC[ counter ] = Column_Hit_ADC[ch][i];
         counter++; 
      }
   }
   CMOS_Nhit = counter;

   return 0;
}


int CMOS::calc_cluster_hit()
{
   //-------  Make Cluster --------//
   
   // 1.   Temporal assignment of Cluster ID
   for( int i = 0;  i < CMOS_Nhit; i++ )
   {
      cluster_id[i] = i;
   }

   // 2.  Merge into clusters
   for( int i = 0;  i < CMOS_Nhit-1; i++ )
   {
      for( int j = i+1; j < CMOS_Nhit; j++)
      {
         double diff_col =  CMOS_Hit_Pos_col[i] - CMOS_Hit_Pos_col[j] ; 
         double diff_row = CMOS_Hit_Pos_row[i] - CMOS_Hit_Pos_row[j] ;
         double distance = sqrt( diff_col*diff_col + diff_row*diff_row );
         
         if( distance < 2.0 ) // this is regarded as the same cluster
         {
            for( int index = 0;  index < CMOS_Nhit; index++ ) // Scan all of htis and change cluster ID for this "GROUP" 
            {
               if( cluster_id[index] == cluster_id[i] ) 
               {
                  cluster_id[index] = cluster_id[j];
               }
            }
         }
      } 
   }

   // Align Cluster Info. 
   for( int id = 0;  id < CMOS_Nhit ; id++ ) 
   {
      for( int i = 0;  i < CMOS_Nhit; i++ ) 
      {    
         if( cluster_id[i] == id )
         {
            SUM_ADC[ id ] +=  CMOS_Hit_ADC[i];
            SUM_Pos_col[id] += CMOS_Hit_ADC[i]*CMOS_Hit_Pos_col[i];
            SUM_Pos_row[id] += CMOS_Hit_ADC[i]*CMOS_Hit_Pos_row[i];
            CMOS_Cluster_pre_Nhit[id]++;
         }
      }
      
      if( CMOS_Cluster_pre_Nhit[id] > 0 )
      {
         CMOS_Cluster_Hit_ADC[CMOS_Ncluster] = SUM_ADC[id] ;
         CMOS_Cluster_Hit_Pos_col[CMOS_Ncluster] = SUM_Pos_col[id] / SUM_ADC[id] ;
         CMOS_Cluster_Hit_Pos_row[CMOS_Ncluster] = SUM_Pos_row[id] / SUM_ADC[id] ;
         CMOS_Cluster_Nhit[CMOS_Ncluster] = CMOS_Cluster_pre_Nhit[id] ;

         CMOS_Ncluster++;
      }
   }
   
   return 0;
}


      
         
void CMOS::fill_to_histo()
{

   // Number of Hit for each channel/All
   std::stringstream hist_title_nhit, hist_title_cluster_nhit;
   std::stringstream hist_title_nhit_ch5, hist_title_nhit_ch6, hist_title_nhit_ch7, hist_title_nhit_ch8;

   hist_title_nhit     << "h_nhit" ;
   hist_title_cluster_nhit     << "h_cluster_nhit" ;
   
   hist_title_nhit_ch5 << "h_nhit_ch5" ;
   hist_title_nhit_ch6 << "h_nhit_ch6" ;
   hist_title_nhit_ch7 << "h_nhit_ch7" ;
   hist_title_nhit_ch8 << "h_nhit_ch8" ;

   TH1F* htmp_nhit    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_nhit.str().c_str()));           
   TH1F* htmp_cluster_nhit    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_cluster_nhit.str().c_str()));           

   TH1F* htmp_nhit_ch5    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_nhit_ch5.str().c_str()));   
   TH1F* htmp_nhit_ch6    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_nhit_ch6.str().c_str()));   
   TH1F* htmp_nhit_ch7    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_nhit_ch7.str().c_str()));   
   TH1F* htmp_nhit_ch8    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_nhit_ch8.str().c_str()));   

   htmp_nhit->Fill(CMOS_Nhit);
   htmp_cluster_nhit->Fill(CMOS_Ncluster);

   htmp_nhit_ch5->Fill(Column_Nhit[4]);
   htmp_nhit_ch6->Fill(Column_Nhit[5]);
   htmp_nhit_ch7->Fill(Column_Nhit[6]);
   htmp_nhit_ch8->Fill(Column_Nhit[7]);


   // ADC distribution 
   std::stringstream hist_title_adc_all;
   std::stringstream hist_title_adc_ch5, hist_title_adc_ch6, hist_title_adc_ch7, hist_title_adc_ch8;
   
   hist_title_adc_all << "h_adc_all" ;
   hist_title_adc_ch5 << "h_adc_ch5" ;
   hist_title_adc_ch6 << "h_adc_ch6" ;
   hist_title_adc_ch7 << "h_adc_ch7" ;
   hist_title_adc_ch8 << "h_adc_ch8" ;

   TH1F* htmp_adc_all    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_all.str().c_str()));     
   TH1F* htmp_adc_ch5    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_ch5.str().c_str()));      
   TH1F* htmp_adc_ch6    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_ch6.str().c_str()));      
   TH1F* htmp_adc_ch7    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_ch7.str().c_str()));      
   TH1F* htmp_adc_ch8    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_adc_ch8.str().c_str()));      

   for(int i=0; i<CMOS_Nhit; i++)
   {
      htmp_adc_all->Fill( CMOS_Hit_ADC[i] );
   }

   for(int i=0;i<Column_Nhit[4];i++){ htmp_adc_ch5->Fill(Column_Hit_ADC[4][i]); }
   for(int i=0;i<Column_Nhit[5];i++){ htmp_adc_ch6->Fill(Column_Hit_ADC[5][i]); }
   for(int i=0;i<Column_Nhit[6];i++){ htmp_adc_ch7->Fill(Column_Hit_ADC[6][i]); }
   for(int i=0;i<Column_Nhit[7];i++){ htmp_adc_ch8->Fill(Column_Hit_ADC[7][i]); }


   // Cluster Info. 
   std::stringstream hist_title_cluster_adc_all,hist_title_cluster_adc_center;

   hist_title_cluster_adc_all << "h_cluster_adc_all" ;
   hist_title_cluster_adc_center << "h_cluster_adc_center" ;

   TH1F* htmp_cluster_adc_all    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_cluster_adc_all.str().c_str()));      
   TH1F* htmp_cluster_adc_center    = dynamic_cast<TH1F*>(gDirectory->Get(hist_title_cluster_adc_center.str().c_str()));      

   for(int i=0; i<CMOS_Ncluster; i++)
   {
      htmp_cluster_adc_all->Fill(CMOS_Cluster_Hit_ADC[i]);

      if( CMOS_Cluster_Hit_Pos_col[i]>=5 && CMOS_Cluster_Hit_Pos_col[i]<=6 )
         {
            htmp_cluster_adc_center->Fill(CMOS_Cluster_Hit_ADC[i]);
         }
   }
}
