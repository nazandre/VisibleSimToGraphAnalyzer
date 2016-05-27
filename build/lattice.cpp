#include <iostream>
#include <cstdlib>
#include "lattice.hpp"
#include "tinyxml.h"

using namespace std;

#define EVEN(x) (x%2 == 1)
#define ODD(x) (x%2 == 0)

#define IS_3D (type == SIMPLE_CUBIC_LATTICE || type == FACE_CENTERED_CUBIC_LATTICE)
#define IS_2D (type == SQUARE_LATTICE || type == HEXAGONAL_LATTICE)

/****** Lattice Class ******/

std::string Lattice::typeShortName[] = {"s","h","sc","fcc"};
std::string Lattice::typeFullName[] = {"Square","Hexagonal","Simple-Cubic","Face-Centered Cubic"};

Lattice::Lattice() {
  grid = NULL;
}

Lattice::Lattice(string vs, LatticeType t) {
  visibleSimXML = vs;
  grid = NULL;
  type = t;
  readXML();
}

Lattice::Lattice(const Lattice &l) {
  visibleSimXML = l.visibleSimXML;
  type = l.type;
  grid = NULL;
  readXML();
}

Lattice::~Lattice() {
  for (vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
    Node *n = *it;
    delete n;
  }
  delete [] grid;
}

LatticeType Lattice::getType(std::string t) {
  for(int i = 0; i < NUM_LATTICE_TYPES; i++) {
    if(t == typeShortName[i]) {
      return LatticeType(i);
    }
  }
  return UNKNOWN_LATTICE;
}

int Lattice::getIndex(Vector3D &p) {
  // VisibleSim code: 
  // inline BlinkyBlocksBlock* getGridPtr(int ix,int iy,int iz) { return gridPtrBlocks[ix+(iy+iz*gridSize[1])*gridSize[0]]; };
  // gridSize[0]=slx;
  // gridSize[1]=sly;
  // gridSize[2]=slz;
  
  int index = p.x + (p.z*size.y + p.y)*size.x;
  return index;
}

void Lattice::insert(Node* n,Vector3D &p) {
  grid[getIndex(p)] = n;
}

Node* Lattice::getNode(Vector3D &p) {
  return grid[getIndex(p)];
}

