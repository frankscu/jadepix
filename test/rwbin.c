int rwbin(){
  string fileout="../data/sampleTest.bin";
  fout = new fstream(fileout.c_str(),ios::out | ios::binary);

  if(!fout){
    cout<<""<<endl;
    cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
  }
  int adc[]={100,101,102,244,245,246};


  for(int i=0; i<6; i++){
    std::stringstream ssA, ssB;
    ssA << std::hex<<setfill('0')<<setw(1)<<int(adc[i]/16);
    ssB << std::hex<<setfill('0')<<setw(1)<<int(adc[i]%16);
    string adcstr =  ssA.str() + ssB.str();
    //string adcstrA = "\\x" + ssA.str();
    //string adcstrB = "\\x" + ssB.str();
    std::cout << "ADC string:  " << adcstr << std::endl;
    (*fout)<<std::hex<<adcstr;
    (*fout)<<TString(adcstr);
    (*fout)<<std::hex<<std::setfill('\x00')<<std::setw(2)<<'\x00';
  }
  fout->close();

  return 0;

}

