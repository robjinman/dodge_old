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
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const Vec3i& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const XmlNode data) {
   if (data.isNull() || data.name() != "Vec3f")
      throw XmlException("Error parsing XML for instance of class Vec3f; Expected 'Vec3f' tag", __FILE__, __LINE__);

   XmlAttribute attr = data.firstAttribute();

   if (attr.isNull() || attr.name() != "x")
      throw XmlException("Error parsing XML for instance of class Vec3f; Expected 'x' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &x);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "y")
      throw XmlException("Error parsing XML for instance of class Vec3f; Expected 'y' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &y);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "z")
      throw XmlException("Error parsing XML for instance of class Vec3f; Expected 'z' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &z);
}


}
