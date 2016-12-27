#ifndef BASESELECTOR_H
#define BASESELECTOR_H

#include"EventTree.h"

class BaseSelector {

public:
  BaseSelector () {}
  ~BaseSelector () {}

  void Init( EventTree *tree, bool verbose = false, bool IsMC = false)
  {
    ftree = tree;
    fVerbose = verbose;
    fIsMC = IsMC;
    fIsLoose = false;
    fIsTight = false;
  }
  void Pass(int i) {}
  bool PassLoose(int i) { Pass(i); return fIsLoose; }
  bool PassTight(int i) { Pass(i); return fIsTight; }
  
protected:
  EventTree *ftree;
  bool fVerbose;
  bool fIsMC;
  bool fIsLoose;
  bool fIsTight;

};
#endif
