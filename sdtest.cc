/*=============================================================================
FileName: sdtest.cc
Desc: scintillator dectector test simulation
Author: Haiyun.TENG
Email: haiyun.teng@gmail.com
Version: 0.0.1
=============================================================================*/ 

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.h"
#include "Randomize.hh"

#include "SDTest01DetectorConstruction.h"
#include "SDTest01PhysicsList.h"
//#include "QGSP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "SDTest01ActionInitialization.h"
//#include "SDTest01PrimaryGeneratorAction.h"
//#include "SDTest01RunAction.h"
//#include "SDTest01EventAction.h"
//#include "SDTest01SteppingAction.h"

#include "SDTest01SteppingVerbose.h"
//#include "G4GlobalFastSimulationManager.hh"

#include "G4GDMLParser.hh"
#include "G4VPhysicalVolume.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

std::ofstream outFile;

int main(int argc, char** argv) {

    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Run manager
    #ifdef G4MULTITHREADED
    G4MTRunManager * runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    // User Verbose output class
    G4VSteppingVerbose* verbosity = new SDTest01SteppingVerbose;
    G4VSteppingVerbose::SetInstance(verbosity);

    // UserInitialization class - mandatory
    G4VUserDetectorConstruction* theDetector = new SDTest01DetectorConstruction;
    runManager->SetUserInitialization(theDetector);
    
    //G4VUserPhysicsList* thePhysicsList = new QGSP_BERT();
    G4VUserPhysicsList* thePhysicsList = new SDTest01PhysicsList;
    thePhysicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(thePhysicsList);

    runManager->SetUserInitialization(new SDTest01ActionInitialization());

    theRunManager->Initialize();

    // paste to GDML file
    G4VPhysicalVolume* W = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();
    G4GDMLParser parser;
    parser.Write("output.gdml", W);


    #ifdef G4VIS_USE
    G4VisManager* theVisManager = new G4VisExecutive;
    theVisManager->Initialize();
    #endif

    // User Interface
    G4UImanager* theUIManager = G4UImanager::GetUIpointer();

    if(argc != 1) {
        G4String theCommand = "/control/execute ";
        G4String theFileName = argv[1];
        theUIManager->ApplyCommand(theCommand+theFileName);
    }
    else {
        #ifdef G4UI_USE
        G4UIExecutive *theUI = new G4UIExecutive(argc, argv);
            #ifdef G4VIS_USE
            theUIManager->ApplyCommand("/control/execute vis.mac");
            #else
            UImanager->ApplyCommand("/control/execute init.mac");
            #endif
        if(ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        theUI->SessionStart();
        delete theUI;
        #endif
    }

    G4int EventNumber = 10;
    theRunManager->BeamOn(EventNumber);

    #ifdef G4VIS_USE
    delete theVisManager;   
    #endif 

    //Job termination
    delete verbosity;
    delete runManager;

    return 0;
}

