/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __VEC2F_HPP__
#define __VEC2F_HPP__


#include <math.h>
#include "../definitions.hpp"
#include "../rapidxml/rapidxml.hpp"


namespace Dodge {


class Vec2i;

class Vec2f {
   public:
      float32_t x, y;

      Vec2f() {}
      Vec2f(float32_t x_, float32_t y_) : x(x_), y(y_) {}
      Vec2f(const Vec2f& v) : x(v.x), y(v.y) {}
      Vec2f(const Vec2i& v);

      Vec2f operator+(const Vec2f& v) const { return Vec2f(x + v.x, y + v.y); }
      Vec2f operator-(const Vec2f& v) const { return Vec2f(x - v.x, y - v.y); }
      Vec2f operator*(float32_t s) const { return Vec2f(x * s, y * s); }
      Vec2f operator/(float32_t s) const { return Vec2f(x / s, y / s); }

      bool operator==(const Vec2f& v) const { return x == v.x && y == v.y; }
      bool operator!=(const Vec2f& v) const { return !(*this == v); }

      Vec2f& operator=(const Vec2f& rhs) { x = rhs.x; y = rhs.y; return *this; }

      void rotate(const Vec2f& p, float32_t a);

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
