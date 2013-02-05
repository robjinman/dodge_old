/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <math/Vec2f.hpp>
#include <math/Vec2i.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Vec2i::Vec2i
//===========================================
Vec2i::Vec2i(const Vec2f& v)
   : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)) {}

//===========================================
// Vec2i::Vec2i
//===========================================
Vec2i::Vec2i(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Vec2i);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, x);
      x = attr.getInt();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, y);
      y = attr.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Vec2i; ");
      throw;
   }
}

//===========================================
// Vec2i::rotate
//===========================================
void Vec2i::rotate(const Vec2i& p, float32_t a) {
   float32_t x_ = static_cast<float32_t>(x);

   x = static_cast<int>(static_cast<float32_t>(x - p.x) * cos(DEG_TO_RAD(a))
      - static_cast<float32_t>(y - p.y) * sin(DEG_TO_RAD(a)) + static_cast<float32_t>(p.x));

   y = static_cast<int>(static_cast<float32_t>(x_ - p.x) * sin(DEG_TO_RAD(a))
      + static_cast<float32_t>(y - p.y) * cos(DEG_TO_RAD(a)) + static_cast<float32_t>(p.y));
}


}
