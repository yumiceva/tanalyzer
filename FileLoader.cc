//////////////////////////////////////////////////////////
//
//
// Francisco Yumiceva
// Florida Institute of Technology
/////////////////////////////////////////////////////////

#include "FileLoader.h"
#include<iostream>
using namespace std;

FileLoader::FileLoader( vector<string> filenames)
{

  chain_f = new TChain("ggNtuplizer/EventTree");

  for ( vector<string>::iterator it = filenames.begin(); it != filenames.end(); it++ ) {

  //for (int fileI=0; fileI < nFiles; fileI++) {
    //cout << sizeof(filenames[fileI]) <<endl;
    //cout << "adding " << filenames[fileI] << endl;
    
    chain_f->Add( (*it).c_str() );
    
  }

}

FileLoader::~FileLoader()
{
  delete chain_f;
}
