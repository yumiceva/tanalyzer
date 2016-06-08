
#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "FileLoader.h"
#include "EventTree.h"


// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
  copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
  return os;
}


int main(int ac, char** av)
{

  vector<string> inputfiles;
  bool fverbose = false;

  // boost Parser ------------------------------------------------------------------------
  try {

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("sample,s", po::value<string>(), "set sample name")
      ("output-path,o", po::value<string>(), "set path to output directory")
      ("input-file,i", po::value< vector<string> >()->multitoken(), "set input file(s)")
      ("verbose,v", po::value<string>()->implicit_value(""), "enable verbosity")
      ;

    po::variables_map vm;        
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
      cout << "Usage: tanalyzer [options]\n";
      cout << desc << "\n";
      return 0;
    }

    if (vm.count("sample")) {
      cout << "== Sample name was set to: " 
           << vm["sample"].as<string>() << "\n";
    } else {
      cout << "== Sample name was not set.\n" << desc << "\n";
      return 1;
    }
    
    if (vm.count("output-path")) {
      cout << "== Output path was set to: "
           << vm["output-path"].as<string>() << "\n";
    } else {
      cout << "== Output path was not set.\n" << desc << "\n";
      return 1;
    }

    if (vm.count("input-file")) {
      cout << "== Input file(s) was set to: " << "\n";
      //<< vm["input-file"].as< vector<string> >() << "\n";

      inputfiles = vm["input-file"].as< vector<string> >();
      for ( vector<string>::iterator it = inputfiles.begin(); it != inputfiles.end(); it++ )
        {
          cout << "== " << *it << endl;
        }

    } else {
      cout << "== Input file(s) was not set.\n" << desc << "\n";
      return 1;
    }

    if (vm.count("verbose")) {
      cout << "== Verbosity enabled."
           << "\n";
      fverbose = true;
    }

  }
  catch(exception& e) {
    cerr << "== error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "== Exception of unknown type!\n";
  }
  // end Parser ------------------------------------------------------------------------
  

  FileLoader *fileloader = new FileLoader( inputfiles);    
  if (fverbose) cout << "== Files loaded." << endl;

  EventTree* tree = new EventTree( fileloader->getTree() );
  tree->GetEntry(0);

  Long64_t nentries = fileloader->getTree()->GetEntries();
  cout << "== Total number of entries: " << nentries << endl;

  for (Long64_t jentry=0; jentry < nentries; jentry++) {
      
    if ( jentry%500 == 0 || fverbose )
      cout << "== Processing entry: " << jentry << "  ==================="<<endl;

    Long64_t ientry = tree->LoadTree(jentry);
    if (ientry < 0) break;
    tree->GetEntry(jentry);

    // if (Cut(ientry) < 0) continue;
  }

    return 0;

}
