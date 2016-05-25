#include <iostream>
#include <cstdlib>
#include "arguments.hpp"
#include "lattice.hpp"

using namespace std;

Arguments::Arguments() {}

Arguments::Arguments(int argc, char *argv[]) {

  argv++;
  argc--;

  while ((argc > 0) && (argv[0][0] == '-')) {
    switch(argv[0][1]) {
    case 'i':
      input = argv[1];
      argc--;
      argv++;
      break;
    case 'o':
      output = argv[1];
      argc--;
      argv++;
      break;
    case 't':
      target = argv[1];
      argc--;
      argv++;
      break;
    case 'h':
      help();
      break;
    default:
      cerr << "ERROR: Unknown argument -" <<  argv[0][1] << endl;
      help();
    }
    argc--;
    argv++;
  }

  if (input == "") {
    cerr << "ERROR: No input file" << endl;
    help();
  }

  size_t lastDot = input.find_last_of(".");
  string ext = input.substr(lastDot + 1);
  if (ext != "xml") {
    help();
  }

  if(target == "") {
    cerr << "ERROR: No target lattice specified" << endl;
    help();
  }

  LatticeType t = Lattice::getType(target);

  if (t == UNKNOWN_LATTICE) {
    cerr << "ERROR: Unknown target lattice " << target << endl;
    help();
  }
  
  if (output == "") {
    size_t lastSlash = input.find_last_of("/\\");
    string n =  input.substr(0, lastDot).substr(lastSlash+1);
    output = n + ".aj";
  }
}

Arguments::Arguments(const Arguments &a) {
  input = a.input;
  output = a.output;
  target = a.target;
}

Arguments::~Arguments() { }

void Arguments::help() {
  //cerr << "VisibleSim Configuration File To Graph Analyzer File:" << endl;
  cerr << "Usage: VisibleSimToGraphAnalyzer -i <input xml image> -t <target lattice> [options]"<< endl;
  cerr << "Supported target lattices (<target lattice>): " << endl;
  for (int i = 0; i < NUM_LATTICE_TYPES; i++) {
    cerr << "  " << Lattice::typeShortName[i] << ": " << Lattice::typeFullName[i] << endl;
  }
  cerr <<"Options:" << endl;
  cerr <<"-o <output aj file> : output file for GraphAnalyzer" << endl;
  cerr <<"-h                  : print this usage and exit" << endl;

  exit(EXIT_SUCCESS);
} 
