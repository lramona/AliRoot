//-*- Mode: C++ -*-
// $Id: $

//**************************************************************************
//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//*                                                                        *
//* Primary Authors: Jochen Thaeder <thaeder@kip.uni-heidelberg.de>        *
//*                  for The ALICE HLT Project.                            *
//*                                                                        *
//* Permission to use, copy, modify and distribute this software and its   *
//* documentation strictly for non-commercial purposes is hereby granted   *
//* without fee, provided that the above copyright notice appears in all   *
//* copies and that both the copyright notice and this permission notice   *
//* appear in the supporting documentation. The authors make no claims     *
//* about the suitability of this software for any purpose. It is          *
//* provided "as is" without express or implied warranty.                  *
//**************************************************************************

/** @file   AliHLTJETConeJetComponent.cxx
    @author Jochen Thaeder <thaeder@kip.uni-heidelberg.de>
    @date   
    @brief  Component to run the ConeJet jetfinder
*/

#if __GNUC__>= 3
using namespace std;
#endif

#include <cstdlib>
#include <cerrno>
#include <sys/time.h>

#include "AliHLTJETConeJetComponent.h" 

#include "TString.h"
#include "TObjString.h"

#include "AliAODJet.h"

/** ROOT macro for the implementation of ROOT specific class methods */
ClassImp(AliHLTJETConeJetComponent)

/*
 * ---------------------------------------------------------------------------------
 *                            Constructor / Destructor
 * ---------------------------------------------------------------------------------
 */

// #################################################################################
AliHLTJETConeJetComponent::AliHLTJETConeJetComponent() :
  fJetFinder(NULL),
  fJetHeader(NULL),
  fSeedCuts(NULL),
  fJetReader(NULL),
  fJetReaderHeader(NULL),
  fTrackCuts(NULL),
  fJetCuts(NULL),
  fJets(NULL) {
  // see header file for class documentation
  // or
  // refer to README to build package
  // or
  // visit http://web.ift.uib.no/~kjeks/doc/alice-hlt

}

// #################################################################################
AliHLTJETConeJetComponent::~AliHLTJETConeJetComponent() {
  // see header file for class documentation
}

/*
 * ---------------------------------------------------------------------------------
 * Public functions to implement AliHLTComponent's interface.
 * These functions are required for the registration process
 * ---------------------------------------------------------------------------------
 */

// #################################################################################
const Char_t* AliHLTJETConeJetComponent::GetComponentID() {
  // see header file for class documentation
  return "JETConeJetFinder";
}

// #################################################################################
void AliHLTJETConeJetComponent::GetInputDataTypes( vector<AliHLTComponentDataType>& list) {
  // see header file for class documentation
  list.clear(); 
  list.push_back( kAliHLTDataTypeMCObject|kAliHLTDataOriginHLT );
  list.push_back( kAliHLTDataTypeESDObject|kAliHLTDataOriginOffline );
  list.push_back( kAliHLTDataTypeESDObject|kAliHLTDataOriginHLT );
}

// #################################################################################
AliHLTComponentDataType AliHLTJETConeJetComponent::GetOutputDataType() {
  // see header file for class documentation
  return (kAliHLTDataTypeJet|kAliHLTDataOriginHLT);
}

// #################################################################################
void AliHLTJETConeJetComponent::GetOutputDataSize( ULong_t& constBase, Double_t& inputMultiplier ) {
  // see header file for class documentation

  constBase = 1000;
  inputMultiplier = 0.3;
}

// #################################################################################
AliHLTComponent* AliHLTJETConeJetComponent::Spawn() {
  // see header file for class documentation
  return new AliHLTJETConeJetComponent();
}

/*
 * ---------------------------------------------------------------------------------
 * Protected functions to implement AliHLTComponent's interface.
 * These functions provide initialization as well as the actual processing
 * capabilities of the component. 
 * ---------------------------------------------------------------------------------
 */

