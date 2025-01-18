#include "NucTechSteppingAction.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

NucTechSteppingAction::NucTechSteppingAction()
: fV_hitEdep()
, fV_hitPos()
{}

void NucTechSteppingAction::BeginOfEventAction()
{
  fV_hitEdep.clear();
  fV_hitPos.clear();
}

void NucTechSteppingAction::EndOfEventAction()
{
  // Check at least one phantom hit happened during the event
  if (fV_hitEdep.empty()) return;

  G4AnalysisManager* mgr = G4AnalysisManager::Instance();

  // Store the energy deposited inside the phantom during the event
  const G4double Edep_event =
   std::accumulate(fV_hitEdep.begin(), fV_hitEdep.end(), 0.);

  mgr->FillNtupleDColumn(1, 0, Edep_event/MeV);
  mgr->AddNtupleRow(1);


  // Then record the individual hit energy and coordinates of this event
  for (std::size_t i = 0; i < fV_hitEdep.size(); i++) 
  {
    auto energy = fV_hitEdep[i] / MeV;
    auto position = fV_hitPos[i];
    
    mgr->FillNtupleDColumn(2, 0, energy);
    mgr->FillNtupleDColumn(2, 1, position.x() / cm);
    mgr->FillNtupleDColumn(2, 2, position.y() / cm);
    mgr->FillNtupleDColumn(2, 3, position.z() / cm);
    mgr->AddNtupleRow(2);
  }
}

void NucTechSteppingAction::UserSteppingAction(const G4Step* step)
{ 
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

  // To only record the interactions inside the phantom
  if (postStepPoint->GetPhysicalVolume() == nullptr) return;
  if (postStepPoint->GetPhysicalVolume()->GetName() != "Detector") return;

  // Store the energy deposited in the phantom
  const G4double Edep = step->GetTotalEnergyDeposit();
  
  if(Edep <= 0.)
  {
    return;
  } 

  fV_hitEdep.push_back(Edep);
  
  // Store the hit position
  const G4ThreeVector hitPos = postStepPoint->GetPosition();
  fV_hitPos.push_back(hitPos);      
}