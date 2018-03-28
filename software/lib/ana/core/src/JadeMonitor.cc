#include "JadeMonitor.hh"

JadeMonitor::JadeMonitor(const JadeOption& opt)
  :m_opt(opt),m_ev_n(0), m_ev_print(0),
   m_last_df_n(0), m_enable_print_discon(false){
  m_ev_print = opt.GetIntValue("PRINT_EVENT_N");
  m_enable_print_discon = opt.GetBoolValue("PRINT_EVENT_DISCONTINUOUS");

  m_last_frame_adc.resize(16*48,0);
  m_cds_frame_adc.resize(16*48,0);
}

JadeMonitor::~JadeMonitor(){

}

void JadeMonitor::Reset(){
  m_ev_n = 0;
}

void JadeMonitor::Monitor(JadeDataFrameSP df){
  if(m_ev_print!=0 && m_ev_n%m_ev_print == 0){
    df->Print(std::cout);
  }
  m_ev_n++;

  if(m_enable_print_discon){
    uint32_t df_n = df->GetFrameCount();
    if(m_last_df_n!=0 && m_last_df_n+1!=df_n){
    std::cout<<"JadeMonitor: data frame counter is discontinuous "
	     <<m_last_df_n<<" =>  "<< df_n<<std::endl;
    }
    m_last_df_n = df_n;
  }

  uint32_t df_n = df->GetFrameCount();
  
  auto adc_value = df->GetFrameData(); 

  if( m_ev_n %2==1 ) m_last_frame_adc.assign(adc_value.begin(), adc_value.end()); 

  if( m_ev_n %2==0 ) {

    std::transform(adc_value.begin(), adc_value.end(), m_last_frame_adc.begin(), m_cds_frame_adc.begin(), std::minus<int16_t>()); 

    //df->SetFrameCDS(m_cds_frame_adc);
    
    auto u_seed = df->CalFrameSeed(m_cds_frame_adc);
    
    //auto u_seed = df->GetFrameSeed();

    for(auto& seed : u_seed){
      std::cout << "seed" << seed.second << std::endl;
    }
  }
}
