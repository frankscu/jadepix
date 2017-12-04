#include "JadepixDecoder.h"

JadepixDecoder* JadepixDecoder::jadepix_decoder=NULL;

JadepixDecoder* JadepixDecoder::Instance(){
  if (!jadepix_decoder) jadepix_decoder= new JadepixDecoder();
  return jadepix_decoder;
}

void JadepixDecoder::OpenRawFile(const char *dirName, int runnum){
  char fname[500];
  sprintf(fname,"%sRawRunData_%04d.dat", dirName, runnum);
  jadepix_rawFile = new std::fstream();
  jadepix_rawFile->open(fname, std::ios::binary | std::ios::in);

  if(!jadepix_rawFile) {
    std::cout << "" << std::endl;
    std::cout << "------------> Error:: " << fname << " can NOT be opened!!!"<< std::endl;
  }
  std::cout << "------------> Process: " << fname << std::endl;
}

int JadepixDecoder::ReadEvent(){
  std::string line;  //All binary data
  std::string lineHex; // Convert to hex data
  std::string sublineHex; // Part hex data

  if(jadepix_rawFile->good()){
    std::getline(*jadepix_rawFile, line);
    std::string timeStampStr=line.substr(0,35);
    std::cout << "Time stamp: " << timeStampStr <<std::endl;
    jadepix_rawFile->seekg(35,std::ios::beg);
  }else{
    return -1;
  }

  while(jadepix_rawFile->good()){
    //Ignore "\n\t"
    std::getline(*jadepix_rawFile, line, ';');
    lineHex = ToHex(line);

    //std::cout << lineHex << std::endl;
    std::size_t eventHeaderIndex = lineHex.find(EventHeader);
    //std::cout << "Line end:  " << lineHex.substr(eventHeaderIndex-1,2) << std::endl;
    //std::cout << "EventHeader check:  " << lineHex.substr(eventHeaderIndex-1,10) << std::endl;

    //TO DO. Consider line end flag. 0D0A AAAA AAAA problem 
   if(lineHex.substr(eventHeaderIndex-1,10) == LineFeedFlag+EventHeader){
     std::cout<<"The line of time stamp ended with \\r\\n." << std::endl;
     eventHeaderIndex++; 
   }

    //std::cout << "eventHeaderIndex: " << eventHeaderIndex << std::endl;
    if(eventHeaderIndex != std::string::npos){
      //std::cout << "Find event head." <<std::endl;

      //std::cout << "EventLength: " << EventLength << std::endl;
      sublineHex = lineHex.substr(eventHeaderIndex, EventLength);
      //std::cout << sublineHex << std::endl;

      std::size_t eventTrailerIndex = sublineHex.find(EventTrailer);
      //std::cout << "eventTrailerIndex: " << eventTrailerIndex <<std::endl;

      // TO DO. This is a simple method to check the quality of event. It should be updated.
      if(eventTrailerIndex!=EventLength-8){
        std::cout << "This event is broken" <<std::endl;
        //jadepix_eventId++;
        IsGoodEvent[jadepix_eventId]=false;
      }else{
        std::cout << "Find event." <<std::endl;
        jadepix_eventId++;
        IsGoodEvent[jadepix_eventId]=true;
        ReadFrame(sublineHex);
      }
    }
    //if(jadepix_eventId > 1) break; // TO DO. It should be removed
    jadepix_rawFile->seekg(EventLength,std::ios::beg); //TO DO. The pointer of file move eventlength???
  }
  return  jadepix_eventId;
}

int JadepixDecoder::ReadFrame(std::string dataStr){
  JadepixFrame *jadepix_frame = new JadepixFrame(jadepix_frameId);
  for(int row=0; row<48; row++){
    for(int col=0; col<16; col++){
      //framePair = std::make_pair(row,col);
      //std::cout<<"framePair: "<<framePair<<std::endl;
      //frameDataMapStr[framePair]=dataStr.substr((8+row*20*4)+(8+col*4),4);
      //std::cout<<"Pixel: "<<row<<" "<<col<<"  Data String: "<<frameDataStr[framePair]<<std::endl;
      //frameDataMap[framePair]=std::stol(frameDataMapStr[framePair],0,16);
      //std::cout<<"Pixel: "<<row<<" "<<col<<"  Data: "<<frameData[framePair]<<std::endl;
      std::string adcStr = dataStr.substr((8+row*20*4)+(8+col*4),4);
      double adc = std::stol(adcStr,0,16);
      JadepixDigi* _digi = new JadepixDigi(row,col,adc);
      jadepix_frame->AddDigi(_digi);
    }
  }
  jadepix_frameVec.push_back(jadepix_frame); 
  jadepix_frameId++;

  return jadepix_frameId;

}

void JadepixDecoder::Reset(){
  for(jadepix_itframe=jadepix_frameVec.begin(); jadepix_itframe!=jadepix_frameVec.end();++jadepix_itframe){
    delete *jadepix_itframe; 
  }
  jadepix_frameVec.clear();
}

std::string JadepixDecoder::ToHex(const std::string& s, bool upper_case) {
  std::ostringstream ret;

  for(std::string::size_type i=0; i<s.length(); ++i) {
    ret << std::hex
      << std::setfill('0')
      << std::setw(2)
      << (upper_case ? std::uppercase : std::nouppercase)
      << int(s[i]&0xff);
  }
  return ret.str();
}
