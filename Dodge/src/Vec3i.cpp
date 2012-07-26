/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <Vec3f.hpp>
#include <Vec3i.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Vec3i::Vec3i
//===========================================
Vec3i::Vec3i(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3i::assignData
//===========================================
void Vec3i::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Vec3i") != 0)
      throw Exception("Error parsing XML for instance of class Vec3i", __FILE__, __LINE__);

   xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "x") == 0) {
      sscanf(attr->value(), "%d", &x);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "y") == 0) {
      sscanf(attr->value(), "%d", &y);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "z") == 0) {
      sscanf(attr->value(), "%d", &z);
      attr = attr->next_attribute();
   }
}


}
