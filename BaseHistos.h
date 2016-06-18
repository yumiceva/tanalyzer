#ifndef BASEHISTOS_H
#define BASEHISTOS_H

#include<map>
#include"TH1F.h"
#include"TFile.h"
#include"EventTree.h"

class BaseHistos {

public:
  BaseHistos () {}
  ~BaseHistos () {}

  void Init( EventTree *tree)
  {
    ftree = tree;
    fVerbose = false;
    fIsMC = false;
  }
  void SetVerbose( bool verbose = true ) { fVerbose = verbose; }
  void SetIsMC( bool IsMC = true ) { fIsMC = IsMC; }
  void Fill(int imu, std::map< std::string, std::vector<float> > uservar = std::map< std::string, std::vector<float> >() ) {}
  void Write(TFile *tfile) {
   
    tfile->cd();

    for ( std::map< std::string, TH1F*>::iterator it= fhists.begin(); it != fhists.end(); ++it)
      {
        it->second->Write();
        it->second->SetDirectory(0);
      }
  }
  void Make();
  void Book(string hname, string htitle, int nbins, double xlow, double xhigh, const char* xlabel, const char* ylabel) {

    TH1F* h = new TH1F(hname.c_str(), htitle.c_str(), nbins, xlow, xhigh);
    h->GetXaxis()->SetTitle(xlabel);
    h->GetYaxis()->SetTitle(ylabel);
    h->SetDirectory(0);
    h->Sumw2();
    fhists[hname] = h;
  }

protected:

  std::map< std::string, TH1F* > fhists;
  EventTree *ftree;
  bool fVerbose;
  bool fIsMC;
  std::string fprefix;

};
#endif
