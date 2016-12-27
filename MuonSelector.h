#ifndef MUONSELECTOR_H
#define MUONSELECTOR_H

#include "BaseSelector.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class MuonSelector: public BaseSelector {

public:
  MuonSelector() {}
  ~MuonSelector() {}

  bool PassLoose(int i) { Pass(i); return fIsLoose; }
  bool PassTight(int i) { Pass(i); return fIsTight; }

  void Pass(int imu)
  {
    if (fVerbose) cout << "== Muon selector called" <<endl;

    // get pt and eta
    double pt = ftree->muPt->at(imu);
    double eta = ftree->muEta->at(imu);

    // check muon type
    static const unsigned int GlobalMuon     =  1<<1;
    static const unsigned int TrackerMuon    =  1<<2;
    static const unsigned int PFMuon =  1<<5;
    bool isGlobalMuon  = ftree->muType->at(imu) & GlobalMuon;
    bool isTrackerMuon = ftree->muType->at(imu) & TrackerMuon;
    bool isPFMuon      = ftree->muType->at(imu) & PFMuon;

    double relIsocorr = ( ftree->muPFChIso->at(imu) + fmax(0.0, ftree->muPFNeuIso->at(imu) + ftree->muPFPhoIso->at(imu) -0.5*ftree->muPFPUIso->at(imu) ) ) / pt;

    frelIsoM[imu] = relIsocorr;

    //cout << "fill map" << endl;
    //if ( fuservar.count("muRelIsoCC") == 0 ) { cout<< "create map entry" << endl; fuservar["muRelIsoCC"] = std::vector<float>();}
    //fuservar.insert ( std::pair< std::string, vector>('a',100) );
    //fuservar["muRelIso"].push_back(frelIsocorr);

    double rho_zero = std::max(0.0, (double) ftree->rho);
                
    fIsLoose = 
      pt > 10.0 && 
      TMath::Abs(eta) < 2.4 && 
      relIsocorr < 0.25 &&
      isPFMuon && ( isGlobalMuon || isTrackerMuon);

    fIsTight =
      pt > 26.0 && 
      TMath::Abs(eta) < 2.1 && 
      relIsocorr < 0.15 &&
      ftree->muChi2NDF->at(imu) < 10 &&
      ftree->muTrkLayers->at(imu) > 5 &&
      ftree->muMuonHits->at(imu) > 0 &&
      ftree->muD0->at(imu) < 0.2 &&
      fabs( ftree->muDz->at(imu) ) < 0.5 && //check this
      ftree->muPixelHits->at(imu) > 0 &&
      ftree->muStations->at(imu) > 1 &&
      isPFMuon && isGlobalMuon && isTrackerMuon;
    
  }

  std::map< int, float> GetRelIsoMap() { return frelIsoM; }

private:
  std::map< int, float> frelIsoM;

};
#endif
