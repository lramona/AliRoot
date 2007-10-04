/***************************************************************************
 *
 * $Id$ 
 *
 * 
 ***************************************************************************
 *
 * 
 *              
 *
 ***************************************************************************
 *
 * $Log$
 * Revision 1.3  2007/05/22 09:01:42  akisiel
 * Add the possibiloity to save cut settings in the ROOT file
 *
 * Revision 1.2  2007/05/21 10:38:25  akisiel
 * More coding rule conformance
 *
 * Revision 1.1  2007/05/16 10:25:06  akisiel
 * Making the directory structure of AliFemtoUser flat. All files go into one common directory
 *
 * Revision 1.4  2007/05/03 09:46:10  akisiel
 * Fixing Effective C++ warnings
 *
 * Revision 1.3  2007/04/27 07:25:59  akisiel
 * Make revisions needed for compilation from the main AliRoot tree
 *
 * Revision 1.1.1.1  2007/04/25 15:38:41  panos
 * Importing the HBT code dir
 *
 * Revision 1.4  2007-04-03 16:00:08  mchojnacki
 * Changes to iprove memory managing
 *
 * Revision 1.3  2007/03/13 15:30:03  mchojnacki
 * adding reader for simulated data
 *
 * Revision 1.2  2007/03/08 14:58:03  mchojnacki
 * adding some alice stuff
 *
 * Revision 1.1.1.1  2007/03/07 10:14:49  mchojnacki
 * First version on CVS
 *
 **************************************************************************/

#include "AliFemtoESDTrackCut.h"
#include <cstdio>

#ifdef __ROOT__ 
ClassImp(AliFemtoESDTrackCut)
#endif

