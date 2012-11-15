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
// Vec3i::Vec3i
//===========================================
Vec3i::Vec3i(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3i::Vec3i
//===========================================
Vec3i::Vec3i(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Vec3i);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, x);
      x = attr.getInt();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, y);
      y = attr.getInt();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, z);
      z = attr.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Vec3i; ");
      throw;
   }
}


}
