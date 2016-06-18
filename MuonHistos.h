
#ifndef MUONHISTOS_H
#define MUONHISTOS_H

#include "BaseHistos.h"
          //#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class MuonHistos: public BaseHistos {

public:
  MuonHistos( string prefix = "") {
    if ( prefix != "" ) fprefix = prefix+"_";
    else fprefix = prefix;

    Make();
  }
  ~MuonHistos() {}

  void Fill(int imu, std::map< int, float > uservar = std::map< int, float > () );
  void Make();

private:

};
#endif
