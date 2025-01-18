#include "NucTechDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

NucTechDetectorConstruction::NucTechDetectorConstruction()
: fStepLimit(nullptr)
{}

NucTechDetectorConstruction::~NucTechDetectorConstruction()
{
  delete fStepLimit;
}

G4VPhysicalVolume* NucTechDetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;

  // Materials
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Definition of the volumes - exeperimental hall and phantom

  /***** Experimental hall *****/
  
  G4double worldHalfLength = 2.*m;

  G4VSolid* world =
   new G4Box("World", worldHalfLength, worldHalfLength, worldHalfLength);
  
  G4LogicalVolume* world_logical = 
  new G4LogicalVolume(world, world_mat, "World");
  
  G4VPhysicalVolume* world_physical =
   new G4PVPlacement(G4Transform3D(), world_logical, "World",
                     0, false, 0);

  /***** Detector *****/

  G4double det_halfX = 15.*cm;
  G4double det_halfY = 15.*cm;
  G4double det_halfDepth = 15.*cm;
 
  G4VSolid* det_solid =
   new G4Box("Detector", det_halfX, det_halfY, det_halfDepth);

  G4LogicalVolume* det_logical =
   new G4LogicalVolume(det_solid, det_mat, "Detector");

  G4double& det_PosZ = det_halfDepth;

  new G4PVPlacement(nullptr,                              // No rotation
                    G4ThreeVector(0., 0., det_PosZ),      // Translation
                    det_logical,                          // Logical volume
                    "Detector",                           // Name
                    world_logical,                        // Mother volume
                    false,                                // Not a parameterized volume
                    0,                                    // Copy number
                    checkOverlaps);                       // Overlap checking

  /***** Step limit *****/
  
  G4double maxStep = 1.*mm;
  fStepLimit = new G4UserLimits(maxStep);
  det_logical->SetUserLimits(fStepLimit);

  /***** Visualisation *****/

  world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  G4VisAttributes* det_vis =
   new G4VisAttributes(G4Colour(173./255., 216./255., 230./255., .8));
  det_vis->SetDaughtersInvisible(false);
  det_vis->SetForceSolid(true);
  det_logical->SetVisAttributes(det_vis);

  return world_physical;
}