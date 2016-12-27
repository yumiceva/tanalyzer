CC = g++
CFLAGS = `root-config --auxcflags`
INC = -I`root-config --incdir`
LDFLAGS = `root-config --libs`
# setup boost libraries
# check version using scram tool info boost
INC += -I/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/boost/1.57.0-jlbgio/include
LDFLAGS += -L$(CMSSW_RELEASE_BASE)/external/slc6_amd64_gcc491/lib/ -lboost_program_options -lboost_date_time -lboost_iostreams

SOURCES = EventTree.C FileLoader.cc MuonHistos.cc JetHistos.cc
#OBJECTS = $(SOURCES:.C=.o)
#OBJECTS += $(SOURCES:.cc=.o)
OBJECTS = EventTree.o FileLoader.o MuonHistos.o JetHistos.o
#EXECUTABLE = tanalyzer


all: $(SOURCES) tanalyzer

print-%  : ; @echo $* = $($*)

tanalyzer: $(OBJECTS)
	$(CC) -o tanalyzer $(LDFLAGS) $(INC) $(CFLAGS) $(OBJECTS) tanalyzer.cc

#$(EXCUTABLE): $(OBJECTS)
#	$(CC) -o $(EXECUTABLE) $(LDFLAGS) $(INC) $(CFLAGS) $(OBJECTS)

.C.o:
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

.cc.o:
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

cleanall: clean
	rm -f tanalyzer
