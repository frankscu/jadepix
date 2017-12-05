#ifndef JadepixDigi_H
#define JadepixDigi_H


class JadepixDigi{
  public:
    JadepixDigi(){};
    JadepixDigi(int rowId, int colId, double adc):jadepix_rowId(rowId),jadepix_colId(colId), jadepix_adc(adc){};
    ~JadepixDigi(){};

    void SetADC(double adc){jadepix_adc = adc;};
    void SetRowId(int rowId){jadepix_rowId = rowId;};
    void SetColId(int colId){jadepix_colId = colId;};

    double GetADC() const {return jadepix_adc;};
    int GetRowId() const {return jadepix_rowId;};
    int GetColId() const {return jadepix_colId;};

  private:
    int jadepix_rowId;
    int jadepix_colId;
    double jadepix_adc;

};

#endif //JadepixDigi_H
