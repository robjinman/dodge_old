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
// Vec3f::Vec3f
//===========================================
Vec3f::Vec3f(const Vec3i& v) : x(v.x), y(v.y), z(v.z) {}

//===========================================
// Vec3f::assignData
//===========================================
void Vec3f::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Vec3f") != 0)
      throw Exception("Error parsing XML for instance of class Vec3f", __FILE__, __LINE__);

   xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "x") == 0) {
      sscanf(attr->value(), "%f", &x);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "y") == 0) {
      sscanf(attr->value(), "%f", &y);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "z") == 0) {
      sscanf(attr->value(), "%f", &z);
      attr = attr->next_attribute();
   }
}


}
