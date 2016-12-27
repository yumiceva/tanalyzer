//////////////////////////////////////////////////////////
//
//
// Francisco Yumiceva
// Florida Institute of Technology
/////////////////////////////////////////////////////////

#include "JetHistos.h"
#include<iostream>
using namespace std;

void JetHistos::Fill(int imu, std::map< int, float > uservar )
{
    
  fhists[fprefix+"Pt"]->Fill( ftree->jetPt->at(imu) );
  fhists[fprefix+"Eta"]->Fill( ftree->jetEta->at(imu) );
  fhists[fprefix+"Phi"]->Fill( ftree->jetPhi->at(imu) );

  if ( ! uservar.empty() ) {
    string var = fprefix+"RelIso";
    if (uservar.count(imu) ==1 ) {
      //cout<< "found.\n"<< uservar[imu] <<endl; 
      fhists[var]->Fill( uservar[imu] );
    }
    else cout << "== ERROR in map, no variable name: " << var << endl;
  }

}

void JetHistos::Make()
{
  Book(fprefix+"Pt",fprefix+"Pt",30,0,300,"Jet p_{T} [GeV]","Events / 10 GeV");
  Book(fprefix+"Eta",fprefix+"Eta",26,-2.6,2.6,"Jet #eta","Events / 0.2");
  Book(fprefix+"Phi",fprefix+"Phi",20,-3.15,3.15,"Jet #phi","Events /");
  //Book(fprefix+"RelIso",fprefix+"RelIso",120,0,1.2,"Jet relative isolation","Events / 0.01");

}

