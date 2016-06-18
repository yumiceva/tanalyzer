
#ifndef PVHISTOS_H
#define PVHISTOS_H

#include "BaseHistos.h"
          //#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class PVHistos: public BaseHistos {

public:
  PVHistos( string prefix = "") {
    if ( prefix != "" ) fprefix = prefix+"_";
    else fprefix = prefix;

    Make();
  }
  ~PVHistos() {}

  void Fill(int imu = 0, std::map< int, float > uservar = std::map< int, float > () )
  {
    //cout << "fill "<< ftree->nVtx << endl;
    fhists[fprefix+"nPV"]->Fill( ftree->nVtx );
  }
  void Make()
  {
    Book(fprefix+"nPV",fprefix+"nPV",30,0,30,"Primary Vertices","Events");

  }

private:

};
#endif
