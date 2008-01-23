#ifndef ALIPLANEEFF_H
#define ALIPLANEEFF_H
/* Copyright(c) 2007-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#include <TObject.h>
#include <TString.h>
#include "AliLog.h"

//class Alisegmentation;
//class Aligeom;

////////////////////////////////////////////////////
//                                                //
// Virtual base class for Plane Efficiency        //
// Origin: Giuseppe.Bruno@ba.infn.it              //
//                                                //
////////////////////////////////////////////////////

class AliPlaneEff : public TObject {
 public:
 
    AliPlaneEff();// Default constructor
    virtual ~AliPlaneEff(){;}; 
    // copy constructor. See detector specific implementation.
    AliPlaneEff(const AliPlaneEff &source);
    // Assignment operator. See detector specific implementation.
    AliPlaneEff& operator=(const AliPlaneEff &source);
    // Average Plane efficiency (including dead/noisy)
    //Int_t   GetRunNumber() const {return fRunNumber;}
    //void    SetRunNumber(Int_t n) {fRunNumber=n;}
    //
    // Write into the data base 
    virtual Bool_t WriteIntoCDB() const
       {AliError("This method must be implemented in a derived class"); return kFALSE;};
    virtual Bool_t ReadFromCDB()
       {AliError("This method must be implemented in a derived class"); return kFALSE;};
    virtual Bool_t AddFromCDB()
       {AliError("This method must be implemented in a derived class"); return kFALSE;};

 protected:

    virtual void InitCDB(){;};
    virtual void Copy(TObject &obj) const;
    //Int_t	fRunNumber;	//! run number (to access CDB)
    //TString	fCDBUri;	//! Uri of the default CDB storage
    //Bool_t	fInitCDBCalled;	//! flag to check if CDB storages are already initialized
   
 private:
    //Int_t	fRunNumber;	// run number (to access CDB)

    ClassDef(AliPlaneEff,1) // Plane Efficiency virtual base class 
};
#endif
