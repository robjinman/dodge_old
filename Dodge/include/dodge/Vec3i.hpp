/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __VEC3I_HPP__
#define __VEC3I_HPP__


#include <math.h>
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class Vec3f;

class Vec3i {
   public:
      int x, y, z;

      Vec3i() {}
      Vec3i(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
      Vec3i(const Vec3i& v) : x(v.x), y(v.y), z(v.z) {}
      Vec3i(const Vec3f& v);

      Vec3i operator+(const Vec3i& v) const { return Vec3i(x + v.x, y + v.y, z + v.z); }
      Vec3i operator-(const Vec3i& v) const { return Vec3i(x - v.x, y - v.y, z - v.z); }
      Vec3i operator*(int s) const { return Vec3i(x * s, y * s, z * s); }
      Vec3i operator/(int s) const { return Vec3i(x / s, y / s, z / s); }

      bool operator==(const Vec3i& v) const { return x == v.x && y == v.y && z == v.z; }
      bool operator!=(const Vec3i& v) const { return !(*this == v); }

      Vec3i& operator=(const Vec3i& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
