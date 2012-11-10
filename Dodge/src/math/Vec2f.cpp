/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <math/Vec2f.hpp>
#include <math/Vec2i.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Vec2f::Vec2f
//===========================================
Vec2f::Vec2f(const Vec2i& v) : x(v.x), y(v.y) {}

//===========================================
// Vec2f::Vec2f
//===========================================
Vec2f::Vec2f(const XmlNode data) {
   if (data.isNull() || data.name() != "Vec2f")
      throw XmlException("Error parsing XML for instance of class Vec2f; Expected 'Vec2f' tag", __FILE__, __LINE__);

   XmlAttribute attr = data.firstAttribute();

   if (attr.isNull() || attr.name() != "x")
      throw XmlException("Error parsing XML for instance of class Vec2f; Expected 'x' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &x);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "y")
      throw XmlException("Error parsing XML for instance of class Vec2f; Expected 'y' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &y);
}

//===========================================
// Vec2f::rotate
//===========================================
void Vec2f::rotate(const Vec2f& p, float32_t a) {
   float32_t x_ = x;
   x = (x - p.x) * cos(DEG_TO_RAD(a)) - (y - p.y) * sin(DEG_TO_RAD(a)) + p.x;
   y = (x_ - p.x) * sin(DEG_TO_RAD(a)) + (y - p.y) * cos(DEG_TO_RAD(a)) + p.y;
}


}
