#ifndef JadeParser_H
#define JadeParser_H 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include <algorithm>
#include <vector>
#include <map>

using namespace std;

typedef pair<string,string> JadeParserPair;
typedef map<string,string> JadeParserMap;

class JadeParser{
  public:
    static JadeParser* Instance();
    ~JadeParser();

    void Initialize(string confile);
    string GetParName() const {return m_par;};
    bool ParExist(string parName);
    int GetIntPar(string parName);
    double GetDoublePar(string parName);
    string GetStringPar(string parName);

    bool GetIntVector(string parName, int vecSize, vector<int>& dbParVec);
    bool GetDoubleVector(string parName, int vecSize, vector<double>& dbParVec);
    bool GetStringVector(string parName, int vecSize, vector<string>& dbParVec);
    
    void SplitString(std::string& str,std::string pattern,std::vector<std::string>& result);

    void SaveConfig();
  private:
    JadeParser();

    bool GetVectorCore(int vecSize, vector<string>& dbParVec);
    
    JadeParserMap m_JadeParserMap;
    string m_par;
    string m_val;

    static JadeParser* m_JadeParser;
    bool m_initFlag;
};

#endif
