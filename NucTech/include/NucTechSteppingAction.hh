#ifndef AnnihilationPhotonsSteppingAction_hh
#define AnnihilationPhotonsSteppingAction_hh

#include "NucTechVSteppingAction.hh"

#include "G4ThreeVector.hh"

#include <numeric>
#include <vector>

class NucTechSteppingAction: public NucTechVSteppingAction
{
public:
  NucTechSteppingAction();
  virtual void BeginOfEventAction();
  virtual void UserSteppingAction(const G4Step*);
  virtual void EndOfEventAction();

private:
  // These are used to remember quantities from call to call of
  // UserSteppingAction
  std::vector<G4double> fV_hitEdep;
  std::vector<G4ThreeVector> fV_hitPos;
};

#endif