// #################################################################################
Int_t AliHLTJETConeJetComponent::DoInit( Int_t argc, const Char_t** argv ) {
  // see header file for class documentation

  if ( fJetFinder || fJetHeader || fJetReader || fJetReaderHeader || 
       fTrackCuts || fSeedCuts || fJetCuts || fJets )
    return -EINPROGRESS;

  // ---------------------------------------------------------------------
  // -- Defaults
  // ---------------------------------------------------------------------

  Float_t coneRadius    =  0.4;
  Float_t trackCutMinPt =  1.0;
  Float_t seedCutMinPt  =  5.0;
  Float_t jetCutMinEt   = 15.0;

  // ---------------------------------------------------------------------
  // -- Get Arguments
  // ---------------------------------------------------------------------

  TString comment;

  Int_t iResult = 0;
  Int_t bMissingParam=0;
  
  TString argument="";

  // -- Loop over all arguments
  for ( Int_t iter = 0; iter<argc && iResult>=0; iter++) {
    argument=argv[iter];

    if (argument.IsNull()) 
      continue;

    // -- coneRadius
    if ( !argument.CompareTo("-coneRadius") ) {
      if ((bMissingParam=(++iter>=argc))) break;
      
      TString parameter(argv[iter]);
      parameter.Remove(TString::kLeading, ' ');
      
      if ( parameter.IsFloat() ) {
	coneRadius = parameter.Atof();
	comment += argument;
	comment += " ";
	comment += parameter;
	comment += ' ';
      }
      else {
	HLTError("Wrong parameter %s for argument %s.", parameter.Data(), argument.Data());
	iResult=-EINVAL;
      }
    } 

    // -- trackCutMinPt
    else if ( !argument.CompareTo("-trackCutMinPt") ) {
      if ((bMissingParam=(++iter>=argc))) break;
      
      TString parameter(argv[iter]);
      parameter.Remove(TString::kLeading, ' ');
      
      if ( parameter.IsFloat() ) {
	trackCutMinPt = parameter.Atof();
	comment += argument;
	comment += " ";
	comment += parameter;
	comment += ' ';
      }
      else {
	HLTError("Wrong parameter %s for argument %s.", parameter.Data(), argument.Data());
	iResult=-EINVAL;
      }
    } 

    // -- seedCutMinPt
    else if ( !argument.CompareTo("-seedCutMinPt") ) {
      if ((bMissingParam=(++iter>=argc))) break;
      
      TString parameter(argv[iter]);
      parameter.Remove(TString::kLeading, ' ');
      
      if ( parameter.IsFloat() ) {
	seedCutMinPt = parameter.Atof();
	comment += argument;
	comment += " ";
	comment += parameter;
	comment += ' ';
      }
      else {
	HLTError("Wrong parameter %s for argument %s.", parameter.Data(), argument.Data());
	iResult=-EINVAL;
      }
    } 

    // -- jetCutMinEt
    else if ( !argument.CompareTo("-jetCutMinEt") ) {
      if ((bMissingParam=(++iter>=argc))) break;
      
      TString parameter(argv[iter]);
      parameter.Remove(TString::kLeading, ' ');
      
      if ( parameter.IsFloat() ) {
	jetCutMinEt = parameter.Atof();
	comment += argument;
	comment += " ";
	comment += parameter;
	comment += ' ';
      }
      else {
	HLTError("Wrong parameter %s for argument %s.", parameter.Data(), argument.Data());
	iResult=-EINVAL;
      }
    } 
    
    // -- Argument not known
    else {
      HLTError("Unknown argument %s.", argument.Data());
      iResult = -EINVAL;
    }
  } // for ( Int iter = 0; iter<argc && iResult>=0; iter++) {
  
  // -- Check if parameter is missing
  if ( bMissingParam ) {
    HLTError("Missing parameter for argument %s.", argument.Data());
    iResult=-EINVAL;
  }

  if (iResult)
    return iResult;


  // ---------------------------------------------------------------------
  // -- Jet Track Cuts
  // ---------------------------------------------------------------------
  if ( ! (fTrackCuts = new AliHLTJETTrackCuts()) ) {
    HLTError("Error instantiating track cuts");
    return -EINPROGRESS;
  }

  fTrackCuts->SetChargedOnly( kTRUE );
  fTrackCuts->SetMinPt( trackCutMinPt );

  // ---------------------------------------------------------------------
  // -- Jet Seed Cuts
  // ---------------------------------------------------------------------
  if ( ! (fSeedCuts = new AliHLTJETConeSeedCuts()) ) {
    HLTError("Error instantiating seed cuts");
    return -EINPROGRESS;
  }

  // Set pt cut
  fSeedCuts->SetMinPt( seedCutMinPt );

  // Set Eta min/max and Phi min/max
  fSeedCuts->SetEtaRange( (-0.9+coneRadius), (0.9-coneRadius) );
  fSeedCuts->SetPhiRange( 0.0, TMath::TwoPi() );

  // ---------------------------------------------------------------------
  // -- Jet Jet Cuts
  // ---------------------------------------------------------------------
  if ( ! (fJetCuts = new AliHLTJETJetCuts()) ) {
    HLTError("Error instantiating jet cuts");
    return -EINPROGRESS;
  }

  // Set pt cut
  fJetCuts->SetMinEt( jetCutMinEt );

  // ---------------------------------------------------------------------
  // -- Jet Reader Header
  // ---------------------------------------------------------------------
  if ( ! (fJetReaderHeader = new AliHLTJETReaderHeader()) ) {
    HLTError("Error instantiating jet reader header");
    return -EINPROGRESS;
  }

  // Set prt to track cuts
  fJetReaderHeader->SetTrackCuts( fTrackCuts );
  fJetReaderHeader->SetSeedCuts( fSeedCuts );

  // Set Eta min/max and Phi min/max
  fJetReaderHeader->SetFiducialEta( -0.9, 0.9) ;
  fJetReaderHeader->SetFiducialPhi(  0.0, TMath::TwoPi() ) ;

  // Set grid binning
  fJetReaderHeader->SetGridEtaBinning( 0.05 );
  fJetReaderHeader->SetGridPhiBinning( 0.05 );
 
  // Set cone radius
  fJetReaderHeader->SetConeRadius(coneRadius);

  // ---------------------------------------------------------------------
  // -- Jet Reader
  // ---------------------------------------------------------------------
  if ( ! (fJetReader = new AliHLTJETReader()) ) {
    HLTError("Error instantiating jet reader");
    return -EINPROGRESS;
  }

  // Set reader header
  fJetReader->SetReaderHeader(fJetReaderHeader);

  // ---------------------------------------------------------------------
  // -- Jet Container
  // ---------------------------------------------------------------------
  if ( ! (fJets = new AliHLTJets()) ) {
    HLTError("Error instantiating jet container");
    return -EINPROGRESS;
  }

  fJets->SetComment(comment);

  // ---------------------------------------------------------------------
  // -- Jet Header
  // ---------------------------------------------------------------------
  if ( ! (fJetHeader = new AliHLTJETConeHeader()) ) {
    HLTError("Error instantiating cone jet header");
    return -EINPROGRESS;
  }
  fJetHeader->SetJetCuts(fJetCuts);
  fJetHeader->SetUseLeading(kTRUE);

  // ---------------------------------------------------------------------
  // -- Jet Finder
  // ---------------------------------------------------------------------
  if ( ! (fJetFinder = new AliHLTJETConeFinder()) ) {
    HLTError("Error instantiating jet finder");
    return -EINPROGRESS;
  }

  fJetFinder->SetJetHeader(fJetHeader);
  fJetFinder->SetJetReader(fJetReader);
  fJetFinder->SetOutputJets(fJets);

  // ---------------------------------------------------------------------
  // -- Initialize Jet Finder
  // ---------------------------------------------------------------------
  if ( (fJetFinder->Initialize()) ) {
    HLTError("Error initializing cone jet finder");
    return -EINPROGRESS;
  }

  return 0;
}

