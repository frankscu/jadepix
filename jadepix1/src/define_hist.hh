void make_histogram(TObjArray *hlist)
{
   // Nhit
   std::stringstream hist_title_nhit, hist_title_cluster_nhit;
   std::stringstream hist_title_nhit_ch5, hist_title_nhit_ch6, hist_title_nhit_ch7, hist_title_nhit_ch8;
   
   hist_title_nhit     << "h_nhit" ;
   hist_title_cluster_nhit     << "h_cluster_nhit" ;
   
   hist_title_nhit_ch5 << "h_nhit_ch5" ;
   hist_title_nhit_ch6 << "h_nhit_ch6" ;
   hist_title_nhit_ch7 << "h_nhit_ch7" ;
   hist_title_nhit_ch8 << "h_nhit_ch8" ;

   TH1F* htmp_nhit        = new TH1F(hist_title_nhit.str().c_str(), "Number of hits", 30, 0, 30);           
   TH1F* htmp_cluster_nhit   = new TH1F(hist_title_cluster_nhit.str().c_str(), "Cluster NHIT", 30, 0, 30);           

   TH1F* htmp_nhit_ch5        = new TH1F(hist_title_nhit_ch5.str().c_str(), "Number of hits (column 5)", 20, 0, 20);           
   TH1F* htmp_nhit_ch6        = new TH1F(hist_title_nhit_ch6.str().c_str(), "Number of hits (column 6)", 20, 0, 20);           
   TH1F* htmp_nhit_ch7        = new TH1F(hist_title_nhit_ch7.str().c_str(), "Number of hits (column 7)", 20, 0, 20);           
   TH1F* htmp_nhit_ch8        = new TH1F(hist_title_nhit_ch8.str().c_str(), "Number of hits (column 8)", 20, 0, 20);           

   hlist->Add(htmp_nhit);
   hlist->Add(htmp_cluster_nhit);
   hlist->Add(htmp_nhit_ch5);
   hlist->Add(htmp_nhit_ch6);
   hlist->Add(htmp_nhit_ch7);
   hlist->Add(htmp_nhit_ch8);


   // ADC distribution 
   std::stringstream hist_title_adc_all;
   std::stringstream hist_title_adc_ch5, hist_title_adc_ch6, hist_title_adc_ch7, hist_title_adc_ch8;
   
   hist_title_adc_all << "h_adc_all" ;
   hist_title_adc_ch5 << "h_adc_ch5" ;
   hist_title_adc_ch6 << "h_adc_ch6" ;
   hist_title_adc_ch7 << "h_adc_ch7" ;
   hist_title_adc_ch8 << "h_adc_ch8" ;

   TH1F* htmp_adc_all  = new TH1F(hist_title_adc_all.str().c_str(), "ADC distribution", 10000, -6000, 4000);       
   TH1F* htmp_adc_ch5  = new TH1F(hist_title_adc_ch5.str().c_str(), "ADC distribution (column 5)", 10000, -6000, 4000);       
   TH1F* htmp_adc_ch6  = new TH1F(hist_title_adc_ch6.str().c_str(), "ADC distribution (column 6)", 10000, -6000, 4000);       
   TH1F* htmp_adc_ch7  = new TH1F(hist_title_adc_ch7.str().c_str(), "ADC distribution (column 7)", 10000, -6000, 4000);       
   TH1F* htmp_adc_ch8  = new TH1F(hist_title_adc_ch8.str().c_str(), "ADC distribution (column 8)", 10000, -6000, 4000);       

   hlist->Add(htmp_adc_all);
   hlist->Add(htmp_adc_ch5);
   hlist->Add(htmp_adc_ch6);
   hlist->Add(htmp_adc_ch7);
   hlist->Add(htmp_adc_ch8);


   std::stringstream hist_title_cluster_adc_all,hist_title_cluster_adc_center;

   hist_title_cluster_adc_all << "h_cluster_adc_all" ;
   hist_title_cluster_adc_center << "h_cluster_adc_center" ;

   TH1F* htmp_cluster_adc_all  = new TH1F(hist_title_cluster_adc_all.str().c_str(), "Cluster_ADC distribution", 10000, -6000, 4000);       
   TH1F* htmp_cluster_adc_center  = new TH1F(hist_title_cluster_adc_center.str().c_str(), "Cluster_ADC distribution (Center Part)", 10000, -6000, 4000);       

   hlist->Add(htmp_cluster_adc_all);
   hlist->Add(htmp_cluster_adc_center);
}      
