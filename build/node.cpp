#include "node.hpp"

Node::Node() {}

Node::Node(int li, int ni, Vector3D &p) {
  latticeId = li;
  nodeId = ni;
  position = p;
}

Node::Node(const Node &n) {
  latticeId = n.latticeId;
  nodeId = n.nodeId;
  position = n.position;
}

Node::~Node() {}
