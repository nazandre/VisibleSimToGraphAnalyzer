#ifndef LATTICE_H_
#define LATTICE_H_

#include <string>
#include <vector>
#include <list>

#include "node.hpp"
#include "vector3D.hpp"

typedef enum LatticeType_t
{
  UNKNOWN_LATTICE = -1,
  SQUARE_LATTICE = 0,
  HEXAGONAL_LATTICE = 1,
  SIMPLE_CUBIC_LATTICE = 2,
  FACE_CENTERED_CUBIC_LATTICE = 3,
  NUM_LATTICE_TYPES = 4
} LatticeType;

class Lattice {

public:
  std::string visibleSimXML;

  LatticeType type;
  Vector3D size; // grid size (3D)
  Node ** grid;
  std::vector<Node*> nodes;
  

 
  Lattice();
  Lattice(std::string vs, LatticeType t);
  Lattice(const Lattice &l);
  ~Lattice();

  static std::string typeShortName[];
  static std::string typeFullName[];
  static LatticeType getType(std::string t);

  bool isIn(Vector3D &p); 
  int getIndex(Vector3D &p);
  void insert(Node* n,Vector3D &p);
  Node* getNode(Vector3D &p);

  void readXML();
  void exportToAJ(std::ostream &output);

  std::list<Node*> getNeighbors(Node *n);

  virtual std::list<Vector3D> getRelativeConnectivity(Vector3D &p) = 0;
  
};

// Hexgaonal (H) Lattice
class HLattice : public Lattice {

public:
  HLattice();
  HLattice(std::string vs);
  HLattice(const HLattice &l);
  ~HLattice();
  
  std::list<Vector3D> getRelativeConnectivity(Vector3D &p);
};

// Simple Cubic (SC) Lattice (okay for Square (S) Lattice as well)
class SCLattice : public Lattice {

public:
  std::list<Vector3D> nCells;
  
  SCLattice();
  SCLattice(std::string vs);
  SCLattice(const SCLattice &l);
  ~SCLattice();
  
  std::list<Vector3D> getRelativeConnectivity(Vector3D &p);
};

// Face-Centered Cubic (FCC) Lattice
class FCCLattice : public Lattice {

public:
  
  FCCLattice();
  FCCLattice(std::string vs);
  FCCLattice(const FCCLattice &l);
  ~FCCLattice();
  
  std::list<Vector3D> getRelativeConnectivity(Vector3D &p);
};


#endif