// #################################################################################
Int_t AliHLTJETConeJetComponent::DoDeinit() {
  // see header file for class documentation

  if ( fJetFinder )
    delete fJetFinder;
  fJetFinder = NULL;

  if ( fJetHeader )
    delete fJetHeader;
  fJetHeader = NULL;

  if ( fJetReader )
    delete fJetReader;
  fJetReader = NULL;

  if ( fJetReaderHeader )
    delete fJetReaderHeader;
  fJetReaderHeader = NULL;

  if ( fTrackCuts )
    delete fTrackCuts;
  fTrackCuts = NULL;

  if ( fSeedCuts )
    delete fSeedCuts;
  fSeedCuts = NULL;

  if ( fJetCuts )
    delete fJetCuts;
  fJetCuts = NULL;

  if ( fJets )
    delete fJets;
  fJets = NULL;
  
  return 0;
}

// #################################################################################
Int_t AliHLTJETConeJetComponent::DoEvent( const AliHLTComponentEventData& /*evtData*/,
					  AliHLTComponentTriggerData& /*trigData*/ ) {
  // see header file for class documentation

  Int_t iResult = 0;

  const TObject* iter = NULL;

  // -- Start-Of-Run
  // -----------------
  if ( GetFirstInputObject(kAliHLTDataTypeSOR) && !iResult ) {
    HLTInfo("On-line SOR Event");
  }

  // -- ADD MC Object -- On-line
  // ------------------------------
  for ( iter=GetFirstInputObject(kAliHLTDataTypeMCObject|kAliHLTDataOriginHLT); 
	iter != NULL && !iResult; iter=GetNextInputObject() ) {

    // -- Reset 
    fJetFinder->Reset();
    
    // -- Set input event
    fJetReader->SetInputEvent( NULL, NULL, const_cast<TObject*>(iter) );    

    // -- Fill grid with MC
    if ( ! fJetReader->FillGridMC() ) {
      HLTError("Error filling grid.");
      iResult = -EINPROGRESS;
    }

    // -- Find jets
    if ( !iResult) {
      if ( ! fJetFinder->ProcessConeEvent() ) {
	HLTError("Error processing cone event.");
	iResult = -EINPROGRESS;
      }
    }
    
    // -- PushBack
    if ( !iResult) {
      PushBack(fJets, kAliHLTDataTypeJet|kAliHLTDataOriginHLT, GetSpecification());
    }
  }

  // -- ADD ESD Object -- Off-line
  // -------------------------------
  for ( iter=GetFirstInputObject(kAliHLTDataTypeESDObject|kAliHLTDataOriginOffline); 
	iter != NULL && !iResult; iter=GetNextInputObject() ) {

    // -- Reset 
    fJetFinder->Reset();

    // -- Set input event
    fJetReader->SetInputEvent( const_cast<TObject*>(iter), NULL, NULL );    
  
    // -- Fill grid with ESD
    if ( ! fJetReader->FillGridESD() ) {
      HLTError("Error filling grid.");
      iResult = -1;  
    }

    // -- Find jets
    if ( !iResult) {
      if ( ! fJetFinder->ProcessConeEvent() ) {
	HLTError("Error processing cone event.");
	iResult = -EINPROGRESS;
      }
    }

    // -- PushBack
    if ( !iResult) {
      PushBack(fJets, kAliHLTDataTypeJet|kAliHLTDataOriginHLT, GetSpecification());
    }
  }

  // -- ADD ESD Object -- On-line
  // ------------------------------
  for ( iter=GetFirstInputObject(kAliHLTDataTypeESDObject|kAliHLTDataOriginHLT); 
	iter != NULL && !iResult; iter=GetNextInputObject() ) {

    // -- Set input event
    fJetReader->SetInputEvent( const_cast<TObject*>(iter), NULL, NULL );    

    // -- Reset 
    fJetFinder->Reset();

    // -- Fill grid with ESD
    if ( ! fJetReader->FillGridESD() ) {
      HLTError("Error filling grid.");
      iResult = -1;  
    }

    // -- Find jets
    if ( !iResult) {
      if ( ! fJetFinder->ProcessConeEvent() ) {
	HLTError("Error processing cone event.");
	iResult = -EINPROGRESS;
      }
    }

    // -- PushBack
    if ( !iResult) {
      PushBack(fJets, kAliHLTDataTypeJet|kAliHLTDataOriginHLT, GetSpecification());
    }
  }

  return iResult;
}
