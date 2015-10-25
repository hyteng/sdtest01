/*=============================================================================
#     FileName: SDTest01DetectorConstruction.cpp
#         Desc: detector construction
#       Author: Haiyun Teng
#        Email: haiyun.teng@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2015-10-25 12:08:38
#      History: 
=============================================================================*/

#include "SDTest01DetectorConstruction.hh"
#include "SDTest01MagneticField.hh"
#include "SDTest01TrackerSD.hh"
#include "SDTest01MuonModel.hh"
#include "SDTest01TrackerROGeometry.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProductionCuts.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

SDTest01DetectorConstruction::SDTest01DetectorConstruction() : experimentalHall_log(0), trackerBlock_log(0), trackerLayer_log(0), experimentalHall_phys(0), trackerBlock_phys(0), trackerLayer_phys(0) {
    efficiency = 100.0;
    addNoise = false;
    //fpMagField = new SDTest01MagneticField();
    //fpMagField->SetMagFieldValue(G4ThreeVector(0., 0.05, 0.));
}

SDTest01DetectorConstruction::~SDTest01DetectorConstruction() {
    //delete fpMagField;
    delete fpSolenoidPart;
    delete fStepLimit;
}

G4VPhysicalVolume* SDTest01DetectorConstruction::Construct() {

    //------------------------------------------------------ materials

    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double nel;
    G4double density;

    H = new G4Element("Hydrogen", "H", z=1., a=  1.01*g/mole);
    C = new G4Element("Carbon",   "C", z=6., a= 12.01*g/mole);
    N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
    O = new G4Element("Oxygen",   "O", z=8., a= 16.00*g/mole);

    G4Material* Ar = new G4Material("ArgonGas", z=18., a=39.95*g/mole, density=1.782*mg/cm3);
    G4Material* Al = new G4Material("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);
    G4Material* Pb = new G4Material("Lead", z=82., a=207.19*g/mole, density=11.35*g/cm3);
    G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
    Air->AddElement(N, 70.*perCent);
    Air->AddElement(O, 30.*perCent);
    G4Material* Silicon = new G4Material("Silicon", z=14., a= 28.09*g/mole, density= 2.33*g/cm3);
    G4Material* Scinti = new G4Material("Scintillator", density= 1.032*g/cm3, nel=2);
    Scinti->AddElement(C, 9);
    Scinti->AddElement(H, 10);

    //------------------------------------------------------ volumes

    //------------------------------ experimental hall (world volume)
    //------------------------------ beam line along x axis

    G4double expHall_x = 1.0*m;
    G4double expHall_y = 1.0*m;
    G4double expHall_z = 1.0*m;
    G4Box* experimentalHall_box = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
    experimentalHall_log = new G4LogicalVolume(experimentalHall_box, Ar, "expHall_log", 0, 0, 0);
    experimentalHall_phys = new G4PVPlacement(0, G4ThreeVector(), experimentalHall_log, "expHall", 0, false, 0);

    //------------------------------ a solenoid block
    G4double part_x = 50.0*cm;
    G4double part_y = 100.0*cm;
    G4double part_z = 100.0*cm;
    G4Box* solenoidPart_box = new G4Box("solenoidPart_box", part_x, part_y, part_z);
    solenoidPart_log = new G4LogicalVolume(solenoidPart_box, Ar, "solenoidPart_log", 0, 0, 0);
    G4double partPos_x = -0.5*m;
    G4double partPos_y = 0.0*m;
    G4double partPos_z = 0.0*m;
    solenoidPart_phys = new G4PVPlacement(0, G4ThreeVector(partPos_x, partPos_y, partPos_z), solenoidPart_log, "solenoidPart", experimentalHall_log, false, 0);

    //------------------------------ solenoid part B field
    fpSolenoidPart = new SDTest01SimpleSolenoidPart(G4ThreeVector(0,-0.05,0), solenoidPart_log, G4ThreeVector(partPos_x, partPos_y, partPos_z));

    //------------------------------ a tracker block

    G4double block_x = 50.0*cm;
    G4double block_y = 100.0*cm;
    G4double block_z = 100.0*cm;
    G4Box* trackerBlock_box = new G4Box("trackerBlock_box", block_x, block_y, block_z);
    trackerBlock_log = new G4LogicalVolume(trackerBlock_box, Al, "trackerBlock_log", 0, 0, 0);
    G4double blockPos_x = 0.5*m;
    G4double blockPos_y = 0.0*m;
    G4double blockPos_z = 0.0*m;
    trackerBlock_phys = new G4PVPlacement(0, G4ThreeVector(blockPos_x, blockPos_y, blockPos_z), trackerBlock_log, "trackerBlock", experimentalHall_log, false, 0);

    //------------------------------ tracker layers

    G4double tracker_x = 1.*cm;
    G4double tracker_y = 100.*cm;
    G4double tracker_z = 100.*cm;
    G4Box* trackerLayer_box = new G4Box("trackerLayer_box", tracker_x, tracker_y, tracker_z);
    trackerLayer_log = new G4LogicalVolume(trackerLayer_box, Scinti, "trackerLayer_log", 0, 0, 0);
    double maxStep = 0.1*tracker_x;
    fStepLimit = new G4UserLimits(maxStep);
    trackerLayer_log->SetUserLimits(fStepLimit);
    
    for(G4int i = 0; i < 5; i++)  {
        G4double trackerPos_x = (i-2)*20.*cm;
        G4double trackerPos_y = 0.0*m;
        G4double trackerPos_z = 0.0*m;
        trackerLayer_phys = new G4PVPlacement(0, G4ThreeVector(trackerPos_x, trackerPos_y, trackerPos_z), trackerLayer_log, "trackerLayer", trackerBlock_log, false, i);
    }

    //------------------------------ SD 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String trackerSDName = "SDTest01/TrackerSD";
    SDTest01TrackerSD* trackerSD = new SDTest01TrackerSD(trackerSDName, efficiency, addNoise);
    SDman->AddNewDetector(trackerSD);
    trackerLayer_log->SetSensitiveDetector(trackerSD);

    G4String ROgeometryName = "TrackerROGeometry";
    G4VReadOutGeometry* trackerRO = new SDTest01TrackerROGeometry(ROgeometryName, StripN);
    trackerRO->BuildROGeometry();
    trackerSD->SetROgeometry(trackerRO);

    //------------------ Parameterisation Models --------------------------
    
    G4Region* trackerRegion = new G4Region("tracker_region");
    trackerRegion->AddRootLogicalVolume(trackerLayer_log);
    std::vector<double> cuts; cuts.push_back(1.0*mm);cuts.push_back(1.0*mm);cuts.push_back(1.0*mm);
    trackerRegion->SetProductionCuts(new G4ProductionCuts());
    trackerRegion->GetProductionCuts()->SetProductionCuts(cuts);
    new SDTest01MuonModel("MuonModel", trackerRegion); 
    

    //------------------------------ Visualisaion
    G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    experimentalHall_log->SetVisAttributes(BoxVisAtt);
    trackerBlock_log->SetVisAttributes(BoxVisAtt);

    G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    trackerLayer_log->SetVisAttributes(ChamberVisAtt);
    //------------------------------------------------------------------

    return experimentalHall_phys;
}

void SDTest01DetectorConstruction::SetMagField(G4double fieldValue) {
    fpMagField->SetMagFieldValue(fieldValue);
}
