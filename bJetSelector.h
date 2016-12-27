#ifndef BJETSELECTOR_H
#define BJETSELECTOR_H

#include "BaseSelector.h"
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class bJetSelector: public BaseSelector {

public:
  bJetSelector() {}
  ~bJetSelector() {}

  bool PassLoose(int i) { Pass(i); return fIsLoose; }
  bool PassTight(int i) { Pass(i); return fIsTight; }

  void Pass(int icand)
  {
    if (fVerbose) cout << "== b Jet selector Pass() called" <<endl;
    
    fIsLoose = ftree->jetpfCombinedInclusiveSecondaryVertexV2BJetTags->at(icand) > 0.89;

  }

private:
  //std::map< int, float> frelIsoM;

};
#endif
