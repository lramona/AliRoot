#ifndef ALIPHOSGEOMETRY_H
#define ALIPHOSGEOMETRY_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

////////////////////////////////////////////////
//  Geometry class  for PHOS : singleton      //
//  Version SUBATECH                          //
//  Author  Y. Schutz SUBATECH                //
//       geometry parametrized for any        //  
//       shape of modules                     //
////////////////////////////////////////////////

// --- ROOT system ---

#include "TNamed.h"
#include "TString.h"
#include "TObjArray.h"
#include "TVector3.h" 

// --- AliRoot header files ---

#include "AliGeometry.h"
#include "AliPHOSRecPoint.h"

static const TString kDegre("deg") ; 
static const TString kRadian("rad") ; 

class AliPHOSGeometry : public AliGeometry {

public: 

  AliPHOSGeometry() {} ;  // must be kept public for root persistency purposes
  virtual ~AliPHOSGeometry(void) ; 
  static AliPHOSGeometry * GetInstance(const Text_t* name, const Text_t* title) ; 
  static AliPHOSGeometry * GetInstance() ; 
  virtual void  GetGlobal(const AliRecPoint* RecPoint, TVector3 & gpos, TMatrix & gmat)  ;
  virtual void  GetGlobal(const AliRecPoint* RecPoint, TVector3 & gpos)  ; 

protected:

  AliPHOSGeometry(const Text_t* name, const Text_t* title) : AliGeometry(name, title) { Init() ; }  
  void Init(void) ;            // steering method for PHOS and CPV
  void InitPHOS(void) ;        // defines the various PHOS geometry parameters
  void InitPPSD(void) ;        // defines the various PPSD geometry parameters

public: 

  // General

  Bool_t AbsToRelNumbering(const Int_t AbsId, Int_t * RelId) ;           // converts the absolute PHOS numbering to a relative 
  void EmcModuleCoverage(const Int_t m, Double_t & tm, Double_t & tM, Double_t & pm, Double_t & pM, Option_t * opt = kRadian);    
                                                                         // calculates the angular coverage in theta and phi of a EMC module
  void EmcXtalCoverage(Double_t & theta, Double_t & phi, Option_t * opt = kRadian) ; 
                                                                         // calculates the angular coverage in theta and phi of a 
                                                                         // single crystal in a EMC module
  void ImpactOnEmc(const Double_t theta, const Double_t phi, Int_t & ModuleNumber, Double_t & x, Double_t & z) ; 
                                                                         // calculates the impact coordinates of a neutral particle  
                                                                         // emitted in direction theta and phi in ALICE
  void   RelPosInModule(const Int_t * RelId, Float_t & y, Float_t & z) ; // gets the position of element (pad or Xtal) relative to 
                                                                         // center of PHOS module  
  void   RelPosInAlice(const Int_t AbsId, TVector3 &  pos) ;             // gets the position of element (pad or Xtal) relative to 
                                                                         // Alice
  Bool_t RelToAbsNumbering(const Int_t * RelId, Int_t & AbsId) ;         // converts the absolute PHOS numbering to a relative 
                                                                         // inlines

  ///////////// PHOS related parameters