void Lattice::readXML() {
  TiXmlDocument xmlDoc(visibleSimXML);
  TiXmlNode *node = NULL;
  int currentId = 1;

  // open xml file
  if (!xmlDoc.LoadFile()) {
    cerr << "Could not load " << visibleSimXML << endl;
    exit(EXIT_FAILURE);
  }

  // read grid size
  node = xmlDoc.FirstChild("world");
  if (node) {
    TiXmlElement* worldElement = node->ToElement();
    const char *attr= worldElement->Attribute("gridsize");
    if (!attr) {
      attr= worldElement->Attribute("gridSize");
    }
    int lx = 0,ly = 0,lz = 0;
    if (attr) {
      string str=attr;
      if (IS_3D) {
	int pos1 = str.find_first_of(',');
	int pos2 = str.find_last_of(',');
	lx = atoi(str.substr(0,pos1).c_str());
	ly = atoi(str.substr(pos1+1,pos2-pos1-1).c_str());
	lz = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
      } else {
	int pos = str.find_first_of(',');
	lx = atoi(str.substr(0,pos).c_str());
	ly = 1;
	lz = atoi(str.substr(pos+1,str.length()-pos-1).c_str());
      }
      size = Vector3D(lx,ly,lz);
      cerr << "Grid size: "
	   << "x=" << size.x << ","
	   << "y=" << size.y << ","
	   << "z=" << size.z << ")"
	   << endl;  
    } else {
      cerr << "ERROR: Grid size not specified! " << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cerr << "ERROR: No world in XML file" << endl;
    exit(EXIT_FAILURE);
  }
  
  // Grid initialization
  grid = new Node*[size.x*size.y*size.z]{NULL};

  // read modules and import them into the grid
  TiXmlNode *nodeBlock = node->FirstChild("blockList");
  if (nodeBlock) {
    nodeBlock = nodeBlock->FirstChild("block");
    while (nodeBlock) {
      Vector3D position;
      int id = 0;
      TiXmlElement* element = nodeBlock->ToElement();
      const char *attr = element->Attribute("position");
      if (attr) {
	string str(attr);
	if (IS_3D) {
	  int pos1 = str.find_first_of(',');
	  int pos2 = str.find_last_of(',');
	  position.x = atoi(str.substr(0,pos1).c_str());
	  position.y = atoi(str.substr(pos1+1,pos2-pos1-1).c_str());
	  position.z = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
	} else {
	  if (type == HEXAGONAL_LATTICE) {
	    int pos = str.find_first_of(',');
	    position.x = atof(str.substr(0,pos).c_str());
	    position.y = 0;
	    position.z = atoi(str.substr(pos+1,str.length()-pos-1).c_str());
	  } else if (type == SQUARE_LATTICE) {
	    int pos = str.find_first_of(',');
	    position.x = atof(str.substr(0,pos).c_str());
	    position.y = atoi(str.substr(pos+1,str.length()-pos-1).c_str());
	    position.z = 0;
	  }
	}
      } else {
	// line number ?
	cerr << "ERROR: No position specified for a module!" << endl;
	exit(EXIT_FAILURE);
      }

      id = currentId;
      currentId++;
      
      // add block into the grid
      Node *n = new Node(id,id,position);
      nodes.push_back(n);
      insert(n,position);
      
      nodeBlock = nodeBlock->NextSibling("block");
    }
  } else {
    cerr << "WARNING: No module in " << visibleSimXML << endl;
  }
}

void Lattice::exportToAJ(ostream &output) {
  // Format:
  // [# of nodes]
  // [nodeId] [# of neighbors with > latticeId] [neighbors latticeId ... ]
  // [nodeId] [# of neighbors with > latticeId] [neighbors latticeId ... ]
  // ... (n times)
  
  output << nodes.size() << endl;
  for (vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
    Node *node = *it;
    list<Node*> neighbors = getNeighbors(node);
    int numNeighbors = 0; // count neighbors with biggest id (to avoid printing  multiple times the same edge)
    output << node->nodeId;

    for (list<Node*>::iterator it=neighbors.begin();
       it != neighbors.end(); ++it) {
      Node *neighbor = *it;
      if (neighbor->latticeId > node->latticeId) {
	numNeighbors++;
      }
    }
    output << " " << numNeighbors;

    for (list<Node*>::iterator it=neighbors.begin();
       it != neighbors.end(); ++it) {
      Node *neighbor = *it;
      if (neighbor->latticeId > node->latticeId) {
	output << " " << neighbor->latticeId;
      }
    }
    output << endl;
  }
}

bool Lattice::isIn(Vector3D &p) {
  return (p.x >= 0 && p.x < size.x &&
	  p.y >= 0 && p.y < size.y &&
	  p.z >= 0 && p.z < size.z);
}


list<Node*> Lattice::getNeighbors(Node *n) {
  list<Node*> l;
  list<Vector3D> nCells = getRelativeConnectivity(n->position); // depends on p and lattice type
  for (list<Vector3D>::iterator it=nCells.begin();
       it != nCells.end(); ++it) {
    Vector3D v = n->position + *it;
    if (isIn(v)) {
      Node* node = getNode(v);
      if (node != NULL) {
	l.push_back(node);
      }
    }
  }
  return l;
}

/****** Hexgonal Lattice Class ******/

HLattice::HLattice() {}

HLattice::HLattice(std::string vs) : Lattice(vs, HEXAGONAL_LATTICE) {}

HLattice::HLattice(const HLattice &l) : Lattice(l) {}

HLattice::~HLattice() {}

list<Vector3D> HLattice::getRelativeConnectivity(Vector3D &p) {
  list<Vector3D> nCells;

  nCells.push_back(Vector3D(1,0,0));
  nCells.push_back(Vector3D(-1,0,0));

  nCells.push_back(Vector3D(0,0,1));
  nCells.push_back(Vector3D(0,0,-1));
  
  if (EVEN(p.z)) {
    nCells.push_back(Vector3D(1,0,1));
    nCells.push_back(Vector3D(1,0,-1));
  } else {
    nCells.push_back(Vector3D(-1,0,1));
    nCells.push_back(Vector3D(-1,0,-1));
  }

  return nCells;
}

/****** Simple Cubic Lattice Class ******/

SCLattice::SCLattice() {}

SCLattice::SCLattice(std::string vs) : Lattice(vs, SIMPLE_CUBIC_LATTICE) {
  nCells.push_back(Vector3D(1,0,0));
  nCells.push_back(Vector3D(-1,0,0));
  nCells.push_back(Vector3D(0,0,1));
  nCells.push_back(Vector3D(0,0,-1));
  nCells.push_back(Vector3D(0,1,0));
  nCells.push_back(Vector3D(0,-1,0));
}

SCLattice::SCLattice(const SCLattice &l) : Lattice(l) {
  nCells = l.nCells;
}

SCLattice::~SCLattice() {
}

list<Vector3D> SCLattice::getRelativeConnectivity(Vector3D &p) {
  return nCells;
}

/****** Face-Centered Cubic Lattice Class ******/

FCCLattice::FCCLattice() {}

FCCLattice::FCCLattice(std::string vs) : Lattice(vs, FACE_CENTERED_CUBIC_LATTICE) {}

FCCLattice::FCCLattice(const FCCLattice &l) : Lattice(l) {}

FCCLattice::~FCCLattice() {}

list<Vector3D> FCCLattice::getRelativeConnectivity(Vector3D &p) {
  list<Vector3D> l;
  list<Vector3D> nCells;
  
  if (ODD(p.z)) {
    // z + 1
    nCells.push_back(Vector3D(-1,-1,1));
    nCells.push_back(Vector3D(-1,0,1));
    nCells.push_back(Vector3D(0,0,1));
    nCells.push_back(Vector3D(0,-1,1));
    // z
    nCells.push_back(Vector3D(-1,0,0));
    nCells.push_back(Vector3D(1,0,0));
    nCells.push_back(Vector3D(0,-1,0));
    nCells.push_back(Vector3D(0,1,0));
    // z - 1
    nCells.push_back(Vector3D(-1,-1,-1));
    nCells.push_back(Vector3D(-1,0,-1));
    nCells.push_back(Vector3D(0,0,-1));
    nCells.push_back(Vector3D(0,-1,-1));
  } else {
    // z + 1
    nCells.push_back(Vector3D(0,0,1));
    nCells.push_back(Vector3D(0,1,1));
    nCells.push_back(Vector3D(1,0,1));
    nCells.push_back(Vector3D(1,1,1));
    // z
    nCells.push_back(Vector3D(-1,0,0));
    nCells.push_back(Vector3D(1,0,0));
    nCells.push_back(Vector3D(0,-1,0));
    nCells.push_back(Vector3D(0,1,0));
    // z - 1
    nCells.push_back(Vector3D(0,0,-1));
    nCells.push_back(Vector3D(0,1,-1));
    nCells.push_back(Vector3D(1,0,-1));
    nCells.push_back(Vector3D(1,1,-1));
  }
  return nCells;
}
