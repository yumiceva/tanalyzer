#ifndef JETSELECTOR_H
#define JETSELECTOR_H

#include "BaseSelector.h"
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class JetSelector: public BaseSelector {

public:
  JetSelector() {}
  ~JetSelector() {}

  bool PassLoose(int i) { Pass(i); return fIsLoose; }
  bool PassTight(int i) { Pass(i); return fIsTight; }

  void Pass(int icand)
  {
    if (fVerbose) cout << "== Jet selector Pass() called" <<endl;

    bool jetID_pass = false;
    
    if ( ftree->jetPt->size() == ftree->jetPFLooseId->size() ) {
      jetID_pass = ( ftree->jetPFLooseId->at(icand) == 1) ; 
    }

    fIsLoose = 
      TMath::Abs(ftree->jetEta->at(icand)) < 2.4 &&
      ftree->jetPt->at(icand) > 30.0 &&
      jetID_pass ;
  
  }

private:
  //std::map< int, float> frelIsoM;

};
#endif
