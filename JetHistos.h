
#ifndef JETHISTOS_H
#define JETHISTOS_H

#include "BaseHistos.h"
          //#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class JetHistos: public BaseHistos {

public:
  JetHistos( string prefix = "") {
    if ( prefix != "" ) fprefix = prefix+"_";
    else fprefix = prefix;

    Make();
  }
  ~JetHistos() {}

  void Fill(int imu, std::map< int, float > uservar = std::map< int, float > () );
  void Make();

private:

};
#endif
