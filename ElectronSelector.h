#ifndef ELECTRONSELECTOR_H
#define ELECTRONSELECTOR_H

#include "BaseSelector.h"
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class ElectronSelector: public BaseSelector {

public:
  ElectronSelector() {}
  ~ElectronSelector() {}


  double eleEffArea03(double SCEta) {
    double eta = TMath::Abs(SCEta);
    static const double area[7] = {0.130, 0.137, 0.067, 0.089, 0.107, 0.110, 0.138};
    int region = 0;
    if( eta >= 1.0 )   region++;
    if( eta >= 1.479 ) region++;
    if( eta >= 2.0 )   region++;
    if( eta >= 2.2 )   region++;
    if( eta >= 2.3 )   region++;
    if( eta >= 2.4 )   region++;
    return area[region];
  }

  void Pass(int icand)
  {
    if (fVerbose) cout << "== Electron selector called" <<endl;

    double pt = ftree->elePt->at(icand);
    double eta = ftree->eleSCEta->at(icand);

    double rho_zero = std::max(0.0, (double)ftree->rho);
    double relIsocorr = (ftree->elePFChIso->at(icand) + 
                         std::max(0.0, ftree->elePFNeuIso->at(icand) + 
                                  ftree->elePFPhoIso->at(icand) - 
                                  rho_zero * eleEffArea03(eta) )
                         ) / pt ;

    frelIsoM[icand] = relIsocorr;

    bool cutbasedID_veto = ( ftree->eleIDbit->at(icand) >> 0 & 1)  ;
    bool cutbasedID_loose = ( ftree->eleIDbit->at(icand) >> 1 & 1)  ;
    bool cutbasedID_tight = ( ftree->eleIDbit->at(icand) >> 3 & 1)  ;

    bool mvabasedID = ftree->eleIDMVATrg->at(icand) > 0;
    
    fIsLoose =
      pt > 15.0 &&
      TMath::Abs(eta) < 2.5 &&
      cutbasedID_veto;

    fIsTight =
      pt > 30.0 &&  
      TMath::Abs(eta) < 2.1 &&
      cutbasedID_tight;
  }

  std::map< int, float> GetRelIsoMap() { return frelIsoM; }

private:

  std::map< int, float> frelIsoM;

};
#endif