  Bool_t     IsInitialized(void)                  const { return fInit ; }  
  Float_t    GetAirFilledBoxSize(Int_t index)     const { return fAirFilledBoxSize[index] ;}
  Float_t    GetCrystalHolderThickness(void)      const { return fCrystalHolderThickness ; } 
  Float_t    GetCrystalSize(Int_t index)          const { return fXtlSize[index] ; }
  Float_t    GetCrystalSupportHeight(void)        const { return fCrystalSupportHeight ; } 
  Float_t    GetCrystalWrapThickness(void)        const { return fCrystalWrapThickness;}
  Float_t    GetGapBetweenCrystals(void)          const { return fGapBetweenCrystals ; }
  Float_t    GetIPtoCrystalSurface(void)          const { return fIPtoCrystalSurface ; }
  Float_t    GetIPtoOuterCoverDistance(void)      const { return fIPtoOuterCoverDistance ; }
  Float_t    GetIPtoTopLidDistance(void)          const { return fIPtoTopLidDistance ; }
  Float_t    GetLowerThermoPlateThickness(void)   const { return fLowerThermoPlateThickness ; }
  Float_t    GetLowerTextolitPlateThickness(void) const { return fLowerTextolitPlateThickness ; }
  Float_t    GetModuleBoxThickness(void)          const { return fModuleBoxThickness ; }
  Int_t      GetNPhi(void)                        const { return fNPhi ; }
  Int_t      GetNZ(void)                          const { return fNZ ; }
  Int_t      GetNModules(void)                    const { return fNModules ; }
  Float_t    GetOuterBoxSize(Int_t index)         const { return fOuterBoxSize[index] ;    }
  Float_t    GetOuterBoxThickness(Int_t index)    const { return fOuterBoxThickness[index] ; } 
  Float_t    GetPHOSAngle(Int_t index)            const { return fPHOSAngle[index-1] ; } 
  Float_t    GetPinDiodeSize(Int_t index)         const { return fPinDiodeSize[index] ; }
  Float_t    GetSecondUpperPlateThickness(void)   const { return fSecondUpperPlateThickness ; }
  Float_t    GetSupportPlateThickness(void)       const { return fSupportPlateThickness ; }    
  Float_t    GetTextolitBoxSize(Int_t index)      const { return fTextolitBoxSize[index] ; }
  Float_t    GetTextolitBoxThickness(Int_t index) const { return fTextolitBoxThickness[index]; } 
  Float_t    GetUpperPlateThickness(void)         const { return fUpperPlateThickness ; }
  Float_t    GetUpperCoolingPlateThickness(void)  const { return fUpperCoolingPlateThickness ; }

private:

  void       SetPHOSAngles() ; // calculates the PHOS modules PHI angle

public: 
 
  ///////////// PPSD (PHOS PRE SHOWER DETECTOR)  related parameters


  Float_t GetAnodeThickness(void)          const { return fAnodeThickness ; } 
  Float_t GetAvalancheGap(void)            const { return fAvalancheGap ; }
  Float_t GetCathodeThickness(void)        const { return fCathodeThickness ; } 
  Float_t GetCompositeThickness(void)      const { return fCompositeThickness ; } 
  Float_t GetConversionGap(void)           const { return fConversionGap ; } 
  Float_t GetLeadConverterThickness(void)  const { return fLeadConverterThickness ; }
  Float_t GetLeadToMicro2Gap(void)         const { return fLeadToMicro2Gap ; }       
  Float_t GetLidThickness(void)            const { return fLidThickness ; }
  Float_t GetMicromegas1Thickness(void)    const { return fMicromegas1Thickness ; } 
  Float_t GetMicromegas2Thickness(void)    const { return fMicromegas2Thickness ; } 
  Float_t GetMicromegasWallThickness(void) const { return fMicromegasWallThickness ; } 
  Float_t GetMicro1ToLeadGap(void)         const { return fMicro1ToLeadGap ; } 
  Int_t   GetNumberOfPadsPhi(void)         const { return fNumberOfPadsPhi ; }
  Int_t   GetNumberOfPadsZ(void)           const { return fNumberOfPadsZ ; }
  Int_t   GetNumberOfModulesPhi(void)      const { return fNumberOfModulesPhi ; }          
  Int_t   GetNumberOfModulesZ(void)        const { return fNumberOfModulesZ ; }               
  Float_t GetPCThickness(void)             const { return fPCThickness ; }   
  Float_t GetPhiDisplacement(void)         const { return fPhiDisplacement ; }                           
  Float_t GetPPSDBoxSize(Int_t index)      const { return fPPSDBoxSize[index] ; }
  Float_t GetPPSDModuleSize(Int_t index)   const { return fPPSDModuleSize[index] ; } 
  Float_t GetZDisplacement(void)           const { return fZDisplacement ; }                           
 
  void SetLeadConverterThickness(Float_t e) ; // should ultimately disappear 

private:
  
  ///////////// PHOS related parameters

