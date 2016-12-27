# tanalyzer
A simple ntuple analyzer.

## Installation

1. Install a CMSSW release. The latest version of tanalyzer has been tested with CMSSW_7_4_5:

cmsrel CMSSW_7_4_5; cd CMSSW_7_4_5/src/; cmsenv

2. Get a copy of the package:

git clone git@github.com:yumiceva/tanalyzer.git 

## Build executable

cd tanalyzer
make

## Run analyzer

Display the command line options:

./tanalyzer -h

For example,

./tanalyzer -s ttjets -o ./ -i /eos/uscms/store/user/troy2012/skims_13TeV/ttjets.root -m 10 -v


