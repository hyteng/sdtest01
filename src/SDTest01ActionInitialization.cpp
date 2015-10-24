#include "SDTest01ActionInitialization.h"

#include "SDTest01PrimaryGeneratorAction.h"
#include "SDTest01RunAction.h"
#include "SDTest01EventAction.h"
#include "SDTest01TrackingAction.h"
#include "SDTest01SteppingAction.h"

#include "G4GlobalFastSimulationManager.hh"

SDTest01ActionInitialization::SDTest01ActionInitialization() : G4VUserActionInitialization() {
}

SDTest01ActionInitialization::~SDTest01ActionInitialization() {    
}

void SDTest01ActionInitialization::BuildForMaster() {
    SetUserAction(new SDTest01RunAction);
}

void SDTest01ActionInitialization::Build() {
    SetUserAction(new SDTest01PrimaryGeneratorAction);
    SetUserAction(new SDTest01RunAction);
    SetUserAction(new SDTest01EventAction);
    SetUserAction(new SDTest01TrackingAction);
    SetUserAction(new SDTest01SteppingAction);
}
