/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <math/Vec3f.hpp>
#include <math/Vec3i.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const Vec3i& v)
   : x(static_cast<float32_t>(v.x)), y(static_cast<float32_t>(v.y)), z(static_cast<float32_t>(v.z)) {}

//===========================================
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Vec3f);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, x);
      x = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, y);
      y = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, z);
      z = attr.getFloat();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Vec3f; ");
      throw;
   }
}


}
