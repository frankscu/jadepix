#ifndef JadepixDecoder_H
#define JadepixDecoder_H

// C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>

// Jadipix
#include "JadepixFrame.h"
#include "JadepixDigi.h"

class JadepixDecoder{
  public:
    JadepixDecoder():jadepix_rawFile(NULL),jadepix_eventId(0),jadepix_frameId(0){
      if(jadepix_decoder){ std::cout<<"Warning::MIO is constructed twice."<< std::endl;}
      jadepix_decoder=this;
    };
    ~JadepixDecoder(){if(jadepix_rawFile) delete jadepix_rawFile;};

    void OpenRawFile(const char*, int);
    void CloseRawFile(){jadepix_rawFile->close();};
    
    int ReadEvent();
    int ReadFrame(std::string);
    int NoOfFrame(){return jadepix_frameVec.size();};
    JadepixFrame* GetFrame(int frameId) const {return jadepix_frameVec[frameId];};

    void Reset();
    std::string ToHex(const std::string& s, bool upper_case=true);
    static JadepixDecoder* Instance();

  private:
    std::fstream* jadepix_rawFile;
    int jadepix_eventId;
    int jadepix_frameId;
    std::map<int, bool>IsGoodEvent;
    
    std::vector<JadepixFrame*>jadepix_frameVec; 
    std::vector<JadepixFrame*>::iterator jadepix_itframe; 

    static JadepixDecoder* jadepix_decoder;

    std::string EventHeader = "AAAAAAAA";
    std::string EventTrailer = "F0F0F0F0";
    std::string LineFeedFlag = "0A"; // windows: \r\n -> 0D0A;  linux: \n -> 0A
    std::string RowHeader_Ahead = "5753";
    std::string RowTriler_Ahead = "5753";
    std::size_t EventFlags=4;

    std::size_t adcbits = 4;
    std::size_t RowNumber = 48;
    std::size_t RowFlags = 4;
    std::size_t ColumnNumber = 16;
    const std::size_t EventLength = adcbits*(ColumnNumber+RowFlags)*RowNumber+adcbits*EventFlags;

};

#endif //JadepixDecoder_H
