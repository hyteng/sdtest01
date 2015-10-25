/*=============================================================================
#     FileName: SDTest01ActionInitialization.h
#         Desc: user action header for multi thread scheme
#       Author: Haiyun Teng
#        Email: haiyun.teng@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2015-10-25 12:00:54
#      History: 
=============================================================================*/

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
