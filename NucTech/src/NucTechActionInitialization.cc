#include "NucTechActionInitialization.hh"
#include "NucTechEventAction.hh"
#include "NucTechPrimaryGeneratorAction.hh"
#include "NucTechRunAction.hh"
#include "NucTechSteppingAction.hh"

#include <memory>

NucTechActionInitialization::NucTechActionInitialization(
  const G4String& outFileName)
 : fOutFileName(outFileName)
{}

NucTechActionInitialization::~NucTechActionInitialization() = default;

void NucTechActionInitialization::BuildForMaster() const
{
    // Use unique_ptr to manage memory safely
    auto runAction = std::make_unique<NucTechRunAction>(fOutFileName);

    // Transfer ownership to SetUserAction
    SetUserAction(runAction.release());
}

void NucTechActionInitialization::Build() const
{
    // Use unique_ptr for steppingAction and eventAction
    auto steppingAction = std::make_unique<NucTechSteppingAction>();
    auto eventAction = std::make_unique<NucTechEventAction>(steppingAction.get());

    // Transfer ownership to SetUserAction
    SetUserAction(std::make_unique<NucTechPrimaryGeneratorAction>().release());
    SetUserAction(std::make_unique<NucTechRunAction>(fOutFileName).release());
    SetUserAction(eventAction.release());
    SetUserAction(steppingAction.release());
}