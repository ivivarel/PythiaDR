SHELL = /bin/sh
PYTHIA8LOCATION = $(shell echo $(PYTHIA8__HOME))
LIBDIR=lib
HEPMCLOCATION = $(shell echo $(HEPMC__HOME))
ROOTCFLAGS      := $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTGLIBS       := $(shell $(ROOTSYS)/bin/root-config --glibs)
# flags:
#
#FFLAGSSHARED = -fPIC
CFLAGSSHARED = -fPIC
CXXFLAGSSHARED = -fPIC

LIBSFASTJET += $(shell $(FASTJET_CONFIG) --libs --plugins ) -lstdc++
FJCXXFLAGS+= $(shell $(FASTJET_CONFIG) --cxxflags)

CPPFLAGS= $(ROOTCFLAGS) -I$(PYTHIA8LOCATION)/include  -I$(HEPMCLOCATION)/include
LIBS=   $(ROOTGLIBS)  -lEG  -L$(PYTHIA8LOCATION)/$(LIBDIR) -lpythia8  -L$(HEPMCLOCATION)/lib -lHepMC 

all: runpythia

readlhe: readlhe.o 
	$(CXX) -o readlhe readlhe.o  $(LIBS)

runpythia: runpythia.o 
	$(CXX) -o runpythia runpythia.o  $(LIBS)

readlhe.o:  readlhe.cc 
	$(CXX) -c $(CPPFLAGS) -o readlhe.o readlhe.cc

runpythia.o:  runpythia.cc 
	$(CXX) -c $(CPPFLAGS) -o runpythia.o runpythia.cc

