/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __VEC3F_HPP__
#define __VEC3F_HPP__


#include <math.h>
#include "../definitions.hpp"
#include "../rapidxml/rapidxml.hpp"


namespace Dodge {


class Vec3i;

class Vec3f {
   public:
      float32_t x, y, z;

      Vec3f() {}
      Vec3f(float32_t x_, float32_t y_, float32_t z_) : x(x_), y(y_), z(z_) {}
      Vec3f(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}
      Vec3f(const Vec3i& v);

      Vec3f operator+(const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
      Vec3f operator-(const Vec3f& v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
      Vec3f operator*(float32_t s) const { return Vec3f(x * s, y * s, z * s); }
      Vec3f operator/(float32_t s) const { return Vec3f(x / s, y / s, z / s); }

      bool operator==(const Vec3f& v) const { return x == v.x && y == v.y && z == v.z; }
      bool operator!=(const Vec3f& v) const { return !(*this == v); }

      Vec3f& operator=(const Vec3f& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
