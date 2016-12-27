//////////////////////////////////////////////////////////
//
//
// Francisco Yumiceva
// Florida Institute of Technology
/////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iterator>
using namespace std;

#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace boost::posix_time;
using namespace boost::gregorian;
using boost::format;

#include "FileLoader.h"
#include "EventTree.h"
#include "MuonSelector.h"
#include "ElectronSelector.h"
#include "JetSelector.h"
#include "bJetSelector.h"
#include "BaseHistos.h"
#include "MuonHistos.h"
#include "PVHistos.h"
#include "JetHistos.h"

#include "TLorentzVector.h"

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
  string outpath;
  string sample;
  bool fverbose = false;
  int fMaxEvents = -1; // default run over all events

  // Time Stamp --------------------------------------------------------------------------
  //get the current time from the clock -- one second resolution
  ptime now = second_clock::local_time();
  //Get the date part out of the time
  date today = now.date();
  cout << "== Begin at " << to_simple_string( now ) << endl;
  // end Time Stamp ----------------------------------------------------------------------

  // boost Parser ------------------------------------------------------------------------
  try {

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("sample,s", po::value<string>(), "set sample name.")
      ("output-path,o", po::value<string>(), "set path to output directory.")
      ("input-file,i", po::value< vector<string> >()->multitoken(), "set input file(s).")
      ("max-events,m", po::value<int>(), "set a maximum number of events to be run.")
      ("verbose,v", po::value<string>()->implicit_value(""), "enable verbosity.")
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
      sample = vm["sample"].as<string>();
      cout << "== Sample name was set to: " 
           << sample << "\n";
    } else {
      cout << "== Sample name was not set.\n" << desc << "\n";
      return 1;
    }
    
    if (vm.count("output-path")) {
      outpath = vm["output-path"].as<string>();
      cout << "== Output path was set to: "
           << outpath << "\n";
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

    if (vm.count("max-events")) {
      fMaxEvents = vm["max-events"].as< int >();
      cout << "== Run over maximum "
           << fMaxEvents << " events.\n";
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

  // Load files into TChain
  FileLoader *fileloader = new FileLoader( inputfiles);    
  if (fverbose) cout << "== Files loaded." << endl;

  // Setup tree
  EventTree* tree = new EventTree( fileloader->getTree() );
  tree->GetEntry(0);

  Long64_t nentries = fileloader->getTree()->GetEntries();
  cout << "== Total number of entries: " << nentries << endl;

  // Object Selectors ------------------------------------------------------------------
  MuonSelector muSelector = MuonSelector();
  muSelector.Init( tree , fverbose );
  ElectronSelector eSelector = ElectronSelector();
  eSelector.Init( tree, fverbose );
  JetSelector jetSelector = JetSelector();
  jetSelector.Init( tree, fverbose );
  bJetSelector bjetSelector = bJetSelector();
  bjetSelector.Init( tree, fverbose );

  PVHistos pvHistos = PVHistos();
  pvHistos.Init( tree );
  MuonHistos mu1Histos = MuonHistos("mu1"); // leding muon
  mu1Histos.Init( tree );

  // Counters --------------------------------------------------------------------------
  map< string, float > counter;
  vector<string> counterlabels = {"start",
                                  "HLT",
                                  "PV",
                                  "TightMuon",
                                  "vetoLooseMuon",
                                  "vetoLooseElectron",
                                  "3jets",
                                  "1btag"
  };

  for ( vector<string>::iterator ii = counterlabels.begin(); ii != counterlabels.end(); ++ii) {
    counter[*ii] = 0.;
  }


  // Output file -----------------------------------------------------------------------
  string outfilename = outpath +"/hist_"+sample+".root";
  TFile *fFile = TFile::Open( outfilename.c_str(), "RECREATE");

  // Loop over entries -----------------------------------------------------------------
  for (Long64_t jentry=0; jentry < nentries; jentry++) {
    
    if ( fMaxEvents == jentry ) {cout << "== Reached maximum number of events set to " << jentry << endl; break; }
    if ( jentry%500 == 0 || fverbose )
      cout << "== Processing entry: " << jentry << "  ==================="<<endl;

    Long64_t ientry = tree->LoadTree(jentry);
    if (ientry < 0) break;
    tree->GetEntry(jentry);
    
    counter["start"]++;

    // HLT -----------------------------------------------------------------------------
    if (fverbose) cout << "== Check HLT.\n";
    bool No_trigger = false;
    if ( (tree->HLTEleMuX  >> 31 & 1 ) || (tree->HLTEleMuX  >> 32 & 1 ) ) counter["HLT"]++;
    else continue;

    // Primary Vertices ----------------------------------------------------------------
    if (fverbose) cout << "== Check good PV.\n";
    if ( tree->hasGoodVtx ) counter["PV"]++; //change to isPVGood 
    else continue;
    
    pvHistos.Fill();

    // Loop over MUONS -----------------------------------------------------------------
    if (fverbose) cout << "== Loop over muons." << endl;
    int NLooseMuons = 0;
    int NTightMuons = 0;
    int the_imu = -1;
    for ( int imu = 0; imu < tree->nMu; ++imu)
      {
        if ( muSelector.PassLoose(imu) ) NLooseMuons++;
        if ( muSelector.PassTight(imu) ) { 
          if (the_imu = -1 ) the_imu = imu; 
          NTightMuons++; 
        }
      }
    //if (fverbose) cout<< "== NTightMuons = " << NTightMuons << endl;

    if ( NTightMuons == 1 ) counter["TightMuon"]++;
    else continue;
    
    if ( NLooseMuons > 1 ) continue;
    else counter["vetoLooseMuon"]++;
    

    // Loop over ELECTRONS -----------------------------------------------------------------
    if (fverbose) cout << "== Loop over electrons." << endl;
    int NLooseEle = 0;
    int NTightEle = 0;
    int the_iele = -1;
    for ( int iele = 0; iele < tree->nEle; ++iele)
      {
        if ( eSelector.PassLoose(iele) ) NLooseEle++;
      }

    if ( NLooseEle > 0 ) continue;
    else counter["vetoLooseElectron"]++;
    

    mu1Histos.Fill(the_imu, muSelector.GetRelIsoMap() );
    

    // Loop over JETS ------------------------------------------------------------------
    if (fverbose) cout << "== Loop over jets." << endl;
    int NgoodJets = 0;
    int Nloose_bjets = 0;
    vector<TLorentzVector> jet4;

    for ( int ijet = 0; ijet < tree->nJet; ++ijet)
      {
        if ( jetSelector.PassLoose(ijet) ) {
          NgoodJets++;
          TLorentzVector tlv;
          tlv.SetPtEtaPhiE( tree->jetPt->at(ijet), tree->jetEta->at(ijet), tree->jetPhi->at(ijet), tree->jetEn->at(ijet) );
          jet4.push_back( tlv );
          
        }
        if ( bjetSelector.PassLoose(ijet) ) Nloose_bjets++;
      }

    if ( NgoodJets >= 3) counter["3jets"]++;
    else continue;

    if ( Nloose_bjets >= 1) counter["1btag"]++;
    else continue;

  } // End loop over entries -------------------------------------------------------------
  if (fverbose) cout << "== end loop over entries.\n";

  // Print cut flow --------------------------------------------------------------------
  cout << "== Cut flow:\n";
  //for ( map<string, float>::iterator it = counter.begin(); it != counter.end(); it++ )
  for ( vector<string>::iterator ii= counterlabels.begin(); ii != counterlabels.end(); ++ii)
    {
      cout << format("%-25s %12.4f\n") % *ii % counter[*ii];
    }

  // Write histograms
  mu1Histos.Write( fFile );
  pvHistos.Write( fFile );

  cout << "== End at " << to_simple_string( now ) << endl;
  return 0;

}
