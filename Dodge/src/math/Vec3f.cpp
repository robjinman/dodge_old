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
Vec3f::Vec3f(const Vec3i& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const XmlNode data) {
   string msg("Error parsing XML for instance of class Vec3f");

   XML_NODE_CHECK(msg, data, Vec3f);

   XmlAttribute attr = data.firstAttribute();
   XML_ATTR_CHECK(msg, attr, x);
   sscanf(attr.value().data(), "%f", &x);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, y);
   sscanf(attr.value().data(), "%f", &y);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, z);
   sscanf(attr.value().data(), "%f", &z);
}


}
