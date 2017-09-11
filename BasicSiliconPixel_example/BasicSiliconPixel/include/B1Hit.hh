#ifndef B1Hit_h
#define B1Hit_h 1


#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class B1EventAction;

class B1Hit : public G4VHit 
{
  public:
    /// Constructor
    B1Hit();
    B1Hit(const B1Hit&);
    /// Destructor
    virtual ~B1Hit();
    
    // operators
    const B1Hit& operator=(const B1Hit&);
    G4int operator==(const B1Hit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    virtual void  Draw();
    virtual void  Print();
  

    void SetEdep    (G4double de) 		{ fEdep = de; };
    void AddEdep    (G4double de) 		{ fEdep += de;}
    G4double GetEdep() 			const   { return fEdep; };

    void SetShapeNb (G4int shap)		{ fShapeNb = shap; }
    G4int GetShapeNb()			const   { return fShapeNb; }

    void SetPos     (G4ThreeVector xyz)		{ fPos = xyz; };
    G4ThreeVector GetPos() 		const 	{ return fPos; };

    void SetPolarization (G4ThreeVector rthetaphi) { fPolarization = rthetaphi; };
    G4ThreeVector GetPolar()     const   { return fPolarization; };

    void SetTrackID (G4int track)  		{ fTrackID = track; };
    G4int GetTrackID()			const	{ return fTrackID; };
  
  private:
    G4double 		fEdep;
    G4ThreeVector 	fPos;
    G4int		fTrackID;
    G4int 		fShapeNb;
    G4ThreeVector       fPolarization;
};

typedef G4THitsCollection<B1Hit> B1HitsCollection;
extern G4ThreadLocal G4Allocator<B1Hit>* B1HitAllocator;

inline void* B1Hit::operator new(size_t)
{
  if(!B1HitAllocator)
      B1HitAllocator = new G4Allocator<B1Hit>;
  return (void *) B1HitAllocator->MallocSingle();
}

inline void B1Hit::operator delete(void *aHit)
{
  B1HitAllocator->FreeSingle((B1Hit*) aHit);
}

#endif

