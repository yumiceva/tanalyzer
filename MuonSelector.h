#ifndef MUONSELECTOR_H
#define MUONSELECTOR_H

#include "BaseSelector.h"
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class MuonSelector: public BaseSelector {

public:
  MuonSelector() {}
  ~MuonSelector() {}

  bool PassLoose( int imu)
  {
    Pass(imu);
    return fIsLoose;
  }
 
  void Pass(int imu)
  {
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

    frelIsocorr = ( ftree->muPFChIso->at(imu) + fmax(0.0, ftree->muPFNeuIso->at(imu) + ftree->muPFPhoIso->at(imu) -0.5*ftree->muPFPUIso->at(imu) ) ) / pt;

    double rho_zero = std::max(0.0, (double) ftree->rho);
                
    bool IsoPass = frelIsocorr >= 0.0 && frelIsocorr <= 0.15;

    //if (mu_Iso_invert) IsoPass = !IsoPass;

    fIsLoose = 
      pt > 10.0 && 
      TMath::Abs(eta) < 2.4 && 
      frelIsocorr < 0.25 
      && isPFMuon && ( isGlobalMuon || isTrackerMuon);

    
  }


  float get_relIso() { return frelIso; }
  float get_relIsocorr() { return frelIsocorr; }

private:
  float frelIso;
  float frelIsocorr;
};
#endif
