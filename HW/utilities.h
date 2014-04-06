#include <cmath>
#include <cassert>

// epsilon values used in comparing the edge lengths & angles between
// edges note that these values are dependent on the precision of
// the coordinates and the overall scale of the objects
#define DISTANCE_EPSILON 0.0001
#define ANGLE_EPSILON 0.1

// -----------------------------------------------------------------

// Stores a 2D coordinate
class Point {
public:
  Point(double _x, double _y) : x(_x),y(_y) {}
  double x;
  double y;
};

// Stores a 2D vector, constructed from 2 Points
class Vector {
public:
  Vector(const Point &a, const Point &b) { dx = b.x-a.x; dy = b.y-a.y; }
  double Length() { return sqrt(dx*dx+dy*dy); }
  void Normalize() {
    // make this a unit vector (length = 1)
    double length = Length();
    assert (length > DISTANCE_EPSILON);
    dx /= length;
    dy /= length;
  }
  // representation
  double dx;
  double dy;
};

// -----------------------------------------------------------------

// calculate the length of an edge, the distance between 2 points
inline double DistanceBetween(const Point &a, const Point &b) { 
  Vector v(a,b);
  return v.Length();
}

// calculate the dot product between two vectors
inline double DotProduct(const Vector &a, const Vector &b) { 
  return a.dx*b.dx + a.dy*b.dy;
}

// calculate the angle at vertex b in degrees
// (that is, the angle between edges ab and bc)
inline double Angle(const Point &a, const Point &b, const Point &c) {
  // make unit vector along each of the edges
  Vector ba(b,a);  ba.Normalize();
  Vector bc(b,c);  bc.Normalize();
  // calculate the angle in radians
  double dot_product = DotProduct(ba,bc);
  assert (dot_product >= -1 && dot_product <= 1);
  double angle_in_radians = acosf(dot_product);
  assert (angle_in_radians >= 0 && angle_in_radians <= M_PI);
  // convert to degrees
  return angle_in_radians * 180 / M_PI;
}

// returns true if these two vectors are parallel
inline bool Parallel(const Vector &a, const Vector &b) {
  Vector a2 = a; a2.Normalize();
  Vector b2 = b; b2.Normalize();
  double dot_product = DotProduct(a2,b2);
  if (fabs(dot_product) > 1-DISTANCE_EPSILON) return true;
  return false;
}

// -----------------------------------------------------------------
// simple comparison functions for angles & sides

inline bool EqualSides(double a, double b) {
  return (fabs(a-b) < DISTANCE_EPSILON);
}

inline bool EqualAngles(double a, double b) {
  return (fabs(a-b) < ANGLE_EPSILON);
}

inline bool RightAngle(double a) {
  return (fabs(a-90) < ANGLE_EPSILON);
}

// -----------------------------------------------------------------



