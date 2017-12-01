#ifndef JADIPIXDECODER_H
#define JADIPIXDECODER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>

class JadipixDecoder {
  public:
    JadipixDecoder():jadipix_rawFile(NULL),jadipix_eventId(0),jadipix_frameId(0){
      if(jadipix_decoder){ std::cout<<"Warning::MIO is constructed twice."<< std::endl;}
      jadipix_decoder=this;
    };
    ~JadipixDecoder(){if(jadipix_rawFile) delete jadipix_rawFile;};

    void OpenRawFile(const char*, int);
    void CloseRawFile(){jadipix_rawFile->close();};
    
    //TO DO, change the event to a class
    int ReadEvent();
    int ReadFrame(std::string);
    std::pair<int, int> GetFramePair(){return framePair;};
    std::map<std::pair<int, int>, int> GetFrameDataMap(){return frameDataMap;};
    std::string ToHex(const std::string& s, bool upper_case=true);
    static JadipixDecoder* Instance();

  private:
    std::fstream* jadipix_rawFile;
    int jadipix_eventId;
    int jadipix_frameId;
    std::map<int, bool>IsGoodEvent;
    std::pair<int,int>framePair;
    std::map<std::pair<int, int>, std::string>frameDataMapStr;
    std::map<std::pair<int, int>, int>frameDataMap;

    static JadipixDecoder* jadipix_decoder;

    std::string EventHeader = "AAAAAAAA";
    std::string EventTrailer = "F0F0F0F0";
    std::string LineFeedFlag = "0A"; // windows: \r\n -> 0D0A;  linux: \n -> 0A
    std::string RowHeader_Ahead = "5753";
    std::string RowTriler_Ahead = "5753";
    std::size_t EventFlags=4;

    std::size_t adcbits = 4;
    std::size_t RowNumber = 16;
    std::size_t RowFlags = 4;
    std::size_t ColumnNumber = 48;
    const std::size_t EventLength = adcbits*(RowNumber+RowFlags)*ColumnNumber+adcbits*EventFlags;

};

#endif