  Float_t fAirFilledBoxSize[3] ;          // Air filled box containing one module
  Float_t fAirThickness[3] ;              // Space filled with air between the module box and the Textolit box
  Float_t fCrystalSupportHeight ;         // Height of the support of the crystal    
  Float_t fCrystalWrapThickness ;         // Thickness of Tyvek wrapping the crystal
  Float_t fCrystalHolderThickness ;       // Titanium holder of the crystal
  Float_t fGapBetweenCrystals ;           // Total Gap between two adjacent crystals 
  Bool_t  fInit ;                         // Tells if geometry has been succesfully set up 
  Float_t fIPtoOuterCoverDistance ;       // Distances from interaction point to outer cover 
  Float_t fIPtoCrystalSurface ;           // Distances from interaction point to Xtal surface
  Float_t fModuleBoxThickness ;           // Thickness of the thermo insulating box containing one crystals module 
  Float_t fLowerTextolitPlateThickness ;  // Thickness of lower textolit plate
  Float_t fLowerThermoPlateThickness ;    // Thickness of lower thermo insulating plate
  Int_t   fNModules ;                     // Number of modules constituing PHOS
  Int_t   fNPhi ;                         // Number of crystal units in X (phi) direction
  Int_t   fNZ ;                           // Number of crystal units in Z direction
  Float_t fOuterBoxSize[3] ;              // Size of the outer  thermo insulating foam box
  Float_t fOuterBoxThickness[3] ;         // Thickness of the outer thermo insulating foam box
  Float_t fPHOSAngle[4] ;                 // Position angles of modules
  Float_t fPinDiodeSize[3] ;              // Size of the PIN Diode 
  TObjArray *  fRotMatrixArray ;          // Liste of rotation matrices (one per phos module)
  Float_t fSecondUpperPlateThickness ;    // Thickness of  upper polystyrene foam plate
  Float_t fSupportPlateThickness ;        // Thickness of the Aluminium support plate  
  Float_t fUpperCoolingPlateThickness ;   // Thickness of the upper cooling plate 
  Float_t fUpperPlateThickness ;          // Thickness of the uper thermo insulating foam plate 
  Float_t fTextolitBoxSize[3] ;           // Size of the Textolit box inside the insulating foam box
  Float_t fTextolitBoxThickness[3] ;      // Thicknesses of th Textolit box
  Float_t fXtlSize[3] ;                   // PWO4 crystal dimensions


  ///////////// PPSD (PHOS PRE SHOWER DETECTOR)  related parameters

  Float_t fAnodeThickness ;               // Thickness of the copper layer which makes the anode 
  Float_t fAvalancheGap ;                 // Thickness of the gas in the avalanche stage
  Float_t fCathodeThickness ;             // Thickeness of composite material ensuring rigidity of cathode
  Float_t fCompositeThickness ;           // Thickeness of composite material ensuring rigidity of anode
  Float_t fConversionGap ;                // Thickness of the gas in the conversion stage
  Float_t fIPtoTopLidDistance ;           // Distance from interaction point to top lid of PPSD
  Float_t fLeadConverterThickness ;       // Thickness of the Lead converter 
  Float_t fLeadToMicro2Gap ;              // Thickness of the air gap between the Lead and Micromegas 2        
  Float_t fLidThickness ;                 // Thickness of top lid 
  Float_t fMicromegas1Thickness ;         // Thickness of the first downstream Micromegas 
  Float_t fMicromegas2Thickness ;         // Thickness of the second downstream Micromegas 
  Float_t fMicromegasWallThickness ;      // Thickness of the Micromegas leak tight box
  Float_t fMicro1ToLeadGap ;              // Thickness of the air gap between Micromegas 1 and the Lead
  Int_t   fNumberOfPadsPhi ;              // Number of pads on a micromegas module ;  
  Int_t   fNumberOfPadsZ ;                // Number of pads on a micromegas module ;  
  Int_t   fNumberOfModulesPhi ;           // Number of micromegas modules in phi
  Int_t   fNumberOfModulesZ ;             // Number of micromegas modules in z
  Float_t fPCThickness ;                  // Thickness of the printed circuit board of the anode   
  Float_t fPhiDisplacement ;              // Phi displacement of micromegas1 with respect to micromegas2  
  Float_t fPPSDBoxSize[3] ;               // Size of large box which contains PPSD; matches PHOS module size
  Float_t fPPSDModuleSize[3] ;            // Size of an individual micromegas module
  Float_t fZDisplacement ;                // Z displacement of micromegas1 with respect to micromegas2  

  static AliPHOSGeometry * fGeom ; // pointer to the unique instance of the singleton 

  ClassDef(AliPHOSGeometry,1)  // PHOS geometry class , version subatech

} ;

#endif // AliPHOSGEOMETRY_H
