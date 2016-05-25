#ifndef NODE_H_
#define NODE_H_

#include "vector3D.hpp"

class Node {
public:
  int latticeId;
  int nodeId;
  Vector3D position;

  Node();
  Node(int li, int ni, Vector3D &position);
  Node(const Node &n);
  ~Node();
};

#endif