AliFemtoESDTrackCut::AliFemtoESDTrackCut() :
    fCharge(0),
    fLabel(0),
    fStatus(0),
    fminTPCclsF(0),
    fminITScls(0),
    fNTracksPassed(0),
    fNTracksFailed(0),
    fRemoveKinks(kFALSE)
{
  // Default constructor
  fNTracksPassed = fNTracksFailed = 0;
  fCharge = 0;  // takes both charges 0
  fPt[0]=0.0;              fPt[1] = 100.0;//100
  fRapidity[0]=-2;       fRapidity[1]=2;//-2 2
  fPidProbElectron[0]=-1;fPidProbElectron[1]=2;
  fPidProbPion[0]=-1;    fPidProbPion[1]=2;
  fPidProbKaon[0]=-1;fPidProbKaon[1]=2;
  fPidProbProton[0]=-1;fPidProbProton[1]=2;
  fPidProbMuon[0]=-1;fPidProbMuon[1]=2;
  fLabel=false;
  fStatus=0;
  fminTPCclsF=0;
  fminITScls=0;
    
}
//------------------------------
//AliFemtoESDTrackCut::~AliFemtoESDTrackCut(){
//  /* noop */
//}
//------------------------------
bool AliFemtoESDTrackCut::Pass(const AliFemtoTrack* track)
{
  // test the particle and return 
  // true if it meets all the criteria
  // false if it doesn't meet at least one of the criteria
  
  //cout<<"AliFemtoESD  cut"<<endl;
  //cout<<fPidProbPion[0]<<" < pi ="<<track->PidProbPion()<<" <"<<fPidProbPion[1]<<endl;
  if (fStatus!=0)
    {
      //cout<<" status "<<track->Label()<<" "<<track->Flags()<<" "<<track->TPCnclsF()<<" "<<track->ITSncls()<<endl;
      if ((track->Flags()&fStatus)!=fStatus)
	{
	  //	  cout<<track->Flags()<<" "<<fStatus<<" no go through status"<<endl;
	  return false;
	}
	
    }
  if (fminTPCclsF>track->TPCnclsF())
    {
      //cout<<" No go because TPC Number of ClsF"<<fminTPCclsF<< " "<<track->TPCnclsF()<<endl;
      return false;
    }
  if (fminITScls>track->ITSncls())
    {
      //cout<<" No go because ITS Number of Cls"<<fminITScls<< " "<<track->ITSncls()<<endl;
      return false;
    }
	
  if (fLabel)
    {
      //cout<<"labels"<<endl;
      if(track->Label()<0)
	{
	  fNTracksFailed++;
	  //   cout<<"No Go Through the cut"<<endl;
	  //  cout<<fLabel<<" Label="<<track->Label()<<endl;
	  return false;
	}    
    }
  if (fCharge!=0)
    {              
      //cout<<"AliFemtoESD  cut ch "<<endl;
      //cout<<fCharge<<" Charge="<<track->Charge()<<endl;
      if (track->Charge()!= fCharge)	
	{
	  fNTracksFailed++;
	  //  cout<<"No Go Through the cut"<<endl;
	  // cout<<fCharge<<" Charge="<<track->Charge()<<endl;
	  return false;
	}
    }
  float tEnergy = ::sqrt(track->P().mag2()+fMass*fMass);
  float tRapidity = 0.5*::log((tEnergy+track->P().z())/(tEnergy-track->P().z()));
  float tPt = ::sqrt((track->P().x())*(track->P().x())+(track->P().y())*(track->P().y()));
  if ((tRapidity<fRapidity[0])||(tRapidity>fRapidity[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;   
      //cout<<fRapidity[0]<<" < Rapidity ="<<tRapidity<<" <"<<fRapidity[1]<<endl;
      return false;
    }
  if ((tPt<fPt[0])||(tPt>fPt[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPt[0]<<" < Pt ="<<Pt<<" <"<<fPt[1]<<endl;
      return false;
    }
  if ((track->PidProbElectron()<fPidProbElectron[0])||(track->PidProbElectron()>fPidProbElectron[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPidProbElectron[0]<<" < e ="<<track->PidProbElectron()<<" <"<<fPidProbElectron[1]<<endl;
      return false;
    }
  if ((track->PidProbPion()<fPidProbPion[0])||(track->PidProbPion()>fPidProbPion[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPidProbPion[0]<<" < pi ="<<track->PidProbPion()<<" <"<<fPidProbPion[1]<<endl;
      return false;
    }
  if ((track->PidProbKaon()<fPidProbKaon[0])||(track->PidProbKaon()>fPidProbKaon[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPidProbKaon[0]<<" < k ="<<track->PidProbKaon()<<" <"<<fPidProbKaon[1]<<endl;
      return false;
    }
  if ((track->PidProbProton()<fPidProbProton[0])||(track->PidProbProton()>fPidProbProton[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPidProbProton[0]<<" < p  ="<<track->PidProbProton()<<" <"<<fPidProbProton[1]<<endl;
      return false;
    }
  if ((track->PidProbMuon()<fPidProbMuon[0])||(track->PidProbMuon()>fPidProbMuon[1]))
    {
      fNTracksFailed++;
      //cout<<"No Go Through the cut"<<endl;
      //cout<<fPidProbMuon[0]<<" <  mi="<<track->PidProbMuon()<<" <"<<fPidProbMuon[1]<<endl;
      return false;
    }
  if (fRemoveKinks) {
    if ((track->KinkIndex(0)) || (track->KinkIndex(1)) || (track->KinkIndex(2)))
      return false;
  }
  
  // cout<<"Go Through the cut"<<endl;
  // cout<<fLabel<<" Label="<<track->Label()<<endl;
  // cout<<fCharge<<" Charge="<<track->Charge()<<endl;
  // cout<<fPt[0]<<" < Pt ="<<Pt<<" <"<<fPt[1]<<endl;
  //cout<<fRapidity[0]<<" < Rapidity ="<<tRapidity<<" <"<<fRapidity[1]<<endl;
  //cout<<fPidProbElectron[0]<<" <  e="<<track->PidProbElectron()<<" <"<<fPidProbElectron[1]<<endl;
  //cout<<fPidProbPion[0]<<" <  pi="<<track->PidProbPion()<<" <"<<fPidProbPion[1]<<endl;
  //cout<<fPidProbKaon[0]<<" <  k="<<track->PidProbKaon()<<" <"<<fPidProbKaon[1]<<endl;
  //cout<<fPidProbProton[0]<<" <  p="<<track->PidProbProton()<<" <"<<fPidProbProton[1]<<endl;
  //cout<<fPidProbMuon[0]<<" <  mi="<<track->PidProbMuon()<<" <"<<fPidProbMuon[1]<<endl;
  fNTracksPassed++ ;
  return true;
    
    
}
//------------------------------
AliFemtoString AliFemtoESDTrackCut::Report()
{
  string tStemp;
  char tCtemp[100];
  sprintf(tCtemp,"Particle mass:\t%E\n",this->Mass());
  tStemp=tCtemp;
  sprintf(tCtemp,"Particle charge:\t%d\n",fCharge);
  tStemp+=tCtemp;
  sprintf(tCtemp,"Particle pT:\t%E - %E\n",fPt[0],fPt[1]);
  tStemp+=tCtemp;
  sprintf(tCtemp,"Particle rapidity:\t%E - %E\n",fRapidity[0],fRapidity[1]);
  tStemp+=tCtemp;
  sprintf(tCtemp,"Number of tracks which passed:\t%ld  Number which failed:\t%ld\n",fNTracksPassed,fNTracksFailed);
  tStemp += tCtemp;
  AliFemtoString returnThis = tStemp;
  return returnThis;
}
TList *AliFemtoESDTrackCut::ListSettings()
{
  // return a list of settings in a writable form
  TList *tListSetttings = new TList();
  char buf[200];
  snprintf(buf, 200, "AliFemtoESDTrackCut.mass=%lf", this->Mass());
  tListSetttings->AddLast(new TObjString(buf));

  snprintf(buf, 200, "AliFemtoESDTrackCut.charge=%i", fCharge);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobpion.minimum=%lf", fPidProbPion[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobpion.maximum=%lf", fPidProbPion[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobkaon.minimum=%lf", fPidProbKaon[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobkaon.maximum=%lf", fPidProbKaon[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobproton.minimum=%lf", fPidProbProton[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobproton.maximum=%lf", fPidProbProton[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobelectron.minimum=%lf", fPidProbElectron[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobelectron.maximum=%lf", fPidProbElectron[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobMuon.minimum=%lf", fPidProbMuon[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pidprobMuon.maximum=%lf", fPidProbMuon[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.minimumtpcclusters=%i", fminTPCclsF);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.minimumitsclusters=%i", fminTPCclsF);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pt.minimum=%lf", fPt[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.pt.maximum=%lf", fPt[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.rapidity.minimum=%lf", fRapidity[0]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.rapidity.maximum=%lf", fRapidity[1]);
  tListSetttings->AddLast(new TObjString(buf));
  snprintf(buf, 200, "AliFemtoESDTrackCut.removekinks=%i", fRemoveKinks);
  tListSetttings->AddLast(new TObjString(buf));

  return tListSetttings;
}
void AliFemtoESDTrackCut::SetRemoveKinks(const bool& flag)
{
  fRemoveKinks = flag;
}
