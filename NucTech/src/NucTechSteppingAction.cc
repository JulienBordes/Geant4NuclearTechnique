#include "NucTechSteppingAction.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

NucTechSteppingAction::NucTechSteppingAction() : fV_hitEdep(), fV_hitPos() {}

void NucTechSteppingAction::BeginOfEventAction() {
  fV_hitEdep.clear();
  fV_hitPos.clear();
}

void NucTechSteppingAction::EndOfEventAction() {
  // Check at least one phantom hit happened during the event
  if (fV_hitEdep.empty())
    return;

  G4AnalysisManager *mgr = G4AnalysisManager::Instance();

  // Store the energy deposited inside the phantom during the event
  const G4double Edep_event =
      std::accumulate(fV_hitEdep.begin(), fV_hitEdep.end(), 0.);

  mgr->FillNtupleFColumn(0, 0, static_cast<G4float>(Edep_event / MeV));
  mgr->AddNtupleRow(0);

  // Record individual hits
  for (std::size_t i = 0; i < fV_hitEdep.size(); i++) {
    const auto energy = fV_hitEdep[i] / MeV;
    const auto position = fV_hitPos[i];

    mgr->FillNtupleFColumn(1, 0, static_cast<G4float>(energy));
    mgr->FillNtupleFColumn(1, 1, static_cast<G4float>(position.x() / cm));
    mgr->FillNtupleFColumn(1, 2, static_cast<G4float>(position.y() / cm));
    mgr->FillNtupleFColumn(1, 3, static_cast<G4float>(position.z() / cm));
    mgr->AddNtupleRow(1);
  }
}

void NucTechSteppingAction::UserSteppingAction(const G4Step *step) {
  G4StepPoint *postStepPoint = step->GetPostStepPoint();

  // To only record the interactions inside the phantom
  if (postStepPoint->GetPhysicalVolume() == nullptr) {
    return;
  }
  if (postStepPoint->GetPhysicalVolume()->GetName() != "Detector") {
    return;
  }

  // Store the energy deposited in the phantom
  const G4double Edep = step->GetTotalEnergyDeposit();

  if (Edep <= 0.) {
    return;
  }

  fV_hitEdep.push_back(Edep);

  // Store the hit position
  const G4ThreeVector hitPos = postStepPoint->GetPosition();
  fV_hitPos.push_back(hitPos);
}