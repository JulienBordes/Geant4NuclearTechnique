#include "NucTechRunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"

NucTechRunAction::NucTechRunAction(const G4String &outFileName)
    : fOutputFileName(outFileName) {}

NucTechRunAction::~NucTechRunAction() = default;

void NucTechRunAction::BeginOfRunAction(const G4Run * /*run*/) {
  G4AnalysisManager *mgr = G4AnalysisManager::Instance();

  mgr->SetDefaultFileType("root");
  mgr->SetNtupleMerging(true);
  // mgr->SetFirstNtupleId(1);

  const G4bool fileOpen = mgr->OpenFile(fOutputFileName);

  if (!fileOpen) {
    const G4String errorMessage =
        "Could not open the output file: " + mgr->GetFileName();

    G4Exception("NucTechRunAction::BeginOfRunAction", "FileOpenError",
                FatalException, errorMessage.c_str());
    return;
  }

  mgr->CreateNtuple("EnergySpectrum", "NucTech");
  mgr->CreateNtupleFColumn("EventEdep");
  mgr->FinishNtuple();

  mgr->CreateNtuple("IndividualHits", "NucTech");
  mgr->CreateNtupleFColumn("HitEdep");
  mgr->CreateNtupleFColumn("HitX");
  mgr->CreateNtupleFColumn("HitY");
  mgr->CreateNtupleFColumn("HitZ");
  mgr->FinishNtuple();

  if (IsMaster()) {
    G4cout << "Opening: " << mgr->GetFileName() << G4endl;
    fTimer.Start();
  }
}

void NucTechRunAction::EndOfRunAction(const G4Run * /*run*/) {
  if (IsMaster()) {
    fTimer.Stop();
    G4cout << "\nReal simulation time = " << fTimer.GetRealElapsed() << "s \n"
           << G4endl;
  }

  G4AnalysisManager *mgr = G4AnalysisManager::Instance();
  mgr->Write();
  mgr->CloseFile();
  mgr->Clear();
}