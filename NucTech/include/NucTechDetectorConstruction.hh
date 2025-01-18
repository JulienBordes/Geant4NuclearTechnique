#ifndef NucTechDetectorConstruction_hh
#define NucTechDetectorConstruction_hh 1

#include "G4VUserDetectorConstruction.hh"

class G4UserLimits;

class NucTechDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  NucTechDetectorConstruction();
  virtual ~NucTechDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

private:
  G4UserLimits* fStepLimit;
};

#endif

