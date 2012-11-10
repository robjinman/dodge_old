/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <math/Vec3f.hpp>
#include <math/Vec3i.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Vec3i::Vec3i
//===========================================
Vec3i::Vec3i(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3i::Vec3i
//===========================================
Vec3i::Vec3i(const XmlNode data) {
   if (data.isNull() || data.name() != "Vec3i")
      throw XmlException("Error parsing XML for instance of class Vec3i; Expected 'Vec3i' tag", __FILE__, __LINE__);

   XmlAttribute attr = data.firstAttribute();

   if (attr.isNull() || attr.name() != "x")
      throw XmlException("Error parsing XML for instance of class Vec3i; Expected 'x' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%d", &x);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "y")
      throw XmlException("Error parsing XML for instance of class Vec3i; Expected 'y' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%d", &y);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "z")
      throw XmlException("Error parsing XML for instance of class Vec3i; Expected 'z' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%d", &z);
}


}
