/*=============================================================================
#     FileName: SDTest01DetectorConstruction.h
#         Desc: detector construction header
#       Author: Haiyun.Teng
#        Email: tenghy@ihep.ac.cn
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2015-10-25 12:02:15
#      History:
=============================================================================*/

#ifndef SDTest01DetectorConstruction_h 
#define SDTest01DetectorConstruction_h 1

#include "SDTest01MagneticField.hh"
#include "SDTest01SimpleSolenoidPart.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4UserLimits;
class G4Element;

class SDTest01DetectorConstruction : public G4VUserDetectorConstruction {
    public:
        SDTest01DetectorConstruction();
        ~SDTest01DetectorConstruction();

    public:
        G4VPhysicalVolume* Construct();

        inline void SetROStripNumber(G4int n) {StripN = n;};
        inline G4int GetROStripNumber() {return StripN;};
        inline void SetNoise(G4bool noise) {addNoise = noise;};
        inline void SetEfficiency(G4double eff) {efficiency = eff;};
    private:
        void SetMagField(G4double);

        G4LogicalVolume* experimentalHall_log;
        G4LogicalVolume* trackerBlock_log;
        G4LogicalVolume* trackerLayer_log;
        G4LogicalVolume* solenoidPart_log;
        G4VPhysicalVolume* experimentalHall_phys;
        G4VPhysicalVolume* trackerBlock_phys;
        G4VPhysicalVolume* trackerLayer_phys;
        G4VPhysicalVolume* solenoidPart_phys;

        G4UserLimits* fStepLimit;

        SDTest01MagneticField* fpMagField;
        SDTest01SimpleSolenoidPart* fpSolenoidPart;

        G4Element* H;
        G4Element* O;
        G4Element* C;
        G4Element* N;
        
        G4int StripN;
        G4double efficiency;
        G4bool addNoise;
};
#endif
