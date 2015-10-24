#ifndef SDTest01ActionInitialization_h
#define SDTest01ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class SDTest01ActionInitialization :public G4VUserActionInitialization {
    public:
        SDTest01ActionInitialization();
        ~SDTest01ActionInitialization();
    
        virtual void BuildForMaster() const;
        virtual void Build() const;

    private:
        /* data */
};
#endif
