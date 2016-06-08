//////////////////////////////////////////////////////////
//
//
// Francisco Yumiceva
// Florida Institute of Technology
/////////////////////////////////////////////////////////

#ifndef FileLoader_h
#define FileLoader_h

#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>

class FileLoader {

public:

  TFile *tfile_f;
  TChain *chain_f;

  FileLoader( std::vector<std::string> filenames);
  ~FileLoader();

  TTree *getTree() { return chain_f; }

};

#endif

