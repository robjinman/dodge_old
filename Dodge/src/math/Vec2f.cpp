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
// Vec2f::assignData
//===========================================
void Vec2f::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Vec2f") != 0)
      throw Exception("Error parsing XML for instance of class Vec2f", __FILE__, __LINE__);

   xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "x") == 0) {
      sscanf(attr->value(), "%f", &x);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "y") == 0) {
      sscanf(attr->value(), "%f", &y);
      attr = attr->next_attribute();
   }
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
