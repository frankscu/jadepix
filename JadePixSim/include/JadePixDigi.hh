//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description:
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: Oct.26, 2004
//Modified:
//Comment:
//---------------------------------------------------------------------------//

#ifndef JadePixDigi_h
#define JadePixDigi_h 1

#include <CLHEP/Units/PhysicalConstants.h>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class JadePixDigi : public G4VDigi
{
  public:
    JadePixDigi();
    virtual ~JadePixDigi();

    JadePixDigi(const JadePixDigi&);
    const JadePixDigi& operator=(const JadePixDigi&);

    virtual G4int operator==(const JadePixDigi&) const;
    inline void* operator new(size_t);
    inline void  operator delete(void*);

  //    virtual void Draw();
    virtual void Print();

public:
  
  void SetTrackID(G4int track){ trackID = track; };
  void SetLayerID(G4int layer){layerID = layer;};
  void SetLadderID(G4int ladder){ladderID = ladder;};
  void SetChipID(G4int chip){chipID = chip;};
  void SetGlobalChipID(G4int chip){globalChipID = chip;};
  void SetRow(G4int row){rowID = row;};
  void SetCol(G4int column){columnID = column;};
  void SetEdep     (G4double de)      { edep = de;};
  void AddEdep(G4double de) {edep += de;};
  void SetGlobalT   (G4double time)    { globalT = time;};
  void SetADC (G4int a) {adc=a;};
  void SetTDC (G4int t) {tdc=t;};
  
  G4int GetTrackID() const { return  trackID ;};
  G4int GetLayerID() const { return layerID ;};
  G4int GetLadderID() const { return ladderID ;};
  G4int GetChipID() const { return chipID ;};
  G4int GetGlobalChipID() const { return globalChipID ;};
  G4int GetRow() const { return rowID ;};
  G4int GetCol() const { return columnID ;};
  G4double GetEdep() const { return edep; };      
  G4double GetGlobalT() const { return globalT;};     
  G4int GetADC() const {return adc;};
  G4int GetTDC() const {return tdc;};
  
private:
  
  G4int trackID;
  G4int layerID;
  G4int ladderID;
  G4int chipID;
  G4int globalChipID;
  G4int rowID;
  G4int columnID;
  G4double      edep;
  G4double      globalT;
  G4int adc;
  G4int tdc;

};

typedef G4TDigiCollection<JadePixDigi> JadePixDigisCollection;

extern G4Allocator<JadePixDigi> JadePixDigiAllocator;

inline void* JadePixDigi::operator new(size_t)
{
  void *aDigi;
  aDigi = (void *) JadePixDigiAllocator.MallocSingle();
  return aDigi;
}

inline void JadePixDigi::operator delete(void *aDigi)
{
  JadePixDigiAllocator.FreeSingle((JadePixDigi*) aDigi);
}

#endif

