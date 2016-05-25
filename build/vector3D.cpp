#include "vector3D.hpp"

Vector3D::Vector3D() {};

Vector3D::Vector3D(int _x, int _y, int _z) {
  x = _x;
  y = _y;
  z = _z;
}

Vector3D::Vector3D(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3D::~Vector3D() {}

Vector3D operator+(const Vector3D  &v1, const Vector3D &v2) {
  return Vector3D(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}
