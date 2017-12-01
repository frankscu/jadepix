#include "JadipixDecoder.h"

JadipixDecoder* JadipixDecoder::jadipix_decoder=NULL;

JadipixDecoder* JadipixDecoder::Instance(){
  if (!jadipix_decoder) jadipix_decoder= new JadipixDecoder();
  return jadipix_decoder;
}

void JadipixDecoder::OpenRawFile(const char *dirName, int runnum){
  char fname[500];
  sprintf(fname,"%sRawRunData_%04d.dat", dirName, runnum);
  jadipix_rawFile = new std::fstream();
  jadipix_rawFile->open(fname, std::ios::binary | std::ios::in);

  if(!jadipix_rawFile) {
    std::cout << "" << std::endl;
    std::cout << "------------> Error:: " << fname << " can NOT be opened!!!"<< std::endl;
  }
  std::cout << "------------> Process: " << fname << std::endl;
}

int JadipixDecoder::ReadEvent(){
  std::string line;  //All binary data
  std::string lineHex; // Convert to hex data
  std::string sublineHex; // Part hex data

  if(jadipix_rawFile->good()){
    std::getline(*jadipix_rawFile, line);
    std::string timeStampStr=line.substr(0,35);
    std::cout << "Time stamp: " << timeStampStr <<std::endl;
    jadipix_rawFile->seekg(35,std::ios::beg);
  }else{
    return -1;
  }

  while(jadipix_rawFile->good()){
    //Ignore "\n\t"
    std::getline(*jadipix_rawFile, line, ';');
    lineHex = ToHex(line);

    //std::cout << lineHex << std::endl;
    std::size_t eventHeaderIndex = lineHex.find(EventHeader);
    //std::cout << "Line end:  " << lineHex.substr(eventHeaderIndex-1,2) << std::endl;
    //std::cout << "EventHeader check:  " << lineHex.substr(eventHeaderIndex-1,10) << std::endl;

    // Consider line end flag
   if(lineHex.substr(eventHeaderIndex-1,10) == LineFeedFlag+EventHeader){
     std::cout<<"Line ended with \\r\\n" << std::endl;
     eventHeaderIndex++; 
   }

    std::cout << "eventHeaderIndex: " << eventHeaderIndex << std::endl;
    if(eventHeaderIndex != std::string::npos){
      std::cout << "Find event head." <<std::endl;

      std::cout << "EventLength: " << EventLength << std::endl;
      sublineHex = lineHex.substr(eventHeaderIndex, EventLength);
      std::cout << sublineHex << std::endl;

      std::size_t eventTrailerIndex = sublineHex.find(EventTrailer);
      std::cout << "eventTrailerIndex: " << eventTrailerIndex <<std::endl;

      // TO DO. This is a simple method to check the quality of event. It should be updated.
      if(eventTrailerIndex!=EventLength-8){
        std::cout << "This event is broken" <<std::endl;
        jadipix_eventId++;
        IsGoodEvent[jadipix_eventId]=false;
      }else{
        std::cout << "Find event." <<std::endl;
        jadipix_eventId++;
        IsGoodEvent[jadipix_eventId]=true;
        ReadFrame(sublineHex);
      }
    }
    if(jadipix_eventId > 10) break; // TO DO. It should be removed
    jadipix_rawFile->seekg(EventLength,std::ios::beg); //TO DO. The pointer of file move eventlength???
  }
  return  jadipix_eventId;
}

int JadipixDecoder::ReadFrame(std::string dataStr){
  for(int row=0; row<48; row++){
    for(int col=0; col<16; col++){
      framePair = std::make_pair(row,col);
      //std::cout<<"framePair: "<<framePair<<std::endl;
      frameDataMapStr[framePair]=dataStr.substr((8+row*20*4)+(8+col*4),4);
      //std::cout<<"Pixel: "<<row<<" "<<col<<"  Data String: "<<frameDataStr[framePair]<<std::endl;
      frameDataMap[framePair]=std::stol(frameDataMapStr[framePair],0,16);
      //std::cout<<"Pixel: "<<row<<" "<<col<<"  Data: "<<frameData[framePair]<<std::endl;
    }
  }
  jadipix_frameId++;

  return jadipix_frameId;

}

std::string JadipixDecoder::ToHex(const std::string& s, bool upper_case) {
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
