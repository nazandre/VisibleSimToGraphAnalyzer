#include <cstdlib>
#include <iostream>
#include <fstream>

#include "arguments.hpp"
#include "lattice.hpp"

using namespace std;

int main(int argc, char *argv[]) {

  Arguments arguments(argc,argv);
  cerr << "Input: " << arguments.input << endl;
  cerr << "Output: " << arguments.output << endl;
  cerr << "Target lattice: " << arguments.target << endl; 

  // open and read VisibleSim configuration file
  Lattice *lattice;
						   
  switch(Lattice::getType(arguments.target)) {
    case(SQUARE_LATTICE) :
      lattice = new SCLattice(arguments.input);
    break;
    case(HEXAGONAL_LATTICE) :
      lattice = new HLattice(arguments.input);
    break;
    case(SIMPLE_CUBIC_LATTICE) :
      lattice = new SCLattice(arguments.input);
    break;
    case(FACE_CENTERED_CUBIC_LATTICE) :
      lattice = new FCCLattice(arguments.input);
    break;
    default:
      cerr << "ERROR: Unknown lattice type." << endl;
  }
						   
  cerr << arguments.input << " opened and read!" << endl;

  // Export to Graph Analyzer file
  ofstream output;
  output.open (arguments.output);
  lattice->exportToAJ(output);

  cerr << arguments.input << " exported in " << arguments.output << endl;
  return EXIT_SUCCESS;
}
