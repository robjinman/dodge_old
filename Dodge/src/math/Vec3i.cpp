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
   string msg("Error parsing XML for instance of class Vec3i");

   XML_NODE_CHECK(msg, data, Vec3i);

   XmlAttribute attr = data.firstAttribute();
   XML_ATTR_CHECK(msg, attr, x);
   sscanf(attr.value().data(), "%d", &x);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, y);
   sscanf(attr.value().data(), "%d", &y);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, z);
   sscanf(attr.value().data(), "%d", &z);
}


}
