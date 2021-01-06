// main11.cc is a part of the PYTHIA event generator.
// Copyright (C) 2015 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses the ttsample.lhe(.gz) input file, the latter only with 100 events.

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include <string.h>

using namespace Pythia8;
using namespace std;
int main(int argc, char* argv[]) {
  // Check that correct number of command-line arguments
  if (argc != 3) {
    cerr << " Unexpected number of command-line arguments. \n"
         << " You are expected to provide a file name and nothing else. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Check that the provided file name corresponds to an existing file.
  ifstream is(argv[1]);
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }
  // Confirm that external file will be used for settings..
  cout << " Command line file  " << argv[1] << endl;

  // Confirm that external file will be used for settings..
  cout << " Output will be written on  " << argv[2] << endl;

  string outfile=argv[2];

  // Interface for conversion from Pythia8::Event to HepMC event. 
  // Interface for conversion from Pythia8::Event to HepMC event. 
  HepMC::Pythia8ToHepMC ToHepMC;
  HepMC::IO_GenEvent ascii_io(outfile, std::ios::out);
  //  Number of listed events. Allow for possibility of a few faulty events.
  int nPrintLHA  = 1;
  int nPrintRest = 0;
  int nAbort     = 10;


  // You can always read an plain LHE file,
  // but you ran "./configure --with-gzip" before "make"
  // then you can also read a gzipped LHE file.
#ifdef GZIPSUPPORT
  bool useGzip = true;
#else
  bool useGzip = false;
#endif

  // Generator. We here stick with default values, but changes
  // could be inserted with readString or readFile.
  Pythia pythia;

  // Load configuration file
  pythia.readFile(argv[1]);
  // Read in main settings
  int nEvent      = pythia.settings.mode("Main:numberOfEvents");
  int nSkip       = pythia.settings.mode("Main:spareMode1");
  int nError      = pythia.settings.mode("Main:timesAllowErrors");

  pythia.init();

  // List changes.
  pythia.settings.listChanged();
  pythia.particleData.listChanged();

  // Set counters.
  int iPrintLHA  = 0;
  int iPrintRest = 0;
  int iAbort     = 0;
  int iFile      = 1;

  // Begin event loop; generate until none left in input file.
  for (int iEvent = 0; ; ++iEvent) {

    // Generate events, and check whether generation failed.
    if (!pythia.next()) {

      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) break;

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      break;
    }
   // Construct new empty HepMC event and fill it.
   // Units will be as chosen for HepMC build; but can be changed
   // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)  
   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
   ToHepMC.fill_next_event( pythia, hepmcevt );
   // do something with HepMC event
   ascii_io << hepmcevt;
   // delete it
   delete hepmcevt;
    // List first few Les Houches and other events.
    if (pythia.info.isLHA() && iPrintLHA < nPrintLHA) {
      pythia.LHAeventList();
      pythia.info.list();
      pythia.process.list();
      pythia.event.list();
      ++iPrintLHA;
    } else if (!pythia.info.isLHA() && iPrintRest < nPrintRest) {
      pythia.info.list();
      pythia.process.list();
      pythia.event.list();
      ++iPrintRest;
    }
    if (nEvent != 0 && iEvent == nEvent) break;
    // End of event loop.
  }
  // Give statistics. Print histogram.
  pythia.stat();
  // Done.
  return 0;
}
