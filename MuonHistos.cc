//////////////////////////////////////////////////////////
//
//
// Francisco Yumiceva
// Florida Institute of Technology
/////////////////////////////////////////////////////////

#include "MuonHistos.h"
#include<iostream>
using namespace std;

void MuonHistos::Fill(int imu, std::map< int, float > uservar )
{
    
  fhists[fprefix+"Pt"]->Fill( ftree->muPt->at(imu) );
  fhists[fprefix+"Eta"]->Fill( ftree->muEta->at(imu) );
  fhists[fprefix+"Phi"]->Fill( ftree->muPhi->at(imu) );

  if ( ! uservar.empty() ) {
    string var = fprefix+"RelIso";
    if (uservar.count(imu) ==1 ) {
      //cout<< "found.\n"<< uservar[imu] <<endl; 
      fhists[var]->Fill( uservar[imu] );
    }
    else cout << "== ERROR in map, no variable name: " << var << endl;
  }

}

void MuonHistos::Make()
{
  Book(fprefix+"Pt",fprefix+"Pt",30,0,300,"Muon p_{T} [GeV]","Events / 10 GeV");
  Book(fprefix+"Eta",fprefix+"Eta",26,-2.6,2.6,"Muon #eta","Events / 0.2");
  Book(fprefix+"Phi",fprefix+"Phi",20,-3.15,3.15,"Muon #phi","Events /");
  Book(fprefix+"RelIso",fprefix+"RelIso",120,0,1.2,"Muon relative isolation","Events / 0.01");

}

